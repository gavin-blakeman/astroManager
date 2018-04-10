//*********************************************************************************************************************************
//
// PROJECT:							AstroManager
// FILE:								FrameWindow
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2005-2018 Gavin Blakeman.
//                      This file is part of the AstroManager software.
//
//                      AstroManager is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      AstroManager is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with AstroManager.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Implements the MDI Frame window for the application. There is a single class implemented:
//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CFrameWindow
//
// CLASS HIERARCHY:     QMainWindow
//                        - CFrameWindow
//
// HISTORY:             2018-02-03 GGB - Rename Project to AstroManager
//                      2018-02-01 GGB - Added support for resources (sites, observers, telescopes)
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-05-27 GGB - Moved all dialogs into seperate files.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-02-03 GGB - Moved CMdiSubWindow to own file and renamed.
//                      2012-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//                      2005-07-06 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef FRAMEWINDOW_H
#define FRAMEWINDOW_H

  // AIRDAS include files

#include "ACL/astroFile.h"
#include "dockWidgets/dockWidget.h"
#include "Error.h"
#include "qtExtensions/MdiSubWindow.h"
#include "qtExtensions/qt.h"
#include "VSOP.h"

// Standard include files

#include <list>
#include <map>
#include <memory>
#include <vector>

  // Boost Library

#include "boost/filesystem.hpp"

#ifdef _MSC_VER
# pragma warning( disable : 4290 )  /* Disable the warning about C++ exception specifications. */
#endif

namespace AstroManager
{
  namespace mdiframe
  {
      /// Constant values for all the action items. Using the enum makes it easy to add additional items without having to recount
      /// or renumber all the items.

    enum EActionItems
    {
      IDA_FILE_OPEN,
      IDA_FILE_INSPECT,
      IDA_FILE_SEARCH,
      IDA_FILE_SAVE,
      IDA_FILE_SAVEAS,
      IDA_FILE_SAVE_DATABASE,
      IDA_FILE_EXPORTIMAGE_DNG,
      IDA_FILE_EXPORTIMAGE_JPEG,
      IDA_FILE_EXPORTIMAGE_TIFF,
      IDA_FILE_EXPORTIMAGE_PNG,
      IDA_FILE_EXPORT_CSV,
      IDA_FILE_BATCHCONVERT,
      IDA_FILE_IMPORTIMAGES,
      IDA_FILE_PRINT,
      IDA_FILE_PRINTPREVIEW,
      IDA_FILE_PRINTSETUP,
      IDA_FILE_EXIT,

      IDA_EDIT_COPY,
      IDA_EDIT_IMAGEMETADATA,
      IDA_EDIT_ANNOTATEIMAGE,
      IDA_EDIT_OPTIONS,
      IDA_EDIT_RESOURCES,

      IDA_ANALYSIS_EXTRACTOBJECTS,
      IDA_ANALYSIS_LOADOBJECTS,

      IDA_ASTROMETRY_REFERENCEIMAGE,
      IDA_ASTROMETRY_LOADTARGETLIST,

      IDA_PHOTOMETRY_SINGLEIMAGE,
      IDA_PHOTOMETRY_LOADTARGETLIST,
      IDA_PHOTOMETRY_LIGHTCURVES,

      IDA_CALIBRATE_SINGLEIMAGE,
      IDA_CALIBRATE_MULTIIMAGES,
      IDA_CALIBRATE_CREATEMASTERDARK,
      IDA_CALIBRATE_CREATEMASTERFLAT,
      IDA_CALIBRATE_CREATEMASTERBIAS,
      IDA_PROCESS_COMPAREIMAGES,
      IDA_PROCESS_STACKIMAGES,

      IDA_IA_COLOUR_SPLIT_RGB,
      IDA_IA_COLOUR_SPLIT_LRGB,
      IDA_IA_COLOUR_SPLIT_GRAYSCALE,

      IDA_UTILITIES_CALCULATE_GREGORIAN2JD,
      IDA_UTILITIES_CALCULATE_JD2GREGORIAN,

      IDA_UTILITIES_OBJECTINFORMATION,

      IDA_UTILITIES_WEATHER_HISTORY,
      IDA_UTILITIES_WEATHER_MASTERDATA,

      IDA_UTILITIES_PLANNINGWINDOW,

