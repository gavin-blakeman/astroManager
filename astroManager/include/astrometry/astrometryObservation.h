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

#ifndef ASTROMETRYOBSERVATION
#define ASTROMETRYOBSERVATION

  // Standard C++ library header files.

#include <memory>
#include <vector>

  // Miscellaneous library header files.

#include <ACL>
#include <QCL>

namespace astroManager
{
  namespace astrometry
  {
    class CAstrometryObservation : public ACL::CAstrometryObservation
    {
    public:
      QGraphicsItemGroup *group;
      QGraphicsSimpleTextItem *text;

      CAstrometryObservation();
      CAstrometryObservation(ACL::CAstrometryObservation const &);
      CAstrometryObservation(std::string const &);
      CAstrometryObservation(std::shared_ptr<ACL::CTargetAstronomy>);

      virtual ~CAstrometryObservation();

      friend std::ostream &operator << (std::ostream &, CAstrometryObservation &);
    };

  } // namespace astrometry
} // namespace astroManager

#endif // ASTROMETRYOBSERVATION

