//*********************************************************************************************************************************
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

#include "../../Include/database/databaseWeather.h"

  // AIRDAS header files

#include "../../Include/Settings.h"

namespace AstroManager
{
  namespace database
  {
    CDatabaseWeather *databaseWeather = nullptr;

    //*****************************************************************************************************************************
    //
    // CdatabaseWeather
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the CDatabaseWeather class.
    /// @details The class reads the database key and calls the relevant setup routine.
    /// @throws None.
    /// @version 2017-06-21/GGB - Added sqlWriter information.
    /// @version 2017-06-20/GGB - Updated to reflect changes to CDatabase (Bug #69)
    /// @version 2013-05-15/GGB - Default to disabled.
    /// @version 2013-02-09/GGB - Added the _disabled member.
    /// @version 2013-01-26/GGB - Added SQLite support and moved code to connectToDatabase()
    /// @version 2011-08-20/GGB - Function created.

    CDatabaseWeather::CDatabaseWeather() : CDatabase("WEATHER"), WDdisabled_(false)
    {
      QVariant variant = settings::VSOPSettings->value(settings::WEATHER_DATABASE_DISABLE, QVariant());

      if (variant.isNull())
      {
        settings::VSOPSettings->setValue(settings::WEATHER_DATABASE_DISABLE, QVariant(true));
        WDdisabled_ = true;
      }
      else
      {
        WDdisabled_ = variant.toBool();
      }

      sqlWriter.createTable("TBL_SITE");
        sqlWriter.createColumn("TBL_SITE", "ID");
        sqlWriter.createColumn("TBL_SITE", "Description");
        sqlWriter.createColumn("TBL_SITE", "Latitude");
        sqlWriter.createColumn("TBL_SITE", "Longitude");
        sqlWriter.createColumn("TBL_SITE", "Altitude");
      sqlWriter.createTable("TBL_INSTRUMENT");
        sqlWriter.createColumn("TBL_INSTRUMENT", "ID");
        sqlWriter.createColumn("TBL_INSTRUMENT", "Description");
      sqlWriter.createTable("TBL_ARCHIVE");
        sqlWriter.createColumn("TBL_ARCHIVE", "ARCHIVE_ID");
        sqlWriter.createColumn("TBL_ARCHIVE", "SITE_ID");
        sqlWriter.createColumn("TBL_ARCHIVE", "INSTRUMENT_ID");
        sqlWriter.createColumn("TBL_ARCHIVE", "MJD");
        sqlWriter.createColumn("TBL_ARCHIVE", "TIME");
        sqlWriter.createColumn("TBL_ARCHIVE", "outsideTemp");
        sqlWriter.createColumn("TBL_ARCHIVE", "hiOutsideTemp");
        sqlWriter.createColumn("TBL_ARCHIVE", "lowOutsideTemp");
        sqlWriter.createColumn("TBL_ARCHIVE", "insideTemp");
        sqlWriter.createColumn("TBL_ARCHIVE", "barometer");
        sqlWriter.createColumn("TBL_ARCHIVE", "outsideHumidity");
        sqlWriter.createColumn("TBL_ARCHIVE", "insideHumidity");
        sqlWriter.createColumn("TBL_ARCHIVE", "rain");
        sqlWriter.createColumn("TBL_ARCHIVE", "hiRainRate");
        sqlWriter.createColumn("TBL_ARCHIVE", "windSpeed");
        sqlWriter.createColumn("TBL_ARCHIVE", "hiWindSpeed");
        sqlWriter.createColumn("TBL_ARCHIVE", "windDirection");
        sqlWriter.createColumn("TBL_ARCHIVE", "solarRad");
        sqlWriter.createColumn("TBL_ARCHIVE", "hiSolarRad");
        sqlWriter.createColumn("TBL_ARCHIVE", "UV");
        sqlWriter.createColumn("TBL_ARCHIVE", "hiUV");
    }

    /// @details Destructor for the class. Ensures that the database connection is removed.
    /// @throws None.
    /// @version 2013-05-15/GGB - Conditional on database not disabled.
    /// @version 2013-02-09/GGB - Conditional removal of the database connection.
    /// @version 2011-08-20/GGB

    CDatabaseWeather::~CDatabaseWeather()
    {
      if (!WDdisabled_)
      {
        if (dBase)
        {
          dBase->close();
          QSqlDatabase::removeDatabase(szConnectionName);
          delete dBase;
          dBase = nullptr;
        };
      };
    }

    /// @brief Connects to the database.
    /// @details Reads the database connection type and then calls the relevant database connection function.
    /// @throws None.
    /// @version 2017-07-01/GGB - Update logic and error handling. No longer throw errors, just disable the weather database.
    /// @version 2017-06-19/GGB - Remove redundant try...catch block.
    /// @version 2013-02-09/GGB - Added support for disabling the database.
    /// @version 2013-01-26/GGB - Function created.

    void CDatabaseWeather::connectToDatabase()
    {
      QVariant database = settings::VSOPSettings->value(settings::WEATHER_DATABASE_DBMS);
      QString szDatabase;

      if (!WDdisabled_)
      {

        if ( !database.isNull() )
        {
          szDatabase = database.toString();		// Get the database type

          if (!CDatabase::connectToDatabase(szDatabase))
          {
            INFOMESSAGE("Unable to connect to weather database. Disabling weather database.");
            WDdisabled_ = true;
          }
        }
        else
        {
          WARNINGMESSAGE("Setting " + settings::WEATHER_DATABASE_DBMS.toStdString() + "not found.");
          INFOMESSAGE("Unable to connect to weather database. Disabling weather database.");
          WDdisabled_ = true;
        };
      };
    }

