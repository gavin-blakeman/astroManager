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

#include "../qtExtensions/qt.h"

#include <memory>
#include <vector>

#include <ACL>

namespace AstroManager
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
      CAstrometryObservation(ACL::SPTargetAstronomy &);

      virtual ~CAstrometryObservation();

      friend std::ostream &operator << (std::ostream &, CAstrometryObservation &);
    };
    typedef std::shared_ptr<CAstrometryObservation> PAstrometryObservation;

    typedef std::vector<PAstrometryObservation> DAstrometryObservationStore;
    typedef std::vector<PAstrometryObservation> DTaggedObjectsStore;

  } // namespace astrometry
} // namespace AstroManager

#endif // ASTROMETRYOBSERVATION

