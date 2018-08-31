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


#ifndef DOCKWIDGETWEATHERSCALE
#define DOCKWIDGETWEATHERSCALE

#include "dockWidgetWeather.h"


#include <QCL>

namespace astroManager
{
  namespace dockwidgets
  {
    class CDockWidgetWeatherScale: public CDockWidgetWeather
    {
      Q_OBJECT

    private:
      QRadioButton *radioButtonExtentDay;
      QRadioButton *radioButtonExtentMonth;
      QRadioButton *radioButtonExtentYear;

      QRadioButton *radioButtonMajorHour;
      QRadioButton *radioButtonMajorDay;
      QRadioButton *radioButtonMajorWeek;
      QRadioButton *radioButtonMajorMonth;

      QRadioButton *radioButtonMinorMinute;
      QRadioButton *radioButtonMinorHour;
      QRadioButton *radioButtonMinorDay;
      QRadioButton *radioButtonMinorWeek;

      void setupUI();

    protected:
    public:
      CDockWidgetWeatherScale(QWidget *,QAction *);

    };


  } // namespace dockwidgets

} // namespace AstroManager

#endif // DOCKWIDGETWEATHERSCALE

