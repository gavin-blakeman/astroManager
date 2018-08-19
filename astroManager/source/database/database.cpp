//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								Database
// SUBSYSTEM:           Data support functions
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// LIBRARY DEPENDANCE:	Qt, cfitsio.
// NAMESPACE:						AstroManager::imaging
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2012-2018 Gavin Blakeman.
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
// OVERVIEW:            Data support functions.
//
// CLASSES INCLUDED:    CDatabase
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                       - CdatabaseWeather
//
// HISTORY:             2018-04-13 GGB - Project Name changed to astroManager.
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-05-15 GGB - Split CDatabaseWeather into own file.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-24 GGB - Split CATID into it's own file
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-01-04 GGB - Classes developed for astroManager
//
//*********************************************************************************************************************************

#include "../../include/database/database.h"

  // astroManager files

#include "../../include/Settings.h"
#include "../../include/astroManager.h"

  // Qt Library

#include <QtSql/QtSql>

  // Boost library headers

#include <boost/algorithm/string.hpp>

namespace astroManager
{
  namespace database
  {
    CDatabase::TDatabaseDriverStorage CDatabase::databaseDrivers;

    //*****************************************************************************************************************************
    //
    // Class CDatabase
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the database class.
    /// @param[in] connectionName - The name of the connection associated with the database.
    /// @throws None
    /// @version 2017-06-20/GGB - Function created.

    CDatabase::CDatabase(QString const &connectionName) : szConnectionName(connectionName), dBase(nullptr)
    {
    }

    /// @brief Creates a connection to the database by calling the relevant functions.
    /// @param[in] szDatabase - Database driver string to use.
    /// @returns true - Connection created
    /// @returns false - The driver type is not supported.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    bool CDatabase::connectToDatabase(QString const &szDatabase)
    {
      bool returnValue = false;

      if (szDatabase == "Oracle")
      {
        if (isDriverAvailable(SQLDB_QOCI))
        {
          Oracle();
          returnValue = true;
        }
        else
        {
          WARNINGMESSAGE("The database driver QOCI is not available or has not been loaded.");
        }
      }
      else if (szDatabase == "ODBC")
      {
        if (isDriverAvailable(SQLDB_ODBC))
        {
          returnValue = ODBC();
        }
        else
        {
          WARNINGMESSAGE("The database driver QODBC is not available or has not been loaded.");
        };
      }
      else if (szDatabase == "MySQL")
      {
        if (isDriverAvailable(SQLDB_MYSQL))
        {
          returnValue = MySQL();
        }
        else
        {
          WARNINGMESSAGE("The database driver QMYSQL is not available or has not been loaded.");
        };
      }
      else if (szDatabase == "SQLite")
      {
        if (isDriverAvailable(SQLDB_SQLITE))
        {
          returnValue =  SQLite();
        }
        else
        {
          WARNINGMESSAGE("The database driver QSQLITE is not available or has not been loaded.");
        }
      }
      else if (szDatabase == "PostgreSQL")
      {
        if (isDriverAvailable(SQLDB_PSQL))
        {
          returnValue = PostgreSQL();
        }
        else
        {
          WARNINGMESSAGE("The database driver QPSQL is not available or has not been loaded.");
        }
      }
      else
      {
        WARNINGMESSAGE("The database driver " + szDatabase.toStdString() + " is unknown.");
      }

      return returnValue;
    }

    /// @brief Connects to the specified database.
    /// @param[in] driverName - The name of the driver to use to connect with.
    /// @param[in] hostName - The name or IP address of the host to connect to.
    /// @param[in] portNumber - The number of the port to connect to on the host.
    /// @param[in] databaseName - The name of the database to connect to.
    /// @param[in] userName - The name of the user to connect to.
    /// @param[in] password - The password to use when connecting.
    /// @returns true - Connection created.
    /// @returns false - Unable to create connection.
    /// @pre 1. szConnectionName must have been set before this function is called.
    /// @throws
    /// @version 2017-06-19/GGB - Function created.

    bool CDatabase::createConnection(QString const &driverName, QString const &hostName, std::uint16_t portNumber,
                                     QString const &databaseName,
                                     QString const &userName, QString const &password)
    {
      bool returnValue = false;

        // Create the database instance and setup the connection strings.

      dBase = new QSqlDatabase();
      (*dBase) = QSqlDatabase::addDatabase(driverName, szConnectionName);
      dBase->setHostName(hostName);
      dBase->setDatabaseName(databaseName);
      dBase->setUserName(userName);
      dBase->setPassword(password);
      dBase->setPort(portNumber);

        // Attempt to open the database and if there is an error, throw an exception.

      if ( !dBase->open() )
      {
        QSqlError error = dBase->lastError();     // 1045 - Access denied
                                                  // 2005 - Unable to connect

        ERRORMESSAGE("Error while connecting to Database: " + hostName.toStdString() + "." + databaseName.toStdString());
        ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      }
      else
      {
        INFOMESSAGE("Connected to MySQL Database: " + hostName.toStdString() + "." + databaseName.toStdString());
        returnValue = true;
      }

      return returnValue;
    }

