//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                AstroGraphicsView
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, CFitsIO.
// NAMESPACE:						AstroManager::imaging
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2020 Gavin Blakeman.
//                      This file is part of the Astronomy Manager software (astroManager)
//
//                      astroManager is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      astroManager is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
//                      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//                      License for more details.
//
//                      You should have received a copy of the GNU General Public License along with astroManager.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
//
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2010-10-30 GGB - File created.
//
//*********************************************************************************************************************************

#include "include/AstroGraphicsView.h"

  // Standard C++ header files

#include <cmath>

  // astroManager header files.

#include "include/dockWidgets/dockWidgetImageInformation.h"
#include "include/dockWidgets/dockWidgetMagnify.h"
#include "include/dockWidgets/dockWidgetNavigator.h"
#include "include/FrameWindow.h"

namespace astroManager
{
  namespace imaging
  {

    int const MARGIN_X = 10;
    int const MARGIN_Y = 10;
    double const ZOOM_FACTOR = 1.1;

    //******************************************************************************************************************************
    //
    // CAstroGraphicsView
    //
    //******************************************************************************************************************************

    /// @brief Constructor for the CAstrographicsView class.
    /// @param[in] parent: The owner of this instance.
    /// @version 2013-06-19/GGB - Added different definitions for windows and other.
    /// @version 2013-05-25/GGB - Added support for the navigator widget.
    /// @version 2011-06-03/GGB
    ///           @li 1) Make use of controlImage rather than having a pointer to the astroFile.
    ///           @li 2) Integrate functionallity with the dock widgets for information.
    /// @version 2011-03-03/GGB
    ///           @li 1) removed libAstroImages::CastroFile from the header
    ///           @li 2) astroFile is set to NULL
    /// @version 2010-10-30/GGB - Function created.

#ifdef _WIN32
    CAstroGraphicsView::CAstroGraphicsView(QWidget *parent) : QGraphicsView(parent), mode(M_SCROLL), zoomFactor(1),
      bitmapAstrometry(QString(":/cursor/cursorAstrometry.bmp")), maskAstrometry(QString(":/cursor/maskAstrometryWindows.bmp")),
      bitmapPhotometry(QString(":/cursor/cursorPhotometry.bmp")), maskPhotometry(QString(":/cursor/maskPhotometryWindows.bmp")),
      cursorAstrometry(bitmapAstrometry, maskAstrometry, 15, 15), cursorPhotometry(bitmapPhotometry, maskPhotometry, 15, 15),
      maxZoomFactor(2)
#else
    CAstroGraphicsView::CAstroGraphicsView(QWidget *parent) : QGraphicsView(parent), zoomFactor(1), mode(M_SCROLL),
      bitmapAstrometry(QString(":/cursor/cursorAstrometry.bmp")), maskAstrometry(QString(":/cursor/maskAstrometry.bmp")),
      bitmapPhotometry(QString(":/cursor/cursorPhotometry.bmp")), maskPhotometry(QString(":/cursor/maskPhotometry.bmp")),
      cursorAstrometry(bitmapAstrometry, maskAstrometry, 15, 15), cursorPhotometry(bitmapPhotometry, maskPhotometry, 15, 15),
      maxZoomFactor(2)
#endif
    {
      parentObject = dynamic_cast<CAstroImageWindow *>(parent);

      QGraphicsView::setInteractive(true);
      QGraphicsView::setDragMode(QGraphicsView::NoDrag);
      QGraphicsView::setCursor(Qt::CrossCursor);
      QGraphicsView::setMouseTracking(true);

        // Intercept the scrollbar messages to ensure the viewport is updated in the navigator dock widget.

      connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateNavigator(int)));
      connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateNavigator(int)));
    }

    /// @brief Handles the mouse event.
    /// @param[in] mouseEvent: The mouseEvent to be processed.
    /// @details Used to display the image coordinates as well as the pixel value.
    /// @version 2017-08-27/GGB - Change PV display to not include decimal places. (Bug #34)
    /// @version 2017-08-25/GGB - Update WCS values to correct system.
    /// @version 2016-04-25/GGB - Bug# 1574420
    ///   @li Convert Dec to degrees before printing.
    ///   @li Use QString::fromLocal8Bit to convert the degrees sign correctly.
    /// @version 2015-01-01/GGB - Changed some C style casts to static_cast.
    /// @version 2013-08-16/GGB - Corrected an error when calculating the wcs coordinates. (Bug #1213039)
    /// @version 2013-02-24/GGB - Updated to reflect changed return values from pix2wcs.
    /// @version 2013-02-14/GGB - Added support for RA/DEC display if WCS information is available.
    /// @version 2010-10-30/GGB - Function created

    void CAstroGraphicsView::mouseMoveEvent(QMouseEvent *mouseEvent)
    {
      QPointF point = mapToScene(mouseEvent->pos());
      AXIS_t x = static_cast<AXIS_t>(std::floor(point.x()));
      AXIS_t y = static_cast<AXIS_t>(std::floor(point.y()));
      std::optional<ACL::CAstronomicalCoordinates> wcsCoords;
      mdiframe::CFrameWindow *frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      if (parentObject->getControlImage())
      {
        if (parentObject->getControlImage()->astroFile)
        {
          ACL::CAstroFile *af = parentObject->getControlImage()->astroFile.get();

          if ( (x < 0) || (y < 0) ||
            (x >= parentObject->getControlImage()->astroFile->getAstroImage(parentObject->getControlImage()->currentHDB)->width()) ||
            (y >= parentObject->getControlImage()->astroFile->getAstroImage(parentObject->getControlImage()->currentHDB)->height()) )
          {
            parentObject->getControlImage()->szCoords = QString("");
            parentObject->getControlImage()->szPV = QString("");
            parentObject->getControlImage()->szRA = QString("");
            parentObject->getControlImage()->szDEC = QString("");
            dynamic_cast<dockwidgets::CImageControlWidget *>(frameWindow->getDockWidget(mdiframe::IDDW_IMAGECONTROL))->updateValues();
          }
          else
          {
            parentObject->getControlImage()->szCoords = QString("(%1, %2)").arg(x).arg(y);
            parentObject->getControlImage()->szPV = QString(tr("PV=%1")).
                arg((parentObject->getControlImage()->astroFile->
                     getAstroImage(parentObject->getControlImage()->currentHDB)->getValue(x, y)));

            wcsCoords = af->getHDB(parentObject->getControlImage()->currentHDB)->pix2wcs(MCL::TPoint2D<FP_t>(point.x(), point.y()));

            if (wcsCoords)
            {
              parentObject->getControlImage()->szRA = QString::fromStdString((*wcsCoords).RA().A2SHMS());
              parentObject->getControlImage()->szDEC = QString::fromStdString((*wcsCoords).DEC().A2SDMS());
            }
            else
            {
              parentObject->getControlImage()->szRA = QString("--h--'--\"");
              parentObject->getControlImage()->szDEC = QString(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--\"");
            }
            dynamic_cast<dockwidgets::CImageControlWidget *>(frameWindow->getDockWidget(mdiframe::IDDW_IMAGECONTROL))->updateValues();
            dynamic_cast<dockwidgets::CDockWidgetMagnify *>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_MAGNIFY))->updateCenter(point);
          };
        }
        else
        {
          parentObject->getControlImage()->szCoords = QString("");
          parentObject->getControlImage()->szPV = QString("");
          parentObject->getControlImage()->szRA = QString("");
          parentObject->getControlImage()->szDEC = QString("");
          dynamic_cast<dockwidgets::CImageControlWidget *>(frameWindow->getDockWidget(mdiframe::IDDW_IMAGECONTROL))->updateValues();
        };
      };
    }

    /// @brief Handles the mouse release event.
    /// @details The mouse release event is passed onto the CImageWindow class for further processing.
    /// @throws None.
    /// @version 2015-01-01/GGB - Changed some C style casts to static_cast.
    /// @version 2013-06-13/GGB - Added check that the controlImage is non-null.
    /// @version 2011-06-03/GGB - Integrated to docking widgets.
    /// @version 2010-10-31/GGB - Function created.

    void CAstroGraphicsView::mousePressEvent(QMouseEvent *mouseEvent)
    {
      QPointF point = mapToScene(mouseEvent->pos());
      long x = static_cast<long>(std::floor(point.x()));
      long y = static_cast<long>(std::floor(point.y()));
      SControlImage *controlImage = parentObject->getControlImage();

      if (controlImage)     // Note, not an error if there is no control image.
      {
        if ( (x >= 0) && (y >= 0) &&
             (x < controlImage->astroFile->getAstroImage(controlImage->currentHDB)->width()) &&
             (y < controlImage->astroFile->getAstroImage(controlImage->currentHDB)->height()) )
        {
            // Handle the mouse event if it is inside the image.

          parentObject->eventMousePress(mouseEvent);
          QGraphicsView::mousePressEvent(mouseEvent);
        }
        else
        {
          parentObject->eventMousePress(mouseEvent);
        }
      };
    }

    /// @brief Function to repaint the graphics scene when a new image is inserted.
    /// @throws None
    /// @version 2013-07-08/GGB - Removed line scale(zoomFactor, zoomFactor) before the updateNavigator() line. (Bug #1193635)
    /// @version 2013-05-25/GGB - Added support for the View | Navigator window.
    /// @version 2013-05-20/GGB - Removed some dead (commented out code) to redraw the image.
    /// @version 2013-01-20/GGB - Added the minZoomFactor member and calculations.
    /// @version 2011-06-03/GGB - Integrated with the docking widgets.
    /// @version 2011-03-03/GGB - Function created.

    void CAstroGraphicsView::Paint()
    {
      if (parentObject->getControlImage())
      {
        if (parentObject->getControlImage()->astroFile)
        {
          updateNavigator();
        };
      };
    }

    /// @brief Captures the resize event to ensure smooth zooming.
    /// @param[in] event: The event that has been captured.
    /// @throws None.
    /// @version 2013-05-25/GGB - Added support for the View | Navigator window.
    /// @version 2013-01-22/GGB - Function created.

    void CAstroGraphicsView::resizeEvent (QResizeEvent *event)
    {
      QGraphicsView::resizeEvent(event);

      setMinZoom();

      zoomFactor = std::max(zoomFactor, minZoomFactor);

      resetTransform();
      scale(zoomFactor, zoomFactor);
      repaint();

      updateNavigator();
    }

    /// @brief Function to call when an image is added or changed in the view.
    /// @throws None.
    /// @version 2015-09-21/GGB - Changed code to use std::min()
    /// @version 2013-01-20/GGB - Function created.

    void CAstroGraphicsView::setMinZoom()
    {
      const int viewWidth = width() - MARGIN_X;
      const int viewHeight = height() - MARGIN_Y;

      const qreal sceneWidth = scene()->width();
      const qreal sceneHeight = scene()->height();

      double zoomX = viewWidth / sceneWidth;
      double zoomY = viewHeight / sceneHeight;

      minZoomFactor = std::min(zoomX, zoomY);

      zoomFactor = std::max(zoomFactor, minZoomFactor);
    }

    /// @brief Sets the mode.
    /// @param[in] newMode: The new mode to select.
    /// @details Selects the correct cursor to use depending on the mode that has been chosen.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2011-06-26/GGB - Function created

    void CAstroGraphicsView::setMode(E_Mode newMode)
    {
      mode = newMode;

      switch (mode)
      {
        case M_NONE:
          setCursor(Qt::CrossCursor);
          break;
        case M_ZOOMDYNAMIC:
        case M_ZOOMSELECTION:
        case M_ZOOMIN:
        case M_ZOOMOUT:
        case M_SCROLL:
          break;
        case M_ASTROMETRY:
          setCursor(cursorAstrometry);
          break;
        case M_PHOTOMETRY:
          setCursor(cursorPhotometry);
          break;
        default:
        {
          CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Function to update the navigator dockwidget when the zoom factor is changed.
    /// @throws None.
    /// @version 2017-07-03/GGB - Updated to reflect new dockwidget storage method.
    /// @version 2013-05-25/GGB - Function created.

    void CAstroGraphicsView::updateNavigator(int)
    {
      QPoint TL(0,0);                       // Top Left
      QPoint BR(width(), height());         // Bottom right

      QPointF rvTL = mapToScene(TL);
      QPointF rvBR = mapToScene(BR);

      QRectF limits(rvTL, rvBR);

      mdiframe::CFrameWindow *frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      dynamic_cast<dockwidgets::CDockWidgetNavigator *>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_NAVIGATOR))->updateLimits(limits);
    }

    /// @brief Manage the wheel event. The image is automatically zoomed.
    /// @param[in] event: The event to manage.
    /// @throws None.
    /// @version 2013-08-03/GGB - Added suport to zoom on the mouse position. (Bug #1102931)
    /// @version 2013-07-20/GGB - Added suport to center on the mouse position. (Bug #1102931)
    /// @version 2013-05-25/GGB - Added support for the View | Navigator window.
    /// @version 2013-01-22/GGB - Added the minZoomFactor and maxZoomFactor class members.
    /// @version 2012-12-03/GGB - Removed the zoom mode
    /// @version 2011-06-02/GGB - Function created.

    void CAstroGraphicsView::wheelEvent(QWheelEvent *event)
    {
      QPointF scenePos = mapToScene(event->pos());
      int numSteps = event->delta() / 120;
      double prevZoomFactor = zoomFactor;

      setMinZoom();

      if (numSteps < 0)
      {
        zoomFactor *= std::pow(1/ZOOM_FACTOR, std::abs(numSteps));
      }
      else
      {
        zoomFactor *= std::pow(ZOOM_FACTOR, std::abs(numSteps));
      };

      zoomFactor = std::max(zoomFactor, minZoomFactor);
      zoomFactor = std::min(zoomFactor, maxZoomFactor);

      if (zoomFactor != prevZoomFactor)
      {
        resetTransform();
        scale(zoomFactor, zoomFactor);

        QPoint viewportPos = mapFromScene(scenePos);
        QPoint delta = event->pos() - viewportPos;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
      };
    }

    /// @brief Zooms the image to 1:1
    /// @throws None.
    /// @version 2013-05-25/GGB - Added support for the View | Navigator window.
    /// @version 2012-07-21/GGB - Function created.

    void CAstroGraphicsView::zoom11()
    {
      zoomFactor = 1;   // Zoom factor should be 1:1.
      resetTransform();
      scale(zoomFactor, zoomFactor);  // Scale the scene

      updateNavigator();
    }

    /// @brief Zooms the view to ensure that the entire scene is visible.
    /// @throws None.
    /// @version 2013-05-25/GGB - Added support for the View | Navigator window.
    /// @version 2013-01-22/GGB - Added the minZoomFactor class member.
    /// @version 2011-06-02/GGB - Function created.

    void CAstroGraphicsView::zoomAll()
    {
      setMinZoom();
      zoomFactor = minZoomFactor;
      resetTransform();
      scale(zoomFactor, zoomFactor);

      updateNavigator();
    }

    /// @brief Zoom in by one step.
    /// @throws None.
    /// @version 2013-07-14/GGB - Removed the zoom mode and added the ZOOMFACTOR (Bug # 1201088)
    /// @version 2011-06-02/GGB - Function created.

    void CAstroGraphicsView::zoomIn()
    {
      double prevZoomFactor = zoomFactor;

      zoomFactor *= ZOOM_FACTOR;


      zoomFactor = std::max(zoomFactor, minZoomFactor);
      zoomFactor = std::min(zoomFactor, maxZoomFactor);

      if (zoomFactor != prevZoomFactor)
      {
        resetTransform();
        scale(zoomFactor, zoomFactor);
      };

      updateNavigator();
    }

    /// @brief Zoom out by one step.
    /// @throws None.
    /// @version 2013-07-14/GGB - Removed the zoom mode and added the ZOOMFACTOR (Bug #1201088)
    /// @version 2011-06-02/GGB - Function created.

    void CAstroGraphicsView::zoomOut()
    {
      double prevZoomFactor = zoomFactor;

      zoomFactor /= ZOOM_FACTOR;

      zoomFactor = std::max(zoomFactor, minZoomFactor);
      zoomFactor = std::min(zoomFactor, maxZoomFactor);

      if (zoomFactor != prevZoomFactor)
      {
        resetTransform();
        scale(zoomFactor, zoomFactor);
      };

      updateNavigator();
    }

    void CAstroGraphicsView::zoomSelection()
    {
      mode = M_ZOOMSELECTION;
    }

  }  // astrographicsview
}  // astroManager
