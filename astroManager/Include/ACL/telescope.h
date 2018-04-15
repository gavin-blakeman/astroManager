//*********************************************************************************************************************************
//
// PROJECT:							AstroManager
// FILE:								telescope
// SUBSYSTEM:						Additions to the ACL::CTelescope class.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman. (GGB)
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
// OVERVIEW:						Implements the class for managing telescopes.
//
// CLASSES INCLUDED:    CTelescope
//
// CLASS HIERARCHY:     ACL::CTelescope
//                        AstroManager::CTelesope
//
// HISTORY:             2018-02-03 GGB - Project name changed to AstroManager
//                      2017-07-24 GGB - File created
//
//*********************************************************************************************************************************

#ifndef TELESCOPE_H
#define TELESCOPE_H

#include <cstdint>

#include <ACL>

namespace AstroManager
{
  class CTelescope : public ACL::CTelescope
  {
  private:
    std::uint32_t telescopeID_;

  protected:
  public:
    CTelescope() : ACL::CTelescope() {}
    CTelescope(CTelescope const &toCopy) : ACL::CTelescope(toCopy), telescopeID_(toCopy.telescopeID_) {}

    virtual ACL::CTelescope *createCopy() const { return new CTelescope(*this); }

    virtual void telescopeID(std::uint32_t const &telescopeID) noexcept { telescopeID_ = telescopeID; }
    virtual std::uint32_t const &telescopeID() const noexcept { return telescopeID_; }
    virtual std::uint32_t &telescopeID() noexcept { return telescopeID_; }

  };

} // namespace AstroManager

#endif // TELESCOPE_H

