//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetMagnify
// SUBSYSTEM:						Provides a magnified view of the current cursor position.
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
// CLASSES INCLUDED:    CMagnifyDockWidget
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CPhotometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//                        - CDockWidgetMagnify
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-05-19 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dockWidgets/dockWidgetMagnify.h"

  // astroManager header files

#include "../../include/settings.h"

namespace astroManager
{
  namespace dockwidgets
  {

    //*****************************************************************************************************************************
    //
    // CDockWidgetMagnify
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @param[in] parent - Pointer to the parent object
    /// @param[in] action - Pointer to the action associated with the dockwidget
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of arguments. Updated class to reflect the weather dock widgets.
    /// @version 2013-07-27/GGB - Added object name.
    /// @version 2013-05-19/GGB - Function created.

    CDockWidgetMagnify::CDockWidgetMagnify(QWidget *parent, QAction *action)
      : CDockWidgetImage(tr("Magnify"), parent, action, settings::DW_IMAGE_MAGNIFY_VISIBLE), popupMenu(nullptr)
    {
      size_t index;

      for (index = 0; index < IDA_DW_MAGNIFY_MAX; ++index)
      {
        menuActions[index] = nullptr;
      };

      createActions();
      createPopupMenu();

      setupUI();

      setObjectName(DW_MAGNIFIER_NAME);
    }

    /// @brief Ensure dynamically allocaed memory is deleted.
    /// @throws None.
    /// @version 2013-05-23/GGB - Function created.

    CDockWidgetMagnify::~CDockWidgetMagnify()
    {
      size_t index;

      if (popupMenu)
      {
        delete popupMenu;
        popupMenu = nullptr;
      };

      for (index = 0; index < IDA_DW_MAGNIFY_MAX; ++index)
      {
        if (menuActions[index])
        {
          delete menuActions[index];
          menuActions[index] = nullptr;
        };
      };
    }

    /// @brief Create the actions for the widget popup menu.
    /// @throws None.
    /// 2013-05-23/GGB - Function created.

    void CDockWidgetMagnify::createActions()
    {
      menuActions[IDA_DW_MAGNIFY_11] = new QAction(QIcon(":/images/zoom_11.png"), "1:1", this);
      menuActions[IDA_DW_MAGNIFY_11]->setStatusTip(tr("Magnify to 1:1"));
      connect(menuActions[IDA_DW_MAGNIFY_11], SIGNAL(triggered()), this, SLOT(eventMagnify11()));

      menuActions[IDA_DW_MAGNIFY_21] = new QAction(QIcon(":/images/zoom_21.png"), "2:1", this);
      menuActions[IDA_DW_MAGNIFY_21]->setStatusTip(tr("Magnify to 2:1"));
      connect(menuActions[IDA_DW_MAGNIFY_21], SIGNAL(triggered()), this, SLOT(eventMagnify21()));

      menuActions[IDA_DW_MAGNIFY_41] = new QAction(QIcon(":/images/zoom_41.png"), "4:1", this);
      menuActions[IDA_DW_MAGNIFY_41]->setStatusTip(tr("Magnify to 4:1"));
      connect(menuActions[IDA_DW_MAGNIFY_41], SIGNAL(triggered()), this, SLOT(eventMagnify41()));
    }

    /// @brief Function to create the popup menu used for changing the magnification ratio.
    /// @throws None.
    /// @version 2013-05-23/GGB - Function created.

    void CDockWidgetMagnify::createPopupMenu()
    {
      popupMenu = new QMenu();

      popupMenu->addAction(menuActions[IDA_DW_MAGNIFY_11]);
      popupMenu->addAction(menuActions[IDA_DW_MAGNIFY_21]);
      popupMenu->addAction(menuActions[IDA_DW_MAGNIFY_41]);
    }

    /// Sets the magnification to 1:1
    //
    // 2013-05-23/GGB - Function created.

    void CDockWidgetMagnify::eventMagnify11()
    {
      graphicsView->resetTransform();
      graphicsView->scale(1, 1);
      graphicsView->update();
      setWindowTitle(QString("Magnify " % UTF16_MULTIPLYSIGN % "1"));
      settings::astroManagerSettings->setValue(settings::DW_MAGNIFY_MAGNIFICATION, QVariant(1));
    }

    /// Sets the magnification to 2:1
    //
    // 2013-05-23/GGB - Function created.

    void CDockWidgetMagnify::eventMagnify21()
    {
      graphicsView->resetTransform();
      graphicsView->scale(2, 2);
      graphicsView->update();
      setWindowTitle(QString("Magnify " % UTF16_MULTIPLYSIGN % "2"));
      settings::astroManagerSettings->setValue(settings::DW_MAGNIFY_MAGNIFICATION, QVariant(2));
    }

