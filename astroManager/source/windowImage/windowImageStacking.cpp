//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageStacking.cpp
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, CFitsIO, gsl.
// NAMESPACE:						AstroManager::imagingstacking
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CStackImagesWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CMdiSubWindow
//                          - CAstroImageWindow
//                            - CStackImagesWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-11 GGB - Development of classes for astroManager
//                      2011-02-13 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../include/windowImage/windowImageStacking.h"

  // astroManager header files

#include "../../include/ACL/astroFile.h"
#include "../../include/AstroGraphicsView.h"
#include "../../include/database/databaseARID.h"
#include "../../include/dialogs/dialogSelectImages.h"
#include "../../include/dockWidgets/dockWidgetHistogram.h"
#include "../../include/dockWidgets/dockWidgetImageInformation.h"
#include "../../include/dockWidgets/dockWidgetMagnify.h"
#include "../../include/dockWidgets/dockWidgetNavigator.h"
#include "../../include/FrameWindow.h"
#include "../../include/settings.h"

  // Qxt Library

#include <QxtGui/QxtConfirmationMessage>

  // Standard libraries

#include <memory>

namespace astroManager
{
  namespace imaging
  {

    //*******************************************************************************************************************************
    //
    // CStackImagesWindow
    //
    // User interface to the Stack Images function. Contains all the functionallity to identify the
    // files, open the images, set the alignment points and calibrate and stack the images.
    //
    // Images are only loaded for display when the user selects the image in the image list. There
    // is also a graphical delay showing whether the image has had the alignment points marked.
    //
    //*******************************************************************************************************************************

    int const ROLE_PATH         = Qt::UserRole + 0;
    int const ROLE_CONTROLIMAGE = Qt::UserRole + 1;
    int const ROLE_ALIGN1		    = Qt::UserRole + 2;
    int const ROLE_ALIGN2		    = Qt::UserRole + 3;
    int const ROLE_IMAGEID      = Qt::UserRole + 4;
    int const ROLE_OPENFROM     = Qt::UserRole + 5;

    enum
    {
      OF_DATABASE = 1,
      OF_FILE
    };

    enum
    {
      NOWCS_IGNORE,
      NOWCS_ASK,
      NOWCS_STOP,
      NOWCS_DELETE
    };

    /// @brief Create the class. Call the setupUI function to get the template and UI setup.
    /// @param[in] aParent - parent of the instance.
    /// @throws std::bad_alloc
    /// @version 2013-06-19/GGB - Added different definitions for windows and other.
    /// @version 2011-02-13/GGB - Function created

#ifdef _WIN32
    CStackImagesWindow::CStackImagesWindow(QWidget *aParent) : CAstroImageWindow(aParent), imageList(),
      cursorAstrometry(QString(":/cursor/cursorAstrometry.bmp")), maskAstrometry(QString(":/cursor/maskAstrometryWindows.bmp")),
      astrometry(cursorAstrometry, maskAstrometry, 15, 15), imageStack(), outputControlImage(this), alignPoint1(), alignPoint2()
#else
    CStackImagesWindow::CStackImagesWindow(QWidget *aParent) : CAstroImageWindow(aParent), imageList(),
      cursorAstrometry(QString(":/cursor/cursorAstrometry.bmp")), maskAstrometry(QString(":/cursor/maskAstrometry.bmp")),
      astrometry(cursorAstrometry, maskAstrometry, 15, 15), imageStack(), outputControlImage(this), alignPoint1(), alignPoint2(),
      addImagesMenu(nullptr)
#endif
    {
      setAttribute(Qt::WA_DeleteOnClose);

      setupUI();

      setWindowTitle(tr("Stack Images"));
    }

    /// @brief Ensures that all dyanically allocated memory is correctly returned.
    /// @throws None.
    /// @version 2013-03-10/GGB - Function created.

    CStackImagesWindow::~CStackImagesWindow()
    {
      clearImageList();

      if (addImagesMenu)
      {
        delete addImagesMenu;
        addImagesMenu = nullptr;
      };
    }

    /// @brief Ensures that all the dynamically allocated memory is freed.
    /// @throws None. (This needs to be noexcept as it is called by the destructor.)
    /// @version 2013-07-14/GGB - Added code to disable the zoom actions. (Bug #1195976)
    /// @version 2013-03-10/GGB - Function created.

    void CStackImagesWindow::clearImageList() noexcept
    {
      std::size_t itemCount = listImages->count();
      std::size_t index = 0;
      imaging::SControlImage *controlImage;

      for (; (index < itemCount) ; index++)
      {
        controlImage = reinterpret_cast<imaging::SControlImage *>(listImages->item(index)->data(ROLE_CONTROLIMAGE).toULongLong());
        if (controlImage)
        {
          delete controlImage;
        };
        controlImage = nullptr;
      };

      imageList.clear();			// Clear the images list.
      listImages->clear();		// Clear the items from the table widget.

      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      if (pw)
      {
          // View Menu

        pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);
      };

      outputControlImageValid_ = false;
    }

    /// @brief Ensures that the window name is removed from the list of windows in the frame.
    /// @param[in] event - The event data.
    /// @throws None.
    /// @version 2011-03-03/GGB - Function created

    void CStackImagesWindow::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

    /// @brief Deletes an item from the image list (listImages)
    /// @param[in] toDelete - The item to delete.
    /// @throws None.
    /// @verison 2017-08-26/GGB - Function created.

    void CStackImagesWindow::deleteListItem(QListWidgetItem *toDelete)
    {
      imaging::SControlImage *controlImage;

        // Delete any data stored in the control data.

      controlImage = reinterpret_cast<imaging::SControlImage *>(toDelete->data(ROLE_CONTROLIMAGE).toULongLong());
      if (controlImage)
      {
        delete controlImage;
        controlImage = nullptr;
      };

        // Remove the item from the list.

      delete (toDelete);
    }

    /// @brief Draws the alignment point 1
    /// @param[in] point - The x,y coordinates of the alignment point.
    /// @throws None.
    /// @version 2011-03-05/GGB - Function created

    void CStackImagesWindow::displayAlign1(const QPointF &point)
    {
      QPen pen;

      pen.setColor(Qt::red);
      alignPoint1.ellipse = gsImageInput->addEllipse(point.x()-10, point.y()-10, 2 * 11, 2 * 11, pen);
      alignPoint1.text = gsImageInput->addSimpleText(QString("1"));
      alignPoint1.text->setPos(point.x(), point.y() + 10 + 1);
      alignPoint1.text->setPen(pen);
    }

    /// @brief Draws the alignment point 2
    /// @param[in] point - The x,y coordinates of the alignment point.
    /// @throws None.
    /// @version 2011-03-05/GGB - Function created

    void CStackImagesWindow::displayAlign2(const QPointF &point)
    {
      QPen pen;

      pen.setColor(Qt::green);
      alignPoint2.ellipse = gsImageInput->addEllipse(point.x()-10, point.y()-10, 2 * 11, 2 * 11, pen);
      alignPoint2.text = gsImageInput->addSimpleText(QString("2"));
      alignPoint2.text->setPos(point.x(), point.y() + 10 + 1);
      alignPoint2.text->setPen(pen);
    }

