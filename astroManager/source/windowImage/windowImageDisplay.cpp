//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:								ImageDisplay
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Boost, GCL, ACL, SOFA, Qt
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
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2010-10-17 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../include/windowImage/windowImageDisplay.h"

  // Standard library

#include <list>

  // Qt Framework

#include <QtUiTools/QtUiTools>

  // astroManager include files

#include "../../include/database/databaseARID.h"
#include "../../include/database/databaseATID.h"
#include "../../include/dialogs/dialogBinPixels.h"
#include "../../include/dialogs/dialogExportAsJPEG.h"
#include "../../include/dialogs/dialogExportAsPNG.h"
#include "../../include/dialogs/dialogFindStars.h"
#include "../../include/dialogs/dialogImageCalibration.h"
#include "../../include/dialogs/dialogImageCrop.h"
#include "../../include/dialogs/dialogImageDetails.h"
#include "../../include/dialogs/dialogImageFloat.h"
#include "../../include/dialogs/dialogImageResample.h"
#include "../../include/dialogs/dialogWeatherEdit.h"
#include "../../include/dockWidgets/dockWidgetAstrometry.h"
#include "../../include/dockWidgets/dockWidgetHistogram.h"
#include "../../include/dockWidgets/dockWidgetImageInformation.h"
#include "../../include/dockWidgets/dockWidgetMagnify.h"
#include "../../include/dockWidgets/dockWidgetNavigator.h"
#include "../../include/dockWidgets/dockWidgetPhotometry.h"
#include "../../include/settings.h"
#include "../../include/astroManager.h"

  // Miscellaneous library header files.

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <GCL>
#include <PCL>
#include "sofam.h"

namespace astroManager
{
  namespace imaging
  {
    int const SEA_FINDSTARS = 1;
    int const SEA_SIMPLEXY  = 2;

    //*****************************************************************************************************************************
    //
    // CImageWindow
    //
    // Class to display and measure astronomical images.
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @version 2016-03-26/GGB - Changed implicit cast to static_cast.
    /// @version 2015-08-01/GGB - Change HDB name code to use QString::fromStdString()
    /// @version 2013-06-22/GGB - Removed the code to set the location
    /// @version 2013-05-08/GGB - Removed bAstrometry and bPhotometry and replaced with functions calls.
    /// @version 2013-03-30/GGB - Added bPhotometryHDB to determine if the Photometry HDB exists.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-02-05/GGB - Added bAstrometryHDB to determine if the Astrometry HDB exists.
    /// @version 2011-03-14/GGB - Changed class to use CAstroFile with no direct references to CCFits
    /// @version 2010-10-30/GGB - Function Created

    CImageWindow::CImageWindow(std::shared_ptr<CAstroFile> newAstroFile, QWidget *parent) :
      CAstroImageWindow(parent), controlImage(this, newAstroFile)
    {
      ACL::DHDBStore::size_type nIndex;
      setAttribute(Qt::WA_DeleteOnClose);

      createActions();
      createMenu();

        // Setup all the information for the widgets

      SetupUI();

      setWindowTitle(QString::fromStdString(newAstroFile->getImageName() + "[*]"));

        // Populate the combo box with the names of all the HDB's (Header and Data Block)
        // An HDB is the same as an HDU if a FITS file is being accessed.
        // If a non-fits file is being accesed, there should only be one HDB
        // Depending on the type of the file being accessed, the HDB may contain different information.
        // We need the number of HDB's, and each HDB's name and type.

      for (nIndex = 0; nIndex < controlImage.astroFile->HDBCount(); nIndex++)
      {
        cbHDU->addItem(QString::fromStdString(controlImage.astroFile->HDBName(nIndex)));  // Side effect - Calls eventUpdateHDU
      };

      cbHDU->setCurrentIndex(0);
    }

    /// @brief Ensure that all dynamically allocated memory is deleted on exit.
    /// @throws None.
    /// @details The astroFile object is assumbed to be owned by this class, thus it must clean up the memory before exiting.
    /// @version 2013-06-09/GGB - Not necessary to delete astroFile. Will be deleted by control image or smart pointer.
    /// @version 2013-03-10/GGB - Delete the astroFile on exit.
    /// @version 2010-10-17/GGB - Function created.

    CImageWindow::~CImageWindow()
    {
    }

    /// @brief Loads the astrometry targets.
    /// @version 2017-09-23/GGB - Updated to use CAngle
    /// @version 2017-07-03/GGB - Updated to reflect new dockwidget storage method.

    void CImageWindow::astrometryLoadTargets()
    {
      dockwidgets::CAstrometryDockWidget &adw =
          dynamic_cast<dockwidgets::CAstrometryDockWidget &>(
            dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL));

      std::vector<astrometry::PAstrometryObservation> astrometryTargets;
      std::vector<astrometry::PAstrometryObservation>::iterator targetIterator;

        // Check if there is WCS information present in the current image. If there is no WCS information present in the image,
        // then the function should return with an error to the user.

      LOGMESSAGE(info, "Starting Load Astrometry Targets...");

      if (controlImage.astroFile->hasWCSData(controlImage.currentHDB))
      {
          // There is WCS information present in the current image. Query the user for the filename of the Astrometry file.

        QString szFileName = QFileDialog::getOpenFileName(this, tr("Open Astrometry Target File"),
          settings::astroManagerSettings->value(settings::ASTROMETRY_TARGET_DIRECTORY, QVariant(0)).toString(), EXTENSION_CSV);

        if (!szFileName.isEmpty())
        {
          size_t lineNumber = 0;

            // Have the file name. Open the file and read all the information into an array, while parsing the data

          try
          {
            std::ifstream csvFile(szFileName.toStdString());
            std::string szLine, szValue;

            while (csvFile.good())
            {
              FP_t RA, Dec;

              std::getline(csvFile, szLine);
              if (szLine.size() != 0)
              {
                astrometry::PAstrometryObservation currentTarget(new astrometry::CAstrometryObservation);

                  // Now parse the data.

                size_t comma1, comma2, comma3;
                ACL::CAstronomicalCoordinates coordinates;

                comma1 = szLine.find(',', 0);
                comma2 = szLine.find(',', comma1 + 1);
                comma3 = szLine.find(',', comma2 + 1);

                currentTarget->objectName(szLine.substr(0, comma1));    // Object Name

                szValue = szLine.substr(comma1 + 1, comma2 - comma1 - 1);     // RA
                boost::trim(szValue);
                RA = boost::lexical_cast<FP_t>(szValue);

                szValue = szLine.substr(comma2 + 1, comma3 - comma2 - 1);
                boost::trim(szValue);
                Dec = boost::lexical_cast<FP_t>(szValue);

                coordinates(MCL::angle_t(RA, MCL::AF_Dd), MCL::angle_t(Dec, MCL::AF_Dd));

                currentTarget->observedCoordinates(coordinates);

                astrometryTargets.push_back(currentTarget);
              };

              ++lineNumber;
            };
          }
          catch(...)
          {
            QMessageBox::information(this, tr("Error while processing file."),
                                     tr("Error while processing the file."),
                                     QMessageBox::Ok, QMessageBox::Ok);
            LOGMESSAGE(warning, "Error while loading photometry targets in line: " + std::to_string(lineNumber) + ".");
            return;
          };

          // Have all the data. Convert the RA/Dec to CCD coordinates

          for (targetIterator = astrometryTargets.begin(); targetIterator != astrometryTargets.end(); ++targetIterator)
          {
            std::optional<MCL::TPoint2D<FP_t>> result = controlImage.astroFile->wcs2pix(controlImage.currentHDB,
                                                                                          *(*targetIterator)->observedCoordinates());
            if (result)
            {
              (*targetIterator)->CCDCoordinates(*result);
            }
            else
            {
              LOGMESSAGE(warning, (*targetIterator)->objectName() + ": Coordinates not on image. Deleting Astrometry target.");
              targetIterator = astrometryTargets.erase(targetIterator);
            };
          }

          // Have all the CCD coordinates, now centroid the coordinates, and add to the photometry list.

          size_t targetCount = 0;

          for (targetIterator = astrometryTargets.begin(); targetIterator != astrometryTargets.end(); ++targetIterator)
          {
            MCL::TPoint2D<AXIS_t> point = (*targetIterator)->CCDCoordinates();

            std::optional<MCL::TPoint2D<ACL::FP_t> > centroid =
                controlImage.astroFile->centroid(controlImage.currentHDB, point,
                                                 settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_RADIUS,
                                                                               QVariant(20)).toInt(),
                                                 settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                               QVariant(3)).toInt());

            if (centroid)
            {
              ACL::SPAstrometryObservation existingObject;

                // Check the list for another target that is close.

              bool bClose = false;

              if (controlImage.astroFile->hasAstrometryHDB())
              {
                existingObject = controlImage.astroFile->astrometryObjectFirst();

                while ( (existingObject) && !bClose)
                {
                  //bClose = existingObject->isClose(point, adw->getRadius2());
                  existingObject = controlImage.astroFile->astrometryObjectNext();
                };
              };

              if (!bClose)
              {

                  // Object not already in the list. Add object to the list.

                try
                {
                  astrometry::PAstrometryObservation astrometryObject;

                  astrometryObject.reset(new astrometry::CAstrometryObservation());
                  astrometryObject->objectName( (*targetIterator)->objectName());
                  astrometryObject->CCDCoordinates(*centroid);
                  astrometryObject->observedCoordinates( *(*targetIterator)->observedCoordinates() );

                    // Add the Astrometry observation to the list.

                  if ( !controlImage.astroFile->hasAstrometryHDB() )
                  {
                    ACL::CHDBAstrometry *phdb = controlImage.astroFile->createAstrometryHDB();
                    phdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
                    phdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
                  };

                  ++targetCount;
                  controlImage.astroFile->astrometryObjectAdd(astrometryObject);
                  controlImage.astrometryObservations.push_back(astrometryObject);          // Add the objects into the vector.

                    // Now draw the astro indicator

                  changeAstrometrySelection(astrometryObject);

                    // Update the information in the dockwidget.

                  adw.addNewObject(std::dynamic_pointer_cast<astrometry::CAstrometryObservation>(astrometryObject));
                  //adw->displayAstrometry(boost::dynamic_pointer_cast<dockwidgets::CAstrometryObservation>(astrometryObject));

                  astrometryReferenceAdd(astrometryObject);

                  LOGMESSAGE(info, astrometryObject->objectName() + ": Added to photometry list sucesfully.");
                }
                catch(...)
                {
                  LOGMESSAGE(warning, "Unable to add " + (*targetIterator)->objectName() + ". Error while performing astrometry.");
                };
              };
            }
            else
            {
              LOGMESSAGE(warning, "Unable to add " + (*targetIterator)->objectName() + ". Could not find centroid.");
            };
          };

            // Indicate the number of points applied sucesfully.

          LOGMESSAGE(info, "Added " + std::to_string(targetCount) + " astrometry targets to image.");
          LOGMESSAGE(info, "Load Astrometry targets completed.");

            // Update the image and window characteristics.

