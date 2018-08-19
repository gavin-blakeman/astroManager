//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidget
// SUBSYSTEM:						The dockWidget class derived from QDockWidget
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CDockWidget
//
// CLASS HIERARCHY:     QDockWidget
//                        - CDockWidget
//                          - CAstrometryDockWidget
//                          - CPhotometryDockWidget
//                          - CInstrumentDockwidget
//                          - CMessageWidget
//                          - CDockWidgetMagnify
//                          - CDockWidgetWeatherScale
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-03-17 GGB - Removed CSelectObjectDialog into file DialogSelectObject.
//                      2013-03-17 GGB - Removed CImageControlWidget into file DockWidgetImageControl.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into it's own file
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dockWidgets/dockWidget.h"

#include "../../include/Settings.h"

namespace astroManager
{
  namespace dockwidgets
  {
    //*****************************************************************************************************************************
    //
    // CDockWidget
    //
    //*****************************************************************************************************************************

    /// @brief Default constructor for the class.
    /// @param[in] title - The name for the window
    /// @param[in] parent - The owner window.
    /// @param[in] action - The action to associate with the dockWidget.
    /// @throws None.
    /// @version 2017-07-01/GGB - Updated function to use with weather dock widgets.
    /// @version 2013-03-17/GGB - Function created.

    CDockWidget::CDockWidget(QString const &title, QWidget *parent, QAction *action, QString const &visibleSetting)
      : QDockWidget(title, parent), dockWidgetAction(action), visibleSettingString(visibleSetting)
    {
    }

    /// @brief Handles the close event. Calls the writeSettings() function.
    /// @param[in] event - The event to process.
    /// @throws None.
    /// @version 2013-07-02/GGB - Function created.

    void CDockWidget::closeEvent(QCloseEvent *event)
    {
      settings::astroManagerSettings->setValue(visibleSettingString, QVariant(isVisible()));
      event->accept();
    }

    /// @brief Called when the widget is hiding.
    /// @param[in] event - The event to process.
    /// @throws None.
    /// @version 2017-07-01/GGB - Updated for weather dockWidgets
    /// @version 2011-06-12/GGB - Function created.

    void CDockWidget::hideEvent(QHideEvent *event)
    {
      if (!event->spontaneous() && dockWidgetAction)
      {
        dockWidgetAction->setChecked(false);
      }

      QDockWidget::hideEvent(event);
    }

    /// @brief Handles a show event and checks the widget action.
    /// @param[in] event - The event to process.
    /// @throws None.
    /// @version 2017-07-01/GGB - Updated for weather dockWidgets
    /// @version 2011-06-12/GGB - Function created.

    void CDockWidget::showEvent(QShowEvent *event)
    {
      if (!event->spontaneous() && dockWidgetAction)
      {
        dockWidgetAction->setChecked(true);
      };

      QDockWidget::showEvent(event);
    }



  } // namespace dockwidgets

} // namespace AstroManager
