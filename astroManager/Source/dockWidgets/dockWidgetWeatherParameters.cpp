//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dockWidgetWeatherParameters
// SUBSYSTEM:						Code to manage the Weather Scale Dock Widget
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, qwt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017 Gavin Blakeman.
//                      This file is part of the Astronomical Image Reduction and Data Analysis Software (AIRDAS)
//
//                      AIRDAS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      AIRDAS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with AIRDAS.  If not,
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
// HISTORY:             2017-07-06/GGB - File Created
//
//*********************************************************************************************************************************

#include "../../Include/dockWidgets/dockWidgetWeatherParameters.h"

  // AIRDAS include files

#include "../../Include/Settings.h"
#include "../../Include/VSOP.h"

namespace AstroManager
{
  namespace dockwidgets
  {
    /// @brief Class constuctor.
    /// @param[in] widget - The widget that owns the dock widget. (The frame window)
    /// @param[in] action - The action associated with the widget.
    /// @throws None.
    /// @version 2017-07-06/GGB - Function created.

    CDockWidgetWeatherParameters::CDockWidgetWeatherParameters(QWidget *widget, QAction *action) :
      CDockWidgetWeather(DW_WEATHER_PARAMETERS, widget, action, settings::DW_WEATHER_PARAMETERS_VISIBLE)
    {
      setObjectName(DW_WEATHER_PARAMETERS);
      setupUI();
    }

    /// @brief Function to setup the window
    /// @throws GCL::CRuntimeAssert(AIRDAS)
    /// @throws GCL::CError(AIRDAS, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2017-07-06/GGB - Function created.

    void CDockWidgetWeatherParameters::setupUI()
    {
      QUiLoader loader;

      QFile file(":/dockWidgets/dockWidgetWeatherParameters.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/dockWidgets/dockWidgetWeatherParameters.ui");
        ERROR(AIRDAS, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);
    }

  } // namespace dockwidgets
} // namespace AstroManager
