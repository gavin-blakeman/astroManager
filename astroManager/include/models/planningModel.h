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
// CLASSES INCLUDED:    CPlanningModel
//
// CLASS HIERARCHY:     QAbstractTableModel
//                        - CPlanningModel
//
// HISTORY:             2020-09-15 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef PLANNINGMODEL_H
#define PLANNINGMODEL_H

  // C++ library header files.

#include <map>
#include <memory>

  // Miscellaneous library header files

#include <QCL>

  // astroManager header files

#include "include/astroManager.h"
#include "include/ACL/targetAstronomy.h"

namespace astroManager
{
  namespace models
  {
    /// @brief  CPlanningModel implements a table view model with deferred data access. Data is stored locally and only fetched from
    ///         the database as required. A mechanism is included to do a lookahead for data retrieval.
    /// @todo   Implement a threaded data retrieval model to fetch the data in a thread while the main thread returns.

    class CPlanningModel : public QAbstractTableModel
    {
    private:

      using targetsVector_t = std::vector<std::unique_ptr<CTargetAstronomy>>;

      planID_t const &planID;
      mutable std::map<std::uint64_t, std::unique_ptr<CTargetAstronomy>> recordCache; ///< Use a map as it can be sparse rather than full.
      mutable std::uint64_t cacheStartIndex;
      std::uint64_t cacheMaximumSize = 16384;       ///< Limit to around 10MB of data.
      std::uint64_t cacheReadRecords = 1024;        ///< Number of records to read at a time.
      std::uint64_t recordCount;                    ///< Number of records in the current recordSet.

      mutable int rowCount_ = 0;
      mutable bool rowCountValid_ = false;
      ACL::CAstroTime const &currentTime_;
      ACL::CGeographicLocation const &observingSite_;
      ACL::CWeather const &observationWeather_;

      CPlanningModel() = delete;
      CPlanningModel(CPlanningModel const &) = delete;
      CPlanningModel(CPlanningModel &&) = delete;
      CPlanningModel &operator=(CPlanningModel const &) = delete;

      void loadData(int) const;

    protected:


    public:
      CPlanningModel(QObject *, planID_t const &, ACL::CAstroTime const &, ACL::CGeographicLocation const &, ACL::CWeather const &);

      int rowCount(QModelIndex const &parent = QModelIndex()) const override;
      int columnCount(QModelIndex const &parent = QModelIndex()) const override;
      QVariant data(QModelIndex const &index, int role = Qt::DisplayRole) const override;
      QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
      Qt::ItemFlags flags(const QModelIndex &index) const override;
    };
  }   // namespace models
}   // namespace astroManager


#endif // PLANNINGMODEL_H
