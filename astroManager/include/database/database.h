//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:			          Database
// SUBSYSTEM:           Data support functions
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// LIBRARY DEPENDANCE:	Qt, citsio.
// NAMESPACE:						AstroManager::imaging
// AUTHOR:              Gavin BLakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2012-2018 Gavin Blakeman.
//                      This file is part of the Astronomy Manager software(astroManager)
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
// OVERVIEW:            Data support functions.
//
// CLASSES INCLUDED:    CDatabase
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                       - CDatabaseWeather
//
// HISTORY:             2018-04-13 GGB - Project Name changed to astroManager.
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2013-05-15 GGB - Split CDatabaseWeather into own file.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-24 GGB - Split CATID into it's own file
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-01-04 GGB - Classes developed for astroManager
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_DATABASE_H
#define ASTROMANAGER_DATABASE_H

#include "../Error.h"
#include "../qtExtensions/qt.h"

  // Standard header files

#include <list>
#include <map>
#include <string>

  // Astronomy Class ibrary

#include <ACL>

  // General class library

#include <GCL>

#ifdef _MSC_VER
# pragma warning( disable : 4290 )  /* Disable the warning about C++ exception specifications. */
#endif

namespace astroManager
{
  namespace database
  {
    const int ROLE_FILTERID  = Qt::UserRole + 0;

    class CDatabase : public QObject
    {
    public:
      enum
      {
        SQLDB_MYSQL,
        SQLDB_QOCI,
        SQLDB_SQLITE,
        SQLDB_PSQL,
        SQLDB_ODBC,
        SQLDB_TDS,
      };

    private:
    protected:
      QString szConnectionName;                     ///< Name of the connection.
      QSqlDatabase *dBase;                          ///< Pointer to the database driver.
      GCL::sqlwriter::CSQLWriter sqlWriter;         ///< The mapped SQL writer that the instances can customise and use.

      virtual bool createConnection(QString const &, QString const &, std::uint16_t, QString const &, QString const &, QString const &);
      virtual bool createConnectionODBC(QString const &, QString const &);
      virtual bool createConnectionSQLite(QString const &, QString const &);

      virtual bool ODBC() = 0;
      virtual bool Oracle() = 0;
      virtual bool MySQL() = 0;
      virtual bool SQLite() = 0;
      virtual bool PostgreSQL() = 0;

    public:
      class SDatabaseDriver
      {
      public:
        QString driverText;
        QString driverName;

        SDatabaseDriver(QString const &st1, QString const &st2) : driverText(st1), driverName(st2) {}
        SDatabaseDriver() {}
      };
      typedef std::map<int, SDatabaseDriver> TDatabaseDriverStorage;

      static TDatabaseDriverStorage databaseDrivers;

      static void initialiseDrivers();
      static QString getDriverName(int);
      static bool isDriverAvailable(int);

      CDatabase(QString const &);
      inline QSqlDatabase &database() { return *dBase;}

      virtual bool connectToDatabase(QString const &);
    };

    typedef std::list<ACL::SPTargetStellar> DStellarObject;

    //void RebuildConstellationNames(int = 0);


    const QString GetNameFromObject(QVariant *);
    bool ValidateObjectName(QString &);		// validates the name of an object against the database.
    QVariant GetObjectID(QString &);	// Gets an object ID when the name is known.
    void GetInstrumentDetails(long long, float &, float &, long &, long &, float &);
    void GetFilterDetails(const long long, std::string &, std::string &);

    void CreateRecord_TBL_SITES(QSqlRecord &);
    void CreateRecord_TBL_OBSERVERS(QSqlRecord &);
    void CreateRecord_TBL_INSTRUMENTS(QSqlRecord &);
    void CreateRecord_TBL_TELESCOPES(QSqlRecord &);
    void CreateRecord_TBL_CATALOGORDER(QSqlRecord &);
    void CreateRecord_TBL_LEAPSECONDS(QSqlRecord &);

      // WEATHER Database Records

    void CreateRecord_TBL_DAYSUMMARY(QSqlRecord &);
    void CreateRecord_TBL_ARCHIVE(QSqlRecord &);

    void CreateObserver(QVariant const &, QVariant const &, bool, QVariant &);

  } // namespace database

} // namespace AstroManager

#endif  // astroManager_DATABASE_H
