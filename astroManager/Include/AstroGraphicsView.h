//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:								AstroGraphicsView
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, CFitsIO.
// NAMESPACE:						AstroManager::imaging
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2017 Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2010-10-30 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef ASTROGRAPHICSVIEW_H
#define ASTROGRAPHICSVIEW_H

#include "qtExtensions/qt.h"

//#include "ImageDisplay.h"

namespace AstroManager
{
  namespace imaging
  {
    class CAstroImageWindow;

    // Class for managing the graphics view to do the specifics required for Astrometry and Photometry.

    class CAstroGraphicsView : public QGraphicsView
    {
      Q_OBJECT

    private:
      CAstroImageWindow *parentObject;
      double zoomFactor;
      double maxZoomFactor;
      double minZoomFactor;

      QBitmap bitmapAstrometry;
      QBitmap maskAstrometry;
      QCursor cursorAstrometry;

      QBitmap bitmapPhotometry;
      QBitmap maskPhotometry;
      QCursor cursorPhotometry;

      virtual void setMinZoom();

    protected:
      virtual void mouseMoveEvent(QMouseEvent *);
      virtual void mousePressEvent(QMouseEvent *);
      virtual void wheelEvent(QWheelEvent *);
      virtual void resizeEvent ( QResizeEvent * event );

    public:
      enum E_Mode
      {
        M_NONE,
        M_ZOOMDYNAMIC,
        M_ZOOMSELECTION,
        M_ZOOMIN,
        M_ZOOMOUT,
        M_SCROLL,
        M_ASTROMETRY,
        M_PHOTOMETRY
      };

      E_Mode mode;

      CAstroGraphicsView(QWidget * = 0);

      void setMode(E_Mode newMode);

      virtual void Paint();

        // Zooming functions

      virtual void zoomIn();
      virtual void zoomOut();
      virtual void zoomAll();
      virtual void zoomSelection();
      virtual void zoom11();

    private slots:
      void updateNavigator(int = 0);
    };

  }  // astrographicsview
}  // AIRDAS

#endif  // ASTROGRAPHICSVIEW_H
