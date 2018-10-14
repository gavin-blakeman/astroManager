//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageComparison.h
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imagedisplay
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
//
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CImageComparisonWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CAstroImageWindow
//                          - CImageComparisonWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../include/ImageComparison.h"

  // astroManager header files

#include "../include/AstroGraphicsView.h"
#include "../include/dialogs/dialogSaveAligned.h"
#include "../include/dockWidgets/dockWidgetAstrometry.h"
#include "../include/dockWidgets/dockWidgetHistogram.h"
#include "../include/dockWidgets/dockWidgetImageInformation.h"
#include "../include/dockWidgets/dockWidgetMagnify.h"
#include "../include/dockWidgets/dockWidgetNavigator.h"
#include "../include/dockWidgets/dockWidgetPhotometry.h"
#include "../include/settings.h"

  // Qt Framework

#include <QtUiTools/QtUiTools>

  // Qxt Library

#include <QxtGui/QxtConfirmationMessage>

  // Other libraries

#include <GCL>

namespace astroManager
{
  namespace imaging
  {

    //*****************************************************************************************************************************
    //
    // SControlBlock
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @param[in] parent - The "owner" window
    /// @throws None.
    /// @version 2016-04-23/GGB - Function created.

    SControlBlock::SControlBlock(imaging::CAstroImageWindow *parent) : inputImage(parent), outputImage(parent)
    {

    }

    //*****************************************************************************************************************************
    //
    // CImageComparisonWindow
    //
    //*****************************************************************************************************************************

    int const BLINK_IMAGES    = 1;
    int const SUBTRACT_IMAGES = 2;
    int const COLOUR_IMAGES   = 3;

    /// @brief Class constructor.
    /// @param[in] aParent - The parent (owner) of this window. Will normally be the frame window.
    /// @throws None
    /// @details Sets up all the data members and then calls setupUI() to create the user interface.
    /// @version 2013-06-19/GGB - Added different definitions for windows and other.
    /// @version 2013-06-07/GGB - Added the blinkTimer.
    /// @version 2011-06-11/GGB - Function created.

#ifdef _WIN32
    CImageComparisonWindow::CImageComparisonWindow(QWidget *aParent) :imaging::CAstroImageWindow(aParent),
      cursorAstrometry(QString(":/cursor/cursorAstrometry.bmp")), maskAstrometry(QString(":/cursor/maskAstrometryWindows.bmp")),
      astrometry(cursorAstrometry, maskAstrometry, 15, 15), blinkTimer(new QTimer(this)), bAllowSave(false)
#else
    CImageComparisonWindow::CImageComparisonWindow(QWidget *aParent) :imaging::CAstroImageWindow(aParent),
      cursorAstrometry(QString(":/cursor/cursorAstrometry.bmp")), maskAstrometry(QString(":/cursor/maskAstrometry.bmp")),
      astrometry(cursorAstrometry, maskAstrometry, -1, -1), blinkTimer(new QTimer(this)), bAllowSave(false)
#endif
    {
      setAttribute(Qt::WA_DeleteOnClose);

      setupUI();
      setWindowTitle(tr("Image Comparison"));
    }

    /// @brief Must ensure that all the controlImage objects are properly deleted.
    /// @throws None.
    /// @version 2016-04-21/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-04-21/GGB - Convert C-style casts to reinterpret_cast<>()
    /// @version 2013-06-10/GGB - Ensure that all the roles are correctly deleted on exit.
    /// @version 2013-06-07/GGB - Added the blinkTimer.
    /// @version 2011-06-12/GGB - Function created.

    CImageComparisonWindow::~CImageComparisonWindow()
    {
      QListWidgetItem *lwi;
      const int itemCount = listWidgetImages->count();
      int index;
      qulonglong ci;

      for(index = 0; index < itemCount; index++)
      {
        lwi = listWidgetImages->item(0);

        ci = lwi->data(ROLE_CONTROLBLOCK).toULongLong();
        if (ci)
        {
          delete (reinterpret_cast<SControlBlock *>(ci));
        }

        delete lwi;
        lwi = nullptr;
      };

        // delete the blink timer.

      if (blinkTimer)
      {
        delete blinkTimer;
        blinkTimer = nullptr;
      };
    }

    /// @brief Adds an astrometry reference into the relevant astroFile.
    /// @param[in] controlImage: The class controlling this image.
    /// @param[in] centroid: The centroid of the object
    /// @param[in] objectName: The name for the newly created object.
    /// @throws None.
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-08-04/GGB - Function created.

    astrometry::CAstrometryObservation *CImageComparisonWindow::astrometryAdd(imaging::SControlImage *controlImage,
                                                                              MCL::TPoint2D<ACL::FP_t> const &centroid,
                                                                              QString const &objectName)
    {
      QPen pen;
      int ai = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_TYPE, QVariant(0)).toInt();

      controlImage->astrometryObservations.emplace_back(std::make_shared<astrometry::CAstrometryObservation>());

      controlImage->astrometryObservations.back()->objectName(objectName.toStdString());
      controlImage->astrometryObservations.back()->CCDCoordinates(centroid);

        // Get the image coordinates and convert to WCS coordinates.

      std::optional<ACL::CAstronomicalCoordinates> WCSCoordinates =
        controlImage->astroFile->getHDB(controlImage->currentHDB)->pix2wcs(controlImage->astrometryObservations.back()->CCDCoordinates());

      if (WCSCoordinates)
      {
        controlImage->astrometryObservations.back()->observedCoordinates(*WCSCoordinates);
      };

        // Add the astrometry observation to the reference list.

      if ( !controlImage->astroFile->hasAstrometryHDB() )
      {
        ACL::CHDBAstrometry *ahdb = controlImage->astroFile->createAstrometryHDB();
        ahdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
        ahdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
      };

      controlImage->astroFile->astrometryObjectAdd(controlImage->astrometryObservations.back());

      pen.setColor(settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_COLOUR, QVariant(QColor(Qt::red))).toInt());

      switch (ai)
      {
      case 0:
        drawCrossIndicator(controlImage->astrometryObservations.back().get(), pen);
        break;
      case 1:
        drawCircleIndicator(controlImage->astrometryObservations.back().get(), pen);
        break;
      default:
        drawCrossIndicator(controlImage->astrometryObservations.back().get(), pen);
        break;
      };

      controlImage->astroFile->isDirty(true);
      controlImage->astroFile->hasData(true);

