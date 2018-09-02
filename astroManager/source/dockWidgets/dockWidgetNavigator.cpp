//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetNavigator
// SUBSYSTEM:						Provides an overview of the current viewport on the image.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2018 Gavin Blakeman.
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
// OVERVIEW:            Provides a dock widget showing a magnified view of the current cursor position.
//
// CLASSES INCLUDED:    CDockWidgetNavigator
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CPhotometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//                        - CDockWidgetMagnify
//                        - CDockWidgetNavigator
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-05-23 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dockWidgets/dockWidgetNavigator.h"

  // astroManager header files

#include "../../include/settings.h"
#include "../../include/astroManager.h"

namespace astroManager
{
  namespace dockwidgets
  {

    const int MARGIN_X  = 5;
    const int MARGIN_Y  = 5;

    //*****************************************************************************************************************************
    //
    // CDockWidgetNavigator
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @param[in] parent - The parent object.
    /// @param[in] action - The action object.
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of parameters and updated hierarchy.
    /// @version 2013-07-27/GGB - Added objectName.
    /// @version 2013-05-24/GGB - Function created.

    CDockWidgetNavigator::CDockWidgetNavigator(QWidget *parent, QAction *action)
      : CDockWidgetImage(DW_NAVIGATOR_NAME, parent, action, settings::DW_IMAGE_NAVIGATOR_VISIBLE),
      viewRectangle(nullptr)
    {
      setupUI();
      setObjectName(DW_NAVIGATOR_NAME);
    }

    /// @brief Saves the widget settings on exit.
    //
    // 2013-05-24/GGB - Function created.

    CDockWidgetNavigator::~CDockWidgetNavigator()
    {
    }

    /// @brief Function called when the image is updated.
    //
    // 2013-05-24/GGB - Function created.

    void CDockWidgetNavigator::imageChanged()
    {
      qreal zoomFactor;

      graphicsScene->clear();
      viewRectangle = nullptr; // Killed by the graphicsScene->clear()

      if (currentImage)
      {
        graphicsScene->addPixmap(*currentImage->pixmap);
        graphicsView->resetTransform();

        int viewWidth = graphicsView->width() - ( 2 * MARGIN_X);
        int viewHeight = graphicsView->height() - ( 2 * MARGIN_Y);

        qreal sceneWidth = currentImage->pixmap->width();;
        qreal sceneHeight = currentImage->pixmap->height();
        graphicsView->setSceneRect(0, 0, sceneWidth, sceneHeight);

        qreal zoomX = (qreal) viewWidth / sceneWidth;
        qreal zoomY = (qreal) viewHeight / sceneHeight;

        if (zoomX < zoomY)
        {
          zoomFactor = zoomX;
        }
        else
        {
          zoomFactor = zoomY;
        }

        graphicsView->scale(zoomFactor, zoomFactor);
        graphicsView->centerOn(sceneWidth / 2, sceneHeight / 2);
        graphicsView->repaint();
      };
    }

    /// Deactivates the window if the image deactivates.
    //
    // 2013-06-22/GGB - Changed function name to setEnabled.
    // 2013-05-25/GGB - Function created.

    void CDockWidgetNavigator::setEnabled(bool enabledValue)
    {
      CDockWidget::setEnabled(enabledValue);

      if (!enabledValue)
      {
        imageChanged();
      };
    }

    /// @brief Sets up the user interface for the class.
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2013-05-24/GGB - Function created.

    void CDockWidgetNavigator::setupUI()
    {
      QUiLoader loader;

      QFile file(":/forms/dwNavigator.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/dwNavigator.ui");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      QGridLayout *glayout = (QGridLayout *) formWidget->layout();

      graphicsView = new QGraphicsView(this);
      graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);
      graphicsView->setCursor(Qt::ForbiddenCursor);
      graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      graphicsView->setInteractive(false);      // No interaction allowed.
      glayout->addWidget(graphicsView, 0, 0, 1, 1);    // Only object in the layout

      graphicsScene = new QGraphicsScene();

      graphicsView->setScene(graphicsScene);
    }

    /// @brief Redraws the view limits rectangle.
    /// @param[in] limits - The current rectangle containing the view.
    /// @throws None.
    /// @version 2013-05-25/GGB - Function created.

    void CDockWidgetNavigator::updateLimits(QRectF const &limits)
    {
      QPen pen(Qt::red);

      if (viewRectangle)
      {
        graphicsScene->removeItem(viewRectangle);
        delete viewRectangle;
        viewRectangle = nullptr;
      };

      viewRectangle = new QGraphicsRectItem(limits);
      viewRectangle->setPen(pen);
      graphicsScene->addItem(viewRectangle);
    }

  }  // namespace dockwidgets
}  // namespace AstroManager