    /// @brief Connects to a specified ODBC database driver
    /// @param[in] driverName - The driver to use for the connection.
    /// @param[in] databaseName - The name of the database to open.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @pre 1. szConnectionName must have been set before this function is called
    /// @version 2017-06-20/GGB - Function created.

    bool CDatabase::createConnectionODBC(QString const &driverName, QString const &databaseName)
    {
      bool returnValue = false;

      dBase = new QSqlDatabase();
      (*dBase) = QSqlDatabase::addDatabase(driverName, szConnectionName);
      dBase->setDatabaseName(databaseName);

      if (!dBase->open())
      {
        QSqlError error = dBase->lastError();

        ERRORMESSAGE("Error while connecting to ODBC Database: " + databaseName.toStdString());
        ERRORMESSAGE("Error returned by ODBC Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      }
      else
      {
        INFOMESSAGE("Connected to ODBC Database: " + databaseName.toStdString());
        returnValue = true;
      };

      return returnValue;
    }

    /// @brief Connect to a specified SQLite database.
    /// @param[in] driverName - The name of the driver to use for the connection.
    /// @param[in] databaseName - The name of the database to connect to.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @pre 1. szConnectionName must have been set before this function is called
    /// @version 2017-06=-20/GGB - Function created.

    bool CDatabase::createConnectionSQLite(QString const &driverName, QString const &databaseName)
    {
      bool returnValue = false;

      dBase = new QSqlDatabase();
      (*dBase) = QSqlDatabase::addDatabase(driverName, szConnectionName);
      dBase->setDatabaseName(databaseName);

      if ( !dBase->open() )
      {
        QSqlError error = dBase->lastError();

        ERRORMESSAGE("Error while connecting to SQLite Database: " + databaseName.toStdString());
        ERRORMESSAGE("Error returned by SQLLite Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      }
      else
      {
        INFOMESSAGE("Connected to SQLite Database: " + databaseName.toStdString());
        returnValue = true;
      };

      return returnValue;
    }

    /// @brief Returns the requested driver name if it exists.
    /// @returns The specified driver name.
    /// @version 2017-07-01/GGB - Function created.

    QString CDatabase::getDriverName(int driverID)
    {
      return databaseDrivers[driverID].driverName;
    }

    /// @brief Determines if a driver is available and been loaded.
    /// @param[in] driverID - The ID value of the driver to check.
    /// @returns true - SQL Driver is available
    /// @returns false - SQL Driver is not available.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    bool CDatabase::isDriverAvailable(int driverID)
    {
      return ( databaseDrivers.find(driverID) != databaseDrivers.end() );
    }

    /// @brief initialises the list of drivers available for use.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    void CDatabase::initialiseDrivers()
    {
        // Only load the relevant database drivers.

      QStringList driverList = QSqlDatabase::drivers();

      if (driverList.contains("QMYSQL"))
      {
        databaseDrivers.insert(std::pair<int, SDatabaseDriver>(SQLDB_MYSQL, SDatabaseDriver("MySQL", "QMYSQL")));
        INFOMESSAGE("Enabling MySQL DBMS Support");

      };
      if (driverList.contains("QOCI"))
      {
        databaseDrivers.insert(std::pair<int, SDatabaseDriver>(SQLDB_QOCI, SDatabaseDriver("Oracle", "QOCI")));
        INFOMESSAGE("Enabling Oracle DBMS Support");
      };
      if (driverList.contains("QSQLITE"))
      {
        databaseDrivers.insert(std::pair<int, SDatabaseDriver>(SQLDB_SQLITE, SDatabaseDriver("SQLite", "QSQLITE")));
        INFOMESSAGE("Enabling SQLite DBMS Support");
      };
      if (driverList.contains("QPSQL"))
      {
        databaseDrivers.insert(std::pair<int, SDatabaseDriver>(SQLDB_PSQL, SDatabaseDriver("PostgreSQL", "QPSQL")));
        INFOMESSAGE("Enabling Postgre DBMS Support");
      };
      if (driverList.contains("QODBC"))
      {
        databaseDrivers.insert(std::pair<int, SDatabaseDriver>(SQLDB_ODBC, SDatabaseDriver("ODBC", "QODBC")));
        INFOMESSAGE("Enabling ODBC DBMS Support");
      };
    }

    //*****************************************************************************************************************************
    //
    // General Functions
    //
    //*****************************************************************************************************************************

    // Creates a new observer.
    // 1) The entry in the TBL_OBSERVERS table is created.
    // 2) Entries in the TBL_CATALOGORDER table are created.
    //
    // 2011-06-25/GGB - Function created.

    void CreateObserver(QVariant const &observer, QVariant const &aavso, bool dontdisplay, QVariant &vObserverID)
    {
      QSqlTableModel model;
      QSqlRecord record;
      QString szSQL;
      QSqlQuery query;
      unsigned int sortOrder = 1;

      model.setTable("TBL_OBSERVERS");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.select();

      CreateRecord_TBL_OBSERVERS(record);

        // Create a new record.

      record.setValue("SHORTTEXT", QVariant(observer));
      if (aavso.toString().size() > 0)
        record.setValue("AAVSOINITIALS", QVariant(aavso));
      else
        record.setNull("AAVSOINITIALS");
      record.setValue("DONTDISPLAY", QVariant(dontdisplay));

      model.insertRecord(-1, record);

      record = model.record(model.rowCount()-1);
      vObserverID = record.value("OBSERVER_ID");

        // Now create the values in the TBL_CATALOGORDER table.

      model.setTable("TBL_CATALOGORDER");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.select();

      record.clear();
      CreateRecord_TBL_CATALOGORDER(record);

      szSQL = QString( \
        "SELECT c.CATALOG_ID FROM TBL_CATALOG c ORDER BY c.CATALOG_ID");

      query.exec(szSQL);

      while (query.next())
      {
        record.setValue("OBSERVER_ID", vObserverID);
        record.setValue("CATALOG_ID", query.value(0));
        record.setValue("SORTORDER", QVariant(sortOrder++));

        model.insertRecord(-1, record);
      };
    }

    // Creates the fields for a weather archive record.
    //
    // 2011-07-31/GGB - Function created.

    void CreateRecord_TBL_ARCHIVE(QSqlRecord &record)
    {
      record.clear();

      record.append(QSqlField("ARCHIVE_ID", QVariant::UInt));
      record.append(QSqlField("SITE_ID", QVariant::UInt));
      record.append(QSqlField("MJD", QVariant::UInt));
      record.append(QSqlField("TIME", QVariant::UInt));
      record.append(QSqlField("outsideTemp", QVariant::Double));
      record.append(QSqlField("hiOutsideTemp", QVariant::Double));
      record.append(QSqlField("lowOutsideTemp", QVariant::Double));
      record.append(QSqlField("insideTemp", QVariant::Double));
      record.append(QSqlField("barometer", QVariant::Double));
      record.append(QSqlField("outsideHum", QVariant::Double));
      record.append(QSqlField("insideHum", QVariant::Double));
      record.append(QSqlField("rain", QVariant::Double));
      record.append(QSqlField("hiRainRate", QVariant::Double));
      record.append(QSqlField("windSpeed", QVariant::Double));
      record.append(QSqlField("hiWindSpeed", QVariant::Double));
      record.append(QSqlField("windDirection", QVariant::UInt));
      record.append(QSqlField("solarRad", QVariant::UInt));
      record.append(QSqlField("hiSolarRad", QVariant::UInt));
      record.append(QSqlField("UV", QVariant::UInt));
      record.append(QSqlField("hiUV", QVariant::UInt));
    }

    // Creates the fields for the entire table for TBL_CATALOGORDER
    //
    // 2011-06-25/GGB - Function created.

    void CreateRecord_TBL_CATALOGORDER(QSqlRecord &record)
    {
      record.clear();

      record.append(QSqlField("CATALOGORDER_ID", QVariant::UInt));
      record.append(QSqlField("OBSERVER_ID", QVariant::UInt));
      record.append(QSqlField("CATALOG_ID", QVariant::UInt));
      record.append(QSqlField("SORTORDER", QVariant::UInt));
    }

    // Creates the fields for the entire table for WEATHER::TBL_DAYSUMMARY
    //
    // 2011-07-30/GGB - Function created.

    void CreateRecord_TBL_DAYSUMMARY(QSqlRecord &record)
    {
      record.clear();

      record.append(QSqlField("DAILYSUMMARY_ID", QVariant::UInt));
      record.append(QSqlField("SITE_ID", QVariant::UInt));
      record.append(QSqlField("MJD", QVariant::UInt));
      record.append(QSqlField("hiOutTemp", QVariant::Double));
      record.append(QSqlField("lowOutTemp", QVariant::Double));
      record.append(QSqlField("hiInTemp", QVariant::Double));
      record.append(QSqlField("lowInTemp", QVariant::Double));
      record.append(QSqlField("avgOutTemp", QVariant::Double));
      record.append(QSqlField("avgInTemp", QVariant::Double));
      record.append(QSqlField("hiChill", QVariant::Double));
      record.append(QSqlField("lowChill", QVariant::Double));
      record.append(QSqlField("hiDew", QVariant::Double));
      record.append(QSqlField("lowDew", QVariant::Double));
      record.append(QSqlField("avgChill", QVariant::Double));
      record.append(QSqlField("avgDew", QVariant::Double));
      record.append(QSqlField("hiOutHum", QVariant::Double));
      record.append(QSqlField("lowOutHum", QVariant::Double));
      record.append(QSqlField("hiInHum", QVariant::Double));
      record.append(QSqlField("lowInHum", QVariant::Double));
      record.append(QSqlField("avgOutHum", QVariant::Double));
      record.append(QSqlField("hiBar", QVariant::Double));
      record.append(QSqlField("lowBar", QVariant::Double));
      record.append(QSqlField("avgBar", QVariant::Double));
      record.append(QSqlField("hiSpeed", QVariant::Double));
      record.append(QSqlField("avgSpeed", QVariant::Double));
      record.append(QSqlField("dailyRainTotal", QVariant::Double));
      record.append(QSqlField("hiRainRate", QVariant::Double));
      record.append(QSqlField("dailyUVDose", QVariant::UInt));
      record.append(QSqlField("hiUV", QVariant::UInt));
      record.append(QSqlField("hiSolar", QVariant::UInt));
      record.append(QSqlField("dailySolarEnergy", QVariant::UInt));
      record.append(QSqlField("minSunlight", QVariant::UInt));
    }

    // Creates the fields for the entire table for TBL_INSTRUMENTS
    //
    // 2011-06-23/GGB - Function created.

    void CreateRecord_TBL_INSTRUMENTS(QSqlRecord &record)
    {
      record.clear();

      record.append(QSqlField("INSTRUMENT_ID", QVariant::UInt));
      record.append(QSqlField("SHORTTEXT", QVariant::String));
      record.append(QSqlField("MANUFACTURER", QVariant::String));
      record.append(QSqlField("MODEL", QVariant::String));
      record.append(QSqlField("nX", QVariant::UInt));
      record.append(QSqlField("nY", QVariant::UInt));
      record.append(QSqlField("dX", QVariant::Double));
      record.append(QSqlField("dY", QVariant::Double));
      record.append(QSqlField("ADU", QVariant::Double));
      record.append(QSqlField("DONTDISPLAY", QVariant::Bool));
    }

    // Creates the fields for the entire table for TBL_LEAPSECONDS
    //
    // 2011-12-31/GGB - Function created.

    void CreateRecord_TBL_LEAPSECONDS(QSqlRecord &record)
    {
      //record.append(QSqlField("LEAPSECOND_ID", QVariant::LongLong));
      record.append(QSqlField("MJD", QVariant::LongLong));
      record.append(QSqlField("VALUE", QVariant::Int));
    }

    // Creats the fields for the entire table for TBL_OBSERVERS
    //
    // 2011-06-21/GGB - Function created.

    void CreateRecord_TBL_OBSERVERS(QSqlRecord &record)
    {
      record.append(QSqlField("OBSERVER_ID", QVariant::UInt));
      record.append(QSqlField("SHORTTEXT", QVariant::String));
      record.append(QSqlField("AAVSOINITIALS", QVariant::String));
      record.append(QSqlField("DONTDISPLAY", QVariant::Bool));
    }

    // Creates the fields for the entire table for TBL_SITES.
    //
    // 2011-06-21/GGB - Function created.

    void CreateRecord_TBL_SITES(QSqlRecord &record)
    {
      record.append(QSqlField("SITE_ID", QVariant::UInt));
      record.append(QSqlField("SHORTTEXT", QVariant::String));
      record.append(QSqlField("LATITUDE", QVariant::Double));
      record.append(QSqlField("LONGITUDE", QVariant::Double));
      record.append(QSqlField("ALTITUDE", QVariant::Int));
      record.append(QSqlField("TIMEZONE", QVariant::Int));
      record.append(QSqlField("DONTDISPLAY", QVariant::Bool));
      record.append(QSqlField("IAUCode", QVariant::String));
      record.append(QSqlField("DAYLIGHTSAVING", QVariant::Bool));
    }

    // Creates the fields for the entire table for TBL_TELESCOPES.
    //
    // 2011-06-25/GGB - Function created.

    void CreateRecord_TBL_TELESCOPES(QSqlRecord &record)
    {
      record.append(QSqlField("TELESCOPE_ID", QVariant::UInt));
      record.append(QSqlField("SHORTTEXT", QVariant::String));
      record.append(QSqlField("MANUFACTURER", QVariant::String));
      record.append(QSqlField("MODEL", QVariant::String));
      record.append(QSqlField("APERTURE", QVariant::Double));
      record.append(QSqlField("FRATIO", QVariant::Double));
      record.append(QSqlField("OBSTRUCTION", QVariant::Double));
      record.append(QSqlField("DONTDISPLAY", QVariant::Bool));
    }

  }	// namespace database
}	// namespace astroManager
