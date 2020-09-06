//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								AstroImageWindow
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imaging
// AUTHOR:							Gavin Blakeman. (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CAstroImageWindow         - Window for displaying and manipulating astronomical images.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//                          - CAstroImageWindow
//                            - CImageWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-03-01 GGB - Moved CAstroImageWindow into seperate file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/windowImage/windowImage.h"

  // astroManager include files

#include "../../include/settings.h"
#include "../../include/database/databaseARID.h"
#include "../../include/dockWidgets/dockWidgetAstrometry.h"
#include "../../include/dockWidgets/dockWidgetHistogram.h"
#include "../../include/dockWidgets/dockWidgetImageInformation.h"
#include "../../include/dockWidgets/dockWidgetMagnify.h"
#include "../../include/dockWidgets/dockWidgetNavigator.h"
#include "../../include/dockWidgets/dockWidgetPhotometry.h"

namespace astroManager
{
  namespace imaging
  {
    /// @brief Constructor for class
    /// @param[in] parent: Pointer to the parent widget
    /// @throws None.
    /// @version 2013-03-01 - Function created.

    CAstroImageWindow::CAstroImageWindow(QWidget * parent) : CMdiSubWindow(parent), mode(M_NONE)
    {
    }

    /// @brief Draws the astrometry indicator
    /// @param[in] astrometryObject: The object to be marked on the image.
    /// @param[in] pen: The pen to use for the marking.
    /// @throws None.
    /// @version 2013-08-25/GGB - Function created.

    void CAstroImageWindow::drawAstrometryIndicator(astrometry::CAstrometryObservation *astrometryObject, QPen const &pen)
    {
      int ai = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_TYPE, QVariant(0)).toInt();

      switch (ai)
      {
        case 0:
        {
          drawCrossIndicator(astrometryObject, pen);
          break;
        };
        case 1:
        {
          drawCircleIndicator(astrometryObject, pen);
          break;
        };
        default:
        {
          drawCrossIndicator(astrometryObject, pen);
          break;
        };
      };
    }

    /// @brief Creates the group for a cross indicator.
    /// @throws None.
    /// @version 2011-12-20/GGB - Function created