    /// @brief Function for connecting to a MySQL database.
    /// @details Reads the information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to use new function CDatabase::createConnection()
    /// @version 2011-08-20/GGB - Function created.

    bool CDatabaseWeather::MySQL()
    {
      return ( createConnection(settings::VSOPSettings->value(settings::WEATHER_MYSQL_DRIVERNAME, QVariant(QString("QMYSQL"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_MYSQL_HOSTADDRESS, QVariant(QString("localhost"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_MYSQL_PORT, QVariant(3306)).toInt(),
                                settings::VSOPSettings->value(settings::WEATHER_MYSQL_DATABASENAME, QVariant(QString("WEATHER"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_MYSQL_USERNAME, QVariant(QString("WEATHER"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_MYSQL_PASSWORD, QVariant(QString("WEATHER"))).toString()) );
    }

    /// @brief Function for opening an ODBC database.
    /// @details Reads information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2011-08-20/GGB - Function created.

    bool  CDatabaseWeather::ODBC()
    {
      return ( createConnectionODBC(settings::VSOPSettings->value(settings::WEATHER_ODBC_DRIVERNAME, QVariant(QString("QODBC"))).toString(),
                                    settings::VSOPSettings->value(settings::WEATHER_ODBC_DATASOURCENAME, QVariant(QString("MSA ATID"))).toString()) );

    }

    /// @brief Function for opening an Oracle database.
    /// @details Reads the relevant information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to use new function CDatabase::createConnection()
    /// @version 2011-08-20/GGB - Function created.

    bool CDatabaseWeather::Oracle()
    {
      return ( createConnection(settings::VSOPSettings->value(settings::WEATHER_ORACLE_DRIVERNAME, QVariant(QString("QOCI"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_ORACLE_HOSTADDRESS, QVariant(QString("localhost"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_ORACLE_PORT, QVariant(1521)).toInt(),
                                settings::VSOPSettings->value(settings::WEATHER_ORACLE_SCHEMANAME, QVariant(QString("xe"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_ORACLE_USERNAME, QVariant(QString("ATID"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_ORACLE_PASSWORD, QVariant(QString("ATID"))).toString()) );
    }

    /// @brief Reads data from the database and then adds it into the passed combo box.
    /// @param[in] comboBox - Pointer to the combo box to populate with weather station names.
    /// @throws GCL::CRuntimeAssert(...)
    /// @version 2017-07-01/GGB - Function created.

    void CDatabaseWeather::populateComboBoxWeatherStations(QComboBox *comboBox)
    {
      RUNTIME_ASSERT(WCL, comboBox != nullptr, "parameter comboBox cannot be nullptr");

      if (!WDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        std::string sqlString = sqlWriter.select({"TBL_SITE.ID", "TBL_SITE.DESCRIPTION"}).from({"TBL_SITE"}).
            where({
                    GCL::sqlwriter::parameterTriple(std::string("DONOTUSE"), std::string("="), "false"),
                  }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
          // Iterate all the returned values and add to the combo box.

          while (query.next())
          {
            comboBox->addItem(query.value(1).toString());
          }
        }
        else
        {
          QSqlError error = dBase->lastError();

          ERRORMESSAGE("Error while executing query: " + sqlString);
          ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
          ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
          ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
        }
      }
      else
      {
        DEBUGMESSAGE("populateComboBoxWeatherStations: Weather Database is disbled.");
      }

    }

    /// @brief Function for opening an Postgre database.
    /// @details Reads the relevant information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to use new function CDatabase::createConnection()
    /// @version 2011-08-20/GGB - Function created.

    bool CDatabaseWeather::PostgreSQL()
    {
      return ( createConnection(settings::VSOPSettings->value(settings::WEATHER_POSTGRESQL_DRIVERNAME, QVariant(QString("QOCI"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_POSTGRESQL_HOSTADDRESS, QVariant(QString("localhost"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_POSTGRESQL_PORT, QVariant(1521)).toInt(),
                                settings::VSOPSettings->value(settings::WEATHER_POSTGRESQL_DATABASENAME, QVariant(QString("xe"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_POSTGRESQL_USERNAME, QVariant(QString("ATID"))).toString(),
                                settings::VSOPSettings->value(settings::WEATHER_POSTGRESQL_PASSWORD, QVariant(QString("ATID"))).toString()) );
    }

    /// Procedure to open SQLite database.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to use new function CDatabase::createConnection()
    /// @version 2013-01-25/GGB - Function created.

    bool CDatabaseWeather::SQLite()
    {
      return ( createConnectionSQLite(settings::VSOPSettings->value(settings::WEATHER_SQLITE_DRIVERNAME, QVariant(QString("QSQLITE"))).toString(),
                                      settings::VSOPSettings->value(settings::WEATHER_SQLITE_DATABASENAME, QVariant("Data/WEATHER.sqlite")).toString()) );
    }

  }  // namespace database
}    // namespace AstroManager
