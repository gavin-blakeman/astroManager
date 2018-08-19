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

#include "../../include/imaging/imageControl.h"

namespace astroManager
{
  namespace imaging
  {

    //*****************************************************************************************************************************
    //
    // SControlImage
    //
    //*****************************************************************************************************************************

    /// @brief Default constructor.
    /// @param[in] parent - The parent AstroImageWindow.
    /// @throws std::bad_alloc
    /// @version 2015-08-01/GGB - Added renderMode member.
    /// @version 2013-05-20/GGB - Added pixmap.
    /// @version 2013-03-09/GGB - Function created.

    SControlImage::SControlImage(imaging::CAstroImageWindow *parent) : parent_(parent), astroFile(), currentHDB(0),
      ScreenImage(nullptr), isActive(false), whitePoint(), blackPoint(), pixmap(new QPixmap()), renderMode(ACL::RM_GREY8)
    {
    }

    /// @brief Default constructor. Loads the white and black point from the astroFile if it is specified.
    /// @throws std::bad_alloc
    /// @version 2015-08-01/GGB - Added renderMode member.
    /// @version 2013-06-16/GGB - Changed code around the astrometry store copy to correct bug.
    /// @version 2013-05-20/GGB - Added the pixmap member.
    /// @version 2013-05-08/GGB - Code to initialise the astrometry and photometry dockwidgets:: objects added.
    /// @version 2012-07-17/GGB - Added parameter to function call and extract white and black levels.
    /// @version 2011-06-12/GGB - Function created

    SControlImage::SControlImage(imaging::CAstroImageWindow *parent, PAstroFile af) : parent_(parent), astroFile(af),
      currentHDB(0), ScreenImage(nullptr), pixmap(new QPixmap()), renderMode(ACL::RM_GREY8), isActive(false), whitePoint(),
      blackPoint()
    {
      if (astroFile)
      {
        whitePoint = astroFile->whitePoint(currentHDB);
        blackPoint = astroFile->blackPoint(currentHDB);
      };

      // Create the list of astrometry information.

      if ( astroFile->hasAstrometryHDB() )
      {
        ACL::SPAstrometryObservation ao = astroFile->astrometryObjectFirst();

        while (ao)
        {
          // Create the dockwidgets::CPhotometryObservation and copy the libAstroClass::CPhotometryObservation

          astrometry::PAstrometryObservation nao(new astrometry::CAstrometryObservation(*ao));

          astrometryObservations.push_back(nao);    // Add this to the new list.

          ao = astroFile->astrometryObjectNext();
        };

        astroFile->astrometryObjectRemoveAll();

        // Recreate the list.

        astrometry::DAstrometryObservationStore::const_iterator iterator;

        for (iterator = astrometryObservations.begin(); iterator != astrometryObservations.end(); ++iterator)
        {
          astroFile->astrometryObjectAdd(std::dynamic_pointer_cast<ACL::CAstrometryObservation>(*iterator));
        };
      };

        // Create the list of photometry information

      if ( astroFile->hasPhotometryHDB() )
      {
        ACL::SPPhotometryObservation po = astroFile->photometryObjectFirst();

        while (po)
        {
          // Create the dockwidgets::CPhotometryObservation and copy the libAstroClass::CPhotometryObservation

          photometry::PPhotometryObservation npo(new photometry::CPhotometryObservation(*po));

          photometryObservations.push_back(npo);    // Add this to the new list.

          po = astroFile->photometryObjectNext();
        };

        astroFile->photometryObjectRemoveAll();   // Delete all the items in the photometry HDB

        // Re-create the objects in the astrofile.

        photometry::DPhotometryObservationStore::const_iterator iterator;

        for (iterator = photometryObservations.begin(); iterator != photometryObservations.end(); iterator++)
        {
          astroFile->photometryObjectAdd(std::dynamic_pointer_cast<ACL::CPhotometryObservation>((*iterator)));
        }

        astroFile->isDirty(false);    // Make the astro file clean even though the data has changed.
      };
    }

    /// @brief Copy constructor for the class.
    /// @details The underlying astroFile is copied. (Copy created.) The POD is copied as it stands. THe pixmap, pixmapitem and
    ///          screen image are recreated. The photometry and astrometry lists are recreated.
    /// @throws std::bad_alloc
    /// @version 2015-08-01/GGB - Added renderMode member.
    /// @version 2013-07-09/GGB - Function created.

    SControlImage::SControlImage(SControlImage const &toCopy) : parent_(toCopy.parent_), astroFile(toCopy.astroFile),
      currentHDB(toCopy.currentHDB), ScreenImage(nullptr), pixmap(nullptr), renderMode(toCopy.renderMode),
      whitePoint(toCopy.whitePoint), blackPoint(toCopy.blackPoint), currentAstrometrySelection(), currentPhotometrySelection()
    {
      if (toCopy.ScreenImage)
      {
        ScreenImage = new QImage(*toCopy.ScreenImage);
        pixmap = new QPixmap();
        pixmap->convertFromImage(*ScreenImage);
      };

      // Create the list of astrometry information.

      if ( astroFile->hasAstrometryHDB() )
      {
        ACL::SPAstrometryObservation ao = astroFile->astrometryObjectFirst();

        while (ao)
        {
          // Create the dockwidgets::CPhotometryObservation and copy the libAstroClass::CPhotometryObservation

          astrometry::PAstrometryObservation nao(new astrometry::CAstrometryObservation(*ao));

          astrometryObservations.push_back(nao);    // Add this to the new list.

          ao = astroFile->astrometryObjectNext();
        };

        astroFile->astrometryObjectRemoveAll();

        // Recreate the list.

        astrometry::DAstrometryObservationStore::const_iterator iterator;

        for (iterator = astrometryObservations.begin(); iterator != astrometryObservations.end(); ++iterator)
        {
          astroFile->astrometryObjectAdd(std::dynamic_pointer_cast<ACL::CAstrometryObservation>(*iterator));
        };

      };

      // Create the list of photometry information

      if ( astroFile->hasPhotometryHDB() )
      {
        ACL::SPPhotometryObservation po = astroFile->photometryObjectFirst();

        while (po)
        {
          // Create the dockwidgets::CPhotometryObservation and copy the libAstroClass::CPhotometryObservation

          photometry::PPhotometryObservation npo(new photometry::CPhotometryObservation(*po));

          photometryObservations.push_back(npo);    // Add this to the new list.

          po = astroFile->photometryObjectNext();
        };

        astroFile->photometryObjectRemoveAll();   // Delete all the items in the photometry HDB

        // Re-create the objects in the astrofile.

        photometry::DPhotometryObservationStore::const_iterator iterator;

        for (iterator = photometryObservations.begin(); iterator != photometryObservations.end(); iterator++)
        {
          astroFile->photometryObjectAdd(std::dynamic_pointer_cast<ACL::CPhotometryObservation>((*iterator)));
        }

        astroFile->isDirty(false);    // Make the astro file clean even though the data has changed.

      };
    }

    /// @brief Ensures all objects on the control image are deleted correctly.
    /// @throws None
    /// @version 2013-05-20/GGB - Added the pixmap member.
    /// @version 2011-06-12/GGB - Function created

    SControlImage::~SControlImage()
    {
      if (pixmap)
      {
        delete pixmap;
        pixmap = nullptr;
      };
      if (ScreenImage)
      {
        delete ScreenImage;
        ScreenImage = nullptr;
      };
    }

  } // namespace imaging

} // namespace AstroManager
