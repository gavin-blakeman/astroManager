//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								astrometryObservation
// SUBSYSTEM:
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman. (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2012, 2018 Gavin Blakeman.
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
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:
//
// HISTORY:
//
//*********************************************************************************************************************************

#include "../../include/astrometry/astrometryObservation.h"

#include "../../include/Settings.h"
#include "../../include/astroManager.h"

namespace astroManager
{
  namespace astrometry
  {
    //*****************************************************************************************************************************
    //
    // CAstrometryObservation
    //
    //*****************************************************************************************************************************

    /// @brief Default constructor for the class.
    /// @throws None.
    /// @version 2013-08-25/GGB - Function created.

    CAstrometryObservation::CAstrometryObservation() : ACL::CAstrometryObservation(std::string("")), group(nullptr),
      text(nullptr)
    {
    }

    /// @brief Constructor taking an object name.
    /// @param[in] name - The name of the object being observed.
    /// @throws None.
    /// @version 2013-08-25/GGB - Function created.

    CAstrometryObservation::CAstrometryObservation(std::string const &name) : ACL::CAstrometryObservation(name), group(nullptr),
      text(nullptr)
    {
    }

    /// @brief Class constructor. Must null text and group.
    /// @param[in] ao - Astrometry observation to create this instance from.
    /// @throws None.
    /// @version 2013-08-25/GGB - Function created.

    CAstrometryObservation::CAstrometryObservation(ACL::CAstrometryObservation const &ao) :
      ACL::CAstrometryObservation(ao), group(nullptr), text(nullptr)
    {
    }

    /// @brief Constructor taking an astronomy target.
    /// @param[in] ta - The astronomy target object.
    /// @throws None.
    /// @version 2016-05-04/GGB - Function created.

    CAstrometryObservation::CAstrometryObservation(std::shared_ptr<ACL::CTargetAstronomy> ta)
      : ACL::CAstrometryObservation(ta), group(nullptr), text(nullptr)
    {
    }

    /// @brief Class destructor. Required to ensure dynamically allocated items are deleted.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2015-01-01/GGB - Function created.

    CAstrometryObservation::~CAstrometryObservation()
    {
      if (text)
      {
        text = nullptr;
      };

      if (group)
      {
        delete group;
        group = nullptr;
      }
    }

    /// @brief Friend function to output the object as comma delimited text.
    /// @param[in] os - The output stream to write the observation to
    /// @param[in] ao - The object to write to the stream.
    /// @returns The output stream.
    /// @throws None.
    /// @version 2017-09-23/GGB - Updated to use CAngle.
    /// @version 2013-08-22/GGB - Added degrees output to the function.
    /// @version 2013-08-18/GGB - Added the RA/Dec if available to the output.
    /// @version 2012-11-16/GGB - Function created.

    std::ostream &operator << (std::ostream &os, CAstrometryObservation &ao)
    {
      std::string szRA, szDec;

      os << std::string(ao.objectName()) << ",";

      if (ao.observedCoordinates())
      {
        if (settings::astroManagerSettings->value(settings::ASTROMETRY_CSV_OUTPUTDEGREES, QVariant(true)).toBool())
        {
          os << ao.observedCoordinates()->RA() << ", " << ao.observedCoordinates()->DEC() << ", ";
        }
        else
        {
          szRA = ao.observedCoordinates()->RA().A2SHMS();
          szDec = ao.observedCoordinates()->DEC().A2SDMS();
          os << szRA << "," << szDec << ",";
        };
      }
      else
      {
        if (settings::astroManagerSettings->value(settings::ASTROMETRY_CSV_OUTPUTDEGREES, QVariant(true)).toBool())
        {
          os << "0, 0, ";
        }
        else
        {
          szRA = std::string("--h--'--.--s""");
          szDec = std::string("±--°--'--""");
          os << szRA << "," << szDec << ",";
        };
      }

      os << ao.CCDCoordinates().x() << "," << ao.CCDCoordinates().y() << ",";
      return os;
    }

  } // namespace astrometry
} // namespace AstroManager
