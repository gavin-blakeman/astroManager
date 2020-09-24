//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dockWidgetWeather
// SUBSYSTEM:						Code to manage the Weather Scale Dock Widget
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, qwt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2020 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CDockWidgetWeather
//
// CLASS HIERARCHY:     QDockWidget
//                        - CDockWidgetImage
//                        - CDockWidgetWeather
//                          - CDockWidgetWeatherScale
//
// HISTORY:             2017-07-01/GGB - File Created
//
//*********************************************************************************************************************************

#include "include/dockWidgets/dockWidgetWeather.h"

namespace astroManager
{
  namespace dockwidgets
  {

    /// @brief Default constructor for the class.
    /// @param[in] title - The name for the window
    /// @param[in] parent - The owner window.
    /// @param[in] action - The action to associate with the dockWidget.
    /// @throws None.
    /// @version 2017-07-03/GGB - Function created.

    CDockWidgetWeather::CDockWidgetWeather(QString const &title, QWidget *parent, QAction *action, QString const &visibleSetting)
      : CDockWidget(title, parent, action, visibleSetting)
    {
    }

  } // namespace dockwidgets
} // namespace AstroManager
