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

#include "include/StellarObjects.h"

namespace astroManager
{

  bool CStellarObjects::disableATID_;

  /// @brief        Constructor for the class.
  /// @param[in]    oid: The object ID to associate with the class.
  /// @throws       None.
  /// @version      2015-09-24/GGB - Function created.

  CStellarObjects::CStellarObjects(std::string const &oid) : objectID_(oid)
  {

  }

  /// @brief        Function to allow configuration of the underlying (static) parameters for the class.
  /// @param[in]    da: Disable ATID
  /// @version      2015-09-24/GGB - Function created.

  void CStellarObjects::configure(bool da)
  {
    disableATID_ = da;
  }

  /// @brief        Searches for the object associated with the class.
  /// @returns      true - The object was found
  /// @returns      false - The object was not found.
  /// @throws       None.
  /// @details      Searches for the object with the ID associated with the class. If the ATID database is enabled, the search is
  ///               first made in the ATID database. If the search is not sucessfull, or the ATID database is disabled, then the
  ///               search is made on SIMBAD. Once the object is found the data is parsed or loaded into the class for later
  ///               retrieval by the application.
  /// @version      2015-09-24/GGB - Function created.

  bool CStellarObjects::objectFind()
  {
    bool returnValue = false;

    if (!disableATID_)
    {
        // Try ATID first of not disabled.

    };

    if (!returnValue)
    {
        // If ATID did not return a valid try SIMBAD. Calls to http:\\ will be asynchronous. This can be done through a blocking
        // call but will be slow.

    };

    return returnValue;
  }

}   // namespace AstroManager