    void CAstroImageWindow::drawCrossIndicator(astrometry::CAstrometryObservation *astrometryObject, QPen const &pen)
    {
      const long space = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_SPACE, QVariant(5)).toInt();
      const long length = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_LENGTH, QVariant(10)).toInt() + space;

      QGraphicsLineItem *lineItem = nullptr;

      astrometryObject->group = new QGraphicsItemGroup();
      lineItem = new QGraphicsLineItem(astrometryObject->CCDCoordinates().x() + space,
                                       astrometryObject->CCDCoordinates().y(),
                                       astrometryObject->CCDCoordinates().x() + length,
                                       astrometryObject->CCDCoordinates().y());
      lineItem->setPen(pen);

      astrometryObject->group->addToGroup(lineItem);
      lineItem = new QGraphicsLineItem(astrometryObject->CCDCoordinates().x() - space,
                                       astrometryObject->CCDCoordinates().y(),
                                       astrometryObject->CCDCoordinates().x() - length,
                                       astrometryObject->CCDCoordinates().y());
      astrometryObject->group->addToGroup(lineItem);
      lineItem->setPen(pen);

      lineItem = new QGraphicsLineItem(astrometryObject->CCDCoordinates().x(),
                                       astrometryObject->CCDCoordinates().y() + space,
                                       astrometryObject->CCDCoordinates().x(),
                                       astrometryObject->CCDCoordinates().y() + length);
      astrometryObject->group->addToGroup(lineItem);
      lineItem->setPen(pen);

      lineItem = new QGraphicsLineItem(astrometryObject->CCDCoordinates().x(),
                                       astrometryObject->CCDCoordinates().y() - space,
                                       astrometryObject->CCDCoordinates().x(),
                                       astrometryObject->CCDCoordinates().y() - length);
      astrometryObject->group->addToGroup(lineItem);
      lineItem->setPen(pen);

      astrometryObject->text =  new QGraphicsSimpleTextItem(QString::fromStdString(astrometryObject->objectName()));
      astrometryObject->text->setPos(astrometryObject->CCDCoordinates().x(), astrometryObject->CCDCoordinates().y() + length + 1);
      astrometryObject->text->setPen(pen);

      astrometryObject->group->addToGroup(astrometryObject->text);
    }

    /// @brief Creates the group and text for a photometry indicator.
    /// @param[in] photometryObject:
    /// @param[in] pen:
    /// @throws GCL::CCodeError
    /// @version 2013-05-08/GGB - Support for different types of aperture added.
    /// @version 2012-11-09/GGB - Function created

    void CAstroImageWindow::drawPhotometryIndicator(photometry::CPhotometryObservation *photometryObject, QPen const &pen)
    {
      QGraphicsEllipseItem *newItem = nullptr;
      ACL::PPhotometryApertureCircular pac =
          std::dynamic_pointer_cast<ACL::CPhotometryApertureCircular>(photometryObject->photometryAperture());

      switch (photometryObject->photometryAperture()->apertureType())
      {
        case ACL::PAT_CIRCULAR:
          photometryObject->group = new QGraphicsItemGroup();

          newItem = new QGraphicsEllipseItem(photometryObject->CCDCoordinates().x() - pac->radius1(),
                                             photometryObject->CCDCoordinates().y() - pac->radius1(),
                                             2*pac->radius1(), 2*pac->radius1());
          newItem->setPen(pen);
          photometryObject->group->addToGroup(newItem);

          newItem = new QGraphicsEllipseItem(photometryObject->CCDCoordinates().x()-pac->radius2(),
                                             photometryObject->CCDCoordinates().y()-pac->radius2(),
                                             2*pac->radius2(), 2*pac->radius2());
          newItem->setPen(pen);
          photometryObject->group->addToGroup(newItem);

          newItem = new QGraphicsEllipseItem(photometryObject->CCDCoordinates().x()-pac->radius3(),
                                             photometryObject->CCDCoordinates().y()-pac->radius3(),
                                             2*pac->radius3(), 2*pac->radius3());
          newItem->setPen(pen);
          photometryObject->group->addToGroup(newItem);

          photometryObject->text =  new QGraphicsSimpleTextItem(QString::fromStdString(photometryObject->objectName()) );
          photometryObject->text->setPos(photometryObject->CCDCoordinates().x(), photometryObject->CCDCoordinates().y() + pac->radius3() + 1);
          photometryObject->text->setPen(pen);

          photometryObject->group->addToGroup(photometryObject->text);
          break;
        case ACL::PAT_ELLIPSE:
          break;
        default:
        {
          CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Creates the group for a circle indicator.
    /// @throws std::bad_alloc
    /// @version 2011-12-20/GGB - Function created

    void CAstroImageWindow::drawCircleIndicator(astrometry::CAstrometryObservation *astrometryObject, QPen const &pen)
    {
      long const radius = settings::astroManagerSettings->value(settings::ASTROMETRY_CIRCLE_RADIUS, QVariant(5)).toInt();

      QGraphicsEllipseItem *newItem = nullptr;

      astrometryObject->group = new QGraphicsItemGroup();

      newItem = new QGraphicsEllipseItem(astrometryObject->CCDCoordinates().x()-radius,
                                         astrometryObject->CCDCoordinates().y()-radius,
                                         2*radius, 2*radius);
      newItem->setPen(pen);
      astrometryObject->group->addToGroup(newItem);

      astrometryObject->text = new QGraphicsSimpleTextItem(QString::fromStdString(astrometryObject->objectName()));
      astrometryObject->text->setPos(astrometryObject->CCDCoordinates().x(), astrometryObject->CCDCoordinates().y() + radius + 1);
      astrometryObject->text->setPen(pen);

      astrometryObject->group->addToGroup(astrometryObject->text);
    }

    /// @brief Function that notifies the relevant dock widgets when an image changes.
    /// @param[in] newImage: The newImage that needs to be displayed.
    /// @throws GCL::CCodeError
    /// @version 2017-07-03/GGB - Updated to reflect new dock widget storage.
    /// @version 2013-03-10/GGB - Function created.

    void CAstroImageWindow::imageChange(imaging::SControlImage *newImage)
    {
      dockwidgets::CDockWidgetImage::setCurrentImage(newImage);     // Static function... no problem

      mdiframe::CFrameWindow *frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      if (frameWindow)
      {
        dynamic_cast<dockwidgets::CDockWidgetImage *>(frameWindow->getDockWidget(mdiframe::IDDW_IMAGECONTROL))->imageChanged();
        dynamic_cast<dockwidgets::CDockWidgetImage *>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_HISTOGRAM))->imageChanged();
        dynamic_cast<dockwidgets::CDockWidgetImage *>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_MAGNIFY))->imageChanged();
        dynamic_cast<dockwidgets::CDockWidgetImage *>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_NAVIGATOR))->imageChanged();
        dynamic_cast<dockwidgets::CDockWidgetImage *>(frameWindow->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL))->imageChanged();
        dynamic_cast<dockwidgets::CDockWidgetImage *>(frameWindow->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL))->imageChanged();
      }
      else
      {
        CODE_ERROR;
      };
    }

    /// @brief Called when the window is activating. This function performs actions that are unique to the imageWindow classes.
    /// @throws None.
    /// @version 2018-10-30/GGB - Function created.

    void CAstroImageWindow::windowActivating()
    {
      dockwidgets::CDockWidgetImage::setCurrentImage(getControlImage());     // Static function... no problem

      CMdiSubWindow::windowActivating();    // Last thing before exiting as the dockwidgets need to be informed that the current
                                            // image has changed.
    }

  }  // namespace imaging
}  // namespace AstroManager
