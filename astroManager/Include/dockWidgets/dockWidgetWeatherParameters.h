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

#ifndef DOCKWIDGETWEATHERPARAMETERS
#define DOCKWIDGETWEATHERPARAMETERS

#include "dockWidgetWeather.h"

namespace AstroManager
{
  namespace dockwidgets
  {
    class CDockWidgetWeatherParameters : public CDockWidgetWeather
    {
      Q_OBJECT

    private:
      void setupUI();

    protected:
    public:
      CDockWidgetWeatherParameters(QWidget *, QAction *);

    };

  } // namespace dockwidgets
} // namespace AstroManager

#endif // DOCKWIDGETWEATHERPARAMETERS

