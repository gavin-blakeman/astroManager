//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:                planningModel
// SUBSYSTEM:           SQL Model to support the planning window.
// LANGUAGE:            C++
// TARGET OS:           WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:  Qt
// NAMESPACE:           AstroManager::models
// AUTHOR:              Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2020 Gavin Blakeman.
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
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:
//
//
// HISTORY:             2020-09-15 GGB - File Created.
//
//*********************************************************************************************************************************

#include "include/models/planningModel.h"

  // Standard C++ library header files

#include <string>

  // Miscellaneous libray header files

#include "boost/locale.hpp"
#include <GCL>

namespace astroManager
{
  namespace models
  {
    enum
    {
      column_start = 0,
      column_rank = 0,
      column_name,
      column_type,
      column_ra,
      column_dec,
      column_altitude,
      column_azimuth,
      column_airmass,
      column_appMag,
      column_constellation,
      column_extinction,
      column_hourAngle,
      column_magnitude,
      column_observationCount,
      column_opposition,
      column_riseTime,
      column_setTime,
      column_transitTime,
      column_transitAltitude,
      column_angularSize,
      column_catalogue,
      column_end,              // Always leave this one at the end as it contains the total column count.
    };

    static std::vector<std::string> columnNames = {"Rank", "Name", "Type", "RA", "Dec", "Alt", "Az", "Airmass",
                                                   "Apparent Magniture", "Constellation", "Extinction", "Hour Angle", "Magitude",
                                                   "Observation Count", "Opposition", "Rise Time", "Set Time", "Transit Time",
                                                   "Transit Altitude", "Angular Size", "Catalogue"};

    /// @brief      Class constructor.
    /// @param[in]  parent: The parent object.
    /// @param[in]  dbARID: Pointer to the ARID database instance.
    /// @param[in]  dbATID: Pointer to the ATID database instance.
    /// @throws     std::bad_alloc
    /// @version    2020-09-15/GGB - Function created.

    CPlanningModel::CPlanningModel(QObject *parent, planID_t const &pi, database::CARID *dbARID, database::CATID *dbATID,
                                   ACL::CAstroTime const &tm, ACL::CGeographicLocation const &gl, ACL::CWeather const &wt)
      : QAbstractTableModel(parent), planID(pi), planTargets_(), databaseARID(dbARID), databaseATID(dbATID), currentTime_(tm),
        observingSite_(gl), observationWeather_(wt)
    {

    }

    /// @brief      Returns the number of columns in the model. This is a fixed number.
    /// @param[in]  parent: Not used.
    /// @returns    The number of columns in the model.
    /// @throws
    /// @version    2020-09-16/GGB - Function created.

    int CPlanningModel::columnCount(QModelIndex const &/*parent*/) const
    {
      return columnNames.size();
    }

    /// @brief      Returns the requested data from the model.
    ///

    QVariant CPlanningModel::data(QModelIndex const &index, int role) const
    {
      QVariant returnValue;

      switch (role)
      {
        case Qt::DisplayRole:
        {
            // Display the data (QString).

          switch(index.column())
          {
            case column_rank:
            {
              //returnValue = QVariant(planTargets_.at(index.row())->name());
              break;
            };
            case column_name:
            {
              returnValue = QVariant(planTargets_.at(index.row())->name());
              break;
            };
            case column_type:
            {
              returnValue = QVariant(planTargets_.at(index.row())->type());
              break;
            };
            case column_ra:
            {
              returnValue = QVariant(planTargets_.at(index.row())->RA());
              break;
            };
            case column_dec:
            {
              returnValue = QVariant(planTargets_.at(index.row())->DEC());
              break;
            };
            case column_altitude:
            {
              returnValue = QVariant(planTargets_.at(index.row())->Altitude());
              break;
            };
            case column_azimuth:
            {
              returnValue = QVariant(planTargets_.at(index.row())->Azimuth());
              break;
            };
          };
        };
        case Qt::BackgroundRole:
        {

        };
        case Qt::TextAlignmentRole:
        {

        };
        case Qt::ForegroundRole:
        {

        };
      };

      return returnValue;
    }

    Qt::ItemFlags CPlanningModel::flags(const QModelIndex &index) const
    {
      //return Qt::DisplayRole;
    }

    /// @brief      Returns the column headings.
    /// @param[in]  section:
    /// @param[in]  orientation:
    /// @param[in]  role:
    /// @returns
    /// @throws
    /// @version    2020-09-16/GGB - Function created.

    QVariant CPlanningModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
      QVariant returnValue;

      switch (role)
      {
        case Qt::DisplayRole:
        {
          switch (section)
          {
            returnValue = QString::fromStdString(columnNames[section]);
          };
          break;
        };
      };

      return std::move(returnValue);
    }

    /// @brief      Returns the number of rows in the model. The query is only executed once. In all other cases, the stored value
    ///             is returned.
    /// @param[in]  parent: Not used.
    /// @returns    The number of rows in the model.
    /// @throws
    /// @version    2020-09-16/GGB - Function created.

    int CPlanningModel::rowCount(QModelIndex const &/*parent*/) const
    {
      int returnValue = 0;

      if (rowCountValid_)
      {
        returnValue = rowCount_;
      }
      else
      {
          // Need to run the query and count the rows.

        QSqlQuery sqlQuery(databaseARID->database());
        GCL::sqlWriter sqlWriter;

        sqlWriter.resetQuery();
        sqlWriter.select({}).count("*").from({"TBL_TARGETS"}).where("PLAN_ID", "=", planID);

        if (sqlQuery.exec(QString::fromStdString(sqlWriter.string())))
        {
          sqlQuery.first();
          if (sqlQuery.isValid())
          {
            returnValue = rowCount_ = sqlQuery.value(0).toInt();
            rowCountValid_ = true;
          }
          else
          {
              // Query is not valid. Pretty serious error

            databaseARID->processErrorInformation(sqlQuery);
          };
        }
        else
        {
          //processErrorInformation();
        };
      };

      return returnValue;
    }
  }
}
