//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DatabaseATID
// SUBSYSTEM:           Astronomical Target Information Database
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// LIBRARY DEPENDANCE:	Qt, CFitsIO.
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
// CLASSES INCLUDED:    CATID
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                       - CdatabaseWeather
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-24 GGB - Split CATID into it's own file.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-01-04 GGB - Classes developed for astroManager
//
//*********************************************************************************************************************************

#include "../../Include/database/databaseATID.h"

  // astroManager files

#include "../../Include/Settings.h"
#include "../../Include/qtExtensions/qt.h"
#include "../../Include/astroManager.h"

// Qt Library


//#include <QtSql/QtSql>
//#include <QtNetwork/QtNetwork>

  // Standard libraries

#include <sstream>
#include <string>

  // Boost libraries

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/optional.hpp"

namespace AstroManager
{
  namespace database
  {
    CATID *databaseATID = nullptr;


    //*****************************************************************************************************************************
    //
    // CATID
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the CATID class.
    /// @details The class reads the database key and calls the relevant setup routine.
    /// @throws None.
    /// @version 2017-06-20/GGB - Updated to reflect changes to CDatabase. (Bug #69)
    /// @version 2013-05-15/GGB - Added setting for disabling the database by default.
    /// @version 2013-01-26/GGB - connectToDatabase function created and body of code moved.
    /// @version 2013-01-25/GGB - Added SQLite support
    /// @version 2011-02-26/GGB - Added code to capture that the registry may not be set up and to throw an exception when this is
    ///                           found.
    /// @version 2010-11-23/GGB - Function created

    CATID::CATID(): CDatabase("ATID"), useSIMBAD(true), ATIDdisabled_(false)
    {
      QVariant variant = settings::astroManagerSettings->value(settings::ATID_DATABASE_DISABLE, QVariant());

      if (variant.isNull())
      {
        settings::astroManagerSettings->setValue(settings::ATID_DATABASE_DISABLE, QVariant(true));
        ATIDdisabled_ = true;
      }
      else
      {
        ATIDdisabled_ = variant.toBool();
      };

      if (!ATIDdisabled_)
      {
        useSIMBAD = settings::astroManagerSettings->value(settings::ATID_DATABASE_USESIMBAD, QVariant(true)).toBool();

        sqlWriter.createTable("TBL_CATALOG");
          sqlWriter.createColumn("TBL_CATALOG", "CATALOG_ID");
          sqlWriter.createColumn("TBL_CATALOG", "ABBREVIATION");
          sqlWriter.createColumn("TBL_CATALOG", "CATALOG_NAME");
          sqlWriter.createColumn("TBL_CATALOG", "COMMENTS");
          sqlWriter.createColumn("TBL_CATALOG", "MASK");
          sqlWriter.createColumn("TBL_CATALOG", "DATEADDED");
        sqlWriter.createTable("TBL_CONSTELLATIONS");
          sqlWriter.createColumn("TBL_CONSTELLATIONS", "CONSTELLATION_ID");
          sqlWriter.createColumn("TBL_CONSTELLATIONS", "CONSTELLATIONNAME");
          sqlWriter.createColumn("TBL_CONSTELLATIONS", "ABBREVIATION");
        sqlWriter.createTable("TBL_OBJECTTYPES");
          sqlWriter.createColumn("TBL_OBJECTTYPES", "OBJECTTYPE_ID");
          sqlWriter.createColumn("TBL_OBJECTTYPES", "OBJECTTYPE");
          sqlWriter.createColumn("TBL_OBJECTTYPES", "COMMENTS");
          sqlWriter.createColumn("TBL_OBJECTTYPES", "CALCPOSN");
        sqlWriter.createTable("TBL_SPECTRALTYPES");
          sqlWriter.createColumn("TBL_SPECTRALTYPES", "SPECTRALTYPE_ID");
          sqlWriter.createColumn("TBL_SPECTRALTYPES", "SHORTTEXT");
          sqlWriter.createColumn("TBL_SPECTRALTYPES", "LONGTEXT");
        sqlWriter.createTable("TBL_EPOCH");
          sqlWriter.createColumn("TBL_EPOCH", "EPOCH_ID");
          sqlWriter.createColumn("TBL_EPOCH", "EPOCH");
          sqlWriter.createColumn("TBL_EPOCH", "JULIANDAY");
        sqlWriter.createTable("TBL_NAMES");
          sqlWriter.createColumn("TBL_NAMES", "NAME_ID");
          sqlWriter.createColumn("TBL_NAMES", "NAME");
          sqlWriter.createColumn("TBL_NAMES", "OBJECT_ID");
          sqlWriter.createColumn("TBL_NAMES", "CATALOG_ID");
      };
    }

    /// @brief Destructor for the class. Ensures that the database connection is removed.
    /// @throws None.
    /// @version 2013-05-15/GGB - Conditional deletion of database.
    /// @version 2013-02-09/GGB - Added conditional delete of the database.
    /// @version 2010-11-28/GGB - Function created