      IDA_TRANSFORM_FLIP,
      IDA_TRANSFORM_FLOP,
      IDA_TRANSFORM_RESAMPLE,
      IDA_TRANSFORM_BINPIXELS,
      IDA_TRANSFORM_FLOAT,
      IDA_TRANSFORM_CROP,
      IDA_TRANSFORM_ROTATE,

      IDA_VIEW_IMAGECONTROL,
      IDA_VIEW_HISTOGRAM,
      IDA_VIEW_INSTRUMENTINFORMATION,
      IDA_VIEW_ZOOMIN,
      IDA_VIEW_ZOOMOUT,
      IDA_VIEW_ZOOMALL,
      IDA_VIEW_ZOOMSELECTION,
      IDA_VIEW_ZOOM11,
      IDA_VIEW_CONSOLEWINDOW,
      IDA_VIEW_ASTROMETRY,
      IDA_VIEW_PHOTOMETRY,
      IDA_VIEW_ANNOTATIONS,
      IDA_VIEW_MAGNIFY,
      IDA_VIEW_NAVIGATOR,
      IDA_VIEW_DOCKWIDGET_WEATHERSCALE,
      IDA_VIEW_DOCKWIDGET_WEATHERPARAMETERS,

      IDA_WINDOW_CLOSE,
      IDA_WINDOW_CLOSEALL,
      IDA_WINDOW_TILE,
      IDA_WINDOW_CASCADE,
      IDA_WINDOW_NEXT,
      IDA_WINDOW_PREVIOUS,

      IDA_HELP_CONTENTS,
      IDA_HELP_ABOUTATID,
      IDA_HELP_ABOUT_AIRDAS,
      IDA_HELP_ABOUTQT,
    };

    enum EDockWidgets
    {
      IDDW_IMAGECONTROL,
      IDDW_INSTRUMENTINFORMATION,
      IDDW_ASTROMETRYCONTROL,
      IDDW_PHOTOMETRYCONTROL,
      IDDW_MESSAGECONTROL,
      IDDW_VIEW_HISTOGRAM,
      IDDW_VIEW_MAGNIFY,
      IDDW_VIEW_NAVIGATOR,
      IDDW_WEATHER_SCALE,
      IDDW_WEATHER_PARAMETERS,
    };

    enum EToolBars
    {
      IDTB_FILE,
      IDTB_EDIT,
      IDTB_IMAGE_VIEW,
      IDTB_IMAGE_TRANSFORM,
    };

    enum ESubMenus
    {
      IDSM_NULL_FILE,
      IDSM_NULL_EDIT,

      IDSM_IMAGE_FILE,
      IDSM_IMAGE_EDIT,
      IDSM_IMAGE_VIEW,
      IDSM_IMAGE_CALIBRATE,
      IDSM_IMAGE_ANALYSE,
      IDSM_IMAGE_TRANSFORM,
      IDSM_IMAGE_ENHANCE,
      IDSM_IMAGE_COLOUR,

      IDSM_UTILITIES,
      IDSM_WINDOW,
      IDSM_HELP,

      IDSM_RECENT_FILE,

    };

    enum EMenuBars
    {
      IDMB_NULL,
      IDMB_IMAGE,
      IDMB_WEATHER,
    };

    class CFrameWindow : public QMainWindow
    {
      Q_OBJECT

    private:
      CMdiSubWindow::EWindowClass currentWindowClass;

      QTimer *timer1s;
      QMdiArea *mdiArea;
      QSignalMapper *windowMapper;

      typedef std::unique_ptr<dockwidgets::CDockWidget> PDockWidget;
      std::map<EDockWidgets, PDockWidget> dockWidgets;

      typedef std::unique_ptr<QAction> PAction;
      std::map<EActionItems, PAction> menuActions;

      typedef std::unique_ptr<QToolBar> PToolBar;
      std::map<EToolBars, PToolBar> toolBars;

      typedef CMdiSubWindow *PMdiSubWindow;
      std::map<ESubWindowType, PMdiSubWindow> uniqueWindows;

      typedef std::unique_ptr<QMenu> PMenu;
      std::map<ESubMenus, PMenu> subMenus;
      std::vector<QAction *> recentFileActions;

      std::list<boost::filesystem::path> lastOpened;

      //int currentModule;

