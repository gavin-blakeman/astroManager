﻿//*********************************************************************************************************************************
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

  // astroManager application header files

#include "include/database/databaseATID.h"
#include "include/error.h"

namespace astroManager
{

  /// @brief Constructor for the class. Also set the size of the vector.
  /// @param[in] parent: The table widget associated with this target.
  /// @param[in] newTarget: The target to assign to this instance.
  /// @throws std::bad_alloc
  /// @version 2018-08-31/GGB - Function created

  CTargetAstronomy::CTargetAstronomy(std::unique_ptr<ACL::CTargetAstronomy>newTarget, ACL::CAstroTime const &ct,
                                     ACL::CGeographicLocation const &gl, ACL::CWeather const &wt)
    : targetAstronomy_(std::move(newTarget)), currentTime_(ct), observerLocation(gl), observerWeather(wt)
  {

  }

  /// @brief Returns a pointer to the managed object
  /// @returns Raw pointer to the managed object.

  ACL::CTargetAstronomy *CTargetAstronomy::targetAstronomy() const
  {
    return targetAstronomy_.get();
  }

  /// @brief Updates the widget with the object name.
  /// @throws None.
  /// @version      2020-09-16/GGB - Changed return type to QString().
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::name()
  {
    return QString::fromStdString(targetAstronomy_->objectName());
  }

  /// @brief        Updates the widget with the type of the object.
  /// @throws       None.
  /// @version      2020-09-16/GGB - Changed return type to QString().
  /// @version      2018-09-03/GGB - Function created.

  QString CTargetAstronomy::type()
  {
    QString returnValue;

    switch (targetAstronomy_->targetType())
    {
      case ACL::CTargetAstronomy::TT_STELLAR:
      {
        returnValue = QString::fromStdString(dynamic_cast<ACL::CTargetStellar *>(targetAstronomy())->stellarType());
        break;
      };
      case ACL::CTargetAstronomy::TT_MAJORPLANET:
      {
        returnValue = "Planet";
        break;
      };
      case ACL::CTargetAstronomy::TT_MINORPLANET:
      {
        returnValue = "Minor Planet";
        break;
      };
      case ACL::CTargetAstronomy::TT_COMET:
      {
        returnValue = "Comet";
        break;
      };
      case ACL::CTargetAstronomy::TT_NONE:
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

  }

  /// @brief Sets the Altitude value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::Altitude()
  {

  }

  /// @brief Sets the Azimuth value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::Azimuth()
  {

  }

  /// @brief Sets the Airmass value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::Airmass()
  {

  }

  /// @brief Sets the Apparent Magnitude value into the widget.
  /// @throws None.
  /// @version 2018-09-03/GGB - Function created.

  QString CTargetAstronomy::ApparentMagnitude()
  {

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

  }
  QString CTargetAstronomy::HourAngle()
  {

  }
  QString CTargetAstronomy::Magnitude()
  {

  }
  QString CTargetAstronomy::ObservationCount()
  {

  }
  QString CTargetAstronomy::RiseTime()
  {

  }
  QString CTargetAstronomy::SetTime()
  {

  }
  QString CTargetAstronomy::TransitTime()
  {

  }
  QString CTargetAstronomy::TransitAltitude()
  {

  }
  QString CTargetAstronomy::Catalogue()
  {

  }


} // namespace astroManager
