//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:                windowPlanning
// SUBSYSTEM:           Window for observation planning.
// LANGUAGE:            C++
// TARGET OS:           WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:  Qt
// NAMESPACE:           AstroManager::widgets
// AUTHOR:              Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CWindowPlanning
//
// CLASS HIERARCHY:     CMdiSubWindow
//                        - CWindowPlanning
//
// HISTORY:             2018-02-03 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../Include/windowPlanning/windowPlanning.h"

#include "../../Include/database/databaseARID.h"
#include "../../Include/Settings.h"

namespace AstroManager
{
  namespace windowPlanning
  {
    std::uint_least8_t const TIME_LT = 0;
    std::uint_least8_t const TIME_UT = 1;
    std::uint_least8_t const TIME_LST = 2;

    /// @brief Constructor for the class.
    /// @param[in] parent - The window that owns this instance.
    /// @version 2017-06-20/GGB - Function created.

    CWindowPlanning::CWindowPlanning(QWidget *parent) : CMdiSubWindow(parent), planTargets()
    {
      setAttribute(Qt::WA_DeleteOnClose);

      setupUI();

      setWindowTitle(tr("Observation Planning"));
    }

    /// @brief Function called when a different Observing Plan is selected.
    /// @param[in] currentIndex - The new items index. (Zero based)
    /// @throws None.
    /// @version 2018-02-13/GGB - Function created.

    void CWindowPlanning::currentIndexChangedPlans(int currentIndex)
    {
        // Clear the current list of objects.

      planTargets.clear();
    }

    /// @brief Respond to the LT Radio button being pressed.
    /// @param[in] checked - Is the button in the checked state.
    /// @throws None.
    /// @version 2018-04-15/GGB - Function created.

    void CWindowPlanning::radioButtonLTClicked(bool checked)
    {
      if (checked)
      {
        settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_TIMESCALE, TIME_LT);
      }
    }

    /// @brief Respond to the UT Radio button being pressed.
    /// @param[in] checked - Is the button in the checked state.
    /// @throws None.
    /// @version 2018-04-15/GGB - Function created.

    void CWindowPlanning::radioButtonUTClicked(bool checked)
    {
      if (checked)
      {
        settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_TIMESCALE, TIME_UT);
      }
    }

    /// @brief Respond to the LST Radio button being pressed.
    /// @param[in] checked - Is the button in the checked state.
    /// @throws None.
    /// @version 2018-04-15/GGB - Function created.

    void CWindowPlanning::radioButtonLSTClicked(bool checked)
    {
      if (checked)
      {
        settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_TIMESCALE, TIME_LST);
      }
    }

    /// @brief Sets up the user interface for the class.
    /// @throws GCL::CRuntimeError(AstroManager)
    /// @version 2018-02-03/GGB - Function created.

    void CWindowPlanning::setupUI()
    {
      QUiLoader loader;
      QGridLayout *gridLayout;

        // Create the window details from the template

      QFile file(":/windows/windowPlanning.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/windows/windowPlanning.ui.");
        ERROR(AstroPlanner, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      ASSOCIATE_CONTROL(comboBoxPlans, formWidget, "comboBoxPlans", QComboBox);
      ASSOCIATE_CONTROL(comboBoxSites, formWidget, "comboBoxSites", QComboBox);

      ASSOCIATE_CONTROL(tableViewPlanning, formWidget, "tableViewPlanning", QTableView);
      ASSOCIATE_CONTROL(dateEditSelectedDate, formWidget, "dateEditSelectedDate", QDateEdit);
      ASSOCIATE_CONTROL(timeEditSelectedTime, formWidget, "timeEditSelectedTime", QTimeEdit);
      ASSOCIATE_CONTROL(radioButtonUT, formWidget, "radioButtonUT", QRadioButton);
      ASSOCIATE_CONTROL(radioButtonLT, formWidget, "radioButtonLT", QRadioButton);
      ASSOCIATE_CONTROL(radioButtonST, formWidget, "radioButtonST", QRadioButton);
      ASSOCIATE_CONTROL(pushButtonTimeMinutePlus, formWidget, "pushButtonTimeMinutePlus", QPushButton);
      ASSOCIATE_CONTROL(pushButtonTimeMinuteMinus, formWidget, "pushButtonTimeMinuteMinus", QPushButton);
      ASSOCIATE_CONTROL(pushButtonTimeHourPlus, formWidget, "pushButtonTimeHourPlus", QPushButton);
      ASSOCIATE_CONTROL(pushButtonTimeHourMinus, formWidget, "pushButtonTimeHourMinus", QPushButton);
      ASSOCIATE_CONTROL(pushButtonTimeDayPlus, formWidget, "pushButtonTimeDayPlus", QPushButton);
      ASSOCIATE_CONTROL(pushButtonTimeDayMinus, formWidget, "pushButtonTimeDayMinus", QPushButton);
      ASSOCIATE_CONTROL(pushButtonRealTime, formWidget, "pushButtonRealTime", QPushButton);


      //      if (!gridLayout = dynamic_cast<QGridLayout *>(formWidget->layout()))
      //      {
      //        CODE_ERROR(AIRDAS);
      //      };

      //        // Set the end time to the time now.

      //      dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

      //        // Populate the list of weather stations.

      //      database::databaseWeather->populateComboBoxWeatherStations(comboBoxWeatherStation);

        // Populate combo boxes

      database::databaseARID->populateComboSite(comboBoxSites, true);
      database::databaseARID->populateComboObservingPlans(comboBoxPlans, true);

        // Setup initial values.

      switch (settings::astroManagerSettings->value(settings::WINDOWPLANNING_TIMESCALE, TIME_UT).toInt())
      {
        case TIME_UT:
        {
          radioButtonUT->setChecked(true);
          break;
        };
        case TIME_LT:
        {
          radioButtonLT->setChecked(true);
          break;
        };
        case TIME_LST:
        {
          radioButtonST->setChecked(true);
          break;
        };
        default:
        {
          settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_TIMESCALE, TIME_UT);
          radioButtonUT->setChecked(true);
          break;
        };
      }



        // Create signal/slot connections

      connect(comboBoxPlans, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedPlans(int)));
      connect(radioButtonLT, SIGNAL(clicked(bool)), this, SLOT(radioButtonLTClicked(bool)));
      connect(radioButtonUT, SIGNAL(clicked(bool)), this, SLOT(radioButtonUTClicked(bool)));
      connect(radioButtonST, SIGNAL(clicked(bool)), this, SLOT(radioButtonLSTClicked(bool)));
    }
  }
}
