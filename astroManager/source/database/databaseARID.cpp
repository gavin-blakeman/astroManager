//*********************************************************************************************************************************
//
// PROJECT:							AstroManager
// FILE:			          DatabaseARID
// SUBSYSTEM:           Data support functions
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// AUTHOR:              Gavin BLakeman (GGB)
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
// OVERVIEW:            Data support functions.
//
// CLASSES INCLUDED:    CARID
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                       - CdatabaseWeather
//
// HISTORY:             2018-02-03 GGB - Changed application name to AstroManager.
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2013-03-01 GGB - Split CARID into it's own file.
//                      2013-02-24 GGB - Split CATID into it's own file
//                      2013-01-20 GGB - Release of astroManager 0000.00
//                      2012-01-04 GGB - Classes developed for astroManager
//
//*********************************************************************************************************************************

#include "../../include/database/databaseARID.h"

  // Standard C++ library header files

#include <cstdint>
#include <limits>

  // astroManager application headerfiles

#include "../../include/database/databaseATID.h"
#include "../../include/dialogs/dialogConfigureSite.h"
#include "../../include/dialogs/dialogConfigureTelescope.h"
#include "../../include/dialogs/dialogImageDetails.h"
#include "../../include/error.h"
#include "../../include/settings.h"
#include "../../include/astroManager.h"

  // Miscellaneous library header files

#include <ACL>
#include "GeographicLib/Geodesic.hpp"
#include "GeographicLib/Math.hpp"
#include <QCL>

namespace astroManager
{
  namespace database
  {

    CARID *databaseARID = nullptr;

    //*****************************************************************************************************************************
    //
    // ARID - Astronomical Research Information Database
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @throws std::bad_alloc
    /// @version 2017-06-21/GGB - Added sqlWriter information.
    /// @version 2017-06-20/GGB - Updated to match changes to CDatabase. (Bug #69)
    /// @version 2013-05-15/GGB - Added code to check if the database is disabled. (Will be disabled by default!)
    /// @version 2013-01-26/GGB - Body of code moved to function connectToDatabase.
    /// @version 2013-01-25/GGB - Added SQLite support
    /// @version 2012-01-01/GGB - Function created

    CARID::CARID() : CDatabase("ARID"), ARIDdisabled_(false)
    {
      QVariant variant = settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE, QVariant());

      if (variant.isNull())
      {
        settings::astroManagerSettings->setValue(settings::ARID_DATABASE_DISABLE, QVariant(true));
        ARIDdisabled_ = true;
      }
      else
      {
        ARIDdisabled_ = variant.toBool();
      };