      return controlImage->astrometryObservations.back().get();
    }

    /// @brief Ensures that all the dynamically allocated memory is freed.
    /// @throws None.
    /// @version 2016-04-22/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-04-17/GGB
    ///   @li Changed C-style cast to reinterpret_cast<>()
    ///   @li Bug# 1571309 - Discovered that all data is not being deleted. Memory leak corrected.
    /// @version 2013-07-01/GGB - Function created.

    void CImageComparisonWindow::clearImageList()
    {
      int itemCount = listWidgetImages->count();
      SControlBlock *controlBlock;

      for (int index = 0; (index < itemCount) ; index++)
      {
        controlBlock =
            reinterpret_cast<SControlBlock *>(listWidgetImages->item(index)->data(ROLE_CONTROLBLOCK).toULongLong());

        if (controlBlock)
        {
          delete controlBlock;
          controlBlock = nullptr;
        };
      };

      imageList.clear();			// Clear the images list.
      listWidgetImages->clear();		// Clear the items from the table widget.
    }

    /// @brief Ensures that the window name is removed from the list of windows in the frame.
    /// @throws None
    /// @version 2011-03-04/GGB - Function created.

    void CImageComparisonWindow::closeEvent(QCloseEvent *event)
    {
      settings::astroManagerSettings->setValue(settings::IMAGE_COMPARISON_BLINKINTERVAL, QVariant(spinBoxInterval->value()));
      //dynamic_cast<dockwidgets::CImageControlWidget *>(dockWidgets[IDDW_IMAGECONTROL])->deactivateImage(&controlImage); (***CHECK WHY***)
      //dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.

      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

    /// @brief Draws the alignment point 1
    /// @param[in] point - The location of the alignment point.
    /// @throws None.
    /// @version 2011-03-05/GGB - Function created.

    void CImageComparisonWindow::displayAlign1(MCL::TPoint2D<ACL::FP_t> const &point)
    {
      QPen pen;

      if (alignPoint1.ellipse)
      {
        graphicsSceneImageInput->removeItem(alignPoint1.ellipse);
        graphicsSceneImageInput->removeItem(alignPoint1.text);
      };

      pen.setColor(Qt::red);
      alignPoint1.ellipse = graphicsSceneImageInput->addEllipse(point.x()-10, point.y()-10, 2*10, 2*10, pen);
      alignPoint1.text = graphicsSceneImageInput->addSimpleText(QString("1"));
      alignPoint1.text->setPos(point.x(), point.y() + 10 + 1);
      alignPoint1.text->setPen(pen);
    }

    /// @brief Draws the alignment point 2
    /// @param[in] point - The location of the alignment point.
    /// @throws None.
    /// @version 2011-03-05/GGB - Function created.

    void CImageComparisonWindow::displayAlign2(MCL::TPoint2D<ACL::FP_t> const &point)
    {
      QPen pen;

      if (alignPoint2.ellipse)
      {
        graphicsSceneImageInput->removeItem(alignPoint2.ellipse);
        graphicsSceneImageInput->removeItem(alignPoint2.text);
      };

      pen.setColor(Qt::green);
      alignPoint2.ellipse = graphicsSceneImageInput->addEllipse(point.x()-10, point.y()-10, 2*10, 2*10, pen);
      alignPoint2.text = graphicsSceneImageInput->addSimpleText(QString("2"));
      alignPoint2.text->setPos(point.x(), point.y() + 10 + 1);
      alignPoint2.text->setPen(pen);
    }

    /// @brief Swap the images over.
    /// @throws None.
    /// @version 2016-04-22/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-04-21/GGB - Changed C-style casts to reinterpret_cast<>()
    /// @version 2013-08-11/GGB - Added code to update the dockwidgets when the image is blinked.(Bug #1210738)
    /// @version 2013-07-08/GGB - Changed the data Role to ROLE_CONTROLIMAGEOP. (Bug #1198958)
    /// @version 2013-06-10/GGB - Function created.

    void CImageComparisonWindow::eventBlink()
    {
      SControlBlock *controlBlock;
      bool finished = false;
      bool rollOver = false;

      while (!finished)
      {
        ++imageNumber;

        if ( imageNumber >= listWidgetImages->count() )
        {
          imageNumber = 0;
          if (!rollOver)
          {
            rollOver = true;
          }
          else
          {
            finished = true;        // Exit if the imageNumber has rolled over twice.
          }
        };

        controlBlock = reinterpret_cast<SControlBlock *>(listWidgetImages->item(imageNumber)->data(ROLE_CONTROLBLOCK).toULongLong());

        if (controlBlock->outputImageValid)
        {
          finished = true;

            // display the new pixmap

          redrawImage();      // Is this needed????

          labelCurrentFile->setText(QString::fromStdString(controlBlock->outputImage.astroFile->getImageName()));

          imageChange(&(controlBlock->outputImage));
        }
      };
    }

    /// @brief Allows the user to select a single image or selection of images
    /// @details Opens the file selection dialog and allows the selection of multiple images. Image names are added to the list of images.
    /// The image list is not cleared.
    /// @throws None.
    /// @version 2016-04-21/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-04-17/GGB - Added code to enable the buttons as required.
    /// @version 2011-06-12/GGB - Function created.

    void CImageComparisonWindow::eventButtonAddImages(bool)
    {
      QStringList::const_iterator newImageIterator;
      QStringList newImages;
      bool firstPass = true;
      QListWidgetItem *lwi = nullptr;
      QIcon iconRed(":/images/BMP_IMAGESTACK_RED.bmp");
      std::string myPath;
      boost::filesystem::path filePath;
      SControlBlock *newControlBlock = nullptr;

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Open Image"),
                    settings::astroManagerSettings->value(settings::IMAGE_COMPARISON_DIRECTORY, QVariant(0)).toString(),
                    EXTENSION_IMAGE);

        // Iterate the list of items and only add items that are not repeats.

      for (newImageIterator = newImages.constBegin(); newImageIterator != newImages.constEnd(); ++newImageIterator)
      {
        if ( !(imageList.contains((*newImageIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list is saved if necessary to allow
            // reopen to the same directory.

          myPath = (*newImageIterator).toStdString();
          filePath = boost::filesystem::path(myPath);

          if ( firstPass && (settings::astroManagerSettings->value(settings::IMAGE_COMPARISON_UPDATEONOPEN, QVariant(true)).toBool()) )
          {
              // Get the directory details from the file name.

            settings::astroManagerSettings->setValue(settings::IMAGE_COMPARISON_DIRECTORY,
                                             QVariant(QString::fromStdString(filePath.parent_path().string())));
            firstPass = false;
          };

          imageList << (*newImageIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(iconRed, QString::fromStdString(filePath.filename().string()));

          newControlBlock = new SControlBlock(this);        // Create the control block.
          newControlBlock->inputFilename = *newImageIterator;

          lwi->setData(ROLE_CONTROLBLOCK, QVariant(qulonglong(newControlBlock)));   // Add the control block to the data.
          lwi->setForeground(Qt::gray);
          listWidgetImages->addItem(lwi);
          lwi = nullptr;
        };

        if (listWidgetImages->count() > 0)
        {
          pushButtonRemoveAll->setEnabled(true);
        };
      };
    }

    /// @brief Function to handle the alignment button being unchecked by the user.
    /// @param[in] isChecked - true if the button is checked (pressed)
    /// @throws None.
    /// @version 2011-03-04/GGB - Function created.

    void CImageComparisonWindow::eventButtonAlignment1(bool isChecked)
    {
        // Ensure that only one checkable button is checked.

      if (pushButtonAutomatic->isChecked())
      {
        pushButtonAutomatic->setChecked(false);
        blinkTimer->stop();
      };

      if (tabWidget->currentIndex() != 0)
      {
        tabWidget->setCurrentIndex(0);      // Switch to the input image tab.
      };

      if ( isChecked )
      {
          // Button is selected. Allow user to select the first alignment point.
          // Cursor is changed.

        graphicsViewImageInput->setCursor(astrometry);

          // Check if the other button is checked as well

        if ( pushButtonAlign2->isChecked() )
        {
          pushButtonAlign2->setChecked(false);
        };
      }
      else
      {
          // Button is not selected. Change the cursor back to the normal cursor.

        graphicsViewImageInput->setCursor(Qt::CrossCursor);
      };
    }

    /// @brief Function to handle the alignment button being unchecked by the user.
    /// @param[in] isChecked - Indicate if the button should be checked.
    /// @throws None.
    /// @version 2011-03-05/GGB - Function created.

    void CImageComparisonWindow::eventButtonAlignment2(bool isChecked)
    {
        // Ensure that only one checkable button is checked.

      if (pushButtonAutomatic->isChecked())
      {
        pushButtonAutomatic->setChecked(false);
        blinkTimer->stop();
      };

      if (tabWidget->currentIndex() != 0)
      {
        tabWidget->setCurrentIndex(0);    // Switch to the input image tab.
      };

      if (isChecked)
      {
          // Button is selected. Allow the user to select the second alignment point.
          // Change the cursor

        graphicsViewImageInput->setCursor(astrometry);

          // Check if the other button is pressed, if so uncheck the other button.

        if ( pushButtonAlign1->isChecked() )
        {
          pushButtonAlign1->setChecked(false);
        };
      }
      else
      {
          // Button is unselected. Change back to the normal cursor.

        graphicsViewImageInput->setCursor(Qt::CrossCursor);
      };
    }

    /// @brief Responds to the automatic button being pressed.
    /// @param[in] checked - Indicates if the button is checked.
    /// @throws None.
    /// @version 2013-06-11/GGB - Function created.

    void CImageComparisonWindow::eventButtonAutomatic(bool checked)
    {
        // Ensure that only one checkable button is checked.

      if (pushButtonAlign1->isChecked())
      {
        pushButtonAlign1->setChecked(false);
      };

      if (pushButtonAlign2->isChecked())
      {
        pushButtonAlign2->setChecked(false);
      };

      if (checked)
      {
        tabWidget->setCurrentIndex(1);        // Change to output tab.
        blinkTimer->start(spinBoxInterval->value() * 1000);   // Convert to msec.
      }
      else
      {
        blinkTimer->stop();
      };
    }

    /// @brief Removes all the images from the comparison window.
    /// @throws None.
    /// @version 2016-04-17/GGB - Bug# 1571309 fix.
    /// @version 2013-08-04/GGB - Added code to disable the dockwidgets.
    /// @version 2013-07-01/GGB - Function created.

    void CImageComparisonWindow::eventButtonImagesRemoveAll(bool)
    {
        // Give the user a warning!

      QxtConfirmationMessage msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("All files will be removed. Are you sure you want to continue?"));
      msgBox.setConfirmationText(tr("Do not show again."));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setOverrideSettingsKey(settings::CM_IMAGECOMPARISON_REMOVEALL);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        eventButtonAutomatic(false);              // Bug# 1571309 fix. Ensure that the image is not blinking.
        imageChange(nullptr);                    // Bug# 1571309 fix. When all images are deleted, there is no image to display.
        graphicsSceneImageInput->clear();         // Clear the graphics scene.
        graphicsSceneImageOutput->clear();
        clearImageList();                         // Clear the list of images.

        pushButtonRemoveAll->setEnabled(false);
        pushButtonRemove->setEnabled(false);

        graphicsViewImageInput->Paint();

        toggleImageWidgets(false);
        pushButtonManual->setEnabled(false);
        pushButtonAutomatic->setEnabled(false);
        spinBoxInterval->setEnabled(false);

          // Now disable the dockwidgets

        dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(false);
      };
    }

    /// @brief Moves onto the next image when the button is clicked.
    /// @details Calls the eventBlink routine to perform the actual blinking.
    /// @throws None.
    /// @version 2013-06-11/GGB - Function created.

    void CImageComparisonWindow::eventButtonManual(bool)
    {
      if (pushButtonAutomatic->isChecked())
      {
        pushButtonAutomatic->setChecked(false);
        blinkTimer->stop();
      };

      if (tabWidget->currentIndex() != 1)
      {
        tabWidget->setCurrentIndex(1);  // Set the output tab.
      };

      eventBlink();
    }

    /// @brief This function prepares the images for blinking or subtraction.
    /// @details The preparation is to align all the images using the alignment points generated.
    /// Steps: 1)  Ensure all the images have had two alignment points selected. If there are not two alignment points selected per
    ///            image display an error to the user, but allow the user to continue.
    ///        2)  Copy all the items and their alignment points to an libAstroImages::CImageAlign object.
    /// @throws None.
    /// @version 2016-04-22/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-03-28/GGB - Changed C-style casting to reinterpret_cast<>()
    /// @version 2013-07-13/GGB - Check for less than one image and exit.
    /// @version 2013-07-08/GGB - Changed the data Role to ROLE_CONTROLIMAGEOP. (Bug #1198958)
    /// @version 2013-06-10/GGB - Function created.

    void CImageComparisonWindow::eventButtonPrepareImages(bool)
    {
      bool bAllAlign = true, bAbort = false;
      int nIndex = 0;
      QMessageBox msgBox;
      ACL::CRegisterImages registerImages;
      SControlBlock *controlBlock = nullptr;

      if (listWidgetImages->count() <= 1)
      {
        bAbort = true;
      }
      else
      {

          // Check that all the images have valid alignment points.

        for (nIndex = 0; (nIndex < listWidgetImages->count()) && bAllAlign; nIndex++)
        {
          controlBlock = reinterpret_cast<SControlBlock *>(listWidgetImages->item(nIndex)->data(ROLE_CONTROLBLOCK).toULongLong());

          if (!controlBlock->alignmentPoint1 || !controlBlock->alignmentPoint2)
          {

              // All images do not have alignment points. Ask the user if they wish to continue without aligning all the images.

            msgBox.setText("Information: All Images do not have alignment points selected. Do you wish to continue without aligning" \
                            "all images?");
            msgBox.setInformativeText("Please assign two alignment points for each image.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);

            if (msgBox.exec() == QMessageBox::Cancel)
            {
              bAbort = true;
            };
          };
        };
      };

      if (!bAbort)
      {
          // No problems, or user wishes to continue and only align the specified images.
          // Add only the images into the register images object that have two alignment points.

        for (nIndex = 0; nIndex < listWidgetImages->count(); nIndex++)
        {
          controlBlock = reinterpret_cast<SControlBlock *>(listWidgetImages->item(nIndex)->data(ROLE_CONTROLBLOCK).toULongLong());

            // Add the images with it's alignment points into the registration class.

          if ( controlBlock->alignmentPoint1 && controlBlock->alignmentPoint2 )
          {
            registerImages.addImage(controlBlock->inputImage.astroFile,
                                    0,
                                    *controlBlock->alignmentPoint1,
                                    *controlBlock->alignmentPoint2);
          };
        };

          // Now register the images.

        registerImages.registerImages();    // Register the images.

        bAllowSave = true;

          // The images are now stored in the register images class. Need to get the images with the transformation data
          // into this class.

        ACL::DRegisterImageStore const &images = registerImages.getImages();
        ACL::DRegisterImageStore::const_iterator iterator;


          /* Copy the registered images into the outputImage structures. Remembering that only the images with two alignment points
             have been aligned. */

        nIndex = 0;
        for (iterator = images.begin(); iterator != images.end(); ++iterator)
        {
          controlBlock = reinterpret_cast<SControlBlock *>(listWidgetImages->item(nIndex)->data(ROLE_CONTROLBLOCK).toULongLong());

          if (controlBlock->alignmentPoint1 && controlBlock->alignmentPoint2)
          {

              // Copy the images to the output image.

          //controlBlock->outputImage.astroFile = (*iterator)->outputFile;

//          std::string newFilePath(controlBlock->inputImage.astroFile->getFilePath() + "/" +
//                                  controlBlock->inputImage.astroFile->getFileName());

//          newFilePath = boost::filesystem::path(newFilePath).root_directory().string() +
//              boost::filesystem::path(newFilePath).stem().string() +
//              std::string("_aligned") +
//              boost::filesystem::path(controlBlock->inputImage.astroFile->getFileName()).extension().string();

//          controlBlock->outputImage.astroFile->setFilePath(newFilePath);
//          controlBlock->outputFilename = QString::fromStdString(newFilePath);
          controlBlock->outputImageValid = true;

          //imageChanged(&controlBlock->outputImage);   // Update the image information.

          controlBlock->registerImageInformation = (*(*iterator));
          ++nIndex;
          };
        };

          // Now draw the output image.

        imageNumber = 0;

        controlBlock = reinterpret_cast<SControlBlock *>(listWidgetImages->item(imageNumber)->data(ROLE_CONTROLBLOCK).toULongLong());

        tabWidget->setTabEnabled(1, true);
        tabWidget->setCurrentIndex(1);  // Switch to the output tab.
        redrawImage();

        graphicsViewImageOutput->zoomAll();
        labelCurrentFile->setText(QString::fromStdString(controlBlock->outputImage.astroFile->getImageName()));

        pushButtonManual->setEnabled(true);
        pushButtonAutomatic->setEnabled(true);
        spinBoxInterval->setEnabled(true);
      };
    }

    /// @brief Removes images from the image list.
    /// @throws None.
    /// @version 2017-07-03/GGB - Updated to reflect new dockwidgets storage method.
    /// @version 2016-04-22/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-04-17/GGB - Bug#1571327 corrected.
    /// @version 2011-02-21/GGB - Function Created.

    void CImageComparisonWindow::eventButtonRemoveImages(bool)
    {
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;
      QList<QGraphicsItem *> items;
      QList<QGraphicsItem *>::iterator giIter;
      SControlBlock *controlBlock;

              // Give the user a warning!

      QxtConfirmationMessage msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("All the selected files will be removed. Are you sure you want to continue?"));
      msgBox.setConfirmationText(tr("Do not show again."));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setOverrideSettingsKey(settings::CM_IMAGECOMPARISON_REMOVE);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
          // Remove the existing items from the scene (without destroying them)

        imageChange(nullptr);                    // Bug# 1571327 fix. When all images are deleted, there is no image to display.

        items = graphicsSceneImageInput->items();

        for (giIter = items.begin(); giIter != items.end(); giIter++)
        {
          graphicsSceneImageInput->removeItem((*giIter));
        };

        dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_IMAGECONTROL).setEnabled(false);

        QList<QListWidgetItem *> toRemove = listWidgetImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;

          controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());
          imageList.removeOne(controlBlock->inputFilename);

          delete controlBlock;
          controlBlock = nullptr;

          delete lwi;
          lwi = nullptr;
        };

        listWidgetImages->repaint();

        pushButtonRemove->setEnabled(false);

        graphicsViewImageInput->Paint();

        toggleImageWidgets(false);
      };
    }

    /// @brief Changes the interval of the timer.
    /// @param[in] newInterval - The new blink interval in ms
    /// @throws None.
    /// @version 2013-06-11/GGB - Function created.

    void CImageComparisonWindow::eventIntervalChange(double newInterval)
    {
      blinkTimer->setInterval(newInterval * 1000);
    }

    /// @brief Handles the mouse press event from the graphics view window.
    /// @param[in] mouseEvent - The data associated with the mouse button click.
    /// @throws None.
    /// @detailsIf one of the alignment buttons is pressed (down), then the position is recorded and marked.
    /// If the Alignment1 button is down, then the Alignment1 button is raised and the Alignement2 button pressed
    /// If the Alignment2 button is down, then the Alignment2 button is raised.
    /// If the alignment position already exists, then it is changed.
    /// @version 2016-04-22/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2011-03-04/GGB - Function created.

    void CImageComparisonWindow::eventMousePress(QMouseEvent *mouseEvent)
    {
      QPointF point;
      std::optional<MCL::TPoint2D<ACL::FP_t>> centroid;
      SControlBlock *controlBlock;
      QIcon iconGreen(":/images/BMP_IMAGESTACK_GREEN.bmp");

      QListWidgetItem *selectedItem = listWidgetImages->currentItem();	// Get the current item.
      controlBlock = reinterpret_cast<SControlBlock *>(selectedItem->data(ROLE_CONTROLBLOCK).toULongLong());

      if ( pushButtonAlign1->isChecked() && (mouseEvent->button() == Qt::LeftButton) )
      {
          // Alignment 1 button is down.

        point = graphicsViewImageInput->mapToScene(mouseEvent->pos());									// Get the image coordinates

          // Find the centroid within a reasonable area.

        centroid = controlBlock->inputImage.astroFile->centroid(controlBlock->inputImage.currentHDB,
                                                                MCL::TPoint2D<ACL::AXIS_t>(point.x(), point.y()),
                                                                settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS,
                                                                                   QVariant(20)).toLongLong(),
                                                                settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                   QVariant(3)).toInt());

        if (!centroid)
        {
          QMessageBox::information(this, tr("Unable to find centroid."),
                                   tr("A centroid was not found at the indicated spot."),
                                   QMessageBox::Ok, QMessageBox::Ok);
        }
        else
        {
          controlBlock->alignmentPoint1 = centroid;

          labelAlign1->setText(QString("(%1, %2)").arg(centroid->x(), 0, 'f', 1).arg(centroid->y(), 0, 'f', 1));

          // Draw the alignment points onto the screen

          displayAlign1(*centroid);

          // Update the buttons

          pushButtonAlign1->setChecked(false);

          if (controlBlock->alignmentPoint2)
          {
            selectedItem->setIcon(iconGreen);
          }
          else
          {
            pushButtonAlign2->setChecked(true);
          };
        };
      }
      else if ( pushButtonAlign2->isChecked() && (mouseEvent->button() == Qt::LeftButton) )
      {

          // Alignment 2 button is down.

        point = graphicsViewImageInput->mapToScene(mouseEvent->pos());		// Get the image coordinates

          // Find the centroid within a reasonable area.

        centroid = controlBlock->inputImage.astroFile->centroid(controlBlock->inputImage.currentHDB,
                                                                MCL::TPoint2D<ACL::AXIS_t>(point.x(), point.y()),
                                                                settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS,
                                                                                   QVariant(20)).toLongLong(),
                                                                settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                   QVariant(3)).toInt());

        if (!centroid)
        {
          QMessageBox::information(this, tr("Unable to find centroid."),
                                   tr("A centroid was not found at the indicated spot."),
                                   QMessageBox::Ok, QMessageBox::Ok);
        }
        else
        {

          controlBlock->alignmentPoint2 = centroid;

          labelAlign2->setText(QString("(%1, %2)").arg(centroid->x(), 0, 'f', 1).arg(centroid->y(), 0, 'f', 1));

          // Draw the alignment points onto the screen

          displayAlign2(*centroid);

          // Update the buttons

          pushButtonAlign2->setChecked(false);

          // Change back to the standard cursor

          graphicsViewImageInput->setCursor(Qt::CrossCursor);

          if (controlBlock->alignmentPoint1)
          {
            selectedItem->setIcon(iconGreen);
          };
        };
      }
      else if ( mode == CAstroImageWindow::M_ASTROMETRY )
      {
        mousePressAstrometry(mouseEvent);
      }
      else if ( mode == CAstroImageWindow::M_PHOTOMETRY )
      {
        mousePressPhotometry(mouseEvent);
      };
    }

    /// @brief Handles selection of the blink images radio button.
    //
    // 2011-06-12/GGB - Function created.

    void CImageComparisonWindow::eventRadioButtonBlinkImages(bool)
    {
      spinBoxInterval->setEnabled(true);
    }

    /// @brief Handles selection of the subtract images radio button.
    //
    // 2011-06-12/GGB - Function created.

    void CImageComparisonWindow::eventRadioButtonSubtractImages(bool)
    {
      spinBoxInterval->setEnabled(false);
    }

    /// @brief Procedure called when the tab index is changed.
    /// @param[in] index - The index of the new tab.
    /// @returns None.
    /// @details The navigator and magnifier windows need to be updated.
    /// @throws CRuntimeAssert(astroManager)
    /// @version 2016-04-22/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2013-08-10/GGB - Function created.

    void CImageComparisonWindow::eventTabCurrentChanged(int index)
    {
      QListWidgetItem *lwi;
      SControlBlock *controlBlock;

      RUNTIME_ASSERT(astroManager, index <= 1, "Invalid Tab Index.");

      if (index == 0)
      {
          // Currently open on the Original images.
          // The currently selected items in the list widget is the current image that is being displayed.

        lwi = listWidgetImages->currentItem();
        controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());
        if (controlBlock->inputImageValid)
        {
          imageChange(&controlBlock->inputImage);
          dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(true);
        }
        else
        {
          imageChange(nullptr);
          dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(false);
        }
      }
      else if (index == 1)
      {
          // Currently open on the Aligned images.
          // Image number is the number of the image that is being currently selected.

        lwi = listWidgetImages->item(imageNumber);
        if (lwi)
        {
          controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());
          if (controlBlock->outputImageValid)
          {
            imageChange(&controlBlock->outputImage);
            dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(true);
          }
          else
          {
            imageChange(nullptr);
            dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(false);
          };
        }
        else
        {
          imageChange(nullptr);
          dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(false);
        }
      };
    }

    /// @brief Returns the current control image, or nullptr of there is no image selected.
    /// @returns Pointer to the current controlImage structure. nullptr if there is no current image.
    /// @throws None
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2011-06-12/GGB - Function created.

    imaging::SControlImage *CImageComparisonWindow::getControlImage()
    {
      if (listWidgetImages)
      {
        if (listWidgetImages->currentItem())
        {
          return (&reinterpret_cast<SControlBlock *>(listWidgetImages->currentItem()->data(ROLE_CONTROLBLOCK).toULongLong())->inputImage);
        }
        else
        {
          return nullptr;
        };
      }
      else
      {
        return nullptr;
      };
    }

    /// @brief Function called when an astrometry mouse press is used
    /// @param[in] mouseEvent - The mouse event information.
    /// @throws CRuntimeAssert(astroManager)
    /// @throws CCodeError(astroManager)
    /// @version 2017-07-03/GGB - Updated to reflect new dockwidgets storage method.
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2013-08-03/GGB - Function created.

    void CImageComparisonWindow::mousePressAstrometry(QMouseEvent *mouseEvent)
    {
      QPointF scenePoint;
      dockwidgets::CAstrometryDockWidget &dwa =
          dynamic_cast<dockwidgets::CAstrometryDockWidget &>(
            dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_ASTROMETRYCONTROL));
      QListWidgetItem *lwi;
      SControlBlock *controlBlock;
      std::optional<MCL::TPoint2D<ACL::FP_t>> centroid;

      switch (mouseEvent->button())
      {
      case Qt::LeftButton:
      {
        if (tabWidget->currentIndex() == 0)       // Selecting on the Input image.
        {
          scenePoint = graphicsViewImageInput->mapToScene(mouseEvent->pos());		// Get the image coordinates
          lwi = listWidgetImages->currentItem();

          RUNTIME_ASSERT(astroManager, lwi != nullptr, "List widget should have a relevant item.");

            // Astrometry on the input image. Need to also mark up the output image (If there is one)
            // The original image is the currentImage in the dockwidgets.

          controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());

          QString objectName = QString("A:%1").arg(controlBlock->inputImage.astroFile->astrometryObjectCount() + 1);

          centroid = controlBlock->inputImage.astroFile->centroid(controlBlock->inputImage.currentHDB,
                                                                  MCL::TPoint2D<ACL::AXIS_t>(scenePoint.x(), scenePoint.y()),
                                                                  settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS,
                                                                                                QVariant(20)).toLongLong(),
                                                                  settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                                QVariant(3)).toInt());
          if (centroid)
          {
              // Create the astrometry reference in the original image.

            astrometry::CAstrometryObservation *astrometryObjectOriginal = astrometryAdd(&controlBlock->inputImage,
                                                                                         *centroid, objectName);
            graphicsSceneImageInput->addItem(astrometryObjectOriginal->group);    // Ownership passes to the scene.

              // Create the astrometry reference on the aligned image (if there is one)

            if (controlBlock->outputImageValid)
            {
              MCL::TPoint2D<ACL::FP_t> alignedCentroid = ACL::imageTransformForward(*centroid,
                                                                                    controlBlock->registerImageInformation.align1,
                                                                                    controlBlock->registerImageInformation.tr,
                                                                                    controlBlock->registerImageInformation.dth,
                                                                                    controlBlock->registerImageInformation.sc,
                                                                                    controlBlock->registerImageInformation.pixSize);

              astrometry::CAstrometryObservation *astrometryObjectAligned = astrometryAdd(&controlBlock->outputImage,
                                                                                         alignedCentroid,
                                                                                         objectName);
              graphicsSceneImageOutput->addItem(astrometryObjectAligned->group);    // Ownership passes to the scene.
            };
          }
          else
          {
            QMessageBox::information(this, tr("Unable to find centroid."),
                                     tr("A centroid was not found at the indicated spot."),
                                     QMessageBox::Ok, QMessageBox::Ok);
          };
        }
        else if (tabWidget->currentIndex() == 1)      // Selecting on the output (aligned image)
        {

          scenePoint = graphicsViewImageOutput->mapToScene(mouseEvent->pos());		// Get the image coordinates
          lwi = listWidgetImages->item(imageNumber);

          RUNTIME_ASSERT(astroManager, lwi, "There should be a corresponding list widget item.");

            // Astrometry on the output image. Need to also markup the input image.

          controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());

          QString objectName = QString("A:%1").arg(controlBlock->outputImage.astroFile->astrometryObjectCount() + 1);

          centroid = controlBlock->outputImage.astroFile->centroid(controlBlock->outputImage.currentHDB,
                                                                   MCL::TPoint2D<ACL::AXIS_t>(scenePoint.x(), scenePoint.y()),
                                                                   settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS,
                                                                                                 QVariant(20)).toLongLong(),
                                                                   settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                                 QVariant(3)).toInt());
          if (centroid)
          {
              // Create the astrometry reference in the aligned image.

            astrometry::CAstrometryObservation *astrometryObjectAligned = astrometryAdd(&controlBlock->outputImage,
                                                                                        *centroid, objectName);
            graphicsSceneImageOutput->addItem(astrometryObjectAligned->group);    // Ownership passes to the scene.

              // Create the astrometry reference on the original image.

            MCL::TPoint2D<ACL::FP_t> originalCentroid = ACL::imageTransformReverse(*centroid,
                                                                                   controlBlock->registerImageInformation.align1,
                                                                                   controlBlock->registerImageInformation.tr,
                                                                                   controlBlock->registerImageInformation.dth,
                                                                                   controlBlock->registerImageInformation.sc,
                                                                                   controlBlock->registerImageInformation.pixSize);

            astrometry::CAstrometryObservation *astrometryObjectOriginal = astrometryAdd(&controlBlock->inputImage, originalCentroid,
                                                                                         objectName);
            graphicsSceneImageInput->addItem(astrometryObjectOriginal->group);    // Ownership passes to the scene.
          }
          else
          {
            QMessageBox::information(this, tr("Unable to find centroid."),
                                     tr("A centroid was not found at the indicated spot."),
                                     QMessageBox::Ok, QMessageBox::Ok);
          };

        }
        else
        {
          CODE_ERROR(astroManager);
        }
        break;
      };
      case Qt::RightButton:
      case Qt::MidButton:
      case Qt::NoButton:
      default:
        break;
      };

    }

    /// @brief Adds a photometry label at the selected spot.
    /// @param[in] mouseEvent: The mouse event information.
    /// @returns None.
    /// @throws CRuntimeAssert(astroManager)
    /// @throws CCodeError(astroManager)
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2013-08-05/GGB - Function created.

    void CImageComparisonWindow::mousePressPhotometry(QMouseEvent *mouseEvent)
    {
      QPointF scenePoint;
      dockwidgets::CPhotometryDockWidget &dwp = dynamic_cast<dockwidgets::CPhotometryDockWidget &>(
            dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));
      QListWidgetItem *lwi;
      SControlBlock *controlBlock;
      std::optional<MCL::TPoint2D<ACL::FP_t>> centroid;

      switch (mouseEvent->button())
      {
      case Qt::LeftButton:
      {
        if (tabWidget->currentIndex() == 0)     // Working on the input image.
        {
          scenePoint = graphicsViewImageInput->mapToScene(mouseEvent->pos());		// Get the image coordinates
          lwi = listWidgetImages->currentItem();

          RUNTIME_ASSERT(astroManager, lwi != nullptr, "There should be a valid list widget item.");

            // Photometry on the input image. Need to also mark up the output image (If there is one)
            // The original image is the currentImage in the dockwidgets.

          controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());

          QString objectName = QString("P:%1").arg(controlBlock->inputImage.astroFile->photometryObjectCount() + 1);

          centroid = controlBlock->inputImage.astroFile->centroid(controlBlock->inputImage.currentHDB,
                                                                  MCL::TPoint2D<ACL::AXIS_t>(scenePoint.x(), scenePoint.y()),
                                                                  settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_RADIUS,
                                                                                             QVariant(20)).toLongLong(),
                                                                  settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                             QVariant(3)).toInt());
          if (centroid)
          {
              // Create the photometry reference in the original image.

            photometry::CPhotometryObservation *photometryObjectOriginal = photometryAdd(&controlBlock->inputImage,
                                                                                         *centroid, objectName);
            dwp.addNewObject(photometryObjectOriginal);
            dwp.displayPhotometry(photometryObjectOriginal);

            graphicsSceneImageInput->addItem(photometryObjectOriginal->group);    // Ownership passes to the scene.

              // Create the photometry reference on the aligned image (if there is one)

            if (controlBlock->outputImageValid)
            {
              MCL::TPoint2D<ACL::FP_t> alignedCentroid = ACL::imageTransformForward(*centroid,
                                                                                    controlBlock->registerImageInformation.align1,
                                                                                    controlBlock->registerImageInformation.tr,
                                                                                    controlBlock->registerImageInformation.dth,
                                                                                    controlBlock->registerImageInformation.sc,
                                                                                    controlBlock->registerImageInformation.pixSize);


              photometry::CPhotometryObservation *photometryObjectAligned = photometryAdd(&controlBlock->outputImage,
                                                                                          alignedCentroid, objectName);

              graphicsSceneImageOutput->addItem(photometryObjectAligned->group);    // Ownership passes to the scene.
            }
          }
          else
          {
            QMessageBox::information(this, tr("Unable to find centroid."),
                                     tr("A centroid was not found at the indicated spot."),
                                     QMessageBox::Ok, QMessageBox::Ok);
          };
        }
        else if (tabWidget->currentIndex() == 1)      // Working on the output (aligned) image.
        {

          scenePoint = graphicsViewImageOutput->mapToScene(mouseEvent->pos());		// Get the image coordinates
          lwi = listWidgetImages->item(imageNumber);

          RUNTIME_ASSERT(astroManager, lwi != nullptr, "There should be a valid list widget item.");

            // Photometry on the output image. Need to also markup the input image.

          controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());

          QString objectName = QString("P:%1").arg(controlBlock->outputImage.astroFile->photometryObjectCount() + 1);

          centroid = controlBlock->outputImage.astroFile->centroid(controlBlock->outputImage.currentHDB,
                                                                   MCL::TPoint2D<ACL::AXIS_t>(scenePoint.x(), scenePoint.y()),
                                                                   settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_RADIUS,
                                                                                            QVariant(20)).toLongLong(),
                                                                   settings::astroManagerSettings->value(settings::PHOTOMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                                            QVariant(3)).toInt());
          if (centroid)
          {
              // Create the photometry reference in the aligned image.

            photometry::CPhotometryObservation *photometryObjectAligned = photometryAdd(&controlBlock->outputImage,
                                                                                        *centroid, objectName);

            dwp.addNewObject(photometryObjectAligned);
            dwp.displayPhotometry(photometryObjectAligned);

            graphicsSceneImageOutput->addItem(photometryObjectAligned->group);    // Ownership passes to the scene.

              // Create the astrometry reference on the original image.

            MCL::TPoint2D<ACL::FP_t> originalCentroid = ACL::imageTransformReverse(*centroid,
                                                                                   controlBlock->registerImageInformation.align1,
                                                                                   controlBlock->registerImageInformation.tr,
                                                                                   controlBlock->registerImageInformation.dth,
                                                                                   controlBlock->registerImageInformation.sc,
                                                                                   controlBlock->registerImageInformation.pixSize);

            photometry::CPhotometryObservation *photometryObjectOriginal = photometryAdd(&controlBlock->inputImage,
                                                                                         originalCentroid, objectName);
            graphicsSceneImageInput->addItem(photometryObjectOriginal->group);    // Ownership passes to the scene.
          }
          else
          {
            QMessageBox::information(this, tr("Unable to find centroid."),
                                     tr("A centroid was not found at the indicated spot."),
                                     QMessageBox::Ok, QMessageBox::Ok);
          };

        }
        else
        {
          CODE_ERROR(astroManager);
        }
        break;
      };
      case Qt::RightButton:
      case Qt::MidButton:
      case Qt::NoButton:
      default:
        break;
      };
    }

    /// @brief Adds an photometry reference into the relevant astroFile.
    /// @param[in] controlImage: The image to act on
    /// @param[in] centroid: The centroid of the object to add.
    /// @param[in] objectName: The name of the object
    /// @throws None
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-08-05/GGB - Function created.

    photometry::CPhotometryObservation *CImageComparisonWindow::photometryAdd(imaging::SControlImage *controlImage,
                                                                                              MCL::TPoint2D<ACL::FP_t> const &centroid,
                                                                                              QString const &objectName)
    {
      QPen pen;
      dockwidgets::CPhotometryDockWidget &pw =
          dynamic_cast<dockwidgets::CPhotometryDockWidget &>(
            dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->getDockWidget(mdiframe::IDDW_PHOTOMETRYCONTROL));

      ACL::PPhotometryAperture photometryAperture(new ACL::CPhotometryApertureCircular(pw.getRadius1(),
                                                                                       pw.getRadius2(),
                                                                                       pw.getRadius3()));

        // Add the object into the vector.

      controlImage->photometryObservations.emplace_back(std::make_shared<photometry::CPhotometryObservation>());


      controlImage->photometryObservations.back()->objectName(objectName.toStdString());
      controlImage->photometryObservations.back()->CCDCoordinates(centroid);
      controlImage->photometryObservations.back()->photometryAperture(photometryAperture);
      controlImage->photometryObservations.back()->exposure() = controlImage->astroFile->getHDB(controlImage->currentHDB)->EXPOSURE();
      controlImage->photometryObservations.back()->gain(static_cast<FP_t>((controlImage->astroFile->getHDB(controlImage->currentHDB)->keywordData(ACL::SBIG_EGAIN))));
      controlImage->photometryObservations.back()->FWHM(controlImage->astroFile->FWHM(controlImage->currentHDB, centroid));
      controlImage->astroFile->pointPhotometry(controlImage->currentHDB, *controlImage->photometryObservations.back());    // Now perform the photometry on the object

      // Get the image coordinates and convert to WCS coordinates.

      std::optional<ACL::CAstronomicalCoordinates> WCSCoordinates =
        controlImage->astroFile->getHDB(controlImage->currentHDB)->pix2wcs(controlImage->photometryObservations.back()->CCDCoordinates());

      if (WCSCoordinates)
      {
        controlImage->photometryObservations.back()->observedCoordinates(*WCSCoordinates);
      }

      // Add the photometry observation to the reference list.

      if ( !controlImage->astroFile->hasPhotometryHDB() )
      {
        ACL::CHDBPhotometry *phdb = controlImage->astroFile->createPhotometryHDB();
        phdb->keywordWrite(ACL::HEASARC_CREATOR, CREATOR(), ACL::HEASARC_COMMENT_CREATOR);
        phdb->keywordWrite(ACL::FITS_DATE, getDate(), ACL::FITS_COMMENT_DATE);
      };

      controlImage->astroFile->photometryObjectAdd(controlImage->photometryObservations.back());

      pen.setColor(settings::astroManagerSettings->value(settings::PHOTOMETRY_INDICATOR_COLOUR, QVariant(QColor(Qt::red))).toInt());

      drawPhotometryIndicator(controlImage->photometryObservations.back().get(), pen);

      controlImage->astroFile->isDirty(true);
      controlImage->astroFile->hasData(true);

      return controlImage->photometryObservations.back().get();
    }

    /// @brief Redraws the image when required.
    /// @param None.
    /// @return None.
    /// @throws None.
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2013-08-04/GGB - Function created.

    void CImageComparisonWindow::redrawImage()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      SControlBlock *controlBlock;
      imaging::SControlImage *controlImage = nullptr;
      QGraphicsScene *gsImage = nullptr;
      QListWidgetItem *lwi;

      RUNTIME_ASSERT(astroManager, pw, "Parent Widget should not be equal to nullptr.");


      if (tabWidget->currentIndex() == 0)
      {
        lwi = listWidgetImages->currentItem();
        controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());

        controlImage = &controlBlock->inputImage;
        gsImage = graphicsSceneImageInput;
      }
      else if (tabWidget->currentIndex() == 1)
      {
        lwi = listWidgetImages->item(imageNumber);
        controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());

        controlImage = &controlBlock->outputImage;
        gsImage = graphicsSceneImageOutput;
      }
      else
      {
        CODE_ERROR(astroManager);
      };

      repaintImage();

        // Repaint the astrometry

      if (pw->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->isChecked())
      {
        QPen pen;

        pen.setColor(Qt::red);

        for(auto iterator = controlImage->astrometryObservations.begin(); iterator != controlImage->astrometryObservations.end(); iterator++)
        {
          drawCrossIndicator((*iterator).get(), pen);
          gsImage->addItem((*iterator)->group);
        };
      };

        // Repaint the photometry

      if (pw->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->isChecked())
      {
        QPen pen;

        pen.setColor(Qt::red);

        for(auto iterator = controlImage->photometryObservations.begin(); iterator != controlImage->photometryObservations.end(); iterator++)
        {
          drawPhotometryIndicator((*iterator).get(), pen);
          gsImage->addItem((*iterator)->group);
        };
      };
    }

    /// @brief Ensures that the windows is repainted when called.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @note The repaintImage() function must call the gvImage->Paint() function and update the scene before repainting. This
    /// function can be called internally, or extenally by the Histogram dock widget. (and others in the future??)
    /// @details A selection must be made to ensure that the correct graphics scene and graphics view are updated to reflect
    /// the current selections in the window.
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-03-28/GGB - Function created.

    void CImageComparisonWindow::repaintImage()
    {
      QGraphicsScene *gsImage = nullptr;
      CAstroGraphicsView *gvImage = nullptr;
      QListWidgetItem *lwi;
      SControlBlock *controlBlock;
      imaging::SControlImage *controlImage = nullptr;

      if (tabWidget->currentIndex() == 0)
      {
        lwi = listWidgetImages->currentItem();
        controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());
        controlImage = &controlBlock->inputImage;
        gsImage = graphicsSceneImageInput;
        gvImage = graphicsViewImageInput;
      }
      else if (tabWidget->currentIndex() == 1)
      {
        lwi = listWidgetImages->item(imageNumber);
        controlBlock = reinterpret_cast<SControlBlock *>(lwi->data(ROLE_CONTROLBLOCK).toULongLong());
        controlImage = &controlBlock->outputImage;
        gsImage = graphicsSceneImageOutput;
        gvImage = graphicsViewImageOutput;
      }
      else
      {
        CODE_ERROR(astroManager);
      };

      gsImage->clear();         // This invalidates the pixmapItem as the scene owns the pixmapItem.
      gsImage->addPixmap(*controlImage->pixmap);
      gvImage->Paint();         // This causes the repaint of the image.
    }

    /// @brief Function to save the images. Call up a dialog allowing the user to select the images to be saved.
    /// @throws None.
    /// @version 2013-06-12/GGB - Function created.

    bool CImageComparisonWindow::save()
    {
      dialogs::CDialogSaveAligned saveDialog(listWidgetImages);

      saveDialog.exec();

      return false;
    }

    /// @brief The save function and the save function are identical in this window.
    /// @note Just call the save function.
    /// @version 2013-06-12/GGB - Function created.

    bool CImageComparisonWindow::saveAs()
    {
      return save();
    }

    /// @brief Function used when an image is going to have a star reference inserted.
    /// @details Send message to graphics window to tell it that it in astrometry mode.
    /// @version 2013-08-03/GGB - Function created

    void CImageComparisonWindow::setMode(EMode newMode)
    {
      mode = newMode;

      if (mode == M_ASTROMETRY)
      {
        graphicsViewImageInput->setMode(CAstroGraphicsView::M_ASTROMETRY);
        graphicsViewImageOutput->setMode(CAstroGraphicsView::M_ASTROMETRY);
      }
      else if (mode == M_PHOTOMETRY)
      {
        graphicsViewImageInput->setMode(CAstroGraphicsView::M_PHOTOMETRY);
        graphicsViewImageOutput->setMode(CAstroGraphicsView::M_PHOTOMETRY);
      }
      else if (mode == M_NONE)
      {
        graphicsViewImageInput->setMode(CAstroGraphicsView::M_NONE);
        graphicsViewImageOutput->setMode(CAstroGraphicsView::M_NONE);
      };
    }

    /// @brief Sets up the user interface.
    /// @throws CRuntimeAssert
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2016-04-17/GGB - Added code to update the "Remove Images" and "Remove All Images" buttons.
    /// @version 2013-08-01/GGB - Added object tagging functionality.
    /// @version 2013-07-01/GGB - Added button remove all.
    /// @version 2013-06-02/GGB - Members created for all controls.
    /// @version 2011-06-12/GGB - Function created.

    void CImageComparisonWindow::setupUI()
    {
      QUiLoader loader;
      QGridLayout *glImage;
      QGroupBox *groupBoxImage;
      QWidget *inputTabWidget, *outputTabWidget;

        // Create the window details from the template

      QFile file(":/forms/windowCompareImages.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/windowCompareImages.ui.");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tabWidget = formWidget->findChild<QTabWidget *>("tabWidget");
      listWidgetImages = formWidget->findChild<QListWidget *>("listWidgetImages");
      radioButtonBlink = formWidget->findChild<QRadioButton *>("radioButtonBlink");
      radioButtonSubtract = formWidget->findChild<QRadioButton *>("radioButtonSubtract");
      radioButtonColour = formWidget->findChild<QRadioButton *>("radioButtonColour");
      groupBoxImage = formWidget->findChild<QGroupBox *>("groupBoxImage");
      spinBoxInterval = formWidget->findChild<QDoubleSpinBox *>("doubleSpinBoxInterval");
      pushButtonAdd = formWidget->findChild<QPushButton *>("pushButtonAdd");
      pushButtonRemove = formWidget->findChild<QPushButton *>("pushButtonRemove");
      pushButtonRemoveAll = formWidget->findChild<QPushButton *>("pushButtonRemoveAll");
      pushButtonManual = formWidget->findChild<QPushButton *>("pushButtonManual");
      pushButtonAutomatic = formWidget->findChild<QPushButton *>("pushButtonAutomatic");
      pushButtonAlign1 = formWidget->findChild<QPushButton *>("pushButtonAlign1");
      pushButtonAlign2 = formWidget->findChild<QPushButton *>("pushButtonAlign2");
      pushButtonPrepare = formWidget->findChild<QPushButton *>("pushButtonPrepare");
      labelAlign1 = formWidget->findChild<QLabel *>("labelAlign1");
      labelAlign2 = formWidget->findChild<QLabel *>("labelAlign2");

      inputTabWidget = tabWidget->widget(0);
      outputTabWidget = tabWidget->widget(1);
      labelCurrentFile = formWidget->findChild<QLabel *>("labelCurrentFile");

      if ( !tabWidget || !listWidgetImages || !radioButtonBlink || !radioButtonSubtract || !radioButtonColour || !groupBoxImage ||
           !spinBoxInterval || !pushButtonAdd || !pushButtonRemove || !pushButtonRemoveAll || !pushButtonManual ||
           !pushButtonAutomatic || !pushButtonAlign1 || !pushButtonAlign2 || !pushButtonPrepare || !labelAlign1 || !labelAlign2 ||
           !inputTabWidget || !outputTabWidget || !labelCurrentFile)
        CODE_ERROR(astroManager);

      glImage = (QGridLayout *) inputTabWidget->layout();

      graphicsViewImageInput = new imaging::CAstroGraphicsView(this);
      glImage->addWidget(graphicsViewImageInput, 0, 0, 1, 1);

      graphicsSceneImageInput = new QGraphicsScene();
      graphicsViewImageInput->setScene(graphicsSceneImageInput);

        // Create the output layouts, graphics view and scene.

      glImage = (QGridLayout *) outputTabWidget->layout();

      graphicsViewImageOutput = new imaging::CAstroGraphicsView(this);
      glImage->addWidget(graphicsViewImageOutput, 1, 0, 1, 1);

      graphicsSceneImageOutput = new QGraphicsScene();
      graphicsViewImageOutput->setScene(graphicsSceneImageOutput);

      switch (settings::astroManagerSettings->value(settings::IMAGE_COMPARISON_METHOD, QVariant(BLINK_IMAGES)).toInt())
      {
      case BLINK_IMAGES:
        radioButtonBlink->setChecked(true);
        break;
      case SUBTRACT_IMAGES:
        radioButtonSubtract->setChecked(true);
        spinBoxInterval->setEnabled(false);
        break;
      case COLOUR_IMAGES:
        radioButtonColour->setChecked(true);
        spinBoxInterval->setEnabled(false);
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };

      pushButtonManual->setEnabled(false);
      pushButtonAutomatic->setEnabled(false);
      spinBoxInterval->setEnabled(false);

      pushButtonRemove->setEnabled(false);
      pushButtonRemoveAll->setEnabled(false);

      spinBoxInterval->setValue(settings::astroManagerSettings->value(settings::IMAGE_COMPARISON_BLINKINTERVAL, QVariant(1)).toDouble());

      connect(pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(eventButtonAddImages(bool)));
      connect(pushButtonRemove, SIGNAL(clicked(bool)), this, SLOT(eventButtonRemoveImages(bool)));
      connect(pushButtonRemoveAll, SIGNAL(clicked(bool)), this, SLOT(eventButtonImagesRemoveAll(bool)));
      connect(pushButtonAlign1, SIGNAL(clicked(bool)), this, SLOT(eventButtonAlignment1(bool)));
      connect(pushButtonAlign2, SIGNAL(clicked(bool)), this, SLOT(eventButtonAlignment2(bool)));
      connect(radioButtonBlink, SIGNAL(clicked(bool)), this, SLOT(eventRadioButtonBlinkImages(bool)));
      connect(radioButtonSubtract, SIGNAL(clicked(bool)), this, SLOT(eventRadioButtonSubtractImages(bool)));
      connect(pushButtonPrepare, SIGNAL(clicked(bool)), this, SLOT(eventButtonPrepareImages(bool)));
      connect(pushButtonManual, SIGNAL(clicked(bool)), this, SLOT(eventButtonManual(bool)));
      connect(pushButtonAutomatic, SIGNAL(clicked(bool)), this, SLOT(eventButtonAutomatic(bool)));
      connect(listWidgetImages, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(signalItemClickedImageList(QListWidgetItem *)));
      connect(blinkTimer, SIGNAL(timeout()), this, SLOT(eventBlink()));
      connect(spinBoxInterval, SIGNAL(valueChanged(double)), this, SLOT(eventIntervalChange(double)));
      connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(eventTabCurrentChanged(int)));

      tabWidget->setCurrentIndex(0);    // Switch to the input tab.

      tabWidget->setTabEnabled(1, false);
    }

    /// @brief Processes an activated item signal in the image list.
    /// @param[in] selectedItem - The item in the list that has been selected by the user.
    /// @returns None.
    /// @throws None.
    /// @details The selected item contains the path of the item in the Qt::UserRole data. This allows quick selection of the
    /// filename and path of the item.
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-03-28/GGB - Changed C-style casts to reinterpret_cast<>()
    /// @version 2011-03-03/GGB - Function created.

    void CImageComparisonWindow::signalItemClickedImageList(QListWidgetItem *selectedItem)
    {
      SControlBlock *controlBlock;

      controlBlock = reinterpret_cast<SControlBlock *>(selectedItem->data(ROLE_CONTROLBLOCK).toULongLong());
      tabWidget->setCurrentIndex(0);  // Switch to the input tab.

        // Check if the selectedItem has already had a CAstroFile loaded. If so, we need to display the image.
        // If the astro image has not been loaded, we need to load it and change the color of the test in the widget.

      try
      {
        if ( !controlBlock->inputImageValid )
        {
            // The image has not been loaded, it needs to be loaded.
            // Create the astroFile from the filename.

//          controlBlock->inputImage.astroFile.reset(new ACL::CAstroFile(controlBlock->inputFilename.toStdString()));
          controlBlock->inputImageValid = true;

            // Change the colour of the item to signify that the image is already loaded.

          selectedItem->setForeground(Qt::black);
        };

          // The image is now loaded, it must be displayed.

        imageChange(&controlBlock->inputImage);
        redrawImage();
        graphicsViewImageInput->setCursor(Qt::CrossCursor);
        zoomAll();

          // Check if the alignment points exist and display them as required.

        alignPoint1.ellipse = nullptr;
        alignPoint2.ellipse = nullptr;

        if (controlBlock->alignmentPoint1)
        {
          labelAlign1->setText(QString("(%1, %2)").
                               arg(controlBlock->alignmentPoint1->x(), 0, 'f', 1).arg(controlBlock->alignmentPoint1->y(), 0, 'f', 1));
          displayAlign1(*controlBlock->alignmentPoint1);
        }
        else
        {
          labelAlign1->setText(QString(""));
        };

        if (controlBlock->alignmentPoint2)
        {
          labelAlign2->setText(QString("(%1, %2)").
                               arg(controlBlock->alignmentPoint2->x(), 0, 'f', 1).arg(controlBlock->alignmentPoint2->y(), 0, 'f', 1));
          displayAlign2(*controlBlock->alignmentPoint2);
        }
        else
        {
          labelAlign2->setText(QString(""));
        };

          // Now enable all the relevant widgets in the window.

        toggleImageWidgets(true);
        pushButtonRemove->setEnabled(true);

          // Now enable the dockwidgets.

        dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(true);

      }
      catch(...)
      {

      }

//      catch (CCfits::FitsException &e)
  //    {
    //    QString msgString = QString("<html><b>%1</b><br />Do you wish to remove the file from the stacking list?</html>").
      //      arg(QString::fromStdString(e.message()));

//        QMessageBox msgBox;
  //      msgBox.setWindowTitle(tr("Error while reading FITS file"));
    //    msgBox.setText(QString("<html>Error reading FITS file <b>%1</b></html>").arg(QString::fromStdString(fileName)));
      //  msgBox.setInformativeText(msgString);
//        msgBox.setIcon(QMessageBox::Information);
  //      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //    msgBox.setDefaultButton(QMessageBox::Yes);

//        if (msgBox.exec() == QMessageBox::Yes)
  //      {
            // Remove the file from the list. (fileName is the name of the file.)

    //      imageList.removeOne(QString::fromStdString(fileName));

      //    listWidgetImages->setCurrentRow(-1);
        //  listWidgetImages->removeItemWidget(selectedItem);
//          delete selectedItem;		// Don't know why this is necessary, but does not work otherwise.
  //        listWidgetImages->repaint();
    //      if (listWidgetImages->count() == 0)
      //      pushButtonRemove->setEnabled(false);

            // Set the image to NULL and repaint.

//          graphicsViewImageInput->Paint();

            // Update the image widgets

//          toggleImageWidgets(false);
//        };
//      };
    }

    /// @briefToggles all the widgets to the state specified true = enabled.
    /// @param[in] toToggle - the state to toggle the widgets to.
    /// @returns None.
    /// @throws None.
    /// @details Only the widgets associated with the image window are toggled. Should be set to "true" when an image is selected.
    /// @version 2011-06-03/GGB - Updated to reflect changes to the formWidget, as well as the use of the dockWidgets to display information.
    /// @version 2011-03-04/GGB - Function created.

    void CImageComparisonWindow::toggleImageWidgets(bool toToggle)
    {
      pushButtonAlign1->setEnabled(toToggle);
      pushButtonAlign1->setChecked(false);

      pushButtonAlign2->setEnabled(toToggle);
      pushButtonAlign2->setChecked(false);

      labelAlign1->setEnabled(toToggle);
      labelAlign2->setEnabled(toToggle);
    }

    /// @brief Procedure called when the window is activating
    /// @param None.
    /// @returns None.
    /// @throws CodeError()
    /// @version 2016-04-24/GGB - Bug# 1574157 - Ensure the view photometry and astrometry are in the correct state.
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2016-03-28/GGB - Changed C-style casts to reinterpret_cast<>()
    /// @version 2013-08-04/GGB - Added code to enable/disable the dockwidgets.
    /// @version 2013-06-03/GGB - Updated to use imageChanged() and enable more menu items.
    /// @version 2011-06-11/GGB - Function created.

    void CImageComparisonWindow::windowActivating()
    {
      mdiframe::CFrameWindow *frameWindow;
      SControlBlock *controlBlock;

      frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        // Update the dock widgets.

      if (listWidgetImages->currentItem())
      {
        controlBlock = reinterpret_cast<SControlBlock *>(listWidgetImages->currentItem()->data(ROLE_CONTROLBLOCK).toULongLong());

        if (controlBlock->inputImageValid)
        {
          imageChange(&controlBlock->inputImage);

          frameWindow->enableDockWidgetsImage(true);
        }
        else
        {
          imageChange(nullptr);

          frameWindow->enableDockWidgetsImage(false);
        };
      }
      else
      {
        imageChange(nullptr);

        frameWindow->enableDockWidgetsImage(false);
      };

        // File Menu

      frameWindow->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(false);

       // Process Menu

      frameWindow->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // View Menu

      frameWindow->getAction(mdiframe::IDA_VIEW_ZOOM11)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->setEnabled(true);
      frameWindow->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->setEnabled(true);

        // Transform Menu

      frameWindow->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      frameWindow->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);
    }

    /// @brief Menu function "View | Zoom 1:1
    /// @throws None.
    /// @version 2013-06-02/GGB - Function created.

    void CImageComparisonWindow::zoom11()
    {
      switch (tabWidget->currentIndex())
      {
      case 0:
        graphicsViewImageInput->zoom11();
        break;
      case 1:
        graphicsViewImageInput->zoom11();
        graphicsViewImageOutput->zoom11();
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Menu function View | Zoom All
    /// @throws None.
    /// @version 2013-06-02/GGB - Function created.

    void CImageComparisonWindow::zoomAll()
    {
      switch (tabWidget->currentIndex())
      {
      case 0:
        graphicsViewImageInput->zoomAll();
        break;
      case 1:
        graphicsViewImageInput->zoomAll();
        graphicsViewImageOutput->zoomAll();
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Menu function View | Zoom In
    /// @throws None.
    /// @version 2013-06-02/GGB - Function created.

    void CImageComparisonWindow::zoomIn()
    {
      switch (tabWidget->currentIndex())
      {
      case 0:
        graphicsViewImageInput->zoomIn();
        break;
      case 1:
        graphicsViewImageInput->zoomIn();
        graphicsViewImageOutput->zoomIn();
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

    /// @brief Menu function View | Zoom Out
    /// @throws None.
    /// @version 2013-06-02/GGB - Function created.

    void CImageComparisonWindow::zoomOut()
    {
      switch (tabWidget->currentIndex())
      {
      case 0:
        graphicsViewImageInput->zoomOut();
        break;
      case 1:
        graphicsViewImageInput->zoomOut();
        graphicsViewImageOutput->zoomOut();
        break;
      default:
        break;
      };
    }

    /// @brief Menu function View | Zoom Selection
    /// @throws CCodeError(astroManager)
    /// @version 2013-06-02/GGB - Function created.

    void CImageComparisonWindow::zoomSelection()
    {
      switch (tabWidget->currentIndex())
      {
      case 0:
        graphicsViewImageInput->zoomSelection();
        break;
      case 1:
        graphicsViewImageInput->zoomSelection();
        graphicsViewImageOutput->zoomSelection();
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };
    }

  }  // namespace imagecomparison
}  // namespace AstroManager
