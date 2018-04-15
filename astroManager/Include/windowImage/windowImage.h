//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								AstroImageWindow
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imaging
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CAstroImageWindow         - Window for displaying and manipulating astronomical images.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CMdiSubWindow
//                          - CAstroImageWindow
//                            - CImageWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-03-01 GGB - Moved CAstroImageWindow into seperate file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef ASTROIMAGEWINDOW_H
#define ASTROIMAGEWINDOW_H

  // astroManager include files

#include "../astrometry/astrometryObservation.h"
#include "../imaging/imageControl.h"
#include "../qtExtensions/MdiSubWindow.h"
#include "../photometry/photometryObservation.h"

  // Miscellaneous libraries

#include <ACL>

namespace AstroManager
{
  namespace imaging
  {
    class CAstroGraphicsView;

    /// @brief  Pure virtual class to use in conjuction with CAstroGraphicsView. Any classes wishing to embed CAstroGraphicsView
    /// must derive from this class

    class CAstroImageWindow : public CMdiSubWindow
    {
      Q_OBJECT

    public:
      enum EMode
      {
        M_NONE,
        M_ASTROMETRY,
        M_PHOTOMETRY,
        M_SPECTROSCOPY
      };

    private:
    protected:
      EMode mode;

      void drawAstrometryIndicator(astrometry::PAstrometryObservation, QPen const &);
      void drawCrossIndicator(astrometry::PAstrometryObservation, QPen const &);
      void drawPhotometryIndicator(photometry::PPhotometryObservation, QPen const &);
      void drawCircleIndicator(astrometry::PAstrometryObservation, QPen const &);

      //void processAstroFile(ACL::PAstroFile);

      virtual void imageChange(SControlImage *);

    public:
      CAstroImageWindow(QWidget *parent = 0);

      virtual EWindowClass getWindowClass() const { return WC_IMAGE; }

      virtual void setMode(EMode) = 0;

      virtual void eventMousePress(QMouseEvent *) = 0;

      virtual SControlImage *getControlImage() = 0;
      virtual void repaintImage() = 0;

        // Astrometry functions

      virtual void changeAstrometrySelection(astrometry::PAstrometryObservation) = 0;

        // Photometry functions

      virtual void changePhotometrySelection(photometry::PPhotometryObservation) = 0;

    };

  }  // namespace imaging
}  // namespace AstroManager

#endif // ASTROIMAGEWINDOW_H