    /// Sets the magnification to 4:1
    //
    // 2013-05-23/GGB - Function created.

    void CDockWidgetMagnify::eventMagnify41()
    {
      graphicsView->resetTransform();
      graphicsView->scale(4, 4);
      graphicsView->update();
      setWindowTitle(QString("Magnify " % UTF16_MULTIPLYSIGN % "4"));
      settings::astroManagerSettings->setValue(settings::DW_MAGNIFY_MAGNIFICATION, QVariant(4));
    }

    /// @brief Activates the window when the image is activated.
    /// @throws None.
    /// @version 2013-05-25/GGB - Function created.

    void CDockWidgetMagnify::imageActivating()
    {
      if (currentImage)
      {
        if (!isEnabled())
        {
          setEnabled(true);
        };
      };
    }

    /// @brief Function called when the image is updated.
    /// @throws None.
    /// @version 2013-05-20/GGB - Function created.

    void CDockWidgetMagnify::imageChanged()
    {
      graphicsScene->clear();

      if (currentImage)
      {
        graphicsScene->addPixmap(*currentImage->pixmap);
      };
    }

    /// @brief Disable the widget if the image is deactivated.
    /// @throws None.
    /// @version 2013-05-25/GGB - Function created.

    void CDockWidgetMagnify::imageDeactivating()
    {
      setEnabled(false);
      imageChanged();
    }

    /// @brief Called when the window is activated. This allows menus etc to be updated.
    /// @param[in] activeSubWindow: The active sub window.
    /// @throws None.
    /// @pre 1. The currentImage member must have been updated before calling this function.
    /// @version 2018-10-30/GGB - Function created.

    void CDockWidgetMagnify::mdiWindowActivating(CMdiSubWindow *activeSubWindow)
    {
      if ( (activeSubWindow) && (activeSubWindow->getWindowClass() == CMdiSubWindow::WC_IMAGE))
      {
        imageChanged();
      }
      else
      {
          // No active window, or window is not an image window. We can disable the controls.
      };
    }

    /// @brief Handles the mouse press event.
    /// @param[in] mouseEvent - The mouse event details
    /// @details The right button is responded to by a pop-up menu to set the magnification.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-05-31/GGB - Function created.

    void CDockWidgetMagnify::mousePressEvent(QMouseEvent *mouseEvent)
    {
      switch (mouseEvent->button())
      {
        case Qt::LeftButton:
          break;
        case Qt::RightButton:
        {
          popupMenu->popup(mapToGlobal(mouseEvent->pos()));
          break;
        };
        case Qt::MidButton:
        case Qt::NoButton:
          break;
        default:
        {
          CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Sets up the user interface for the class.
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Fixed Bug #90.
    /// @version 2013-05-20/GGB - Function created.

    void CDockWidgetMagnify::setupUI()
    {
      QUiLoader loader;

      QFile file(":/forms/dwMagnify.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/dwMagnify.ui.");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      QGridLayout *glayout = dynamic_cast<QGridLayout *>(formWidget->layout());

      graphicsView = new QGraphicsView();
      graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);
      graphicsView->setCursor(Qt::ForbiddenCursor);
      graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      graphicsView->setInteractive(false);      // No interaction allowed.
      glayout->addWidget(graphicsView, 0, 0, 1, 1);    // Only object in the layout

      graphicsScene = new QGraphicsScene();

      graphicsView->setScene(graphicsScene);

      QVariant variant = settings::astroManagerSettings->value(settings::DW_MAGNIFY_MAGNIFICATION, QVariant(4));

      switch (variant.toInt())
      {
      case 1:
        graphicsView->scale(1, 1);
        setWindowTitle(QString("Magnify " % UTF16_MULTIPLYSIGN % "1"));
        break;
      case 2:
        graphicsView->scale(2, 2);
        setWindowTitle(QString("Magnify " % UTF16_MULTIPLYSIGN % "2"));
        break;
      case 4:
      default:
        graphicsView->scale(4, 4);
        setWindowTitle(QString("Magnify " % UTF16_MULTIPLYSIGN % "4"));
        break;
      };
    }

    /// @brief Centers the image.
    /// @param[in] center - The center coordinates.
    /// @throws None.
    /// @version 2013-05-20/GGB - Function created.

    void CDockWidgetMagnify::updateCenter(QPointF const &center)
    {
      graphicsView->centerOn(center);
    }

  }  // namespace dockwidgets
}  // namespace AstroManager
