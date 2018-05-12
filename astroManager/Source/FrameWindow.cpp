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
//                      2015-06-13/GGB - Remove all references to DialogConfigureDatabase (Functionality in Options Dialog)
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-03 GGB - Moved CMdiSubWindow to own file and renamed.
//                      2012-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//                      2005-07-06 GGB - File Created
//
//*********************************************************************************************************************************

#include "../Include/FrameWindow.h"

  // astroManager include files

#include "../Include/ACL/astroFile.h"
#include "../Include/Configure.h"
#include "../Include/database/databaseARID.h"
#include "../Include/database/databaseATID.h"
#include "../Include/database/databaseWeather.h"
#include "../Include/dialogs/dialogOptions.h"
#include "../Include/dialogs/dialogSelectImageVersion.h"
#include "../Include/dialogs/dialogEditResources.h"
#include "../Include/dockWidgets/dockWidgetAstrometry.h"
#include "../Include/dockWidgets/dockWidgetHistogram.h"
#include "../Include/dockWidgets/dockWidgetImageInformation.h"
#include "../Include/dockWidgets/dockWidgetMagnify.h"
#include "../Include/dockWidgets/dockWidgetMessage.h"
#include "../Include/dockWidgets/dockWidgetNavigator.h"
#include "../Include/dockWidgets/dockWidgetPhotometry.h"
#include "../Include/dockWidgets/dockWidgetWeatherParameters.h"
#include "../Include/dockWidgets/dockWidgetWeatherScale.h"
#include "../Include/FileBatchConvert.h"
#include "../Include/windowCalibration/ImageCalibration.h"
#include "../Include/ImageComparison.h"
#include "../Include/windowImage/windowImageDisplay.h"
#include "../Include/windowImage/windowImageStacking.h"
#include "../Include/qtExtensions/qt.h"
#include "../Include/Settings.h"
#include "../Include/TextEditorFITS.h"
#include "../Include/Photometry.h"
#include "../Include/Utilities.h"
#include "../Include/windowCalculation/gregorian2JD.h"
#include "../Include/windowCalculation/JD2Gregorian.h"
#include "../Include/astroManagerHelp.h"
#include "../Include/VSOPView.h"
#include "../Include/windowPlanning/windowPlanning.h"
#include "../Include/windowWeather/windowWeatherHistory.h"
#include "../Include/windowSelectImage.h"

  // Standard libraries

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <utility>

  // Boost Library

#include "boost/thread/thread.hpp"
#include "boost/format.hpp"

// Other Libraries Library

#include <ACL>
#include <GCL>

namespace AstroManager
{
  namespace mdiframe
  {
    unsigned int const MAXRECENTDEPTH(20);

    QString const TB_FILE       ("File Toolbar");
    QString const TB_EDIT       ("Edit Toolbar");
    QString const TB_VIEW       ("View Toolbar");
    QString const TB_TRANSFORM  ("Transform Toolbar");

    //*****************************************************************************************************************************
    //
    // CFrameWindow
    //
    // Main window class for the AstroManager frame window.
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the CFrameWindow Class
    /// @details Calls the CMDIFrameWindow class for the default constructor.
    /// @throws None.
    /// @version 2018-02-03/GGB - Changed application name to AstroManager.
    /// @version 2017-07-03/GGB - Added class member currentWindowClass.
    /// @version 2013-05-30/GGB - Removed some unused data members.
    /// @version 2013-01-22/GGB - Added support for the recent file actions.
    /// @version 2011-05-28/GGB - Added support for the menuActions array and the dockWidgets array.
    /// @version 2010-05-05/GGB - Converted for use with the Qt API.
    /// @version 2009-09-20/GGB - Created the CVSOPRegistryClass and moved all code dealing with the registry into the class.
    /// @version 2009-09-04/GGB - Added code to exit if the ADODB DLL did not initialise.
    /// @version 2005-07-06/GGB - Function created

    CFrameWindow::CFrameWindow() : currentWindowClass(CMdiSubWindow::WC_NULL), dockWidgets(), menuActions(), toolBars(),
      uniqueWindows(), subMenus(), recentFileActions(), lastOpened()
    {
      size_t nIndex;
      std::string path;

        // Load the recent file list.

      QStringList files = settings::astroManagerSettings->value(settings::FILE_LASTOPENED).toStringList();
      for (nIndex = 0; nIndex < ((size_t) files.size()); nIndex++)
      {
        path = files[nIndex].toStdString();
        lastOpened.push_back(path);
      };

      setupUI();

      createActions();
      createToolBars();
      createStatusBar();
      createDockWidgets();
      createSubMenus();

      activateWindowClassNull();

      readSettings();

      setWindowTitle(tr("AstroManager - Astronomy Manager"));
      setUnifiedTitleAndToolBarOnMac(true);

      setWindowIcon(QIcon(":/icons/iconApplication.png"));

        // Create and start the timer for the 1s update of the time

      timer1s = new QTimer(this);
      connect(timer1s, SIGNAL(timeout()), this, SLOT(eventTimer1s()));
      timer1s->start(1000);
    }

    /// @brief Destructor for the CFrameWindow class.
    /// @details  Destroys all windows created elements (Timers etc). Destroys all dynamically created elements.
    /// @throws None.
    /// @version 2017-07-14/GGB - Set menuBar to nullptr before exit. All menuBars deleted as unique_ptr.
    /// @version 2013-01-22/GGB - Added support for recent files.
    /// @version 2011-05-28/GGB - Added support for the menuActions array and the dockWidgets array.
    /// @version 2010-05-27/GGB - Modified to use Qt API.
    /// @version 2009-09-04/GGB - Function created.

    CFrameWindow::~CFrameWindow()
    {
      size_t nIndex;

      menuBar()->clear();

        // Stop and delete the timer before we exit.

      if (timer1s)
      {
        timer1s->stop();
        delete timer1s;
        timer1s = nullptr;
      };

      // Destroy all dynamically allocated recent file information.

      for (nIndex = 0; nIndex < recentFileActions.size(); nIndex++)
      {
        if (recentFileActions[nIndex])
        {
          delete recentFileActions[nIndex];
          recentFileActions[nIndex] = nullptr;
        };
      };
    }

    /// @brief Sets up the menu when a Image window is activated.
    /// @throws None.
    /// @version 2017-07-16/GGB - Function created.

