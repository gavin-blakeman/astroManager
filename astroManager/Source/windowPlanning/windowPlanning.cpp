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

    CWindowPlanning::CWindowPlanning(QWidget *parent) : CMdiSubWindow(parent),
      queryModel(settings::astroManagerSettings->value(settings::WINDOWPLANNING_LASTPLAN, 0).toUInt()), planTargets()
    {
      setAttribute(Qt::WA_DeleteOnClose);

        // Create the timer.

      timer1s = new QTimer(this);
      connect(timer1s, SIGNAL(timeout()), this, SLOT(eventTimer1s()));

      setupUI();

      setWindowTitle(tr("Observation Planning"));
    }

    /// @brief Responds when the observing site is changed.
    /// @param[in] index - The new current index.
    /// @throws None.
    /// @version 2018-04-15/GGB - Function created.

    void CWindowPlanning::comboBoxSiteCurrentIndexChanged(int)
    {
        // Update the time zone offset.

      database::databaseARID->getTimeZoneOffset(comboBoxSites->currentData().toInt(), &timeZoneOffset);
      timeZoneOffset *= 60 * 60;  // Convert to seconds.
    }

    /// @brief Function called when a different Observing Plan is selected.
    /// @param[in] currentIndex - The new items index. (Zero based)
    /// @throws None.
    /// @version 2018-02-13/GGB - Function created.

    void CWindowPlanning::currentIndexChangedPlans(int currentIndex)
    {
        // Save the new index as the default.

      settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_LASTPLAN, comboBoxPlans->currentData());

        // Clear the current list of objects.

      planTargets.clear();
    }

    /// @brief Responds to the 1s timer when triggered to update the time in the window and any other information required.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2018-04-15/GGB - Function created.

    void CWindowPlanning::eventTimer1s()
    {
      if (radioButtonLT->isChecked())
      {
        QDateTime localTime = QDateTime::currentDateTime().toOffsetFromUtc(timeZoneOffset);
        dateEditSelectedDate->setDate(localTime.date());
        timeEditSelectedTime->setTime(localTime.time());
      }
      else if (radioButtonUT->isChecked())
      {
        QDateTime universalTime= QDateTime::currentDateTimeUtc();
        dateEditSelectedDate->setDate(universalTime.date());
        timeEditSelectedTime->setTime(universalTime.time());
      }
      else if (radioButtonST->isChecked())
      {

      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Responds to the Real Time push button being clicked.
    /// @param[in] checked - Checked state of the button.
    /// @throws None.
    /// @version 2018-04-15/GGB - Function created.

    void CWindowPlanning::pushButtonRealTimeClicked(bool checked)
    {
      if (checked)
      {
        pushButtonTimeDayMinus->setEnabled(false);
        pushButtonTimeDayPlus->setEnabled(false);
        pushButtonTimeHourMinus->setEnabled(false);
        pushButtonTimeHourPlus->setEnabled(false);
        pushButtonTimeMinuteMinus->setEnabled(false);
        pushButtonTimeMinutePlus->setEnabled(false);

        timer1s->start(1000);

        settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_REALTIME, true);
      }
      else
      {
        timer1s->stop();

        pushButtonTimeDayMinus->setEnabled(true);
        pushButtonTimeDayPlus->setEnabled(true);
        pushButtonTimeHourMinus->setEnabled(true);
        pushButtonTimeHourPlus->setEnabled(true);
        pushButtonTimeMinuteMinus->setEnabled(true);
        pushButtonTimeMinutePlus->setEnabled(true);

        settings::astroManagerSettings->setValue(settings::WINDOWPLANNING_REALTIME, false);
      }
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

      ASSOCIATE_TABLEVIEW(tableViewPlanning, formWidget, "tableViewPlanning");
      ASSOCIATE_CONTROL(dateEditSelectedDate, formWidget, "dateEditSelectedDate", QDateEdit);
      ASSOCIATE_CONTROL(timeEditSelectedTime, formWidget, "timeEditSelectedTime", QTimeEdit);
      ASSOCIATE_RADIOBUTTON(radioButtonUT, formWidget, "radioButtonUT");
      ASSOCIATE_RADIOBUTTON(radioButtonLT, formWidget, "radioButtonLT");
      ASSOCIATE_RADIOBUTTON(radioButtonST, formWidget, "radioButtonST");
      ASSOCIATE_PUSHBUTTON(pushButtonTimeMinutePlus, formWidget, "pushButtonTimeMinutePlus");
      ASSOCIATE_PUSHBUTTON(pushButtonTimeMinuteMinus, formWidget, "pushButtonTimeMinuteMinus");
      ASSOCIATE_PUSHBUTTON(pushButtonTimeHourPlus, formWidget, "pushButtonTimeHourPlus");
      ASSOCIATE_PUSHBUTTON(pushButtonTimeHourMinus, formWidget, "pushButtonTimeHourMinus");
      ASSOCIATE_PUSHBUTTON(pushButtonTimeDayPlus, formWidget, "pushButtonTimeDayPlus");
      ASSOCIATE_PUSHBUTTON(pushButtonTimeDayMinus, formWidget, "pushButtonTimeDayMinus");
      ASSOCIATE_PUSHBUTTON(pushButtonRealTime, formWidget, "pushButtonRealTime");

      tableViewPlanning->setModel(&queryModel);
      tableViewPlanning->setSortingEnabled(true);
      tableViewPlanning->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        // Populate combo boxes and select the last values selected.

      database::databaseARID->populateComboSite(comboBoxSites, true);
      database::databaseARID->populateComboObservingPlans(comboBoxPlans, true);

        // Get the time zone offset.

      database::databaseARID->getTimeZoneOffset(comboBoxSites->currentData().toInt(), &timeZoneOffset);
      timeZoneOffset *= 60 * 60;  // Convert to seconds.

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

      if (settings::astroManagerSettings->value(settings::WINDOWPLANNING_REALTIME, false).toBool())
      {
        pushButtonTimeDayMinus->setEnabled(false);
        pushButtonTimeDayPlus->setEnabled(false);
        pushButtonTimeHourMinus->setEnabled(false);
        pushButtonTimeHourPlus->setEnabled(false);
        pushButtonTimeMinuteMinus->setEnabled(false);
        pushButtonTimeMinutePlus->setEnabled(false);

        pushButtonRealTime->setChecked(true);

        timer1s->start(1000);
      }
      else
      {
        timer1s->stop();

        pushButtonTimeDayMinus->setEnabled(true);
        pushButtonTimeDayPlus->setEnabled(true);
        pushButtonTimeHourMinus->setEnabled(true);
        pushButtonTimeHourPlus->setEnabled(true);
        pushButtonTimeMinuteMinus->setEnabled(true);
        pushButtonTimeMinutePlus->setEnabled(true);

        pushButtonRealTime->setChecked(false);
      };

        // Create signal/slot connections

      connect(comboBoxPlans, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedPlans(int)));

      connect(comboBoxSites, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSiteCurrentIndexChanged(int)));

      connect(radioButtonLT, SIGNAL(clicked(bool)), this, SLOT(radioButtonLTClicked(bool)));
      connect(radioButtonUT, SIGNAL(clicked(bool)), this, SLOT(radioButtonUTClicked(bool)));
      connect(radioButtonST, SIGNAL(clicked(bool)), this, SLOT(radioButtonLSTClicked(bool)));
      connect(pushButtonRealTime, SIGNAL(clicked(bool)), this, SLOT(pushButtonRealTimeClicked(bool)));
    }
  }
}
