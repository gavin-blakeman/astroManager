//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageCalibration.cpp
// SUBSYSTEM:						Classes to interface image calibration
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	libAstroClass
//                      Qt
// NAMESPACE:						AstroManager::imageCalibration
// AUTHOR:							Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2010-12-08 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/windowCalibration/ImageCalibration.h"


#include "../../include/FrameWindow.h"

//#include <QtUiTools/QtUiTools>

#include "../../include/Settings.h"

#include "boost/filesystem.hpp"

#include <ACL>

namespace astroManager
{
  namespace calibration
  {
    //*****************************************************************************************************************************
    //
    // CCreateMasterDarkWindow
    //
    //*****************************************************************************************************************************

    #define ROLE_PATH			Qt::UserRole + 0

    CCreateMasterDarkWindow::CCreateMasterDarkWindow(QWidget *aParent) : CWindowCalibration(aParent)
    {
      //connect(this, SIGNAL(aboutToActivate()), this, SLOT(windowActivating()));

      setupUI();
    }

    // Processes the close event. Lets the MDI window know that this sub-window has been closed.
    // Saves the state of the check boxes before closing
    /// @version 2017-06-14/GGB - Update to Qt5
    // 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::closeEvent(QCloseEvent *event)
    {
      bool b1 = false;

      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEDARK_USEBIAS,
                                       QVariant(b1 = widget()->findChild<QGroupBox *>("groupBoxIncludeBiasCorrection")->isChecked()));
      if (b1)
      {
        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEDARK_MASTERBIAS,
          QVariant(widget()->findChild<QGroupBox *>("groupBoxMasterBiasFrame")->isChecked()));
        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEDARK_BIASSELECTION,
          QVariant(widget()->findChild<QGroupBox *>("groupBoxBiasFramesSelection")->isChecked()));
        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEDARK_BIASSAVEAS,
          QVariant(widget()->findChild<QGroupBox *>("groupBoxMasterBiasSaveAs")->isChecked()));
      };

      ((mdiframe::CFrameWindow *) nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

    // Changes the state of the bias frame remove button
    //
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-21/GGB - Function created.

    void CCreateMasterDarkWindow::eventBiasListSelectionChanged()
    {
      widget()->findChild<QPushButton *>("buttonRemoveBiasFrames")->setEnabled(true);
    };

    // Called when the create button is clicked by the user.
    // Checks that their is valid data and then runs through the creation process.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-21/GGB - Function created.

    void CCreateMasterDarkWindow::eventBtnCreateClicked(bool)
    {
      QMessageBox msgBox;
      bool bExit = false;
      bool bForceBasic = false;
      bool useMasterBiasFrame = false, useBiasFrames = false, saveMasterBiasFrame = false;

      QStringList::iterator iter;
      ACL::CMasterBiasFrame *masterBiasFrame;
      ACL::CMasterDarkFrame *masterDarkFrame;
      ACL::CAstroFile *masterBiasFile = NULL;

        // Read the state of the check boxes.

      if ( widget()->findChild<QGroupBox *>("groupBoxIncludeBiasCorrection")->isChecked() )
      {
          // Trying to use/create a  master bias frame.

        if ( !(useMasterBiasFrame = widget()->findChild<QGroupBox *>("groupBoxMasterBiasFrame")->isChecked()) )
        {
          if ( (useBiasFrames = (widget()->findChild<QGroupBox *>("groupBoxBiasFramesSelection")->isChecked())) )
            saveMasterBiasFrame = widget()->findChild<QGroupBox *>("groupBoxMasterBiasSaveAs")->isChecked();
          else
            CODE_ERROR(astroManager);
        };
      };

        // Check that there are a number of dark frames selected.

      if (darkImageList.size() < 2)
      {
        msgBox.setText(tr("Not enough Dark Frames Selected."));
        msgBox.setInformativeText(tr("You must select two or more dark frames for combination into a master dark frame."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if (darkFileName.isNull())
      {
        msgBox.setText(tr("Master Dark Name not set."));
        msgBox.setInformativeText(tr("A name for the Master Dark to be saved as must be set."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if ( useMasterBiasFrame && (biasFileName.isNull()) )
      {
        msgBox.setText(tr("Bias Frame Not set."));
        msgBox.setInformativeText(tr("In advanced mode a bias frame is required. Continue without a bias frame and \
                                     use the basic protocol?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
          bForceBasic = true;
        else
          bExit = true;
      }
      else if ( useBiasFrames && (biasImageList.size() < 2) )
      {
        msgBox.setText(tr("Not enough bias images selected to create a master bias frame."));
        msgBox.setInformativeText(tr("At least 2 images must be selected to create a master bias frame.\
                                     Continue without creating or using a master bias frame?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
          bForceBasic = true;
        else
          bExit = false;
      };

      if (!bExit)
      {
          // All the basic requirements have been met. Filenames are selected as required. Additional checks will
          // be carried out by the libAstroClass function.

          // If we are going into forced basic mode, just force all the bias frame use flags to false.

        if (bForceBasic)
        {
          useMasterBiasFrame = false;
          useBiasFrames = false;
          saveMasterBiasFrame = false;
        };

          // If we are using master bias frames, we must create the master bias frame at this stage.

        if (useBiasFrames)
        {
            // Creating a new master bias frame.

          masterBiasFrame = new ACL::CMasterBiasFrame(masterBiasSaveAs.toStdString());

          for (iter = biasImageList.begin(); iter != biasImageList.end(); iter++)
            masterBiasFrame->addBiasFrame( (*iter).toStdString() );

          try
          {
            masterBiasFrame->verifyFrames();
          }
          catch (GCL::CError &err)
          {
            switch (err.errorCode())
            {
            case 0x2000:    // image array sizes
              msgBox.setText(tr("Bias Image Array sizes are inconsistent."));
              msgBox.setInformativeText(tr("All the image arrays must have the same number of axes (NAXIS)."));
              msgBox.setIcon(QMessageBox::Warning);
              msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.setDefaultButton(QMessageBox::Ok);
              msgBox.exec();
              break;
            case 0x2001:    // image sizes
              msgBox.setText(tr("Bias Image sizes are inconsistent."));
              msgBox.setInformativeText(tr("All the image must have the same sizes (NAXIS1, NAXIS2, NAXIS3)."));
              msgBox.setIcon(QMessageBox::Warning);
              msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.setDefaultButton(QMessageBox::Ok);
              msgBox.exec();
              break;
            case 0x2002:    // exposure times
              msgBox.setText(tr("Bias Image exposures are inconsistent."));
              msgBox.setInformativeText(tr("All the images must have the same exposure times."));
              msgBox.setIcon(QMessageBox::Warning);
              msgBox.setStandardButtons(QMessageBox::Ok);
              msgBox.setDefaultButton(QMessageBox::Ok);
              msgBox.exec();
              break;
            default:
              CODE_ERROR(astroManager);
            };  // switch
          }; // catch
          if (saveMasterBiasFrame)
          {
//            masterBiasFrame->save();
          }

          masterBiasFile = masterBiasFrame->setMasterBiasFile(NULL);      // Take control of the master bias frame.

          delete masterBiasFrame;
          masterBiasFrame = NULL;
        }
        else if (useMasterBiasFrame)
        {
//          masterBiasFile = new ACL::CAstroFile(biasFileName.toStdString());
        };

          // Create the master dark frame

        masterDarkFrame = new ACL::CMasterDarkFrame(darkFileName.toStdString());
        masterDarkFrame->setMasterBiasFile(masterBiasFile);

          // Now add the dark frame file names

        for (iter = darkImageList.begin(); iter != darkImageList.end(); iter++)
          masterDarkFrame->addDarkFrame( (*iter).toStdString() );

          // Call the CMasterDarkFrame class to verify all the dark frames are compatible.

        try
        {
          masterDarkFrame->verifyFrames();
        }
        catch (GCL::CError &error)
        {
          if (error.library() == "ACL")
          {
            switch (error.errorCode())
            {
              case 0x2000:    // image array sizes
              {
                msgBox.setText(tr("Image Array sizes are inconsistent."));
                msgBox.setInformativeText(tr("All the image arrays must have the same number of axes (NAXIS)."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              case 0x2001:    // image sizes
              {
                msgBox.setText(tr("Image sizes are inconsistent."));
                msgBox.setInformativeText(tr("All the image must have the same sizes (NAXIS1, NAXIS2, NAXIS3)."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              case 0x2002:    // exposure times
              {
                msgBox.setText(tr("Image exposures are inconsistent."));
                msgBox.setInformativeText(tr("All the images must have the same exposure times."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              default:
              {
                CODE_ERROR(astroManager);
              };
            };  // switch
          };  // if
        };  // catch

        masterDarkFrame->createMasterFrame(ACL::EIC_MEAN);
//        masterDarkFrame->save();

        msgBox.setText(tr("Master Dark Frame Created and Saved."));
        msgBox.setInformativeText(tr("The Master Dark Frame has been created using the dark frames."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        if (masterBiasFile)
        {
          delete masterBiasFile;
          masterBiasFile = nullptr;
        };

        if (masterDarkFrame)
        {
          delete masterDarkFrame;
          masterDarkFrame = nullptr;
        };
      };
    }

    // Function called when the user wished to remove an item from the list of items.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::eventBtnRemoveImagesClicked(bool)
    {
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetDarkFrames");
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;

        // Give the user a warning!

      QMessageBox msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("All the selected files will be removed from the target list. Are you sure \
                                   you want to continue?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        QList<QListWidgetItem *> toRemove = listImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;
          darkImageList.removeOne(lwi->data(ROLE_PATH).toString()); // Remove from the master list.
          delete lwi;
          lwi = NULL;
        };
        widget()->findChild<QPushButton *>("btnRemoveImages")->setEnabled(false);
      };
    };

    // If an item is selected, then it can be removed from the list.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created

    void CCreateMasterDarkWindow::eventDarkListSelectionChanged()
    {
      widget()->findChild<QPushButton *>("btnRemoveImages")->setEnabled(true);
    }

    // Handles the group box check.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-21/GGB - Function created.

    void CCreateMasterDarkWindow::eventGroupBoxBiasFramesSelection(bool checked)
    {
      widget()->findChild<QGroupBox *>("groupBoxMasterBiasFrame")->setChecked(!checked);
    }

    // Handles enabling of the bias frame box.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::eventGroupBoxMasterBiasFrameSelected(bool checked)
    {
      widget()->findChild<QGroupBox *>("groupBoxBiasFramesSelection")->setChecked(!checked);
    }

    // Removes the selected items from the list and changes the state of the remove button.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-21/GGB - Function created.

    void CCreateMasterDarkWindow::eventRemoveBiasFramesClicked(bool)
    {
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetBiasFrames");
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;

        // Give the user a warning!

      QMessageBox msgBox;
      msgBox.setText(tr("Frame Removal."));
      msgBox.setInformativeText(tr("All the selected frames will be removed from the target list. Are you sure \
                                   you want to continue?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        QList<QListWidgetItem *> toRemove = listImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;
          biasImageList.removeOne(lwi->data(ROLE_PATH).toString()); // Remove from the master list.
          delete lwi;
          lwi = NULL;
        };
        widget()->findChild<QPushButton *>("buttonRemoveBiasFrames")->setEnabled(false);
      };
    }

    // Allows the user to select a Save As name for the master bias file.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-21/GGB - Function created.

    void CCreateMasterDarkWindow::eventSaveMasterBiasAsClicked(bool)
    {
      std::string myPath;
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Master Bias Frame as..."),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant("")).toString(),
        tr("FITS Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        masterBiasSaveAs = fileName;
        widget()->findChild<QLineEdit *>("lineEditMasterBiasSaveAs")->setText(masterBiasSaveAs);

        myPath = masterBiasSaveAs.toStdString();
        filePath = myPath.c_str();

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    }

    // Allows the user to select the bias master file name.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::eventSelectBiasFrameClicked(bool)
    {
      std::string myPath;
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getOpenFileName(this, tr("Select Master Bias Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        biasFileName = fileName;
        widget()->findChild<QLineEdit *>("leBiasName")->setText(biasFileName);

        myPath = biasFileName.toStdString();
        filePath = myPath.c_str();

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    };

    // Allows the user to select a number of bias frame names.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-21/GGB - Function created.

    void CCreateMasterDarkWindow::eventSelectBiasFramesClicked(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      std::string myPath;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetBiasFrames");

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Select Bias Frames"),
                    settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(0)).toString(),
                    tr("Image Files (*.fts)"));

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(biasImageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          myPath = (*constIterator).toStdString();
          filePath = myPath.c_str();

          if ( firstPass )
          {	// Get the directory details from the file name.
            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
            firstPass = false;
          };

          biasImageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listImages->addItem(lwi);
          lwi = NULL;
        };
    };

    // The user wishes to select a number of images to create a dark frame.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::eventSelectDarkFramesClicked(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      std::string myPath;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetDarkFrames");

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Open Images"),
                    settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(0)).toString(),
                    tr("Image Files (*.fts)"));

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(darkImageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          myPath = (*constIterator).toStdString();
          filePath = myPath.c_str();

          if ( firstPass )
          {	// Get the directory details from the file name.
            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
            firstPass = false;
          };

          darkImageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listImages->addItem(lwi);
          lwi = nullptr;
        };
    }

    // Allows the user to select the name of the master dark.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::eventSelectMasterNameClicked(bool)
    {
      std::string myPath;
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Master Dark Frame as..."),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        darkFileName = fileName;
        widget()->findChild<QLineEdit *>("leMasterDarkName")->setText(darkFileName);

        myPath = darkFileName.toStdString();
        filePath = myPath;

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    }

    // Sets up the user interface and all the connections.
    // Reads the values from the settings to setup the checkboxes in the window correctly.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-07/GGB - Function created.

    void CCreateMasterDarkWindow::setupUI()
    {
      QUiLoader loader;

        // Create the window details from the template

      QFile file(":/forms/windowCreateMasterDarkFrame.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      setWindowTitle(tr("Create Master Dark Frame"));

        // Read the status of the check groups from the Settings

      formWidget->findChild<QGroupBox *>("groupBoxIncludeBiasCorrection")->
        setChecked(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEDARK_USEBIAS, QVariant(false)).toBool());
      formWidget->findChild<QGroupBox *>("groupBoxMasterBiasFrame")->
        setChecked(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEDARK_MASTERBIAS, QVariant(false)).toBool());
      formWidget->findChild<QGroupBox *>("groupBoxBiasFramesSelection")->
        setChecked(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEDARK_BIASSELECTION, QVariant(false)).toBool());
      formWidget->findChild<QGroupBox *>("groupBoxMasterBiasSaveAs")->
        setChecked(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEDARK_BIASSAVEAS, QVariant(false)).toBool());

        // Create the connections

      connect(formWidget->findChild<QPushButton *>("btnSelectDarkFrames"),
              SIGNAL(clicked(bool)), this, SLOT(eventSelectDarkFramesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnRemoveImages"),
        SIGNAL(clicked(bool)), this, SLOT(eventBtnRemoveImagesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnMasterBiasFrame"),
        SIGNAL(clicked(bool)), this, SLOT(eventSelectBiasFrameClicked(bool)));

      connect(formWidget->findChild<QPushButton *>("btnSaveAs"),
        SIGNAL(clicked(bool)), this, SLOT(eventSelectMasterNameClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("buttonMasterBiasSaveAs"),
        SIGNAL(clicked(bool)), this, SLOT(eventSaveMasterBiasAsClicked(bool)));

      connect(formWidget->findChild<QPushButton *>("buttonSelectBiasFrames"),
        SIGNAL(clicked(bool)), this, SLOT(eventSelectBiasFramesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("buttonRemoveBiasFrames"),
        SIGNAL(clicked(bool)), this, SLOT(eventRemoveBiasFramesClicked(bool)));

      connect(formWidget->findChild<QPushButton *>("btnCreate"),
        SIGNAL(clicked(bool)), this, SLOT(eventBtnCreateClicked(bool)));

      connect(formWidget->findChild<QGroupBox *>("groupBoxMasterBiasFrame"),
        SIGNAL(clicked(bool)), this, SLOT(eventGroupBoxMasterBiasFrameSelected(bool)));
      connect(formWidget->findChild<QGroupBox *>("groupBoxBiasFramesSelection"),
        SIGNAL(clicked(bool)), this, SLOT(eventGroupBoxBiasFramesSelection(bool)));

      connect(formWidget->findChild<QListWidget *>("listWidgetDarkFrames"),
        SIGNAL(itemSelectionChanged()), this, SLOT(eventDarkListSelectionChanged()));
      connect(formWidget->findChild<QListWidget *>("listWidgetBiasFrames"),
        SIGNAL(itemSelectionChanged()), this, SLOT(eventBiasListSelectionChanged()));
    }

    // Procedure called when the window is activating.
    // Actions are enabled/disabled/hidden etc by this procedure.
    //
    // 2011-05-15/GGB - Function created.

    void CCreateMasterDarkWindow::windowActivating()
    {
      mdiframe::CFrameWindow *pw;

      pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(false);

        // Calibration menu

      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);

    }

    //*****************************************************************************************************************************
    //
    // CCreateMasterFlatWindow
    //
    //*****************************************************************************************************************************

    // Class constructor.
    //
    // 2011-05-15/GGB - Function created.

    CCreateMasterFlatWindow::CCreateMasterFlatWindow(QWidget *aParent) : CWindowCalibration(aParent)
    {
      connect(this, SIGNAL(aboutToActivate()), this, SLOT(windowActivating()));

      setupUI();
    }

    /// Processes the close event. Lets the MDI window know that this sub-window has been closed.
    /// Saves the state of the check boxes before closing
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2015-01-04/GGB - Updated Macro constants to c++ constants.
    /// @version 2011-05-20/GGB - Function created.

    void CCreateMasterFlatWindow::closeEvent(QCloseEvent *event)
    {
      bool b1 = false;

      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEFLAT_USEDARK,
        QVariant(b1 = widget()->findChild<QGroupBox *>("groupBoxFlatDark")->isChecked()));
      if (b1)
      {
        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEFLAT_MASTERDARK,
          QVariant(widget()->findChild<QGroupBox *>("groupBoxMasterDark")->isChecked()));
        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEFLAT_DARKSELECTION,
          QVariant(widget()->findChild<QGroupBox *>("groupBoxDarkFrames")->isChecked()));
        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_CREATEFLAT_DARKSAVEAS,
          QVariant(widget()->findChild<QGroupBox *>("groupBoxMasterDarkSaveAs")->isChecked()));
      };

      ((mdiframe::CFrameWindow *) nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

    // Allows the user to select a number of dark frames to create a flat-dark master.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventBtnAddDarkFramesClicked(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      std::string myPath;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetDarkFrames");

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Select Dark Images to add"),
                    settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(0)).toString(),
                    tr("Image Files (*.fts)"));

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(flatImageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          myPath = (*constIterator).toStdString();
          filePath = myPath;

          if ( firstPass )
          {	// Get the directory details from the file name.
            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
            firstPass = false;
          };

          darkImageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listImages->addItem(lwi);
          lwi = nullptr;
        };
    }

    // Does the actual creation work for the master flat and dark (if required)
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-23/GGB - Function created.

    void CCreateMasterFlatWindow::eventBtnCreateClicked(bool)
    {
      QMessageBox msgBox;
      bool bExit = false;
      ACL::CMasterFlatFrame *masterFlatFrame = nullptr;
      ACL::CAstroFile *masterDarkFile = nullptr;
      ACL::CMasterDarkFrame *masterDarkFrame = nullptr;
      QStringList::const_iterator iter;
      bool useMasterDark = false, useDarkFrames = false, saveMasterDark = false;

        // First collect all the flag values.

      if ( (useMasterDark = widget()->findChild<QGroupBox *>("groupBoxFlatDark")->isChecked()) )
      {
        if ( !(useMasterDark = widget()->findChild<QGroupBox *>("groupBoxMasterDark")->isChecked()) )
        {
          useDarkFrames = true;
          saveMasterDark = widget()->findChild<QGroupBox *>("groupBoxMasterDarkSaveAs")->isChecked();
        }
        else
        {
          useMasterDark = true;
        };
      };

        // Now do all the checks for correctness.

      if (flatImageList.size() <= 2)
      {
        msgBox.setText(tr("Not enough Flat Frames Selected."));
        msgBox.setInformativeText(tr("You must select two or more flat frames for combination into a master flat frame."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if (flatFileName.isNull())
      {
        msgBox.setText(tr("A file name must be selected for the master flat."));
        msgBox.setInformativeText(tr("You must select a file name to save the master flat as."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if ( useMasterDark && darkFileName.isNull() )
      {
        msgBox.setText(tr("A file name must be selected for the master flat-dark."));
        msgBox.setInformativeText(tr("You must select a file name for the master flat-dark frame."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if (useDarkFrames)
      {
        if (darkImageList.size() <= 2)
        {
          msgBox.setText(tr("Not enough dark frames selected."));
          msgBox.setInformativeText(tr("Two or more dark frames need to be selected."));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          bExit = true;
        }
        else if (darkSaveAs.isNull())
        {
          msgBox.setText(tr("Master dark file name needed."));
          msgBox.setInformativeText(tr("A file name must be set for the master dark to be saved as."));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          bExit = true;
        };
      };

      if (!bExit)
      {   // Ok to proceed.
          // Create the object to perform the conversion.

        if (useMasterDark)
        {
          if (useDarkFrames)
          {
            masterDarkFrame = new ACL::CMasterDarkFrame(darkSaveAs.toStdString());

            for (iter = darkImageList.begin(); iter != darkImageList.end(); iter++)
              masterDarkFrame->addDarkFrame( (*iter).toStdString() );

            try
            {
              masterDarkFrame->verifyFrames();
            }
            catch (GCL::CError &error)
            {
              if (error.library() == "ACL")
              {
                switch (error.errorCode())
                {
                  case 0x2000:    // image array sizes
                  {
                    msgBox.setText(tr("Dark Frame image ""naxis"" are inconsistent."));
                    msgBox.setInformativeText(tr("All the flat frames must have the same number of axes (NAXIS)."));
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    break;
                  };
                  case 0x2001:    // image sizes
                  {
                    msgBox.setText(tr("Dark Frame image sizes are inconsistent."));
                    msgBox.setInformativeText(tr("All the image must have the same sizes (NAXIS1, NAXIS2, NAXIS3)."));
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    break;
                  };
                  case 0x2002:    // exposure times
                  {
                    msgBox.setText(tr("Dark Frame exposures are inconsistent."));
                    msgBox.setInformativeText(tr("All the Flat Frames must have the same exposure times."));
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                    break;
                  };
                  default:
                  {
                    CODE_ERROR(astroManager);
                  };
                };  // switch
              };  // if
            };  // catch

            if (saveMasterDark)
            {
//              masterDarkFrame->save();
            }

            masterDarkFile = masterDarkFrame->setMasterDarkFile(NULL);    // Take control of the file.
            delete masterDarkFrame;
            masterDarkFrame = nullptr;
          }
          else
          {
//            masterDarkFile = new ACL::CAstroFile(darkFileName.toStdString());
          };
        };

        masterFlatFrame = new ACL::CMasterFlatFrame( flatFileName.toStdString() );
        masterFlatFrame->setMasterFlatDarkFile(masterDarkFile);

          // Copy the flat file names into the object.

        for (iter = flatImageList.begin(); iter != flatImageList.end(); iter++)
          masterFlatFrame->addFlatFrame( (*iter).toStdString() );

          // All data is now copied to the object. Verify that the files are all consistent.

        try
        {
          masterFlatFrame->verifyFrames();
        }
        catch (GCL::CError &error)
        {
          if (error.library() == "ACL")
          {
            switch (error.errorCode())
            {
              case 0x2000:    // image array sizes
              {
                msgBox.setText(tr("Flat Frame image ""naxis"" are inconsistent."));
                msgBox.setInformativeText(tr("All the flat frames must have the same number of axes (NAXIS)."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              case 0x2001:    // image sizes
              {
                msgBox.setText(tr("Flat Frame image sizes are inconsistent."));
                msgBox.setInformativeText(tr("All the image must have the same sizes (NAXIS1, NAXIS2, NAXIS3)."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              case 0x2002:    // exposure times
              {
                msgBox.setText(tr("Flat Frame exposures are inconsistent."));
                msgBox.setInformativeText(tr("All the Flat Frames must have the same exposure times."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              default:
              {
                CODE_ERROR(astroManager);
              };
            };  // switch
          };  // if
        };  // catch

        masterFlatFrame->createMasterFrame(ACL::EIC_MEAN);
//        masterFlatFrame->save();

        msgBox.setText(tr("Master Flat Frame Created and Saved."));
        msgBox.setInformativeText(tr("The Master Flat Frame has been created using the flat frames."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        if (masterFlatFrame)
        {
          delete masterFlatFrame;
          masterFlatFrame = NULL;
        };

        if (masterDarkFile)
        {
          delete masterDarkFile;
          masterDarkFile = NULL;
        };
      };
    }

    /// Removes selected dark frames from the list.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventBtnRemoveDarkFramesClicked(bool)
    {
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetDarkFrames");
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;

        // Give the user a warning!

      QMessageBox msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("The selected files will be removed from the target list. Are you sure you want to continue?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        QList<QListWidgetItem *> toRemove = listImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;
          darkImageList.removeOne(lwi->data(ROLE_PATH).toString()); // Remove from the master list.
          delete lwi;
          lwi = NULL;
        };
        widget()->findChild<QPushButton *>("btnRemoveDarkFrames")->setEnabled(false);
      };
    }

    /// Allows the user to remove a flat frame from the list.
    /// @throws None.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventBtnRemoveFlatFramesClicked(bool)
    {
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetFlatFrames");
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;

        // Give the user a warning!

      QMessageBox msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("The selected files will be removed from the target list. Are you sure you want to continue?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        QList<QListWidgetItem *> toRemove = listImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;
          flatImageList.removeOne(lwi->data(ROLE_PATH).toString()); // Remove from the master list.
          delete lwi;
          lwi = NULL;
        };
        widget()->findChild<QPushButton *>("btnRemoveFlatFrames")->setEnabled(false);
      };
    }

    /// Allows the user to select the name to save the master flat as.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventBtnSaveDarkAsClicked(bool)
    {
      std::string myPath;
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Master Flat Dark Frame as..."),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        darkSaveAs = fileName;
        widget()->findChild<QLineEdit *>("lineEditDarkSaveAs")->setText(darkSaveAs);

        filePath = darkSaveAs.toStdString();

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    }

    /// Allows the user to select the name of a previously saved master dark frame.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventBtnSelectMasterDarkNameClicked(bool)
    {
      std::string myPath;
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getOpenFileName(this, tr("Open Flat-Master-Dark Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        darkFileName = fileName;
        widget()->findChild<QLineEdit *>("lineEditFlatDarkName")->setText(darkFileName);

        filePath = darkFileName.toStdString();

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    }

    /// Changes the remove frames button enabled status
    //
    // 2015-01-04/GGB - qFindChild is deprecated, change to findChild(...)
    // 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventFlatListSelectionChanged()
    {
      widget()->findChild<QPushButton *>("btnRemoveFlatFrames")->setEnabled(true);
    }

    // Manages the check boxes for the group boxes.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventGroupBoxDarkFramesClicked(bool checked)
    {
      if (checked)
      {
        widget()->findChild<QGroupBox *>("groupBoxMasterDark")->setChecked(false);
      }
      else
      {
        widget()->findChild<QGroupBox *>("groupBoxMasterDark")->setChecked(true);
      }
    }

    // Responds to the Master Dark group box being checked.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventGroupBoxMasterDarkClicked(bool checked)
    {
      if (checked)
      {
        widget()->findChild<QGroupBox *>("groupBoxDarkFrames")->setChecked(false);
      }
      else
      {
        widget()->findChild<QGroupBox *>("groupBoxDarkFrames")->setChecked(true);
      }
    }

    // Responds to the user clicking the button to add flat frames.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventSelectFlatFramesClicked(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      std::string myPath;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetFlatFrames");

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Open Images"),
                    settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(0)).toString(),
                    tr("Image Files (*.fts)"));

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(flatImageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          filePath = (*constIterator).toStdString();

          if ( firstPass )
          {
              // Get the directory details from the file name.

            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
            firstPass = false;
          };

          flatImageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listImages->addItem(lwi);
          lwi = NULL;
        };
    }

    /// Allows the user to select the master flat save as name.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::eventSelectMasterFlatNameClicked(bool)
    {
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Master Flat Frame as..."),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        flatFileName = fileName;
        widget()->findChild<QLineEdit *>("leMasterFlatName")->setText(flatFileName);

        filePath = flatFileName.toStdString();

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    }

    // Sets up the user interface and all the connections.
    // Settings are also used to setup the check box states.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2015-01-04/GGB - Change macro constants to C++ contants. Update some instances of qFindChild to findChild(...)
    /// @version 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::setupUI()
    {
      QUiLoader loader;
      bool b1, b2, b3, b4;

        // Create the window details from the template

      QFile file(":/forms/windowCreateMasterFlatFrame.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      setWindowTitle(tr("Create Master Flat Frame"));

        // Setup the check box state using the stored settings.

      formWidget->findChild<QGroupBox *>("groupBoxFlatDark")->
        setChecked(b1 = settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEFLAT_USEDARK, QVariant(false)).toBool());
      formWidget->findChild<QGroupBox *>("groupBoxMasterDark")->
        setChecked(b2 = settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEFLAT_MASTERDARK, QVariant(false)).toBool());
      formWidget->findChild<QGroupBox *>("groupBoxDarkFrames")->
        setChecked(b3 = settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEFLAT_DARKSELECTION, QVariant(false)).toBool());
      formWidget->findChild<QGroupBox *>("groupBoxMasterDarkSaveAs")->
        setChecked(b4 = settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_CREATEFLAT_DARKSAVEAS, QVariant(false)).toBool());

      if ( !b1 && !b2 && !b3 &&! b4)
        formWidget->findChild<QGroupBox *>("groupBoxMasterDark")->setChecked(true);

        // Create the connections

      connect(formWidget->findChild<QPushButton *>("btnAddFlatFrames"),
        SIGNAL(clicked(bool)), this, SLOT(eventSelectFlatFramesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnRemoveFlatFrames"),
        SIGNAL(clicked(bool)), this, SLOT(eventBtnRemoveFlatFramesClicked(bool)));
      //connect(qFindChild<QPushButton *>(formWidget, "btnMasterBiasFrame"), SIGNAL(clicked(bool)), this, SLOT(eventSelectBiasFrameClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnSaveAs"),
        SIGNAL(clicked(bool)), this, SLOT(eventSelectMasterFlatNameClicked(bool)));

      connect(formWidget->findChild<QPushButton *>("btnCreate"),
        SIGNAL(clicked(bool)), this, SLOT(eventBtnCreateClicked(bool)));

      connect(formWidget->findChild<QPushButton *>("btnSelectMasterDarkName"),
        SIGNAL(clicked(bool)), this, SLOT(eventBtnSelectMasterDarkNameClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnAddDarkFrames"),
        SIGNAL(clicked(bool)), this, SLOT(eventBtnAddDarkFramesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnRemoveDarkFrames"),
        SIGNAL(clicked()), this, SLOT(eventBtnRemoveDarkFramesClicked(bool)));

      connect(formWidget->findChild<QGroupBox *>("groupBoxMasterDark"),
        SIGNAL(clicked(bool)), this, SLOT(eventGroupBoxMasterDarkClicked(bool)));
      connect(formWidget->findChild<QGroupBox *>("groupBoxDarkFrames"),
        SIGNAL(clicked(bool)), this, SLOT(eventGroupBoxDarkFramesClicked(bool)));

      connect(formWidget->findChild<QListWidget *>("listWidgetFlatFrames"),
        SIGNAL(itemSelectionChanged()), this, SLOT(eventFlatListSelectionChanged()));
    }

    // Procedure called when the window is activating.
    // Actions are enabled/disabled/hidden etc by this procedure.
    //
    // 2011-05-15/GGB - Function created.

    void CCreateMasterFlatWindow::windowActivating()
    {
      mdiframe::CFrameWindow *pw;

      pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(false);

        // Calibration menu

      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);

    };

    //*****************************************************************************************************************************
    //
    // CCreateMasterBiasWindow
    //
    //****************************************************************************************************************************

    /// Constructor for the class.
    //
    // 2011-05-22/GGB - Function created.

    CCreateMasterBiasWindow::CCreateMasterBiasWindow(QWidget *aParent) : CWindowCalibration(aParent)
    {
      connect(this, SIGNAL(aboutToActivate()), this, SLOT(windowActivating()));

      setupUI();
    }

    /// @brief Procedure to create the master bias frame.
    /// @throws CCodeError
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::eventBtnCreateClicked(bool)
    {
      QMessageBox msgBox;
      bool bExit = false;
      QStringList::iterator iter;
      ACL::CMasterBiasFrame *masterBiasFrame;

        // Check that there are a number of bias frames selected.

      if (biasImageList.size() < 2)
      {
        msgBox.setText(tr("Not enough Bias Frames Selected."));
        msgBox.setInformativeText(tr("You must select two or more bias frames for combination into a master bias frame."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if (masterBiasFileName.isNull())
      {
        msgBox.setText(tr("Master Bias Name not set."));
        msgBox.setInformativeText(tr("A name for the Master Bias to be saved as must be set."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      };

      if (!bExit)
      {
          // All the basic requirements have been met. Filenames are selected as required. Additional checks will
          // be carried out by the libAstroClass function.

          // Create the master bias frame object.

        masterBiasFrame = new ACL::CMasterBiasFrame(std::string(masterBiasFileName.toStdString()));

          // Now add the bias frame file names

        for (iter = biasImageList.begin(); iter != biasImageList.end(); iter++)
        {
          masterBiasFrame->addBiasFrame( std::string((*iter).toStdString()) );
        }

          // Call the CMasterBiasFrame class to verify all the dark frames are compatible.

        try
        {
          masterBiasFrame->verifyFrames();
        }
        catch (GCL::CError &error)
        {
          if (error.library() == "ACL")
          {
            switch (error.errorCode())
            {
              case 0x2000:    // image array sizes
              {
                msgBox.setText(tr("Image Array sizes are inconsistent."));
                msgBox.setInformativeText(tr("All the image arrays must have the same number of axes (NAXIS)."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              case 0x2001:    // image sizes
              {
                msgBox.setText(tr("Image sizes are inconsistent."));
                msgBox.setInformativeText(tr("All the image must have the same sizes (NAXIS1, NAXIS2, NAXIS3)."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              case 0x2002:    // exposure times
              {
                msgBox.setText(tr("Image exposures are inconsistent."));
                msgBox.setInformativeText(tr("All the images must have the same exposure times."));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
                break;
              };
              default:
              {
                CODE_ERROR(astroManager);
              };
            };  // switch
          };  // if
        };  // catch

        masterBiasFrame->createMasterFrame(ACL::EIC_MEAN);
//        masterBiasFrame->save();

        msgBox.setText(tr("Master Bias Frame Created and Saved."));
        msgBox.setInformativeText(tr("The Master Bias Frame has been created using the bias frames."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
      };
    }

    // Enable the remove items button.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::eventBiasListSelectionChanged()
    {
      widget()->findChild<QPushButton *>("btnRemoveImages")->setEnabled(true);
    }

    // Procudere to remove images from the list.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::eventRemoveBiasFramesClicked(bool)
    {
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetBiasFrames");
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;

        // Give the user a warning!

      QMessageBox msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("The selected files will be removed from the target list. Are you sure you want to continue?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        QList<QListWidgetItem *> toRemove = listImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;
          biasImageList.removeOne(lwi->data(ROLE_PATH).toString()); // Remove from the master list.
          delete lwi;
          lwi = NULL;
        };
        widget()->findChild<QPushButton *>("btnRemoveImages")->setEnabled(false);
      };
    }

    /// @brief Allows the user to select the master bias save as name.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::eventSaveMasterBiasAs(bool)
    {
      boost::filesystem::path filePath;

      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Master Bias Frame as..."),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        masterBiasFileName = fileName;
        widget()->findChild<QLineEdit *>("leMasterBiasName")->setText(masterBiasFileName);

        filePath = masterBiasFileName.toStdString();

        settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
      };
    }

    // Called when the user wishes to add frames to the list of bias frames
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::eventSelectBiasFramesClicked(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetBiasFrames");

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Select Bias Frames"),
                    settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(0)).toString(),
                    tr("Image Files (*.fts)"));

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(biasImageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          filePath = (*constIterator).toStdString();

          if ( firstPass )
          {	// Get the directory details from the file name.
            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(QString::fromStdString(filePath.parent_path().string())));
            firstPass = false;
          };

          biasImageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listImages->addItem(lwi);
          lwi = nullptr;
        };
    }

    /// Sets up the UI for the create master bias window
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::setupUI()
    {
      QUiLoader loader;

        // Create the window details from the template

      QFile file(":/forms/windowCreateMasterBiasFrame.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      setWindowTitle(tr("Create Master Bias Frame"));

      connect(formWidget->findChild<QPushButton *>("btnSelectBiasFrames"), SIGNAL(clicked(bool)),
        this, SLOT(eventSelectBiasFramesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("btnRemoveImages"), SIGNAL(clicked(bool)),
        this, SLOT(eventRemoveBiasFramesClicked(bool)));
      connect(formWidget->findChild<QListWidget *>("listWidgetBiasFrames"), SIGNAL(itemSelectionChanged()),
        this, SLOT(eventBiasListSelectionChanged()));

      connect(formWidget->findChild<QPushButton *>("btnSaveAs"), SIGNAL(clicked(bool)),
        this, SLOT(eventSaveMasterBiasAs(bool)));

      connect(formWidget->findChild<QPushButton *>("btnCreate"), SIGNAL(clicked(bool)),
        this, SLOT(eventBtnCreateClicked(bool)));
    }

    /// Procedure called when the window is activating.
    /// Actions are enabled/disabled/hidden etc by this procedure.
    //
    // 2011-05-22/GGB - Function created.

    void CCreateMasterBiasWindow::windowActivating()
    {
      mdiframe::CFrameWindow *pw;

      pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        // File Menu

      pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(false);

        // Calibration Menu

      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(true);

        // Transform Menu

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);
    }

    //*******************************************************************************************************************************
    //
    // CImageCalibrationMultipleWindow
    //
    //*******************************************************************************************************************************

    // Class Constructor
    //
    // 2011-05-23/GGB - Function created.

    CImageCalibrationMultipleWindow::CImageCalibrationMultipleWindow(QWidget *aParent)
      : CWindowCalibration(aParent), imagesList()
    {
      setupUI();
    }

    // Called when the window closes. Must call the parent object to close with the MDI frame correctly.
    //
    // 2011-05-23/GGB - Function created.

    void CImageCalibrationMultipleWindow::closeEvent(QCloseEvent *event)
    {
      CMdiSubWindow::closeEvent(event);
    }

    // Allows the user to specify the name of the master bias frame.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-24/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventBiasFrameName(bool)
    {
      QString fileName = QFileDialog::getOpenFileName(this, tr("Select Master Bias Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        masterBiasFrame = fileName;
        widget()->findChild<QLineEdit *>("lineEditMasterBiasFrame")->setText(masterBiasFrame);
      };
    }

    // Procedure to calibrate the images.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-25/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventCalibrateImages(bool)
    {
      bool bOverwrite = false, bBackup = false, bSaveOriginal = false;
      bool bUseDark = false, bUseFlat = false, bUseBias = false;
      bool bExit = false;
      QMessageBox msgBox;

        // Get the state of the checks and checkboxes.

      bOverwrite = widget()->findChild<QCheckBox *>("checkBoxOverwrite")->checkState();
      bBackup = widget()->findChild<QCheckBox *>("checkBoxBackup")->checkState();
      bSaveOriginal = widget()->findChild<QCheckBox *>("checkBoxSaveOriginal")->checkState();

      bUseDark = widget()->findChild<QGroupBox *>("groupBoxUseDark")->isChecked();
      bUseFlat = widget()->findChild<QGroupBox *>("groupBoxUseFlat")->isChecked();
      bUseBias = widget()->findChild<QGroupBox *>("groupBoxUseBias")->isChecked();

        // Now check that we have valid data

      if (imagesList.size() < 1)
      {
        msgBox.setText(tr("No Images to calibrate."));
        msgBox.setInformativeText(tr("At Least one image must be selected to calibrate."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bExit = true;
      }
      else if ( bUseDark && masterDarkFrame.isNull() )
      {
        msgBox.setText(tr("Dark frame not selected."));
        msgBox.setInformativeText(tr("A dark frame has been indicated as required, but no dark frame has been supplied. \
                                     Continue without using a dark frame?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
          bUseDark = bUseBias = false;
        else
          bExit = false;
      }
      else if ( bUseBias && masterBiasFrame.isNull() )
      {
        msgBox.setText(tr("Bias frame not selected."));
        msgBox.setInformativeText(tr("A bias frame has been indicated as required, but no bias frame has been supplied. \
                                     Continue without using a bias frame?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
          bUseBias = false;
        else
          bExit = false;
      }
      else if ( bUseFlat && masterFlatFrame.isNull() )
      {
        msgBox.setText(tr("Flat frame not selected."));
        msgBox.setInformativeText(tr("A flat frame has been indicated as required, but no flat frame has been supplied. \
                                     Continue without using a flat frame?"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
          bUseFlat = false;
        else
          bExit = false;
      };

      if (!bExit)   // No need to exit yet.
      {

      };
    }

    // Allows the user to select a master dark frame.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-23/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventDarkFrameName(bool)
    {
      QString fileName = QFileDialog::getOpenFileName(this, tr("Select Master Dark Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        masterDarkFrame = fileName;
        widget()->findChild<QLineEdit *>("lineEditMasterDarkFrame")->setText(masterDarkFrame);
      };
    }

    // Allows the user to select the name of the master flat frame.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-24/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventFlatFrameName(bool)
    {
      QString fileName = QFileDialog::getOpenFileName(this, tr("Select Master Flat Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERFLAT_DIRECTORY, QVariant(0)).toString(),
        tr("Image Files (*.fts)"));

      if ( !fileName.isNull() )
      {
        masterFlatFrame = fileName;
        widget()->findChild<QLineEdit *>("lineEditMasterFlatFrame")->setText(masterFlatFrame);
      };
    }

    // Responds to the clicked message from the select files push button.
    // Calls the select files dialog and allows multiple items to be selected.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2010-12-08/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventSelectFilesClicked(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetCalibrationFiles");

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(this, tr("Select Images"),
                    settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_RAWIMAGESDIRECTORY, QVariant(0)).toString(),
                    tr("Image Files (*.fts)"));

        // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(imagesList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          filePath = (*constIterator).toStdString();

          if ( firstPass )
          {	// Get the directory details from the file name.
            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_RAWIMAGESDIRECTORY,
                                             QVariant(filePath.parent_path().string().c_str()));
            firstPass = false;
          };

          imagesList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listImages->addItem(lwi);
          lwi = NULL;
        };
    }

    // Called when the user selects an item in the list.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-23/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventListSelectionChanged()
    {
      widget()->findChild<QPushButton *>("buttonRemoveFiles")->setEnabled(true);
    }

    // Removes the selected images from the list.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-23/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventRemoveFilesClicked(bool)
    {
      QListWidget *listImages = widget()->findChild<QListWidget *>("listWidgetCalibrationFiles");
      QList<QListWidgetItem *>::iterator iter;
      QListWidgetItem *lwi;

        // Give the user a warning!

      QMessageBox msgBox;
      msgBox.setText(tr("File Removal."));
      msgBox.setInformativeText(tr("All the selected files will be removed from the target list. Are you sure \
                                   you want to continue?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if ( msgBox.exec() == QMessageBox::Yes )
      {
        QList<QListWidgetItem *> toRemove = listImages->selectedItems();
        for (iter = toRemove.begin(); iter != toRemove.end(); ++iter)
        {
          lwi = *iter;
          imagesList.removeOne(lwi->data(ROLE_PATH).toString()); // Remove from the master list.
          delete lwi;
          lwi = NULL;
        };
        widget()->findChild<QPushButton *>("buttonRemoveFiles")->setEnabled(false);
      };
    }

    // Allows the user to select the directory to save the files to.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-24/GGB - Function created.

    void CImageCalibrationMultipleWindow::eventSaveDirectory(bool)
    {
      QString fileName = QFileDialog::getExistingDirectory(this, tr("Select Directory to Save to"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_PROCESSEDDIRECTORY, QVariant(QString("/"))).toString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);

      if ( !fileName.isNull() )
      {
        saveDirectory = fileName;
        widget()->findChild<QLineEdit *>("lineEditSaveDirectory")->setText(saveDirectory);
      };
    }

    /// Sets up the UI by loading from file and getting all the window addresses.
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-05-24/GGB - Function created.

    void CImageCalibrationMultipleWindow::setupUI()
    {
      QUiLoader loader;

        // Create the window details from the template

      QFile file(":/forms/windowCalibrateMultiple.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to load resource :/forms/windowCalibrateMultiple.ui.");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      setWindowTitle(tr("Calibrate Multiple Images"));

      connect(formWidget->findChild<QPushButton *>("btnSelectFiles"),
        SIGNAL(clicked(bool)), this, SLOT(eventSelectFilesClicked(bool)));
      connect(formWidget->findChild<QPushButton *>("buttonRemoveFiles"),
        SIGNAL(clicked(bool)), this, SLOT(eventRemoveFilesClicked(bool)));
      connect(formWidget->findChild<QListWidget *>("listWidgetCalibrationFiles"),
        SIGNAL(itemSelectionChanged()), this, SLOT(eventListSelectionChanged()));

      connect(formWidget->findChild<QPushButton *>("buttonDarkFrameName"),
        SIGNAL(clicked(bool)), this, SLOT(eventDarkFrameName(bool)));
      connect(formWidget->findChild<QPushButton *>("buttonBiasFrameName"),
        SIGNAL(clicked(bool)), this, SLOT(eventBiasFrameName(bool)));
      connect(formWidget->findChild<QPushButton *>("buttonFlatFrameName"),
        SIGNAL(clicked(bool)), this, SLOT(eventFlatFrameName(bool)));

      connect(formWidget->findChild<QPushButton *>("buttonSaveDirectory"),
        SIGNAL(clicked(bool)), this, SLOT(eventSaveDirectory(bool)));

      connect(formWidget->findChild<QPushButton *>("btnCalibrateImages"),
        SIGNAL(clicked(bool)), this, SLOT(eventCalibrateImages(bool)));
    }

    /// @brief Procedure called when the window is activating.
    /// @details Actions are enabled/disabled/hidden etc by this procedure.
    //
    // 2013-04-26/GGB - Support for IDA_CALIBRATE_SINGLEIMAGE
    // 2011-05-15/GGB - Function created.

    void CImageCalibrationMultipleWindow::windowActivating()
    {
      mdiframe::CFrameWindow *pw;

      pw = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

      pw->getAction(mdiframe::IDA_FILE_SAVE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_FILE_SAVEAS)->setEnabled(false);

        // Process Menu

      pw->getAction(mdiframe::IDA_CALIBRATE_SINGLEIMAGE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_CALIBRATE_MULTIIMAGES)->setEnabled(false);

      pw->getAction(mdiframe::IDA_TRANSFORM_FLIP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_RESAMPLE)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_BINPIXELS)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_FLOAT)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_CROP)->setEnabled(false);
      pw->getAction(mdiframe::IDA_TRANSFORM_ROTATE)->setEnabled(false);
    }

  }  // namespace imageCalibration
}  // namespace AstroManager