          controlImage.astroFile->isDirty(true);
          controlImage.astroFile->hasData(true);
          updateWindowTitle();
        };
      }
      else
      {
        QMessageBox::information(this, tr("There is no WCS information for this image."),
                                 tr("Without WCS information, it is not possible to load and apply a list of astrometry targets."),
                                 QMessageBox::Ok, QMessageBox::Ok);
        LOGMESSAGE(warning, "Load Astrometry Targets Error: No WCS information for image.");
      };
    }

    /// @brief Function called when the dockwidget needs to add a new reference to the image window.
    /// @details Using this function as the entry point allows the FITS headers and information to be updated without having to
    ///          add special calls back to the CImageWidow.
    ///   @li Get all the information needed to create the astrometryObservation
    ///   @li Call the astroFile to add the observation to the astroFile.
    ///   @li If the astrometry HDB is created, then the combo box needs to be updated and a tab needs to be opened to display the
    ///       astrometry information
    ///   @li The astrometry information in the astrometry HDB needs to be updated
    /// @version 2017-09-23/GGB - Update to use CAngle
    /// @version 2013-08-16/GGB - Corrected bug with the RA/DEC display (Bug #1213076)
    /// @version 2013-02-05/GGB - Added bAstrometryHDB to determine if the Astrometry HDB exists.
    /// @version 2013-02-03/GGB - Moved code to DockWidgetAstrometry.
    /// @version 2012-01-21/GGB - Function created

    void CImageWindow::astrometryReferenceAdd(astrometry::PAstrometryObservation astrometryObject)
    {
      int nColumn = 0, nRow;

        // Need to add the astrometry HDU to the list of available HDU's?

      if ( cbHDU->findText(QString::fromStdString(ACL::ASTROMANAGER_HDB_ASTROMETRY)) == -1)
      {
        cbHDU->addItem(QString::fromStdString(ACL::ASTROMANAGER_HDB_ASTROMETRY));
      }

        // Add the items into the table for references

      nRow = tableWidgetAstrometry->rowCount();
      tableWidgetAstrometry->insertRow(nRow);
      tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString::fromStdString(astrometryObject->objectName())));
      tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("(%1, %2)").
                                                                           arg(astrometryObject->CCDCoordinates().x()).
                                                                           arg(astrometryObject->CCDCoordinates().y())));
      if (astrometryObject->observedCoordinates())
      {
        tableWidgetAstrometry->setItem(nRow, nColumn++,
                                       new QTableWidgetItem(QString::fromStdString(astrometryObject->observedCoordinates()->RA().A2SHMS())));
        tableWidgetAstrometry->setItem(nRow, nColumn++,
                                       new QTableWidgetItem(QString::fromStdString(astrometryObject->observedCoordinates()->DEC().A2SDMS())));
      }
      else
      {
        tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem("--h--'--"""));
        tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN %"--'--"""));
      };
    }

    /// @brief Function called when the astrometry tab needs to be activated.
    /// @throws RUNTIME_ASSERT
    /// @version  2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    /// @version 2016-04-24/GGB - Bug# 1574157 - Ensure the view photometry and astrometry are in the correct state.
    /// @version 2015-01-04/GGB - Corrected incorrect enabled values for the File Menu.
    /// @version 2013-05-12/GGB - Added the export csv option.
    /// @version 2013-04-27/GGB - Function created.

    void CImageWindow::astrometryTabActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw,  "Parent widget cannot be nullptr.");

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_DNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_JPEG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_PNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_TIFF)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORT_CSV)->setEnabled(true);

        // View Menu

      pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOM11)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->setEnabled(false);

        // Process menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);

        // Analyse actions

      pw->getAction(mdiframe::IDA_ANALYSIS_EXTRACTOBJECTS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(false);

        // IA Colour Menu

      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_RGB)->setEnabled(false);
      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_LRGB)->setEnabled(false);
      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_GRAYSCALE)->setEnabled(false);
    }

    // Called when the ASCII table tab is activating.
    // Ensures that all menu items are set up correctly.
    //
    /// @version  2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    // 2015-01-04/GGB - Corrected incorrect enabled values for the File Menu.
    // 2013-05-12/GGB - Added the export csv option.
    // 2013-03-03/GGB - Added additional items to be disabled.
    // 2011-06-26/GGB - Function created.

    void CImageWindow::atableTabActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw,  "Parent widget cannot be nullptr.");

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_DNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_JPEG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_PNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_TIFF)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORT_CSV)->setEnabled(true);

        // View Menu

      pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOM11)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);

        // Process menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);

        // Analyse actions

      pw->getAction(mdiframe::IDA_ANALYSIS_EXTRACTOBJECTS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(false);

        // IA Colour Menu

      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_RGB)->setEnabled(false);
      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_LRGB)->setEnabled(false);
      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_GRAYSCALE)->setEnabled(false);
    }

    /// @brief Handles the menu action to bin pixels
    /// @throws None.
    /// @version 2013-06-23/GGB - Added code to to reset the blackPoint and white Point. (Bug #1193740)
    /// @version 2013-05-25/GGB - Added support for View | Magnify and View | Navigator
    /// @version 2013-02-02/GGB - Added support for the histogram widget.
    /// @version 2013-01-27/GGB - Added logging of the command.
    /// @version 2011-06-10/GGB - Function created

    void CImageWindow::binPixels()
    {
      dialogs::CBinPixelsDialog dlg;
      ACL::AXIS_t ox, oy;

      if (dlg.exec() == QDialog::Accepted)
      {
        ox = controlImage.astroFile->imageWidth(controlImage.currentHDB);
        oy = controlImage.astroFile->imageHeight(controlImage.currentHDB);

        controlImage.astroFile->binPixels(controlImage.currentHDB, dlg.getBinSize());

        controlImage.blackPoint = controlImage.astroFile->blackPoint();
        controlImage.whitePoint = controlImage.astroFile->whitePoint();

        LOGMESSAGE(info, "Binned Image: " + controlImage.astroFile->getImageName() + ". Original Size: " + std::to_string(ox) +
                   " x " + std::to_string(oy) + ". New Size: " + std::to_string(controlImage.astroFile->imageWidth()) +
                   " x " + std::to_string(controlImage.astroFile->imageHeight()) + ".");

        historyUpdate();

        redrawImage();
        gsImage->setSceneRect(gsImage->itemsBoundingRect());
        zoomAll();

        if (controlImage.astroFile->isDirty())
        {
          updateWindowTitle();
        };
      };
    }

    // Called when the binary table tab is activating.
    // Ensures that all menu actions are set up correctly.
    //
    /// @version  2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    // 2015-01-04/GGB - Corrected incorrect enabled values for the File Menu.
    // 2013-05-12/GGB - Added the EXPORT CSV option.
    // 2011-06-26/GGB - Function created.

    void CImageWindow::btableTabActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw,  "Parent widget cannot be nullptr.");

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_DNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_JPEG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_PNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_TIFF)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORT_CSV)->setEnabled(true);

        // View Menu

      pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOM11)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);

        // Process menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);

        // Analyse actions

      pw->getAction(mdiframe::IDA_ANALYSIS_EXTRACTOBJECTS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(false);

        // IA Colour Menu

      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_RGB)->setEnabled(false);
      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_LRGB)->setEnabled(false);
      pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_GRAYSCALE)->setEnabled(false);
    }

    /// Performs a calibration of the selected window.
    /// 1) Check that a window is selected as the active window that can be calibrated.
    /// 2) Bring up the dialog for a single image calibraiton.
    /// 3) Allow the user to select dark frame etc.
    /// 4) Perform the calibration
    //
    // 2013-02-02/GGB - Added support for the histogram widget.
    // 2012-01-27/GGB - Function created.

    void CImageWindow::calibrateImage()
    {
      std::unique_ptr<ACL::SCalibrateImage> calibrateImage(new ACL::SCalibrateImage);
      QMessageBox msgBox;

      if (controlImage.astroFile->HDBType(controlImage.currentHDB) == ACL::BT_IMAGE)
      {
          // Calibration dialog.

        dialogs::CImageCalibrationDialog dlg(calibrateImage.get());

        if (dlg.exec() == QDialog::Accepted)
        {
            // The user has provided valid information.

          try
          {
            controlImage.astroFile->calibrateImage(calibrateImage.get());

            historyUpdate();

              // Redraw the image

            redrawImage();

            if (controlImage.astroFile->isDirty())
              updateWindowTitle();
          }
          catch(GCL::CError &error)
          {
            msgBox.setText(QString("Error while calibrating image. %1").arg(error.errorCode(), 4, 16));
            msgBox.setInformativeText(error.errorMessage().c_str());
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
          }
          catch(...)
          {
            msgBox.setText(tr("Error while calibrating image."));
            msgBox.setInformativeText(tr("???."));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
          };
        };
      }
      else
        CODE_ERROR(astroManager);  // Should not be able to get here. The windows should be disabling the item to prevent gettign here.
    }

    /// @brief Called when a new Astrometry object is being selected.
    /// @details Redraws the Astrometry indicator in the new colours.
    ///
    /// @version  2013-18-25/GGB - Function created.

    void CImageWindow::changeAstrometrySelection(astrometry::PAstrometryObservation newSelection)
    {
      QPen pen;
      QColor const selectedColour = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_SELECTEDCOLOUR,
                                                                  QVariant(QColor(Qt::yellow))).value<QColor>();
      QColor const normalColour = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_COLOUR,
                                                                QVariant(QColor(Qt::red))).value<QColor>();

      if ( (controlImage.currentAstrometrySelection) && (newSelection != controlImage.currentAstrometrySelection) )
      {
        pen.setColor(normalColour);
        if (controlImage.currentAstrometrySelection->group)
        {
          delete controlImage.currentAstrometrySelection->group;
          controlImage.currentAstrometrySelection->group = nullptr;
          controlImage.currentAstrometrySelection->text = nullptr;
        };
        drawAstrometryIndicator(controlImage.currentAstrometrySelection, pen);
        gsImage->addItem(controlImage.currentAstrometrySelection->group);

        pen.setColor(selectedColour);
        if (newSelection->group)
        {
          delete newSelection->group;
          newSelection->group = nullptr;
          newSelection->text = nullptr;
        };
        drawAstrometryIndicator(newSelection, pen);
        gsImage->addItem(newSelection->group);
        controlImage.currentAstrometrySelection = newSelection;
      }
      else if (!controlImage.currentAstrometrySelection)
      {
        pen.setColor(selectedColour);
        if (newSelection->group)
        {
          delete newSelection->group;
          newSelection->group = nullptr;
          newSelection->text = nullptr;
        };
        drawAstrometryIndicator(newSelection, pen);
        gsImage->addItem(newSelection->group);
        controlImage.currentAstrometrySelection = newSelection;
      };
    }

    /// @brief Called when a new Photometry object is being selected.
    /// @details Redraws the photometry indicator in the new colours.
    ///
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2010-11-13/GGB - Function created.

    void CImageWindow::changePhotometrySelection(photometry::PPhotometryObservation newSelection)
    {
      TRACEENTER;

      QPen pen;
      QColor const selectedColour = settings::astroManagerSettings->value(settings::PHOTOMETRY_INDICATOR_SELECTEDCOLOUR,
                                                                  QVariant(QColor(Qt::yellow))).value<QColor>();
      QColor const normalColour = settings::astroManagerSettings->value(settings::PHOTOMETRY_INDICATOR_COLOUR,
                                                                QVariant(QColor(Qt::red))).value<QColor>();

      if ( (controlImage.currentPhotometrySelection) && (newSelection != controlImage.currentPhotometrySelection) )
      {
        pen.setColor(normalColour);

          // Delete the current indicator (hilighted colour)

        if (controlImage.currentPhotometrySelection->group)
        {
          delete controlImage.currentPhotometrySelection->group;
          controlImage.currentPhotometrySelection->group = nullptr;
          controlImage.currentPhotometrySelection->text = nullptr;
        };

          // Create the current indicator (normal colour)

        drawPhotometryIndicator(controlImage.currentPhotometrySelection, pen);
        gsImage->addItem(controlImage.currentPhotometrySelection->group);

          // Draw the hilighed indicator for the new group.
          // Delete the existing indicator for the object (non hilighted)

        pen.setColor(selectedColour);
        if (newSelection->group)
        {
          delete newSelection->group;
          newSelection->group = nullptr;
          newSelection->text = nullptr;
        };

          // Draw the new (hilighted indicator for the object)

        drawPhotometryIndicator(newSelection, pen);
        gsImage->addItem(newSelection->group);
        controlImage.currentPhotometrySelection = newSelection;
      }
      else if (!controlImage.currentPhotometrySelection)
      {
        pen.setColor(selectedColour);
        if (newSelection->group)
        {
          delete newSelection->group;
          newSelection->group = nullptr;
          newSelection->text = nullptr;
        };
        drawPhotometryIndicator(newSelection, pen);
        gsImage->addItem(newSelection->group);
        controlImage.currentPhotometrySelection = newSelection;
      };

      TRACEEXIT;
    }

    /// @brief Processes the close event. Lets the MDI window know that this sub-window has been closed.
    /// @throws
    /// @version 2017-07-03/GGB - Updated to use new dockwidgets storage.
    /// @version 2013-07-18/GGB - Added setEnabled to photometry dock widget code block (Bug #1198192)
    /// @version 2013-05-25/GGB - Added support for View | Navigator and View | Magnifier
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-02-02/GGB - Added support to ensure that the data is saved before the window is closed.
    /// @version 2013-02-02/GGB - Added support for the histogram widget.
    /// @version 2011-06-27/GGB - Added support for the astrometry and controller dock widgets.
    /// @version 2010-10-30/GGB - Function created.

    void CImageWindow::closeEvent(QCloseEvent *event)
    {
      if (saveBeforeClose())
      {
        imageChange(nullptr);
        dockwidgets::CDockWidgetImage::setCurrentImage(nullptr);

        auto frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        dynamic_cast<dockwidgets::CDockWidgetImage &>(frameWindow->getDockWidget(mdiframe::IDDW_IMAGECONTROL)).setEnabled(false);

        dockwidgets::CAstrometryDockWidget &adw = dynamic_cast<dockwidgets::CAstrometryDockWidget &>(frameWindow->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL));
        adw.setEnabled(false);
        adw.imageDeactivating();

        dockwidgets::CPhotometryDockWidget &pdw = dynamic_cast<dockwidgets::CPhotometryDockWidget &>(frameWindow->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));
        pdw.setEnabled(false);
        pdw.imageDeactivating();

        dynamic_cast<dockwidgets::CHistogram &>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_HISTOGRAM)).setEnabled(false);
        dynamic_cast<dockwidgets::CDockWidgetMagnify &>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_MAGNIFY)).imageDeactivating();
        dynamic_cast<dockwidgets::CDockWidgetNavigator &>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_NAVIGATOR)).setEnabled(false);

        frameWindow->childClosing(this);  // Let the parent know.

        event->accept();
      }
      else
      {
        event->ignore();
      }
    }

    /// @brief Creates the actions that are used by the popup window.
    /// @throws None.
    /// @version 2017-08-25/GGB - Function created.

    void CImageWindow::createActions()
    {
      menuActions[IDA_IMAGEDETAILS] = new QAction("Image Details", this);
      menuActions[IDA_IMAGEDETAILS]->setStatusTip(tr("Edit the image details"));
      menuActions[IDA_IMAGEDETAILS]->setCheckable(true);
      //connect(menuActions[IDA_IMAGEDETAILS], SIGNAL(triggered()), this, SLOT(eventMagnify11()));
    }

    /// @brief Create the popup menu
    /// @throws None.
    /// @version 2017-08-25/GGB - Function created.

    void CImageWindow::createMenu()
    {
      popupMenu = new QMenu();

      comboBoxQuality = new QComboBox;
      QWidgetAction *widgetAction = new QWidgetAction(nullptr);
      widgetAction->setDefaultWidget(comboBoxQuality);
      comboBoxQuality->addItem("*");
      comboBoxQuality->addItem("**");

      popupMenu->addAction(menuActions[IDA_IMAGEDETAILS]);
      popupMenu->addAction(widgetAction);

      menuActions[IDA_IMAGEDETAILS]->setChecked(true);
    }

    /// @brief Function to crop the image. Called from CFrameWindow.
    /// @throws None.
    /// @version 2015-09-21/GGB - Found and corrected memory leak.
    /// @version 2013-09-16/GGB - Default to image size for the dialog. (Bug #1219189)
    /// @version 2013-06-28/GGB - Added historyUpdate() and std::clog output.
    /// @version 2013-02-02/GGB - Added support for the histogram widget.
    /// @version 2011-08-15/GGB - Function created.

    void CImageWindow::cropImage()
    {
      MCL::TPoint2D<ACL::AXIS_t> origin(0, 0);
      MCL::TPoint2D<ACL::AXIS_t> dims(controlImage.astroFile->imageWidth(controlImage.currentHDB),
                                      controlImage.astroFile->imageHeight(controlImage.currentHDB));

      dialogs::CDialogImageCrop dlg(origin, dims);

      if (dlg.exec() == QDialog::Accepted)
      {
        controlImage.astroFile->imageCrop(controlImage.currentHDB, origin, dims);

        historyUpdate();

        LOGMESSAGE(info, "Image: " + controlImage.astroFile->getImageName() + ". Cropped: Origin: (" + std::to_string( origin.x()) +
                   ", " + std::to_string(origin.y()) + ") " + " Dimensions: (" + std::to_string(dims.x()) + ", " +
                   std::to_string(dims.y()) + ").");

          // Update the screen image.

        redrawImage();

        if (controlImage.astroFile->isDirty())
        {
          updateWindowTitle();
        };
      };
    }

    /// @brief Function to display the observatory, telescope and instrument.
    /// @throws None.
    /// @version 2013-06-29/GGB - Function created.

    void CImageWindow::displayEquipmentInformation()
    {
      std::string szValue;

      if ( controlImage.astroFile->keywordExists(ACL::HDU_PRIMARY, ACL::NOAO_OBSERVATORY) )
      {
        szValue = static_cast<std::string>(controlImage.astroFile->keywordData(ACL::HDU_PRIMARY, ACL::NOAO_OBSERVATORY));
        labelObservatory->setText(QString::fromStdString(szValue));
      };

      if ( controlImage.astroFile->keywordExists(ACL::HDU_PRIMARY, ACL::FITS_TELESCOPE) )
      {
        szValue = static_cast<std::string>(controlImage.astroFile->keywordData(ACL::HDU_PRIMARY, ACL::FITS_TELESCOPE));
        labelTelescope->setText(QString::fromStdString(szValue));
      };

      if ( controlImage.astroFile->keywordExists(ACL::HDU_PRIMARY, ACL::FITS_INSTRUMENT) )
      {
        szValue = static_cast<std::string>(controlImage.astroFile->keywordData(ACL::HDU_PRIMARY, ACL::FITS_INSTRUMENT));
        labelInstrument->setText(QString::fromStdString(szValue));
      };
    }

    /// @brief Called to display the data for an HDU.
    /// @param[in] nHDU = number of the HDU to display information for. 0 = Primary.
    /// @details All the keywords are read from the HDU file. All the standard keywords are also shown, even if there is no value
    ///          associated with them.
    /// @throws None.
    /// @version 2013-06-20/GGB
    ///                           @li Updated QString use.
    ///                           @li Fixed bug: 1165777 (Exposure time not displayed.)
    /// @version 2011-03-17/GGB - Changed to use the ACL::CAstroFile class.
    /// @version 2010-10-23/GGB - Function created.

    void CImageWindow::DisplayKeywords()
    {
      int nRow = 0;
      unsigned int nIndex;
      std::string szAxis;
      std::string szValue;
      std::string szComment;
      std::string szComments;
      ACL::DKeywordStore &keywords = controlImage.astroFile->keywords(controlImage.currentHDB);
      ACL::DKeywordStore::const_iterator keywordIterator;
      const int rowHeight = 20;

      while (twHDU->rowCount() > 0)
      {
        twHDU->removeRow(0);
      };

        // Simple keyword only if Primary HDU

      if (controlImage.currentHDB == 0)
      {
        twHDU->insertRow(nRow);
        twHDU->setRowHeight(nRow, rowHeight);
        twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(ACL::FITS_SIMPLE)));

        if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_SIMPLE, szValue, szComment) )
        {
          twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
          twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
        };

        nRow++;
      }
      else		// XTENSION
      {
        twHDU->insertRow(nRow);
        twHDU->setRowHeight(nRow, rowHeight);
        twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(ACL::FITS_XTENSION)));

        if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_XTENSION, szValue, szComment) )
        {
          twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
          twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
        };
        nRow++;
      };

        // BITPIX - Valid for all HDU types

      twHDU->insertRow(nRow);
      twHDU->setRowHeight(nRow, rowHeight);
      twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(ACL::FITS_BITPIX)));

      if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_BITPIX, szValue, szComment) )
      {
        twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
        twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
      };
      nRow++;

        // NAXIS - Valid for all conforming HDU's

      twHDU->insertRow(nRow);
      twHDU->setRowHeight(nRow, rowHeight);
      twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(ACL::FITS_NAXIS)));

      if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_NAXIS, szValue, szComment) )
      {
        twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
        twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
      };
      nRow++;

        // NAXISn - Valid for all conforming HDU's

      for(nIndex = 1; nIndex <= controlImage.astroFile->NAXIS(controlImage.currentHDB); nIndex++)
      {
        twHDU->insertRow(nRow);
        twHDU->setRowHeight(nRow, rowHeight);
        szAxis = ACL::FITS_NAXIS + std::to_string(nIndex);
        twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(szAxis)));

        if (controlImage.astroFile->keywordData(controlImage.currentHDB, szAxis, szValue, szComment) )
        {
          twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
          twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
        };
        nRow++;
      };

      if (controlImage.currentHDB != 0)		// PCOUNT and GCOUNT are mandatory in extension headers
      {
        twHDU->insertRow(nRow);
        twHDU->setRowHeight(nRow, rowHeight);
        twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(ACL::FITS_PCOUNT)));

        if ( controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_PCOUNT, szValue, szComment) )
        {
          twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
          twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
        };
        nRow++;

        twHDU->insertRow(nRow);
        twHDU->setRowHeight(nRow, rowHeight);
        twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(ACL::FITS_GCOUNT)));

        if ( controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_GCOUNT, szValue, szComment) )
        {
          twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(szValue)));
          twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString(szComment)));
        };
        nRow++;
      };

        // All the compulsary keywords are now complete.
        // Iterate over the remaining actual keywords in the selected header to find all the
        // Keywords.

      for (keywordIterator = keywords.begin(); keywordIterator != keywords.end(); keywordIterator++)
      {
        if (	( (*(*keywordIterator)).keyword() != ACL::FITS_SIMPLE) &&
              ( (*(*keywordIterator)).keyword() != ACL::FITS_XTENSION) &&
              ( (*(*keywordIterator)).keyword() != ACL::FITS_BITPIX) &&
              ( (*(*keywordIterator)).keyword() != ACL::FITS_NAXIS) &&
              ( (*(*keywordIterator)).keyword() != ACL::FITS_PCOUNT) &&
              ( (*(*keywordIterator)).keyword() != ACL::FITS_GCOUNT) &&
              ( (*(*keywordIterator)).keyword().find(ACL::FITS_NAXIS) == std::string::npos) )
        {
          twHDU->insertRow(nRow);
          twHDU->setRowHeight(nRow, rowHeight);
          twHDU->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString((*keywordIterator)->keyword())));
          twHDU->setItem(nRow, 1, new QTableWidgetItem(QString::fromStdString(static_cast<std::string>(**keywordIterator))));
          twHDU->setItem(nRow, 2, new QTableWidgetItem(QString::fromStdString((*keywordIterator)->comment())));

          /*if (szKeyword == "FILTER")					// Special treatment for the "FILTER" keyword.
          {
            int nFilter = comboFilter->findText(QString(szValue.c_str()), Qt::MatchStartsWith);

            if (nFilter != -1)
            {
              bInternalCheck = true;
              comboFilter->setCurrenINDEX_t(nFilter);
              comboFilter->setEnabled(false);
              bInternalCheck = false;
            };
          };*/
        };
      };

        // Set the comments and history fields.

      szComments = controlImage.astroFile->commentGet(controlImage.currentHDB);
      teComments->setPlainText(QString::fromStdString(szComments));

      historyUpdate();

        // Set the Observation Information values

      labelDateTime->setText(QString::fromStdString(ACL::sprintFITSDateTime(controlImage.astroFile->getObservationTime().UTC().JD())));

      if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::NOAO_EXPTIME, szValue, szComment) )
      {
        labelExposure->setText(QString::fromStdString(szValue));
      }
      else if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::HEASARC_EXPOSURE, szValue, szComment) )
      {
        labelExposure->setText(QString::fromStdString(szValue));
      }
      else
      {
        labelExposure->setText("----");
      }
    }

    /// @brief Called to display the data for the selected HDU.
    /// @details This function hides unused tabs and calls the relevant routines for getting the correct data into the correct tab.
    /// @pre This function must be called after the HDUType has been set by a call to DisplayKeywords
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-07-28/GGB - Added default and BT_NONE to switch statement.
    /// @version 2012-01-09/GGB - Added functionallity for astrometry and photometry tabs.
    /// @version 2011-03-19/GGB - Converted to handle changes to ACL::CastroFile caused by changes to ACL::CAstroFile
    /// @version 2010-10-24/GGB - Function created.

    void CImageWindow::DisplayData()
    {
      QString text = cbHDU->currentText();
      std::string hdbName = text.toStdString();

      switch (controlImage.astroFile->HDBType(hdbName))
      {
        case ACL::BT_IMAGE:
          tabWidget->insertTab(1, imageTab, QString(tr("Image")));
          DisplayImage();
          break;
        case ACL::BT_ATABLE:
          tabWidget->insertTab(1, atableTab, QString(tr("ASCII Table")));
          DisplayAsciiTable();
          break;
        case ACL::BT_BTABLE:
          tabWidget->insertTab(1, btableTab, QString(tr("Binary Table")));
          DisplayBinaryTable();
          break;
        case ACL::HDB_ASTROMETRY:
          tabWidget->insertTab(1, astrometryTab, QString(tr("Astrometry HDB")));
          DisplayAstrometry();
          break;
        case ACL::HDB_PHOTOMETRY:
          tabWidget->insertTab(1, photometryTab, QString(tr("Photometry HDB")));
          DisplayPhotometry();
          break;
        case ACL::BT_NONE:
        default:
        {
          CODE_ERROR(astroManager);
          break;
        };
      };
    }

    /// @brief Function to display the image from the HDU passed in nHDU.
    /// @details Any existing image data is deleted and the new image data is loaded, the Histogram created and all relevant data
    ///           calculated and displayed.
    /// @throws None.
    /// @version 2013-03-01/GGB - Added CODE_ERROR exceptions to the dockwidget if statements.
    /// @version 2013-02-06/GGB - Added code to display astrometry indicators.
    /// @version 2013-02-02/GGB - Added support for the histogram widget.
    /// @version 2011-05-30/GGB - Function created.

    void CImageWindow::DisplayImage()
    {
      dockwidgets::CDockWidgetImage::setCurrentImage(&controlImage);
      redrawImage();
    }

    void CImageWindow::DisplayAsciiTable()
    {
    }

    void CImageWindow::DisplayBinaryTable()
    {
    }

    /// Function to display the information in the astrometry tab. The existing data is removed, and the most current data is
    /// re-drawn.
    /// @version 2017-09-23/GGB - Update to use CAngle
    // 2013-08-17/GGB - Corrected RA/Dec display (Bug #1213076)
    // 2013-02-08/GGB - Ensure that the table is cleared before adding rows.
    // 2012-01-21/GGB - Function created

    void CImageWindow::DisplayAstrometry()
    {
      ACL::SPAstrometryObservation ao;
      int nRow = 0, nColumn;

      tableWidgetAstrometry->clearContents();

      while (tableWidgetAstrometry->rowCount() != 0)
        tableWidgetAstrometry->removeRow(0);

      if (controlImage.astroFile->hasAstrometryHDB())
      {
        ao = controlImage.astroFile->astrometryObjectFirst();

        while (ao)
        {
          nColumn = 0;
          tableWidgetAstrometry->insertRow(nRow);
          tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString::fromStdString(ao->objectName())));
          tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("(%1, %2)").arg(ao->CCDCoordinates().x()).arg(ao->CCDCoordinates().y())));
          if (ao->observedCoordinates())
          {
            tableWidgetAstrometry->setItem(nRow, nColumn++,
                                           new QTableWidgetItem(QString::fromStdString(ao->observedCoordinates()->RA().A2SHMS())));
            tableWidgetAstrometry->setItem(nRow, nColumn++,
                                           new QTableWidgetItem(QString::fromStdString(ao->observedCoordinates()->DEC().A2SDMS())));
          }
          else
          {
            tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("--h--'--""")));
            tableWidgetAstrometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("--" % UTF16_DEGREESIGN % "--'--""")));
          };

          ao = controlImage.astroFile->astrometryObjectNext();
        };
      };
    }

    /// Function to redraw the data in the photometry tab.
    //
    /// @version 2017-09-23/GGB - Update to use CAngle
    // 2013-08-17/GGB - Corrected RA/Dec display (Bug #1213076)
    // 2013-04-09/GGB - Function created.

    void CImageWindow::DisplayPhotometry()
    {
      ACL::SPPhotometryObservation po;
      int nRow = 0, nColumn;

      tableWidgetPhotometry->clearContents();

      while (tableWidgetPhotometry->rowCount() != 0)
        tableWidgetPhotometry->removeRow(0);

      if (controlImage.astroFile->hasPhotometryHDB())
      {
        po = controlImage.astroFile->photometryObjectFirst();

        while (po)
        {
          nColumn = 0;
          tableWidgetPhotometry->insertRow(nRow);
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString::fromStdString(po->objectName())));

          if (po->observedCoordinates())
          {
            tableWidgetPhotometry->setItem(nRow, nColumn++,
                                           new QTableWidgetItem(QString::fromStdString(po->observedCoordinates()->RA().A2SHMS())));
            tableWidgetPhotometry->setItem(nRow, nColumn++,
                                           new QTableWidgetItem(QString::fromStdString(po->observedCoordinates()->DEC().A2SDMS())));
          }
          else
          {
            tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem("--h--'--"""));
            tableWidgetPhotometry->setItem(nRow, nColumn++,
                                           new QTableWidgetItem(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--"""));
          };
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("(%1, %2)").arg(po->CCDCoordinates().x()).
                                                                               arg(po->CCDCoordinates().y())));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(*(po->instrumentMagnitude()))));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->magnitudeError())));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->sourceADU())));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->sourceArea())));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->skyADU())));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->skyArea())));
          tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(*po->FWHM())));


          po = controlImage.astroFile->photometryObjectNext();
        };
      }
    }

    /// @brief Displays the observation site information.
    /// @throws None.
    /// @version 2018-08-25/GGB - Changed getObservationLocation() to a raw pointer return.
    /// @version 2011-07-17/GGB - Function created

    void CImageWindow::displaySiteInformation()
    {
      ACL::CObservatory *location = controlImage.astroFile->getObservationLocation();
      char szNumber[30];

      if (!location)
      {
        labelLatitude->setText(tr("Not Set"));
        labelLongitude->setText(tr("Not Set"));
        labelAltitude->setText(tr("Not Set"));
      }
      else
      {
        ACL::sprintfLMS(szNumber, ACL::dms2deg(location->latitude()), 2, 0);
        labelLatitude->setText(QString::fromLocal8Bit(szNumber));

        ACL::sprintfLMS(szNumber, ACL::dms2deg(location->longitude()), 2, 1);
        labelLongitude->setText(QString::fromLocal8Bit(szNumber));

        labelAltitude->setText(QString("%1").arg(location->altitude()));
      };
    }

    /// @brief Adds the target information to the specified labels.
    /// @throws None.
    /// @version 2017-09-23/GGB - Update to use CAngle
    /// @version 2017-08-25/GGB - Function created.

    void CImageWindow::displayTargetInformation()
    {
      labelTarget->setText(QString::fromStdString(controlImage.astroFile->getObservationTarget()));
      labelTargetRA->setText(QString::fromStdString(controlImage.astroFile->imageCenter().RA().A2SHMS()));
      labelTargetDEC->setText(QString::fromStdString(controlImage.astroFile->imageCenter().DEC().A2SDMS()));
    }

    /// @brief Displays the weather information.
    /// @throws None.
    /// @version 2018-08-25/GGB - Changed getObservationWeather() to a raw pointer.
    /// @version 2011-07-17/GGB - Function created.

    void CImageWindow::displayWeatherInformation()
    {
      ACL::CWeather *weather = controlImage.astroFile->getObservationWeather();

      if (!(&*weather))
      {
        labelTemperature->setText("---" % UTF16_DEGREESIGN % "C");
        labelPressure->setText("---kPa");
        labelRH->setText("---%");
      }
      else
      {
        try
        {
          labelTemperature->setText(QString("%1").arg((*weather->temperature())(PCL::TU_C)));
        }
        catch (GCL::CError &err)
        {
          if (err.errorCode() == 0x1100)
            labelTemperature->setText("");
          else
            throw;
        };
        try
        {
          labelPressure->setText(QString("%1").arg((*weather->pressure())(PCL::PU::BAR)));
        }
        catch (GCL::CError &err)
        {
          if (err.errorCode() == 0x1101)
            labelPressure->setText("");
          else
            throw;
        };
        try
        {
          labelRH->setText(QString("%1").arg(*(weather->RH())));
        }
        catch (GCL::CError &err)
        {
          if (err.errorCode() == 0x1102)
            labelRH->setText("");
          else
            throw;
        };
      };
    }

    /// @brief Opens the dialog to edit the image metadata.
    /// @note This will only be called if the ARID database is enabled.
    /// @throws GCL::CCodeError(...)
    /// @version 2017-08-20/GGB - Function created.

    void CImageWindow::editMetadata()
    {
      dialogs::CDialogImageDetails dialogImageDetails(this, controlImage.astroFile->imageID());

      dialogImageDetails.exec();
    }

    /// @brief Event handler when the current HDU is changed from the combo box.
    /// @param[in] nHDU: The number of the new HDU
    /// @details All exisiting display data must be cleared and the new data loaded and displayed.
    /// @throws None.
    /// @version 2010-10-24/GGB - Function created.

    void CImageWindow::eventCurrentHDUChanged(int nHDU)
    {
      controlImage.currentHDB = nHDU;

        // Remove all the rows from the table.

      while (twHDU->rowCount() != 0)
      {
        twHDU->removeRow(0);
      };
      tabWidget->removeTab(1);		// Hide the tab widget.
      DisplayKeywords();
      DisplayData();
      tabWidget->setCurrentIndex(1);
    }

    /// @brief Dialog to edit the weather information.
    /// @throws None.
    /// @version 2016-03-26/GGB - Function created.

    void CImageWindow::eventEditWeather()
    {
        // If the weather from the astro file is null, then a weather record needs to be created.

      ACL::CWeather *weather = controlImage.astroFile->getObservationWeather();
      ACL::CWeather weatherData;

      if (weather)
      {
        weatherData = *weather;
      };

      dialogs::CDialogWeatherEdit dlg(weatherData);

      if (dlg.exec() == QDialog::Accepted)
      {
        if (!weather)
        {
          controlImage.astroFile->setObservationWeather(std::make_unique<ACL::CWeather>(weatherData));
        }
        else
        {
          *weather = weatherData;
        };

          // Update the display

        displayWeatherInformation();

          // Update the saved status

        controlImage.astroFile->isDirty(true);
        controlImage.astroFile->hasData(true);
        updateWindowTitle();
      };
    }

    /// @brief Handles the mouse event from the graphics window.
    /// @details If there is no current mode selected, the event is ignored.
    ///          If the current mode is Astrometry, then the astrometry cross hairs is drawn
    ///          If the current mode is Photometry, then the photometry circles are drawn.
    /// @throws GCL::CCodeError()
    /// @version 2017-08-25/GGB - Added popup menu.
    /// @version 2011-06-19/GGB - 1) Added better support for new modes of operation.
    ///                         - 2) Moved worker code into new procedures to create a cleaner easier to read procedure.
    /// @version 2010-11-27/GGB - Function created.

    void CImageWindow::eventMousePress(QMouseEvent *mouseEvent)
    {
      if (controlImage.astroFile->HDBType(controlImage.currentHDB) == ACL::BT_IMAGE)
      {
        switch (mouseEvent->button())
        {
          case Qt::LeftButton:
          {
            switch (mode)
            {
              case CImageWindow::M_ASTROMETRY:
                mousePressAstrometry(mouseEvent);
                break;
              case CImageWindow::M_PHOTOMETRY:
                mousePressPhotometry(mouseEvent);
                break;
              case CImageWindow::M_SPECTROSCOPY:
              case CImageWindow::M_NONE:
                break;
              default:
                CODE_ERROR(astroManager);
                break;
            };
            break;
          };
          case Qt::RightButton:
          {
            popupMenu->popup(mapToGlobal(mouseEvent->pos()));
            break;
          };
          case Qt::MidButton:
          case Qt::NoButton:
            break;
          default:
          {
            CODE_ERROR(astroManager);
            break;
          };
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Function called to export astrometry or photometry data to .csv file.
    /// @throws CCodeError
    /// @version 2013-05-12/GGB - Function created.

    void CImageWindow::exportAsCSV()
    {
      QString text = cbHDU->currentText();
      std::string hdbName = text.toStdString();

      switch (controlImage.astroFile->HDBType(hdbName))
      {
        case ACL::BT_NONE:
        case ACL::BT_ATABLE:
        case ACL::BT_BTABLE:
        case ACL::BT_IMAGE:
          CODE_ERROR(astroManager);
        case ACL::HDB_ASTROMETRY:
          exportAstrometryAsCSV();
          break;
        case ACL::HDB_PHOTOMETRY:
          exportPhotometryAsCSV();
          break;
        default:
          CODE_ERROR(astroManager);
      };
    }

    void CImageWindow::exportAsDNG()
    {
    }

    /// @brief Function to export the file as a JPEG.
    /// Calls the JPEG export function of the CAstroFile class.
    //
    /// @version 2013-04-27/GGB - Function created.

    void CImageWindow::exportAsJPEG()
    {
       QString fileName = QFileDialog::getSaveFileName(this, tr("Export file as JPEG..."),
        settings::astroManagerSettings->value(settings::IMAGING_DIRECTORY, QVariant("")).toString(),
        tr("JPEG Files (*.jpg)"));

      if (!fileName.isNull())
      {
        ACL::AXIS_t width, height;
        int quality = 80;

        width = gsImage->sceneRect().width();
        height = gsImage->sceneRect().height();

        dialogs::CDialogExportAsJPEG dialog(width, height, quality);
        if (dialog.exec() == QDialog::Accepted)
        {

          boost::filesystem::path JPEGpath(fileName.toStdString());
          JPEGpath.replace_extension(".jpg");

          QSize sceneSize(width, height);
          QImage sceneImage(sceneSize, QImage::Format_RGB32);
          QPainter scenePainter;

          scenePainter.begin(&sceneImage);
          gsImage->render(&scenePainter);   // Render the scene to the image.
          scenePainter.end();

          if (sceneImage.save(QString::fromStdString(JPEGpath.string()), "JPG", quality))
            LOGMESSAGE(info, JPEGpath.string() + " saved.");
          else
            LOGMESSAGE(warning, "Failed to save " + JPEGpath.string() + ".");
        };
      };
    }

    /// Exports the selected scene as a .png file.
    //
    // 2014-12-30/GGB - Use GCL::logger rather than std::clog for message logging.
    // 2013-07-25/GGB - Function created.

    void CImageWindow::exportAsPNG()
    {
      QString fileName = QFileDialog::getSaveFileName(this, tr("Export file as PNG..."),
       settings::astroManagerSettings->value(settings::IMAGING_DIRECTORY, QVariant("")).toString(),
       tr("PNG Files (*.png)"));

     if (!fileName.isNull())
     {
       ACL::AXIS_t width, height;
       int quality = 80;

       width = gsImage->sceneRect().width();
       height = gsImage->sceneRect().height();

       dialogs::CDialogExportAsPNG dialog(width, height, quality);
       if (dialog.exec() == QDialog::Accepted)
       {

         boost::filesystem::path PNGpath(fileName.toStdString());
         PNGpath.replace_extension(".png");

         QSize sceneSize(width, height);
         QImage sceneImage(sceneSize, QImage::Format_RGB32);
         QPainter scenePainter;

         scenePainter.begin(&sceneImage);
         gsImage->render(&scenePainter);   // Render the scene to the image.
         scenePainter.end();
         if (sceneImage.save(QString::fromStdString(PNGpath.string()), "PNG", quality))
           LOGMESSAGE(info, PNGpath.string() + " saved.");
         else
           LOGMESSAGE(warning, "Failed to save " + PNGpath.string());
       };
     };
    }

    void CImageWindow::exportAsTIFF()
    {
    }

    /// @brief Exports the data from the Astrometry HDB as a .csv file. This file can then be opened in a spreadhseet program.
    /// @throws None
    /// @version 2015-01-04/GGB - Function created.

    void CImageWindow::exportAstrometryAsCSV()
    {
      std::ofstream outputFile;
      std::string szDateTime;
      astrometry::DAstrometryObservationStore::iterator iterator;

      ACL::CHDB *currentHDB = controlImage.astroFile->getHDB(0);

        // Read the date/time from the image.

      if (currentHDB->keywordExists(ACL::FITS_DATEOBS))
      {
        szDateTime = static_cast<std::string>(currentHDB->keywordData(ACL::FITS_DATEOBS));
      }
      else
      {
        szDateTime = std::string("Not Available");
      }

        // Get the name of the file to save.

      QString fileName = QFileDialog::getSaveFileName(this, tr("Export Astrometry as..."),
                                                      settings::astroManagerSettings->value(settings::ASTROMETRY_CSVDIRECTORY,
                                                                                    QVariant("")).toString(),
                                                      tr("CSV Files (*.csv)"));

      if (!fileName.isNull())
      {
        std::string fileString(fileName.toStdString());
        outputFile.open(fileString, std::ios::out | std::ios::trunc);

        outputFile << "DATE-OBS: " << szDateTime << std::endl;

        outputFile << "Object Name, RA, Dec, CCD (x), CCD (y), Inst Mag, MagErr" << std::endl;

        for (iterator = controlImage.astrometryObservations.begin(); iterator != controlImage.astrometryObservations.end(); iterator++)
        {
          outputFile << (*(*iterator)) << std::endl;
        };

        outputFile.close();
      };

    }

    /// @brief Exports the data from the photometry HDB as .csv data to a file. This file can then be opened using a spreadsheet program.
    //
    /// @version 2013-05-12/GGB - Function created.

    void CImageWindow::exportPhotometryAsCSV()
    {
      std::ofstream outputFile;
      photometry::DPhotometryObservationStore::iterator iterator;
      ACL::CHDB *currentHDB;
      FP_t zmag = 0;
      std::string filterName;
      std::string szDateTime;

      currentHDB = controlImage.astroFile->getHDB(0);     // Need to check the original image for ZMAG and filter.

      if (currentHDB->HDBType() == ACL::BT_IMAGE)
      {
        if (currentHDB->keywordExists(ACL::ASTROMANAGER_ZMAG))
        {
          zmag = static_cast<FP_t>(currentHDB->keywordData(ACL::ASTROMANAGER_ZMAG));
        }
        else
        {
          zmag = 0;
        };

        if (currentHDB->keywordExists(ACL::HEASARC_FILTER))
        {
          filterName = static_cast<std::string>(currentHDB->keywordData(ACL::HEASARC_FILTER));
        }
        else
        {
          filterName = std::string("Not specified");
        }

        if (currentHDB->keywordExists(ACL::FITS_DATEOBS))
        {
          szDateTime = static_cast<std::string>(currentHDB->keywordData(ACL::FITS_DATEOBS));
        }
        else
        {
          szDateTime = std::string("Not Available");
        };

      };


        // Get the name of the file to save.

      QString fileName = QFileDialog::getSaveFileName(this, tr("Export Photometry as..."),
                                                      settings::astroManagerSettings->value(settings::PHOTOMETRY_CSVDIRECTORY,
                                                                                    QVariant("")).toString(),
                                                      tr("CSV Files (*.csv)"));

      if (!fileName.isNull())
      {
        std::string fileString(fileName.toStdString());
        outputFile.open(fileString, std::ios::out | std::ios::trunc);

        outputFile << "DATE-OBS: " << szDateTime << std::endl;

        outputFile << "Filter: " << filterName << std::endl;

        outputFile << "ZMAG: " << zmag << std::endl;

        outputFile << "Object Name, RA, Dec, CCD (x), CCD (y), Inst Mag, MagErr" << std::endl;

        for (iterator = controlImage.photometryObservations.begin(); iterator != controlImage.photometryObservations.end(); iterator++)
          outputFile << (*(*iterator)) << std::endl;

        outputFile.close();
      };
    }

    /// @brief Function to extract objects using the find stars routine.
    /// @param[out] sourceContainer: The list of "found" stars.
    /// @returns true: The dialog was accepted.
    /// @returns false: The dialog was not accepted.
    /// @throws None.
    /// @version 2014-12-29/GGB - Function created.

    bool CImageWindow::extractFindStars(ACL::TImageSourceContainer &sourceContainer)
    {
      QPen pen;
      QColor const current = Qt::yellow;
      pen.setColor(current);
      std::list<QGraphicsEllipseItem *> markers;
      std::list<QGraphicsEllipseItem *>::iterator markersIterator;
      ACL::TImageSourceContainer::const_iterator sourceIterator;
      QGraphicsEllipseItem *graphicsItem;

      auto &pw = dynamic_cast<dockwidgets::CPhotometryDockWidget &>
          (dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));

      dialogs::SDialogFindStars sourceParameters;
      sourceParameters.minBorder = pw.getRadius3();
      dialogs::CDialogFindStars dialogFindStars(sourceParameters);

      int dialogReturn;

      while ( (dialogReturn = dialogFindStars.exec()) == dialogs::DialogExtract)
      {
          // Remove all markers and delete.

        if (!markers.empty())
        {
          for (markersIterator = markers.begin(); markersIterator != markers.end(); ++markersIterator)
          {
            (*markersIterator)->hide();
            gsImage->removeItem(*markersIterator);
            delete (*markersIterator);
            (*markersIterator) = nullptr;
          };
          markers.clear();
        };

        sourceContainer.clear();    // Need to remove all items from the list.
        controlImage.astroFile->findStars(controlImage.currentHDB, sourceContainer, sourceParameters);

          // Draw the objects on the screen.

        TRACEMESSAGE("Drawing items...");

        std::for_each(sourceContainer.begin(), sourceContainer.end(),
                      [&] (ACL::PImageSource pis)
        {
          graphicsItem = new QGraphicsEllipseItem( pis->center.x() - pis->radius, pis->center.y() - pis->radius,
                                                   pis->radius * 2, pis->radius * 2);

          graphicsItem->setPen(pen);
          gsImage->addItem(graphicsItem);
          markers.push_back(graphicsItem);
          graphicsItem = nullptr;
        });


        TRACEMESSAGE("Finished drawing items.");
      };

        // Delete all the markers. Not required any further.

      if (!markers.empty())
      {
        for (markersIterator = markers.begin(); markersIterator != markers.end(); ++markersIterator)
        {
          (*markersIterator)->hide();
          gsImage->removeItem(*markersIterator);
          delete (*markersIterator);
          (*markersIterator) = nullptr;
        };
        markers.clear();
      };

      LOGMESSAGE(info, "Number of objects identified: " + std::to_string(sourceContainer.size()));

      return (dialogReturn == QDialog::Accepted);
    }

    /// @brief Extract objects using the Simple XY routine.
    /// @param[out] sourceContainer: The list of "found" stars.
    /// @returns true: The dialog was accepted.
    /// @returns false: The dialog was not accepted.
    /// @throws None.
    /// @version 2014-12-29/GGB - Function created.

    bool CImageWindow::extractSimpleXY(ACL::TImageSourceContainer &sourceContainer)
    {
      QPen pen;
      QColor const current = Qt::yellow;
      pen.setColor(current);

      std::list<QGraphicsEllipseItem *> markers;
      std::list<QGraphicsEllipseItem *>::iterator markersIterator;

      QGraphicsEllipseItem *graphicsItem;

      ACL::TImageSourceContainer::const_iterator sourceIterator;

      if (!markers.empty())
      {
        for (markersIterator = markers.begin(); markersIterator != markers.end(); ++markersIterator)
        {
          graphicsItem = (*markersIterator);
          graphicsItem->hide();
          gsImage->removeItem(graphicsItem);
          delete (graphicsItem);
          (*markersIterator) = nullptr;
        };
        markers.clear();
      };

        // Draw the objects on the screen.

      for (sourceIterator = sourceContainer.begin(); sourceIterator != sourceContainer.end(); ++sourceIterator)
      {
        graphicsItem = new QGraphicsEllipseItem( (*sourceIterator)->center.x() - (*sourceIterator)->radius,
                                                 (*sourceIterator)->center.y() - (*sourceIterator)->radius,
                                                 (*sourceIterator)->radius * 2, (*sourceIterator)->radius * 2);

        graphicsItem->setPen(pen);
        gsImage->addItem(graphicsItem);
        markers.push_back(graphicsItem);
        graphicsItem = nullptr;
      }

      LOGMESSAGE(info, "Number of objects identified: " + std::to_string(sourceContainer.size()));
      return false;
    }

    /// @brief Handles the flip event from the menu.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2014-12-28/GGB - Changed logging to use GCL::logger.
    /// @version 2013-06-29/GGB - Bug #1195952 fixed.
    /// @version 2013-06-28/GGB - Added historyUpdate().
    /// @version 2013-02-02/GGB - Added support for the histogram widget
    /// @version 2011-05-29/GGB - Function created.

    void CImageWindow::flipImage()
    {
      if (controlImage.astroFile)
      {
        controlImage.astroFile->flipImage(controlImage.currentHDB);

        historyUpdate();

        LOGMESSAGE(info, "Image: " + controlImage.astroFile->getImageName() + ". Mirrored horizontal.");

          // Update the screen image.

        redrawImage();
        zoomAll();

        if (controlImage.astroFile->isDirty())
        {
          updateWindowTitle();
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Floats the image onto a larger canvas.
    /// 1) Get the size of the canvas from the user.
    /// 2) Float the image.
    //
    // 2015-07-30/GGB - Changed the dialog to stack assigned rather than dynamically assigned.
    // 2014-12-30/GGB - Use GLC::logger rather than std::clog for messaging.
    // 2013-06-28/GGB - Added historyUpdate() and std::clog output.
    // 2013-02-02/GGB - Added support for the histogram widget.
    // 2011-06-04/GGB - Function created.

    void CImageWindow::floatImage()
    {
      dialogs::CDialogImageFloat dlg(controlImage.astroFile->getAstroImage(controlImage.currentHDB));

      if (dlg.exec() == QDialog::Accepted)
      {
        controlImage.astroFile->imageFloat(controlImage.currentHDB, dlg.getWidth(), dlg.getHeight(), dlg.getBackground());

        historyUpdate();

        LOGMESSAGE(info, "Image: " + controlImage.astroFile->getImageName() + ". Floated: (" + std::to_string(dlg.getWidth()) +
                   ", " + std::to_string(dlg.getHeight()) +  ").");

        redrawImage();

        if (controlImage.astroFile->isDirty())
        {
          updateWindowTitle();
        };
      };
    }

    /// @brief Handles the flip event from the menu.
    /// @details Call the astroFile flip event.
    //
    // 2014-12-30/GGB - Use GCL::logger rather than std::clog for messaging.
    // 2013-06-29/GGB - Bug #1195952 fixed.
    // 2013-06-28/GGB - Added historyUpdate()
    // 2013-02-02/GGB - Added support for the Histogram widget.
    // 2011-05-29/GGB - Function created.

    void CImageWindow::flopImage()
    {
        // Modify the image

      if (controlImage.astroFile)
      {
        controlImage.astroFile->flopImage(controlImage.currentHDB);

        historyUpdate();

        LOGMESSAGE(info, "Image: " + controlImage.astroFile->getImageName() + ". Mirrored vertical.");

          // Now update the screen view.

        redrawImage();
        zoomAll();

        if (controlImage.astroFile->isDirty())
          updateWindowTitle();
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    // event function when the tab widget is changed.
    //
    // 2013-03-31/GGB - Added support for the photometry tab.
    // 2011-06-02/GGB - FUnction created.

    void CImageWindow::eventTabWidgetChanged(int tabIndex)
    {
      QString text = cbHDU->currentText();
      std::string hdbName = std::string(text.toStdString());

      if (tabIndex == 1)
      {
        switch( controlImage.astroFile->HDBType(hdbName) )
        {
        case ACL::BT_NONE:
        case ACL::BT_IMAGE:
          imageTabActivating();
          break;
        case ACL::BT_ATABLE:
          atableTabActivating();
          break;
        case ACL::BT_BTABLE:
          btableTabActivating();
          break;
        case ACL::HDB_ASTROMETRY:
          astrometryTabActivating();
          break;
        case ACL::HDB_PHOTOMETRY:
          photometryTabActivating();
          break;
        default:
          CODE_ERROR(astroManager);
          break;
        };
      }
      else
        infoTabActivating();
    }

    /// @brief Returns the astroFile associated with the window.
    /// @returns Pointer to the astroFile.
    /// @version 2018-09-22/GGB - Converted to std::shared_ptr.
    /// @version 2013-03-03/GGB - Additional items enabled.
    /// @version 2012-01-07/GGB - Function created.

    ACL::CAstroFile *CImageWindow::getAstroFile() const
    {
      return controlImage.astroFile.get();
    }

    /// Updates the history being displayed.
    //
    // 2013-06-28/GGB - Function created.

    void CImageWindow::historyUpdate()
    {
        // Update the history text.

      std::string szHistory = controlImage.astroFile->historyGet(controlImage.currentHDB);
      teHistory->setPlainText(QString::fromStdString(szHistory));
    }

    /// @brief Called when the imageTab is selected.
    /// @details Ensures that the transform menu items and zoom menu items are made visible.
    /// @throws CRuntimeAssert
    /// @version  2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    /// @version 2016-04-24/GGB - Bug# 1574157 - Ensure the view photometry and astrometry are in the correct state.
    /// @version 2015-09-21/GGB - (Bug 83) Added code to unhide Zoom1:1 option.
    /// @version 2015-09-20/GGB - (Bug 86) Added code to check for colour/mono images.
    /// @version 2013-05-12/GGB - Added the EXPORT CSV option.
    /// @version 2012-01-07/GGB - Function created.

    void CImageWindow::imageTabActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw != nullptr, "The parent widget should exist.")

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_DNG)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_JPEG)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_PNG)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_TIFF)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORT_CSV)->setEnabled(false);

        // View Menu

      pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(true);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(true);
      pw->getAction(mdiframe::IDA_VIEW_ZOOM11)->setEnabled(true);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(true);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(true);
      pw->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->setEnabled(true);
      pw->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->setEnabled(true);

        // Process menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(true);
      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Analyse actions

      pw->getAction(mdiframe::IDA_ANALYSIS_EXTRACTOBJECTS)->setEnabled(true);
      if (controlImage.astroFile->hasWCSData(controlImage.currentHDB))
      {
        pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(true);
      }
      else
      {
        pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(false);
      }

      pw->getAction(mdiframe::IDA_PHOTOMETRY_LOADTARGETLIST)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(true);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(true);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(true);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(true);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(true);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(true);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(true);

        // IA Colour Menu

      if ( controlImage.astroFile->isPolyImage(controlImage.currentHDB) )
      {
        pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_RGB)->setEnabled(true);
        pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_LRGB)->setEnabled(true);
        pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_GRAYSCALE)->setEnabled(true);
      }
      else
      {
        pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_RGB)->setEnabled(false);
        pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_LRGB)->setEnabled(false);
        pw->getAction(mdiframe::IDA_IA_COLOUR_SPLIT_GRAYSCALE)->setEnabled(false);
      };
    }

    // Procedure called when the info Tab is activating.
    // disabled menu items as required.
    //
    /// @version  2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    // 2013-05-12/GGB - Added the export CSV option.
    // 2011-06-02/GGB - Function created.

    void CImageWindow::infoTabActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      RUNTIME_ASSERT(astroManager, pw != nullptr, "The parent widget should exist.")

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_DNG)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_JPEG)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_PNG)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_TIFF)->setEnabled(true);
      pw->getAction(mdiframe::IDA_FILE_EXPORT_CSV)->setEnabled(false);

        // View Menu

      pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);

        // Process menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Analysis menu

      pw->getAction(mdiframe::IDA_ANALYSIS_EXTRACTOBJECTS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(false);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);
    }

    /// @brief Loads objects into the image.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2016-04-25/GGB - Function created.

    void CImageWindow::loadObjects()
    {
      auto &dw = dynamic_cast<dockwidgets::CAstrometryDockWidget &>
          (dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL));
      std::optional<ACL::CAstronomicalCoordinates> tl, br;
      ACL::DTargetAstronomy targetList;       // Will be destructed when it goes out of scope.
      int targetCount = 0, targetOutside = 0, targetCentroid = 0;

        // Important to remember that the image does not have to be aligned to RA/Dec, but can be at an angle. This becomes
        // important when querying the SIMBAD database as the type of the surrounding box to access all the objects
        // needs to take this into account.

        // Due to the seperation of image from database and internet access, the astroManager application needs to perform the database
        // or internet access to obtain a list of objects that fall within the image.
        // The ACL routines for SIMBAD access will be used if the data is going to be fetched from the SIMBAD database.


        // Get the surrounding square

      MCL::TPoint2D<FP_t> topLeft(0, controlImage.astroFile->imageHeight(controlImage.currentHDB) - 1);
      MCL::TPoint2D<FP_t> bottomRight(controlImage.astroFile->imageWidth(controlImage.currentHDB) - 1, 0);

      tl = controlImage.astroFile->pix2wcs(controlImage.currentHDB, topLeft);
      br = controlImage.astroFile->pix2wcs(controlImage.currentHDB, bottomRight);

        // Query the database

      database::databaseATID->queryByCoordinates(*tl, *br, targetList);

      LOGMESSAGE(info, "Adding to target list...");

        // Add the targets to the target list.
        // For each target, we need to convert the RA/DEC to a pixel pair.
        // The pixel pair is then used to search for the centroid.
        // When the centroid is found, the object is checked against the list for repeats.
        // The object can then be added to the list.


      for (auto &elem : targetList)
      {
        std::optional<MCL::TPoint2D<FP_t>> ccdPixel;

        ccdPixel = controlImage.astroFile->wcs2pix(controlImage.currentHDB, elem->positionICRS(ACL::CAstroTime()));

          // It is possible for the converted coordinate to fall outside of the image.

        if (ccdPixel)
        {
            // Search for the centroid of the object.

          ccdPixel = controlImage.astroFile->centroid(controlImage.currentHDB, *ccdPixel,
                                                      settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS,
                                                                                    QVariant(20)).toLongLong(),
                                                      settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                    QVariant(3)).toInt());;
          if (ccdPixel)
          {
              // Found a centroid.

            bool bClose = false;
            astrometry::PAstrometryObservation astrometryObject;
            ACL::SPAstrometryObservation existingObject;

              // Check if the object is already identified.

            if (controlImage.astroFile->hasAstrometryHDB())
            {
              existingObject = controlImage.astroFile->astrometryObjectFirst();

              while ( (existingObject) && !bClose)
              {
                bClose = existingObject->isClose(*ccdPixel,
                                                 settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS).toInt());
                existingObject = controlImage.astroFile->astrometryObjectNext();
              };
            };

              // Object not identified - add to the list.

            if (!bClose)
            {
              astrometryObject.reset(new astrometry::CAstrometryObservation(elem)); // Associate the target with the element.
              astrometryObject->CCDCoordinates(*ccdPixel);

                // Add the astrometry observation to the reference list.

              if ( !controlImage.astroFile->hasAstrometryHDB() )
              {
                ACL::CHDBAstrometry *ahdb = controlImage.astroFile->createAstrometryHDB();
                ahdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
                ahdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
              };

              controlImage.astroFile->astrometryObjectAdd(astrometryObject);
              controlImage.astrometryObservations.push_back(astrometryObject);          // Add the objects into the vector.

              changeAstrometrySelection(astrometryObject);

              astrometryReferenceAdd(astrometryObject);
              dw.referenceCompleted(astrometryObject);

              controlImage.astroFile->isDirty(true);
              controlImage.astroFile->hasData(true);
              updateWindowTitle();

              targetCount++;

              LOGMESSAGE(info, "Added object " + elem->objectName() + " to astrometry list.");

              astrometryObject.reset();
            };
          }
          {
              // No centroid found. Note this in the debug log.

            targetCentroid++;
            LOGMESSAGE(debug, "Object " + elem->objectName() + " could not find centroid. Not added to image.");
          }
        }
        else
        {
            // Pixel falls outside of the image.  Note it in the debug log.

          targetOutside++;
          LOGMESSAGE(debug, "Object " + elem->objectName() + " falls outside the image.");
        }
      };
      LOGMESSAGE(info, "Added " + std::to_string(targetCount) + " objects to image.");
      LOGMESSAGE(debug, "Failed to add " + std::to_string(targetCentroid) + " objects for \"Centroid not found\".");
      LOGMESSAGE(debug, "Failed to add " + std::to_string(targetOutside) + " objects for \"Object falls Outside the image boundaries\".");
      LOGMESSAGE(info, "Completed adding objects to Astrometry list.");
    }

    /// @brief Function to extract all the objects in the image.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2015-09-20/GGB - (Bug 81) Added try...catch block around pointPhotometry() call as this can throw.
    /// @version 2014-02-09/GGB - Added support for algorithm choice.
    /// @version 2012-07-28/GGB - Function created.

    void CImageWindow::extractObjects()
    {
      TRACEENTER;

      ACL::TImageSourceContainer imageObjectList;
      bool returnValue = false;

      auto frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      auto &adw = dynamic_cast<dockwidgets::CAstrometryDockWidget &>(frameWindow->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL));
      auto &pdw = dynamic_cast<dockwidgets::CPhotometryDockWidget &>(frameWindow->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));
      ACL::CAstroImage *astroImage = controlImage.astroFile->getAstroImage(controlImage.currentHDB);

      RUNTIME_ASSERT(astroManager, astroImage != nullptr, "The astro image should not be a nullptr.");

      LOGMESSAGE(info, "Starting function Source Extraction.");

        // Determine the objects automatically

      int algorithm = settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_ALGORITHM, QVariant(SEA_FINDSTARS)).toInt();

      switch(algorithm)
      {
        case SEA_FINDSTARS:
        {
          returnValue = extractFindStars(imageObjectList);
          break;
        };
        case SEA_SIMPLEXY:
        {
          returnValue = extractSimpleXY(imageObjectList);
          break;
        };
        default:
        {
          CODE_ERROR(astroManager);
        };
      };    // End of switch(algorithm) statement

      if (returnValue)    // Need to add the objects to the relevant list(s).
      {

        if (settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_ADD_ASTROMETRY, QVariant(false)).toBool())
        {
          DEBUGMESSAGE("Adding objects to Astrometry list...");

          bool bClose = false;
          astrometry::PAstrometryObservation astrometryObject;
          ACL::SPAstrometryObservation existingObject;

          for (auto iter : imageObjectList)
          {
            if (controlImage.astroFile->hasAstrometryHDB())
            {
              existingObject = controlImage.astroFile->astrometryObjectFirst();

              while ( (existingObject) && !bClose)
              {
                bClose = existingObject->isClose(iter->center,
                                                 settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS).toInt());
                existingObject = controlImage.astroFile->astrometryObjectNext();
              };
            };

            if (!bClose)
            {
              astrometryObject.reset(new astrometry::CAstrometryObservation());
              astrometryObject->CCDCoordinates(iter->center);

              // Get the image coordinates and convert to WCS coordinates.

              std::optional<ACL::CAstronomicalCoordinates> WCSCoordinates =
                  controlImage.astroFile->getHDB(controlImage.currentHDB)->pix2wcs(astrometryObject->CCDCoordinates());

              if (WCSCoordinates)
              {
                astrometryObject->observedCoordinates(*WCSCoordinates);
              };

              QString objectName = QString("A:%1").arg(controlImage.astroFile->astrometryObjectCount() + 1);
              astrometryObject->objectName(objectName.toStdString());

                // Add the astrometry observation to the reference list.

              if ( !controlImage.astroFile->hasAstrometryHDB() )
              {
                ACL::CHDBAstrometry *ahdb = controlImage.astroFile->createAstrometryHDB();
                ahdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
                ahdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
              };

              controlImage.astroFile->astrometryObjectAdd(astrometryObject);
              controlImage.astrometryObservations.push_back(astrometryObject);          // Add the objects into the vector.

              changeAstrometrySelection(astrometryObject);

              adw.addNewObject(std::dynamic_pointer_cast<astrometry::CAstrometryObservation>(astrometryObject));
              adw.displayAstrometry(std::dynamic_pointer_cast<astrometry::CAstrometryObservation>(astrometryObject));

              astrometryReferenceAdd(astrometryObject);
              astrometryObject.reset();
            };

            controlImage.astroFile->isDirty(true);
            controlImage.astroFile->hasData(true);
            updateWindowTitle();
          };

          DEBUGMESSAGE("Completed adding objects to Astrometry list.");
        };

        if (settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_ADD_PHOTOMETRY, QVariant(false)).toBool())
        {
          DEBUGMESSAGE("Adding objects to Photometry list...");

          bool bClose = false;
          photometry::PPhotometryObservation photometryObject;
          ACL::SPPhotometryObservation existingObject;

          for (auto iter : imageObjectList)
          {
            if (controlImage.astroFile->hasPhotometryHDB())
            {
              existingObject = controlImage.astroFile->photometryObjectFirst();

              while ( (existingObject) && !bClose)
              {
                bClose = existingObject->isClose(iter->center, pdw.getRadius2());
                existingObject = controlImage.astroFile->photometryObjectNext();
              };
            };

            if (!bClose)
            {
                // Object not already in the list. Add object to the list.

              ACL::PPhotometryAperture photometryAperture(new ACL::CPhotometryApertureCircular(pdw.getRadius1(),
                                                                                               pdw.getRadius2(),
                                                                                               pdw.getRadius3()));

              photometryObject.reset(new photometry::CPhotometryObservation());
              photometryObject->CCDCoordinates(iter->center);

              photometryObject->observedCoordinates() =
                  controlImage.astroFile->getHDB(controlImage.currentHDB)->pix2wcs(photometryObject->CCDCoordinates());

              photometryObject->photometryAperture(photometryAperture);
              photometryObject->exposure() = controlImage.astroFile->getHDB(controlImage.currentHDB)->EXPOSURE();
              photometryObject->gain(static_cast<FP_t>(controlImage.astroFile->getHDB(controlImage.currentHDB)->keywordData(ACL::SBIG_EGAIN)));
              photometryObject->FWHM(controlImage.astroFile->FWHM(controlImage.currentHDB, iter->center));

              try
              {
                controlImage.astroFile->pointPhotometry(controlImage.currentHDB, photometryObject);    // Now perform the photometry on the object

                  // Give the object a temporary name and add it into the two lists.

                QString objectName = QString("P:%1").arg(controlImage.astroFile->photometryObjectCount() + 1);
                photometryObject->objectName(objectName.toStdString());

                  // Add the photometry observation to the reference list.

                if ( !controlImage.astroFile->hasPhotometryHDB() )
                {
                  ACL::CHDBPhotometry *phdb = controlImage.astroFile->createPhotometryHDB();
                  phdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
                  phdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
                };

                controlImage.astroFile->photometryObjectAdd(photometryObject);

                controlImage.photometryObservations.push_back(photometryObject);          // Add the objects into the vector.

                  // Now draw the photometry indicator

                changePhotometrySelection(photometryObject);

                  // Update the information in the dockwidget.

                pdw.addNewObject(std::dynamic_pointer_cast<photometry::CPhotometryObservation>(photometryObject));
                pdw.displayPhotometry(std::dynamic_pointer_cast<photometry::CPhotometryObservation>(photometryObject));

                photometryReferenceAdd(photometryObject);
              }
              catch(...)
              {
                  // The pointPhotometry function can throw for out of bounds. In this case, the best response is to do nothing and
                  // to carry on with the rest of the objects.
              };
            };
          };
          DEBUGMESSAGE("Completed adding objects to Photometry list.");

            // Update the image and window characteristics.

          controlImage.astroFile->isDirty(true);
          controlImage.astroFile->hasData(true);
          updateWindowTitle();

        };
      };
      TRACEEXIT;
    }

    /// Procedure to handle the mouse press when the window is in Astronometry Mode.
    /// @version 2017-07-03/GGB - Updated to new style dockwidget storage.
    /// @version 2013-08-27/GGB - Added code to prevent duplicate object selection. (Bug #1210902)
    /// @version 2013-08-25/GGB - Changed code to support the changedAstrometrySelection() function.
    /// @version 2013-08-03/GGB - Moved code from dockwidgetAstrometry into this function to align with requirements of ImageComparison.
    /// @version 2013-02-03/GGB - Dockwidget->referenCAstronomicalCoordinatesompleted changed to return void.
    /// @version 2011-12-20/GGB - Function created.

    void CImageWindow::mousePressAstrometry(QMouseEvent *mouseEvent)
    {
      auto &dw = dynamic_cast<dockwidgets::CAstrometryDockWidget &>
          (dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL));
      QPointF point;
      astrometry::PAstrometryObservation astrometryObject(new astrometry::CAstrometryObservation());
      bool bClose;
      ACL::SPAstrometryObservation existingObject;

      ACL::CAstroImage *astroImage = controlImage.astroFile->getAstroImage(controlImage.currentHDB);
      if (!astroImage)
      {
        CODE_ERROR(astroManager);
      }

      switch (mouseEvent->button())
      {
      case Qt::LeftButton:
      {
        point = gvImage->mapToScene(mouseEvent->pos());                         // Coordinates of the mouse.

        std::optional<MCL::TPoint2D<ACL::FP_t>> centroid =
            astroImage->centroid(MCL::TPoint2D<ACL::AXIS_t>(point.x(), point.y()),
                                 settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS,
                                                               QVariant(20)).toLongLong(),
                                 settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                               QVariant(3)).toInt());

        if (centroid)
        {
          // Check the list for another target that is close.

          bClose = false;

          if (controlImage.astroFile->hasAstrometryHDB())
          {
            existingObject = controlImage.astroFile->astrometryObjectFirst();

            while ( (existingObject) && !bClose)
            {
              bClose = existingObject->isClose(MCL::TPoint2D<FP_t>(point.x(), point.y()),
                                               settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS).toInt());
              existingObject = controlImage.astroFile->astrometryObjectNext();
            };
          };

          if (!bClose)
          {
            astrometryObject->CCDCoordinates(*centroid);

            // Get the image coordinates and convert to WCS coordinates.

            std::optional<ACL::CAstronomicalCoordinates> WCSCoordinates =
                controlImage.astroFile->getHDB(controlImage.currentHDB)->pix2wcs(astrometryObject->CCDCoordinates());

            if (WCSCoordinates)
              astrometryObject->observedCoordinates(*WCSCoordinates);

            QString objectName = QString("A:%1").arg(controlImage.astroFile->astrometryObjectCount() + 1);
            astrometryObject->objectName(objectName.toStdString());

            // Add the astrometry observation to the reference list.

            if ( !controlImage.astroFile->hasAstrometryHDB() )
            {
              ACL::CHDBAstrometry *ahdb = controlImage.astroFile->createAstrometryHDB();
              ahdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
              ahdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
            };

            controlImage.astroFile->astrometryObjectAdd(astrometryObject);
            controlImage.astrometryObservations.push_back(astrometryObject);          // Add the objects into the vector.

            changeAstrometrySelection(astrometryObject);

            astrometryReferenceAdd(astrometryObject);

            controlImage.astroFile->isDirty(true);
            controlImage.astroFile->hasData(true);
            updateWindowTitle();
            dw.referenceCompleted(astrometryObject);
          };
        }
        else
        {
          QMessageBox::information(this, tr("Unable to find centroid."),
                                   tr("A centroid was not found at the indicated spot."),
                                   QMessageBox::Ok, QMessageBox::Ok);
        };
      };
        break;
      case Qt::RightButton:
      case Qt::MidButton:
      case Qt::NoButton:
        break;
      default:
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Handles the mouse press event when the window is in the photometry mode.
    /// @param[in] mouseEvent - The mouse event data
    /// @throws GCL::CRuntimeAssert(astroManager)
    /// @version 2013-07-27/GGB - Added code to catch the error when the photometry overlaps the edge. (Bug #1205629)
    /// @version 2011-12-20/GGB - Function created.

    void CImageWindow::mousePressPhotometry(QMouseEvent *mouseEvent)
    {
      QPointF point;
      bool bClose;
      auto &pw = dynamic_cast<dockwidgets::CPhotometryDockWidget &>
          (dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));
      photometry::PPhotometryObservation photometryObject;
      ACL::SPPhotometryObservation existingObject;

      switch (mouseEvent->button())
      {
        case Qt::LeftButton:
        {
          try
          {
            point = gvImage->mapToScene(mouseEvent->pos());
            std::optional<MCL::TPoint2D<ACL::FP_t> > centroid =
                controlImage.astroFile->centroid(controlImage.currentHDB, MCL::TPoint2D<ACL::AXIS_t>(point.x(), point.y()),
                                                 settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_RADIUS,
                                                                               QVariant(20)).toInt(),
                                                 settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                               QVariant(3)).toInt());

            if (centroid)
            {

                // Check the list for another target that is close.

              bClose = false;

              if (controlImage.astroFile->hasPhotometryHDB())
              {
                existingObject = controlImage.astroFile->photometryObjectFirst();

                while ( (existingObject) && !bClose)
                {
                  bClose = existingObject->isClose(MCL::TPoint2D<FP_t>(point.x(), point.y()), pw.getRadius2());
                  existingObject = controlImage.astroFile->photometryObjectNext();
                };
              };

              if (!bClose)
              {
                // Object not already in the list.
                // Add object to the list.

                ACL::PPhotometryAperture photometryAperture(new ACL::CPhotometryApertureCircular(pw.getRadius1(),
                                                                                                 pw.getRadius2(),
                                                                                                 pw.getRadius3()));

                photometryObject.reset(new photometry::CPhotometryObservation());
                photometryObject->CCDCoordinates(*centroid);

                photometryObject->observedCoordinates() =
                    controlImage.astroFile->getHDB(controlImage.currentHDB)->pix2wcs(photometryObject->CCDCoordinates());

                photometryObject->photometryAperture(photometryAperture);
                photometryObject->exposure() = controlImage.astroFile->getHDB(controlImage.currentHDB)->EXPOSURE();
                photometryObject->gain(static_cast<FP_t>(controlImage.astroFile->getHDB(controlImage.currentHDB)->keywordData(ACL::SBIG_EGAIN)));
                photometryObject->FWHM(controlImage.astroFile->FWHM(controlImage.currentHDB, MCL::TPoint2D<FP_t>(point.x(), point.y())));

                controlImage.astroFile->pointPhotometry(controlImage.currentHDB, photometryObject);    // Now perform the photometry on the object

                  // Give the object a temporary name and add it into the two lists.

                QString objectName = QString("P:%1").arg(controlImage.astroFile->photometryObjectCount() + 1);
                photometryObject->objectName(objectName.toStdString());

                  // Add the photometry observation to the reference list.

                if ( !controlImage.astroFile->hasPhotometryHDB() )
                {
                  ACL::CHDBPhotometry *phdb = controlImage.astroFile->createPhotometryHDB();
                  phdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
                  phdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
                };

                controlImage.astroFile->photometryObjectAdd(photometryObject);

                controlImage.photometryObservations.push_back(photometryObject);          // Add the objects into the vector.

                  // Now draw the photometry indicator

                changePhotometrySelection(photometryObject);

                  // Update the information in the dockwidget.

                pw.addNewObject(std::dynamic_pointer_cast<photometry::CPhotometryObservation>(photometryObject));
                pw.displayPhotometry(std::dynamic_pointer_cast<photometry::CPhotometryObservation>(photometryObject));

                photometryReferenceAdd(photometryObject);

                  // Update the image and window characteristics.

                controlImage.astroFile->isDirty(true);
                controlImage.astroFile->hasData(true);
                updateWindowTitle();
              };
            }
            else
            {
              QMessageBox::information(this, tr("Unable to find centroid."),
                                       tr("A centroid was not found at the indicated spot."),
                                       QMessageBox::Ok, QMessageBox::Ok);
            };
          }
          catch (GCL::CError &error)
          {
            if (error.errorCode() == 0x0003)
            {
              QMessageBox msgBox;

              msgBox.setText(QString::fromStdString("Code Error in ACL Library"));
              msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.setDefaultButton(QMessageBox::Ok);
              msgBox.setIcon(QMessageBox::Warning);
              msgBox.setInformativeText(QString::fromStdString(error.errorMessage()));
              msgBox.exec();
            }
            else
            {
              throw;      // Rethrow the message.
            };
          };
          break;
        };
        case Qt::RightButton:
        {
          break;
        };
        case Qt::MidButton:
        {
          break;
        };
        case Qt::NoButton:
        {
          break;
        };
        default:
        {
          CODE_ERROR(astroManager);
        };
      };
    }

    /// @brief Called when the photometry tab is activating. Allows the program to disable/enable menu items as required.
    /// @throws GCL::CRuntimeAssert(astroManager)
    /// @version  2016-04-25/GGB - Added action for IDA_ANALYSIS_LOADOBJECTS
    /// @version 2016-04-24/GGB - Bug# 1574157 - Ensure the view photometry and astrometry are in the correct state.
    /// @version 2013-05-12/GGB - Added the export CSV option.
    /// @version 2013-03-31/GGB - Function created.

    void CImageWindow::photometryTabActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      RUNTIME_ASSERT(astroManager, pw != nullptr, "The parent widget should exist.")

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_DNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_JPEG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_PNG)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORTIMAGE_TIFF)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_EXPORT_CSV)->setEnabled(true);

        // Edit menu

      pw->getAction(mdiframe::IDA_EDIT_ANNOTATEIMAGE)->setEnabled(false);

        // View Menu

      pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->setEnabled(false);
      pw->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->setEnabled(false);

        // Process menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);

        // Analyse actions

      pw->getAction(mdiframe::IDA_ANALYSIS_EXTRACTOBJECTS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_ANALYSIS_LOADOBJECTS)->setEnabled(false);
    }

    /// @brief Function to load photometry targets and apply to current image.
    /// @throws None.
    /// @version 2017-07-03/GGB - Use new style dockwidget storage.
    /// @version 2014-12-30/GGB - Use GCL::logger rather than std::clog.
    /// @version 2013-08-21/GGB - Function created.

    void CImageWindow::photometryLoadTargets()
    {
      auto &pw = dynamic_cast<dockwidgets::CPhotometryDockWidget &>
          (dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));
      std::vector<photometry::PPhotometryObservation> photometryTargets;
      std::vector<photometry::PPhotometryObservation>::iterator targetIterator;

        // Check if there is WCS information present in the current image. If there is no WCS information present in the image,
        // then the function should return with an error to the user.

      LOGMESSAGE(info, "Starting Load Photometry Targets...");

      std::optional<ACL::CAstronomicalCoordinates> wcsCoords = controlImage.astroFile->getHDB(controlImage.currentHDB)->pix2wcs(MCL::TPoint2D<FP_t>(0, 0));

      if (wcsCoords)
      {
          // There is WCS information present in the current image. Query the user for the filename of the Photometry file.

        QString szFileName = QFileDialog::getOpenFileName(this, tr("Open Photometry Target File"),
          settings::astroManagerSettings->value(settings::PHOTOMETRY_TARGET_DIRECTORY, QVariant(0)).toString(), EXTENSION_CSV);

        if (!szFileName.isEmpty())
        {
          size_t lineNumber = 0;

            // Have the file name. Open the file and read all the information into an array, while parsing the data

          try
          {
            std::ifstream csvFile(szFileName.toStdString());
            std::string szLine, szValue;

            while (csvFile.good())
            {
              std::getline(csvFile, szLine);
              if (szLine.size() != 0)
              {
                FP_t RA, Dec;

                photometry::PPhotometryObservation currentTarget(new photometry::CPhotometryObservation);

                  // Now parse the data.

                size_t comma1, comma2, comma3;
                ACL::CAstronomicalCoordinates coordinates;

                comma1 = szLine.find(',', 0);
                comma2 = szLine.find(',', comma1 + 1);
                comma3 = szLine.find(',', comma2 + 1);

                currentTarget->objectName(szLine.substr(0, comma1));    // Object Name

                szValue = szLine.substr(comma1 + 1, comma2 - comma1 - 1);     // RA
                boost::trim(szValue);
                RA = boost::lexical_cast<FP_t>(szValue);

                szValue = szLine.substr(comma2 + 1, comma3 - comma2 - 1);
                boost::trim(szValue);
                Dec = boost::lexical_cast<FP_t>(szValue);

                coordinates(RA, Dec);

                currentTarget->observedCoordinates(coordinates);

                photometryTargets.push_back(currentTarget);
              };

              ++lineNumber;
            };
          }
          catch(...)
          {
            QMessageBox::information(this, tr("Error while processing file."),
                                     tr("Error while processing the file."),
                                     QMessageBox::Ok, QMessageBox::Ok);
            LOGMESSAGE(warning, "Error while loading photometry targets in line: " + std::to_string(lineNumber) + ".");
            return;
          };

          // Have all the data. Convert the RA/Dec to CCD coordinates

          for (targetIterator = photometryTargets.begin(); targetIterator != photometryTargets.end(); ++targetIterator)
          {
            std::optional<MCL::TPoint2D<FP_t>> result = controlImage.astroFile->wcs2pix(controlImage.currentHDB,
                                                                                          *(*targetIterator)->observedCoordinates());
            if (result)
              (*targetIterator)->CCDCoordinates(*result);
            else
            {
              LOGMESSAGE(warning, (*targetIterator)->objectName() + ": Coordinates not on image. Deleting photometry target.");
              targetIterator = photometryTargets.erase(targetIterator);
            };
          }

          // Have all the CCD coordinates, now centroid the coordinates, and add to the photometry list.

          size_t targetCount = 0;

          for (targetIterator = photometryTargets.begin(); targetIterator != photometryTargets.end(); ++targetIterator)
          {
            MCL::TPoint2D<AXIS_t> point = (*targetIterator)->CCDCoordinates();

            std::optional<MCL::TPoint2D<ACL::FP_t> > centroid =
                controlImage.astroFile->centroid(controlImage.currentHDB, point,
                                                 settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_RADIUS,
                                                                               QVariant(20)).toInt(),
                                                 settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                               QVariant(3)).toInt());

            if (centroid)
            {
              ACL::SPPhotometryObservation existingObject;

                // Check the list for another target that is close.

              bool bClose = false;

              if (controlImage.astroFile->hasPhotometryHDB())
              {
                existingObject = controlImage.astroFile->photometryObjectFirst();

                while ( (existingObject) && !bClose)
                {
                  bClose = existingObject->isClose(point, pw.getRadius2());
                  existingObject = controlImage.astroFile->photometryObjectNext();
                };
              };

              if (!bClose)
              {

                  // Object not already in the list. Add object to the list.

                try
                {
                  photometry::PPhotometryObservation photometryObject;

                  ACL::PPhotometryAperture photometryAperture(new ACL::CPhotometryApertureCircular(pw.getRadius1(),
                                                                                                   pw.getRadius2(),
                                                                                                   pw.getRadius3()));

                  photometryObject.reset(new photometry::CPhotometryObservation());
                  photometryObject->objectName( (*targetIterator)->objectName());
                  photometryObject->CCDCoordinates(*centroid);
                  photometryObject->observedCoordinates( *(*targetIterator)->observedCoordinates() );
                  photometryObject->photometryAperture(photometryAperture);
                  photometryObject->exposure() = controlImage.astroFile->getHDB(controlImage.currentHDB)->EXPOSURE();
                  photometryObject->gain(static_cast<FP_t>(controlImage.astroFile->getHDB(controlImage.currentHDB)->keywordData(ACL::SBIG_EGAIN)));
                  photometryObject->FWHM(controlImage.astroFile->FWHM(controlImage.currentHDB, point));
                  controlImage.astroFile->pointPhotometry(controlImage.currentHDB, photometryObject);    // Now perform the photometry on the object

                    // Add the photometry observation to the list.

                  if ( !controlImage.astroFile->hasPhotometryHDB() )
                  {
                    ACL::CHDBPhotometry *phdb = controlImage.astroFile->createPhotometryHDB();
                    phdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
                    phdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
                  };

                  ++targetCount;
                  controlImage.astroFile->photometryObjectAdd(photometryObject);
                  controlImage.photometryObservations.push_back(photometryObject);          // Add the objects into the vector.

                    // Now draw the photometry indicator

                  changePhotometrySelection(photometryObject);

                    // Update the information in the dockwidget.

                  pw.addNewObject(std::dynamic_pointer_cast<photometry::CPhotometryObservation>(photometryObject));
                  pw.displayPhotometry(std::dynamic_pointer_cast<photometry::CPhotometryObservation>(photometryObject));

                  photometryReferenceAdd(photometryObject);

                  LOGMESSAGE(info, photometryObject->objectName() + ": Added to photometry list sucesfully.");
                }
                catch(...)
                {
                  LOGMESSAGE(warning, "Unable to add " + (*targetIterator)->objectName() + ". Error while performing photometry.");
                };
              };
            }
            else
            {
              LOGMESSAGE(warning, "Unable to add " + (*targetIterator)->objectName() + ". Could not find centroid.");
            };
          };

            // Indicate the number of points applied sucesfully.

          LOGMESSAGE(info, "Added " + std::to_string(targetCount) + " photometry targets to image.");
          LOGMESSAGE(info, "Load Photometry targets completed.");

            // Update the image and window characteristics.

          controlImage.astroFile->isDirty(true);
          controlImage.astroFile->hasData(true);
          updateWindowTitle();
        };
      }
      else
      {
        QMessageBox::information(this, tr("There is no WCS information for this image."),
                                 tr("Without WCS information, it is not possible to load and apply a list of photometry targets."),
                                 QMessageBox::Ok, QMessageBox::Ok);
        LOGMESSAGE(warning, "Load Photometry Targets Error: No WCS information for image.");
      };
    }

    /// @brief Function called when the dockwidget needs to add a new reference to the image window.
    /// @param[in] po - The photometry object to add to the HDU.
    /// @throws None.
    /// @details  Using this function as the entry point allows the FITS headers and information to be updated without having to
    /// add special calls back to the CImageWidow.
    ///   @li Get all the information needed to create the photometryObservation
    ///   @li Call the astroFile to add the observation to the astroFile.
    ///   @li If the photometry HDB is created, then the combo box needs to be updated and a tab needs to be opened to display the
    /// astrometry information
    ///   @li The photometry information in the photometry HDB needs to be updated.
    /// @version 2017-09-23/GGB - Update to use CAngle
    /// @version 2013-08-17/GGB - Corrected RA/Dec display (Bug #1213076)
    /// @version 2013-03-30/GGB - Function created

    void CImageWindow::photometryReferenceAdd(photometry::PPhotometryObservation po)
    {
      int nColumn = 0, nRow;

        // Need to add the photometry HDU to the list of available HDU's?

      if ( cbHDU->findText(QString::fromStdString(ACL::ASTROMANAGER_HDB_PHOTOMETRY)) == -1 )
      {
        cbHDU->addItem(QString::fromStdString(ACL::ASTROMANAGER_HDB_PHOTOMETRY));
      };

        // Add the items into the table for references

      nRow = tableWidgetPhotometry->rowCount();
      tableWidgetPhotometry->insertRow(nRow);
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString::fromStdString(po->objectName())));
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("(%1, %2)").arg(po->CCDCoordinates().x()).
                                                                           arg(po->CCDCoordinates().y())));
      if (po->observedCoordinates())
      {
        tableWidgetPhotometry->setItem(nRow, nColumn++,
                                       new QTableWidgetItem(QString::fromStdString(po->observedCoordinates()->RA().A2SHMS())));
        tableWidgetPhotometry->setItem(nRow, nColumn++,
                                       new QTableWidgetItem(QString::fromStdString(po->observedCoordinates()->DEC().A2SDMS())));
      }
      else
      {
        tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("--h--'--""")));
        tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("--" % UTF16_DEGREESIGN % "--'--""")));
      };
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(*(po->instrumentMagnitude()))));
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->magnitudeError())));
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->sourceADU())));
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->sourceArea())));
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->skyADU())));
      tableWidgetPhotometry->setItem(nRow, nColumn++, new QTableWidgetItem(QString("%1").arg(po->skyArea())));
    }

    /// @brief Function to print. Called from the parent window.
    /// @throws None.
    /// @version 2013-03-29/GGB - Function created.

    void CImageWindow::print()
    {
      // display print dialog and if accepted print
      QPrinter       printer( QPrinter::HighResolution );
      QPrintDialog   dialog( &printer, this );
      if ( dialog.exec() == QDialog::Accepted ) printImage( &printer );
    }

    /// @brief Function to actually print the image.
    //
    // 2013-03-29/GGB - Function created.

    void CImageWindow::printImage(QPrinter *printer)
    {
      QPainter painter(printer);
      int w = printer->pageRect().width();
      int h = printer->pageRect().height();
      QRect  page( 0, 0, w, h );

        // create a font appropriate to page size

      QFont font = painter.font();
      font.setPixelSize( (w+h) / 100 );
      painter.setFont( font );

        // draw labels in corners of page

      painter.drawText( page, Qt::AlignTop    | Qt::AlignLeft, "astroManager" );
      painter.drawText( page, Qt::AlignBottom | Qt::AlignLeft, QString(getenv("USER")) );
      painter.drawText( page, Qt::AlignBottom | Qt::AlignRight, QDateTime::currentDateTime().toString( Qt::DefaultLocaleShortDate));

        // draw simulated landscape

      page.adjust( w/20, h/20, -w/20, -h/20 );
      gsImage->render( &painter, page );
    }

    /// @brief Perform the print preview function.
    /// @throws None.
    /// @version 2013-03-29/GGB - Function created.

    void CImageWindow::printPreview()
    {
      QPrinter printer(QPrinter::HighResolution);
      QPrintPreviewDialog printPreviewDialog(&printer, this);
      connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printImage(QPrinter *)));
      printPreviewDialog.exec();
    }

    void CImageWindow::printSetup()
    {
    }

    /// @brief Redraws the image and other elements as required when flags or items change.
    /// @throws None.
    /// @version 2013-06-22/GGB - Changed control order and use function imageChanged(...)
    /// @version 2013-05-24/GGB - Added support for the navigator window.
    /// @version 2013-05-20/GGB - Added support for the magnify image.
    /// @version 2013-03-17/GGB - Function created.

    void CImageWindow::redrawImage()
    {
        // Inform the dockwidgets that the image has changed and needs to be redrawn.

      imageChange(&controlImage);

      repaintImage();                   // This needs to follow the line above. (GGB)
    }

    void CImageWindow::repaintAnnotations()
    {
    }

    /// @brief Redraws the astrometry indicators as required.
    /// @throws None.
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-05-16/GGB - Function created.

    void CImageWindow::repaintAstrometry()
    {
      QPen pen;
      astrometry::DAstrometryObservationStore::iterator iterator;

      pen.setColor(settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_COLOUR, QVariant(QColor(Qt::red))).value<QColor>());

      for(iterator = controlImage.astrometryObservations.begin(); iterator != controlImage.astrometryObservations.end(); iterator++)
      {
        drawCrossIndicator(*iterator, pen);
        gsImage->addItem((*iterator)->group);
      };
    }

    /// @brief Repaints the image as required.
    /// @note NOTE: This is the routine that changes the image on the screen when the image is updated.
    /// @throws None.
    /// @version 2013-05-20/GGB - Added pixmap to control image.
    /// @version 2013-03-17/GGB - Function created.

    void CImageWindow::repaintImage()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw, "Parent Widget should not == nullptr");

        // This is the code that updates the screen when the image needs updating.

      gsImage->clear();         // This invalidates the pixmapItem as the scene owns the pixmapItem.
      gsImage->addPixmap(*controlImage.pixmap);
      gvImage->Paint();         // NOTE: Any code that updates the image needs to ensure that this is called!!!

      if (pw->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->isChecked())
      {
        repaintAstrometry();
      };

      if (pw->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->isChecked())
      {
        repaintPhotometry();
      };

      if (pw->getAction(mdiframe::IDA_VIEW_ANNOTATIONS)->isChecked())
      {
        repaintAnnotations();
      };
    }

    /// Function to repaint the photometry indicators when the image is loaded, or redisplayed.
    /// @throws None
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-05-10/GGB - Function created.

    void CImageWindow::repaintPhotometry()
    {
      QPen pen;
      QColor normalColor = settings::astroManagerSettings->value(settings::PHOTOMETRY_INDICATOR_COLOUR,
                                                         QVariant(QColor(Qt::red))).value<QColor>();
      photometry::DPhotometryObservationStore::iterator iterator;

      pen.setColor(normalColor);

      for(iterator = controlImage.photometryObservations.begin(); iterator != controlImage.photometryObservations.end(); iterator++)
      {
        drawPhotometryIndicator(*iterator, pen);
        gsImage->addItem((*iterator)->group);
      };
    }

    /// @brief Procedure called when the image is to be resampled.
    /// @throws GCL::CCodeError
    /// @details Brings up the dialog box for the sizing.
    /// @version 2015-08-16/GGB
    ///   @li Use a stack allocation for dlg
    ///   @li Update to use cfitsio
    ///   @li Use function ACL::Bitpix2String() to convert BITPIX values to string values.
    /// @version 2014-12-30/GGB - Use GCL::logger rather than std::clog for message logging.
    /// @version 2013-06-27/GGB
    ///   @li Bug #1162197. If bitpix value changes, needs to be reflected in the fits info tab.
    ///   @li Added historyUpdate()
    /// @version 2013-02-02/GGB - Added support for the histogram widget.
    /// @version 2011-06-07/GGB - Function created.

    void CImageWindow::resampleImage()
    {
      AXIS_t ox, oy;
      int oldBitpix, newBitpix;
      std::string szComment, szValue;

        // Get the size of the resampled image

      ox = controlImage.astroFile->imageWidth();
      oy = controlImage.astroFile->imageHeight();

      dialogs::CImageResampleDialog dlg(controlImage.astroFile->getAstroImage(controlImage.currentHDB));


      if (dlg.exec() == QDialog::Accepted)
      {
        oldBitpix = controlImage.astroFile->getHDB(controlImage.currentHDB)->BITPIX();
        controlImage.astroFile->imageResample(controlImage.currentHDB, dlg.getWidth(), dlg.getHeight());
        newBitpix = controlImage.astroFile->getHDB(controlImage.currentHDB)->BITPIX();

        historyUpdate();

        LOGMESSAGE(info, "Resampled Image: " + controlImage.astroFile->getImageName() + ". Original Size: " + std::to_string(ox) +
                   " x " + std::to_string(oy) + ". New Size: " + std::to_string(dlg.getWidth()) + " x " +
                   std::to_string(dlg.getHeight()) + ".");

        if (newBitpix != oldBitpix)
        {
          szComment = "BITPIX Changed: Original BITPIX = ";

          szComment += ACL::Bitpix2String(oldBitpix);
          szComment += ". New BITPIX = ";
          szComment += ACL::Bitpix2String(newBitpix);
          LOGMESSAGE(info, szComment);
          szComment.clear();

            // Update the table widget item.

          int rowCount = twHDU->rowCount();

          for (int row = 0; row < rowCount; ++row)
          {
            if (twHDU->item(row, 0)->text() == QString::fromStdString(ACL::FITS_BITPIX) )
            {
              if (controlImage.astroFile->keywordData(controlImage.currentHDB, ACL::FITS_BITPIX, szValue, szComment) )
                twHDU->item(row, 1)->setText(QString::fromStdString(szValue));
              else
                CODE_ERROR(astroManager);
            };
          };
        };

          // Update the screen image.

        redrawImage();
        zoomAll();
        updateWindowTitle();
      };
    }

    /// @brief Called when the image needs to be rotated by an angle.
    /// @throws None.
    /// @details Dialog box is used to get angle of rotation
    /// @version 2015-09-19/GGB - Use word "degrees" instead of UTF16 character for logging.
    /// @version 2014-12-30/GGB
    ///   @li Use GCL::logger for message logging instead of std::clog.
    ///   @li Change dynamic allocation of dialog to stack allocation. (Leaking memory)
    /// @version 2013-06-28/GGB - Added historyUpdate() and clog output.
    /// @version 2013-03-01/GGB - Added tests for the dockwidget pointers.
    /// @version 2013-02-02/GGB - Added support for the histogram widget.
    /// @version 2011-05-31/GGB - Function created.

    void CImageWindow::rotateImage()
    {
      FP_t angle;

        // Get the angle of rotation.

      dialogs::CRotateImageDialog dlg(&angle);

      if (dlg.exec() == QDialog::Accepted)
      {
        controlImage.astroFile->rotateImage(controlImage.currentHDB, angle * DD2R);

        historyUpdate();

        LOGMESSAGE(info, "Image: " + controlImage.astroFile->getImageName() + ". Rotated: " +
                   boost::str(boost::format("%.2f") % angle) + "degrees.");

          // Update the screen image.

        redrawImage();

        if (controlImage.astroFile->isDirty())
        {
          updateWindowTitle();
        };
      };
    }

    /// @brief Handles the save function from the menu
    /// @throws CCodeError
    /// @version 2017-09-03/GGN - Changed to use the save() function in CAstroFile
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2011-06-04/GGB - Function created.

    bool CImageWindow::save()
    {
      return controlImage.astroFile->save();
    }

    /// @brief Menu File | Save As
    /// @throws None.
    /// @version 2017-09-03/GGN - Changed to use the saveAs() function in CAstroFile
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2015-09-19/GGB - Updated function flow to use variable returnValue.
    /// @version 2011-06-04/GGB - Function created.

    bool CImageWindow::saveAs()
    {
      return controlImage.astroFile->saveAs();
    }

    /// @brief Pops up the dialog box to ask if the window should be saved before it is closed.
    /// @throws GCL::CodeError(astroManager)
    /// @version 2013-03-15/GGB - Updated function to operate correctly.
    /// @version 2013-02-02/GGB - Function created.

    bool CImageWindow::saveBeforeClose()
    {
      QMessageBox msgBox;
      int returnValue;

      if (controlImage.astroFile->isDirty())
      {
        msgBox.setText(tr("Save before closing?"));
        msgBox.setInformativeText(tr("Save the file before the window is closed?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        returnValue = msgBox.exec();
        if (returnValue == QMessageBox::Save)
        {
          return save();
        }
        else if (returnValue == QMessageBox::Discard)
        {
          return true;
        }
        else if (returnValue == QMessageBox::Cancel)
        {
          return false;
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else
        return true;
    }

    /// @brief Function used when an image is going to have a star reference inserted.
    /// @throws CCodeError
    /// @details Send message to graphics window to tell it that it in astrometry mode.
    /// @version 2011-06-14/GGB - Function created

    void CImageWindow::setMode(EMode newMode)
    {
      mode = newMode;
      if (mode == M_ASTROMETRY)
      {
        if (gvImage)
        {
          gvImage->setMode(CAstroGraphicsView::M_ASTROMETRY);
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else if (mode == M_PHOTOMETRY)
      {
        if (gvImage)
        {
          gvImage->setMode(CAstroGraphicsView::M_PHOTOMETRY);
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      else if (mode == M_NONE)
      {
        if (gvImage)
        {
          gvImage->setMode(CAstroGraphicsView::M_NONE);
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      };
    }

    /// @brief Sets up the user interface. Called from the constructor.
    /// @throws CRuntimeAssert
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2015-07-30/GGB - Replaced some control checks with runtime asserts.
    /// @version 2012-01-09/GGB - Added functionality for the astrometry tab and functions.
    /// @version 2010-10-17/GGB - Function created

    void CImageWindow::SetupUI()
    {
      QUiLoader loader;

        // Create the window details from the template

      QFile file(":/forms/windowFITSFile.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/windowFITSFile.ui.");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      ASSOCIATE_LABEL(labelDateTime, formWidget, "labelDateTime");
      ASSOCIATE_LABEL(labelTarget, formWidget, "labelTarget");
      ASSOCIATE_LABEL(labelTargetRA, formWidget, "labelTargetRA");
      ASSOCIATE_LABEL(labelTargetDEC, formWidget, "labelTargetDEC");
      ASSOCIATE_LABEL(labelExposure, formWidget, "labelExposure");
      ASSOCIATE_LABEL(labelObservatory, formWidget, "labelObservatory");
      ASSOCIATE_LABEL(labelTelescope, formWidget, "labelTelescope");
      ASSOCIATE_LABEL(labelInstrument, formWidget, "labelInstrument");
      ASSOCIATE_LABEL(labelLatitude, formWidget, "labelLatitude");
      ASSOCIATE_LABEL(labelLongitude, formWidget, "labelLongitude");
      ASSOCIATE_LABEL(labelAltitude, formWidget, "labelAltitude");
      ASSOCIATE_LABEL(labelTemperature, formWidget, "labelTemperature");
      ASSOCIATE_LABEL(labelPressure, formWidget, "labelPressure");
      ASSOCIATE_LABEL(labelRH, formWidget, "labelRH");

        // Get the widget addresses

      ASSOCIATE_CONTROL(cbHDU, formWidget, "cbHDU", QComboBox);

      ASSOCIATE_CONTROL(teComments, formWidget, "teComments", QPlainTextEdit);
      ASSOCIATE_CONTROL(teHistory, formWidget, "teHistory", QPlainTextEdit);

      ASSOCIATE_CONTROL(tabWidget, formWidget, "tabWidget", QTabWidget);

        // Get the widgets for the various tab areas.

      imageTab = tabWidget->widget(1);
      atableTab = tabWidget->widget(2);
      btableTab = tabWidget->widget(3);

        // Get the widgets for the astrometryTab

      astrometryTab = tabWidget->widget(4);
        tableWidgetAstrometry = formWidget->findChild<QTableWidget *>("tableWidgetAstrometry");

        photometryTab = tabWidget->widget(5);
          tableWidgetPhotometry = formWidget->findChild<QTableWidget *>("tableWidgetPhotometry");

      RUNTIME_ASSERT(astroManager, imageTab != nullptr, "Control imageTab not found.");
      RUNTIME_ASSERT(astroManager, atableTab != nullptr, "Control atableTab not found.");
      RUNTIME_ASSERT(astroManager, btableTab, "Control btableTab not found.");
      RUNTIME_ASSERT(astroManager, astrometryTab, "Control astrometryTab not found.");
      RUNTIME_ASSERT(astroManager, photometryTab, "Control photometryTab not found.");
      RUNTIME_ASSERT(astroManager, tableWidgetAstrometry, "Control tableWidgetAstrometry not found");
      RUNTIME_ASSERT(astroManager, tableWidgetPhotometry, "Control tableWidgetPhotometry not found");

        // Remove unused tabs from the tab view.

      tabWidget->removeTab(1);
      tabWidget->removeTab(1);
      tabWidget->removeTab(1);
      tabWidget->removeTab(1);
      tabWidget->removeTab(1);

      twHDU = formWidget->findChild<QTableWidget *>("twHDU");

        // Create the scene

      QGridLayout *glImage = (QGridLayout *) imageTab->layout();

      gvImage = new CAstroGraphicsView(this);       //**** NOTE: This is the area that displays the image.
      glImage->addWidget(gvImage, 0, 0, 1, 1);      // Only object in the layout

      gsImage = new QGraphicsScene();               //**** NOTE:

      gvImage->setScene(gsImage);                   //**** NOTE: Graphics view displays the scene!

      connect(cbHDU, SIGNAL(currentIndexChanged(int)), this, SLOT(eventCurrentHDUChanged(int)));
      connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(eventTabWidgetChanged(int)));
      connect(formWidget->findChild<QPushButton *>("pushButtonEditWeather"), SIGNAL(clicked()), this, SLOT(eventEditWeather()));

      displayTargetInformation();
      displayEquipmentInformation();
      displaySiteInformation();
      displayWeatherInformation();
    }

    /// @brief Function called by the user interface to solve the WCS for a marked image.
    /// @throws None.
    /// @version 2012-08-12/GGB - Function created.

    void CImageWindow::solveWCS()
    {
      controlImage.astroFile->plateSolve(controlImage.currentHDB, 0);
    }

    /// @brief Updates the window title to show the correct title.
    /// @throws GCL::CodeError(astroManager)
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2013-03-03/GGB - Changed method of operation to use windowModified flag.
    /// @version 2012-01-29/GGB - Repaired bug #101, plus only display filename and not complete path.
    /// @version 2011-06-04/GGB - Function Created

    void CImageWindow::updateWindowTitle()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      RUNTIME_ASSERT(astroManager, pw != nullptr, "The parent widget must exist.");

        // View Menu

      if (controlImage.astroFile)
      {
        if (controlImage.astroFile->isDirty())
        {
          setWindowModified(true);
          pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(true);
        }
        else
        {
          setWindowModified(false);
          pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(false);
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Called when the window is activating.
    /// @details Sets up the enabled status on the actions. Sets the currentImage with the dockwindow.
    /// @throws CCodeError
    /// @throws CRuntimeAssert
    /// @version 2016-04-24/GGB - Use function enableDockwidgets()
    /// @version 2013-08-13/GGB - Added code to select the appropriate action based on the current HDB.
    /// @version 2013-07-03/GGB - Added code for Transform menu. (Bug #1197352)
    /// @version 2013-05-25/GGB - Added support for View | Magnify and View | Navigator
    /// @version 2013-02-02/GGB - Added support for the Histogram widget.
    /// @version 2011-05-29/GGB - Function created.

    void CImageWindow::windowActivating()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw != nullptr,  "Parent widget cannot be nullptr.");

      std::string hdbName = cbHDU->currentText().toStdString();

      switch (controlImage.astroFile->HDBType(hdbName))
      {
        case ACL::BT_IMAGE:
        {
          imageChange(&controlImage);  // Will not update dockwidgets when changing windows if missing!

          // The following calls enable the dock widgets.

          dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(true);

          imageTabActivating();
          break;
        };
        case ACL::HDB_ASTROMETRY:
        {
          imageChange(nullptr);  // Will not update dockwidgets when changing windows if missing!
          astrometryTabActivating();
          break;
        };
        case ACL::HDB_PHOTOMETRY:
        {
          imageChange(nullptr);  // Will not update dockwidgets when changing windows if missing!
          photometryTabActivating();
          break;
        };
        case ACL::BT_NONE:
        default:
        {
          CODE_ERROR(astroManager);
          break;
        };
      };

      // Common Actions

      // File Menu

      if (isWindowModified())
      {
        pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(true);
      }
      else
      {
        pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(false);
      };

      pw->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(true);

    }

    /// @brief Menu function "View | Zoom 1:1
    /// @throws GCL::CRuntimeAssert
    /// @version 2015-08-04/GGB - Added runtime assert rather than comparison to check for gvImage validity.
    /// @version 2012-07-21/GGB - Function created.

    void CImageWindow::zoom11()
    {
      RUNTIME_ASSERT(astroManager, gvImage, "gvImage == nullptr.");

      gvImage->zoom11();
    }

    /// @brief Menu function View | Zoom All
    /// @throws CCodeError
    /// @version 2011-06-02/GGB - Function created.

    void CImageWindow::zoomAll()
    {
      RUNTIME_ASSERT(astroManager, gvImage != nullptr, "gvImage should not be null");

      gvImage->zoomAll();
    }

    /// @brief Menu function View | Zoom In
    /// @throws CCodeError
    /// @version 2011-06-02/GGB - Function created.

    void CImageWindow::zoomIn()
    {
      RUNTIME_ASSERT(astroManager, gvImage != nullptr, "gvImage should not be null");

      gvImage->zoomIn();
    }

    /// @brief Menu function View | Zoom Out
    /// @throws CCodeError
    /// @version 2011-06-02/GGB - Function Created

    void CImageWindow::zoomOut()
    {
      RUNTIME_ASSERT(astroManager, gvImage != nullptr, "gvImage should not be null");

      gvImage->zoomOut();
    }

    /// @brief Menu function View | Zoom Selection
    /// @throws CCodeError
    /// @version 2011-06-02/GGB - Function Created

    void CImageWindow::zoomSelection()
    {
      RUNTIME_ASSERT(astroManager, gvImage != nullptr, "gvImage should not be null");

      gvImage->zoomSelection();
    }

  }  // namespace imagedisplay
}  // namespace AstroManager
