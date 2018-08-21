//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								photometryObservations
// SUBSYSTEM:						Photometry Observations
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CPhotometryObservation
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-03-17 GGB - Removed CSelectObjectDialog into file DialogSelectObject.
//                      2013-03-17 GGB - Removed CImageControlWidget into file DockWidgetImageControl.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into it's own file
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef PHOTOMETRYOBSERVATION
#define PHOTOMETRYOBSERVATION

#include "../qtExtensions/qt.h"

#include <ACL>

namespace astroManager
{
  namespace photometry
  {
    class CPhotometryObservation : public ACL::CPhotometryObservation
    {
    public:
      QGraphicsItemGroup *group;
      QGraphicsSimpleTextItem *text;

      CPhotometryObservation();
      CPhotometryObservation(ACL::CPhotometryObservation const &);
      CPhotometryObservation(std::shared_ptr<ACL::CTargetAstronomy>);

      virtual ~CPhotometryObservation();

      friend std::ostream &operator << (std::ostream &, CPhotometryObservation &);
    };
    typedef std::shared_ptr<CPhotometryObservation> PPhotometryObservation;

    typedef std::vector<photometry::PPhotometryObservation> DPhotometryObservationStore;

  } // namespace photometry

} // namespace AstroManager

#endif // PHOTOMETRYOBSERVATION

