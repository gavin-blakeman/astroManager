//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetPhotometry
// SUBSYSTEM:						The dock widget for single image photometry.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
// OVERVIEW:            The Photometry dock widget is used to control and select the photometry functions on images. The list of
//                      selected photometry objects is also displayed.
//
// CLASSES INCLUDED:    CPhotometryDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CPhotometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - AIRDAS 0000.00.000 release.
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef PHOTOMETRYDOCKWIDGET_H
#define PHOTOMETRYDOCKWIDGET_H

  // AIRDAS files

#include "dockWidgetImage.h"
#include "../photometry/photometryObservation.h"
#include "../qtExtensions/qt.h"
#include "../FrameWindow.h"

  // Standard libraries

#include <memory>

  // libAstroClass include file

#include <ACL>

  // Qwt Library

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace AstroManager
{
  namespace dockwidgets
  {
    class CPhotometryDockWidget : public CDockWidgetImage
    {
      Q_OBJECT

    private:
      QLabel *labelFilter;
      QLabel *labelZMAG;
      QTabWidget *infoTab;
        QWidget *tabAperture;
          QSpinBox *sbRadius1;
          QSpinBox *sbRadius2;
          QSpinBox *sbRadius3;
        QWidget *tabObjectInformation;
          QPushButton *btnObjectName;
          QLabel *tlObjectName;
          QLabel *labelRA;
          QLabel *labelDec;
          QLabel *tlVariable;
        QWidget *tabCounts;
          QLabel *tlADUAperture;
          QLabel *tlAreaAperture;
          QLabel *tlADUAnnulus;
          QLabel *tlAreaAnnulus;
        QWidget *tabPhotometry;
          QLabel *tlStar;
          QLabel *tlSky;
          QLabel *tlStarSky;
          QLabel *tlMagnitude;
          QLabel *labelMagnitudeError;
          QLabel *labelFWHM;
        QWidget *tabElectrons;
          QLabel *tlStarE;
          QLabel *tlNoiseE;
          QLabel *tlSNR;
        QWidget *tabProfile;
          QwtPlot *profilePlot;
          QwtPlotCurve *profileCurve;

      QTableWidget *tableWidgetPhotometry;

      QPushButton *pushButtonSelect;
      QPushButton *pushButtonRemove;

      ACL::AXIS_t uiRadius1, uiRadius2, uiRadius3;

      mdiframe::CFrameWindow *parentObject;

      void setupUI();

      void redraw();    // Redraws all the information in the window.
      void insertRow(int, photometry::PPhotometryObservation);
      void drawProfile(MCL::TPoint2D<ACL::FP_t> const &);

    protected:
    public:
      CPhotometryDockWidget(QWidget *, QAction *);

      void imageActivating();
      void imageDeactivating();
      virtual void imageChanged();

      virtual void setEnabled(bool);

      void referenceCompleted(photometry::PPhotometryObservation);
      void addNewObject(photometry::PPhotometryObservation);
      void displayPhotometry(photometry::PPhotometryObservation);

      ACL::AXIS_t getRadius1() const { return uiRadius1; }
      ACL::AXIS_t getRadius2() const { return uiRadius2; }
      ACL::AXIS_t getRadius3() const { return uiRadius3; }


    private slots:
      void eventButtonSelect(bool);
      void eventButtonRemove(bool);

      void eventButtonObjectName(bool);

      void eventRadius1Changed(int);
      void eventRadius2Changed(int);
      void eventRadius3Changed(int);

      void eventReferenceSelected(int, int);
    };

  }  // namespace dockwidgets
}  // namespace AstroManager

#endif // PHOTOMETRYDOCKWIDGET_H
