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
//                      This file is part of the AstroManager software.
//
//                      AstroManager is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      AstroManager is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with AstroManager.  If not,
//                      see <http://www.gnu.org/licenses/>.
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

namespace AstroManager
{
  namespace windowPlanning
  {

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
      //      ASSOCIATE_CONTROL(dateTimeEditStart, formWidget, "dateTimeEditStart", QDateTimeEdit);
      //      ASSOCIATE_CONTROL(dateTimeEditEnd, formWidget, "dateTimeEditEnd", QDateTimeEdit);
      //      ASSOCIATE_CONTROL(groupBoxAutoUpdate, formWidget, "groupBoxAutoUpdate", QGroupBox);


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

        // Create signal/slot connections

      connect(comboBoxPlans, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedPlans(int)));
    }
  }
}
