//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetWeatherScale
// SUBSYSTEM:						Code to manage the Weather Scale Dock Widget
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, qwt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CDockWidgetWeatherScale
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//                        - CDockWidgetWeatherScale
//
// HISTORY:             2017-07-01/GGB - File Created
//
//*********************************************************************************************************************************

#include "../../Include/dockWidgets/dockWidgetWeatherScale.h"

#include "../../Include/Settings.h"
#include "../../Include/astroManager.h"

namespace AstroManager
{
  namespace dockwidgets
  {

    /// @brief Class constructor.
    /// @param[in] action - Pointer to the action (button)
    /// @param[in] parent - Pointer to the parent object.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    CDockWidgetWeatherScale::CDockWidgetWeatherScale(QWidget *parent,QAction *action)
      : CDockWidgetWeather(DW_WEATHER_SCALE, parent, action, settings::DW_WEATHER_SCALE_VISIBLE)
    {
      setObjectName(DW_WEATHER_SCALE);
      setupUI();
    }

    /// @brief Sets up the UI
    /// @throws GCL::CRuntimeAssert(...)
    /// @throws GCL::CError(AIRDAS, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2017-07-01/GGB - Function Created.

    void CDockWidgetWeatherScale::setupUI()
    {
      QUiLoader loader;

      QFile file(":/dockWidgets/dockWidgetWeatherScale.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/dockWidgets/dockWidgetWeatherScale.ui.");
        ERROR(AIRDAS, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      ASSOCIATE_RADIOBUTTON(radioButtonExtentDay, formWidget, "radioButtonExtentDay");
      ASSOCIATE_RADIOBUTTON(radioButtonExtentMonth, formWidget, "radioButtonExtentMonth");
      ASSOCIATE_RADIOBUTTON(radioButtonExtentYear, formWidget, "radioButtonExtentYear");

      ASSOCIATE_RADIOBUTTON(radioButtonMajorHour, formWidget, "radioButtonMajorHour");
      ASSOCIATE_RADIOBUTTON(radioButtonMajorDay, formWidget, "radioButtonMajorDay");
      ASSOCIATE_RADIOBUTTON(radioButtonMajorWeek, formWidget, "radioButtonMajorWeek");
      ASSOCIATE_RADIOBUTTON(radioButtonMajorMonth, formWidget, "radioButtonMajorMonth");

      ASSOCIATE_RADIOBUTTON(radioButtonMinorMinute, formWidget, "radioButtonMinorMinute");
      ASSOCIATE_RADIOBUTTON(radioButtonMinorHour, formWidget, "radioButtonMinorHour");
      ASSOCIATE_RADIOBUTTON(radioButtonMinorDay, formWidget, "radioButtonMinorDay");
      ASSOCIATE_RADIOBUTTON(radioButtonMinorWeek, formWidget, "radioButtonMinorWeek");

      setEnabled(false);
    }
  } // namespace dockwidgets

} // namespace AstroManager