      sqlWriter.createTable("TBL_SITES");
        sqlWriter.createColumn("TBL_SITES", "SITE_ID");
        sqlWriter.createColumn("TBL_SITES", "SHORTTEXT");
        sqlWriter.createColumn("TBL_SITES", "Latitude");
        sqlWriter.createColumn("TBL_SITES", "Longitude");
        sqlWriter.createColumn("TBL_SITES", "Altitude");
        sqlWriter.createColumn("TBL_SITES", "TimeZone");
        sqlWriter.createColumn("TBL_SITES", "DontDisplay");
        sqlWriter.createColumn("TBL_SITES", "IAUCode");
        sqlWriter.createColumn("TBL_SITES", "DAYLIGHTSAVING");
      sqlWriter.createTable("TBL_OBSERVERS");
        sqlWriter.createColumn("TBL_OBSERVERS", "OBSERVER_ID");
        sqlWriter.createColumn("TBL_OBSERVERS", "SHORTTEXT");
        sqlWriter.createColumn("TBL_OBSERVERS", "AAVSOINITIALS");
        sqlWriter.createColumn("TBL_OBSERVERS", "DontDisplay");
      sqlWriter.createTable("TBL_TELESCOPES");
        sqlWriter.createColumn("TBL_TELESCOPES", "TELESCOPE_ID");
        sqlWriter.createColumn("TBL_TELESCOPES", "ShortText");
        sqlWriter.createColumn("TBL_TELESCOPES", "Manufacturer");
        sqlWriter.createColumn("TBL_TELESCOPES", "Model");
        sqlWriter.createColumn("TBL_TELESCOPES", "Aperture");
        sqlWriter.createColumn("TBL_TELESCOPES", "FRATIO");
        sqlWriter.createColumn("TBL_TELESCOPES", "OBSTRUCTION");
        sqlWriter.createColumn("TBL_TELESCOPES", "DoneDisplay");
       sqlWriter.createTable("TBL_INSTRUMENTS");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "INSTRUMENT_ID");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "SHORTTEXT");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "Manufacturer");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "Model");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "nX");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "nY");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "dX");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "dY");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "DontDisplay");
        sqlWriter.createColumn("TBL_INSTRUMENTS", "ADU");
      sqlWriter.createTable("TBL_FILTERS");
        sqlWriter.createColumn("TBL_FILTERS", "FILTER_ID");
        sqlWriter.createColumn("TBL_FILTERS", "SHORTTEXT");
        sqlWriter.createColumn("TBL_FILTERS", "DESCRIPTION");
      sqlWriter.createTable("TBL_IMAGES");
        sqlWriter.createColumn("TBL_IMAGES", "IMAGE_ID");
        sqlWriter.createColumn("TBL_IMAGES", "IMAGE_UUID");
        sqlWriter.createColumn("TBL_IMAGES", "FILENAME");
        sqlWriter.createColumn("TBL_IMAGES", "FILEPATH");
        sqlWriter.createColumn("TBL_IMAGES", "IMAGEDATE");
        sqlWriter.createColumn("TBL_IMAGES", "SITE_ID");
        sqlWriter.createColumn("TBL_IMAGES", "TELESCOPE_ID");
        sqlWriter.createColumn("TBL_IMAGES", "dX");
        sqlWriter.createColumn("TBL_IMAGES", "dY");
        sqlWriter.createColumn("TBL_IMAGES", "RA");
        sqlWriter.createColumn("TBL_IMAGES", "Declination");
    }

    /// @brief Destructor for the class. Ensures that the database connection is removed.
    /// @throws None.
    /// @version 2013-05-15/GGB - Conditional removal of the database.
    /// @version 2010-11-28/GGB - Function created.

    CARID::~CARID()
    {
      if (!ARIDdisabled_)
      {
        dBase->close();
        QSqlDatabase::removeDatabase(szConnectionName);
        delete dBase;
        dBase = nullptr;
      };
    }

    /// @brief Connects to the database.
    /// @details In addition to creating the connection, the sqlQuery member is also initialised.
    /// @throws std::bad_alloc
    /// @version 2017-08-13/GGB - Create the sqlQuery instance.
    /// @version 2017-07-09/GGB - Updated logic to reflect new CDatabase functions.
    /// @version 2017-06-20/GGB - Correcting error handling. (Bug #70)
    /// @version 2015-06-14/GGB - Corected bug found with SQLite entry being created everytime.
    /// @version 2013-05-15/GGB - Conditional connection to database if not disabled.
    /// @version 2013-04-25/GGB - Function simplified and will create registry settings if none exist.
    /// @version 2013-01-26/GGB - Function created

    void CARID::connectToDatabase()
    {
      QVariant database = settings::astroManagerSettings->value(settings::ARID_DATABASE_DBMS);
      QString szDatabase;

      if (!ARIDdisabled_)
      {
        if ( !database.isNull() )
        {
          szDatabase = database.toString();		// Get the database type

          if (!CDatabase::connectToDatabase(szDatabase))
          {
            INFOMESSAGE("Unable to connect to ARID database. Disabling ARID database.");
            ARIDdisabled_ = true;
          }
          else
          {
            sqlQuery.reset(new QSqlQuery(*dBase));
          }
        }
        else
        {
          WARNINGMESSAGE("Setting " + settings::ARID_DATABASE_DBMS.toStdString() + "not found.");
          INFOMESSAGE("Unable to connect to ARID database. Disabling ARID database.");
          ARIDdisabled_ = true;
        };
      };
    }

    /// @brief Downloads an image from the database.
    /// @param[in] imageID: The ID of the image to download.
    /// @param[in] imageVersion: The version of the image to download.
    /// @param[out] byteArray: The QByteArray to receive the downloaded image.
    /// @throws None.
    /// @version 2017-08-12/GGB - Function created.

    bool CARID::downLoadImage(imageID_t imageID, imageVersion_t imageVersion, QByteArray &byteArray)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        byteArray.clear();    // Ensure the byteArray is empty.

        sqlWriter.resetQuery();
        sqlWriter.select({"IMAGE_DATA"}).from({"TBL_IMAGESTORAGE"}).
            where({GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID),
                   GCL::sqlwriter::parameterTriple(std::string("IMAGE_VERSION"), std::string("="), imageVersion)});
        if (query.exec(QString::fromStdString(sqlWriter.string())))
        {
          query.first();
          if (query.isValid())
          {
            byteArray = query.value(0).toByteArray();
            returnValue = true;
          }
          else
          {
            QSqlError error = query.lastError();

            ERRORMESSAGE("CARID::downLoadImage: Error with SQL Query.");

            ERRORMESSAGE("Query: " + sqlWriter.string());
            ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
            ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
            ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
          }
        }
        else
        {
          QSqlError error = query.lastError();

          ERRORMESSAGE("CARID::downLoadImage - Error when executing query.");
          ERRORMESSAGE("Query: " + sqlWriter.string());
          ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
          ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
          ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
        }
      }
      else
      {
        ASTROMANAGER_CODE_ERROR;
      };

      return returnValue;
    }

    /// @brief Finds the observing site closest to the coordinate passed in observatory.
    /// @param[in/out] observatory: The coordinates to search for the observing site. The site name, ID and IAUCode are written back.
    /// @returns true - Site found
    /// @returns false  - Site not found.
    /// @throws None.
    /// @version 2017-07-25/GGB - Function created.

    bool CARID::findObservingSite(CObservatory *observatory)
    {
      static std::uint32_t lastFound = 0;     // Stores the last found observatory. This is a convenient place to start searching.

      bool returnValue = false;
      QSqlQuery query(*dBase);
      std::string sqlString;
      GeographicLib::Math::real distance;
      GeographicLib::Geodesic geodesic(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

      if (lastFound != 0)
      {
        sqlWriter.resetQuery();
        sqlString = sqlWriter.select({"SITE_ID", "SHORTTEXT", "LATITUDE, LONGITUDE, ALTITUDE, TIMEZONE, IAUCODE"})
                             .from({"TBL_SITES"})
                             .where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), "false"),
                                      GCL::sqlwriter::parameterTriple(std::string("SITE_ID"), std::string("="), lastFound)
                                     }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
            // Check the first value. (There should only be one).

          query.next();   // Move to first valid record.

          geodesic.Inverse(query.value(2).toDouble(), query.value(3).toDouble(),
                           observatory->latitude(), observatory->longitude(), distance);

          if (distance <= settings::astroManagerSettings->value(settings::SETTINGS_SITE_SAMEDISTANCE, QVariant(500)).toDouble())
          {
            INFOMESSAGE("Succesfully found site from coordinates...");
            INFOMESSAGE("Site Identified: " + query.value(1).toString().toStdString());

            observatory->siteID(query.value(0).toUInt());
            observatory->siteName(query.value(1).toString().toStdString());
            if (!query.value(6).isNull())
            {
              observatory->IAUCode(query.value(6).toString().toStdString());
            }
            else
            {
              observatory->IAUCode("");
            };
            observatory->altitude(query.value(4).toInt());
            observatory->timeZone(query.value(5).toInt());

            INFOMESSAGE("Succesfully found site from coordinates...");
            INFOMESSAGE("Site Identified: " + observatory->siteName());
            INFOMESSAGE("Distance from Observation to Site: " + std::to_string(distance) + "m");

            returnValue = true;
          };
        };
      };

      if (!returnValue)
      {
          // Site not found yet.

        CObservatory closestSite;
        GeographicLib::Math::real closestDistance = std::numeric_limits<GeographicLib::Math::real>::max();
        bool possibleFound = false;

        sqlWriter.resetQuery();
        sqlString = sqlWriter.select({"SITE_ID", "SHORTTEXT", "LATITUDE, LONGITUDE, ALTITUDE, TIMEZONE, IAUCODE"})
                             .from({"TBL_SITES"})
                             .where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), "false"), })
                             .string();

        std::cout << sqlString << std::endl;

         if (query.exec(QString::fromStdString(sqlString)))
        {
          while (query.next())
          {
            geodesic.Inverse(query.value(2).toDouble(), query.value(3).toDouble(),
                             observatory->latitude(), observatory->longitude(), distance);

            if (distance <= settings::astroManagerSettings->value(settings::SETTINGS_SITE_SAMEDISTANCE, QVariant(500)).toDouble())
            {
                // Found a possible site. Store it and continue to the next site.

              if (distance < closestDistance)
              {
                possibleFound = true;
                closestDistance = distance;
                closestSite.siteID(query.value(0).toUInt());
                closestSite.siteName(query.value(1).toString().toStdString());
                if (!query.value(6).isNull())
                {
                  closestSite.IAUCode(query.value(6).toString().toStdString());
                }
                else
                {
                  closestSite.IAUCode("");
                };
                closestSite.latitude(query.value(2).toDouble());
                closestSite.longitude(query.value(3).toDouble());
                closestSite.altitude(query.value(4).toInt());
                closestSite.timeZone(query.value(5).toInt());
              };
            };
          };
          if (possibleFound)
          {
            if (closestDistance < settings::astroManagerSettings->value(settings::SETTINGS_SITE_SAMEDISTANCE, QVariant(500)).toDouble())
            {
                // Take the site as found and assign it.

              *observatory = closestSite;     // Automatically generated copy operator.

              INFOMESSAGE("Succesfully found site from coordinates...");
              INFOMESSAGE("Site Identified: " + observatory->siteName());
              INFOMESSAGE("Distance from Observation to Site: " + std::to_string(closestDistance) + "m");

              returnValue = true;
            };
          };
        }
        else
        {
          processErrorInformation();
        };
      };

      return returnValue;
    }

    /// @brief Searches the database to find the details of a telescope that is defined in telescope.
    /// @param[in/out] telescope: Structure with data that will be filled with database data.
    /// @returns true - telescope found
    /// @returns false - telescope not found.
    /// @throws GCL::CRuntimeAssert(...)
    /// @versio 2017-08-05/GGB - Function created.

    bool CARID::findTelescope(CTelescope *telescope)
    {
      RUNTIME_ASSERT(astroManager, !telescope->telescopeName().empty(), "Telscope Name cannot be empty string." );

      bool returnValue = false;

      sqlWriter.resetQuery();
      sqlWriter.select({"TELESCOPE_ID", "MANUFACTURER", "MODEL", "APERTURE", "FOCALLENGTH", "OBSTRUCTION"})
          .from({"TBL_TELESCOPES"})
          .where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), "false"),
                   GCL::sqlwriter::parameterTriple(std::string("SHORTTEXT"), std::string("="), telescope->telescopeName()) });

      sqlQuery->clear();

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        sqlQuery->next();
        if (sqlQuery->isValid())
        {
          std::uint_least8_t index = 0;

          telescope->telescopeID() = sqlQuery->value(index++).toUInt();
          telescope->manufacturer() = sqlQuery->value(index++).toString().toStdString();
          telescope->model() = sqlQuery->value(index++).toString().toStdString();
          telescope->aperture() = sqlQuery->value(index++).toDouble();
          telescope->focalLength() = sqlQuery->value(index++).toDouble();
          telescope->obstruction() = sqlQuery->value(index++).toDouble();

          INFOMESSAGE("Telescope Found: " + telescope->telescopeName());

          returnValue = true;
        }
        else
        {
          ERRORMESSAGE("Unable to find telescope.");
          processErrorInformation();
        }
      }
      else
      {
        ERRORMESSAGE("Unable to find telescope.");
        processErrorInformation();
      };

      return returnValue;
    }

    /// @brief Retrieves an imageName from the database.
    /// @param[in] imageID: The ID of the image to get the name.
    /// @param[out] imageName: The retrieved name of the image.
    /// @returns true - image found and imageName valid.
    /// @returns false - image not found. imageName invalid.
    /// @throws None.
    /// @version 2017-08-13/GGB - Function created.

    bool CARID::getImageName(imageID_t imageID, std::string &imageName)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();
        sqlWriter.select({"IMAGENAME"}).from({"TBL_IMAGES"}).
            where({ GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID) });

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          sqlQuery->first();
          if (sqlQuery->isValid())
          {
              // Image found.

            imageName = sqlQuery->value(0).toString().toStdString();
            returnValue = true;
          }
          else
          {
            processErrorInformation();
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      }
      else
      {
        DEBUGMESSAGE(QObject::tr("ARID Database is disabled.").toStdString());
      };

      return returnValue;
    }

    /// @brief Gets information on a specified observing site (if available).
    /// @param[in] siteID: The ID of the site to retrieve information for.
    /// @param[out] site: The object to store the information in.
    /// @returns true - Success
    /// @returns false - The object was not found.
    /// @throws None.
    /// @version 2018-02-02/GGB - Function created.

    bool CARID::getObservingSite(std::uint32_t siteID, CObservatory *site)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();
        sqlWriter.select({"SHORTTEXT", "LATITUDE", "LONGITUDE", "ALTITUDE", "TIMEZONE", "DONTDISPLAY", "IAUCODE", "DAYLIGHTSAVING"}).
            from({"TBL_SITES"}).
            where({ GCL::sqlwriter::parameterTriple(std::string("SITE_ID"), std::string("="), siteID) });

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          sqlQuery->first();
          if (sqlQuery->isValid())
          {
              // Site found.

            site->siteID(siteID);
            site->siteName(sqlQuery->value(0).toString().toStdString());
            site->latitude(sqlQuery->value(1).toDouble());
            site->longitude(sqlQuery->value(2).toDouble());
            site->altitude(sqlQuery->value(3).toInt());
            site->timeZone(sqlQuery->value(4).toDouble());
            site->dontDisplay(sqlQuery->value(5).toBool());
            site->IAUCode(sqlQuery->value(6).toString().toStdString());
            site->daylightSaving(sqlQuery->value(7).toUInt());

            returnValue = true;
          }
          else
          {
            processErrorInformation();
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      }
      else
      {
        DEBUGMESSAGE(QObject::tr("ARID Database is disabled.").toStdString());
      };

      return returnValue;
    }

    /// @brief Gets the timezone offset from the database.
    /// @param[in] siteID: The site ID to use.
    /// @param[out] offset: The offset value from the site ID.
    /// @returns true - Success
    /// @returns false - Unable to find site.
    /// @throws None.
    /// @version 2018-04-15/GGB - Function created.

    bool CARID::getTimeZoneOffset(std::uint32_t siteID, std::int_least32_t *offset)
    {
      RUNTIME_ASSERT("astroManager", offset != nullptr, "parameter offset cannot be nullptr.");

      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.select({"TimeZone"}).
            from({"TBL_SITES"}).
            where({ GCL::sqlwriter::parameterTriple(std::string("SITE_ID"), std::string("="), siteID) });

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          sqlQuery->first();
          if (sqlQuery->isValid())
          {
              // Site found.

            *offset = sqlQuery->value(0).toString().toInt();

            returnValue = true;
          }
          else
          {
            processErrorInformation();
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      };

      return returnValue;
    }

    /// @brief Deletes an image. This includes the meta-data and the image data.
    /// @param[in] imageID: The ID of the image to delete.
    /// @note 1. All image versions are deleted.
    /// @note 2. All image data is deleted.
    /// @note 3. The image master record is deleted.
    /// @returns true - The imageData was deleted.
    /// @returns false - The imageData was not deleted.
    /// @throws None.
    /// @version 2018-05-12/GGB - Function created.

    bool CARID::imageDeleteImage(imageID_t imageID)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

          // Delete the image data first.

        sqlWriter.deleteFrom("TBL_IMAGESTORAGE").where("IMAGE_ID", "=", imageID);

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          sqlWriter.deleteFrom("TBL_IMAGES").where("IMAGE_ID", "=", imageID);
          if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
          {
            returnValue = true;
          }
          else
          {
            processErrorInformation();
          };
        }
        else
        {
          processErrorInformation();
        };
      }
      else
      {
        DEBUGMESSAGE(QObject::tr("ARID Database is disabled.").toStdString());
      };

      return returnValue;
    }

    /// @brief Function to delete all the image data (not the master record) associated with the imageID
    /// @param[in] imageID: The ID of the image to delete.
    /// @note 1. All image versions are deleted.
    /// @note 2. All image version data is deleted.
    /// @returns true - The imageData was deleted.
    /// @returns false - The imageData was not deleted.
    /// @throws None.
    /// @version 2018-05-12/GGB - Function created.

    bool CARID::imageDeleteImageData(imageID_t imageID)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.deleteFrom("TBL_IMAGESTORAGE").where("IMAGE_ID", "=", imageID);

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          returnValue = true;
        }
        else
        {
          processErrorInformation();
        };
      }
      else
      {
        DEBUGMESSAGE(QObject::tr("ARID Database is disabled.").toStdString());
      };

      return returnValue;
    }

    /// @brief Deletes the specified version of an image.
    /// @param[in] imageID: The ID of the image.
    /// @param[in] imageVersion: The version of the image to delete.
    /// @returns true - The imageData was deleted.
    /// @returns false - The imageData was not deleted.
    /// @version 2018-05-12/GGB - Function created.

    bool CARID::imageDeleteImageData(imageID_t imageID, imageVersion_t)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.deleteFrom("TBL_IMAGESTORAGE").where("IMAGE_ID", "=", imageID);

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          returnValue = true;
        }
        else
        {
          processErrorInformation();
        };
      }
      else
      {
        DEBUGMESSAGE(QObject::tr("ARID Database is disabled.").toStdString());
      }

      return returnValue;
    }


    /// @brief Checks if an image is registered.
    /// @param[in] fileName: The filename to check
    /// @returns true - The image is registered
    /// @returns false - The image is not registered.
    /// @throws 0x4000:
    /// @version 2017-08-05/GGB - Function created.

    bool CARID::isImageNameRegistered(std::string const &imageName)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        std::string sqlString = sqlWriter.select({"IMAGE_ID"}).from({"TBL_IMAGES"}).
            where({
                    GCL::sqlwriter::parameterTriple(std::string("IMAGENAME"), std::string("="), imageName),
                  }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
          query.first();
          if (query.isValid())
          {
              // Image found.

            returnValue = true;
          };
        }
        else
        {
          processErrorInformation();
          ASTROMANAGER_ERROR(0x4000);
        }
      }
      else
      {
        DEBUGMESSAGE(QObject::tr("ARID Database is disabled.").toStdString());
      }

      return returnValue;
    }

    /// @brief Checks if an image has been registered with the same file name.
    /// @param[in] fileName: The file name to check.
    /// @param[out] uuid: The UUID of the image if the image has been registered. Undefined if the image has not been registered.
    /// @returns true - The file name is already registered.
    /// @returns false - The file name is not registered
    /// @throws
    /// @version 2017-07-23/GGB - Function created

    bool CARID::isImageNameRegistered(std::string const &imageName, QString &uuid)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        std::string sqlString = sqlWriter.select({"IMAGE_UUID"}).from({"TBL_IMAGES"}).
            where({
                    GCL::sqlwriter::parameterTriple(std::string("IMAGENAME"), std::string("="), imageName),
                  }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
          query.first();
          if (query.isValid())
          {
              // Image found.

            uuid = query.value(0).toString();
            returnValue = true;
          };
        }
        else
        {
          processErrorInformation();
          ASTROMANAGER_ERROR(0x4000);
        }
      }
      else
      {
        DEBUGMESSAGE("ARID Database is disbled.");
      }

      return returnValue;
    }

    /// @brief Checks if an image has been registered with the same file name.
    /// @param[in] fileName: The file name to check.
    /// @param[out] imageId: The ID f the image if the image has been registered. Undefined if the image has not been registered.
    /// @returns true - The file name is already registered.
    /// @returns false - The file name is not registered
    /// @throws GCL::CError(astroManager, 0x4000)
    /// @version 2017-07-26/GGB - Function created

    bool CARID::isImageNameRegistered(std::string const &imageName, uint32_t &imageId)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        std::string sqlString = sqlWriter.select({"IMAGE_ID"}).from({"TBL_IMAGES"}).
            where({
                    GCL::sqlwriter::parameterTriple(std::string("IMAGENAME"), std::string("="), imageName),
                  }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
          query.first();
          if (query.isValid())
          {
              // Image found.

            imageId = query.value(0).toUInt();
            returnValue = true;
          };
        }
        else
        {
          processErrorInformation();
          ASTROMANAGER_ERROR(0x4000);
        }
      }
      else
      {
        DEBUGMESSAGE("ARID Database is disbled.");
      }

      return returnValue;
    }

    /// @brief Determines if the UUID has been registered.
    /// @param[in] UUID: The UUID to check for registration.
    /// @param[out] imageID: The imageID correesponding to the UUID.
    /// @returns true - UUID is registered and imageID is a valid ID.
    /// @returns false - The UUID is not registered.
    /// @throws
    /// @version 2017-09-02/GGB - Function created.

    bool CARID::isImageUUIDRegistered(QUuid const &UUID, imageID_t &imageID)
    {
      bool returnValue = false;

      sqlWriter.resetQuery();

      sqlWriter.select({"IMAGE_ID"}).from({"TBL_IMAGES"}).
          where({
                  GCL::sqlwriter::parameterTriple(std::string("IMAGE_UUID"), std::string("="), UUID.toString().toStdString()),
                });

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        sqlQuery->first();
        if (sqlQuery->isValid())
        {
          imageID = sqlQuery->value(0).toUInt();
          returnValue = true;
        }
        else
        {
          processErrorInformation();
        };
      }
      else
      {
        processErrorInformation();
      };

      return returnValue;
    }

    /// @brief Loads default data into internal data structures.
    /// @details This includes
    ///   @li Photometry Filter Data
    /// @throws
    /// @version 2017-07-23/GGB - Function created.

    void CARID::loadDefaultData()
    {
      loadPhotometryFilterData();
    }

    /// @brief Loads the photometry filter data.
    /// @throws
    /// @version 2017-07-23/GGB - Function created.

    void CARID::loadPhotometryFilterData()
    {
      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        std::string sqlString = sqlWriter.select({"FILTER_ID", "SHORTTEXT", "DESCRIPTION"}).from({"TBL_FILTERS"}).
            where({
                    GCL::sqlwriter::parameterTriple(std::string("OBSOLETE"), std::string("="), "false"),
                  }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
          // Iterate all the returned values and add process witht he Photometry filter connection.

          while (query.next())
          {
            ACL::CPhotometryFilterCollection::processDatabaseEntry(query.value(0).toUInt(),
                                                                   query.value(1).toString().toStdString(),
                                                                   query.value(2).toString().toStdString());
          }
          DEBUGMESSAGE("loadPhotometryFilterData: Sucessfully processed Photometry Filter Data.");
        }
        else
        {
          processErrorInformation();
        }
      }
      else
      {
        DEBUGMESSAGE("loadPhotometryFilterData: ARID Database is disbled.");
      }
    }

    /// @brief Function for connecting to a MySQL database.
    /// @details Reads the information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect new CDatabase functionality. (Bug #69)
    /// @version 2015-06-13/GGB - Use databaseDrivers static member for driver names.
    /// @version 2011-06-06/GGB - Function Created

    bool CARID::MySQL()
    {
      return ( createConnection(settings::astroManagerSettings->value(settings::ARID_MYSQL_DRIVERNAME, QVariant(databaseDrivers[SQLDB_MYSQL].driverName)).toString(),
                                settings::astroManagerSettings->value(settings::ARID_MYSQL_HOSTADDRESS, QVariant(QString("localhost"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_MYSQL_PORT, QVariant(3306)).toInt(),
                                settings::astroManagerSettings->value(settings::ARID_MYSQL_DATABASENAME, QVariant(QString("ARID"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_MYSQL_USERNAME, QVariant(QString("ARID"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_MYSQL_PASSWORD, QVariant(QString("ARID"))).toString()) );
    }

    /// @brief Function for opening an ODBC database.
    /// @details Reads information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect new CDatabase functionality. (Bug #69)
    /// @version 2015-06-14/GGB - Updated to reflect the settings changing to constants.
    /// @version 2010-12-09/GGB - Function created.

    bool CARID::ODBC()
    {
      return ( createConnectionODBC(settings::astroManagerSettings->value(settings::ARID_ODBC_DRIVERNAME, QVariant(QString("QODBC"))).toString(),
                                    settings::astroManagerSettings->value(settings::ARID_ODBC_DATASOURCENAME, QVariant(QString("MSA ATID"))).toString()) );
    }

    /// @brief Populates the fields in the imageDetails dialog.
    /// @param[in] dialog - Pointer to the dialog
    /// @throws None.
    /// @version 2017-08-17/GGB - Function created.

    void CARID::populateDialogImageDetails(dialogs::CDialogImageDetails *dialog)
    {
      sqlQuery->clear();
      sqlWriter.resetQuery();

      sqlWriter.select({"TBL_IMAGES.IMAGE_UUID", "TBL_IMAGES.TARGET", "TBL_IMAGES.FILEPATH", "TBL_FILTERS.SHORTTEXT",
                        "TBL_SITES.SHORTTEXT", "TBL_TELESCOPES.SHORTTEXT", "TBL_IMAGES.IMAGEDATE", "TBL_IMAGES.IMAGETIME",
                        "TBL_IMAGES.RA", "TBL_IMAGES.DECLINATION", "TBL_IMAGES.QUALITY", "TBL_IMAGES.SYNTHETIC",
                        "TBL_IMAGES.ASTROMETRIC", "TBL_IMAGES.PHOTOMETRIC", "TBL_IMAGES.IMAGE_ID", "COMMENTS"}).from("TBL_IMAGES")
          .join({std::make_tuple("TBL_IMAGES", "FILTER_ID", GCL::sqlwriter::CSQLWriter::JOIN_LEFT, "TBL_FILTERS", "FILTER_ID"),
                 std::make_tuple("TBL_IMAGES", "SITE_ID", GCL::sqlwriter::CSQLWriter::JOIN_LEFT, "TBL_SITES", "SITE_ID"),
                 std::make_tuple("TBL_IMAGES", "TELESCOPE_ID", GCL::sqlwriter::CSQLWriter::JOIN_LEFT, "TBL_TELESCOPES", "TELESCOPE_ID")})
          .where("TBL_IMAGES.IMAGE_ID", "=", dialog->imageID_);

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        sqlQuery->first();
        if (sqlQuery->isValid())
        {
          dialog->labelImageID->setText(sqlQuery->value(14).toString());
          dialog->labelImageUUID->setText(sqlQuery->value(0).toUuid().toString());
          dialog->labelFilePath->setText(sqlQuery->value(2).toString());
          dialog->labelImageDate->setText(QString::fromStdString(ACL::TJD(sqlQuery->value(6).toDouble()).gregorianDate()));
          dialog->labelImageTime->setText(QString::fromStdString(GCL::sprintfHMS(sqlQuery->value(7).toUInt())));
          dialog->labelObservingSite->setText(sqlQuery->value(4).toString());
          dialog->labelTelescope->setText(sqlQuery->value(5).toString());
          dialog->labelTarget->setText(sqlQuery->value(1).toString());
          dialog->comboBoxQuality->setCurrentIndex(sqlQuery->value(10).toUInt()-1);
          dialog->labelFilter->setText(sqlQuery->value(3).toString());
          dialog->labelCoordinates->setText("(" +
                                            QString::fromStdString(GCL::sprintfHMS(sqlQuery->value(8).toDouble())) + ", " +
                                            QString::fromStdString(GCL::sprintfDMS(sqlQuery->value(9).toDouble())) + ")");

          dialog->groupBoxSyntheticImage->setChecked(sqlQuery->value(11).toBool());
          dialog->checkBoxAstrometric->setChecked(sqlQuery->value(12).toBool());
          dialog->checkBoxPhotometric->setChecked(sqlQuery->value(13).toBool());
          dialog->plainTextEditComments->setPlainText(sqlQuery->value(15).toString());
        }
        else
        {
          processErrorInformation();
        };
      }
      else
      {
        processErrorInformation();
      };

    }

    /// @brief Function to save the original image.
    /// @param[in] astroFile: Pointer to the astroFile that needs to be saved.
    /// @note 1. When an original image is saved, it is always saved as version 0. Any other image saves will only save from
    ///          revision 1.
    /// @note 2. While the astroFile contains the information to be saved, the actual information that will be written is on the
    ///          disk and pointed to by the filename in the astroFile.
    /// @note 3. This function can be called before the astroFile has been loaded.
    /// @throws GCL::CError(astroManager, 0x4001)
    /// @version 2017-07-25/GGB - Function created.

    void CARID::saveOriginalImage(CAstroFile *astroFile)
    {
      std::uint32_t imageID;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        INFOMESSAGE("Beginning saving original image...");

          // Save the TBL_IMAGE data. IE register the image. In this case we do not have complete data as the file has not been
          // opened yet.
          // Before doing this, lets check if the image is already registered.

        if (!isImageNameRegistered(astroFile->getImageName(), imageID))
        {
          registerImage(astroFile);
          isImageNameRegistered(astroFile->getImageName(), imageID);
        }
        else
        {
          INFOMESSAGE("The image is already registered!");
        };

          // We now have the imageID.
          // Start by checking if there is already an image with version 0 entered into the image storage table.

        sqlWriter.resetQuery();
        std::string sqlString = sqlWriter.select({"ID"}).from({"TBL_IMAGESTORAGE"}).
            where({
                    GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID),
                    GCL::sqlwriter::parameterTriple(std::string("IMAGE_VERSION"), std::string("="), 0),
                  }).string();

        if (query.exec(QString::fromStdString(sqlString)))
        {
          query.first();

          if (!query.isValid())
          {
              // Image not in database.

            uploadImage(QString::fromStdString(astroFile->getFileName().string()), imageID, 0, "Original Image");

          }
          else
          {
            INFOMESSAGE("Found existing image in database.");
            INFOMESSAGE("Image Not added to database.");
          };
        }
        else
        {
          processErrorInformation();
        };
      }
      else
      {
        DEBUGMESSAGE("ARID Database is disbled.");
      }
    }

    /// @brief Function to open an SQLite database.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Updated to reflect new CDatabase functionality (Bug #69)
    /// @version 2013-01-25/GGB - Function created.

    bool CARID::SQLite()
    {
      return ( createConnectionSQLite(settings::astroManagerSettings->value(settings::ARID_SQLITE_DRIVERNAME, QVariant(QString("QSQLITE"))).toString(),
                                      settings::astroManagerSettings->value(settings::ARID_SQLITE_DATABASENAME, QVariant(QString("Data/ARID.sqlite"))).toString()) );
    }

    /// @brief Function for opening an Oracle database.
    /// @details Reads the relevant information from the settings and then creates the database connection.
    /// @returns true - Connection created.
    /// @returns false - Connection not created.
    /// @throws None.
    /// @version 2017-07-01/GGB - Return type changed from void to bool.
    /// @version 2017-06-20/GGB - Function created.
    /// @version 2015-06-14/GGB - Updated as settings changed to constants.
    /// @version 2011-02-25/GGB - Added code to capture errors and throw exceptions when settings not found.
    /// @version 2010-11-28/GGB - Function created.

    bool CARID::Oracle()
    {
      return ( createConnection(settings::astroManagerSettings->value(settings::ARID_ORACLE_DRIVERNAME, QVariant(QString("QOCI"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_ORACLE_HOSTADDRESS, QVariant(QString("localhost"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_ORACLE_PORT, QVariant(1521)).toInt(),
                                settings::astroManagerSettings->value(settings::ARID_ORACLE_SCHEMANAME, QVariant(QString("xe"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_ORACLE_USERNAME, QVariant(QString("ATID"))).toString(),
                                settings::astroManagerSettings->value(settings::ARID_ORACLE_PASSWORD, QVariant(QString("ATID"))).toString()) );
    }

    /// @brief Populates a list widget with filter information.
    /// @param[in] listWidget: The listWidget to populate.
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CARID::populateFiltersListWidget(QListWidget *listWidget)
    {
      sqlWriter.resetQuery();

      sqlWriter.select({"SHORTTEXT", "FILTER_ID", "DESCRIPTION"}).from({"TBL_FILTERS"});

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery->next())
        {
          listWidget->addItem(new QListWidgetItem(QString(sqlQuery->value(0).toString() + " - " + sqlQuery->value(2).toString())));
        };
      }
      else
      {
        processErrorInformation();
      };
    }

    /// @brief Populates a combo with the available filters. TBL_FILTERS
    /// @param[in] combo: Pointer to the comboBox to populate.
    /// @throws None.
    /// @version 2010-05-29/GGB - Function created.

    void CARID::PopulateFiltersAvailableCombo(QComboBox *combo)
    {
      QString szSQL = "SELECT f.TYPE, f.FILTER_ID, f.color FROM TBL_FILTERS f ORDER BY f.FILTER_ID";
      QSqlQuery query(*dBase);

      query.exec(szSQL);
      while (query.next())
      {
        combo->addItem(*new QString(query.value(0).toString() + " - " + query.value(2).toString()), *new QVariant(query.value(1)));
      };
    }

    /// Populates the instrument combo box
    //
    // 2013-05-15/GGB - Added conditional on not disabled.
    // 2010-05-28/GGB - 1) Changed parameter 2 from BOOL to bool.
    //							    2) Changed to use the Qt API rather than the windows API.
    // 2010-01-18/GGB - Added additional paramter to determine if the current selected value should be selected.
    // 2009-11-21/GGB - Function created.

    void CARID::PopulateInstrumentCombo(QComboBox *combo, bool bSelect)
    {
      if (!ARIDdisabled_)
      {
        QString szSQL = "SELECT i.DESCRIPTION, i.INSTRUMENT_ID FROM tbl_Instruments i WHERE i.DONTDISPLAY <> -1";
        QSqlQuery query(*dBase);
        int nRow = 0;

        query.exec(*new QString(szSQL));
        while (query.next())
        {
          combo->addItem(*new QString(query.value(0).toString()) , *new QVariant(query.value(1)));
        };

        if (bSelect)
        {
          szSQL = "SELECT i.DESCRIPTION, i.DONTDISPLAY FROM TBL_INSTRUMENTS i WHERE i.INSTRUMENT_ID = " \
            + settings::astroManagerSettings->value("Site", QVariant(0)).toString();
          query.exec(*new QString(szSQL));

          if (query.first())
          {
            nRow = combo->findText(query.value(0).toString());
            if (nRow >= 0)
            {
              combo->setCurrentIndex(nRow);
            };
          };
        };
      };
    }

    /// @brief Populates a list widget with a list of imager names.
    /// @param[in] listWidget: Pointer to the list widget to populate.
    /// @param[in] includeDeleted: Include deleted sites.
    /// @throws GCL::CRuntimeError
    /// @version 2018-02-02/GGB - Function created.

    void CARID::populateListInstrumentNames(QListWidget *listWidget, bool includeDeleted)
    {
      RUNTIME_ASSERT("AstroManager", listWidget != nullptr, "parameter listWidget cannot be nullptr.");

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.select({"SHORTTEXT", "INSTRUMENT_ID"}).from({"TBL_INSTRUMENTS"});

        if (!includeDeleted)
        {
          sqlWriter.where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), false)});
        };

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          while (sqlQuery->next())
          {
            QListWidgetItem *lwi = new QListWidgetItem(sqlQuery->value(0).toString());
            lwi->setData(Qt::UserRole, sqlQuery->value(1));
            listWidget->addItem(lwi);
            lwi = nullptr;
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      };
    }

    /// @brief Populates a list widget with a list of site names.
    /// @param[in] listWidget: Pointer to the list widget to populate.
    /// @param[in] includeDeleted: Include deleted sites.
    /// @throws GCL::CRuntimeError
    /// @version 2018-02-02/GGB - Function created.

    void CARID::populateListSiteNames(QListWidget *listWidget, bool includeDeleted)
    {
      RUNTIME_ASSERT("astroManager", listWidget != nullptr, "parameter listWidget cannot be nullptr.");

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.select({"SHORTTEXT", "SITE_ID"}).from({"TBL_SITES"});

        if (!includeDeleted)
        {
          sqlWriter.where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), false)});
        };

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          while (sqlQuery->next())
          {
            QListWidgetItem *lwi = new QListWidgetItem(sqlQuery->value(0).toString());
            lwi->setData(Qt::UserRole, sqlQuery->value(1));
            listWidget->addItem(lwi);
            lwi = nullptr;
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      };
    }

    /// @brief Populates a list widget with a list of telescope names.
    /// @param[in] listWidget: Pointer to the list widget to populate.
    /// @throws GCL::CRuntimeError
    /// @version 2018-02-03/GGB - Function created.

    void CARID::populateListTelescopeNames(QListWidget *listWidget, bool includeDeleted)
    {
      RUNTIME_ASSERT("astroManager", listWidget != nullptr, "parameter listWidget cannot be nullptr.");

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.select({"SHORTTEXT", "TELESCOPE_ID"}).from({"TBL_TELESCOPES"});

        if (!includeDeleted)
        {
          sqlWriter.where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), false)});
        };

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          while (sqlQuery->next())
          {
            QListWidgetItem *lwi = new QListWidgetItem(sqlQuery->value(0).toString());
            lwi->setData(Qt::UserRole, sqlQuery->value(1));
            listWidget->addItem(lwi);
            lwi = nullptr;
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      };
    }

    /// @brief Fills a comboBox with available observing plans.
    /// @param[in] comboBox: The comboBox to populate.
    /// @param[in] selectDefault: Select the default (last selected plan)
    /// @throws None.
    /// @version 2018-02-06/GGB - Function created.

    void CARID::populateComboObservingPlans(QComboBox *comboBox, bool selectDefault)
    {
      RUNTIME_ASSERT("astroManager", comboBox != nullptr, "parameter comboBox cannot be nullptr.");

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.select({"SHORTTEXT", "PLAN_ID"}).from({"TBL_PLANS"}).
            where({GCL::sqlwriter::parameterTriple(std::string("COMPLETE"), std::string("<>"), true)});;

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          while (sqlQuery->next())
          {
            comboBox->addItem(sqlQuery->value(0).toString(), sqlQuery->value(1).toULongLong());
          };

          if (selectDefault)
          {
            std::uint_least32_t siteID = settings::astroManagerSettings->value(settings::SETTINGS_SITE_DEFAULTID, QVariant(0)).toULongLong();

            std::uint_fast32_t index;

            for (index = 0; index < static_cast<std::uint_least32_t>(comboBox->count()); index++)
            {
              if (comboBox->itemData(index) == siteID)
              {
                comboBox->setCurrentIndex(index);
                index = comboBox->count();    // Jump out of the loop.
              };
            };
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      };
    }

    /// @brief Populates a combo box with site information.
    /// @param[in] comboBox: The combo box to populate.
    /// @param[in] bSelect: boolean of whether the default value should be selected.
    /// @throws None.
    /// @version 2018-02-06/GGB - Changed function name and updated logic.
    /// @version 2010-05-28/GGB - 1) Parameter 2 changed to bool from BOOL
    /// @version 2010-05-28/GGB - 2) Changed to use the Qt API.
    /// @version 2010-01-18/GGB - Added additional paramter to determine if the current selected value should be selected.
    /// @version 2009-11-20/GGB - Function created.

    void CARID::populateComboSite(QComboBox *comboBox, bool bSelect)
    {
      RUNTIME_ASSERT("astroManager", comboBox != nullptr, "parameter comboBox cannot be nullptr.");

      if (!ARIDdisabled_)
      {
        sqlWriter.resetQuery();

        sqlWriter.select({"SHORTTEXT", "SITE_ID"}).from({"TBL_SITES"}).
            where({GCL::sqlwriter::parameterTriple(std::string("RETIRED"), std::string("="), false)});;

        if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
        {
          while (sqlQuery->next())
          {
            comboBox->addItem(sqlQuery->value(0).toString(), sqlQuery->value(1).toULongLong());
          };

          if (bSelect)
          {
            std::uint_least32_t siteID = settings::astroManagerSettings->value(settings::SETTINGS_SITE_DEFAULTID, QVariant(0)).toULongLong();

            std::uint_fast32_t index;

            for (index = 0; index < comboBox->count(); index++)
            {
              if (comboBox->itemData(index) == siteID)
              {
                comboBox->setCurrentIndex(index);
                index = comboBox->count();    // Jump out of the loop.
              };
            };
          };
        }
        else
        {
          processErrorInformation();
        };

        sqlQuery->finish();
      };
    }

    /// @brief Called to populate a combo box with the observer names.
    /// @param[in] combo: The comboBox to populate.
    /// @param[in] bSelect:
    /// @version 2010-05-28/GGB - Uses the Qt API.
    /// @version 2010-01-18/GGB - Added additional paramter to determine if the current selected value should be selected.
    /// @version 2009-11-20/GGB - Function created.

    void CARID::PopulateObserverCombo(QComboBox *combo, bool bSelect)
    {
      QString szSQL = "SELECT n.SHORTTEXT, n.OBSERVER_ID FROM TBL_OBSERVERS n WHERE n.DONTDISPLAY = 0";

      QSqlQuery query(*dBase);
      int nRow = 0;

      query.exec(*new QString(szSQL));

      while (query.next())
      {
        combo->addItem(*new QString(query.value(0).toString()), *new QVariant(query.value(1)));
      };

      if (bSelect)
      {
        szSQL = "SELECT n.SHORTTEXT, n.DONTDISPLAY FROM TBL_OBSERVERS n WHERE n.OBSERVER_ID = " \
          + settings::astroManagerSettings->value("Observer", QVariant(0)).toString();
        query.exec(* new QString(szSQL));
        query.first();
        nRow = combo->findText(query.value(0).toString());
        if (nRow >= 0)
        {
          combo->setCurrentIndex(nRow);
        };
      };
    }

    /// @brief Populates a combo box with the table of standard filters.
    /// @param[in] combo: The comboBox to populate.
    /// @throws None.
    /// @version 2010-05-29/GGB - Function created.

    void CARID::populateStandardFiltersCombo(QComboBox *combo)
    {
      sqlWriter.resetQuery();

      sqlWriter.select({"SHORTTEXT", "FILTER_ID", "DESCRIPTION"}).from({"TBL_FILTERS"});

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery->next())
        {
          combo->addItem(QString(sqlQuery->value(0).toString() + " - " + sqlQuery->value(2).toString()), QVariant(sqlQuery->value(1)));
        };
      }
      else
      {
        processErrorInformation();
      };
    }

    /// @brief Populates available telescope names into a combo box.
    /// @param[in] combo: The comboBox to populate.
    /// @param[in] bSelect:
    /// @throws
    /// @version 2010-05-28/GGB - Changed to use Qt API.
    /// @version 2010-01-18/GGB - Added additional paramter to determine if the current selected value should be selected.
    /// @version 2009-11-20/GGB - Function created.

    void CARID::PopulateTelescopeCombo(QComboBox *combo, bool bSelect)
    {
      QString szSQL = "SELECT t.SHORTTEXT, t.TELESCOPE_ID FROM TBL_Telescope t WHERE t.DONTDISPLAY <> -1";
      QSqlQuery query(*dBase);
      int nRow = 0;

      query.exec(*new QString(szSQL));

      while (query.next())
      {
        combo->addItem(*new QString(query.value(0).toString()), *new QVariant(query.value(1)));
      }

      if (bSelect)
      {
        szSQL = "SELECT t.SHORTTEXT, t.DONTDISPLAY FROM TBL_TELESCOPE t WHERE t.TELESCOPE_ID = " \
          + settings::astroManagerSettings->value("Telescope", QVariant(0)).toString();

        query.exec(*new QString(szSQL));
        if (query.first())
        {
          nRow = combo->findText(query.value(0).toString());
          if (nRow >= 0)
          {
            combo->setCurrentIndex(nRow);
          };
        };
      };
    }

    /// @brief Reads the plan targets in from the database.
    /// @param[in] planID: The ID of the plan to read.
    /// @param[out] targetList: The vector to write the targets to.
    /// @throws
    /// @version 2018-09-01/GGB - Function created.

    void CARID::readObservingPlanTargets(planID_t planID, std::vector<std::unique_ptr<CTargetAstronomy>> &targetList)
    {
        // Create the query.

      sqlWriter.resetQuery();
      sqlWriter.select({"TARGET_ID", "RANK", "TARGETTYPE_ID", "NAME_ID", "TARGET_NAME"})
               .from({"TBL_TARGETS"})
               .where({GCL::sqlwriter::parameterTriple(std::string("PLAN_ID"), std::string("="), planID)});

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery->next())
        {
            // Check the type and create the object.

          switch(sqlQuery->value(2).toUInt())
          {
            case MAJORPLANET:
            {
              targetList.emplace_back(std::make_unique<CTargetAstronomy>(
                                        std::make_unique<ACL::CTargetMajorPlanet>(static_cast<ACL::CTargetMajorPlanet::EPlanets>(sqlQuery->value(3).toUInt()))));
              targetList.back()->targetAstronomy()->objectName(sqlQuery->value(4).toString().toStdString());
              break;
            };
            case MINORPLANET:
            {
              targetList.emplace_back(std::make_unique<CTargetAstronomy>(
                                      std::make_unique<ACL::CTargetMinorPlanet>(
                                          settings::astroManagerSettings->value(settings::FILE_MPCORB,
                                                                                "Data/MPCORB.DAT").toString().toStdString(),
                                          sqlQuery->value(4).toString().toStdString())));
              targetList.back()->targetAstronomy()->objectName(sqlQuery->value(4).toString().toStdString());
              break;
            };
            case COMET:
            {
              targetList.emplace_back(std::make_unique<CTargetAstronomy>(
                                        std::make_unique<ACL::CTargetComet>(
                                            settings::astroManagerSettings->value(settings::FILE_COMETELS,
                                                                                  "Data/CometEls.txt").toString().toStdString(),
                                            sqlQuery->value(4).toString().toStdString())));
              targetList.back()->targetAstronomy()->objectName(sqlQuery->value(4).toString().toStdString());
              break;
            };
            case STELLAR:
            {
                // Need to load the information needed form the ATID database.

              targetList.emplace_back(std::make_unique<CTargetAstronomy>(std::make_unique<ACL::CTargetStellar>()));

              databaseATID->queryStellarObjectByNameID(sqlQuery->value(3).toULongLong(),
                                                         dynamic_cast<ACL::CTargetStellar *>(targetList.back()->targetAstronomy()));

              break;
            };
            default:
            {
              ASTROMANAGER_CODE_ERROR;
              break;
            };
          };
        };
      }
      else
      {
        processErrorInformation();
      };
    }

    /// @brief Adds an image record to the images table.
    /// @details This is the step of linking the UUID that has been saved into the FITS
    ///          file with the image name and some other parameters to allow a search of the images to find information. The UUID is
    ///          used to synchronise the data between the FITS file and the database. It also allows the user to (theoretically)
    ///          change the file path without losing the linkage.
    /// @pre 1. The astroFile must have been loaded.
    /// @version 2017-09-23/GGB - Update to use CAngle
    /// @version 2017-09-02/GGB - Changed fileName to imageName.
    /// @version 2013-05-18/GGB - Added check if ARID disabled.
    /// @version 2013-05-15/GGB - Function created.

    bool CARID::registerImage(CAstroFile *astroFile)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        if (!isImageNameRegistered(astroFile->getImageName()))
        {
            // Image is not registered.

          std::string uuid;
          std::uint32_t siteID;
          std::uint32_t telescopeID;
          bool hasWCSData = false;
          std::optional<ACL::CAstronomicalCoordinates> corner1, corner2, corner3, corner4;

          std::uint16_t filterID = ACL::CPhotometryFilterCollection::findFilterID(astroFile->imageFilter());

          if (findObservingSite(dynamic_cast<CObservatory *>(astroFile->getObservationLocation())))
          {
            siteID = dynamic_cast<CObservatory *>(astroFile->getObservationLocation())->siteID();
          }
          else if (registerObservingSite(dynamic_cast<CObservatory *>(&*astroFile->getObservationLocation())))
          {
            siteID = dynamic_cast<CObservatory *>(astroFile->getObservationLocation())->siteID();
          }
          else
          {
            ERRORMESSAGE("Unable to register observing site.");
            siteID = 0;
          };

          if (findTelescope(dynamic_cast<CTelescope *>(&*astroFile->getObservationTelescope())))
          {
            telescopeID = dynamic_cast<CTelescope *>(&*astroFile->getObservationTelescope())->telescopeID();
          }
          else if (registerTelescope(dynamic_cast<CTelescope *>(&*astroFile->getObservationTelescope())))
          {
            telescopeID = dynamic_cast<CTelescope *>(&*astroFile->getObservationTelescope())->telescopeID();
          }
          else
          {
            ERRORMESSAGE("Unable to register telescope.");
            telescopeID = 0;
          }

            // UUID

          if (astroFile->keywordExists(0, ACL::ASTROMANAGER_UUID))
          {
            uuid = static_cast<std::string>(astroFile->keywordData(0, ACL::ASTROMANAGER_UUID));
          }
          else
          {
            uuid = QUuid::createUuid().toString().toStdString();
          };

            // WCS

          if (astroFile->hasWCSData(0))
          {
            hasWCSData = true;
            corner1 = astroFile->pix2wcs(0, MCL::TPoint2D<ACL::INDEX_t>(0, 0));
            corner2 = astroFile->pix2wcs(0, MCL::TPoint2D<ACL::INDEX_t>(astroFile->imageWidth() - 1, 0));
            corner3 = astroFile->pix2wcs(0, MCL::TPoint2D<ACL::INDEX_t>(0, astroFile->imageHeight() - 1));
            corner4 = astroFile->pix2wcs(0, MCL::TPoint2D<ACL::INDEX_t>(astroFile->imageWidth() - 1, astroFile->imageHeight() - 1));
          };

          sqlWriter.resetQuery();
          sqlWriter.insertInto("TBL_IMAGES",
                              {"IMAGE_UUID", "IMAGENAME", "FILEPATH", "IMAGEDATE", "IMAGETIME", "SITE_ID", "TELESCOPE_ID", "TARGET",
                               "RA", "DECLINATION", "FILTER_ID", "HASWCS", "CORNER_1_RA", "CORNER_1_DEC", "CORNER_2_RA",
                               "CORNER_2_DEC", "CORNER_3_RA", "CORNER_3_DEC", "CORNER_4_RA", "CORNER_4_DEC", "SYNTHETIC"
                               })
              .values({{ uuid,
                         astroFile->getImageName(),
                         astroFile->fileNameValid() ? astroFile->getFileName().parent_path().string() : boost::filesystem::path(),
                         astroFile->getObservationTime().UTC().decompose().first,
                         astroFile->getObservationTime().UTC().decompose().second, siteID, telescopeID,
                         astroFile->getObservationTarget(), GCL::sqlwriter::bindValue(":ra"), GCL::sqlwriter::bindValue(":dec"),
                         filterID, hasWCSData, GCL::sqlwriter::bindValue(":c1ra"), GCL::sqlwriter::bindValue(":c1dec"),
                         GCL::sqlwriter::bindValue(":c2ra"), GCL::sqlwriter::bindValue(":c2dec"),
                         GCL::sqlwriter::bindValue(":c3ra"), GCL::sqlwriter::bindValue(":c3dec"),
                         GCL::sqlwriter::bindValue(":c4ra"), GCL::sqlwriter::bindValue(":c4dec"),
                         astroFile->syntheticImage()
                       }});

          sqlQuery->clear();
          sqlQuery->prepare(QString::fromStdString(sqlWriter.string()));
          sqlQuery->bindValue(":ra", astroFile->getTargetCoordinates().RA().hours(), QSql::In | QSql::Binary);
          sqlQuery->bindValue(":dec", astroFile->getTargetCoordinates().DEC().degrees(), QSql::In | QSql::Binary);
          if (hasWCSData)
          {
            sqlQuery->bindValue(":c1ra", (*corner1).RA().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c1dec", (*corner1).DEC().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c2ra", (*corner2).RA().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c2dec", (*corner2).DEC().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c3ra", (*corner3).RA().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c3dec", (*corner3).DEC().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c4ra", (*corner1).RA().degrees(), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c4dec", (*corner1).DEC().degrees(), QSql::In | QSql::Binary);
          }
          else
          {
            sqlQuery->bindValue(":c1ra", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c1dec", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c2ra", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c2dec", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c3ra", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c3dec", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c4ra", QVariant(0.0), QSql::In | QSql::Binary);
            sqlQuery->bindValue(":c4dec", QVariant(0.0), QSql::In | QSql::Binary);
          }

          if (sqlQuery->exec())
          {
            astroFile->imageID(sqlQuery->lastInsertId().toUInt());
            returnValue = true;
            INFOMESSAGE("Image Registered.");
          }
          else
          {
            processErrorInformation();

            INFOMESSAGE("Image not registered.");
          }
        };
      };

      return returnValue;
    }

    /// @brief Registers an observing site based on the existing CObservatory parameters passed to the function.
    /// @param[in] newSite: The new site to register.
    /// @returns true = success
    /// @returns false = fail
    /// @throws None
    /// @version 2017-08-04/GGB - Function created.

    bool CARID::registerObservingSite(CObservatory *observatory)
    {
      bool returnValue = false;
      QSqlQuery query(*dBase);

      dialogs::CDialogConfigureSite dialogConfigureSite(observatory);

      if (dialogConfigureSite.exec())
      {
        sqlWriter.resetQuery();
        sqlWriter.insertInto("TBL_SITES", {"SHORTTEXT", "LATITUDE", "LONGITUDE", "ALTITUDE", "TIMEZONE", "DONTDISPLAY",
                                            "IAUCODE", "DAYLIGHTSAVING"})
            .values({{ observatory->siteName(), observatory->latitude(), observatory->longitude(), observatory->altitude(),
                       observatory->timeZone(), observatory->dontDisplay(), observatory->IAUCode(),
                       observatory->daylightSaving() }});

        if (query.exec(QString::fromStdString(sqlWriter.string())))
        {
          observatory->siteID() = query.lastInsertId().toUInt();
          returnValue = true;
          INFOMESSAGE("Observatory Registered. SiteId: " + observatory->siteID());
        }
        else
        {
          processErrorInformation();

          INFOMESSAGE("Observatory not registered.");
        };
      }
      else
      {
        INFOMESSAGE("User cancelled observatory registration.");
      }

      return returnValue;
    }

    /// @brief Function to register a new telescope.
    /// @param[in] telescope: The new telescope to register.
    /// @returns true = success
    /// @returns false = fail
    /// @throws None.
    /// @version 2017-08-05/GGB - Function created

    bool CARID::registerTelescope(CTelescope *telescope)
    {
      bool returnValue = false;
      QSqlQuery query(*dBase);

      dialogs::CDialogConfigureTelescope dialogConfigureTelescope(telescope);

      if (dialogConfigureTelescope.exec())
      {
        sqlWriter.resetQuery();
        sqlWriter.insertInto("TBL_TELESCOPES", {"SHORTTEXT", "MANUFACTURER", "MODEL", "APERTURE", "FOCALLENGTH", "OBSTRUCTION"})
            .values({{ telescope->telescopeName(), telescope->manufacturer(), telescope->model(), telescope->aperture(),
                       telescope->focalLength(), telescope->obstruction() }});

        if (query.exec(QString::fromStdString(sqlWriter.string())))
        {
          telescope->telescopeID() = query.lastInsertId().toUInt();
          returnValue = true;
          INFOMESSAGE("Telescope Registered. TelescopeId: " + telescope->telescopeID());
        }
        else
        {
          processErrorInformation();

          INFOMESSAGE("Telescope not registered.");
        };
      }
      else
      {
        INFOMESSAGE("User cancelled telescope registration.");
      };

      return returnValue;
    }

    /// @brief Register and upload an image to the database.
    /// @param[in] image: Pointer to the image to upload.
    /// @param[in] filePath: The complete path and file name.
    /// @returns true - Image registered and uploaded.
    /// @returns false - Image not registered or uploaded.
    /// @throws None.
    /// @version 2017-08-05/GGB - Function created.

    bool CARID::registerUploadImage(CAstroFile *astroFile, boost::filesystem::path const &filePath)
    {
      bool returnValue = false;

      if (!isImageNameRegistered(astroFile->getImageName()))
      {
        registerImage(astroFile);

          // Upload as version 0 file if necessary.

        uploadImage(QString::fromStdString(filePath.string()), astroFile->imageID(), 0, "Original Image");
      }
      else
      {
          // Check if the version has been uploaded.

        INFOMESSAGE("Image: " + astroFile->getImageName() + " already registered. It has not been uploaded.");
      }

      return returnValue;
    }

    /// @brief Updates the image flags.
    /// @param[in] imageID: The ID of the image to update.
    /// @param[in] astrometryFlag: New value for the astrometric value.
    /// @param[in] photometryFlag: New value for the photometric value.
    /// @returns true - Success
    /// @returns false - Failure
    /// @throws None.

    bool CARID::updateImageFlags(imageID_t imageID, bool astrometryFlag, bool photometryFlag)
    {
      bool returnValue = false;

      sqlWriter.resetQuery();
      sqlWriter.update("TBL_IMAGES")
          .set("ASTROMETRIC", astrometryFlag)
          .set("PHOTOMETRIC", photometryFlag)
          .where("IMAGE_ID", "=", imageID);

      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        returnValue = true;
        DEBUGMESSAGE("Image Flags Updated.");
      }
      else
      {
        processErrorInformation();

        DEBUGMESSAGE("Image Flags not Updated.");
      };

      return returnValue;
    }

    /// @brief Updates the image name.
    /// @param[in] imageID: The ID of the image to update.
    /// @param[in] imageName: The new image name to assign.
    /// @returns true - Success
    /// @returns false - Failure
    /// @throws None.
    /// @version 2017-08-20/GGB - Function created.

    bool CARID::updateImageComments(imageID_t imageID, QString const &imageComments)
    {
      bool returnValue = false;

      sqlWriter.resetQuery();
      sqlQuery->clear();

      sqlWriter.update("TBL_IMAGES")
          .where("IMAGE_ID", "=", imageID);

      if (imageComments.isEmpty())
      {
        sqlWriter.set("COMMENTS", std::string(""));
        sqlQuery->prepare(QString::fromStdString(sqlWriter.string()));
      }
      else
      {
        sqlWriter.set("COMMENTS", GCL::sqlwriter::bindValue(":comments"));
        sqlQuery->prepare(QString::fromStdString(sqlWriter.string()));
        sqlQuery->bindValue(":comments", imageComments, QSql::In);
      };

      if (sqlQuery->exec())
      {
        returnValue = true;
        DEBUGMESSAGE("Comments Updated.");
      }
      else
      {
        processErrorInformation();

        DEBUGMESSAGE("Comments not Updated.");
      };

      return returnValue;
    }

    /// @brief Updates the image Quality.
    /// @param[in] imageID: The ID of the image to update.
    /// @param[in] imageQuality: The new Quality value.
    /// @returns true - Success
    /// @returns false - Failure
    /// @throws None.
    /// @version 2017-08-21/GGB - Function created.

    bool CARID::updateImageQuality(imageID_t imageID, std::uint8_t imageQuality)
    {
      bool returnValue = false;

      sqlWriter.resetQuery();

      sqlWriter.update("TBL_IMAGES")
          .set("QUALITY", static_cast<std::uint16_t>(imageQuality))
          .where("IMAGE_ID", "=", imageID);

      sqlQuery->clear();
      if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
      {
        returnValue = true;
        DEBUGMESSAGE("Quality Updated.");
      }
      else
      {
        processErrorInformation();

        DEBUGMESSAGE("Quality not Updated.");
      };

      return returnValue;
    }

    /// @brief Saves an image into the image storage table.
    /// @param[in] fileName: The filename of the image to save.
    /// @param[in] imageID: The ID to associate with the imaged.
    /// @param[in] imageVersion: The version number to associate with the image.
    /// @throws None.
    /// @version 2017-07-28/GGB - Function created.

    void CARID::uploadImage(QString const &fileName, imageID_t imageID, imageVersion_t imageVersion, QString const &comment)
    {
      RUNTIME_ASSERT(astroManager, fileName.size() != 0, "Parameter filename must have length > 0");
      RUNTIME_ASSERT(astroManager, imageID != 0, "Parameter imageID cannot be zero.");
      RUNTIME_ASSERT(astroManager, comment.size() != 0, "Parameter comment cannot have zero length.");

      ACL::TJD JD;

      QByteArray imageArray;

      INFOMESSAGE("Saving image to database...");

      QFile file(fileName);
      if (file.open(QIODevice::ReadOnly))
      {
        imageArray = file.readAll();
        file.close();
      }
      else
      {
        ASTROMANAGER_ERROR(0x4002);
      }

      sqlWriter.resetQuery();
      std::string sqlString = sqlWriter.insertInto("TBL_IMAGESTORAGE", {"IMAGE_ID", "IMAGE_VERSION", "IMAGE_DATA",
                                                                        "DATETIME", "COMMENT"})
          .values({ {imageID, imageVersion, GCL::sqlwriter::bindValue(":image"), GCL::sqlwriter::bindValue(":datetime"),
                     GCL::sqlwriter::bindValue(":comment")}}).string();

      sqlQuery->clear();
      sqlQuery->prepare(QString::fromStdString(sqlString));
      sqlQuery->bindValue(":image", imageArray, QSql::In | QSql::Binary);
      sqlQuery->bindValue(":datetime", static_cast<double>(JD), QSql::In | QSql::Binary);
      sqlQuery->bindValue(":comment", comment, QSql::In);

      if (sqlQuery->exec())
      {
        INFOMESSAGE("Image saved in database.");
      }
      else
      {
        INFOMESSAGE("Image not saved.");
        processErrorInformation();
      };
    }

    /// @brief Uploads an image to database.
    /// @param[in] imageArray - The byteArray containing the image.
    /// @param[in] imageID - The ID of the image to save.
    /// @param[in] imageVersion - The version of the image to save.
    /// @param[in] comment - The comment to associate with the version.

    void CARID::uploadImage(QByteArray const &imageArray, imageID_t imageID, imageVersion_t imageVersion, QString const &comment)
    {
      RUNTIME_ASSERT(astroManager, imageID != 0, "Parameter imageID cannot be zero.");
      RUNTIME_ASSERT(astroManager, imageVersion != 0, "Parameter imageVersion cannot be zero.");

      ACL::TJD JD;

      INFOMESSAGE("Saving image to database...");

      sqlWriter.resetQuery();
      sqlWriter.insertInto("TBL_IMAGESTORAGE", {"IMAGE_ID", "IMAGE_VERSION", "IMAGE_DATA", "DATETIME", "COMMENT"})
          .values({ {imageID, imageVersion, GCL::sqlwriter::bindValue(":image"), GCL::sqlwriter::bindValue(":datetime"),
                     GCL::sqlwriter::bindValue(":comment")}});

      sqlQuery->clear();
      sqlQuery->prepare(QString::fromStdString(sqlWriter.string()));
      sqlQuery->bindValue(":image", imageArray, QSql::In | QSql::Binary);
      sqlQuery->bindValue(":datetime", static_cast<double>(JD), QSql::In | QSql::Binary);
      sqlQuery->bindValue(":comment", comment, QSql::In);

      if (sqlQuery->exec())
      {
        INFOMESSAGE("Image saved in database.");
      }
      else
      {
        processErrorInformation();

        INFOMESSAGE("Image not saved.");
      };
      sqlQuery->finish();
    }

    /// @brief Counts the number of versions associated with the image.
    /// @param[in] imageID - The ID of the image to count.
    /// @returns The number of versions.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2017-08-12/GGB - Function created.

    imageVersion_t CARID::versionCount(imageID_t imageID)
    {
      imageVersion_t returnValue = 0;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        sqlWriter.select({}).count("*").from({"TBL_IMAGESTORAGE"}).
            where({GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID)});

        if (query.exec(QString::fromStdString(sqlWriter.string())))
        {
          query.first();
          if (query.isValid())
          {
            returnValue = query.value(0).toUInt();
          }
          else
          {
            processErrorInformation();
          }
        }
        else
        {
          processErrorInformation();
        }
      }
      else
      {
        ASTROMANAGER_CODE_ERROR;
      };
      return returnValue;
    }

    /// @brief Returns the latest version number for the spedified image. (Latest == highest version number)
    /// @param[in] imageID - The ID of the image to get the version number.
    /// @returns The latest imageVersion.
    /// @throws None.
    /// @version 2017-08-12/GGB - Function created.

    bool CARID::versionLatest(imageID_t imageID, imageVersion_t &imageVersion)
    {
      bool returnValue = false;

      if (!ARIDdisabled_)
      {
        QSqlQuery query(*dBase);

        sqlWriter.resetQuery();
        sqlWriter.select({"IMAGE_VERSION"}).from({"TBL_IMAGESTORAGE"})
            .where({GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID)})
            .orderBy({std::make_pair("IMAGE_VERSION", GCL::sqlwriter::CSQLWriter::DESC)});

        if (query.exec(QString::fromStdString(sqlWriter.string())))
        {
          query.first();
          if (query.isValid())
          {
            imageVersion = query.value(0).toUInt();
            returnValue = true;
          }
          else
          {
            processErrorInformation();
          }
        }
        else
        {
          processErrorInformation();
        }
      }
      else
      {
        ASTROMANAGER_CODE_ERROR;
      };
      return returnValue;
    }

  }  // namespace database
}  // namespace AstroManager
