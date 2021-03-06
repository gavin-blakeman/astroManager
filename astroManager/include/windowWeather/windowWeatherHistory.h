﻿//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowWeatherHistory
// SUBSYSTEM:						Display weather data from the weather database.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Boost, libAstroClass, Qt
// NAMESPACE:						AstroManager::imageing
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2016-2018 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CWindowWeatherHistory         - Window for displaying weather data.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CMdiSubWindow
//                          - CWindowWeatherHistory
//
// HISTORY:             2016-02-02 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_WINDOWSWEATHERHISTORY_H
#define ASTROMANAGER_WINDOWSWEATHERHISTORY_H

  // astroManager

#include "windowWeather.h"

  // Standard libraries

#include <memory>
#include <vector>

  // Miscellaneous library header files

#include <QCL>
#include <qwt_plot.h>
//#include <qwt_plot.h>


namespace astroManager
{
  namespace weather
  {
    class CWindowWeatherHistory : public CWindowWeather
    {
      Q_OBJECT

    private:
      QComboBox *comboBoxWeatherStation;
      QDateTimeEdit *dateTimeEditStart;
      QDateTimeEdit *dateTimeEditEnd;
      QGroupBox *groupBoxAutoUpdate;
      QSpinBox *spinBoxUpdateInterval;

      //QwtPlotCurve histogramData;
      QwtPlot *histogramPlot;

      void setupUI();

    protected:
    public:
      CWindowWeatherHistory(QWidget *parent = 0);

      virtual ESubWindowType getWindowType() const { return SWT_WEATHER_HISTORY; }
      virtual bool save() {}
      virtual bool saveAs() {}

    public slots:
      virtual void windowActivating() {}
    };

  } // namespace weather

}   // namespace AstroManager

#endif // astroManager_WINDOWSWEATHERHISTORY_H
