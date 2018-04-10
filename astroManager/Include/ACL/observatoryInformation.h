//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								observatoryInformation
// SUBSYSTEM:						Display weather data
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	ACL, Qt
// NAMESPACE:						AIRDAS
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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

#ifndef OBSERVATORYINFORMATION
#define OBSERVATORYINFORMATION

  // Standard libraries

#include <cstdlib>

  // Miscellaneous libraries

#include <ACL>

namespace AstroManager
{
  class CObservatory : public ACL::CObservatory
  {
  private:
    std::uint32_t siteID_;        ///< Numeric ID for the site. Allows integration with database applications.
    bool dontDisplay_;

  protected:
  public:
    CObservatory() : siteID_(0), dontDisplay_(false) {}
    CObservatory(CObservatory const &);
    virtual ~CObservatory() {}

    virtual CObservatory *createCopy() const;

    void siteID(std::uint32_t siteID) { siteID_ = siteID; }
    void dontDisplay(bool dd) { dontDisplay_ = dd; }

    std::uint32_t siteID() const noexcept { return siteID_; }
    std::uint32_t &siteID() { return siteID_; }
    bool dontDisplay() const noexcept { return dontDisplay_; }
    bool &dontDisplay() { return dontDisplay_; }
  };


}   // namespace AstroManager

#endif // OBSERVATORYINFORMATION

