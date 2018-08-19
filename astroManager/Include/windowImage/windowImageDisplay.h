//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageDisplay.h
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Boost, ACL, SOFA, Qt
// NAMESPACE:						AstroManager::imageing
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
//                        -CARPAASubWindow
//                          - CAstroImageWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-03-01 GGB - Moved CAstroImageWindow into seperate file.
//                      2013-02-07 GGB - Moved CDialogImageCrop, CBinPixelsDialog, CImageFloatDialog into it's own file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_IMAGEDISPLAY_H
#define ASTROMANAGER_IMAGEDISPLAY_H

  // astroManager files

#include "../ACL/astroFile.h"
#include "../AstroGraphicsView.h"
#include "../astrometry/astrometryObservation.h"
#include "windowImage.h"
#include "../dialogs/dialogs.h"
#include "../Error.h"
#include "../FrameWindow.h"
#include "../qtExtensions/MdiSubWindow.h"
#include "../photometry/photometryObservation.h"

  // ACL Library

#include <ACL>

  // Boost Library

#include "boost/optional/optional.hpp"

  // Standard Library

#include <vector>

namespace AstroManager
{
  namespace imaging
  {
    class CAstroGraphicsView;

    // Window class for FITS File Display and processing.
    // All information is stored in this class, the dockwidgets access the information in this class and treat it as there own,
    // when the window is active.
    // The window does it's own drawing and updating of the screen.

    class CImageWindow : public CAstroImageWindow
    {
      Q_OBJECT

    private:
      enum
      {
        IDA_IMAGEDETAILS,
        IDA_MENUMAX
      };

      SControlImage controlImage;

      QComboBox *cbHDU;
      QTabWidget *tabWidget;
      QTableWidget *twHDU;
      QPlainTextEdit *teComments;
      QPlainTextEdit *teHistory;

      QLabel *labelDateTime;
      QLabel *labelTarget;
      QLabel *labelTargetRA;
      QLabel *labelTargetDEC;
      QLabel *labelExposure;
      QLabel *labelObservatory;
      QLabel *labelTelescope;
      QLabel *labelInstrument;
      QLabel *labelLongitude;
      QLabel *labelLatitude;
      QLabel *labelAltitude;
      QLabel *labelTemperature;
      QLabel *labelPressure;
      QLabel *labelRH;

      QWidget *imageTab;			// Handle to the tab to display the image and info
      QWidget *atableTab;
      QWidget *btableTab;
      QWidget *astrometryTab;
        QTableWidget *tableWidgetAstrometry;
      QWidget *photometryTab;
        QTableWidget *tableWidgetPhotometry;

      QGraphicsScene *gsImage;
      CAstroGraphicsView *gvImage;

      QAction *menuActions[IDA_MENUMAX];
      QMenu *popupMenu;
      QComboBox *comboBoxQuality;

      void historyUpdate();

      void createActions();
      void createMenu();
      void DisplayData();
      void DisplayKeywords();
      void SetupUI();

      virtual bool save();
      virtual bool saveAs();
      virtual bool saveBeforeClose();

      virtual void windowActivating();

      virtual void imageTabActivating();
      virtual void infoTabActivating();
      virtual void atableTabActivating();
      virtual void btableTabActivating();
      virtual void astrometryTabActivating();
      virtual void photometryTabActivating();

      void mousePressAstrometry(QMouseEvent *);
      void mousePressPhotometry(QMouseEvent *);

      void displayTargetInformation();
      void displayEquipmentInformation();
      void displaySiteInformation();
      void displayWeatherInformation();

        // Repainting functions

      void repaintAstrometry();
      void repaintPhotometry();
      void repaintAnnotations();

      void exportPhotometryAsCSV();
      void exportAstrometryAsCSV();

        // Source Extraction functions

      bool extractFindStars(ACL::TImageSourceContainer &);
      bool extractSimpleXY(ACL::TImageSourceContainer &);

    protected:
       void DisplayImage();
       void DisplayAsciiTable();
       void DisplayBinaryTable();
       void DisplayAstrometry();
       void DisplayPhotometry();

       virtual void closeEvent(QCloseEvent *);

    public:
      CImageWindow(PAstroFile, QWidget * = 0);
      virtual ~CImageWindow();

      virtual void setMode(EMode);

      virtual ESubWindowType getWindowType() const {return SWT_IMAGEWINDOW;}
      ACL::PAstroFile getAstroFile() const;

      virtual void eventMousePress(QMouseEvent *);

      virtual SControlImage *getControlImage() {return &controlImage;}

      void updateWindowTitle();

      void calibrateImage();
      void redrawImage();
      virtual void repaintImage();

        // File functions

      void exportAsJPEG();
      void exportAsTIFF();
      void exportAsPNG();
      void exportAsDNG();
      void exportAsCSV();

        // Print functions

      void print();
      void printPreview();
      void printSetup();

        // Edit functions

      void editMetadata();

        // Image transformation functions

      void flipImage();
      void flopImage();
      void rotateImage();
      void floatImage();
      void resampleImage();
      void binPixels();
      void cropImage();

        // Zoom functions

      void zoomAll();
      void zoomIn();
      void zoomOut();
      void zoomSelection();
      void zoom11();

        // Analyse functions

      void extractObjects();
      void loadObjects();
      void solveWCS();

        // Astrometry functions

      void astrometryReferenceAdd(astrometry::PAstrometryObservation);      // Adds an astrometry reference to the list.
      void astrometryLoadTargets();
      virtual void changeAstrometrySelection(astrometry::PAstrometryObservation);

        // Photometry functions

      void photometryReferenceAdd(photometry::PPhotometryObservation);   // Adds a photometry reference to the list.
      void photometryLoadTargets();
      virtual void changePhotometrySelection(photometry::PPhotometryObservation);


    private slots:
      void printImage(QPrinter *);
      void eventCurrentHDUChanged(int);
      void eventTabWidgetChanged(int);
      void eventEditWeather();
    };

  }	// namespace imagedisplay

}	// namespace AstroManager

#endif // __IMAGEDISPLAY_H
