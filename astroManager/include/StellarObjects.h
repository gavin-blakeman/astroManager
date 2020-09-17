//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:			          StellarObjects
// SUBSYSTEM:           Stellar object information system
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// AUTHOR:              Gavin BLakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2018-2020 Gavin Blakeman.
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
// OVERVIEW:            The stellar objects information system implements the interface between the application an the method of
//                      obtaining stellar object information. There are two ways of obtaining stellar object information, the first
//                      is from the ATID database; the second is from the SIMBAD web service.
//                      The class in this sub-system implements the interface between the astroManager GUI and the two information
//                      systems.
//
// CLASSES INCLUDED:    CSIMBAD
//
// HIERARCHY:           CSIMBAD
//
// HISTORY:             2015-09-23 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_STELLAROBJECTS_H
#define ASTROMANAGER_STELLAROBJECTS_H

  // Standard C++ library headers

#include <optional>
#include <string>
#include <vector>

  // Miscellaneous libraries

#include <ACL>
#include <MCL>

namespace astroManager
{
  /// The CStellarObjects class is designed to get information on Stellar targets. The class can utilise the ATID database or the
  /// SIMBAD web service for the information.
  /// The use of the class is to create a new instance for each search required.
  /// @pre The static function configure(...) should be called before first use.

  class CStellarObjects
  {
  private:
    static bool disableATID_;                  ///< Flag to indicate that the use of ATID is disabled.

    std::string objectID_;
    ACL::CAstronomicalCoordinates ICRS;
    std::optional<MCL::TPoint2D<ACL::FP_t>> properMotion_;
    std::optional<ACL::FP_t> parallax_;
    std::optional<ACL::FP_t> radialVelocity_;
    std::optional<ACL::FP_t> redShift_;
    std::vector<std::string> identifiers_;
    std::string spectralType_;

    CStellarObjects(CStellarObjects const &) = delete;
    CStellarObjects &operator=(CStellarObjects const &) = delete;

  protected:
  public:
    CStellarObjects(std::string const &);
    virtual ~CStellarObjects() {}

    bool objectFind();

    static void configure(bool);

  };

}   // namespace AstroManager

#endif // ASTROMANAGER_STELLAROBJECTS_H
