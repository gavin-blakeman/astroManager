//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowWeather
// SUBSYSTEM:						Display weather data
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Boost, libAstroClass, Qt
// NAMESPACE:						AstroManager::imageing
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2016-2017 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CWindowWeatherHistory         - Window for displaying weather data.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CMdiSubWindow
//                          - CWindowWeather
//
// HISTORY:             2016-02-02 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef WINDOWWEATHER
#define WINDOWWEATHER

#include "../qtExtensions/MdiSubWindow.h"

namespace AstroManager
{
  namespace weather
  {

    class CWindowWeather : public CMdiSubWindow
    {
    public:
      CWindowWeather(QWidget *parent) : CMdiSubWindow(parent) {}

      virtual EWindowClass getWindowClass() const { return WC_WEATHER; }
    };

  } // namespace weather
} // namespace AstroManager

#endif // WINDOWWEATHER