    CATID::~CATID()
    {
      if (!ATIDdisabled_)
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

    /// @brief Connects to the ATID database.
    /// @throws None.
    /// @version 2013-05-15/GGB - Conditional connection to database.
    /// @version 2013-01-26/GGB - Function created.

    void CATID::connectToDatabase()
    {
      QVariant database = settings::astroManagerSettings->value(settings::ATID_DATABASE_DBMS);
      QString szDatabase;
      
      if ( !ATIDdisabled_ && !useSIMBAD )
      {

        if ( !database.isNull() )
        {         
          szDatabase = database.toString();		// Get the database type

          if (!CDatabase::connectToDatabase(szDatabase))
          {
            INFOMESSAGE("Unable to connect to ATID database. Disabling ATID database.");
            ATIDdisabled_ = true;
            useSIMBAD = true;
          }
        }
        else
        {
          useSIMBAD = true;

          WARNINGMESSAGE("Setting " + settings::ATID_DATABASE_DBMS.toStdString() + "not found.");
          INFOMESSAGE("Unable to connect to ATID database. Disabling ATID database.");
          ATIDdisabled_ = true;
        };
      };
    }

    /// @brief Gets the most current magnitude and mag error values for the given filter number.
    //
    /// @version 2010-11-28/GGB - Function created

    void CATID::GetLatestMagnitude(const long long OBJECT_ID, const long long FILTER_ID, float &Mag, float &Mag_err) const
    {
      QSqlQuery query(*dBase);
      QString szSQL = QString("SELECT TBL_MAGNITUDES.MAG, TBL_MAGNITUDES.MAG_ERR " \
                              "FROM TBL_MAGNITUDES " \
                              "WHERE (((TBL_MAGNITUDES.OBJECT_ID)=%1) AND ((TBL_MAGNITUDES.FILTER_ID)=%2)) " \
                              "ORDER BY TBL_MAGNITUDES.VALID_DATE DESC").arg(OBJECT_ID).arg(FILTER_ID);

      query.exec(szSQL);
      query.first();

      if (!query.value(0).isValid())
      {
        ASTROMANAGER_ERROR(0x0100);
      }
      else
      {
        Mag = query.value(0).toFloat();
        Mag_err = query.value(1).toFloat();
      };
    }

    /// @brief Function for connecting to a MySQL database.
    /// @details Reads the information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect changes to CDatabase (Bug #69)
    /// @version 2015-06-13/GGB - Use databaseDrivers static member for driver names.
    /// @version 2011-06-06/GGB - Function created

    bool CATID::MySQL()
    {
      return ( createConnection(settings::astroManagerSettings->value(settings::ATID_MYSQL_DRIVERNAME, QVariant(databaseDrivers[SQLDB_MYSQL].driverName)).toString(),
                                settings::astroManagerSettings->value(settings::ATID_MYSQL_HOSTADDRESS, QVariant(QString("localhost"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_MYSQL_PORT, QVariant(3306)).toInt(),
                                settings::astroManagerSettings->value(settings::ATID_MYSQL_DATABASENAME, QVariant(QString("ATID"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_MYSQL_USERNAME, QVariant(QString("ATID"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_MYSQL_PASSWORD, QVariant(QString("ATID"))).toString()) );
    }

    /// @brief Function for opening an ODBC database.
    /// @details Reads information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect changes to CDatabase (Bug #69)
    /// @version 2010-12-09/GGB - Function created

    bool CATID::ODBC()
    {
      return ( createConnectionODBC(settings::astroManagerSettings->value(settings::ATID_ODBC_DRIVERNAME, QVariant(QString("QODBC"))).toString(),
                                    settings::astroManagerSettings->value(settings::ATID_ODBC_DATASOURCENAME, QVariant(QString("MSA ATID"))).toString()) );
    }

    /// @brief Function for opening an Oracle database.
    /// @details Reads the relevant information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect changes to CDatabase (Bug #69)
    /// @version 2014-02-16/GGB - Updated constant values to constants and not #defines.
    /// @version 2011-02-25/GGB - 1) Changed exception type to (...)
    ///                           2) Added code to capture errors and throw exceptions when settings not found.
    /// @version 2010-11-28/GGB - Function created.

    bool CATID::Oracle()
    {
      return ( createConnection(settings::astroManagerSettings->value(settings::ATID_ORACLE_DRIVERNAME, QVariant(QString("QOCI"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_ORACLE_HOSTNAME, QVariant(QString("localhost"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_ORACLE_PORT, QVariant(1521)).toInt(),
                                settings::astroManagerSettings->value(settings::ATID_ORACLE_DATABASENAME, QVariant(QString("xe"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_ORACLE_USERNAME, QVariant(QString("ATID"))).toString(),
                                settings::astroManagerSettings->value(settings::ATID_ORACLE_PASSWORD, QVariant(QString("ATID"))).toString()) );
    }

    bool CATID::parseSIMBADObjectQuery(std::string const &, ACL::CTargetStellar &)
    {

    }

    /// @brief Parse a returned reply from SIMBAD.
    /// @param[in] replyString - The SIMBAD reply.
    /// @param[in] targetStore - Vector containing the returned objects.
    /// @returns true - Returned string is a valid return from SIMBAD
    /// @returns false - Returned string is not a valid return from SIMBAD.
    /// @throws Nome.
    /// @version 2016-04-26/GGB - Function created.

    bool CATID::parseSIMBADReply(std::string const &replyString, ACL::DTargetAstronomy &targetStore)
    {
      bool returnValue = false;

        // First search for errors. Will be in form of "::error

      if (replyString.find("::error", 0) == std::string::npos)
      {
        std::istringstream inputStream(replyString);
        std::string szLine;
        std::string szValue;
        FP_t RA, Dec, fpValue;
        std::string objectName;

          // return string is ";" delimited.
          // Form is ID; COO(A); COO(D); PM(A); PM(D); PLX(V); RV(V);

        while ( !inputStream.eof() )
        {
          std::string::size_type tokenBegin = 0, tokenEnd = 0;
          std::getline(inputStream, szLine);
          if (szLine.length() < 6)
          {
            break;
          };

            // Read the values.

          tokenEnd = szLine.find(";", 0);
          objectName = szLine.substr(0, tokenEnd);
          boost::trim(objectName);

            // RA Value is in HH MM SS.SSS format.

          tokenBegin = tokenEnd + 1;
          tokenEnd = szLine.find(";", tokenBegin);
          szValue = szLine.substr(tokenBegin, tokenEnd - tokenBegin);
          boost::trim(szValue);
          ACL::sscanfHMS(szValue, RA);

          tokenBegin = tokenEnd + 1;
          tokenEnd = szLine.find(";", tokenBegin);
          szValue = szLine.substr(tokenBegin, tokenEnd - tokenBegin);
          boost::trim(szValue);
          ACL::sscanfDMS(szValue, Dec);

            // Create the stellar target object.

          ACL::SPTargetStellar targetStellar(new ACL::CTargetStellar(objectName, ACL::CAstronomicalCoordinates(RA, Dec)));

          tokenBegin = tokenEnd + 1;
          tokenEnd = szLine.find(";", tokenBegin);
          szValue = szLine.substr(tokenBegin, tokenEnd - tokenBegin);
          boost::trim(szValue);
          if (szValue != "~")
          {
            fpValue = boost::lexical_cast<FP_t>(szValue);
            targetStellar->pmRA(fpValue);
          };

          tokenBegin = tokenEnd + 1;
          tokenEnd = szLine.find(";", tokenBegin);
          szValue = szLine.substr(tokenBegin, tokenEnd - tokenBegin);
          boost::trim(szValue);
          if (szValue != "~")
          {
            fpValue = boost::lexical_cast<FP_t>(szValue);
            targetStellar->pmDec(fpValue);
          };

          tokenBegin = tokenEnd + 1;
          tokenEnd = szLine.find(";", tokenBegin);
          szValue = szLine.substr(tokenBegin, tokenEnd - tokenBegin);
          boost::trim(szValue);
          if (szValue != "~")
          {
            fpValue = boost::lexical_cast<FP_t>(szValue);
            targetStellar->parallax(fpValue);
          };

          tokenBegin = tokenEnd + 1;
          tokenEnd = szLine.find(";", tokenBegin);
          szValue = szLine.substr(tokenBegin, tokenEnd - tokenBegin);
          boost::trim(szValue);
          if (szValue != "~")
          {
            fpValue = boost::lexical_cast<FP_t>(szValue);
            targetStellar->radialVelocity(fpValue);
          };

          targetStore.push_back(targetStellar);
        };

        returnValue = true;
      };

      return returnValue;
    }

    // Adds the filters into the specified list widget.
    //
    // 2011-06-18/GGB - Initial creation of function

    void CATID::PopulateFiltersList(QListWidget *lw)
    {
      QListWidgetItem *lwi;
      QString szSQL;
      std::auto_ptr<QSqlQuery> query(new QSqlQuery(*dBase));

      szSQL = QString("SELECT f.FILTER_ID, f.SHORTTEXT FROM TBL_FILTERS f");

      query->exec(szSQL);

      while ( query->next() )
      {
        lwi = new QListWidgetItem(QString(query->value(1).toString()));
        lwi->setData(ROLE_FILTERID, query->value(0));
        lw->addItem(lwi);
        lwi = NULL;
      };
    }

    // Adds the filters into the specified list widget.
    //
    // 2011-06-18/GGB - Function Created

    void CATID::PopulateFiltersList(QListWidget *lw, QIcon &icon)
    {
      QListWidgetItem *lwi;
      QString szSQL;
      std::auto_ptr<QSqlQuery> query(new QSqlQuery(*dBase));

      szSQL = QString("SELECT f.FILTER_ID, f.SHORTTEXT FROM TBL_FILTERS f");

      query->exec(szSQL);

      while ( query->next() )
      {
        lwi = new QListWidgetItem(icon, QString(query->value(1).toString()));
        lwi->setData(ROLE_FILTERID, query->value(0));
        lw->addItem(lwi);
        lwi = nullptr;
      };
    }

    /// @brief Function to read stellar object information from the database and write/copy it into the stellar object
    /// @param[in] vNameID - The name of the object to read.
    /// @param[in] stellarObject - The stellar object to populate.
    /// @returns true - object found
    /// @returns false - object not found.
    /// @throws None.
    /// @version 2011-07-10/GGB - Function created.

    bool CATID::populateStellarObject(QVariant const &vNameID, ACL::SPTargetStellar stellarObject)
    {
      QSqlQuery query(*dBase);
      QString szSQL;
      bool returnValue = false;

      szSQL = QString( \
        "SELECT n.NAME, o.RA, o.DEC, o.pmRA, o.pmDEC, o.RADIALVELOCITY, o.PARALLAX, e.JULIANDAY, o.FK4, o.FK5, o.ICRS "
        "FROM TBL_NAMES n INNER JOIN (TBL_STELLAROBJECTS o INNER JOIN TBL_EPOCH e ON o.EPOCH_ID = e.EPOCH_ID) ON n.STELLAROBJECT_ID = o.OBJECT_ID "
        "WHERE n.NAME_ID = %1").arg(vNameID.toString());

      query.exec(szSQL);

      QSqlError lastError = query.lastError();

      if (query.first())
      {
        szSQL = query.value(0).toString();

        stellarObject->objectName(szSQL.toStdString());

        stellarObject->catalogueCoordinates(ACL::CAstronomicalCoordinates(MCL::CAngle(query.value(1).toDouble(), MCL::AF_HMSs),
                                                                          MCL::CAngle(query.value(2).toDouble(), MCL::AF_DMSs)));

        //if (query.value(8).toBool())
//          stellarObject->setReferenceSystem(ACL::CStellarObject::RS_FK4);
  //      else if (query.value(9).toBool())
    //      stellarObject->setReferenceSystem(ACL::CStellarObject::RS_FK5);
      //  else if (query.value(10).toBool())
        //  stellarObject->setReferenceSystem(ACL::CStellarObject::RS_ICRS);
//        else
  //        ARPAA_ERROR(0x2000);    // Reference system not specified for object.

//        stellarObject->setPM(query.value(3).toDouble(), query.value(4).toDouble());
  //      stellarObject->setRadialVelocity(query.value(5).toDouble());
    //    stellarObject->setParallax(query.value(6).toDouble());
      //  stellarObject->setEpoch(query.value(7).toDouble());
        returnValue = true;
      };

      return returnValue;
    }

  /*
    // Runs through the database and calculates the constellation names for all the objects.
    //
    // Input:		nAll = 0 - Only rebuild objects without constellations.
    //					nAll <> 0 - Rebuild all constellation names.
    //
    // Revision:		1.00
    // Written by:	Gavin Blakeman
    // Date:				25 January 2010

    void RebuildConstellationNames(int nAll)
    {
      _RecordsetPtr pRstObjects("ADODB.Recordset");
      _RecordsetPtr pRstEpoch("ADODB.Recordset");
      _RecordsetPtr pRstConstellations("ADODB.Recordset");
      char szSQL[1024];
      _variant_t vtRA;
      _variant_t vtDec;
      _variant_t vtEpochID;
      _variant_t vtEpoch;
      _variant_t vtConstellationID;
      _variant_t vtConstellationName;
      char szConstellation[10];
      char szEpoch[20];
      double dEpoch, dRA, dDec;
      int nIterations = 0;

      if (nAll)
        strcpy_s(szSQL, sizeof(szSQL), "SELECT TBL_STELLAROBJECTS.RA, TBL_STELLAROBJECTS.DEC, TBL_STELLAROBJECTS.EpochID, TBL_STELLAROBJECTS.CONSTELLATION_ID FROM TBL_STELLAROBJECTS");
      else
        strcpy_s(szSQL, sizeof(szSQL), "SELECT TBL_STELLAROBJECTS.RA, TBL_STELLAROBJECTS.DEC, TBL_STELLAROBJECTS.EpochID, TBL_STELLAROBJECTS.CONSTELLATION_ID FROM TBL_STELLAROBJECTS WHERE TBL_STELLAROBJECTS.Constellation_ID Is Null");

      try
      {
        pRstObjects->CursorLocation = adUseServer;
        pRstObjects->Open(szSQL, _variant_t((IDispatch *)pAstroDBConnection,true), adOpenForwardOnly, adLockOptimistic, adCmdText);

        strcpy_s(szSQL, sizeof(szSQL), "SELECT co.CONSTELLATION_ID, co.Abbreviation FROM TBL_CONSTELLATIONS co");
        pRstConstellations->CursorLocation = adUseServer;
        pRstConstellations->Open(szSQL, _variant_t((IDispatch *)pAstroDBConnection, true), adOpenKeyset, adLockReadOnly, adCmdText);

        pRstEpoch->CursorLocation = adUseServer;
        strcpy_s(szSQL, sizeof(szSQL), "SELECT ep.EPOCHID, ep.EPOCH FROM TBL_EPOCH ep");
        pRstEpoch->Open(szSQL, _variant_t((IDispatch *)pAstroDBConnection, true), adOpenKeyset, adLockReadOnly, adCmdText);

        while (!pRstObjects->EndOfFile)
        {
          vtRA = pRstObjects->Fields->GetItem((long) 0)->GetValue();
          vtDec = pRstObjects->Fields->GetItem((long) 1)->GetValue();
          vtEpochID = pRstObjects->Fields->GetItem((long) 2)->GetValue();

          if ( (vtRA.vt != VT_NULL) && (vtDec.vt != VT_NULL) && (vtEpochID.vt != VT_NULL) )
          {

            pRstEpoch->MoveFirst();
            sprintf_s(szSQL, sizeof(szSQL), "EPOCHID = %s", (char *) ((_bstr_t)vtEpochID));
            pRstEpoch->Find(szSQL, 0, adSearchForward);
            vtEpoch = pRstEpoch->Fields->GetItem((long) 1)->GetValue();

            dRA = (double) vtRA;
            dDec = (double) vtDec;

            dRA = deg(dRA / 10000) * 15;
            dDec = deg(dDec / 10000);

            strcpy_s(szEpoch, sizeof(szEpoch), (char *) ((_bstr_t) vtEpoch));
            dEpoch = atof(szEpoch+1);

            strcpy_s(szConstellation, sizeof(szConstellation), ConstellationName(dRA, dDec, dEpoch));
            pRstConstellations->MoveFirst();
            sprintf_s(szSQL, sizeof(szSQL), "Abbreviation = '%s'", szConstellation);
            pRstConstellations->Find(szSQL, 0, adSearchForward);
            if (!pRstConstellations->EndOfFile)
            {
              vtConstellationID = pRstConstellations->Fields->GetItem((long) 0)->GetValue();
              pRstObjects->Fields->GetItem((long) 3)->Value = vtConstellationID;
              pRstObjects->Update();
              nIterations++;
            };
          };
          pRstObjects->MoveNext();
        };
        pRstObjects->Close();
        pRstConstellations->Close();
        pRstEpoch->Close();
      }
      catch(_com_error &e)
      {
        PrintComError(e);
      };

    }; */

    /// Overloaded readMapFile(). This allows to not read the map file if the application is using SIMBAD.
    //
    // 2013-01-26/GGB - Function created.

    void CATID::readMapFile(boost::filesystem::path const &mfn)
    {
      if (!useSIMBAD)
      {
        sqlWriter.readMapFile(mfn);
      };
    }

    // Builds a TLocation object from the database using the location number passed in the parameter.
    // If no valid location is found, then NULL is returned.
    //
    // 2011-07-16/GGB
    // 2010-07-07/GGB - Function created.

    bool CATID::getLocation(QVariant const &vSiteID, ACL::CGeographicLocation &location)
    {
//      QSqlQuery query(*dBase);
//      QString szSQL = QString( \
//        "SELECT l.Latitude, l.Longitude, l.Altitude, l.TimeZone FROM TBL_SITES l WHERE l.SITE_ID = %1").arg(vSiteID.toString());

//      query.exec(szSQL);

//      if (query.first())
//      {
//        location.setLocation(query.value(0).toDouble(), query.value(1).toDouble(), query.value(2).toInt(), query.value(3).toDouble());
//        return true;
//      }
//      else
//        return false;
    }

    // Gets the location number after querying the database and registry.
    // Calls the BuildLocation function.
    //
    // 2010-06-12/GGB - Function Created

    ACL::CGeographicLocation *CATID::GetLocation()
    {
      QVariant vSiteID;
      ACL::CGeographicLocation *returnValue = new ACL::CGeographicLocation();

      vSiteID = settings::astroManagerSettings->value(settings::SETTINGS_SITE_DEFAULTID);

      getLocation(vSiteID, *returnValue);
      return ( returnValue);
    }

    // Gets an object name from the ATID database.
    // The function first checks if the object has a preferred name defined and returns the
    // preferred name if one is defined. Otherwise it looks at the user preferred name order list and
    // returns the first name in the users preferred name order.
    //
    // PARAMETERS:	vObject - Object number in ATID database.
    //
    // 2010-07-10/GGB - Function Created

    const QString GetNameFromObject(QVariant *vObject)
    {
      QString szSQL;
      QSqlQuery query(databaseATID->database());
      QSqlQuery nameQuery(databaseATID->database());

      szSQL = QString("SELECT o.PREFERREDNAME FROM TBL_STELLAROBJECTS o WHERE o.OBJECT_ID = %1").arg(vObject->toString());
      query.exec(* new QString(szSQL));
      query.first();
      if (query.value(0).isNull())
      {
        szSQL = QString("SELECT TBL_NAMES.Name " \
                        "FROM TBL_NAMES INNER JOIN TBL_CATALOGUEORDER ON TBL_NAMES.CATALOGUE_ID = TBL_CATALOGUEORDER.CATALOGUE_ID " \
                        "WHERE (((TBL_NAMES.STELLAROBJECT_ID)=%1) AND ((TBL_CATALOGUEORDER.OBSERVER_ID)=%2)) " \
                        "ORDER BY TBL_CATALOGUEORDER.SORTORDER").arg(vObject->toString()).arg(settings::astroManagerSettings->value("Observer", QVariant(0)).toString());
        nameQuery.exec(*new QString(szSQL));
      }
      else
      {
        szSQL = QString("SELECT n.Name FROM TBL_NAMES n WHERE n.NAME_ID = %1").arg(query.value(0).toString());
        nameQuery.exec(*new QString(szSQL));
      };
      nameQuery.first();

      return (nameQuery.value(0).toString());
    }

    // Function to populate a combo box with object type information.
    // The data is the index of the record.
    //
    // 2013-07-29/GGB - Added check if the ATID database is disabled.
    // 2010-11-14/GGB - Function created

    void CATID::PopulateObjectTypeCombo(QComboBox *combo)
    {
      if (!ATIDdisabled_)
      {
        QString szSQL = "SELECT s.OBJECTTYPE_ID, s.SHORTTEXT FROM TBL_OBJECTTYPES s";
        QSqlQuery query(*(databaseATID->dBase));

        query.exec(* new QString(szSQL));
        while (query.next())
        {
          combo->addItem(*new QString(query.value(1).toString()), *new QVariant(query.value(0)));
        };
      };
    }

    // Populates a combo box with standard photometry objects within 1 square degree of the
    // Item passed as the second parameter.
    // Photometry Standards are:
    //	1) Primary Standards
    //	2) Johnson standards
    //	3) Cousings standards
    //	4) Landolt standards
    //	5) AAVSO standards
    //
    // PARAMETERS:	combo - Pointer to combo to populate.
    //							object - The photometry object at the center.
    //							addObject - an additional object to add to the combo.
    //
    // 2010-07-10/GGB - Function created.

    void CATID::PopulatePhotometryStandardCombo(QComboBox *combo, QVariant *object, QVariant *addObject)
    {
      QString szSQL;
      QSqlQuery query(*(databaseATID->dBase));
      double dRA, dDEC;
      double dRAmax, dRAmin, dDECmax, dDECmin;
      QString arg1, arg2, arg3, arg4;
      int nIndex;

        // Get the coordinates of the target object. (Don't precess)

      szSQL = "SELECT o.RA, o.DEC FROM TBL_STELLAROBJECTS o WHERE o.OBJECT_ID = " + object->toString();
      query.exec(*new QString(szSQL));
      query.first();
      dRA = query.value(0).toDouble();
      dDEC = query.value(1).toDouble();
      dRA /= 10000;
      dDEC /= 10000;
      dRA = ACL::dms2deg(dRA*15);		// Convert from dms to deg
      dDEC = ACL::dms2deg(dDEC);
      dRAmax = dRA + .5;			// 1 degree window.
      dRAmin = dRA - .5;
      dDECmax = dDEC + .5;
      dDECmin = dDEC - .5;
      if (dRAmax > 360)
        dRAmax -= 360;
      if (dRAmin < 0)
        (dRAmin += 360);
      dRAmax = ACL::dms2deg(dRAmax) * 10000/15;
      dRAmin = ACL::dms2deg(dRAmin) * 10000/15;
      dDECmax = ACL::dms2deg(dDECmax) * 10000;
      dDECmin = ACL::dms2deg(dDECmin) * 10000;

      arg1 = QVariant(dRAmin).toString();
      arg2 = QVariant(dRAmax).toString();
      arg3 = QVariant(dDECmin).toString();
      arg4 = QVariant(dDECmax).toString();

      szSQL = QString("SELECT TBL_NAMES.NAME, TBL_STELLAROBJECTS.OBJECT_ID " \
                    "FROM TBL_NAMES INNER JOIN TBL_STELLAROBJECTS ON TBL_NAMES.NAME_ID = TBL_STELLAROBJECTS.PreferredName " \
                    "WHERE (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.AAVSOStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.LandoltStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.PrimaryStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.JohnsonStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.CousinsStandard)=True))").arg(arg1).arg(arg2).arg(arg3).arg(arg4);
      query.exec(*new QString(szSQL));

        // Fill the combo box.

      while (query.next())
        combo->addItem(*new QString(query.value(0).toString()), *new QVariant(query.value(1)));

        // Now select the additional object and add it if necessary.

      if ( (addObject) && (!addObject->isNull()) )
      {
        szSQL = QString("SELECT n.NAME FROM TBL_NAMES n WHERE n.NAME_ID = %1").arg(addObject->toString());
        query.exec(*new QString(szSQL));
        query.first();
        nIndex = combo->findText(query.value(0).toString(), Qt::MatchExactly);
        if (nIndex < 0)
        {	// Item not in combo box.
          combo->addItem(*new QString(query.value(0).toString()), *new QVariant(*addObject));
        };
        if (addObject->isNull())
          combo->setCurrentIndex(-1);
        else
          combo->setCurrentIndex(nIndex);
      };
    }

    /// @brief Queries for objects around the specified coordinates.
    /// @param[in] coord - The coordinates (RA, Dec) of the center of the search radius.
    /// @param[in] radius - The search radius.
    /// @returns A list of all the objects within the search area.
    /// @throws None.
    /// @version 2013-02-23/GGB - Function created.

    bool CATID::queryByCoordinates(ACL::CAstronomicalCoordinates const &coord, double radius, ACL::DTargetAstronomy &targetList)
    {
      if ( usingSIMBAD() )
      {
        return queryByCoordinatesSIMBAD(coord, radius, targetList);
      }
      else
      {
        return queryByCoordinatesATID(coord, radius, targetList);
      };
    }

    /// @brief Queries for objects within a box bounded by the two corners.
    /// @param[in] tl - The top left of the box
    /// @param[in] rb - The bottom right of the box.
    /// @returns A list of all the objects within the search area.
    /// @throws None.
    /// @note The box edges are assumed to be parallel to lines of RA and DEC.
    /// @version 2013-02-23/GGB - Function created.

    bool CATID::queryByCoordinates(ACL::CAstronomicalCoordinates const &tl, ACL::CAstronomicalCoordinates const &br,
                                   ACL::DTargetAstronomy &targetList)
    {
      if ( usingSIMBAD() )
      {
        return queryByCoordinatesSIMBAD(tl, br, targetList);
      }
      else
      {
        return queryByCoordinatesATID(tl, br, targetList);
      };
    }

    bool CATID::queryByCoordinatesATID(ACL::CAstronomicalCoordinates const &RADEC, double radius, ACL::DTargetAstronomy &targetList)
    {
      return false;
    }

    bool CATID::queryByCoordinatesATID(ACL::CAstronomicalCoordinates const &tl, ACL::CAstronomicalCoordinates const &br,
                                       ACL::DTargetAstronomy &targetList)
    {
      return false;
    }


    /// @brief Queries the SIMBAD database for objects around the specified coordinates.
    /// @param[in] coord - The coordinates (RA, Dec) of the center of the search radius.
    /// @param[in] radius - The search radius.
    /// @returns A list of all the objects within the search area.
    /// @throws None.
    /// @version 2013-02-23/GGB - Function created.

    bool CATID::queryByCoordinatesSIMBAD(ACL::CAstronomicalCoordinates const &coord, double radius,
                                         ACL::DTargetAstronomy &targetList)
    {
      return false;
    }

    /// @brief Queries the SIMBAD database for objects around the specified coordinates.
    /// @param[in] tl - Top left of the box to search
    /// @param[in] br - Bottom right of the box to search
    /// @returns A list of all the objects within the search area.
    /// @throws None.
    /// @version 2017-09-23/GGB - Update to use CAngle
    /// @version 2013-02-23/GGB - Function created.

    bool CATID::queryByCoordinatesSIMBAD(ACL::CAstronomicalCoordinates const &tl, ACL::CAstronomicalCoordinates const &br,
                                         ACL::DTargetAstronomy &targetList)
    {
      bool returnValue = false;
      ACL::CSIMBADScript script;
      FP_t left, right, top, bottom, center_r, center_d, dim_r, dim_d;
      QNetworkAccessManager manager;
      QEventLoop q;
      QTimer tT;

      LOGMESSAGE(info, "Querying SIMBAD for objects.");

      left = tl.RA().degrees();
      right = br.RA().degrees();

      if (left > right)
      {
        std::swap(left, right);
      }

      top = tl.DEC().degrees();
      bottom = br.DEC().degrees();

      if (top < bottom)
      {
        std::swap(bottom, top);
      }

      dim_r = right - left;
      dim_d = top - bottom;
      center_r = dim_r / 2 + left;
      center_d = dim_d / 2 + bottom;

      script.addRegion(ACL::CSIMBADScript::BOX, {center_r, center_d, dim_r, dim_d});
      script.addFields({ACL::CSIMBADScript::IDLIST_1,
                        ACL::CSIMBADScript::COO_A,
                        ACL::CSIMBADScript::COO_D,
                        ACL::CSIMBADScript::PM_A,
                        ACL::CSIMBADScript::PM_D,
                        ACL::CSIMBADScript::PLX_V,
                        ACL::CSIMBADScript::RV_V});
      script.addQueryCriteria({{"maintypes", "star"}});

      std::string str = static_cast<std::string>(script);

      tT.setSingleShot(true);
      connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
      connect(&manager, SIGNAL(finished(QNetworkReply*)), &q, SLOT(quit()));
      QNetworkReply *reply = manager.get(QNetworkRequest(QString::fromStdString(str)));

      tT.start(20000);
      q.exec();

      if (tT.isActive())
      {
        tT.stop();

        QByteArray ba = reply->readAll();
        reply->close();

        QString simbadReply(ba);

        returnValue = parseSIMBADReply(simbadReply.toStdString(), targetList);

        LOGMESSAGE(info, std::to_string(targetList.size()) + " objects loaded from SIMBAD.");
      }
      else
      {
        QMessageBox msgBox;

        msgBox.setWindowTitle(tr("SIMBAD Request timed out"));
        msgBox.setText(tr("The request to the SIMBAD server timed out before a reply was received."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
        LOGMESSAGE(info, "Failed to connect to the SIMBAD server. Request timed out.");
      }
      return returnValue;
    }

    /// @brief Queries the ATID database for the name of the object with an specific OID
    /// @param[in] OID - The OID to search for.
    /// @param[out] objectNames - The names of the object.
    /// @returns true - Object found.
    /// @returns false - Object not found.
    /// @throws None.
    /// @version 2016-05-07/GGB - Function created.

    bool CATID::queryNamesFromATID(std::uint64_t OID, std::vector<std::string> &objectNames)
    {
      bool returnValue = false;
      QSqlQuery query(*dBase);

      query.setForwardOnly(true);
      sqlWriter.resetQuery();

      sqlWriter.select({"TBL_NAMES.Name"}).from({"TBL_NAMES"}).where({ GCL::sqlwriter::parameterTriple("OID", "=", OID) });

      QString szSQL = QString::fromStdString(sqlWriter.string());

      query.exec(szSQL);
      query.first();

      do
      {
        if (query.value(0).isValid())
        {
          objectNames.push_back(query.value(0).toString().toStdString());
          returnValue = true;
        }
      }
      while (query.next());

      return returnValue;
    }

    /// @brief Queries a stellar object based on a name.
    /// @param[in] objectName - The name of the object to query
    /// @param[out] stellarObject - Pointer to the object to populate with data if succesfull.
    /// @param[in] forceQuery - Force the query to use a specific source if required.
    /// @returns true - stellarObject updated with data from query
    /// @returns false - Unable to update stellarObject
    /// @throws CCodeError(astroManager)
    /// @version 2016-05-07/GGB - Function created.

    bool CATID::queryStellarObjectByName(std::string const &objectName, ACL::CTargetStellar *stellarObject, EForce forceQuery)
    {
      bool returnValue = false;

      switch (forceQuery)
      {
        case FORCE_NONE:
        {
          if (useSIMBAD || ATIDdisabled_)
          {
            returnValue = queryStellarObjectByName_SIMBAD(objectName, stellarObject);
          }
          else
          {
            returnValue = queryStellarObjectByName_ATID(objectName, stellarObject);
          }
          break;
        }
        case FORCE_ATID:
        {
          returnValue = queryStellarObjectByName_ATID(objectName, stellarObject);
          break;
        }
        case FORCE_SIMBAD:
        {
          returnValue = queryStellarObjectByName_SIMBAD(objectName, stellarObject);
          break;
        }
        default:
        {
          CODE_ERROR(astroManager);
          break;
        }
      }

      return returnValue;
    }

    bool CATID::queryStellarObjectByName_ATID(const std::string &objectName, ACL::CTargetStellar *)
      {
//      QSqlQuery query(*dBase);
//      QString szSQL;
//      bool returnValue = false;

//      sqlWriter.resetQuery();

//      sqlWriter.select({"n.NAME", "o.RA", "o.DEC", "o.RADIALVELOCITY", "o.PARALLAX", "e.JULIANDAY", "o.FK4", "o.FK5", "o.ICRS"});

//      sqlWriter.join({
//                       std::make_tuple("TBL_NAMES",
//                                        "STELLAROBJECT_ID",
//                                        GCL::sqlwriter::CSQLWriter::JOIN_INNER,
//                                        "TBL_STELLAROBJECTS",
//                                        "OBJECT_ID"),
//                       std::make_tuple("TBL_STELLAROBJECTS", "EPOCH_ID",
//                                        GCL::sqlwriter::CSQLWriter::JOIN_INNER,
//                                        "TBL_EPOCH", "EPOCH_ID")
//                     });
//      sqlWriter.where({
//                        GCL::sqlwriter::parameterTriple("OBJECT_ID", "=", objectName),
//                      });
//      sqlWriter.alias({
//                        {"TBL_NAME", "n"},
//                        {"TBL_STELLAROBJECTS", "o"}
//                      });
//      szSQL = QString::fromStdString(sqlWriter.string());

      //szSQL = QString( \
      //  "SELECT n.NAME, o.RA, o.DEC, o.pmRA, o.pmDEC,  "o.RADIALVELOCITY, o.PARALLAX, e.JULIANDAY, o.FK4, o.FK5, o.ICRS
      //  "FROM TBL_NAMES n INNER JOIN (TBL_STELLAROBJECTS o INNER JOIN TBL_EPOCH e ON o.EPOCH_ID = e.EPOCH_ID) ON n.STELLAROBJECT_ID = o.OBJECT_ID "
       // "WHERE o.OBJECT_ID = %1").arg();

//      query.exec(szSQL);

//      QSqlError lastError = query.lastError();

//      if (query.first())
//      {
//        szSQL = query.value(0).toString();

      //  stellarObject->objectName(std::string(szSQL.toAscii()));

        //stellarObject->catalogueCoordinates(ACL::CAstronomicalCoordinates(MCL::CAngle(query.value(1).toDouble(), MCL::AF_HMSs),
        //                                                                  MCL::CAngle(query.value(2).toDouble(), MCL::AF_DMSs)));

        //if (query.value(8).toBool())
//          stellarObject->setReferenceSystem(ACL::CStellarObject::RS_FK4);
  //      else if (query.value(9).toBool())
    //      stellarObject->setReferenceSystem(ACL::CStellarObject::RS_FK5);
      //  else if (query.value(10).toBool())
        //  stellarObject->setReferenceSystem(ACL::CStellarObject::RS_ICRS);
//        else
  //        ARPAA_ERROR(0x2000);    // Reference system not specified for object.

//        stellarObject->setPM(query.value(3).toDouble(), query.value(4).toDouble());
  //      stellarObject->setRadialVelocity(query.value(5).toDouble());
    //    stellarObject->setParallax(query.value(6).toDouble());
      //  stellarObject->setEpoch(query.value(7).toDouble());
//        returnValue = true;
//      };

//      return returnValue;
    }

    bool CATID::queryStellarObjectByName_SIMBAD(std::string const &, ACL::CTargetStellar *)
    {
      bool returnValue = false;

      return returnValue;
    }

    /// @brief Procedure to open SQLite database.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect changes to CDatabase. (Bug #69)
    /// @version 2013-01-25/GGB - Function created.

    bool CATID::SQLite()
    {
      return ( createConnectionSQLite(settings::astroManagerSettings->value(settings::ATID_SQLITE_DRIVERNAME, QVariant(QString("QSQLITE"))).toString(),
                                      settings::astroManagerSettings->value(settings::ATID_SQLITE_DATABASENAME, QVariant(QString("Data/ATID.sqlite"))).toString()) );
    }

    bool ValidateObjectName(QString *toValidate)
    {
      //QString szSQL = QString("SELECT n.
      return false;
    }

    // Gets an object ID when the name is known.
    //
    // RETURN:			Invalid QVariant if name is not found otherwise QVariant with the value of the
    //							OBJECT_ID
    //
    // 2011-08-20/GGB - Function created

    QVariant GetObjectID(QString &name)
    {
      QVariant retVal;

      QString szSQL = QString("SELECT n.OBJECT_ID FROM TBL_NAMES n WHERE n.NAME = '%1'").arg(name);
      QSqlQuery query(databaseATID->database());

      query.exec(* new QString(szSQL));

      if (query.first())
        return query.value(0);
      else
        return retVal;
    };

    // Gets the number of observations for an object.
    //
    // 2010-06-25/GGB - Function created

    long long CATID::GetObservationCount(const long long objectID) const
    {
      QString szSQL = QString("SELECT Count(TBL_PHOT_OBSRECORD.Key) AS CountOfKey " \
                      "FROM TBL_PHOT_OBSRECORD " \
                      "GROUP BY TBL_PHOT_OBSRECORD.OBJECT_ID " \
                      "HAVING (((TBL_PHOT_OBSRECORD.OBJECT_ID)=%1))").arg(objectID);
      QSqlQuery query(*dBase);
      long long retVal = 0;

      query.exec(szSQL);
      retVal = query.value(0).toLongLong();

      return retVal;
    }

    //*****************************************************************************************************************************
    //
    // General Functions
    //
    //*****************************************************************************************************************************

    // Returns the values associated with the specified instrument. If the instrument is not found,
    // then an exception is thrown.
    //
    // 2010-11-07/GGB - Function Created

    void GetInstrumentDetails(long long szInstrumentName, float &fPixelX, float &fPixelY,
      long &lSensorX, long &lSensorY, float &fGain)
    {
      QString szSQL = QString("SELECT I.nX, I.nY, I.dX, I.dY, I.ADU  " \
                              "FROM tbl_Instruments I " \
                              "WHERE I.INSTRUMENT_ID = %1").arg(szInstrumentName);
      QSqlQuery query(databaseATID->database());

      query.exec(* new QString(szSQL));
      query.first();

      if (query.value(0) == QVariant::Invalid)
      {
        //ARPAA_ERROR(0xF001);
      }
      else
      {
        lSensorX = query.value(0).toInt();
        lSensorY = query.value(1).toInt();
        fPixelX = query.value(2).toFloat() * 1000;
        fPixelY = query.value(3).toFloat() * 1000;
        fGain = query.value(4).toFloat();
      };
    }

    // Gets the filter long text and short text
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2010-11-13/GGB - Function Created

    void GetFilterDetails(const long long FilterName, std::string &shortText, std::string &longText)
    {
      QString szSQL = QString("SELECT f.SHORTTEXT, f.LONGTEXT " \
                              "FROM TBL_FILTERS f " \
                              "WHERE f.FILTER_ID = %1").arg(FilterName);
      QSqlQuery query(databaseATID->database());

      if ( !query.exec(* new QString(szSQL)) )
      {
        //ARPAA_ERROR(0x1000);
      }

      query.first();

      if (query.value(0) == QVariant::Invalid)
      {
        //ARPAA_ERROR(0xF002);
      }

      shortText = query.value(0).toString().toStdString();
      longText = query.value(1).toString().toStdString();
    }

    // Function to populate a combo box with spectral type information.
    // The data is the index of record.
    //
    // 2010-11-14/GGB

    void CATID::PopulateSpectralClassCombo(QComboBox *combo)
    {
      QString szSQL = "SELECT s.SPECTRALCLASS_ID, s.SHORTTEXT FROM TBL_SPECTRALCLASS s";
      QSqlQuery query(*(databaseATID->dBase));

      query.exec(* new QString(szSQL));
      while (query.next())
      {
        combo->addItem(*new QString(query.value(1).toString()), *new QVariant(query.value(0)));
      };
    }

    // Function to populate a combo box with constellation names.
    // The data is the index of the record.
    //
    // 2013-07-29/GGB - Added check for ATID disabled.
    // 2010-11-14/GGB - Function created

    void CATID::PopulateConstellationsCombo(QComboBox *combo)
    {
      if (!ATIDdisabled_)
      {
        QString szSQL = "SELECT c.CONSTELLATION_ID, c.LONGTEXT FROM TBL_CONSTELLATIONS c";
        QSqlQuery query(*(databaseATID->dBase));

        query.exec(* new QString(szSQL));
        while (query.next())
        {
          combo->addItem(*new QString(query.value(1).toString()), *new QVariant(query.value(0)));
        };
      };
    }

    // Populates a combo box with catalog information
    //
    // 2010-11-17/GGB - Function created

    void CATID::PopulateCatalogCombo(QComboBox *combo)
    {
      QString szSQL = "SELECT c.CATALOGUE_ID, c.[CATALOGUENAME] FROM TBL_CATALOGUE c";
      QSqlQuery query(*(databaseATID->dBase));

      query.exec(* new QString(szSQL));
      while (query.next())
      {
        combo->addItem(*new QString(query.value(1).toString()), *new QVariant(query.value(0)));
      };
    }

    // Populates a combo box with photometry program information
    //
    // 2010-11-17/GGB - Function created

    void CATID::PopulatePhotometryProgramCombo(QComboBox *combo)
    {
      QSqlQuery *query;
      char szSQL[2048] = "SELECT pp.SHORTTEXT, pp.OBSPROG_ID " \
                         "FROM TBL_OBSPROGRAM pp " \
                         "WHERE (pp.PROGCOMPLETE = 0) AND (pp.PROGCANCELLED = 0) AND pp.PHOTPROG <> 0";
      QString *program;

      query = new QSqlQuery(*(databaseATID->dBase));
      query->setForwardOnly(true);		// ForwardOnly only needed
      program = new QString(szSQL);
      query->exec(*program);
      program = NULL;

      while (	query->next() )
      {
        program = new QString(query->value(0).toString());
        combo->addItem(*program, query->value(1));
      };

      delete query;
      query = nullptr;
    }

  }
}  // namespace AstroManager
