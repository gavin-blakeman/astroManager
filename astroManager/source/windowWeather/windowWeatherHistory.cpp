//*********************************************************************************************************************************
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

#include "../../include/windowWeather/windowWeatherHistory.h"

#include "../../include/database/databaseWeather.h"

// QWT Library

//#include <qwt_abstract_scale_draw.h>
//#include <qwt_scale_draw.h>
//#include <qwt_series_data.h>
//#include "qwt_scale_engine.h"

namespace astroManager
{
  namespace weather
  {
    /// @brief Constructor for the class.
    /// @param[in] parent - The window that owns this instance.
    /// @version 2017-06-20/GGB - Function created.

    CWindowWeatherHistory::CWindowWeatherHistory(QWidget *parent) : CWindowWeather(parent)
    {
      setAttribute(Qt::WA_DeleteOnClose);

      setupUI();

      setWindowTitle(tr("Weather History"));
    }

    /// @brief Function to setup the user interface.
    /// @details Called from the constructor of the class.
    /// @throws GCL::CRuntimeAssert
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-06-29/GGB - Function created.

    void CWindowWeatherHistory::setupUI()
    {
//      QUiLoader loader;
//      QGridLayout *gridLayout;

//         // Create the window details from the template

//      QFile file(":/forms/windowWeatherHistory.ui");

//      if (!file.open(QFile::ReadOnly))
//      {
//        ERRORMESSAGE("Unable to open resource :/forms/windowWeatherHistory.ui.");
//        ERROR(astroManager, 0x0001);
//      }

//      QWidget *formWidget = loader.load(&file, this);
//      file.close();

//      setWidget(formWidget);

//      ASSOCIATE_CONTROL(comboBoxWeatherStation, formWidget, "comboBoxWeatherStation", QComboBox);
//      ASSOCIATE_CONTROL(dateTimeEditStart, formWidget, "dateTimeEditStart", QDateTimeEdit);
//      ASSOCIATE_CONTROL(dateTimeEditEnd, formWidget, "dateTimeEditEnd", QDateTimeEdit);
//      ASSOCIATE_CONTROL(groupBoxAutoUpdate, formWidget, "groupBoxAutoUpdate", QGroupBox);
//      ASSOCIATE_CONTROL(spinBoxUpdateInterval, formWidget, "spinBoxUpdateInterval", QSpinBox);

//      if (!gridLayout = dynamic_cast<QGridLayout *>(formWidget->layout()))
//      {
//        CODE_ERROR;
//      };

//        // Set the end time to the time now.

//      dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

//        // Populate the list of weather stations.

//      database::databaseWeather->populateComboBoxWeatherStations(comboBoxWeatherStation);

      histogramPlot = new QwtPlot();
    }


  }
}
