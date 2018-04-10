﻿//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:			          DatabaseWeather
// SUBSYSTEM:           Weather data support functions
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// AUTHOR:              Gavin BLakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2012-2017 Gavin Blakeman.
//                      This file is part of the Astronomical Image Reduction and Data Analysis Software (AIRDAS)
//
//                      AIRDAS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      AIRDAS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with AIRDAS.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Data support functions.
//
// CLASSES INCLUDED:    CDatabaseWeather
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                       - CDatabaseWeather
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-05-15 GGB - Split CDatabaseWeather into own file.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-02-24 GGB - Split CATID into it's own file
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2012-01-04 GGB - Classes developed for AIRDAS
//
//*********************************************************************************************************************************

#ifndef AIRDAS_DATABASEWEATHER_H
#define AIRDAS_DATABASEWEATHER_H

#include "database.h"

namespace AstroManager
{
  namespace database
  {
    class CDatabaseWeather : public CDatabase
    {
    private:
      bool WDdisabled_;           ///< Flag to determine if the weather database is disabled.

      virtual bool ODBC();
      virtual bool Oracle();
      virtual bool MySQL();
      virtual bool SQLite();
      virtual bool PostgreSQL();

    protected:
    public:
      CDatabaseWeather();
      virtual ~CDatabaseWeather();

      void connectToDatabase();

      bool enabled() const { return !WDdisabled_; }

      void populateComboBoxWeatherStations(QComboBox *);
    };

    extern CDatabaseWeather *databaseWeather;

  }  // namespace database.
}  // namespace AstroManager

#endif // AIRDAS_DATABASEWEATHER_H_
