﻿//*********************************************************************************************************************************
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

#ifndef DOCKWIDGETWEATHER
#define DOCKWIDGETWEATHER

#include "dockWidget.h"

namespace astroManager
{
  namespace dockwidgets
  {
    class CDockWidgetWeather : public CDockWidget
    {
    private:
    protected:
    public:
      CDockWidgetWeather(QString const &, QWidget *, QAction *, QString const &);
    };

  } // namespace dockwidgets
} // namespace AstroManager

#endif // DOCKWIDGETWEATHER

