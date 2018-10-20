//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								observatoryInformation
// SUBSYSTEM:						Display weather data
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	ACL, Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CObservatory
//
// CLASS HIERARCHY:     CGeographicLocation
//                        ACL::CObservatory
//                          CObservatory
//
// HISTORY:             2016-02-02 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../include/ACL/observatoryInformation.h"

namespace astroManager
{
  /// @brief Copy constructor.
  /// @throws None.
  /// @version 2017-09-02/GGB - Function created.

  CObservatory::CObservatory(CObservatory const &toCopy) : ACL::CObservatory(toCopy), siteID_(toCopy.siteID_),
    dontDisplay_(toCopy.dontDisplay_)
  {
  }

  /// @brief Returns a copy of this.
  /// @returns A copy of this.
  /// @throws std::bad_alloc
  /// @version 2017-09-02/GGB - Function created.

  std::unique_ptr<ACL::CGeographicLocation> CObservatory::createCopy() const
  {
    return std::make_unique<ACL::CObservatory>(*this);
  }
}
