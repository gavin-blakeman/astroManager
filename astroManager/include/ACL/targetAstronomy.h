//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
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

#ifndef TARGETASTRONOMY_H
#define TARGETASTRONOMY_H

  // Standard C++ library header files

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Miscellaneous library header files

#include <ACL>
#include <QCL>

  // astroManager application header files

#include "include/astroManager.h"
#include "include/database/databaseATID.h"

namespace astroManager
{
  /* Note this is not derived from ACL::CTargetAstronomy as it needs to be able to work with any of the types of
   * astronomy targets. If we derived this, then there would need to be three or four classes.
   *
   * This class is used as an aggragator class and to manage position update etc for the Planning window. This automates the
   * updating of position etc, and allows the calling classes to abstract position calculation into calls for position etc.
   */

  class CTargetAstronomy final
  {
  private:
    database::objectID_t targetID;
    std::unique_ptr<ACL::CTargetAstronomy> targetAstronomy_;
    ACL::CAstroTime const &currentTime_;            // Updated externally
    ACL::CGeographicLocation const &observerLocation;
    ACL::CWeather const &observerWeather;
    double deltaTime;

    CTargetAstronomy() = delete;
    CTargetAstronomy(CTargetAstronomy &&) = delete;

    void createTarget(std::uint_least16_t, std::string const &, database::objectID_t);
    void loadTargetData();

  public:
    CTargetAstronomy(database::objectID_t, std::string const &, std::uint_least16_t, ACL::CAstroTime const &, ACL::CGeographicLocation const &, ACL::CWeather const &);

    ACL::CTargetAstronomy *targetAstronomy() const;

    QString name();
    QString type();
    QString RA();
    QString DEC();
    QString Altitude();
    QString Azimuth();
    QString Airmass();
    QString ApparentMagnitude();
    QString Constellation();
    QString Extinction();
    QString HourAngle();
    QString Magnitude();
    QString ObservationCount();
    QString RiseTime();
    QString SetTime();
    QString TransitTime();
    QString TransitAltitude();
    QString Catalogue();
  };

} // namespace astroManager

#endif // TARGETASTRONOMY_H