    /// @brief Displays the alignment points for the selected items.
    /// @param[in] selectedItem - The selected item.
    /// @throws None.
    /// @version 2017-08-31/GGB - Function create.

    void CStackImagesWindow::displayAlignmentGraphics(QListWidgetItem *selectedItem)
    {
      QPointF align1;
      QPointF align2;

      if (selectedItem)
      {

        // Check if the alignment points exist and display them as required.

        if (selectedItem->data(ROLE_ALIGN1).isNull())
        {
          labelAlign1->setText(QString(""));
        }
        else
        {
          align1 = selectedItem->data(ROLE_ALIGN1).toPointF();
          labelAlign1->setText(QString("(%1, %2)").arg(align1.x(), 0, 'f', 1).arg(align1.y(), 0, 'f', 1));
          displayAlign1(align1);
        };

        if (selectedItem->data(ROLE_ALIGN2).isNull())
        {
          labelAlign2->setText(QString(""));
        }
        else
        {
          align2 = selectedItem->data(ROLE_ALIGN2).toPointF();
          labelAlign2->setText(QString("(%1, %2)").arg(align2.x(), 0, 'f', 1).arg(align2.y(), 0, 'f', 1));
          displayAlign2(align2);
        };
      };
    }

    /// @brief Select images from the ARID database.
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CStackImagesWindow::eventButtonAddImagesFromDatabase(bool)
    {
      QIcon iconRed(":/images/BMP_IMAGESTACK_RED.bmp");

      settings::astroManagerSettings->setValue(settings::IMAGESTACK_OPENFROMFOLDER, QVariant(false));
      toolButtonAddImages->setDefaultAction(actionSelectFromDatabase);

      dialogs::CDialogSelectImages dialogSelectImages(this, imageIDList);

      if (dialogSelectImages.exec())
      {
        // imageIDList has a list of imageID's that need to be retrieved and opened.

        for (auto element : imageIDList)
        {
            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          std::string imageName;
          database::databaseARID->getImageName(element, imageName);
          auto lwi = new QListWidgetItem(iconRed, QString::fromStdString(imageName));
          lwi->setData(ROLE_IMAGEID, QVariant(element) );		// Add the image ID as a user role.
          lwi->setData(ROLE_OPENFROM, QVariant(OF_DATABASE) );		// Indicate that this image opens from database.
          lwi->setForeground(Qt::gray);
          listImages->addItem(lwi);
          lwi = nullptr;
        };
        labelCount->setText(QString("%1").arg(listImages->count()));
        btnImageRemove->setEnabled(true);
        btnImageRemoveAll->setEnabled(true);
      };
    }

    /// @brief Allows the user to select a single image or selection of images
    /// @details Opens the file selection dialog and allows the selection of multiple images. Image names are added to the list of
    ///          images. The image list is not cleared.
    /// @throws None.
    /// @version 2013-06/29/GGB - Added support for button remove all.
    /// @version 2013-03-01/GGB - Include the use of the global settings::fileExtensions for the list of file extensions.
    /// @version 2011-06-12/GGB - Function Created

    void CStackImagesWindow::eventButtonAddImagesFromFolder(bool)
    {
      QStringList::const_iterator constIterator;
      QStringList newImages;
      bool firstPass = true;
      QListWidgetItem *lwi = nullptr;
      QIcon iconRed(":/images/BMP_IMAGESTACK_RED.bmp");
      boost::filesystem::path filePath;

      settings::astroManagerSettings->setValue(settings::IMAGESTACK_OPENFROMFOLDER, QVariant(true));
      toolButtonAddImages->setDefaultAction(actionSelectFromFolder);

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Open Image"),
                    settings::astroManagerSettings->value(settings::IMAGESTACK_DIRECTORY, QVariant(0)).toString(),
                    EXTENSION_IMAGE);

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
      {
        if ( !(imageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          filePath = boost::filesystem::path((*constIterator).toStdString());

          if ( firstPass && (settings::astroManagerSettings->value(settings::IMAGESTACK_UPDATEONOPEN, QVariant(true)).toBool()) )
          {
              // Get the directory details from the file name.

            settings::astroManagerSettings->setValue(settings::IMAGESTACK_DIRECTORY, QVariant(QString::fromStdString(filePath.parent_path().string())));
            firstPass = false;
          };

          imageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(iconRed, QString::fromStdString(filePath.filename().string()));
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          lwi->setData(ROLE_OPENFROM, QVariant(OF_FILE) );		// Indicate that this image opens from database.
          lwi->setForeground(Qt::gray);
          listImages->addItem(lwi);
          lwi = nullptr;
        };
      };

      labelCount->setText(QString("%1").arg(listImages->count()));
      btnImageRemove->setEnabled(true);
      btnImageRemoveAll->setEnabled(true);
    }

    /// @brief Function to handle the alignment button being unchecked by the user.
    /// @param[in] isChecked - Indicates the checked status of the button.
    /// @throws
    /// @version 2011-03-04/GGB - Function Created

    void CStackImagesWindow::eventButtonAlignment1(bool isChecked)
    {
      if ( isChecked )
      {
          // Button is selected. Allow user to select the first alignment point.
          // Cursor is changed.

        gvImageInput->setCursor(astrometry);

          // Check if the other button is checked as well

        if ( btnAlign2->isChecked() )
        {
          btnAlign2->setChecked(false);
        };
      }
      else
      {
          // Button is not selected. Change the cursor back to the normal cursor.

        gvImageInput->setCursor(Qt::CrossCursor);
      };
    }

    /// @brief Function to handle the alignment button being unchecked by the user.
    /// @param[in] isChecked - Indicates the checked status of the button.
    /// @throws
    /// @version 2011-03-05/GGB - Function Created

    void CStackImagesWindow::eventButtonAlignment2(bool isChecked)
    {

      if (isChecked)
      {
          // Button is selected. Allow the user to select the second alignment point.
          // Change the cursor

        gvImageInput->setCursor(astrometry);

          // Check if the other button is pressed, if so uncheck the other button.

        if ( btnAlign1->isChecked() )
        {
          btnAlign1->setChecked(false);
        };
      }
      else
      {
          // Button is unselected. Change back to the normal cursor.

        gvImageInput->setCursor(Qt::CrossCursor);
      };
    }

    /// @brief Performs auto stacking of the images.
    /// @details Auto stacking uses the WCS information contained in the image.
    ///          1. Two points are selected on the main BL-TR diagonal
    ///          2. The two points are correlated on all the images.
    ///          3. The images are then stacked.
    ///          4. If required, the images are saved.
    /// @note 1. Exceptions are used internally to capture abort conditions. These should not propogate outside the function.
    /// @note 2. Exceptions are also used to propogate errors outside the function.

