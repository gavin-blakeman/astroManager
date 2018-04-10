//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:                queryModelPlanning
// SUBSYSTEM:           Query Model for the planning window.
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

#ifndef QUERYMODELPLANNING
#define QUERYMODELPLANNING

#include "sqlQueryModel.h"

#include <GCL>

namespace AstroManager
{
  namespace QTE
  {
    class CQueryModelPlanning : public CSQLQueryModel
    {
      Q_OBJECT

    public:
      enum
      {
        id_c = 0,
        images_c,
        name_c,
        type_c,
        ra_c,
        dec_c,
        altitude_c,
        azimuth_c,
        airmass_c,
        appMag_c,
        constellation_c,
        extinction_c,
        hourAngle_c,
        magnitude_c,
        observationCount_c,
        opposition_c,
        riseTime_c,
        setTime_c,
        transitTime_c,
        transitAltitude_c,
        angularSize_c,
        catalogue_c,
      };

      std::uint32_t databasePlanID = 0;

    private:
      CQueryModelPlanning(CQueryModelPlanning const &) = delete;
      GCL::sqlwriter::CSQLWriter sqlWriter_;

    protected:
    public:
      CQueryModelPlanning();
      virtual ~CQueryModelPlanning() {}

      virtual QVariant data(QModelIndex const &item, int role = Qt::DisplayRole) const;
      virtual void resetQuery();

      virtual GCL::sqlwriter::CSQLWriter &sqlWriter() { return sqlWriter_; }
    };

  } // namespace QTE
} // namespace AstroManager

#endif // QUERYMODELPLANNING

