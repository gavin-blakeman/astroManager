//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								targetAstronomy
// SUBSYSTEM:						Display weather data
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	ACL, Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman (GGB)
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:
//
// HISTORY:             2018-08-30 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../include/ACL/targetAstronomy.h"

  // Standard C++ library header files

#include <utility>

  // astroManager application header files

#include "../../include/database/databaseATID.h"
#include "../../include/error.h"

  // Miscellaneous library header files

#include "boost/algorithm/string.hpp"

namespace astroManager
{

  /// @brief Constructor for the class. Also set the size of the vector.
  /// @param[in] parent: The table widget associated with this target.
  /// @param[in] newTarget: The target to assign to this instance.
  /// @throws std::bad_alloc
  /// @version 2018-08-31/GGB - Function created

  CTargetAstronomy::CTargetAstronomy(std::unique_ptr<ACL::CTargetAstronomy>newTarget)
    : columnMap(column_end), targetAstronomy_(std::move(newTarget))
  {

  }

  /// @brief Creates and sets the widgets for the item.
  /// @param[in] tableWidget: The table widget to populate.
  /// @throws None.
  /// @version 2018-08-31/GGB - Function created.

  void CTargetAstronomy::setColumnWidgets(QTableWidget *tableWidget)
  {
    columnMap.clear();
    int row = tableWidget->rowCount();

    tableWidget->insertRow(row);

    for (int index = column_start; index != column_end; index++)
    {
      switch (index)
      {
        case column_rank:
        {
          columnMap.push_back(new QTableWidgetItem(QString("%1").arg(row)));
          break;
        };
        default:
        {
          columnMap.push_back(new QTableWidgetItem("Test"));
          break;
        };
      };

      tableWidget->setItem(row, index, columnMap.back());
    };

    updateAllColumnValues();
  }

  /// @brief Returns a pointer to the managed object
  /// @returns Raw pointer to the managed object.

  ACL::CTargetAstronomy *CTargetAstronomy::targetAstronomy() const
  {
    return targetAstronomy_.get();
  }

  /// @brief Updates the widget with the object name.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnName()
  {
    columnMap[column_name]->setText(QString::fromStdString(targetAstronomy_->objectName()));
  }

  /// @brief Updates the widget with the type of the object.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnType()
  {
    switch (targetAstronomy_->targetType())
    {
      case ACL::CTargetAstronomy::TT_STELLAR:
      {
        columnMap[column_type]->setText(QString::fromStdString(
                                          dynamic_cast<ACL::CTargetStellar *>(targetAstronomy())->stellarType()));
        break;
      };
      case ACL::CTargetAstronomy::TT_MAJORPLANET:
      {
        columnMap[column_type]->setText("Planet");
        break;
      };
      case ACL::CTargetAstronomy::TT_MINORPLANET:
      {
        columnMap[column_type]->setText("Minor Planet");
        break;
      };
      case ACL::CTargetAstronomy::TT_COMET:
      {
        columnMap[column_type]->setText("Comet");
        break;
      };
      case ACL::CTargetAstronomy::TT_NONE:
      default:
      {
        CODE_ERROR;
        break;
      };
    };
  }

  /// @brief Sets the RA value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnRA()
  {

  }

  /// @brief Sets the DEC value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnDEC()
  {

  }

  /// @brief Sets the Altitude value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnAltitude()
  {

  }

  /// @brief Sets the Azimuth value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnAzimuth()
  {

  }

  /// @brief Sets the Airmass value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnAirmass()
  {

  }

  /// @brief Sets the Apparent Magnitude value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  void CTargetAstronomy::updateColumnApparentMagnitude()
  {

  }

  /// @brief Sets the Constellation value into the widget.
  /// @throws Exceptions from called functions.
  /// @version 2018-09-29/GGB - Function created.

  void CTargetAstronomy::updateColumnConstellation()
  {
    if (targetAstronomy()->targetType() == ACL::CTargetAstronomy::TT_STELLAR)
    {
      std::string constellationName;

      database::databaseATID->queryConstellationByName(targetAstronomy()->objectName(), constellationName);

      columnMap[column_constellation]->setText(QString::fromStdString(constellationName));
    };
  }

  void CTargetAstronomy::updateColumnExtinction()
  {

  }
  void CTargetAstronomy::updateColumnHourAngle()
  {

  }
  void CTargetAstronomy::updateColumnMagnitude()
  {

  }
  void CTargetAstronomy::updateColumnObservationCount()
  {

  }
  void CTargetAstronomy::updateColumnRiseTime()
  {

  }
  void CTargetAstronomy::updateColumnSetTime()
  {

  }
  void CTargetAstronomy::updateColumnTransitTime()
  {

  }
  void CTargetAstronomy::updateColumnTransitAltitude()
  {

  }
  void CTargetAstronomy::updateColumnAngularCatalogue()
  {

  }

  /// @brief Update the widget text for all columns.
  /// @throws None.
  /// @version 2018-04-03/GGB - Function created.

  void CTargetAstronomy::updateAllColumnValues()
  {
    updateColumnName();
    updateColumnType();
    updateColumnRA();
    updateColumnDEC();
    updateColumnAltitude();
    updateColumnAzimuth();
    updateColumnAirmass();
    updateColumnApparentMagnitude();
    updateColumnConstellation();
    updateColumnExtinction();
    updateColumnHourAngle();
    updateColumnMagnitude();
    updateColumnObservationCount();
    updateColumnRiseTime();
    updateColumnSetTime();
    updateColumnTransitTime();
    updateColumnTransitAltitude();
    updateColumnAngularCatalogue();
  }

} // namespace astroManager