    void CStackImagesWindow::eventButtonAutoStack(bool)
    {
      FP_t seperationDistance = settings::astroManagerSettings->value(settings::IMAGESTACK_AUTO_DISTANCE, QVariant(0.75)).toFloat();
      int itemCount = listImages->count();
      QListWidgetItem *selectedItem;
      imaging::SControlImage *controlImage;
      std::uint_least8_t noWCSAction = settings::astroManagerSettings->value(settings::IMAGESTACK_AUTO_NOWCSACTION,
                                                                     QVariant(NOWCS_IGNORE)).toInt();
      std::vector<QListWidgetItem *> deleteList;
      bool abortProcess = false;

        // Load all the images and check they all have WCS information.

      for (int index = 0; (index < itemCount) & !abortProcess; ++index)
      {
        selectedItem = listImages->item(index);
        controlImage = loadImage(selectedItem);    // Load the image into memory.
        controlImage->currentHDB = 0;

        if (!controlImage->astroFile->hasWCSData(controlImage->currentHDB) )
        {
          if (noWCSAction == NOWCS_DELETE)
          {
            deleteList.emplace_back(selectedItem);
          }
          else if (noWCSAction == NOWCS_ASK)
          {
              // Popup message box to question user what to do with the image.

            QMessageBox messageBox(this);

            messageBox.setIcon(QMessageBox::Question);
            messageBox.setText(tr("Image has no WCS"));
            QString detailedText = QString("The Image:%1 has no WCS information.").arg(selectedItem->text());
            messageBox.setDetailedText(detailedText);
            QPushButton *stopButton = messageBox.addButton("Stop Processing", QMessageBox::RejectRole);
            QPushButton *ignoreButton = messageBox.addButton("Ignore Image", QMessageBox::YesRole);
            QPushButton *deleteButton = messageBox.addButton("Delete Image", QMessageBox::DestructiveRole);
            messageBox.setDefaultButton(ignoreButton);

            messageBox.exec();

            if (messageBox.clickedButton() == stopButton)
            {
              abortProcess = true;
            }
            else if (messageBox.clickedButton() == ignoreButton)
            {
                // This leads to a potential problem if the first image does not have WCS information.
            }
            else if (messageBox.clickedButton() == deleteButton)
            {
              deleteList.emplace_back(selectedItem);
            }
            else
            {
              ASTROMANAGER_CODE_ERROR;
            }
          }
          else if (noWCSAction == NOWCS_IGNORE)
          {
              // Again this leads to a potential problem if the first image does not have WCS information.

            INFOMESSAGE("The image " + selectedItem->text().toStdString() + "has no WCS information. Ignoring.");
          }
          else if (noWCSAction == NOWCS_STOP)
          {
            abortProcess = true;
          };
        };
      };

        // Delete any images that need to be removed from the list.

      if (!deleteList.empty())
      {
        for (auto element : deleteList)
        {
          deleteListItem(element);
        };
      };

        // Choose the points for alignment and set the alignment points for each image.

      if (!(itemCount = listImages->count()))
      {
          // No images in the list anymore.

        abortProcess = true;
      };


      if (!abortProcess)
      {
        bool outOfBounds = false;
        MCL::TPoint2D<ACL::FP_t> BL, TR;
        std::optional<ACL::CAstronomicalCoordinates>  BLC, TRC;
        int initialIndex = 0;

        do
        {
          outOfBounds = false;

            // This loop runs, reducing the seperation distance until all the images have coordinates that match, or
            // if the seperationDistance falls below 25%, then the abort flag is set and the loop exits.

          selectedItem = listImages->item(initialIndex);
          controlImage = reinterpret_cast<imaging::SControlImage *>(selectedItem->data(ROLE_CONTROLIMAGE).toULongLong());

          // Determine the top-right and bottom left pixels to use.

          BL.x() = controlImage->astroFile->imageWidth() * ((1 - seperationDistance) / 2);
          BL.y() = controlImage->astroFile->imageHeight() * ((1 - seperationDistance) / 2);
          TR.x() = controlImage->astroFile->imageWidth() - (controlImage->astroFile->imageWidth() * ((1 - seperationDistance) / 2));
          TR.y() = controlImage->astroFile->imageHeight() - (controlImage->astroFile->imageHeight() * ((1 - seperationDistance) / 2));

          // Convert the pixels into coordinates.

          BLC = controlImage->astroFile->pix2wcs(controlImage->currentHDB, BL);
          TRC = controlImage->astroFile->pix2wcs(controlImage->currentHDB, TR);

          if (!BLC || !TRC)
          {
              // This can only occur if the first item has no WCS information. In this case, a mechanism is needed to skip to the
              // second item.
              // Update the initialIndex to ignore the first item.

            INFOMESSAGE("Image #" + std::to_string(initialIndex) + " has no WCS information. Skipping to next image.");
            initialIndex++;
            outOfBounds = true;
          };

          selectedItem->setData(ROLE_ALIGN1, QVariant(QPointF(BL.x(), BL.y())));						// Save the Align1 position
          selectedItem->setData(ROLE_ALIGN2, QVariant(QPointF(TR.x(), TR.y())));						// Save the Align2 position

          for (int index = initialIndex + 1; (index < itemCount) && (!outOfBounds) && (!abortProcess); ++index)
          {
            selectedItem = listImages->item(index);
            controlImage = reinterpret_cast<imaging::SControlImage *>(selectedItem->data(ROLE_CONTROLIMAGE).toULongLong());

              // Note that the variables BL and TR are redefined to a different type in this loop to what they are above.

            std::optional<MCL::TPoint2D<ACL::FP_t>> BL = controlImage->astroFile->wcs2pix(controlImage->currentHDB, *BLC);
            std::optional<MCL::TPoint2D<ACL::FP_t>> TR = controlImage->astroFile->wcs2pix(controlImage->currentHDB, *TRC);

            if (!BL || !TR)
            {
              INFOMESSAGE("Coordinates not found in image. Reducing seperation distance.");

              outOfBounds = true;
              seperationDistance *= .95;    // Reduce the seperation distance.

              if (seperationDistance < 0.25)
              {
                abortProcess = true;

                QMessageBox::critical(this, tr("Images are not congruent."),
                                      tr("The images do not appear to all be congruent. The process is being aborted."));
              }
            }
            else
            {
              selectedItem->setData(ROLE_ALIGN1, QVariant(QPointF(BL->x(), BL->y())));						// Save the Align1 position
              selectedItem->setData(ROLE_ALIGN2, QVariant(QPointF(TR->x(), TR->y())));						// Save the Align2 position
            };
          };
        }
        while (outOfBounds && !abortProcess);

      };

      if (!abortProcess)
      {
        stackImages();
      }

    }

    /// @brief When the user presses this button, the images need to be stacked.
    /// @details  1) Check that all the images have both alignment points specified.
    ///           2) Get the status of all the user flags
    ///           3) Get the master files
    ///           4) Setup the libAstroImages::CImageStack class to perform the stacking
    ///           5) Perform the stacking
    /// @version 2017-07-03/GGB - Updated to reflect new dockwidget storage.
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-05-27/GGB - Added support for the magnify and navigator widgets.
    /// @version 2013-05-20/GGB - Added member pixmap to controlImage.
    /// @version 2013-03-10/GGB - Added support for "Open In Window", removed support for zero and resize.
    /// @version 2013-03-02/GGB - Function created.

    void CStackImagesWindow::eventButtonCombineImages(bool)
    {
      stackImages();
    }

