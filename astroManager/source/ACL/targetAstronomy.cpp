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
//                      Copyright 2018-2020 Gavin Blakeman.
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

#include "include/ACL/targetAstronomy.h"

  // Standard C++ library header files

#include <utility>

  // Miscellaneous library header files

#include "boost/algorithm/string.hpp"
#include "boost/locale.hpp"

  // astroManager application header files

#include "include/database/databaseATID.h"
#include "include/error.h"

namespace astroManager
{

  /// @brief        Constructor for the class. Also calls the function to create and load the data for the target.
  /// @param[in]    objectID: The ID of the target associated with the instance.
  /// @param[in]    targetName: The name of the target.
  /// @param[in]    targetType: The type of the target.
  /// @param[in]    ct: Reference to an instance that holds the current time.
  /// @param[in]    gl: Reference to observatory information.
  /// @param[in]    wt: Reference to the weather.
  /// @throws       std::bad_alloc
  /// @version      2018-08-31/GGB - Function created

  CTargetAstronomy::CTargetAstronomy(database::objectID_t tid, std::string const &targetName, std::uint_least16_t targetType,
                                     ACL::CAstroTime const &ct, ACL::CGeographicLocation const &gl, ACL::CWeather const &wt)
    : targetID(tid), targetAstronomy_(), currentTime_(ct), observerLocation(gl), observerWeather(wt)
  {
    createTarget(targetType, targetName, tid);
  }

  /// @brief        Creates a target object for the specified type.
  /// @param[in]    targetType: The type of target to create.
  /// @param[in]    targetName: The name of the target to create.
  /// @throws       std::bad_alloc
  /// @version      2020-09-18/GGB - Function created.

  void CTargetAstronomy::createTarget(std::uint_least16_t targetType, std::string const &targetName, database::objectID_t objectID)
  {
    switch (targetType)
    {
      case ACL::TT_MAJORPLANET:
      {
        targetAstronomy_ = std::move(std::make_unique<ACL::CTargetMajorPlanet>(targetName));
        break;
      }
      case ACL::TT_MINORPLANET:
      {
        targetAstronomy_ = std::move(std::make_unique<ACL::CTargetMinorPlanet>(targetName));
        break;
      }
      case ACL::TT_COMET:
      {
        targetAstronomy_ = std::move(std::make_unique<ACL::CTargetComet>(targetName));
        break;
      }
      case ACL::TT_STELLAR:
      {
        targetAstronomy_ = std::move(std::make_unique<ACL::CTargetStellar>());
        database::databaseATID->readStellarObjectInformation(objectID, dynamic_cast<ACL::CTargetStellar *>(targetAstronomy_.get()));
        break;
      }
      default:
      {
        CODE_ERROR;
      }
    }
  }

  /// @brief Returns a pointer to the managed object
  /// @returns Raw pointer to the managed object.

  ACL::CTargetAstronomy *CTargetAstronomy::targetAstronomy() const
  {
    return targetAstronomy_.get();
  }

  /// @brief        Updates the widget with the object name.
  /// @throws       None.
  /// @version      2020-09-16/GGB - Changed return type to QString().
  /// @version      2018-09-03/GGB - Function created.

  QString CTargetAstronomy::name()
  {
    RUNTIME_ASSERT(targetAstronomy_, boost::locale::translate("targetAstronomy_ pointer is null."));

    return QString::fromStdString(targetAstronomy_->objectName());
  }

  /// @brief        Updates the widget with the type of the object.
  /// @throws       None.
  /// @version      2020-09-16/GGB - Changed return type to QString().
  /// @version      2018-09-03/GGB - Function created.

  QString CTargetAstronomy::type()
  {
    RUNTIME_ASSERT(targetAstronomy_, boost::locale::translate("targetAstronomy_ pointer is null."));

    QString returnValue;

    switch (targetAstronomy_->targetType())
    {
      case ACL::TT_STELLAR:
      {
        returnValue = QString::fromStdString(dynamic_cast<ACL::CTargetStellar *>(targetAstronomy())->stellarType());
        break;
      };
      case ACL::TT_MAJORPLANET:
      {
        returnValue = "Planet";
        break;
      };
      case ACL::TT_MINORPLANET:
      {
        returnValue = "Minor Planet";
        break;
      };
      case ACL::TT_COMET:
      {
        returnValue = "Comet";
        break;
      };
      case ACL::TT_NONE:
      default:
      {
        CODE_ERROR;
        break;
      };
    };

    return returnValue;
  }

  /// @brief Sets the RA value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::RA()
  {
    QString returnValue;

    return returnValue;
  }

  /// @brief Sets the DEC value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::DEC()
  {
    QString returnValue;

    return returnValue;
  }

  /// @brief Sets the Altitude value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::Altitude()
  {
    QString returnValue;

    return returnValue;
  }

  /// @brief Sets the Azimuth value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::Azimuth()
  {
    QString returnValue;

    return returnValue;
  }

  /// @brief Sets the Airmass value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::Airmass()
  {
    QString returnValue;

    return returnValue;
  }

  /// @brief Sets the Apparent Magnitude value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::ApparentMagnitude()
  {
    QString returnValue;

    return returnValue;
  }

  /// @brief Sets the Constellation value into the widget.
  /// @throws Exceptions from called functions.
  /// @version 2018-09-29/GGB - Function created.

  QString CTargetAstronomy::Constellation()
  {
//    if (targetAstronomy()->targetType() == ACL::CTargetAstronomy::TT_STELLAR)
//    {
//      std::string constellationName;

//      database::databaseATID->queryConstellationByName(targetAstronomy()->objectName(), constellationName);

//      columnMap[column_constellation]->setText(QString::fromStdString(constellationName));
//    };
  }

  QString CTargetAstronomy::Extinction()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::HourAngle()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::Magnitude()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::ObservationCount()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::RiseTime()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::SetTime()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::TransitTime()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::TransitAltitude()
  {
    QString returnValue;

    return returnValue;
  }
  QString CTargetAstronomy::Catalogue()
  {
    QString returnValue;

    return returnValue;
  }


} // namespace astroManager