      QLabel *labelRAM;
      QLabel *labelSwap;
      QLabel *labelCPU;
      QLabel *labelUTC;
      QLabel *labelJD;

      QStatusBar *StatusBar;

        // Command Functions

      void createActions(void);
      void createToolBars();
      void createStatusBar();
      void createDockWidgets();
      void createSubMenus();

      void setupUI();

      void readSettings();
      void writeSettings();

        // Sub Menu Creation functions

      void updateRecentFileMenu(unsigned int);
      void createRecentFileMenu();
      void populateRecentFileMenu();
      void updateWindowMenuStatus();


      void loadFromFile(boost::filesystem::path &);

      void setBaseActionStates();

      void activateWindowClassNull();
      void activateWindowClassImage();
      void activateWindowClassWeather();

    protected:
      void closeEvent(QCloseEvent *);

    public:
      CFrameWindow();
      virtual ~CFrameWindow();

      QAction *getAction(EActionItems) const;
      dockwidgets::CDockWidget &getDockWidget(EDockWidgets);

        // Management function

      CMdiSubWindow *activeMdiChild() const;
      void childClosing(QMdiSubWindow *);


      void enableDockWidgetsImage(bool);

      void imageCreateWindow(PAstroFile);
      void imageOpenFromDatabase(imageID_t);

      private slots:
        void eventUnimplemented();

        void openRecentFile();
        void subWindowActivated(QMdiSubWindow *);

        void updateWindowMenu();
        QWidget *createMdiChild();
        void setActiveSubWindow(QWidget *window);
        void menuViewObjectInfo();
        void eventPhotometryLightCurves();
        void eventTimer1s();

          // File Functions

        void eventImageOpen();
        void eventFileInspect();
        void eventFileSearch();
        void eventFileSave();
        void eventFileSaveAs();
        void eventFileSaveDatabase();
        void eventExportAsDNG();
        void eventExportAsTIFF();
        void eventExportAsJPEG();
        void eventExportAsPNG();
        void eventExportAsCSV();
        void eventFileBatchConvert();
        void eventFileUploadImages();
        void eventPrint();
        void eventPrintPreview();
        void eventPrintSetup();

          // Edit Functions

        void eventEditcopy();
        void eventEditImageMetadata();
        void eventEditOptions();
        void eventEditResources();

          // Analysis Functions

        void eventAnalysisExtractObjects();
        void eventAnalysisLoadObjects();

          // Astrometry functions

        void eventReferenceImage();
        void eventAstrometryLoadTargets();

          // Photometry functions

        void eventPhotometrySingleImage();
        void eventPhotometryLoadTargets();

          // Configuration functions

        void eventConfigureSites();
        void eventConfigureObservers();
        void eventConfigureTelescopes();
        void eventConfigureInstruments();
        void eventConfigureCatalogPrefs();

          // Image Calibration Functions

        void eventImagingSingleCalibration();
        void eventImagingMultipleCalibration();
        void eventCreateMasterDarkFrame();
        void eventCreateMasterFlatFrame();
        void eventCreateMasterBiasFrame();
        void eventCompareImages();
        void eventImagingStackImages();

          // View functions

        void eventZoomIn();
        void eventZoomOut();
        void eventZoomAll();
        void eventZoomSelection();
        void eventZoom11();
        void eventImageControl();
        void eventMessageWidget();

        void eventViewHistogram();
        void eventViewAstrometry();
        void eventViewPhotometry();
        void eventViewAnnotations();
        void eventViewMagnify();
        void eventViewNavigator();
        void eventViewDockWidgetWeatherParameters();
        void eventViewDockWidgetWeatherScale();

          // Image Transformation menu items

        void eventFlipImage();
        void eventFlopImage();
        void eventTransformRotate();
        void eventFloatImage();
        void eventResampleImage();
        void eventBinPixels();
        void eventImageCrop();

          // Colour Image Manipulation

        void eventConvertRGB();
        void eventConvertLRGB();
        void eventConvertGrayscale();

          // Utility Functions

        void eventCalculateGreg2JD();
        void eventCalculateJD2Gregorian();


        void eventWeatherHistory();
        void eventUtilityPlanning();

          // Help procedures

        void HelpAbout();
        void HelpATID();
    };

  }	// namespace mdiframe
}		// namespace AstroManager

#endif // FRAMEWINDOW_H