    /// @brief Function to remove selected images from the list.
    /// @throws None.
    /// @version 2013-06-30/GGB - Function created.

    void CStackImagesWindow::eventButtonImageRemove(bool)
    {
      QList<QListWidgetItem *> selectedItems = listImages->selectedItems();
      QList<QListWidgetItem *>::iterator iterator;

        // Give the user a warning!

      QxtConfirmationMessage msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("All the selected files will be removed. Are you sure you want to continue?"));
      msgBox.setConfirmationText(tr("Do not show again."));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setOverrideSettingsKey(settings::CM_IMAGESTACKING_REMOVE);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        if ( selectedItems.size() > 0 )
        {
          imageChange(nullptr);
          removeAlignmentGraphics();
          gsImageInput->clear();      // Clear the graphics scene.

          for (iterator = selectedItems.begin(); iterator != selectedItems.end(); ++iterator)
          {
            imageList.removeOne((*iterator)->data(ROLE_PATH).toString()); // Remove from the master list.

            deleteListItem(*iterator);
          };

          listImages->repaint();

          gvImageInput->Paint();

          toggleImageWidgets(false);

          listImages->clearSelection();
        };
      };
    }

    /// @brief Removes all the images from the image list.
    /// @throws
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-06-29/GGB - Bug #1185415. Clear the graphics scene before deleting the images.
    /// @version 2011-02-21/GGB - Function Created

    void CStackImagesWindow::eventButtonImageRemoveAll(bool)
    {
        // Give the user a warning!

      QxtConfirmationMessage msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("All the files will be removed. Are you sure you want to continue?"));
      msgBox.setConfirmationText(tr("Do not show again."));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setOverrideSettingsKey(settings::CM_IMAGESTACKING_REMOVEALL);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        imageChange(nullptr);
        removeAlignmentGraphics();
        gsImageInput->clear();      // Clear the graphics scene.
        clearImageList();

        btnImageRemoveAll->setEnabled(false);
        btnImageRemove->setEnabled(false);

        labelCount->setText(QString("%1").arg(listImages->count()));

        gvImageInput->Paint();

        toggleImageWidgets(false);
      };
    }

    /// @brief Function to select a bias frame. Only one bias frame can be selected to use with an image stack.
    /// @throws None.
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2011-03-04/GGB - Function Created

    void CStackImagesWindow::eventButtonSelectBias(bool)
    {
      boost::filesystem::path filePath;

      QString newImage = QFileDialog::getOpenFileName(this, tr("Select Bias Frame Image"), QString(""), tr("Image Files (*.fts)"));

      if (newImage.isNull())
      {
          // No file selected.

        widget()->findChild<QLabel *>("labelBiasFrame")->setText(QString(""));
        biasFrameFilename = std::string("");
      }
      else
      {
          // Filename selected

        biasFrameFilename = newImage.toStdString();
        filePath = biasFrameFilename.c_str();
        widget()->findChild<QLabel *>("labelBiasFrame")->setText(filePath.filename().string().c_str());
      };
    }

    /// @brief Function to select a dark frame. Only one dark frame can be selected for use with the image stack.
    /// @throws None
    /// @version 2011-03-04/GGB - Function Created.

    void CStackImagesWindow::eventButtonSelectDark(bool)
    {
      boost::filesystem::path filePath;

      QString newImage = QFileDialog::getOpenFileName(this, tr("Select Dark Frame Image"), QString(""), tr("Image Files (*.fts)"));

      if (newImage.isNull())
      {
          // No file selected.

        widget()->findChild<QLabel *>("labelDarkFrame")->setText(QString(""));
        darkFrameFilename = std::string("");
      }
      else
      {
          // Filename selected

        darkFrameFilename = newImage.toStdString();
        filePath = darkFrameFilename.c_str();
        widget()->findChild<QLabel *>("labelDarkFrame")->setText(filePath.filename().string().c_str());
      };
    }

    /// @brief Function to select a flat frame. Only one flat frame can be selected to use with image stack.
    /// @throws None
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2011-03-04/GGB - Function Created

    void CStackImagesWindow::eventButtonSelectFlat(bool)
    {
      boost::filesystem::path filePath;

      QString newImage = QFileDialog::getOpenFileName(this, tr("Select Flat Frame Image"), QString(""), tr("Image Files (*.fts)"));

      if (newImage.isNull())
      {
          // No file selected.

        widget()->findChild<QLabel *>("labelFlatFrame")->setText(QString(""));
        flatFrameFilename = std::string("");
      }
      else
      {
          // Filename selected

        flatFrameFilename = newImage.toStdString();
        filePath = flatFrameFilename.c_str();
        widget()->findChild<QLabel *>("labelFlatFrame")->setText(filePath.filename().string().c_str());
      };
    }

    /// @brief Handles the mouse press event from the graphics view window.
    /// If one of the alignment buttons is pressed (down), then the position is recorded and marked.
    /// If the Alignment1 button is down, then the Alignment1 button is raised and the Alignement2 button pressed
    /// If the Alignment2 button is down, then the Alignment2 button is raised.
    /// If the alignment position already exists, then it is changed.
    /// @throws
    /// @version 2013-03-02/GGB - Updated to use SControlImage.
    /// @version 2011-03-04/GGB - Function Created.

    void CStackImagesWindow::eventMousePress(QMouseEvent *mouseEvent)
    {
      QPointF point;
      std::optional<MCL::TPoint2D<ACL::FP_t>> centroid;
      imaging::SControlImage *controlImage = getControlImage();
      QIcon iconGreen(":/images/BMP_IMAGESTACK_GREEN.bmp");

      if (controlImage)
      {
        if ( (btnAlign1->isChecked()) && (mouseEvent->button() == Qt::LeftButton) )
        {
            // Alignment 1 button is down.

          point = gvImageInput->mapToScene(mouseEvent->pos());									// Get the image coordinates

            // Find the centroid within a reasonable area.

          centroid = controlImage->astroFile->centroid(controlImage->currentHDB,
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
            point = QPointF((*centroid).x(), (*centroid).y());

            listImages->currentItem()->setData(ROLE_ALIGN1, QVariant(point));							// Save the Align1 position

            labelAlign1->setText(QString("(%1, %2)").arg(point.x(), 0, 'f', 1).arg(point.y(), 0, 'f', 1));

              // Draw the alignment points onto the screen

            displayAlign1( point );

              // Update the buttons

            btnAlign1->setChecked(false);

            if ( !listImages->currentItem()->data(ROLE_ALIGN2).isNull() )
            {
              listImages->currentItem()->setIcon(iconGreen);
            }
            else
            {
              btnAlign2->setChecked(true);
            };
          };
        }
        else if ( (btnAlign2->isChecked()) && (mouseEvent->button() == Qt::LeftButton) )
        {
            // Alignment 2 button is down.

          point = gvImageInput->mapToScene(mouseEvent->pos());		// Get the image coordinates

          centroid = controlImage->astroFile->centroid(controlImage->currentHDB,
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
            point = QPointF((*centroid).x(), (*centroid).y());

            listImages->currentItem()->setData(ROLE_ALIGN2, QVariant(point));

            labelAlign2->setText(QString("(%1, %2)").arg(point.x(), 0, 'f', 1).arg(point.y(), 0, 'f', 1));

            // Draw the alignment points onto the screen

            displayAlign2( point );

            // Update the buttons

            btnAlign2->setChecked(false);

            // Change back to the standard cursor

            gvImageInput->setCursor(Qt::CrossCursor);

            if ( !listImages->currentItem()->data(ROLE_ALIGN1).isNull() )
            {
              listImages->currentItem()->setIcon(iconGreen);
            };
          };
        }
        else
        {	// Not doing anything.
        };
      };
    }

    /// @brief  Takes the current output file, and passes it to the parent object to open a new window containing just the
    ///         new window.
    /// @throws CCodeError(astroManager)
    /// @version 2017-07-03/GGB - Updated for new style dockWidgets storage.
    /// @version 2013-05-20/GGB - Fixed code for outputControlImage.
    /// @version 2013-03-10/GGB - Function created.

    void CStackImagesWindow::eventOpenInWindow(bool)
    {
      if (outputControlImage.astroFile)
      {
        mdiframe::CFrameWindow *frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        frameWindow->getDockWidget(mdiframe::IDDW_IMAGECONTROL).setEnabled(false);
        frameWindow->getDockWidget(mdiframe::IDDW_VIEW_HISTOGRAM).setEnabled(false);

        dynamic_cast<dockwidgets::CDockWidgetMagnify &>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_MAGNIFY)).imageDeactivating();

        frameWindow->getDockWidget(mdiframe::IDDW_VIEW_NAVIGATOR).setEnabled(false);

        frameWindow->imageCreateWindow(outputControlImage.astroFile);

        imageChange(nullptr);
        outputControlImage.astroFile.reset();
        gsImageOutput->clear();
        outputControlImageValid_ = false;

        pbOpenInWindow->setEnabled(false);
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Function called when the tab is changed.
    /// @details Need to update the current image to the relevant image depending on what tab is active.
    ///           @li Tab0 = Input files and images
    ///           @li Tab1 = Output image.
    /// @throws CCodeError(astroManager)
    /// @version 2017-07-03/GGB - Updated to use new style imageChage() functions.
    /// @version 2013-03-10/GGB - Function created.

    void CStackImagesWindow::eventTabChanged(int newTab)
    {
      if (newTab == 0)
      {
        if (listImages->currentRow() != -1)
        {
          QVariant imageData = listImages->currentItem()->data(ROLE_CONTROLIMAGE);
          imaging::SControlImage *controlImage = reinterpret_cast<imaging::SControlImage *>(imageData.toULongLong());

          imageChange(controlImage);
        }
        else
        {
          imageChange(nullptr);
        }
      }
      else if (newTab == 1)   // Output image.
      {
        if (outputControlImageValid_)
        {
          imageChange(&outputControlImage);  // The image is now loaded, it must be displayed.
        }
        else
        {
          imageChange(nullptr);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      };

    }

    /// @brief Returns a pointer to the current images SControlImage structure.
    /// @returns Pointer to the current control image.
    /// @throws None.
    /// @version 2017-08-27/GGB - Updated to return the correct controImage.
    /// @version 2013-03-01/GGB - Function Created.

    imaging::SControlImage *CStackImagesWindow::getControlImage()
    {
      imaging::SControlImage *controlImage = nullptr;

      switch (tabWidget->currentIndex())
      {
        case 0:     // Input images
        {
          QVariant imageData;

          if (listImages->currentItem())
          {
            imageData = listImages->currentItem()->data(ROLE_CONTROLIMAGE);

            if (!imageData.isNull())
            {
              controlImage = reinterpret_cast<imaging::SControlImage *>(imageData.toULongLong());
            }
          };
          break;
        };
        case 1: // Output images
        {
          controlImage = &outputControlImage;
          break;
        }
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        }
      };

      return controlImage;
    }

    /// @brief Function to load the image.
    /// @details The image may be loaded from file or from database depending on how the image has been selected. This function
    ///          handles the details of the loading.
    /// @param[in] selectedItem - Pointer to the listWidgetitem with the data to load or select.
    /// @returns Pointer to the controlImage structure.
    /// @throws GCL::CCodeError(astroManager)
    /// @throws From called functions.
    /// @version 2017-08-27/GGB - Function created.

    imaging::SControlImage *CStackImagesWindow::loadImage(QListWidgetItem *selectedItem)
    {
      QVariant imageData = selectedItem->data(ROLE_CONTROLIMAGE);
      imaging::SControlImage *controlImage = nullptr;

      if (imageData.isNull() )
      {
          // The image has not been loaded, it needs to be loaded.

        switch (selectedItem->data(ROLE_OPENFROM).toUInt() )
        {
          case OF_FILE:
          {
            std::string fileName = selectedItem->data(ROLE_PATH).toString().toStdString();		// Get the filename from the item.

              // Create the astroFile from the filename.

            controlImage = new imaging::SControlImage(this, std::make_shared<CAstroFile>(this, boost::filesystem::path(fileName)));
            break;
          };
          case OF_DATABASE:
          {
            imageID_t imageID = selectedItem->data(ROLE_IMAGEID).toUInt();
            imageVersion_t imageVersion;
            database::databaseARID->versionLatest(imageID, imageVersion);
            controlImage = new imaging::SControlImage(this, std::make_shared<CAstroFile>(this, imageID, imageVersion));
            break;
          };
          default:
          {
            ASTROMANAGER_CODE_ERROR;
            break;
          };
        };

          // Save the image data into the image role.

        imageData = QVariant(reinterpret_cast<qulonglong>(controlImage));
        selectedItem->setData(ROLE_CONTROLIMAGE, imageData);

          // Change the colour of the item to signify that the image is already loaded.

        selectedItem->setForeground(Qt::black);
      }
      else
      {
          // Set astroFile to the address of the item.

        controlImage = reinterpret_cast<imaging::SControlImage *>(imageData.toULongLong());
      };

      controlImage->currentHDB = 0;

      return controlImage;
    }

    /// @brief Remove and delete the alignment graphics.
    /// @throws None.
    /// @version 2017-08-31/GGB - Function created.

    void CStackImagesWindow::removeAlignmentGraphics()
    {
      if (alignPoint1.ellipse)
      {
        gsImageInput->removeItem(alignPoint1.ellipse);
        delete alignPoint1.ellipse;
        alignPoint1.ellipse = nullptr;
      };

      if (alignPoint1.text)
      {
        gsImageInput->removeItem(alignPoint1.text);
        delete alignPoint1.text;
        alignPoint1.text = nullptr;
      };

      if (alignPoint2.ellipse)
      {
        gsImageInput->removeItem(alignPoint2.ellipse);
        delete alignPoint2.ellipse;
        alignPoint2.ellipse = nullptr;
      };

      if (alignPoint2.text)
      {
        gsImageInput->removeItem(alignPoint2.text);
        delete alignPoint2.text;
        alignPoint2.text = nullptr;
      };
    }

    /// @brief Repaints the image when any changes occur.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2017-08-29/GGB - Function created.

    void CStackImagesWindow::repaintImage()
    {
      mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      RUNTIME_ASSERT(astroManager, pw, "Parent Widget should not == nullptr");

      switch (tabWidget->currentIndex())
      {
        case 0:     // Input images
        {
          QVariant imageData;
          imaging::SControlImage *controlImage = nullptr;

          if (listImages->currentItem())
          {
            imageData = listImages->currentItem()->data(ROLE_CONTROLIMAGE);

            if (!imageData.isNull())
            {
              controlImage = reinterpret_cast<imaging::SControlImage *>(imageData.toULongLong());
              removeAlignmentGraphics();
              gsImageInput->clear();
              gsImageInput->addPixmap(*controlImage->pixmap);

              displayAlignmentGraphics(listImages->currentItem());
              gvImageInput->Paint();         // NOTE: Any code that updates the image needs to ensure that this is called!!! Updates navigator
            }
          };
          break;
        };
        case 1: // Output images
        {
          gsImageOutput->clear();
          gsImageOutput->addPixmap(*outputControlImage.pixmap);

          gvImageOutput->Paint();         // NOTE: Any code that updates the image needs to ensure that this is called!!! Updates navigator
          break;
        }
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        }
      };
    }

    /// @brief Performs the save function. Passes control to saveAs.
    /// @throws None.
    /// @version 2013-03-10/GGB - Function created.

    bool CStackImagesWindow::save()
    {
      return saveAs();
    }

    /// @brief Performs the save as function. If there is an astroFile created, then it will be saved.
    /// @throws None
    /// version 2013-03-10/GGB - Function created.

    bool CStackImagesWindow::saveAs()
    {
      try
      {
        outputControlImage.astroFile->saveAs();
        //updateWindowTitle();
        return true;
      }
      catch (GCL::CError &err)
      {
        if (err.errorCode() == 0x1001)
        {
          QMessageBox msgBox;

          msgBox.setText(tr("Unable to save file."));
          msgBox.setInformativeText(tr("Error while saving the file."));
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          return false;
        }
        else
        {
          throw;    // Exception not handled. Throw it again.
        };
      }
      catch(...)
      {
        throw;    // Not sure why there is an error. Report it upwards and exit the application.
      };
    }

    /// @brief Saves the output image to database.
    /// @throws None.
    /// @version 2017-09-01/GGB - Function created.

    bool CStackImagesWindow::saveDatabase()
    {
      try
      {
          // Need to get some basic information to be able to register the image.

        std::string imageName = outputControlImage.astroFile->getObservationTarget();

        outputControlImage.astroFile->imageName(imageName);
        database::databaseARID->registerImage(outputControlImage.astroFile.get());
        outputControlImage.astroFile->imageVersion(0);
        outputControlImage.astroFile->save();
      }
      catch (GCL::CError &err)
      {
        if (err.errorCode() == 0x1001)
        {
          QMessageBox msgBox;

          msgBox.setText(tr("Unable to save file."));
          msgBox.setInformativeText(tr("Error while saving the file."));
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          return false;
        }
        else
        {
          throw;    // Exception not handled. Throw it again.
        };
      }
      catch(...)
      {
      };
    }

    /// @brief Loads the template for the UI. Populates all required fields
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2013-06-29/GGB - Added support for remove all button.
    /// @version 2013-03-10/GGB - Added support for "Open In Window", removed support for zero and resize.
    /// @version 2011-02-13/GGB - Function Created

    void CStackImagesWindow::setupUI()
    {
      QUiLoader loader;
      QWidget *inputTabWidget, *outputTabWidget;
      QGridLayout *glImage;

        // Create the window details from the template

      QFile file(":/windows/windowStackImages.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/windows/windowStackImages.ui.");
        ERROR(astroManager, 0x0001);
      };

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      actionSelectFromFolder = new QAction(QIcon(":/icons/folders/folder_add.png"), tr("Select from Folder"), this);
      connect(actionSelectFromFolder, SIGNAL(triggered(bool)), this, SLOT(eventButtonAddImagesFromFolder(bool)));

      actionSelectFromDatabase = new QAction(QIcon(":/icons/database/database_add.png"), tr("Select from Database"), this);
      connect(actionSelectFromDatabase, SIGNAL(triggered(bool)), this, SLOT(eventButtonAddImagesFromDatabase(bool)));

      ASSOCIATE_CONTROL(tabWidget, formWidget, "tabImages", QTabWidget);
      ASSOCIATE_CONTROL(listImages, formWidget, "listImages", QListWidget);
      ASSOCIATE_CONTROL(pbOpenInWindow, formWidget, "pushButtonOpenInWindow", QPushButton);

      ASSOCIATE_LABEL(labelCount, formWidget, "labelCount");
      ASSOCIATE_LABEL(labelAlign1, formWidget, "labelAlign1");
      ASSOCIATE_LABEL(labelAlign2, formWidget, "labelAlign2");

      ASSOCIATE_RADIOBUTTON(radioMeanCombine, formWidget, "radioMeanCombine");

      ASSOCIATE_PUSHBUTTON(btnAlign1, formWidget, "btnAlign1");
      ASSOCIATE_PUSHBUTTON(btnAlign2, formWidget, "btnAlign2");
      ASSOCIATE_PUSHBUTTON(btnImageRemove, formWidget, "btnImageRemove");
      ASSOCIATE_PUSHBUTTON(btnImageRemoveAll, formWidget, "btnImageRemoveAll");

      ASSOCIATE_PUSHBUTTON(pushButtonAutoProcess, formWidget, "pushButtonAutoProcess");

      ASSOCIATE_CONTROL(toolButtonAddImages, formWidget, "toolButtonAddImages", QToolButton);

      addImagesMenu = new QMenu;
      addImagesMenu->addAction(actionSelectFromFolder);
      addImagesMenu->addAction(actionSelectFromDatabase);

        // Set the default menu to use.

      if (settings::astroManagerSettings->value(settings::IMAGESTACK_OPENFROMFOLDER, QVariant(false)).toBool())
      {
        toolButtonAddImages->setDefaultAction(actionSelectFromFolder);
      }
      else
      {
        toolButtonAddImages->setDefaultAction(actionSelectFromDatabase);
      };

      toolButtonAddImages->setMenu(addImagesMenu);

      inputTabWidget = tabWidget->widget(0);
      outputTabWidget = tabWidget->widget(1);

      labelCount->setText("0");

        // Setup all the buttons and checkboxes

      formWidget->findChild<QGroupBox *>("groupDarkFrame")->setChecked(false);
      formWidget->findChild<QGroupBox *>("groupFlatFrame")->setChecked(false);

      radioMeanCombine->setChecked(true);
      pbOpenInWindow->setEnabled(false);

      toggleImageWidgets(false);

        // Create the connections

      connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(eventTabChanged(int)));
      connect(btnImageRemove, SIGNAL(clicked(bool)), this, SLOT(eventButtonImageRemove(bool)));
      connect(btnImageRemoveAll, SIGNAL(clicked(bool)), this, SLOT(eventButtonImageRemoveAll(bool)));

      connect(listImages, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(signalItemClickedImageList(QListWidgetItem *)));

      connect(formWidget->findChild<QPushButton *>("btnSelectDark"), SIGNAL(clicked(bool)), this, SLOT(eventButtonSelectDark(bool)));
      connect(formWidget->findChild<QPushButton *>("btnSelectFlat"), SIGNAL(clicked(bool)), this, SLOT(eventButtonSelectFlat(bool)));
      connect(formWidget->findChild<QPushButton *>("pushButtonBiasFrame"), SIGNAL(clicked(bool)), this, SLOT(eventButtonSelectBias(bool)));

      connect(btnAlign1, SIGNAL(clicked(bool)), this, SLOT(eventButtonAlignment1(bool)));
      connect(btnAlign2, SIGNAL(clicked(bool)), this, SLOT(eventButtonAlignment2(bool)));

      connect(formWidget->findChild<QPushButton *>("btnCombineImages"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCombineImages(bool)));
      connect(pushButtonAutoProcess, SIGNAL(clicked(bool)), this, SLOT(eventButtonAutoStack(bool)));
      connect(pbOpenInWindow, SIGNAL(clicked(bool)), this, SLOT(eventOpenInWindow(bool)));

        // Create the input layouts, graphics view and scene.

      glImage = static_cast<QGridLayout *>(inputTabWidget->layout());

      gvImageInput = new imaging::CAstroGraphicsView(this);
      glImage->addWidget(gvImageInput, 0, 0, 1, 1);

      gsImageInput = new QGraphicsScene();
      gvImageInput->setScene(gsImageInput);

        // Create the output layouts, graphics view and scene.

      glImage = static_cast<QGridLayout *>(outputTabWidget->layout());

      gvImageOutput = new imaging::CAstroGraphicsView(this);
      glImage->addWidget(gvImageOutput, 0, 0, 1, 1);

      gsImageOutput = new QGraphicsScene();
      gvImageOutput->setScene(gsImageOutput);
    }

    /// @brief Processes an activated item signal in the image list.
    /// @param[in] selectedItem - Pointer to the selected item.
    /// @details The selected item contains the path of the item in the Qt::UserRole data. This allows quick selection of the
    ///          filename and path of the item.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-07-14/GGB - Added code to disable the zoom actions. (Bug #1195976)
    /// @version 2011-03-03/GGB - Function Created

    void CStackImagesWindow::signalItemClickedImageList(QListWidgetItem *selectedItem)
    {
      imaging::SControlImage *controlImage = nullptr;

        // Check if the selectedItem has already had a CAstroFile loaded. If so, we need to display the image.
        // If the astro image has not been loaded, we need to load it and change the color of the test in the widget.

      try
      {
        controlImage = loadImage(selectedItem);

          // The image is now loaded, it must be displayed.

        removeAlignmentGraphics();

        if (tabWidget->currentIndex() == 1)
        {
          tabWidget->setCurrentIndex(0);
        };

        imageChange(controlImage);

        gsImageInput->addPixmap(*controlImage->pixmap);      // Draw the main image
        gvImageInput->setCursor(Qt::CrossCursor);
        //zoomAll();                                           //******

        displayAlignmentGraphics(selectedItem);

          // Now enable all the relevant widgets in the window.

        toggleImageWidgets(true);

          // Now toggle the zoom actions

        mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
        if (pw)
        {
            // View Menu

          pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(true);
          pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(true);
          pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(true);
          pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(true);
        }
        else
        {
          CODE_ERROR(astroManager);
        };
      }
      catch (...)
      {
        QString msgString = QString("<html>Do you wish to remove the file from the stacking list?</html>");

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error while reading file"));
        msgBox.setText(QString("<html>Error reading file <b>%1</b></html>").arg(QString::fromStdString(fileName)));
        msgBox.setInformativeText(msgString);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        if (msgBox.exec() == QMessageBox::Yes)
        {
            // Remove the file from the list. (fileName is the name of the file.)

          imageList.removeOne(QString(QString::fromStdString(fileName)));

          listImages->setCurrentRow(-1);
          listImages->removeItemWidget(selectedItem);
          delete selectedItem;		// Don't know why this is necessary, but does not work otherwise.
          listImages->repaint();
          if (listImages->count() == 0)
          {
            btnImageRemove->setEnabled(false);
            btnImageRemoveAll->setEnabled(false);
          };
          labelCount->setText(QString("%1").arg(listImages->count()));

             // Set the image to NULL and repaint.

          gvImageInput->Paint();

             // Update the image widgets

          toggleImageWidgets(false);

             // Now toggle the zoom actions

          mdiframe::CFrameWindow *pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
          if (pw)
          {
               // View Menu

            pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
            pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
            pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
            pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);
          }
          else
          {
            CODE_ERROR(astroManager);
          };
        };
      };
    }

    /// @brief Performs the actual image stacking.
    /// @pre All images should have alignment points assigned.
    /// @throws
    /// @version 2017-08-27/GGB - Function created.

    void CStackImagesWindow::stackImages()
    {
      int itemCount = listImages->count();
      int index = 0;
      bool bError = false;
      QListWidgetItem *selectedItem;
      MCL::TPoint2D<FP_t> align1, align2;
      QPointF pointF;
      ACL::CImageStack::EStackMode stackMode;
      imaging::SControlImage *controlImage;
      std::uint_least8_t missingAlignmentAction = settings::astroManagerSettings->value(settings::IMAGESTACK_MISSINGALIGNMENTACTION,
                                                                                QVariant(NOWCS_IGNORE)).toUInt();

      imageStack.clearFiles();             // Remmove any existing images from the image stacker.

      for (; (index < itemCount) && !bError; index++)
      {
        selectedItem = listImages->item(index);
        if ( (selectedItem->data(ROLE_ALIGN1).isNull()) || (selectedItem->data(ROLE_ALIGN2).isNull()) )
        {
          if (missingAlignmentAction != NOWCS_IGNORE)
          {
            bError = true;

            QMessageBox msgBox;
            msgBox.setText("Error: All Images do not have alignment points selected");
            msgBox.setInformativeText("Please assign two alignment points for each image.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
          };
        }
        else
        {
          pointF = selectedItem->data(ROLE_ALIGN1).toPointF();
          align1 = MCL::TPoint2D<FP_t>(pointF.x(), pointF.y());

          pointF = selectedItem->data(ROLE_ALIGN2).toPointF();
          align2 = MCL::TPoint2D<FP_t>(pointF.x(), pointF.y());

          controlImage = reinterpret_cast<imaging::SControlImage *>(selectedItem->data(ROLE_CONTROLIMAGE).toULongLong());

          imageStack.addFile(controlImage->astroFile, align1, align2);
        };
      };

      if (!bError)
      {
          // Continue combining the images.

        //if (qFindChild<QGroupBox *>(widget(), "groupDarkFrame")->isChecked())
        //{
        //    // Open the dark frame if one is assigned and pass the image plane to the imageStack.

        //  //imageStack.setDarkFrame(calibrationFrame);
        //};
        //if (qFindChild<QGroupBox *>(widget(), "groupFlatFrame")->isChecked())
        //{
        //    // Open the flat frame is one is assigned

        //  //imageStack.setFlatFrame(calibrationFrame);
        //};
        //if (qFindChild<QGroupBox *>(widget(), "groupBiasFrame")->isChecked())
        //{
        //  // Open the bias frame if one is assigned.

        //  //imageStack.setBiasFrame(calibrationFrame);
        //};

          // All the images are set-up at this point.
          // Bias, flat and dark are passed as required.
          // Flags are assigned.
          // Perform the combine

        if (widget()->findChild<QRadioButton *>("radioAddCombine")->isChecked())
        {
          stackMode = ACL::CImageStack::SM_SUM;
        }
        else if (radioMeanCombine->isChecked())
        {
          stackMode = ACL::CImageStack::SM_MEAN;
        }
        else if (widget()->findChild<QRadioButton *>("radioMedianCombine")->isChecked())
        {
          stackMode = ACL::CImageStack::SM_MEDIAN;
        }
        else if (widget()->findChild<QRadioButton *>("radioSigmaClip")->isChecked())
        {
          stackMode = ACL::CImageStack::SM_SIGMACLIP;
        }
        else
        {
          stackMode = ACL::CImageStack::SM_NONE;
        };

          // This will delete the previous output image and set the new image.

        std::unique_ptr<ACL::CAstroFile> &outputImage = imageStack.stackImages(stackMode);
        outputControlImage.astroFile.reset(dynamic_cast<CAstroFile *>(outputImage.release()));

        outputControlImage.astroFile->keywordWrite(0, ACL::ASTROMANAGER_UUID, QUuid::createUuid().toString().toUpper().toStdString(),
                                                   ACL::ASTROMANAGER_COMMENT_UUID);
        outputControlImage.astroFile->fileNameValid(false);
        outputControlImage.astroFile->imageIDValid(false);
        outputControlImage.astroFile->syntheticImage(true);

        outputControlImage.currentHDB = 0;

        outputControlImageValid_ = true;

          // Now need to add the output file to the output window.

        CAstroImageWindow::imageChange(&outputControlImage);

        outputControlImage.pixmap->convertFromImage(*outputControlImage.ScreenImage);

        gsImageOutput->addPixmap(*outputControlImage.pixmap);      // Draw the main image
        gvImageOutput->setCursor(Qt::CrossCursor);
        gvImageOutput->zoomAll();
        pbOpenInWindow->setEnabled(true);
        tabWidget->setCurrentIndex(1);
      }
      else
      {
        imageStack.clearFiles();
      };
    }

    /// @brief Toggles all the widgets to the state specified true = enabled.
    /// @details Only the widgets associated with the image window are toggled. Should be set to "true" when an image is selected.
    /// @param[in] toToggle - The imageWidget to toggle.
    /// @throws None.
    /// @version 2013-06-29/GGB - Use variables for buttons rather than searching as required.
    /// @version 2011-06-03/GGB - Updated to reflect changes to the formWidget, as well as the use of the dockWidgets to display information.
    /// @version 2011-03-04/GGB - Function created

    void CStackImagesWindow::toggleImageWidgets(bool toToggle)
    {
      btnAlign1->setEnabled(toToggle);
      btnAlign1->setChecked(false);

      btnAlign2->setEnabled(toToggle);
      btnAlign2->setChecked(false);

      labelAlign1->setEnabled(toToggle);
      labelAlign2->setEnabled(toToggle);
    }

    /// @brief This function is called when the window is activating. It updates the menu actions to be current and up to date.
    /// @throws GCL::CCodeError()
    /// @version 2016-04-24/GGB - Bug# 1574157 - Ensure the view photometry and astrometry are in the correct state.
    /// @version 2012-01-26/GGB - Function created

    void CStackImagesWindow::windowActivating()
    {
      mdiframe::CFrameWindow *pw;

      pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
      if (pw)
      {

        dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->enableDockWidgetsImage(true);

          // File Menu

        pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(true);
        pw->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(true);
        pw->getAction(mdiframe::IDA_FILE_SAVE_DATABASE)->setEnabled(true);

          // Process Menu

        pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
        pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

          // View Menu

        pw->getAction(mdiframe::IDA_VIEW_ZOOMIN)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ZOOMOUT)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ZOOMALL)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ZOOMSELECTION)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_ASTROMETRY)->setEnabled(false);
        pw->getAction(mdiframe::IDA_VIEW_PHOTOMETRY)->setEnabled(false);

          // Transform Menu

        pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
        pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
        pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
        pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
        pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
        pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
        pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief Menu function "View | Zoom 1:1
    /// @throws GCL::CRuntimeError()
    /// @version 2013-06-29/GGB - Bug #1195976
    /// @version 2013-03-02/GGB - Function created.

    void CStackImagesWindow::zoom11()
    {
      switch (tabWidget->currentIndex())
      {
        case 0:
        {
          gvImageInput->zoom11();
          break;
        };
        case 1:
        {
          gvImageInput->zoom11();
          gvImageOutput->zoom11();
          break;
        };
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Menu function View | Zoom All
    /// @throws GCL::CRuntimeError()
    /// @version 2013-06-29/GGB - Bug #1195976
    /// @version 2013-02-02/GGB - Function created.

    void CStackImagesWindow::zoomAll()
    {
      switch (tabWidget->currentIndex())
      {
        case 0:
        {
          gvImageInput->zoomAll();
          break;
        }
        case 1:
        {
          gvImageInput->zoomAll();
          gvImageOutput->zoomAll();
          break;
        }
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Menu function View | Zoom In
    /// @throws GCL::CRuntimeError()
    /// @version 2013-06-29/GGB - Bug #1195976
    /// @version 2013-03-02/GGB - Function created

    void CStackImagesWindow::zoomIn()
    {
      switch (tabWidget->currentIndex())
      {
        case 0:
        {
          gvImageInput->zoomIn();
          break;
        }
        case 1:
        {
          gvImageInput->zoomIn();
          gvImageOutput->zoomIn();
          break;
        };
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Menu function View | Zoom Out
    /// @throws GCL::CRuntimeError()
    /// @version 2013-06-29/GGB - Bug #1195976
    /// @version 2013-03-02/GGB - Function Created

    void CStackImagesWindow::zoomOut()
    {
      switch (tabWidget->currentIndex())
      {
        case 0:
        {
          gvImageInput->zoomOut();
          break;
        }
        case 1:
        {
          gvImageInput->zoomOut();
          gvImageOutput->zoomOut();
          break;
        }
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        };
      };
    }

    /// @brief Menu function View | Zoom Selection
    /// @throws GCL::CRuntimeError()
    /// @version 2013-06-29/GGB - Bug #1195976
    /// @version 2013-03-02/GGB - Function Created

    void CStackImagesWindow::zoomSelection()
    {
      switch (tabWidget->currentIndex())
      {
        case 0:
        {
          gvImageInput->zoomSelection();
          break;
        };
        case 1:
        {
          gvImageInput->zoomSelection();
          gvImageOutput->zoomSelection();
          break;
        };
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        };
      };
    }

  }    // namespace imagestacking
}    // namespace AstroManager
