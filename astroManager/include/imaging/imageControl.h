//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								imageControl
// SUBSYSTEM:						Additions to the ACL::astroFile class.
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
// OVERVIEW:						Implements the image management methods.
//
// CLASSES INCLUDED:    SControlImage
//
// CLASS HIERARCHY:     SControlImage
//
// HISTORY:             2017-07-24 GGB - File created
//
//*********************************************************************************************************************************

#ifndef IMAGECONTROL
#define IMAGECONTROL

  // Standard C++ library header files

#include <memory>

  // astroManager header files.

#include "../ACL/astroFile.h"
#include "../astrometry/astrometryObservation.h"
#include "../photometry/photometryObservation.h"
#include "../astroManager.h"

namespace astroManager
{
  namespace imaging
  {
    class CAstroImageWindow;

    struct SControlImage final
    {
    private:
    public:
      astrometry::DAstrometryObservationStore astrometryObservations;
      photometry::DPhotometryObservationStore photometryObservations;

      astrometry::PAstrometryObservation currentAstrometrySelection;
      photometry::PPhotometryObservation currentPhotometrySelection;

      imaging::CAstroImageWindow *parent_ = nullptr;
      std::shared_ptr<CAstroFile> astroFile;
      ACL::DHDBStore::size_type currentHDB = 0;
      QImage *ScreenImage = nullptr;
      QPixmap *pixmap = new QPixmap();
      ACL::ERenderMode renderMode;                ///< The mode that the image must be rendered to.
      boost::optional<FP_t> whitePoint;
      boost::optional<FP_t> blackPoint;
      QString szCoords, szPV;
      QString szRA, szDEC;
      bool isActive = false;                      ///< Indicates if the particular record is Active. Only changed by the dockWidget.

      explicit SControlImage() {}
      SControlImage(imaging::CAstroImageWindow *, std::shared_ptr<CAstroFile>);
      explicit SControlImage(imaging::CAstroImageWindow *);
      explicit SControlImage(SControlImage const &);
      virtual ~SControlImage();
    };

  } // namespace imaging
} // namespace AstroManager

#endif // IMAGECONTROL