    void CFrameWindow::activateWindowClassImage()
    {
      menuBar()->clear();

      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_FILE]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_EDIT]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_VIEW]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_CALIBRATE]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_ANALYSE]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_TRANSFORM]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_ENHANCE]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_COLOUR]);
      menuBar()->addMenu(&*subMenus[IDSM_UTILITIES]);
      menuBar()->addMenu(&*subMenus[IDSM_WINDOW]);
      menuBar()->addMenu(&*subMenus[IDSM_HELP]);
    }

    /// @brief Sets up the menu when a Null window is activated.
    /// @throws None.
    /// @version 2017-07-21/GGB - Added calibration menu (Bug #93)
    /// @version 2017-07-16/GGB - Function created.

    void CFrameWindow::activateWindowClassNull()
    {
      menuBar()->clear();

      menuBar()->addMenu(&*subMenus[IDSM_NULL_FILE]);
      menuBar()->addMenu(&*subMenus[IDSM_NULL_EDIT]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_CALIBRATE]);
      menuBar()->addMenu(&*subMenus[IDSM_UTILITIES]);
      menuBar()->addMenu(&*subMenus[IDSM_WINDOW]);
      menuBar()->addMenu(&*subMenus[IDSM_HELP]);
    }

    /// @brief Function called when a child window associated with weather is activated.
    /// @details Displays the toolbars and menus for weather.
    /// @version 2017-07-21/GGB - Added calibration menu (Bug #93)
    /// @version 2017-07-13/GGB - Function created.

    void CFrameWindow::activateWindowClassWeather()
    {
      menuBar()->clear();

      menuBar()->addMenu(&*subMenus[IDSM_NULL_FILE]);
      menuBar()->addMenu(&*subMenus[IDSM_NULL_EDIT]);
      menuBar()->addMenu(&*subMenus[IDSM_IMAGE_CALIBRATE]);
      menuBar()->addMenu(&*subMenus[IDSM_UTILITIES]);
      menuBar()->addMenu(&*subMenus[IDSM_WINDOW]);
      menuBar()->addMenu(&*subMenus[IDSM_HELP]);
    }

    /// @brief Returns the active MDI child window. This is converted to a CMdiSubWindow.
    /// @returns The currently active MDI child window.
    /// @returns nullptr on failure.
    /// @throws None.
    /// @version 2011-03-06/GGB - Function created.

    CMdiSubWindow *CFrameWindow::activeMdiChild() const
    {
      if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
      {
        return qobject_cast<CMdiSubWindow *>(activeSubWindow);
      }
      else
      {
        return nullptr;
      }
    }

    /// @brief Creates the actions for the application
    /// @throws std::bad_alloc
    /// @version 2018-02-03/GGB - Added Planning Window support. Removed some dead code. (Commented out)
    /// @version 2017-07-28/GGB - Added action for File | Search
    /// @version 2017-07-03/GGB - Added the action for the weather scale dock widget.
    /// @version 2017-06-20/GGB - Added action for IDA_UTILITIES_WEATHER_HISTORY
    /// @version 2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    /// @version 2015-08-03/GGB - Added action IDA_FILE_INSPECT
    /// @version 2013-08-19/GGB - Removed the instrument dock widget.
    /// @version 2016-04-02/GGB - Removed code for configuring the database connection.
    /// @version 2013-08-18/GGB - Added function to load photometry target list.
    /// @version 2013-05-30/GGB - Removed the information mode action.
    /// @version 2013-05-24/GGB - Added the View | Navigator action.
    /// @version 2013-05-19/GGB - Added the View | Magnify action.
    /// @version 2013-05-10/GGB - Added action for export as CSV.
    /// @version 2013-03-29/GGB - Added/updated actions for printing, added code to update the action states when no window is open.
    /// @version 2013-03-15/GGB - Added option to view Astrometry,photometry indicators and annotations.
    /// @version 2013-02-03/GGB - set the enabled on some actions to false.
    /// @version 2013-02-02/GGB - Added the View | Histogram Action.
    /// @version 2013-01-26/GGB - Removed importIERS and add DAT functionality.
    /// @version 2012-01-04/GGB - Added File export functionality
    /// @version 2010-07-23/GGB - Function created.

    void CFrameWindow::createActions()
    {
        // File Menu Actions

      menuActions.emplace(IDA_FILE_OPEN, std::make_unique<QAction>(QIcon(":/images/open.png"), tr("&Open"), this));
      menuActions[IDA_FILE_OPEN]->setShortcuts(QKeySequence::Open);
      menuActions[IDA_FILE_OPEN]->setStatusTip(tr("Open an existing image file"));
      menuActions[IDA_FILE_OPEN]->setToolTip(tr("Open an existing image file"));
      connect(&*menuActions[IDA_FILE_OPEN], SIGNAL(triggered()), this, SLOT(eventImageOpen()));

      menuActions.emplace(IDA_FILE_INSPECT, std::make_unique<QAction>(QIcon(":/images/x_ray.png"), tr("Inspect"), this));
      menuActions[IDA_FILE_INSPECT]->setStatusTip(tr("Inspect the keywords in a FITS file."));
      menuActions[IDA_FILE_INSPECT]->setToolTip(tr("Inspect the keywords in a FITS file."));
      connect(&*menuActions[IDA_FILE_INSPECT], SIGNAL(triggered()), this, SLOT(eventFileInspect()));

      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        menuActions.emplace(IDA_FILE_SEARCH, std::make_unique<QAction>(QIcon(":/icons/data_chooser.png"), tr("Search Image"), this));
        menuActions[IDA_FILE_SEARCH]->setStatusTip(tr("Search images in the ARID database."));
        menuActions[IDA_FILE_SEARCH]->setToolTip(tr("Search images in the ARID database."));
        connect(&*menuActions[IDA_FILE_SEARCH], SIGNAL(triggered()), this, SLOT(eventFileSearch()));
      };

      menuActions.emplace(IDA_FILE_SAVE, std::make_unique<QAction>(QIcon(":/icons/image/image_save.png"), tr("&Save"), this));
      menuActions[IDA_FILE_SAVE]->setShortcuts(QKeySequence::Save);
      menuActions[IDA_FILE_SAVE]->setStatusTip(tr("Save the document to disk"));
      connect(&*menuActions[IDA_FILE_SAVE], SIGNAL(triggered()), this, SLOT(eventFileSave()));

      menuActions.emplace(IDA_FILE_SAVEAS, std::make_unique<QAction>(QIcon(":/icons/save_as.png"), tr("Save &As..."), this));
      menuActions[IDA_FILE_SAVEAS]->setShortcuts(QKeySequence::SaveAs);
      menuActions[IDA_FILE_SAVEAS]->setStatusTip(tr("Save the document under a new name"));
      connect(&*menuActions[IDA_FILE_SAVEAS], SIGNAL(triggered()), this, SLOT(eventFileSaveAs()));

      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        menuActions.emplace(IDA_FILE_SAVE_DATABASE, std::make_unique<QAction>(QIcon(":/icons/database/database_save.png"), tr("Save to Database"), this));
        menuActions[IDA_FILE_SAVE_DATABASE]->setStatusTip(tr("Save image in the ARID database."));
        menuActions[IDA_FILE_SAVE_DATABASE]->setToolTip(tr("Save image in the ARID database."));
        connect(&*menuActions[IDA_FILE_SAVE_DATABASE], SIGNAL(triggered()), this, SLOT(eventFileSaveDatabase()));
      };

      menuActions.emplace(IDA_FILE_EXPORTIMAGE_DNG, std::make_unique<QAction>(QIcon(":/images/File_DNG_Image.png"), tr("as DNG"), this));
      menuActions[IDA_FILE_EXPORTIMAGE_DNG]->setStatusTip(tr("Export information in Digital Negative Format."));
      connect(&*menuActions[IDA_FILE_EXPORTIMAGE_DNG], SIGNAL(triggered()), this, SLOT(eventExportAsDNG()));

      menuActions.emplace(IDA_FILE_EXPORTIMAGE_JPEG, std::make_unique<QAction>(QIcon(":/images/File_JPG_Image.png"), tr("as JPEG"), this));
      menuActions[IDA_FILE_EXPORTIMAGE_JPEG]->setStatusTip(tr("Export information in JPEG Format."));
      connect(&*menuActions[IDA_FILE_EXPORTIMAGE_JPEG], SIGNAL(triggered()), this, SLOT(eventExportAsJPEG()));

      menuActions.emplace(IDA_FILE_EXPORTIMAGE_TIFF, std::make_unique<QAction>(QIcon(":/images/File_TIFF_Image.png"), tr("as TIFF"), this));
      menuActions[IDA_FILE_EXPORTIMAGE_TIFF]->setStatusTip(tr("Export information in TIFF Format."));
      connect(&*menuActions[IDA_FILE_EXPORTIMAGE_TIFF], SIGNAL(triggered()), this, SLOT(eventExportAsTIFF()));

      menuActions.emplace(IDA_FILE_EXPORTIMAGE_PNG, std::make_unique<QAction>(QIcon(":/images/File_PNG_Image.png"), tr("as PNG"), this));
      menuActions[IDA_FILE_EXPORTIMAGE_PNG]->setStatusTip(tr("Export information in PNG Format."));
      connect(&*menuActions[IDA_FILE_EXPORTIMAGE_PNG], SIGNAL(triggered()), this, SLOT(eventExportAsPNG()));

      menuActions.emplace(IDA_FILE_EXPORT_CSV, std::make_unique<QAction>(QIcon(":/images/csv_text.png"), tr("as CSV"), this));
      menuActions[IDA_FILE_EXPORT_CSV]->setStatusTip(tr("Export information in spreadsheet compatible .csv format."));
      connect(&*menuActions[IDA_FILE_EXPORT_CSV], SIGNAL(triggered()), this, SLOT(eventExportAsCSV()));

      menuActions.emplace(IDA_FILE_BATCHCONVERT, std::make_unique<QAction>(QIcon(":/images/construction.png"), tr("Batch Convert"), this));
      menuActions[IDA_FILE_BATCHCONVERT]->setStatusTip(tr("Convert a group of files to FITS."));
      connect(&*menuActions[IDA_FILE_BATCHCONVERT], SIGNAL(triggered()), this, SLOT(eventFileBatchConvert()));

      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        menuActions.emplace(IDA_FILE_IMPORTIMAGES, std::make_unique<QAction>(tr("Upload Images"), this));
        menuActions[IDA_FILE_IMPORTIMAGES]->setStatusTip(tr("Upload a group of files to the ARID database."));
        connect(&*menuActions[IDA_FILE_IMPORTIMAGES], SIGNAL(triggered()), this, SLOT(eventFileUploadImages()));
      };

      menuActions.emplace(IDA_FILE_PRINT, std::make_unique<QAction>(QIcon(":/images/printer.png"), tr("Print"), this));
      menuActions[IDA_FILE_PRINT]->setStatusTip(tr("Print the selected window."));
      connect(&*menuActions[IDA_FILE_PRINT], SIGNAL(triggered()), this, SLOT(eventPrint()));

      menuActions.emplace(IDA_FILE_PRINTPREVIEW, std::make_unique<QAction>(QIcon(":/images/printer_preview.png"), tr("Print Preview"), this));
      menuActions[IDA_FILE_PRINTPREVIEW]->setStatusTip(tr("Preview the print"));
      connect(&*menuActions[IDA_FILE_PRINTPREVIEW], SIGNAL(triggered()), this, SLOT(eventPrintPreview()));

      menuActions.emplace(IDA_FILE_PRINTSETUP, std::make_unique<QAction>(QIcon(":/images/printer_setup.png"), tr("Print setup"), this));
      menuActions[IDA_FILE_PRINTSETUP]->setStatusTip(tr("Set up the print."));
      connect(&*menuActions[IDA_FILE_PRINTSETUP], SIGNAL(triggered()), this, SLOT(eventPrintSetup()));

      menuActions.emplace(IDA_FILE_EXIT,  std::make_unique<QAction>(QIcon(":/images/cross.png"), tr("E&xit"), this));
      menuActions[IDA_FILE_EXIT]->setShortcuts(QKeySequence::Quit);
      menuActions[IDA_FILE_EXIT]->setStatusTip(tr("Exit the application"));
      connect(&*menuActions[IDA_FILE_EXIT], SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

        // Edit Menu Actions

      menuActions.emplace(IDA_EDIT_COPY, std::make_unique<QAction>(QIcon(":/images/clipboard_copy.png"), tr("&Copy"), this));
      menuActions[IDA_EDIT_COPY]->setShortcuts(QKeySequence::Copy);
      menuActions[IDA_EDIT_COPY]->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
      connect(&*menuActions[IDA_EDIT_COPY], SIGNAL(triggered()), this, SLOT(eventEditcopy()));

      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        menuActions.emplace(IDA_EDIT_IMAGEMETADATA, std::make_unique<QAction>(QIcon(":/icons/tags/three_tags.png"), tr("Image MetaData"), this));
        menuActions[IDA_EDIT_IMAGEMETADATA]->setStatusTip(tr("Edit the image metadata."));
        connect(&*menuActions[IDA_EDIT_IMAGEMETADATA], SIGNAL(triggered()), this, SLOT(eventEditImageMetadata()));
      };

      menuActions.emplace(IDA_EDIT_ANNOTATEIMAGE, std::make_unique<QAction>(QIcon(":/images/construction.png"), tr("Annotate Image"), this));
      menuActions[IDA_EDIT_ANNOTATEIMAGE]->setStatusTip(tr("Annotate elements of the image."));
      connect(&*menuActions[IDA_EDIT_ANNOTATEIMAGE], SIGNAL(triggered()), this, SLOT(eventUnimplemented()));

      menuActions.emplace(IDA_EDIT_OPTIONS, std::make_unique<QAction>(QIcon(":/images/Gear.png"), tr("Options"), this));
      menuActions[IDA_EDIT_OPTIONS]->setStatusTip(tr("Options for the program."));
      connect(&*menuActions[IDA_EDIT_OPTIONS], SIGNAL(triggered()), this, SLOT(eventEditOptions()));

      menuActions.emplace(IDA_EDIT_RESOURCES, std::make_unique<QAction>(tr("Add/Edit Resources"), this));
      menuActions[IDA_EDIT_RESOURCES]->setStatusTip(tr("Add/Edit Resources (Sites, Telescopes, Observers, Imagers"));
      menuActions[IDA_EDIT_RESOURCES]->setToolTip(tr("Add/Edit Resources (Sites, Telescopes, Observers, Imagers"));
      connect(&*menuActions[IDA_EDIT_RESOURCES], SIGNAL(triggered()), this, SLOT(eventEditResources()));

        // Image Calibration Actions

      menuActions.emplace(IDA_CALIBRATE_SINGLEIMAGE, std::make_unique<QAction>(tr("Calibrate Image"), this));
      menuActions[IDA_CALIBRATE_SINGLEIMAGE]->setStatusTip(tr("Calibrate a Single Image"));
      menuActions[IDA_CALIBRATE_SINGLEIMAGE]->setToolTip(tr("Calibrate a single image."));
      connect(&*menuActions[IDA_CALIBRATE_SINGLEIMAGE], SIGNAL(triggered()), this, SLOT(eventImagingSingleCalibration()));

      menuActions.emplace(IDA_CALIBRATE_MULTIIMAGES, std::make_unique<QAction>(tr("Calibrate Multiple Images"), this));
      menuActions[IDA_CALIBRATE_MULTIIMAGES]->setStatusTip(tr("Calibrate Multiple Images"));
      connect(&*menuActions[IDA_CALIBRATE_MULTIIMAGES], SIGNAL(triggered()), this, SLOT(eventImagingMultipleCalibration()));

      menuActions.emplace(IDA_CALIBRATE_CREATEMASTERDARK, std::make_unique<QAction>(tr("Create Master Dark"), this));
      menuActions[IDA_CALIBRATE_CREATEMASTERDARK]->setStatusTip(tr("Create a master dark FITS file"));
      connect(&*menuActions[IDA_CALIBRATE_CREATEMASTERDARK], SIGNAL(triggered()), this, SLOT(eventCreateMasterDarkFrame()));

      menuActions.emplace(IDA_CALIBRATE_CREATEMASTERFLAT, std::make_unique<QAction>(tr("Create Master Flat"), this));
      menuActions[IDA_CALIBRATE_CREATEMASTERFLAT]->setStatusTip(tr("Create a master flat FITS file"));
      connect(&*menuActions[IDA_CALIBRATE_CREATEMASTERFLAT], SIGNAL(triggered()), this, SLOT(eventCreateMasterFlatFrame()));

      menuActions.emplace(IDA_CALIBRATE_CREATEMASTERBIAS, std::make_unique<QAction>(tr("Create Master Bias"), this));
      menuActions[IDA_CALIBRATE_CREATEMASTERBIAS]->setStatusTip(tr("Create a master bias FITS file"));
      connect(&*menuActions[IDA_CALIBRATE_CREATEMASTERBIAS], SIGNAL(triggered()), this, SLOT(eventCreateMasterBiasFrame()));

      menuActions.emplace(IDA_PROCESS_STACKIMAGES, std::make_unique<QAction>(tr("Stack Images"), this));
      menuActions[IDA_PROCESS_STACKIMAGES]->setStatusTip(tr("Stack a number of images into a single new image"));
      connect(&*menuActions[IDA_PROCESS_STACKIMAGES], SIGNAL(triggered()), this, SLOT(eventImagingStackImages()));

      menuActions.emplace(IDA_PROCESS_COMPAREIMAGES, std::make_unique<QAction>(tr("Compare Images"), this));
      menuActions[IDA_PROCESS_COMPAREIMAGES]->setStatusTip(tr("Align a number of images to allow blinking or similar..."));
      connect(&*menuActions[IDA_PROCESS_COMPAREIMAGES], SIGNAL(triggered()), this, SLOT(eventCompareImages()));

        // Analysis Actions

      menuActions.emplace(IDA_ANALYSIS_EXTRACTOBJECTS, std::make_unique<QAction>(tr("Extract Sources"), this));
      menuActions[IDA_ANALYSIS_EXTRACTOBJECTS]->setStatusTip(tr("Automatically identify sources in the image..."));
      connect(&*menuActions[IDA_ANALYSIS_EXTRACTOBJECTS], SIGNAL(triggered()), this, SLOT(eventAnalysisExtractObjects()));

      menuActions.emplace(IDA_ANALYSIS_LOADOBJECTS, std::make_unique<QAction>(tr("Load Objects"), this));
      menuActions[IDA_ANALYSIS_LOADOBJECTS]->setStatusTip(tr("Load the objects (stars) for the image."));
      connect(&*menuActions[IDA_ANALYSIS_LOADOBJECTS], SIGNAL(triggered()), this, SLOT(eventAnalysisLoadObjects()));

        // Astrometry Actions

      menuActions.emplace(IDA_ASTROMETRY_REFERENCEIMAGE, std::make_unique<QAction>(QIcon(":/images/astrometry.png"), tr("Reference Image"), this));
      menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]->setStatusTip(tr("Add references to the stars to the image."));
      menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]->setCheckable(true);
      connect(&*menuActions[IDA_ASTROMETRY_REFERENCEIMAGE], SIGNAL(triggered()), this, SLOT(eventReferenceImage()));

      menuActions.emplace(IDA_ASTROMETRY_LOADTARGETLIST, std::make_unique<QAction>(tr("Load Target List"), this));
      menuActions[IDA_ASTROMETRY_LOADTARGETLIST]->setStatusTip(tr("Load list of targets"));
      connect(&*menuActions[IDA_ASTROMETRY_LOADTARGETLIST], SIGNAL(triggered()), this, SLOT(eventAstrometryLoadTargets()));

        // Photometry Actions

      //actionPhotometry[2] = new QAction(tr("Determine Zero Point"), this);
      //actionPhotometry[2]->setStatusTip(tr("Determine the zero point for the night."));

      //actionPhotometry[3] = new QAction(tr("Determine Extinctions"), this);
      //actionPhotometry[3]->setStatusTip(tr("Determine extinctions"));

      //actionPhotometry[4] = new QAction(tr("Enter Observations"), this);
      //actionPhotometry[4]->setStatusTip(tr("Enter new observation information"));

      //actionPhotometry[5] = new QAction(tr("View/Edit Observations"), this);
      //actionPhotometry[5]->setStatusTip(tr("View or Edit observations"));

      //actionPhotometry[6] = new QAction(tr("Submit to AAVSO"), this);
      //actionPhotometry[6]->setStatusTip(tr("Submit observations to AAVSO."));

      menuActions.emplace(IDA_PHOTOMETRY_LIGHTCURVES, std::make_unique<QAction>(tr("Light Curves"), this));
      menuActions[IDA_PHOTOMETRY_LIGHTCURVES]->setStatusTip(tr("View Light Curves"));
      connect(&*menuActions[IDA_PHOTOMETRY_LIGHTCURVES], SIGNAL(triggered()), this, SLOT(eventPhotometryLightCurves()));

      menuActions.emplace(IDA_PHOTOMETRY_SINGLEIMAGE, std::make_unique<QAction>(QIcon(":/images/photometry.png"), tr("Single Image Photometry"), this));
      menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]->setStatusTip(tr("Do Photometry on a single image."));
      menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]->setCheckable(true);
      connect(&*menuActions[IDA_PHOTOMETRY_SINGLEIMAGE], SIGNAL(triggered()), this, SLOT(eventPhotometrySingleImage()));

      menuActions.emplace(IDA_PHOTOMETRY_LOADTARGETLIST, std::make_unique<QAction>(tr("Load Target List"), this));
      menuActions[IDA_PHOTOMETRY_LOADTARGETLIST]->setStatusTip(tr("Load list of targets (RA/Dec) to perform automated photometry"));
      connect(&*menuActions[IDA_PHOTOMETRY_LOADTARGETLIST], SIGNAL(triggered()), this, SLOT(eventPhotometryLoadTargets()));

        // Tools Menu

      //actionCalculate[2] = new QAction(tr("Magnitude"), this);
      //actionCalculate[2]->setStatusTip(tr("Magnitude Corrections"));

      //actionCalculate[3] = new QAction(tr("Darkness Hours"), this);
      //actionCalculate[3]->setStatusTip(tr("See darkness hours for a day/place"));

      //actionCalculate[4] = new QAction(tr("Precess Coordinates"), this);
      //actionCalculate[4]->setStatusTip(tr("Precess coordinates to new epoch"));

      //actionCalculate[5] = new QAction(tr("Field of View"), this);
      //actionCalculate[5]->setStatusTip(tr("Calculate field of view for telescope/eyepiece/instrument combination"));

        // View Menu actions

      menuActions.emplace(IDA_VIEW_IMAGECONTROL, std::make_unique<QAction>(QIcon(":/images/image_information.png"), tr("Image Information"), this));
      menuActions[IDA_VIEW_IMAGECONTROL]->setStatusTip(tr("Show image Information"));
      menuActions[IDA_VIEW_IMAGECONTROL]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_IMAGECONTROL], SIGNAL(triggered()), this, SLOT(eventImageControl()));

      menuActions.emplace(IDA_VIEW_HISTOGRAM, std::make_unique<QAction>(QIcon(":/images/Chart_Bar.png"), tr("Histogram"), this));
      menuActions[IDA_VIEW_HISTOGRAM]->setStatusTip(tr("Display the histogram."));
      menuActions[IDA_VIEW_HISTOGRAM]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_HISTOGRAM], SIGNAL(triggered()), this, SLOT(eventViewHistogram()));

      menuActions.emplace(IDA_VIEW_ASTROMETRY, std::make_unique<QAction>(QIcon(":/images/astrometry_show.png"), tr("Show Astrometry Indicators"), this));
      menuActions[IDA_VIEW_ASTROMETRY]->setStatusTip(tr("Show the astrometry indicators on the image."));
      menuActions[IDA_VIEW_ASTROMETRY]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_ASTROMETRY], SIGNAL(triggered()), this, SLOT(eventViewAstrometry()));

      menuActions.emplace(IDA_VIEW_PHOTOMETRY, std::make_unique<QAction>(QIcon(":/images/photometry_show.png"), tr("Show Photometry Indicators"), this));
      menuActions[IDA_VIEW_PHOTOMETRY]->setStatusTip(tr("Show the photometry indicators on the image."));
      menuActions[IDA_VIEW_PHOTOMETRY]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_PHOTOMETRY], SIGNAL(triggered()), this, SLOT(eventViewPhotometry()));

      menuActions.emplace(IDA_VIEW_ANNOTATIONS, std::make_unique<QAction>(QIcon(":/images/annotation_show.png"), tr("Show Annotations"), this));
      menuActions[IDA_VIEW_ANNOTATIONS]->setStatusTip(tr("Show annotations on the image."));
      menuActions[IDA_VIEW_ANNOTATIONS]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_ANNOTATIONS], SIGNAL(triggered()), this, SLOT(eventViewAnnotations()));

      menuActions.emplace(IDA_VIEW_ZOOMIN, std::make_unique<QAction>(QIcon(":/images/zoom_in.png"), tr("Zoom In"), this));
      menuActions[IDA_VIEW_ZOOMIN]->setStatusTip(tr("Zoom closer into the image."));
      connect(&*menuActions[IDA_VIEW_ZOOMIN], SIGNAL(triggered()), this, SLOT(eventZoomIn()));

      menuActions.emplace(IDA_VIEW_ZOOMOUT, std::make_unique<QAction>(QIcon(":/images/zoom_out.png"), tr("Zoom Out"), this));
      menuActions[IDA_VIEW_ZOOMOUT]->setStatusTip(tr("Zoom the image out."));
      connect(&*menuActions[IDA_VIEW_ZOOMOUT], SIGNAL(triggered()), this, SLOT(eventZoomOut()));

      menuActions.emplace(IDA_VIEW_ZOOMALL, std::make_unique<QAction>(QIcon(":/images/zoom_fit.png"), tr("Zoom All"), this));
      menuActions[IDA_VIEW_ZOOMALL]->setStatusTip(tr("Zoom to show the entire image."));
      connect(&*menuActions[IDA_VIEW_ZOOMALL], SIGNAL(triggered()), this, SLOT(eventZoomAll()));

      menuActions.emplace(IDA_VIEW_ZOOMSELECTION, std::make_unique<QAction>(QIcon(":/images/zoom_selection.png"), tr("Zoom Selection"), this));
      menuActions[IDA_VIEW_ZOOMSELECTION]->setStatusTip(tr("Zoom to show the selected area."));
      connect(&*menuActions[IDA_VIEW_ZOOMSELECTION], SIGNAL(triggered()), this, SLOT(eventZoomSelection()));

      menuActions.emplace(IDA_VIEW_ZOOM11, std::make_unique<QAction>(QIcon(":/images/zoom_actual.png"), tr("Zoom 1:1"), this));
      menuActions[IDA_VIEW_ZOOM11]->setStatusTip(tr("Zoom to actual size."));
      connect(&*menuActions[IDA_VIEW_ZOOM11], SIGNAL(triggered()), this, SLOT(eventZoom11()));

      menuActions.emplace(IDA_VIEW_CONSOLEWINDOW, std::make_unique<QAction>(QIcon(":/images/note.png"), tr("Message Window"), this));
      menuActions[IDA_VIEW_CONSOLEWINDOW]->setStatusTip(tr("Open the message window"));
      menuActions[IDA_VIEW_CONSOLEWINDOW]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_CONSOLEWINDOW], SIGNAL(triggered()), this, SLOT(eventMessageWidget()));

      menuActions.emplace(IDA_VIEW_MAGNIFY, std::make_unique<QAction>(QIcon(":/images/magnifier.png"), tr("Magnified window"), this));
      menuActions[IDA_VIEW_MAGNIFY]->setStatusTip(tr("View a magnified portion of the active image."));
      menuActions[IDA_VIEW_MAGNIFY]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_MAGNIFY], SIGNAL(triggered()), this, SLOT(eventViewMagnify()));

      menuActions.emplace(IDA_VIEW_NAVIGATOR, std::make_unique<QAction>(QIcon(":/images/compass.png"), tr("Navigator window"), this));
      menuActions[IDA_VIEW_NAVIGATOR]->setStatusTip(tr("See the current viewport on the image."));
      menuActions[IDA_VIEW_NAVIGATOR]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_NAVIGATOR], SIGNAL(triggered()), this, SLOT(eventViewNavigator()));

      menuActions.emplace(IDA_VIEW_DOCKWIDGET_WEATHERSCALE, std::make_unique<QAction>(tr("Weather Scale"), this));
      menuActions[IDA_VIEW_DOCKWIDGET_WEATHERSCALE]->setStatusTip(tr("Open the Weather Scaler"));
      menuActions[IDA_VIEW_DOCKWIDGET_WEATHERSCALE]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_DOCKWIDGET_WEATHERSCALE], SIGNAL(triggered()), this, SLOT(eventViewDockWidgetWeatherScale()));

      menuActions.emplace(IDA_VIEW_DOCKWIDGET_WEATHERPARAMETERS, std::make_unique<QAction>(tr("Weather Parameters"), this));
      menuActions[IDA_VIEW_DOCKWIDGET_WEATHERPARAMETERS]->setStatusTip(tr("Open the Weather Parameters"));
      menuActions[IDA_VIEW_DOCKWIDGET_WEATHERPARAMETERS]->setCheckable(true);
      connect(&*menuActions[IDA_VIEW_DOCKWIDGET_WEATHERPARAMETERS], SIGNAL(triggered()), this, SLOT(eventViewDockWidgetWeatherParameters()));

        // Transform Menu actions

      menuActions.emplace(IDA_TRANSFORM_FLIP, std::make_unique<QAction>(QIcon(":/images/shape_flip_horizontal.png"), tr("Mirror Horizontal"), this));
      menuActions[IDA_TRANSFORM_FLIP]->setStatusTip(tr("Flip the image around the horizontal axis."));
      connect(&*menuActions[IDA_TRANSFORM_FLIP], SIGNAL(triggered()), this, SLOT(eventFlipImage()));

      menuActions.emplace(IDA_TRANSFORM_FLOP, std::make_unique<QAction>(QIcon(":/images/shape_flip_vertical.png"), tr("Mirror Vertical"), this));
      menuActions[IDA_TRANSFORM_FLOP]->setStatusTip(tr("Flop the image around the vertical axis."));
      connect(&*menuActions[IDA_TRANSFORM_FLOP], SIGNAL(triggered()), this, SLOT(eventFlopImage()));

      menuActions.emplace(IDA_TRANSFORM_RESAMPLE, std::make_unique<QAction>(QIcon(":/images/transform_scale.png"), tr("Resample"), this));
      menuActions[IDA_TRANSFORM_RESAMPLE]->setStatusTip(tr("Resample the image"));
      connect(&*menuActions[IDA_TRANSFORM_RESAMPLE], SIGNAL(triggered()), this, SLOT(eventResampleImage()));

      menuActions.emplace(IDA_TRANSFORM_BINPIXELS, std::make_unique<QAction>(QIcon(":/images/grid.png"), tr("Bin Pixels"), this));
      menuActions[IDA_TRANSFORM_BINPIXELS]->setStatusTip(tr("Bin the image"));
      connect(&*menuActions[IDA_TRANSFORM_BINPIXELS], SIGNAL(triggered()), this, SLOT(eventBinPixels()));

      menuActions.emplace(IDA_TRANSFORM_FLOAT, std::make_unique<QAction>(QIcon(":/images/canvas_size.png"), tr("Float"), this));
      menuActions[IDA_TRANSFORM_FLOAT]->setStatusTip(tr("Float the image larger"));
      connect(&*menuActions[IDA_TRANSFORM_FLOAT], SIGNAL(triggered()), this, SLOT(eventFloatImage()));

      menuActions.emplace(IDA_TRANSFORM_CROP, std::make_unique<QAction>(QIcon(":/images/transform_crop.png"), tr("Crop"), this));
      menuActions[IDA_TRANSFORM_CROP]->setStatusTip(tr("Crop the image smaller"));
      connect(&*menuActions[IDA_TRANSFORM_CROP], SIGNAL(triggered()), this, SLOT(eventImageCrop()));

      menuActions.emplace(IDA_TRANSFORM_ROTATE, std::make_unique<QAction>(QIcon(":/images/transform_rotate.png"), tr("Rotate"), this));
      menuActions[IDA_TRANSFORM_ROTATE]->setStatusTip(tr("Rotate the image"));
      connect(&*menuActions[IDA_TRANSFORM_ROTATE], SIGNAL(triggered()), this, SLOT(eventTransformRotate()));

        // IA Colour Menu

      menuActions.emplace(IDA_IA_COLOUR_SPLIT_RGB, std::make_unique<QAction>(QIcon(":/images/convert_gray_to_color.png"), tr("Convert -> RGB"), this));
      menuActions[IDA_IA_COLOUR_SPLIT_RGB]->setStatusTip(tr("Convert the image into R, B, G components."));
      connect(&*menuActions[IDA_IA_COLOUR_SPLIT_RGB], SIGNAL(triggered()), this, SLOT(eventConvertRGB()));

      menuActions.emplace(IDA_IA_COLOUR_SPLIT_LRGB, std::make_unique<QAction>(tr("Convert -> LRGB"), this));
      menuActions[IDA_IA_COLOUR_SPLIT_LRGB]->setStatusTip(tr("Convert the image into L, R, B, G components."));
      connect(&*menuActions[IDA_IA_COLOUR_SPLIT_LRGB], SIGNAL(triggered()), this, SLOT(eventConvertLRGB()));

      menuActions.emplace(IDA_IA_COLOUR_SPLIT_GRAYSCALE, std::make_unique<QAction>(QIcon(":/images/convert_color_to_gray.png"), tr("Convert -> Grayscale"), this));
      menuActions[IDA_IA_COLOUR_SPLIT_GRAYSCALE]->setStatusTip(tr("Create a new grayscale image from this image."));
      connect(&*menuActions[IDA_IA_COLOUR_SPLIT_LRGB], SIGNAL(triggered()), this, SLOT(eventConvertGrayscale()));

        // Utilities Menu

      menuActions.emplace(IDA_UTILITIES_CALCULATE_GREGORIAN2JD, std::make_unique<QAction>(tr("Gregorian->JD"), this));
      menuActions[IDA_UTILITIES_CALCULATE_GREGORIAN2JD]->setStatusTip(tr("Convert Gregorian date to Julian Day"));
      connect(&*menuActions[IDA_UTILITIES_CALCULATE_GREGORIAN2JD], SIGNAL(triggered()), this, SLOT(eventCalculateGreg2JD()));

      menuActions.emplace(IDA_UTILITIES_CALCULATE_JD2GREGORIAN, std::make_unique<QAction>(tr("JD->Gregorian"), this));
      menuActions[IDA_UTILITIES_CALCULATE_JD2GREGORIAN]->setStatusTip(tr("Convert Julian Day to Gregorian date"));
      connect(&*menuActions[IDA_UTILITIES_CALCULATE_JD2GREGORIAN], SIGNAL(triggered()), this, SLOT(eventCalculateJD2Gregorian()));

      menuActions.emplace(IDA_UTILITIES_OBJECTINFORMATION, std::make_unique<QAction>(tr("Object Information"), this));
      menuActions[IDA_UTILITIES_OBJECTINFORMATION]->setStatusTip(tr("View information on Objects"));
      connect(&*menuActions[IDA_UTILITIES_OBJECTINFORMATION], SIGNAL(triggered()), this, SLOT(menuViewObjectInfo()));

      menuActions.emplace(IDA_UTILITIES_WEATHER_HISTORY, std::make_unique<QAction>(tr("Weather History"), this));
      menuActions[IDA_UTILITIES_WEATHER_HISTORY]->setStatusTip((tr("View weather history from the database.")));
      connect(&*menuActions[IDA_UTILITIES_WEATHER_HISTORY], SIGNAL(triggered()), this, SLOT(eventWeatherHistory()));

      menuActions.emplace(IDA_UTILITIES_PLANNINGWINDOW, std::make_unique<QAction>(tr("Planning Window"), this));
      menuActions[IDA_UTILITIES_PLANNINGWINDOW]->setStatusTip((tr("Open a Window to allow observation planning.")));
      connect(&*menuActions[IDA_UTILITIES_PLANNINGWINDOW], SIGNAL(triggered()), this, SLOT(eventUtilityPlanning()));

        // Window Menu Actions

      menuActions.emplace(IDA_WINDOW_CLOSE, std::make_unique<QAction>(tr("Cl&ose"), this));
      menuActions[IDA_WINDOW_CLOSE]->setStatusTip(tr("Close the active window"));
      connect(&*menuActions[IDA_WINDOW_CLOSE], SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

      menuActions.emplace(IDA_WINDOW_CLOSEALL, std::make_unique<QAction>(tr("Close &All"), this));
      menuActions[IDA_WINDOW_CLOSEALL]->setStatusTip(tr("Close all the windows"));
      connect(&*menuActions[IDA_WINDOW_CLOSEALL], SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

      menuActions.emplace(IDA_WINDOW_TILE, std::make_unique<QAction>(QIcon(":/images/application_tile.png"), tr("&Tile"), this));
      menuActions[IDA_WINDOW_TILE]->setStatusTip(tr("Tile the windows"));
      connect(&*menuActions[IDA_WINDOW_TILE], SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

      menuActions.emplace(IDA_WINDOW_CASCADE, std::make_unique<QAction>(QIcon(":/images/application_cascade.png"), tr("&Cascade"), this));
      menuActions[IDA_WINDOW_CASCADE]->setStatusTip(tr("Cascade the windows"));
      connect(&*menuActions[IDA_WINDOW_CASCADE], SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

      menuActions.emplace(IDA_WINDOW_NEXT, std::make_unique<QAction>(QIcon(":/images/arrow_right.png"), tr("Ne&xt"), this));
      menuActions[IDA_WINDOW_NEXT]->setShortcuts(QKeySequence::NextChild);
      menuActions[IDA_WINDOW_NEXT]->setStatusTip(tr("Move the focus to the next window"));
      connect(&*menuActions[IDA_WINDOW_NEXT], SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

      menuActions.emplace(IDA_WINDOW_PREVIOUS, std::make_unique<QAction>(QIcon(":/images/arrow_left.png"), tr("Pre&vious"), this));
      menuActions[IDA_WINDOW_PREVIOUS]->setShortcuts(QKeySequence::PreviousChild);
      menuActions[IDA_WINDOW_PREVIOUS]->setStatusTip(tr("Move the focus to the previous window"));
      connect(&*menuActions[IDA_WINDOW_PREVIOUS], SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

        // Help Actions

      menuActions.emplace(IDA_HELP_CONTENTS, std::make_unique<QAction>(QIcon(":/images/construction.png"), tr("Contents"), this));
      menuActions[IDA_HELP_CONTENTS]->setStatusTip(tr("Bring up external help file"));

      menuActions.emplace(IDA_HELP_ABOUTATID, std::make_unique<QAction>(tr("About ATID"), this));
      menuActions[IDA_HELP_ABOUTATID]->setStatusTip(tr("View details about Astronomical Target Information Database."));
      connect(&*menuActions[IDA_HELP_ABOUTATID], SIGNAL(triggered()), this, SLOT(HelpATID()));

      menuActions.emplace(IDA_HELP_ABOUT_astroManager, std::make_unique<QAction>(QIcon(":/images/info_rhombus.png"),
                                                                           tr("&About AstroManager"), this));
      menuActions[IDA_HELP_ABOUT_astroManager]->setStatusTip(tr("Show the application's About box"));
      connect(&*menuActions[IDA_HELP_ABOUT_astroManager], SIGNAL(triggered()), this, SLOT(HelpAbout()));

      menuActions.emplace(IDA_HELP_ABOUTQT, std::make_unique<QAction>(tr("About &Qt"), this));
      menuActions[IDA_HELP_ABOUTQT]->setStatusTip(tr("Show the Qt library's About box"));
      connect(&*menuActions[IDA_HELP_ABOUTQT], SIGNAL(triggered()), qApp, SLOT(aboutQt()));

      setBaseActionStates();    // Ensure all the actions are disabled that need to be disabled.
    }

    /// @brief Creates all the dock widget windows that are required to be available all the time. They are initially created hidden
    /// and only shown as required.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2017-07-03/GGB - Added the weather scale dock widget.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2013-08-19/GGB - Removed the instrument dock widget.
    /// @version 2013-05-24/GGB - Added the navigator dock widget.
    /// @version 2013-05-19/GGB - Added the magnify dock widegt.
    /// @version 2013-02-02/GGB - Added the histogram dock widget.
    /// @version 2012-07-27/GGB - Added the Message control dock widget
    /// @version 2011-05-29/GGB - Function created.

    void CFrameWindow::createDockWidgets()
    {
        // Create the image control dock widget.

      dockWidgets.emplace(IDDW_IMAGECONTROL, std::make_unique<dockwidgets::CImageControlWidget>(this, &*menuActions[IDA_VIEW_IMAGECONTROL]));
      addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_IMAGECONTROL]);
      dockWidgets[IDDW_IMAGECONTROL]->hide();

      dockWidgets.emplace(IDDW_ASTROMETRYCONTROL, std::make_unique<dockwidgets::CAstrometryDockWidget>(this, &*menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]));
      addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_ASTROMETRYCONTROL]);
      dockWidgets[IDDW_ASTROMETRYCONTROL]->hide();

      dockWidgets.emplace(IDDW_PHOTOMETRYCONTROL, std::make_unique<dockwidgets::CPhotometryDockWidget>(this, &*menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]));
      addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_PHOTOMETRYCONTROL]);
      dockWidgets[IDDW_PHOTOMETRYCONTROL]->hide();

      dockWidgets.emplace(IDDW_MESSAGECONTROL, std::make_unique<dockwidgets::CDockWidgetMessage>(this, &*menuActions[IDA_VIEW_CONSOLEWINDOW]));
      addDockWidget(Qt::BottomDockWidgetArea, &*dockWidgets[IDDW_MESSAGECONTROL]);

      dockWidgets.emplace(IDDW_VIEW_HISTOGRAM, std::make_unique<dockwidgets::CHistogram>(this, &*menuActions[IDA_VIEW_HISTOGRAM]));
      addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_VIEW_HISTOGRAM]);
      dockWidgets[IDDW_VIEW_HISTOGRAM]->hide();

      dockWidgets.emplace(IDDW_VIEW_MAGNIFY, std::make_unique<dockwidgets::CDockWidgetMagnify>(this, &*menuActions[IDA_VIEW_MAGNIFY]));
      addDockWidget(Qt::BottomDockWidgetArea, &*dockWidgets[IDDW_VIEW_MAGNIFY]);
      dockWidgets[IDDW_VIEW_MAGNIFY]->hide();

      dockWidgets.emplace(IDDW_VIEW_NAVIGATOR, std::make_unique<dockwidgets::CDockWidgetNavigator>(this, &*menuActions[IDA_VIEW_NAVIGATOR]));
      addDockWidget(Qt::BottomDockWidgetArea, &*dockWidgets[IDDW_VIEW_NAVIGATOR]);

      dockWidgets.emplace(IDDW_WEATHER_SCALE, std::make_unique<dockwidgets::CDockWidgetWeatherScale>(this, &*menuActions[IDA_VIEW_DOCKWIDGET_WEATHERSCALE]));
      addDockWidget(Qt::LeftDockWidgetArea, &*dockWidgets[IDDW_WEATHER_SCALE]);
      dockWidgets[IDDW_WEATHER_SCALE]->setFeatures(QDockWidget::DockWidgetVerticalTitleBar | dockWidgets[IDDW_WEATHER_SCALE]->features());
      dockWidgets[IDDW_WEATHER_SCALE]->hide();

      dockWidgets.emplace(IDDW_WEATHER_PARAMETERS, std::make_unique<dockwidgets::CDockWidgetWeatherParameters>(this, &*menuActions[IDA_VIEW_DOCKWIDGET_WEATHERPARAMETERS]));
      addDockWidget(Qt::LeftDockWidgetArea, &*dockWidgets[IDDW_WEATHER_PARAMETERS]);
      dockWidgets[IDDW_WEATHER_PARAMETERS]->setFeatures(QDockWidget::DockWidgetVerticalTitleBar | dockWidgets[IDDW_WEATHER_PARAMETERS]->features());
      dockWidgets[IDDW_WEATHER_PARAMETERS]->show();

    }

    /// @brief Creates the recently used file list.
    /// @version 2013-07-28/GGB - Changed depth to an unsigned int.
    /// @version 2013-01-22/GGB - Function created.

    void CFrameWindow::createRecentFileMenu()
    {
      unsigned int depth = settings::astroManagerSettings->value(settings::FILE_LASTOPENEDDEPTH, QVariant(5)).toUInt();
      bool valChanged = false;

      if (depth < 5)
      {
        depth = 5;
        valChanged = true;
      }
      else if (depth > MAXRECENTDEPTH)
      {
       depth = MAXRECENTDEPTH;
       valChanged = true;
      };

      if (valChanged)
      {
        settings::astroManagerSettings->setValue(settings::FILE_LASTOPENEDDEPTH, QVariant(depth));
      }

      updateRecentFileMenu(depth);
    }

    /// @brief Create all the relevant sub menus required by the application.
    /// @note: 1. This must be called before the function to create the menuBars.
    /// @throws std::bad_alloc
    /// @version 2017-07-28/GGB - Added File | Search menu action.
    /// @version 2017-07-13/GGB - Function created.

    void CFrameWindow::createSubMenus()
    {
      QMenu *menuTempS;

        // File sub-menu

      subMenus.emplace(IDSM_NULL_FILE, std::make_unique<QMenu>(tr("&File")));
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_OPEN]);
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_INSPECT]);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_SEARCH]);
      };
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_SAVE]);
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_SAVEAS]);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_SAVE_DATABASE]);
      }
      subMenus[IDSM_NULL_FILE]->addSeparator();
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_BATCHCONVERT]);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_IMPORTIMAGES]);
      };
      subMenus[IDSM_NULL_FILE]->addSeparator();
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_PRINTSETUP]);
      subMenus[IDSM_NULL_FILE]->addSeparator();
      subMenus[IDSM_NULL_FILE]->addAction(&*menuActions[IDA_FILE_EXIT]);

        // File sub-menu

      subMenus.emplace(IDSM_IMAGE_FILE, std::make_unique<QMenu>(tr("&File")));
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_OPEN]);
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_INSPECT]);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_SEARCH]);
      };
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_SAVE]);
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_SAVEAS]);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_SAVE_DATABASE]);
      }
      subMenus[IDSM_IMAGE_FILE]->addSeparator();
      menuTempS = subMenus[IDSM_IMAGE_FILE]->addMenu(tr("&Export ..."));
      menuTempS->addAction(&*menuActions[IDA_FILE_EXPORTIMAGE_DNG]);
      menuTempS->addAction(&*menuActions[IDA_FILE_EXPORTIMAGE_JPEG]);
      menuTempS->addAction(&*menuActions[IDA_FILE_EXPORTIMAGE_TIFF]);
      menuTempS->addAction(&*menuActions[IDA_FILE_EXPORTIMAGE_PNG]);
      menuTempS->addAction(&*menuActions[IDA_FILE_EXPORT_CSV]);
      subMenus[IDSM_IMAGE_FILE]->addSeparator();
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_BATCHCONVERT]);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_IMPORTIMAGES]);
      };
      subMenus[IDSM_IMAGE_FILE]->addSeparator();
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_PRINT]);
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_PRINTPREVIEW]);
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_PRINTSETUP]);
      subMenus[IDSM_IMAGE_FILE]->addSeparator();
      subMenus[IDSM_IMAGE_FILE]->addAction(&*menuActions[IDA_FILE_EXIT]);

      createRecentFileMenu();
      populateRecentFileMenu();

      // Edit sub-menu

      subMenus.emplace(IDSM_NULL_EDIT, std::make_unique<QMenu>(tr("&Edit")));
      subMenus[IDSM_NULL_EDIT]->addAction(&*menuActions[IDA_EDIT_OPTIONS]);
      subMenus[IDSM_NULL_EDIT]->addAction(&*menuActions[IDA_EDIT_RESOURCES]);

      subMenus.emplace(IDSM_IMAGE_EDIT, std::make_unique<QMenu>(tr("&Edit")));
      subMenus[IDSM_IMAGE_EDIT]->addAction(&*menuActions[IDA_EDIT_COPY]);
      subMenus[IDSM_IMAGE_EDIT]->addSeparator();
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        subMenus[IDSM_IMAGE_EDIT]->addAction(&*menuActions[IDA_EDIT_IMAGEMETADATA]);
      };
      subMenus[IDSM_IMAGE_EDIT]->addAction(&*menuActions[IDA_EDIT_ANNOTATEIMAGE]);
      subMenus[IDSM_IMAGE_EDIT]->addSeparator();
      subMenus[IDSM_IMAGE_EDIT]->addAction(&*menuActions[IDA_EDIT_OPTIONS]);
      subMenus[IDSM_IMAGE_EDIT]->addAction(&*menuActions[IDA_EDIT_RESOURCES]);

      // View sub-menu

      subMenus.emplace(IDSM_IMAGE_VIEW, std::make_unique<QMenu>(tr("&View")));
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_IMAGECONTROL]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_HISTOGRAM]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_MAGNIFY]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_NAVIGATOR]);
      subMenus[IDSM_IMAGE_VIEW]->addSeparator();
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOM11]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMIN]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMOUT]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMALL]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMSELECTION]);
      subMenus[IDSM_IMAGE_VIEW]->addSeparator();
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ASTROMETRY]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_PHOTOMETRY]);
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ANNOTATIONS]);
      subMenus[IDSM_IMAGE_VIEW]->addSeparator();
      subMenus[IDSM_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_CONSOLEWINDOW]);

        // Calibrate sub-menu

      subMenus.emplace(IDSM_IMAGE_CALIBRATE, std::make_unique<QMenu>(tr("&Preparation")));
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_CALIBRATE_SINGLEIMAGE]);
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_CALIBRATE_MULTIIMAGES]);
      subMenus[IDSM_IMAGE_CALIBRATE]->addSeparator();
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_CALIBRATE_CREATEMASTERDARK]);
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_CALIBRATE_CREATEMASTERFLAT]);
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_CALIBRATE_CREATEMASTERBIAS]);
      subMenus[IDSM_IMAGE_CALIBRATE]->addSeparator();
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_PROCESS_COMPAREIMAGES]);
      subMenus[IDSM_IMAGE_CALIBRATE]->addAction(&*menuActions[IDA_PROCESS_STACKIMAGES]);

        // Analyse sub-menu

      subMenus.emplace(IDSM_IMAGE_ANALYSE, std::make_unique<QMenu>(tr("&Analysis")));
      menuTempS = subMenus[IDSM_IMAGE_ANALYSE]->addMenu(tr("Objects"));
      menuTempS->addAction(&*menuActions[IDA_ANALYSIS_EXTRACTOBJECTS]);
      menuTempS->addAction(&*menuActions[IDA_ANALYSIS_LOADOBJECTS]);
      menuTempS = subMenus[IDSM_IMAGE_ANALYSE]->addMenu(tr("Astrometry"));
      menuTempS->addAction(&*menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]);
      menuTempS->addAction(&*menuActions[IDA_ASTROMETRY_LOADTARGETLIST]);
      menuTempS = subMenus[IDSM_IMAGE_ANALYSE]->addMenu(tr("&Photometry"));
      menuTempS->addAction(&*menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]);
      menuTempS->addAction(&*menuActions[IDA_PHOTOMETRY_LOADTARGETLIST]);
      menuTempS = subMenus[IDSM_IMAGE_ANALYSE]->addMenu(tr("&Spectroscopy"));
      menuTempS->setEnabled(false);

        // Transform sub-menu

      subMenus.emplace(IDSM_IMAGE_TRANSFORM, std::make_unique<QMenu>(tr("Transform")));
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_FLIP]);
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_FLOP]);
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_RESAMPLE]);
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_BINPIXELS]);
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_FLOAT]);
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_CROP]);
      subMenus[IDSM_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_ROTATE]);

        // Enhance sub-menu

      subMenus.emplace(IDSM_IMAGE_ENHANCE, std::make_unique<QMenu>(tr("Enhance")));


        // Colour sub-menu

      subMenus.emplace(IDSM_IMAGE_COLOUR, std::make_unique<QMenu>(tr("Colour")));
      subMenus[IDSM_IMAGE_COLOUR]->addAction(&*menuActions[IDA_IA_COLOUR_SPLIT_RGB]);
      subMenus[IDSM_IMAGE_COLOUR]->addAction(&*menuActions[IDA_IA_COLOUR_SPLIT_LRGB]);
      subMenus[IDSM_IMAGE_COLOUR]->addAction(&*menuActions[IDA_IA_COLOUR_SPLIT_GRAYSCALE]);


      // Utilties sub-menu

      subMenus.emplace(IDSM_UTILITIES, std::make_unique<QMenu>(tr("&Utilities")));
      menuTempS = subMenus[IDSM_UTILITIES]->addMenu(tr("Calculators"));
      menuTempS->addAction(&*menuActions[IDA_UTILITIES_CALCULATE_GREGORIAN2JD]);
      menuTempS->addAction(&*menuActions[IDA_UTILITIES_CALCULATE_JD2GREGORIAN]);
      subMenus[IDSM_UTILITIES]->addAction(&*menuActions[IDA_UTILITIES_OBJECTINFORMATION]);

      // Only add the weather functions if the database is enabled.

      if (database::databaseWeather->enabled())
      {
        subMenus[IDSM_UTILITIES]->addSeparator();
        menuTempS = subMenus[IDSM_UTILITIES]->addMenu(tr("&Weather Data"));
        menuTempS->addAction(&*menuActions[IDA_UTILITIES_WEATHER_HISTORY]);
      };
      subMenus[IDSM_UTILITIES]->addSeparator();
      subMenus[IDSM_UTILITIES]->addAction(&*menuActions[IDA_UTILITIES_PLANNINGWINDOW]);

      // Window sub-menu

      subMenus.emplace(IDSM_WINDOW, std::make_unique<QMenu>(tr("&Window")));

      updateWindowMenu();
      connect(&*subMenus[IDSM_WINDOW], SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
      subMenus[IDSM_WINDOW]->addSeparator();

        // Help sub-menu

      subMenus.emplace(IDSM_HELP, std::make_unique<QMenu>(tr("&Help")));
      subMenus[IDSM_HELP]->addAction(&*menuActions[IDA_HELP_CONTENTS]);
      subMenus[IDSM_HELP]->addSeparator();
      subMenus[IDSM_HELP]->addAction(&*menuActions[IDA_HELP_ABOUTQT]);
      subMenus[IDSM_HELP]->addSeparator();
      subMenus[IDSM_HELP]->addAction(&*menuActions[IDA_HELP_ABOUTATID]);
      subMenus[IDSM_HELP]->addAction(&*menuActions[IDA_HELP_ABOUT_astroManager]);

    }

    /// @brief Create the application toolbars.
    /// @throws None.
    /// @version 2017-07-03/GGB - Changed the toolbar storage to std::map from individual pointers.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr (Bug #85)
    /// @version 2013-08-19/GGB - Removed the image Information option.
    /// @version 2013-08-18/GGB - Added objectName for all toolbars.
    /// @version 2013-06-02/GGB - Added the viewToolBar & transform toolbar.
    /// @version 2011-03-06/GGB - Function created.

    void CFrameWindow::createToolBars()
    {   
      int toolBarHeight = settings::astroManagerSettings->value(settings::TOOLBAR_HEIGHT, QVariant(16)).toInt();

      toolBars.emplace(IDTB_FILE, std::unique_ptr<QToolBar>(addToolBar(tr("File"))));
      toolBars[IDTB_FILE]->setObjectName(TB_FILE);
      toolBars[IDTB_FILE]->setIconSize(QSize(toolBarHeight, toolBarHeight));
      toolBars[IDTB_FILE]->setMovable(true);
      toolBars[IDTB_FILE]->setAllowedAreas(Qt::AllToolBarAreas);
      toolBars[IDTB_FILE]->setFloatable(false);
      toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_OPEN));
      toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_INSPECT));
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_SEARCH));
      }
      toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_SAVE));
      toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_SAVEAS));
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        toolBars[IDTB_FILE]->addAction(&*menuActions[IDA_FILE_SAVE_DATABASE]);
      }
      toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_PRINT));
      toolBars[IDTB_FILE]->addAction(getAction(IDA_FILE_PRINTPREVIEW));

      toolBars.emplace(IDTB_EDIT, std::unique_ptr<QToolBar>(addToolBar(tr("Edit"))));
      toolBars[IDTB_EDIT]->setObjectName(TB_EDIT);
      toolBars[IDTB_EDIT]->setFloatable(false);
      toolBars[IDTB_EDIT]->setIconSize(QSize(toolBarHeight, toolBarHeight));
      toolBars[IDTB_EDIT]->addAction(&*menuActions[IDA_EDIT_COPY]);

      toolBars.emplace(IDTB_IMAGE_VIEW, std::unique_ptr<QToolBar>(addToolBar(tr("View"))));
      toolBars[IDTB_IMAGE_VIEW]->setObjectName(TB_VIEW);
      toolBars[IDTB_IMAGE_VIEW]->setFloatable(false);
      toolBars[IDTB_IMAGE_VIEW]->setIconSize(QSize(toolBarHeight, toolBarHeight));
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_IMAGECONTROL]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_HISTOGRAM]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_MAGNIFY]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_NAVIGATOR]);
      toolBars[IDTB_IMAGE_VIEW]->addSeparator();
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOM11]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMIN]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMOUT]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMALL]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ZOOMSELECTION]);
      toolBars[IDTB_IMAGE_VIEW]->addSeparator();
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ASTROMETRY]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_PHOTOMETRY]);
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_ANNOTATIONS]);
      toolBars[IDTB_IMAGE_VIEW]->addSeparator();
      toolBars[IDTB_IMAGE_VIEW]->addAction(&*menuActions[IDA_VIEW_CONSOLEWINDOW]);

      toolBars.emplace(IDTB_IMAGE_TRANSFORM, std::unique_ptr<QToolBar>(addToolBar(tr("Transform"))));
      toolBars[IDTB_IMAGE_TRANSFORM]->setObjectName(TB_TRANSFORM);
      toolBars[IDTB_IMAGE_TRANSFORM]->setFloatable(false);
      toolBars[IDTB_IMAGE_TRANSFORM]->setIconSize(QSize(toolBarHeight, toolBarHeight));
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_FLIP]);
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_FLOP]);
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_RESAMPLE]);
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_BINPIXELS]);
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_FLOAT]);
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_CROP]);
      toolBars[IDTB_IMAGE_TRANSFORM]->addAction(&*menuActions[IDA_TRANSFORM_ROTATE]);
    }

    /// @brief Processes the close event and determines if the window should be closed or not.
    /// @param[in] event - The close event.
    /// @throws None.
    /// @version 2013-03-02/GGB - Modified to allow child windows to prevent close.
    /// @version 2010-05-27/GGB - Function created.

    void CFrameWindow::closeEvent(QCloseEvent *event)
    {
      mdiArea->closeAllSubWindows();
      if ( !mdiArea->subWindowList().isEmpty() )
      {
        event->ignore();
      }
      else
      {
        writeSettings();
        event->accept();
      }
    }

    /// @brief Enable all the dock widgets to the enabledState.
    /// @param[in] enabledState - State to set the dockwidgets to.
    /// @returns None.
    /// @throws None.
    /// @version 2017-07-02/GGB - Updated for std::map and std::for_each()
    /// @version 2016-04-23/GGB - Function created.

    void CFrameWindow::enableDockWidgetsImage(bool enabledState)
    {
      std::for_each(dockWidgets.begin(), dockWidgets.end(), [&] (std::pair<const EDockWidgets, PDockWidget> &dw)
        { dw.second->setEnabled(enabledState); });
    }

    /// @brief Function to identify all the objects in an image.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-01-28/GGB - Corrected bug with logging. #1107907
    /// @version 2012-07-28/GGB - Function created.

    void CFrameWindow::eventAnalysisExtractObjects()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->extractObjects();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Loads the objects that are reasonable likely to appear in the image.
    /// @throws CCodeError(astroManager)
    /// @version 2016-04-25/GGB - Function created.

    void CFrameWindow::eventAnalysisLoadObjects()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->loadObjects();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }


    }

    /// @brief Loads a target list from file and applies it to the current image.
    /// @throws GCL::CCodeError(astroManager)
    //
    // 2015-01-04/GGB - Function created.

    void CFrameWindow::eventAstrometryLoadTargets()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->astrometryLoadTargets();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Converts a colour image to a grayscale image.
    /// @details The grayscale image is opened in a new window.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2012-01-07/GGB - Function created.

    void CFrameWindow::eventConvertGrayscale()
    {
        // Check that we have a colour image selected.

      CMdiSubWindow *activeChild = activeMdiChild();
      imaging::CImageWindow *imageWindow = nullptr;
      ACL::PAstroFile astroFile;
      ACL::PAstroFile astroFileGrayscale;
      boost::thread_group threadGroup;
      boost::thread *thread;

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          astroFile = dynamic_cast<imaging::CImageWindow *>(activeChild)->getAstroFile();

          //ACL::SRGBHP_Ptr rgbImage = astroFile->renderImageHighPrecision(0);   // Render the image in high colour

            // Create the three colour images. This is multi-threaded to speed up execution.

          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_GRAYSCALE, boost::ref(astroFileGrayscale));
          //threadGroup.add_thread(thread);

          //threadGroup.join_all();     // Wait for all the threads to finish.

            // Create the image Windows.

          //imageWindow = new imaging::CImageWindow(astroFileGrayscale, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Convert a colour image into seperate L, R, G, B images.
    /// @details  As the image can have multiple colour layers, the image needs to be reduced to a colour image before the colour layers are
    ///           extracted.
    /// @throws CCodeError(astroManager)
    //
    // 2013-06-09/GGB - Changed to use smart pointers for astroFile.
    // 2012-01-06/GGB - Function created.

    void CFrameWindow::eventConvertLRGB()
    {
       // Check that we have a colour image selected.

      CMdiSubWindow *activeChild = activeMdiChild();
      imaging::CImageWindow *imageWindow = nullptr;
      ACL::PAstroFile astroFile;
      ACL::PAstroFile astroFileRed, astroFileGreen, astroFileBlue, astroFileLuminance;
      boost::thread_group threadGroup;
      boost::thread *thread;

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          astroFile = dynamic_cast<imaging::CImageWindow *>(activeChild)->getAstroFile();

          //ACL::SRGBHP_Ptr rgbImage = astroFile->renderImageHighPrecision(0);   // Render the image in high colour

            // Create the three colour images. This is multi-threaded to speed up execution.

          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_RED, boost::ref(astroFileRed));
          //threadGroup.add_thread(thread);
          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_GREEN, boost::ref(astroFileGreen));
          //threadGroup.add_thread(thread);
          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_BLUE, boost::ref(astroFileBlue));
          //threadGroup.add_thread(thread);
          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_LUMINANCE, boost::ref(astroFileLuminance));
          //threadGroup.add_thread(thread);
          //thread = nullptr;

          //threadGroup.join_all();     // Wait for all the threads to finish.

            // Create the image Windows.

          //imageWindow = new imaging::CImageWindow(astroFileRed, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();

          //imageWindow = new imaging::CImageWindow(astroFileGreen, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();

          //imageWindow = new imaging::CImageWindow(astroFileBlue, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();

          //imageWindow = new imaging::CImageWindow(astroFileLuminance, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Convert a colour image into seperate R, G, B images.
    /// @throws CCodeError(astroManager)
    /// @details As the image can have multiple colour layers, the image needs to be reduced to a colour image before they colour
    /// layers are extracted.
    /// @version 2013-06-09/GGB - Smart pointers used for astroFile.
    /// @version 2012-01-06/GGB - Function created.

    void CFrameWindow::eventConvertRGB()
    {
        // Check that we have a colour image selected.

      CMdiSubWindow *activeChild = activeMdiChild();
      imaging::CImageWindow *imageWindow = nullptr;
      ACL::PAstroFile astroFile;
      ACL::PAstroFile astroFileRed, astroFileGreen, astroFileBlue;
      boost::thread_group threadGroup;
      boost::thread *thread;

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          astroFile = dynamic_cast<imaging::CImageWindow *>(activeChild)->getAstroFile();

          //ACL::SRGBHP_Ptr rgbImage = astroFile->renderImageHighPrecision(0);   // Render the image in high colour

            // Create the three colour images. This is multi-threaded to speed up execution.

          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_RED, boost::ref(astroFileRed));
          //threadGroup.add_thread(thread);
          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_GREEN, boost::ref(astroFileGreen));
          //threadGroup.add_thread(thread);
          //thread = new boost::thread(&ACL::CAstroFile::CreateFromRGB, astroFile, rgbImage, ACL::COLOUR_BLUE, boost::ref(astroFileBlue));
          //threadGroup.add_thread(thread);
          //thread = nullptr;

          //threadGroup.join_all();     // Wait for all the threads to finish.

            // Create the image Windows.

          //imageWindow = new imaging::CImageWindow(astroFileRed, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();

          //imageWindow = new imaging::CImageWindow(astroFileGreen, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();

          //imageWindow = new imaging::CImageWindow(astroFileBlue, this);
          mdiArea->addSubWindow(imageWindow);
          imageWindow->show();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Function to allow the user to create a master bias frame.
    //
    // 2011-05-22/GGB - Function created.

    void CFrameWindow::eventCreateMasterBiasFrame()
    {
      AstroManager::calibration::CCreateMasterBiasWindow *imageWindow = new AstroManager::calibration::CCreateMasterBiasWindow(this);
      mdiArea->addSubWindow(imageWindow);

      imageWindow->show();
      imageWindow = nullptr;
    }

    /// @brief Function to create as master dark frame.
    //
    // 2011-05-07/GGB - Function created.

    void CFrameWindow::eventCreateMasterDarkFrame()
    {
      AstroManager::calibration::CCreateMasterDarkWindow *imageWindow = new AstroManager::calibration::CCreateMasterDarkWindow(this);
      mdiArea->addSubWindow(imageWindow);

      imageWindow->show();
      imageWindow = nullptr;
    }

    /// @brief Responds to the menu action Calibrate | Create Master Flat Frame
    /// @throw
    /// @version 2011-05-15/GGB - Function created.

    void CFrameWindow::eventCreateMasterFlatFrame()
    {
      AstroManager::calibration::CCreateMasterFlatWindow *imageWindow = new AstroManager::calibration::CCreateMasterFlatWindow(this);
      mdiArea->addSubWindow(imageWindow);

      imageWindow->show();
      imageWindow = nullptr;
    }

    /// @brief Edit | Copy
    /// @throws None.
    /// @version 2013-05-30/GGB - Function created.

    void CFrameWindow::eventEditcopy()
    {
      if (activeMdiChild())
      {
        activeMdiChild()->clipboardCopy();
      };
    }

    /// @brief Opens the Edit Metadata dialog box and allows the user to edit the metadata.
    /// @throws None.
    /// @version 2017-08-20/GGB - Function created.

    void CFrameWindow::eventEditImageMetadata()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->editMetadata();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Function to handle the Edit | Options menu item.
    /// @throws None.
    /// @version 2013-03-29/GGB - Function created.

    void CFrameWindow::eventEditOptions()
    {
      dialogs::CDialogOptions dialog;

      dialog.exec();
    }

    /// @brief Function to handle the Edit | Resources menu item.
    /// @throws None.
    /// @version 2018-02-01/GGB - Function created.

    void CFrameWindow::eventEditResources()
    {
      dialogs::CDialogEditResources dialog(this);

      dialog.exec();
    }

    /// @brief Function to export a table as CSV.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-05-11/GGB - Function created.

    void CFrameWindow::eventExportAsCSV()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->exportAsCSV();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Function to export the current image as a DNG.
    /// @throws GCL::CCCodeError(astroManager)
    /// @version 2013-04-26/GGB - Function created.

    void CFrameWindow::eventExportAsDNG()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->exportAsDNG();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Function to export the current image as a JPEG.
    /// @throws CCodeError(astroManager)
    //
    // 2013-04-26/GGB - Function created.

    void CFrameWindow::eventExportAsJPEG()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(activeChild);
          if (iw)
            iw->exportAsJPEG();
          else
          {
            CODE_ERROR(astroManager);
          };
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Function to export the current image as a PNG
    /// @throws CCodeError(astroManager)
    //
    // 2013-04-26/GGB - Function created.

    void CFrameWindow::eventExportAsPNG()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(activeChild);
          if (iw)
            iw->exportAsPNG();
          else
          {
            CODE_ERROR(astroManager);
          }
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// Function to export the current image as a TIFF
    //
    // 2013-04-26/GGB - Function created.

    void CFrameWindow::eventExportAsTIFF()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->exportAsTIFF();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Manages the File | Batch convert menu option.
    /// @details Each file is opened into an CAstroFile, attributes applied and then saved.
    /// @version 2012-01-25/GGB - Function created.

    void CFrameWindow::eventFileBatchConvert()
    {
      file::CFileBatchConvertDialog dlg;

      dlg.exec();
    }

    /// @brief Upload a group of FITS files into the ARID database.
    /// @throws None.
    /// @version 2018-05-12/GGB - Check for files to upload before beginning upload. (Bug #131)
    /// @version 2017-09-02/GGB - Removed call to registerAndUpload() Bug #115
    /// @version 2017-08-05/GGB - Function created.

    void CFrameWindow::eventFileUploadImages()
    {
      boost::filesystem::path filePath;

        // Ask the user for the files that need to be uploaded.

      QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Upload Image(s)"),
            settings::astroManagerSettings->value(settings::IMAGING_DATABASE_UPLOAD_DIRECTORY, QVariant(0)).toString(), EXTENSION_IMAGE);

      QStringList fileList = fileNames;   // This is suggested in the Qt documentation.

      if (!fileList.empty())
      {
        filePath = (*fileList.begin()).toStdString();

        settings::astroManagerSettings->setValue(settings::IMAGING_DATABASE_UPLOAD_DIRECTORY,
                                                 QVariant(QString::fromStdString(filePath.parent_path().string())));

        QProgressDialog progressDialog(tr("Uploading Files..."), tr("Abort"), 0, fileList.size(), this);
        progressDialog.setWindowModality(Qt::WindowModal);
        progressDialog.setMinimumDuration(1000);
        progressDialog.setWindowTitle("Upload files to Database");

        int fileCount = 0;

        for (auto iter = fileList.begin(); iter != fileList.end(); ++iter)
        {
          try
          {
            filePath = (*iter).toStdString();
            CAstroFile astroFile(this, filePath);
          }
          catch(...)
          {
            ERRORMESSAGE("Error while opening or uploading file:" + filePath.string());
          };

          progressDialog.setValue(++fileCount);

          if (progressDialog.wasCanceled())
          {
            break;
          };
        };
      };
    }

    /// @brief Opens a file for inspection.
    /// @details Inspection of a FITS file is opening the file to view the HDU information in it's native form.
    /// @throws std::bad_alloc
    /// @version 2015-08-04/GGB - Function created.

    void CFrameWindow::eventFileInspect()
    {
      boost::filesystem::path filePath;

      QString szFileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
        settings::astroManagerSettings->value(settings::IMAGING_INSPECT_DIRECTORY, QVariant(0)).toString(), EXTENSION_IMAGE);

      if ( !szFileName.isNull() )
      {
        filePath = boost::filesystem::path(szFileName.toStdString());

        settings::astroManagerSettings->setValue(settings::IMAGING_INSPECT_DIRECTORY, QVariant(QString::fromStdString(filePath.parent_path().string())));

        GCL::logger::defaultLogger().logMessage(GCL::logger::info, "File: " + filePath.string() + " has been opened");

        texteditor::CTextEditorFITS *inspectWindow = new texteditor::CTextEditorFITS(this, filePath);

        mdiArea->addSubWindow(inspectWindow);
        inspectWindow->showMaximized();
        inspectWindow = nullptr;
      };
    }

    /// @brief Handles the save action from the menu/buttons
    /// @throws None.
    /// @version 2010-03-06/GGB - Function created.

    void CFrameWindow::eventFileSave()
    {
      if (activeMdiChild() && activeMdiChild()->save())
      {
        statusBar()->showMessage(tr("File saved"), 2000);
      }
      else
      {
        statusBar()->showMessage(tr("Error while saving file!"), 2000);
      }
    }

    /// @brief Handles the Save As action from the menu/buttons
    /// @throws None.
    /// @version 2010-03-06/GGB - Function created.

    void CFrameWindow::eventFileSaveAs()
    {
      if (activeMdiChild() && activeMdiChild()->saveAs())
      {
        statusBar()->showMessage(tr("File saved"), 2000);
      }
      else
      {
        statusBar()->showMessage(tr("Error while saving file!"), 2000);
      };
     }

    void CFrameWindow::eventFileSaveDatabase()
    {

    }

    /// @brief Managed the menu function "FILE | SEARCH"
    /// @throws std::bad_alloc
    /// @version 2017-07-28/GGB - Function create.

    void CFrameWindow::eventFileSearch()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_FILE_SEARCH));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_FILE_SEARCH, new CWindowSelectImage(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_FILE_SEARCH));
        uniqueWindows.at(SWT_FILE_SEARCH)->showMaximized();
      };
    }

    /// @brief Action from the transform menu
    /// @details Send onto the open child window, if the child window is of the correct type.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2011-05-29/GGB - Function created.

    void CFrameWindow::eventFlipImage()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->flipImage();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Handles the action to float an image.
    /// @throws GCL::CCodeError(astroManager)
    //
    // 2011-06-04/GGB - Function created.

    void CFrameWindow::eventFloatImage()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->floatImage();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Action from the transform menu
    /// @details Send onto the open child window, if the child window is of the correct type.
    /// @throws GCL::CCodeError(astroManager)
    //
    // 2011-05-29/GGB - Function created.

    void CFrameWindow::eventFlopImage()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->flopImage();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Procedure to handle the menu event for the imageControl selection.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2011-05-28/GGB - Function created.

    void CFrameWindow::eventImageControl()
    {
      if (!menuActions[IDA_VIEW_IMAGECONTROL]->isChecked())
      {
        dockWidgets[IDDW_IMAGECONTROL]->hide();
        menuActions[IDA_VIEW_IMAGECONTROL]->setChecked(false);
      }
      else
      {
        if (dockWidgets[IDDW_IMAGECONTROL])
        {
            // Already exists, just show the dock widget.

          dockWidgets[IDDW_IMAGECONTROL]->show();
        }
        else
        {
            // Does not exist. Must be created

          dockWidgets.emplace(IDDW_IMAGECONTROL, std::make_unique<dockwidgets::CImageControlWidget>(this, &*menuActions[IDA_VIEW_IMAGECONTROL]));
          addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_IMAGECONTROL]);
        };
        menuActions[IDA_VIEW_IMAGECONTROL]->setChecked(true);   // Check the action.
      };
    }

    /// @brief Called when the message widget menu item is clicked by the user.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2012-01-20/GGB - Function created

    void CFrameWindow::eventMessageWidget()
    {
      if (menuActions[IDA_VIEW_CONSOLEWINDOW]->isChecked())
      {
          // Window should be open already

        dockWidgets[IDDW_MESSAGECONTROL]->show();
      }
      else
      {
          // Window should be closed... needs to be shown

        dockWidgets[IDDW_MESSAGECONTROL]->hide();
      };
    }

    /// @brief Loads and applies a list of photometry targets to the current image.
    /// @throws CCodeError(astroManager)
    /// @version 2013-08-19/GGB - Function created.

    void CFrameWindow::eventPhotometryLoadTargets()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      if (activeChild)
      {
        if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->photometryLoadTargets();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Menu action File | Print.
    /// @throws CCodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2013-03-29/GGB - Function created.

    void CFrameWindow::eventPrint()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->print();
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Menu action File | Print Preview.
    /// @throws CCodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    //
    // 2013-03-29/GGB - Function created.

    void CFrameWindow::eventPrintPreview()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->printPreview();
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Menu action File | Print Setup.
    /// @throws CCodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2013-03-29/GGB - Function created.

    void CFrameWindow::eventPrintSetup()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->printSetup();
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Menu action Transform | Resample Image
    /// @throws CCodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2011-06-04/GGB - Function created.

    void CFrameWindow::eventResampleImage()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->resampleImage();
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Opens the planning window
    /// @throws None.
    /// @version 2018-03-06/GGB - Function created.

    void CFrameWindow::eventUtilityPlanning()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_UTILITY_PLANNING));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_UTILITY_PLANNING, new windowPlanning::CWindowPlanning(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_UTILITY_PLANNING));

        uniqueWindows.at(SWT_UTILITY_PLANNING)->show();
      };
    }

    /// @brief Notifies the parent window whether the annotations should be displayed or not.
    //
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2013-05-08/GGB - Added code to check the active child is not null.
    /// @version 2013-03-16/GGB - Function Created.

    void CFrameWindow::eventViewAnnotations()
    {
      settings::astroManagerSettings->setValue(settings::ANNOTATIONS_DISPLAY, QVariant(menuActions[IDA_VIEW_ANNOTATIONS]->isChecked()));
    }

    /// @brief Notifies the parent window whether the Astrometry should be displayed or not.
    /// @throws CCodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2013-05-10/GGB - Updates the settings to reflect the latest state.
    /// @version 2013-05-08/GGB - Added code to check if the activeChild is not nullptr.
    /// @version 2013-03-16/GGB - Function Created.

    void CFrameWindow::eventViewAstrometry()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      settings::astroManagerSettings->setValue(settings::ASTROMETRY_DISPLAYINDICATORS, QVariant(menuActions[IDA_VIEW_ASTROMETRY]->isChecked()));

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->repaintImage();
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    void CFrameWindow::eventViewDockWidgetWeatherParameters()
    {

    }

    void CFrameWindow::eventViewDockWidgetWeatherScale()
    {

    }

    /// @brief Called when the user wishes to view/hide the histogram.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2013-02-02/GGB - Function created.

    void CFrameWindow::eventViewHistogram()
    {
      if (!menuActions[IDA_VIEW_HISTOGRAM]->isChecked())
      {
        dockWidgets[IDDW_VIEW_HISTOGRAM]->hide();
        menuActions[IDA_VIEW_HISTOGRAM]->setChecked(false);
      }
      else
      {
        if (dockWidgets[IDDW_VIEW_HISTOGRAM])
        {
            // Already exists, just show the dock widget.

          dockWidgets[IDDW_VIEW_HISTOGRAM]->show();
        }
        else
        {
            // Does not exist. Must be created

          dockWidgets.emplace(IDDW_VIEW_HISTOGRAM, std::make_unique<dockwidgets::CImageControlWidget>(this, &*menuActions[IDA_VIEW_HISTOGRAM]));
          addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_VIEW_HISTOGRAM]);
        };
        menuActions[IDA_VIEW_HISTOGRAM]->setChecked(true);   // Check the action.
      };
    }

    /// @brief Notifies the dock widget if the dockwidget should be displayed or not.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    // 2013-05-19/GGB - Function created.

    void CFrameWindow::eventViewMagnify()
    {
      if (!menuActions[IDA_VIEW_MAGNIFY]->isChecked())
      {
        dockWidgets[IDDW_VIEW_MAGNIFY]->hide();
        menuActions[IDA_VIEW_MAGNIFY]->setChecked(false);
      }
      else
      {
        if (dockWidgets[IDDW_VIEW_MAGNIFY])
        {
            // Already exists, just show the dock widget.

          dockWidgets[IDDW_VIEW_MAGNIFY]->show();
        }
        else
        {
            // Does not exist. Must be created

          dockWidgets.emplace(IDDW_VIEW_MAGNIFY, std::make_unique<dockwidgets::CImageControlWidget>(this, &*menuActions[IDA_VIEW_MAGNIFY]));
          addDockWidget(Qt::AllDockWidgetAreas, &*dockWidgets[IDDW_VIEW_MAGNIFY]);
        };
        menuActions[IDA_VIEW_MAGNIFY]->setChecked(true);   // Check the action.
      };
    }

    /// @brief Notifies the dock widget if the dockwidget should be displayed or not.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    // 2013-05-24/GGB - Function created.

    void CFrameWindow::eventViewNavigator()
    {
      if (!menuActions[IDA_VIEW_NAVIGATOR]->isChecked())
      {
        dockWidgets[IDDW_VIEW_NAVIGATOR]->hide();
        menuActions[IDA_VIEW_NAVIGATOR]->setChecked(false);
      }
      else
      {
        if (dockWidgets[IDDW_VIEW_NAVIGATOR])
        {
            // Already exists, just show the dock widget.

          dockWidgets[IDDW_VIEW_NAVIGATOR]->show();
        }
        else
        {
            // Does not exist. Must be created

          dockWidgets.emplace(IDDW_VIEW_NAVIGATOR, std::make_unique<dockwidgets::CImageControlWidget>(this, &*menuActions[IDA_VIEW_NAVIGATOR]));
          addDockWidget(Qt::AllDockWidgetAreas, &*dockWidgets[IDDW_VIEW_NAVIGATOR]);
        };
        menuActions[IDA_VIEW_NAVIGATOR]->setChecked(true);   // Check the action.
      };
    }

    /// @brief Notifies the parent window whether the Photometry should be displayed or not.
    /// @param None.
    /// @returns None.
    /// @throws GCL::CCodeError(astroManager)
    /// @throws GCL::CRuntimeAssert(astroManager)
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2013-05-10/GGB - Updates the settings to reflect the latest state.
    /// @version 2013-03-16/GGB - Function Created.

    void CFrameWindow::eventViewPhotometry()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_DISPLAYINDICATORS, QVariant(menuActions[IDA_VIEW_PHOTOMETRY]->isChecked()));

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->repaintImage();
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Menu action to open the weather history menu.
    /// @throws None.
    /// @version 2017-06-20/GGB - Function created.

    void CFrameWindow::eventWeatherHistory()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_WEATHER_HISTORY));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.
        
        uniqueWindows.emplace(SWT_WEATHER_HISTORY, new weather::CWindowWeatherHistory(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_WEATHER_HISTORY));

        uniqueWindows.at(SWT_WEATHER_HISTORY)->show();
      };
    }

    /// @brief Menu action to zoom the image to 1:1
    /// @throws GCL::CCodeError(astroManager)
    /// @throws GCL::CRUntimeAssert(astroManager)
    /// @version 2013-07-14/GGB - Added code to zoom the stack images window. (Bug #1195976)
    /// @version 2012-07-21/GGB - Function created.

    void CFrameWindow::eventZoom11()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
        case SWT_IMAGEWINDOW:
        {
          imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(activeChild);
          if (iw)
          {
            iw->zoom11();
          }
          else
          {
            CODE_ERROR(astroManager);
          };
          break;
        }
        case SWT_STACKIMAGESWINDOW:
        {
          imaging::CStackImagesWindow *siw = dynamic_cast<imaging::CStackImagesWindow *>(activeChild);
          if (siw)
          {
            siw->zoom11();
          }
          else
          {
            CODE_ERROR(astroManager);
          };
          break;
        }
        default:
        {
          CODE_ERROR(astroManager);
          break;
        };
      };
    }

    /// @brief Menu Function View | Zoom Out
    /// @throws CCodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2013-07-14/GGB - Added code to zoom the stack images window. (Bug #1195976)
    /// @version 2011-06-02/GGB - Function created.

    void CFrameWindow::eventZoomOut()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
      case SWT_IMAGEWINDOW:
      {
        imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(activeChild);
        if (iw)
        {
          iw->zoomOut();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
        break;
      }
      case SWT_STACKIMAGESWINDOW:
      {
        imaging::CStackImagesWindow *siw = dynamic_cast<imaging::CStackImagesWindow *>(activeChild);
        if (siw)
        {
          siw->zoomOut();
        }
        else
        {
          CODE_ERROR(astroManager);
        }
        break;
      }
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Returns the specified action to allow it to be modified.
    /// @note The action should never be deleted.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2017-07-02/GGB - Converted to use std::map<>.
    /// @version 2013-07-28/GGB - Changed parameter to size_t and removed check of (< 0)
    /// @version 2011-05-15/GGB - Function created.

    QAction *CFrameWindow::getAction(EActionItems actionItem) const
    {
      QAction *returnValue = nullptr;

      try
      {
        returnValue = &*menuActions.at(actionItem);
      }
      catch (std::out_of_range const &)
      {
        // No action necessary. returnValue is already nullptr.
      }

      return returnValue;
    }

    /// brief This function is guaranteed to return a valid value for a dockwidget.
    /// @returns Pointer to the relevant dock widget
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2017-07-02/GGB - Updated to use std::map and std::unique_ptr.
    /// @version 2013-05-28/GGB - Function created.

    dockwidgets::CDockWidget &CFrameWindow::getDockWidget(EDockWidgets dockWidget)
    {
      try
      {
        return *dockWidgets.at(dockWidget);
      }
      catch (std::out_of_range const &)
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Calls up the Help|About dialog box
    /// @throws None.
    /// @version 2012-01-01/GGB - Function created.

    void CFrameWindow::HelpAbout()
    {

      help::CHelpAboutAstroManager dlg;

      dlg.exec();
    }

    /// @brief  Displays a dialog to say that the function is not implemented, but should be implemented in future releases.
    /// @throws None.
    /// @version 2013-04-27/GGB - Changed to single function
    /// @version 2012-01-04/GGB - Function created.

    void CFrameWindow::eventUnimplemented()
    {
      QMessageBox msgBox;

      msgBox.setWindowTitle(tr("This function is not yet implemented."));
      msgBox.setText(tr("This function is planned to be implemented in the future."));
      msgBox.setInformativeText(tr("Please check the project website for more information regarding the implementation of this functionality " \
                                "in future releases."));
      msgBox.setIcon(QMessageBox::Question);
      msgBox.setStandardButtons(QMessageBox::Ok);

      msgBox.exec();
    }

    /// @brief Updates the window menu to display the list of child windows.
    /// @throws None.
    /// @version 2017-07-14/GGB - Added support to enable/disable the menu items.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2010-06-05/GGB - Function created.

    void CFrameWindow::updateWindowMenu()
    {
      subMenus[IDSM_WINDOW]->clear();
      subMenus[IDSM_WINDOW]->addAction(&*menuActions[IDA_WINDOW_CLOSE]);
      subMenus[IDSM_WINDOW]->addAction(&*menuActions[IDA_WINDOW_CLOSEALL]);
      subMenus[IDSM_WINDOW]->addSeparator();
      subMenus[IDSM_WINDOW]->addAction(&*menuActions[IDA_WINDOW_TILE]);
      subMenus[IDSM_WINDOW]->addAction(&*menuActions[IDA_WINDOW_CASCADE]);
      subMenus[IDSM_WINDOW]->addSeparator();
      subMenus[IDSM_WINDOW]->addAction(&*menuActions[IDA_WINDOW_NEXT]);
      subMenus[IDSM_WINDOW]->addAction(&*menuActions[IDA_WINDOW_PREVIOUS]);

      QList<QMdiSubWindow *> windows = mdiArea->subWindowList();

      if (!windows.isEmpty())
      {
        subMenus[IDSM_WINDOW]->addSeparator();
      };

      for (int i = 0; i < windows.size(); ++i)
      {
        auto child = qobject_cast<QMdiSubWindow *>(windows.at(i));

        QString text;
        if (i < 9)
        {
          text = tr("&%1 %2").arg(i + 1).arg(child->windowTitle());
        }
        else
        {
          text = tr("%1 %2").arg(i + 1).arg(child->windowTitle());
        };
        QAction *action  = subMenus[IDSM_WINDOW]->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
      };

      if (windows.isEmpty())
      {
        menuActions[IDA_WINDOW_CLOSE]->setEnabled(false);
        menuActions[IDA_WINDOW_CLOSEALL]->setEnabled(false);
        menuActions[IDA_WINDOW_TILE]->setEnabled(false);
        menuActions[IDA_WINDOW_CASCADE]->setEnabled(false);
        menuActions[IDA_WINDOW_NEXT]->setEnabled(false);
        menuActions[IDA_WINDOW_PREVIOUS]->setEnabled(false);
      }
      else if (windows.size() == 1)
      {
        menuActions[IDA_WINDOW_CLOSE]->setEnabled(true);
        menuActions[IDA_WINDOW_CLOSEALL]->setEnabled(false);
        menuActions[IDA_WINDOW_TILE]->setEnabled(false);
        menuActions[IDA_WINDOW_CASCADE]->setEnabled(false);
        menuActions[IDA_WINDOW_NEXT]->setEnabled(false);
        menuActions[IDA_WINDOW_PREVIOUS]->setEnabled(false);
      }
      else
      {
        menuActions[IDA_WINDOW_CLOSE]->setEnabled(true);
        menuActions[IDA_WINDOW_CLOSEALL]->setEnabled(true);
        menuActions[IDA_WINDOW_TILE]->setEnabled(true);
        menuActions[IDA_WINDOW_CASCADE]->setEnabled(true);
        menuActions[IDA_WINDOW_NEXT]->setEnabled(true);
        menuActions[IDA_WINDOW_PREVIOUS]->setEnabled(true);
      }
    }

    /// @brief Creates an MDI Child window.
    /// @returns a new mdi child window.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr

    QWidget *CFrameWindow::createMdiChild(void)
    {
      QWidget *child = new QWidget();
      mdiArea->addSubWindow(child);
      connect(child, SIGNAL(copyAvailable(bool)), &*menuActions[IDA_EDIT_COPY], SLOT(setEnabled(bool)));

      return child;
    }

    /// @brief Create the application status bar.
    /// @throws std::bad_alloc
    /// @version 2015-01-02/GGB - Added RAM, CPU and virtual memory widgets.
    /// @version 2012-07-28/GGB - Added UTC and JD to status bar.
    /// @version 2010-03-06/GGB - Function created.

    void CFrameWindow::createStatusBar()
    {
      statusBar()->showMessage(tr("Ready"));

        // Add the fields to the status bar.

      statusBar()->addPermanentWidget(labelCPU = new QLabel(""));
      statusBar()->addPermanentWidget(labelRAM = new QLabel(""));
      statusBar()->addPermanentWidget(labelSwap = new QLabel(""));
      statusBar()->addPermanentWidget(labelUTC = new QLabel(""));
      statusBar()->addPermanentWidget(labelJD = new QLabel(""));
    }

    /// @brief Handles the menu action to compare two or more images.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2011-06-12/GGB - Function created

    void CFrameWindow::eventCompareImages()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_IMAGE_COMPARE));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_IMAGE_COMPARE, new imaging::CImageComparisonWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_IMAGE_COMPARE));
        uniqueWindows.at(SWT_IMAGE_COMPARE)->showMaximized();
      };
    }

    /// @brief Menu function to set up instrument information.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2011-06-23/GGB - Function created.

    void CFrameWindow::eventConfigureInstruments()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CONFIGURE_INSTRUMENTS));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CONFIGURE_INSTRUMENTS, new configure::CConfigureInstrumentsWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CONFIGURE_INSTRUMENTS));
        uniqueWindows.at(SWT_CONFIGURE_INSTRUMENTS)->show();
      };
    }

    /// @brief Menu function to set up observer informaiton.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2011-06-22/GGB - Function created.

    void CFrameWindow::eventConfigureObservers()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CONFIGURE_OBSERVERS));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CONFIGURE_OBSERVERS, new configure::CConfigureObserversWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CONFIGURE_OBSERVERS));
        uniqueWindows.at(SWT_CONFIGURE_OBSERVERS)->show();
      };
    }

    /// @brief Allows the user to configure the catalog sort order.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2011-06-25/GGB - Function created.

    void CFrameWindow::eventConfigureCatalogPrefs()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CONFIGURE_CATALOGPREFS));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CONFIGURE_CATALOGPREFS, new configure::CConfigureCatalogWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CONFIGURE_CATALOGPREFS));
        uniqueWindows.at(SWT_CONFIGURE_CATALOGPREFS)->show();
      };
    }

    /// @brief Handles the menu action to configure sites.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2011-06-19/GGB - Function created.

    void CFrameWindow::eventConfigureSites()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CONFIGURE_SITES));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CONFIGURE_SITES, new configure::CConfigureSitesWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CONFIGURE_SITES));
        uniqueWindows.at(SWT_CONFIGURE_SITES)->show();
      };
    }

    /// @brief Handles the menu action to configure telescopes.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2011-06-25/GGB - Function created.

    void CFrameWindow::eventConfigureTelescopes()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CONFIGURE_TELESCOPES));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CONFIGURE_TELESCOPES, new configure::CConfigureTelescopesWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CONFIGURE_TELESCOPES));
        uniqueWindows.at(SWT_CONFIGURE_TELESCOPES)->show();
      };
    }

    /// @brief Function to crop an image.
    /// @throws GCL::CCodeError(astroManager)
    /// @details Entry of the crop parameters is via a dialog box, specifying the origen and dimensions of the final image.
    /// @version 2013-03-16/GGB - Function created.

    void CFrameWindow::eventImageCrop()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->cropImage();
      }
      else
      {
        CODE_ERROR(astroManager);
      }

    }

    /// @brief  Handles the menu item for bringing up the reference image dock widget.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2011-06-12/GGB - Function created.

    void CFrameWindow::eventReferenceImage()
    {
      if (!menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]->isChecked())
      {
        dockWidgets[IDDW_ASTROMETRYCONTROL]->hide();
        menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]->setChecked(false);
      }
      else
      {
        if (dockWidgets[IDDW_ASTROMETRYCONTROL])
        {
            // Already exists, just show the dock widget.

          dockWidgets[IDDW_ASTROMETRYCONTROL]->show();
        }
        else
        {
            // Does not exist. Must be created

          dockWidgets.emplace(IDDW_ASTROMETRYCONTROL, std::make_unique<dockwidgets::CAstrometryDockWidget>(this, &*menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]));
          addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_ASTROMETRYCONTROL]);
        };
        menuActions[IDA_ASTROMETRY_REFERENCEIMAGE]->setChecked(true);   // Check the action.
      };
    }

    /// @brief Writes frame window settings.
    /// @throws None.
    /// @version 2013-07-27/GGB - Added code to save the state and geometry.
    /// @version 2013-06-20/GGB - Changed setting names and added the maximised check and keyword.
    /// @version 2010-05-22/GGB - Function created.

    void CFrameWindow::writeSettings()
    {
      settings::astroManagerSettings->setValue(settings::FRAME_GEOMETRY, saveGeometry());
      settings::astroManagerSettings->setValue(settings::DW_STATE, saveState());

      settings::astroManagerSettings->setValue(settings::FRAME_MAXIMISED, isMaximized());
      settings::astroManagerSettings->setValue(settings::FRAME_POSITION, pos());
      settings::astroManagerSettings->setValue(settings::FRAME_SIZE, size());
    }

    /// @brief Sets the active sub window.
    /// @param[in] window - The window to set as the active window.
    /// @version 2015-08-08/GGB - Function created.

   void CFrameWindow::setActiveSubWindow(QWidget *window)
   {
     if (window)
     {
       mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
     };
   }

    /// @brief Brings up the help dialog for help on the ATID database.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2018-04-15/GGB - 1. Changed raw pointer to std::unique_ptr
    /// @version 2018-04-15/GGB - 2. Added error checking that the resource exists and added CODE_ERROR. (Bug #124)
    /// @version 2010-05-11/GGB - Function created.

    void CFrameWindow::HelpATID(void)
    {
      std::unique_ptr<help::CHelpATID> dialog = std::make_unique<help::CHelpATID>();
      QUiLoader loader;

      QFile file(":/forms/dialogAboutATID.ui");

      if (!file.open(QIODevice::ReadOnly))
      {
        CODE_ERROR(astroManager);
      }
      else
      {
        QDialog *formWidget = (QDialog *) loader.load(&file, (QWidget *) this);
        dialog->setParent(formWidget);
        file.close();

        formWidget->exec();
      };
    }

    /// @brief Opens or switches to the View | Object Information window.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2010-05-12/GGB - Function created.

    void CFrameWindow::menuViewObjectInfo()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_OBJECT_INFORMATION));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_OBJECT_INFORMATION, new CObjectInfoWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_OBJECT_INFORMATION));
        uniqueWindows.at(SWT_OBJECT_INFORMATION)->show();
      };
    }

    /// @brief Called when a child window is closed to null the child window pointer.
    /// @details The removeSubWindow function is also called to remove the child window from the windows menu.
    /// @throws None.
    /// @version 2017-06-20/GGB - Added windowWeatherHistory support.
    /// @version 2013-05-30/GGB - Removed unused window types.
    /// @version 2013-03-29/GGB - Added code to update the action states when there is no child window selected.
    /// @version 2010-05-21/GGB - Function created.

    void CFrameWindow::childClosing(QMdiSubWindow *child)
    {
        // Remove any unique windows.

      auto mdiChild = dynamic_cast<CMdiSubWindow *>(child);
      uniqueWindows.erase(mdiChild->getWindowType());

      mdiArea->removeSubWindow ( child );

        // Check if there are no more child windows open

      if (mdiArea->subWindowList().empty())
      {
        setBaseActionStates();
      }
    }

    /// @brief Calls the function to bin pixels.
    /// @throws GCL::CCodeError(astroManager)
    //// @version 2011-06-10/GGB - Function created

    void CFrameWindow::eventBinPixels()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      if (activeChild->getWindowType() == SWT_IMAGEWINDOW)
      {
        dynamic_cast<imaging::CImageWindow *>(activeChild)->binPixels();
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Processes the event for the Calculate ! Greg->JD menu item.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2010-05-21/GGB - Function created.

    void CFrameWindow::eventCalculateGreg2JD()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CALC_GREG2JD));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CALC_GREG2JD, new calculation::CGregorian2JD(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CALC_GREG2JD));
        uniqueWindows.at(SWT_CALC_GREG2JD)->show();
      };
    }

    /// @brief Processes the event for the Calculate JD->Gregorian menu item.
    /// @throws None.
    /// @version 2017-07-09/GGB - Function created.

    void CFrameWindow::eventCalculateJD2Gregorian()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CALC_JD2GREG));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CALC_JD2GREG, new calculation::CJD2Gregorian(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CALC_JD2GREG));
        uniqueWindows.at(SWT_CALC_JD2GREG)->show();
      };
    }

    /// @brief Opens a light curve analysis window. An unlimited number of light curve analysis windows are allowed to be open at a
    ///        time.
    /// @version 2010-06-27/GGB - Function created.

    void CFrameWindow::eventPhotometryLightCurves()
    {
      photometry::CWindowLightCurves *newWindow = new photometry::CWindowLightCurves(this);
      mdiArea->addSubWindow(newWindow);
      newWindow->show();
    }

    /// @brief Handles the menu event for the Photometry dock widget.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2011-06-19/GGB - Function created.

    void CFrameWindow::eventPhotometrySingleImage()
    {
      if (!menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]->isChecked())
      {
        dockWidgets[IDDW_PHOTOMETRYCONTROL]->hide();
        menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]->setChecked(false);
      }
      else
      {
        if (dockWidgets[IDDW_PHOTOMETRYCONTROL])
        {
            // Already exists, just show the dock widget.

          dockWidgets[IDDW_PHOTOMETRYCONTROL]->show();
        }
        else
        {
            // Does not exist. Must be created

          dockWidgets.emplace(IDDW_PHOTOMETRYCONTROL, std::make_unique<dockwidgets::CPhotometryDockWidget>(this, &*menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]));
          addDockWidget(Qt::RightDockWidgetArea, &*dockWidgets[IDDW_PHOTOMETRYCONTROL]);
        };
        menuActions[IDA_PHOTOMETRY_SINGLEIMAGE]->setChecked(true);   // Check the action.
      };
    }

    /// @brief This procedure is called every second to update the relevant screen displays.
    /// @details The Julian Day number and UTC are updated every second.
    /// @throws None.
    /// @version 2017-07-05/GGB - Changed DateTime to a instance rather than a pointer.
    /// @version 2015-01-02/GGB - Added the functionality to display memory and CPU usage.
    /// @version 2013-09-22/GGB - 1) Removed call to tzset() before time(...) <br>
    ///                           2) Added code tp select local time or UTC.
    /// @version 2010-05-27/GGB - 1) Changed from WM_TIMER to eventTimer1s <br>
    ///                           2) Modified to use the Qt API.
    /// @version 2009-09-19/GGB - Function created.

    void CFrameWindow::eventTimer1s()
    {
      bool LT = settings::astroManagerSettings->value(settings::SETTINGS_LT, QVariant(true)).toBool();
      time_t CurrentTime;
      struct tm *UTCTime;
      char szUTC[80];
      char szJD[80];

      static GCL::CResourceUsage resourceUsage;

      resourceUsage.determineMemory();
      resourceUsage.determineCPU();

      std::string swapMemory = "Swap: " + GCL::prettyPrintBytes(resourceUsage.usedSwapMemory()) + "/" +
                               GCL::prettyPrintBytes(resourceUsage.totalSwapMemory());

      labelSwap->setText(QString::fromStdString(swapMemory));

      std::string ramMemory = "Physical RAM: " + GCL::prettyPrintBytes(resourceUsage.usedRAM()) + "/" +
                              GCL::prettyPrintBytes(resourceUsage.totalRAM());

      labelRAM->setText(QString::fromStdString(ramMemory));

      std::string cpuString = boost::str(boost::format("CPU: %.2f%%") % resourceUsage.usageCPU());
      labelCPU->setText(QString::fromStdString(cpuString));

      time(&CurrentTime);
      if (LT)
      {
        UTCTime = localtime(&CurrentTime);
        strftime(szUTC, sizeof(szUTC), "LT: %Y-%m-%d %H:%M:%S", UTCTime);
      }
      else
      {
        UTCTime = gmtime(&CurrentTime);
        strftime(szUTC, sizeof(szUTC), "UTC: %Y-%m-%d %H:%M:%S", UTCTime);
      };

      labelUTC->setText(QString(szUTC));

      ACL::TJD DateTime(UTCTime);
      DateTime.JD(szJD, sizeof(szJD), 6);
      strcpy(szUTC, "JD: ");
      strcat(szUTC, szJD);
      labelJD->setText(QString(szUTC));
    }

    /// @brief Open an image from file.
    /// @details User must choose file to open from dialog box. Create an CImageWindow and set the filename up
    /// @throws None.
    /// @version 2013-03-02/GGB - Included the global settings::fileExtensions for the files extensions.
    /// @version 2013-01-21/GGB - Moved code into loadImage()
    /// @version 2011-10-05/GGB - Function created

    void CFrameWindow::eventImageOpen()
    {
      boost::filesystem::path filePath;

      QString szFileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
        settings::astroManagerSettings->value(settings::IMAGING_DIRECTORY, QVariant(0)).toString(), EXTENSION_IMAGE);

      if ( !szFileName.isNull() )
      {
        filePath = boost::filesystem::path(szFileName.toStdString());

        loadFromFile(filePath);

        settings::astroManagerSettings->setValue(settings::IMAGING_DIRECTORY,
                                         QVariant(QString::fromStdString(filePath.parent_path().string())));
      };
    }

    /// @brief Function used for calibrating a single image.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2011-06-04/GGB - Function Created

    void CFrameWindow::eventImagingSingleCalibration()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
        case SWT_IMAGEWINDOW:
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->calibrateImage();
          break;
        };
        default:
        {
          CODE_ERROR(astroManager);
          break;
        };
      };
    }

    /// @brief Function used for calibrating multiple images.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use std::map to store image pointers.
    /// @version 2010-12-04/GGB - Function created.

    void CFrameWindow::eventImagingMultipleCalibration()
    {
      try
      {
        mdiArea->setActiveSubWindow(uniqueWindows.at(SWT_CALIBRATION_MULTIPLE));      // Will throw if the entry does not exist.
      }
      catch(std::out_of_range)
      {
          // Window does not exist.

        uniqueWindows.emplace(SWT_CALIBRATION_MULTIPLE, new calibration::CImageCalibrationMultipleWindow(this));
        mdiArea->addSubWindow(uniqueWindows.at(SWT_CALIBRATION_MULTIPLE));
        uniqueWindows.at(SWT_CALIBRATION_MULTIPLE)->show();
      };
    }

    /// @brief Opens an image stacking window.
    /// @throws None.
    /// @version 2017-08-19/GGB - Added check for showMaximised.
    /// @version 2011-02-13/GGB - Function created

    void CFrameWindow::eventImagingStackImages()
    {
      imaging::CStackImagesWindow *imageWindow = new imaging::CStackImagesWindow(this);
      mdiArea->addSubWindow(imageWindow);

      if (settings::astroManagerSettings->value(settings::IMAGESTACK_OPENMAXIMISED, QVariant(true)).toBool())
      {
        imageWindow->showMaximized();
      }
      else
      {
        imageWindow->show();
      };

      imageWindow = nullptr;
    }

    /// @brief Menu Function Transform | Rotate.
    /// @throws GCL::CRuntimeAssert(astroManager)
    /// @version 2011-05-30/GGB - Function Created

    void CFrameWindow::eventTransformRotate()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
        case SWT_IMAGEWINDOW:
        {
          dynamic_cast<imaging::CImageWindow *>(activeChild)->rotateImage();
          break;
        };
        default:
        {
          CODE_ERROR(astroManager);
          break;
        };
      };
    }

    /// @brief Menu Function View | Zoom All
    /// @throws GLC::CCodeError(astroManager)
    /// @version 2013-07-14/GGB - Added code to zoom the stack images window. (Bug #1195976)
    /// @version 2011-06-02/GGB - Function created.

    void CFrameWindow::eventZoomAll()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
      case SWT_IMAGEWINDOW:
      {
        imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(activeChild);
        if (iw)
          iw->zoomAll();
        else
          CODE_ERROR(astroManager);
        break;
      }
      case SWT_STACKIMAGESWINDOW:
      {
        imaging::CStackImagesWindow *siw = dynamic_cast<imaging::CStackImagesWindow *>(activeChild);
        if (siw)
          siw->zoomAll();
        else
          CODE_ERROR(astroManager);
        break;
      }
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Menu Function View | Zoom In
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-07-14/GGB - Added code to zoom the stack images window. (Bug #1195976)
    /// @version 2011-06-02/GGB - Function created.

    void CFrameWindow::eventZoomIn()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
      case SWT_IMAGEWINDOW:
      {
        imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(activeChild);
        if (iw)
        {
          iw->zoomIn();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
        break;
      };
      case SWT_STACKIMAGESWINDOW:
      {
        imaging::CStackImagesWindow *siw = dynamic_cast<imaging::CStackImagesWindow *>(activeChild);
        if (siw)
        {
          siw->zoomIn();
        }
        else
        {
          CODE_ERROR(astroManager);
        };
        break;
      }
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Menu Function View | Zoom Selection
    /// @throws GCL::CRuntimeAssert
    /// @version 2011-06-02/GGB - Function created.

    void CFrameWindow::eventZoomSelection()
    {
      CMdiSubWindow *activeChild = activeMdiChild();

      RUNTIME_ASSERT(astroManager, activeChild != nullptr, "No active child window.");

      switch (activeChild->getWindowType())
      {
      case SWT_IMAGEWINDOW:
        dynamic_cast<imaging::CImageWindow *>(activeChild)->zoomSelection();
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Takes ownership of the astroFile and opens a sub window to display the astro file.
    /// @param[in] astroFile - The astrofile to open a window for.
    /// @note The astrofile should already be loaded.
    /// @throws None.
    /// @version 2013-02-10/GGB - Function created.

    void CFrameWindow::imageCreateWindow(PAstroFile astroFile)
    {
      imaging::CImageWindow *imageWindow = new imaging::CImageWindow(astroFile, this);

      mdiArea->addSubWindow(imageWindow);

      imageWindow->showMaximized();

      imageWindow->zoomAll();
      imageWindow = nullptr;
    }

    /// @brief Opens an image from the ARID database.
    /// @param[in] imageID - The ID value of the image to open.
    /// @throws
    /// @version 2017-08-11/GGB - Function created.

    void CFrameWindow::imageOpenFromDatabase(imageID_t imageID)
    {
      imageVersion_t imageVersion;
      bool versionValid = false;

      if (settings::astroManagerSettings->value(settings::CM_DATABASE_OPENFILE_LASTVERSION, QVariant(false)).toBool())
      {
          // Open the latest version of the image.

        DEBUGMESSAGE("ImageOpenFromDatabase: Open latest version");

        if (database::databaseARID->versionLatest(imageID, imageVersion ))
        {
          versionValid = true;
          DEBUGMESSAGE("Latest version: " + std::to_string(imageVersion));
        };
      }
      else if (database::databaseARID->versionCount(imageID) == 1)
      {
          // Check if there is only 1 version that exists. If so, open it.

        DEBUGMESSAGE("ImageOpenFromDatabase: Only 1 version");

        if (database::databaseARID->versionLatest(imageID, imageVersion))
        {
          versionValid = true;
          DEBUGMESSAGE("Latest version: " + std::to_string(imageVersion));
        };
      }
      else if (database::databaseARID->versionCount(imageID) == 0)
      {
          // Hanging record. This implies that the imageData has been deleted, but not the image. This is
          // done intentionally, so is not an error. (Think if an image in a sequence is poor, the user may
          // wish to track what happened with the missing image.

        QMessageBox msgBox;

        msgBox.setText(tr("No Image Stored in Database."));
        msgBox.setInformativeText(tr("There is no image stored in the database that corresponds to this image record."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
      }
      else
      {
          // Display the dialog.

        dialogs::CDialogSelectImageVersion dialog(imageID, imageVersion, this);

        if (dialog.exec())
        {
          versionValid = true;
        };
      };

      if (versionValid)
      {
        QMessageBox msgBox;
        QStringList recentFileList;

        std::list<boost::filesystem::path>::iterator iterator;

        try
        {
          PAstroFile astroFile(std::make_shared<CAstroFile>(this, imageID, imageVersion));   // Create from database.

          //GCL::logger::defaultLogger().logMessage(GCL::logger::info, "File: " + filePath.string() + " has been opened");

          imageCreateWindow(astroFile);

          // Update the last opened file list.

//          lastOpened.remove(filePath);
//          lastOpened.push_front(filePath);
//          while (lastOpened.size() > settings::VSOPSettings->value(settings::FILE_LASTOPENEDDEPTH, QVariant(5)).toUInt() )
//          {
//            lastOpened.pop_back();
//          }

//          for (iterator = lastOpened.begin(); iterator != lastOpened.end(); iterator++)
//          {
//            recentFileList.push_back(QString::fromStdString((*iterator).string()));
//          };

//          settings::VSOPSettings->setValue(settings::FILE_LASTOPENED, recentFileList);

//          // Populate the last opened file list into the menu.

//          populateRecentFileMenu();

        }
        catch (GCL::CError &err)
        {
          if (err.errorCode() == 0x000D)
          {
            msgBox.setText(tr("File Format Error."));
            msgBox.setInformativeText(tr("The file format chosen is unknown."));
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
          }
          else
          {
            LOGMESSAGE(warning, "Error opening file");
            LOGMESSAGE(warning, std::to_string(err.errorCode()) + " - " + err.errorMessage());

            msgBox.setText(tr("Error Opening file."));
            msgBox.setInformativeText(tr("There was an error opening the file."));
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
          }
        }
        catch (GCL::CCodeError &)
        {
          throw;    // Propogate code errors.
        }
        catch (GCL::CRuntimeAssert &)
        {
          throw;    // Propogate runtime assertions.
        }
        catch (ACL::CFITSException &exception)
        {
          msgBox.setText(tr("cfitsio Error while opening file."));
          msgBox.setInformativeText(QString::fromStdString(exception.errorMessage()));
          msgBox.setIcon(QMessageBox::Warning);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();

          exception.logErrorMessage();
        }

        catch(...)
        {
          LOGMESSAGE(warning, "Unknown exception while opening file.");
        };
      }
    }

    /// @brief Performs the action of loading an image.
    /// @param[in] filePath - The path and filename of the file to load.
    /// @throws std::bad_alloc
    /// @note Exceptions related to file open errors are caught and closed.
    /// @version 2016-04-17/GGB - Added catch() to catch CFITSException errors
    /// @version 2013-06-23/GGB - Added code to propogate code errors.
    /// @version 2013-01-21/GGB - Code taken from eventImageOpen()

    void CFrameWindow::loadFromFile(boost::filesystem::path &filePath)
    {
      QMessageBox msgBox;
      QStringList recentFileList;
      std::list<boost::filesystem::path>::iterator iterator;

      try
      {
        PAstroFile astroFile(std::make_shared<CAstroFile>(this, filePath));   // Create and load all the data from the file.

        GCL::logger::defaultLogger().logMessage(GCL::logger::info, "File: " + filePath.string() + " has been opened");

        imageCreateWindow(astroFile);

          // Update the last opened file list.

        lastOpened.remove(filePath);
        lastOpened.push_front(filePath);
        while (lastOpened.size() > settings::astroManagerSettings->value(settings::FILE_LASTOPENEDDEPTH, QVariant(5)).toUInt() )
        {
          lastOpened.pop_back();
        }

        for (iterator = lastOpened.begin(); iterator != lastOpened.end(); iterator++)
        {
          recentFileList.push_back(QString::fromStdString((*iterator).string()));
        };

        settings::astroManagerSettings->setValue(settings::FILE_LASTOPENED, recentFileList);

          // Populate the last opened file list into the menu.

        populateRecentFileMenu();
      }
      catch (GCL::CError &err)
      {
        if (err.errorCode() == 0x000D)
        {
          msgBox.setText(tr("File Format Error."));
          msgBox.setInformativeText(tr("The file format chosen is unknown."));
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
        }
        else
        {
          LOGMESSAGE(warning, "Error opening file");
          LOGMESSAGE(warning, std::to_string(err.errorCode()) + " - " + err.errorMessage());

          msgBox.setText(tr("Error Opening file."));
          msgBox.setInformativeText(tr("There was an error opening the file."));
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
        }
      }
      catch (GCL::CCodeError &)
      {
        throw;    // Propogate code errors.
      }
      catch (GCL::CRuntimeAssert &)
      {
        throw;    // Propogate runtime assertions.
      }
      catch (ACL::CFITSException &exception)
      {
        msgBox.setText(tr("cfitsio Error while opening file."));
        msgBox.setInformativeText(QString::fromStdString(exception.errorMessage()));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        exception.logErrorMessage();
      }

      catch(...)
      {
        LOGMESSAGE(warning, "Unknown exception while opening file.");
      };
    }

    /// @brief Opens a file from the recent files list.
    /// @throws None
    /// @version 2013-01-22/GGB - Function created.

    void CFrameWindow::openRecentFile()
    {
      QAction *action = qobject_cast<QAction *>(sender());
      boost::filesystem::path filePath;

      if (action)
      {
          // The following is a workaround as otherwise errors are thrown by the string creation/deletion functions with VC++

        std::string actionString = action->data().toString().toStdString();
        filePath = boost::filesystem::path(actionString);
        loadFromFile(filePath);
      };
    }

    /// @brief Populates the recent File Menu with the relevant information.
    /// @throws None.
    /// @version 2013-01-22/GGB - Function created.

    void CFrameWindow::populateRecentFileMenu()
    {
      size_t  nIndex = 0;
      std::list<boost::filesystem::path>::const_iterator iterator;

      for (iterator = lastOpened.begin(); iterator != lastOpened.end(); iterator++)
      {
        recentFileActions[nIndex]->setText(QString::fromStdString((*iterator).string()));
        recentFileActions[nIndex]->setStatusTip(QString::fromStdString((*iterator).string()));
        recentFileActions[nIndex]->setData(QVariant(QString::fromStdString((*iterator).string())));
        recentFileActions[nIndex]->setVisible(true);
        nIndex++;
      };

        // nIndex must not be zeroed as we wish to hide the unused action items.

      for(; nIndex < recentFileActions.size(); nIndex++)
      {
        recentFileActions[nIndex]->setVisible(false);
      };
    }

    /// @brief Reads settings for the frame window
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2013-07-27/GGB - Added code to restore the state and geometry.
    /// @version 2013-06-20/GGB - Added check to maximise the window. Changed setting values.
    /// @version 2013-05-10/GGB - Added code to update the astrometry and photometry display indicators.
    /// @version 2010-05-22/GGB - Function created.

    void CFrameWindow::readSettings()
    {
      QVariant vGeometry, vState;

      vGeometry = settings::astroManagerSettings->value(settings::FRAME_GEOMETRY);
      vState = settings::astroManagerSettings->value(settings::DW_STATE);

      if (!vGeometry.isNull())
      {
        restoreGeometry(vGeometry.toByteArray());
      };

      if (!vState.isNull())
      {
        restoreState(vState.toByteArray());
      };

      bool maximised = settings::astroManagerSettings->value(settings::FRAME_MAXIMISED, QVariant(false)).toBool();
      QPoint pos = settings::astroManagerSettings->value(settings::FRAME_POSITION, QPoint(200, 200)).toPoint();
      QSize size = settings::astroManagerSettings->value(settings::FRAME_SIZE, QSize(400, 400)).toSize();
      move(pos);
      resize(size);

      if (maximised)
      {
        showMaximized();
      };

        // Read menu settings

      if (settings::astroManagerSettings->value(settings::PHOTOMETRY_DISPLAYINDICATORS, QVariant(true)).toBool())
      {
        menuActions[IDA_VIEW_PHOTOMETRY]->setChecked(true);
      }
      else
      {
        menuActions[IDA_VIEW_PHOTOMETRY]->setChecked(false);
      };

      if (settings::astroManagerSettings->value(settings::ASTROMETRY_DISPLAYINDICATORS, QVariant(true)).toBool())
      {
        menuActions[IDA_VIEW_ASTROMETRY]->setChecked(true);
      }
      else
      {
        menuActions[IDA_VIEW_ASTROMETRY]->setChecked(false);
      };
    }

    /// @brief Sets the base state of the actions.
    /// @details All actions that must be enabled are enabled and all actions that need to be disabled are disabled.
    ///          This is the state that should be present when no windows are open.
    /// @throws None.
    /// @version 2017-07-02/GGB - Changed the underlying storage to a std::map with std::unique_ptr
    /// @version 2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    /// @version 2016-04-24/GGB - Bug# 1574157 Added code to diable the astrometry and photometry indicators.
    /// @version 2015-08-21/GGB - (Bug 88) Added code to disable colour menu items.
    /// @version 2013-08-18/GGB - Added function to load photometry target list.
    /// @version 2013-07-03/GGB - Added code for Transform menu. (Bug #1197352)
    /// @version 2013-06-02/GGB - Added view menu actions.
    /// @version 2013-05-11/GGB - Added IDA_FILE_EXPORT_CSV to actions.
    /// @version 2013-03-29/GGB - Function created.

    void CFrameWindow::setBaseActionStates()
    {
        // File menu

      menuActions[IDA_FILE_SAVE]->setEnabled(false);
      menuActions[IDA_FILE_SAVEAS]->setEnabled(false);
      if (!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
      {
        menuActions[IDA_FILE_SAVE_DATABASE]->setEnabled(false);
      };
      menuActions[IDA_FILE_EXPORTIMAGE_DNG]->setEnabled(false);
      menuActions[IDA_FILE_EXPORTIMAGE_JPEG]->setEnabled(false);
      menuActions[IDA_FILE_EXPORTIMAGE_TIFF]->setEnabled(false);
      menuActions[IDA_FILE_EXPORTIMAGE_PNG]->setEnabled(false);
      menuActions[IDA_FILE_EXPORT_CSV]->setEnabled(false);
      menuActions[IDA_FILE_BATCHCONVERT]->setEnabled(false);
      menuActions[IDA_FILE_PRINT]->setEnabled(false);
      menuActions[IDA_FILE_PRINTPREVIEW]->setEnabled(false);
      menuActions[IDA_FILE_PRINTSETUP]->setEnabled(false);

        // Edit Menu

      menuActions[IDA_EDIT_ANNOTATEIMAGE]->setEnabled(false);

        // View Menu

      menuActions[IDA_VIEW_ZOOM11]->setEnabled(false);
      menuActions[IDA_VIEW_ZOOMIN]->setEnabled(false);
      menuActions[IDA_VIEW_ZOOMOUT]->setEnabled(false);
      menuActions[IDA_VIEW_ZOOMALL]->setEnabled(false);
      menuActions[IDA_VIEW_ZOOMSELECTION]->setEnabled(false);
      menuActions[IDA_VIEW_ASTROMETRY]->setEnabled(false);      // Bug# 1574157
      menuActions[IDA_VIEW_PHOTOMETRY]->setEnabled(false);      // Bug# 1574157

        // Preperation menu

      menuActions[IDA_CALIBRATE_SINGLEIMAGE]->setEnabled(false);
      menuActions[IDA_CALIBRATE_MULTIIMAGES]->setEnabled(true);

        // Analysis menu

      menuActions[IDA_ANALYSIS_EXTRACTOBJECTS]->setEnabled(false);
      menuActions[IDA_ANALYSIS_LOADOBJECTS]->setEnabled(false);
      menuActions[IDA_ASTROMETRY_LOADTARGETLIST]->setEnabled(false);
      menuActions[IDA_PHOTOMETRY_LOADTARGETLIST]->setEnabled(false);

        // Transform menu

      menuActions[IDA_TRANSFORM_FLIP]->setEnabled(false);
      menuActions[IDA_TRANSFORM_FLOP]->setEnabled(false);
      menuActions[IDA_TRANSFORM_RESAMPLE]->setEnabled(false);
      menuActions[IDA_TRANSFORM_BINPIXELS]->setEnabled(false);
      menuActions[IDA_TRANSFORM_FLOAT]->setEnabled(false);
      menuActions[IDA_TRANSFORM_CROP]->setEnabled(false);
      menuActions[IDA_TRANSFORM_ROTATE]->setEnabled(false);

      // IA Colour Menu

      menuActions[IDA_IA_COLOUR_SPLIT_RGB]->setEnabled(false);
      menuActions[IDA_IA_COLOUR_SPLIT_LRGB]->setEnabled(false);
      menuActions[IDA_IA_COLOUR_SPLIT_GRAYSCALE]->setEnabled(false);
    }

    /// @brief Function to setup the user interface.
    /// @details Called from the constructor of the class.
    /// @throws GCL::CRuntimeAssert
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2015-08-08/GGB - Changed if(...) checks to runtime assertions for forms and children.
    /// @version 2011-05-28/GGB - Function created.

    void CFrameWindow::setupUI()
    {
      QUiLoader loader;

        // Load the window layout from the resource

      QFile file(":/forms/windowMDIFrame.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/windowMDIFrame.ui.");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      RUNTIME_ASSERT(astroManager, formWidget, "Widget not found for the MDU frame.");

      setCentralWidget(formWidget);


        // Create the MDI area and set up the attributes.

      ASSOCIATE_CONTROL(mdiArea, formWidget, "mdiArea", QMdiArea);

      mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(subWindowActivated(QMdiSubWindow*)));


        // Connect the MDI area to the relevant signals.

      windowMapper = new QSignalMapper(this);
      connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));
    }

    /// @brief Responds to the subWindowActivated signal from the MDIArea.
    /// @details  The sub window is informed that it is going to activate and then the MDI area is informed that the sub window
    ///           is about to activate. The MDI area signal is connected to this function.<br>
    ///           The function checks the window class of the new child window. If the windows class has changed, then the
    ///           functions to change the dockwidgets, menus and toolbars for the new windows class can be called.
    /// @note 1: subWindow == nullptr implies that there are no active child windows.
    /// @param[in] subWindow - THe subWindow that is activating (nullptr is allowed)
    /// @throws None.
    /// @version 2017-08-13/GGB - Updated to support window classes.
    /// @version 2013-07-28/GGB - Function created.

    void CFrameWindow::subWindowActivated(QMdiSubWindow *subWindow)
    {
      auto mdiSubWindow = dynamic_cast<CMdiSubWindow *>(subWindow);

      if (mdiSubWindow)
      {
        mdiSubWindow->windowActivating();   // Always call.

        if (currentWindowClass != mdiSubWindow->getWindowClass())
        {
          currentWindowClass = mdiSubWindow->getWindowClass();
          switch (currentWindowClass)
          {
            case CMdiSubWindow::WC_NULL:
            {
              activateWindowClassNull();
              break;
            }
            case CMdiSubWindow::WC_IMAGE:
            {
              activateWindowClassImage();
              break;
            }
            case CMdiSubWindow::WC_WEATHER:
            {
              activateWindowClassWeather();
              break;
            };
            case CMdiSubWindow::WC_ANALYSE:
            case CMdiSubWindow::WC_CALIBRATE:
            case CMdiSubWindow::WC_PLANNING:
              break;
            default:
            {
              CODE_ERROR(astroManager);
              break;
            };
          }
        }
      }
      else
      {
        currentWindowClass = CMdiSubWindow::WC_NULL;
        activateWindowClassNull();
      }
    }

    /// @brief Create, or update the recent file actions list.
    /// @param[in] nActions - Number of actions to add.
    /// @throws std::bad_alloc
    /// @version 2017-07-13/GGB - Updated to use subMenus std::map
    /// @version 2013-07-28/GGB - Changed parameter and nIndex to unsigned int.
    /// @version 2013-01-22/GGB - Function created.

    void CFrameWindow::updateRecentFileMenu(unsigned int nActions)
    {
      unsigned int nIndex;
      QAction *newAction;

      if (nActions > recentFileActions.size())
      {
          // More actions, need to create the additional actions and push them to the back of the vector.

        for (nIndex = recentFileActions.size(); nIndex <= nActions; nIndex++)
        {
          newAction = new QAction(this);
          newAction->setVisible(false);
          connect(newAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
          recentFileActions.push_back(newAction);

          subMenus[IDSM_NULL_FILE]->addAction(newAction);
          subMenus[IDSM_IMAGE_FILE]->addAction(newAction);
        };
      }
      else if (nActions < recentFileActions.size())
      {
          // Less actions, need to delete the actions at the end of the list. and remove them.

        for (nIndex = recentFileActions.size() - 1; nIndex > nActions; nIndex--)
        {
          if (recentFileActions[nIndex])
          {
            subMenus[IDSM_NULL_FILE]->removeAction(recentFileActions[nIndex]);
            subMenus[IDSM_IMAGE_FILE]->removeAction(recentFileActions[nIndex]);

            delete recentFileActions[nIndex];
            recentFileActions[nIndex] = nullptr;
          };
          recentFileActions.pop_back();
        };
      };
    }

  } // namespace mdiframe
}	// namespace AstroManager
