//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageCalibration.h
// SUBSYSTEM:						Image Calibration Dialog
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2012-2018 Gavin Blakeman.
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
// OVERVIEW:            Dialog for Image Calibration - Allows the user to select dark frame, flat frame etc.
//
// CLASSES INCLUDED:    CImageCalibrationDialog   - A dialog to allow the user to enter the information required to calibrate a
//                                                  frame.
//
// CLASS HIERARCHY:     CDialog
//                        - CImageCalibrationDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-21 GGB - astroManager 0000.00 release.
//                      2012-01-28 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dialogs/dialogImageCalibration.h"

  // astroManager files

#include "../../include/Error.h"
#include "../../include/Settings.h"
#include "../../include/astroManager.h"

namespace astroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CImageCalibrationDialog
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    //
    // 2011-06-04/GGB - Function created.

    CImageCalibrationDialog::CImageCalibrationDialog(ACL::SCalibrateImage_Ptr ci) :
      CDialog(":/forms/dialogCalibrateSingleImage.ui"), calibrateImage(ci)
    {
      setupUI();
    }

    /// @brief Handles the OK button press.
    /// @details Checks that all the specified information is present and correct. Saves the information for future use.
    /// A check is made that the frames specified are valid.
    //
    // 2011-06-04/GGB - Function created.

    void CImageCalibrationDialog::eventButtonCalibrate(bool)
    {
      bool bError = false;
      QMessageBox msgBox;
      QString darkFrameName, flatFrameName, biasFrameName;

      calibrateImage->useFlatFrame = groupBoxUseFlat->isChecked();
      calibrateImage->useBiasFrame = groupBoxUseBias->isChecked();
      calibrateImage->overrideDarkExposure = checkBoxIgnoreExposure->isChecked();
      calibrateImage->overrideDarkTemperature = checkBoxIgnoreTemperature->isChecked();
      calibrateImage->saveOriginal = checkBoxSaveOriginal->isChecked();
      calibrateImage->appendFrames = checkBoxAppendFrames->isChecked();

      darkFrameName = lineEditMasterDarkFrame->text();
      flatFrameName = lineEditMasterFlatFrame->text();
      biasFrameName = lineEditMasterBiasFrame->text();

      calibrateImage->darkFramePath = boost::filesystem::path(darkFrameName.toStdString());
      calibrateImage->flatFramePath = boost::filesystem::path(flatFrameName.toStdString());
      calibrateImage->biasFramePath = boost::filesystem::path(biasFrameName.toStdString());

      if (darkFrameName.isNull())
      {
        msgBox.setText(tr("No Dark Frame Selected."));
        msgBox.setInformativeText(tr("A file name for a dark frame must be selected."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bError = true;
      }
      else if ( !boost::filesystem::exists(calibrateImage->darkFramePath) )
      {
        msgBox.setText(tr("Dark Frame not a valid file."));
        msgBox.setInformativeText(tr("A valid dark frame must be provided."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        bError = true;
      }
      else
      {
        if (calibrateImage->useBiasFrame && biasFrameName.isNull() )
        {
          msgBox.setText(tr("No Bias Frame Selected."));
          msgBox.setInformativeText(tr("A file name for a bias frame must be selected."));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          bError = true;
        }
        else if (calibrateImage->useBiasFrame && !boost::filesystem::exists(calibrateImage->biasFramePath) )
        {
          msgBox.setText(tr("The Bias Frame name is not a valid file."));
          msgBox.setInformativeText(tr("A valid bias frame must be selected."));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          bError = true;
        };

        if (calibrateImage->useFlatFrame  && flatFrameName.isNull() )
        {
          msgBox.setText(tr("No Flat Frame Selected."));
          msgBox.setInformativeText(tr("A file name for a flat frame must be selected."));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          bError = true;
        }
        else if (calibrateImage->useFlatFrame && !boost::filesystem::exists(calibrateImage->flatFramePath) )
        {
          msgBox.setText(tr("The Flat Frame is not a valid file."));
          msgBox.setInformativeText(tr("A valid flat frame must be selected."));
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.setDefaultButton(QMessageBox::Ok);
          msgBox.exec();
          bError = true;
        };
      };

      if (!bError)
      {
        saveUI();
        dlg->accept();
      };
    }

    /// @brief Allows the user to select the name of the bias frame.
    //
    // 2013-07-14/GGB - Use settings::FILEEXTENTIONS (Bug #1201068)
    // 2011-06-04/GGB - Function created.

    void CImageCalibrationDialog::eventButtonSelectBiasFrame(bool)
    {
      QString fileName = QFileDialog::getOpenFileName(dlg, tr("Select Master Bias Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(0)).toString(),
                                                      EXTENSION_IMAGE);

      if ( !fileName.isNull() )
      {
        lineEditMasterBiasFrame->setText(fileName);
      };
    }

    /// @brief Allows the user to select the name of the dark frame.
    //
    // 2013-07-14/GGB - Use settings::FILEEXTENTIONS (Bug #1201068)
    // 2011-06-04/GGB - Function created.

    void CImageCalibrationDialog::eventButtonSelectDarkFrame(bool)
    {
      QString fileName = QFileDialog::getOpenFileName(dlg, tr("Select Master Dark Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(0)).toString(),
                                                      EXTENSION_IMAGE);

      if ( !fileName.isNull() )
      {
        lineEditMasterDarkFrame->setText(fileName);
      };
    }

    // Allows the user to select the name of the flat frame.
    //
    // 2013-07-14/GGB - Use settings::FILEEXTENTIONS (Bug #1201068)
    // 2011-06-04/GGB - Function created.

    void CImageCalibrationDialog::eventButtonSelectFlatFrame(bool)
    {
      QString fileName = QFileDialog::getOpenFileName(dlg, tr("Select Master Flat Frame"),
        settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(0)).toString(),
                                                      EXTENSION_IMAGE);

      if ( !fileName.isNull() )
      {
        lineEditMasterFlatFrame->setText(fileName);
      };
    }

    /// When the user selects that a bias frame is being used, the ignore exposure check can be disabled as it is not needed.
    //
    // 2012-01-28/GGB - Function created.

    void CImageCalibrationDialog::eventGroupSelectBiasFrame(bool checked)
    {
      checkBoxIgnoreExposure->setEnabled(!checked);
    }

    /// Saves all the UI settings
    //
    // 2015-01-04/GGB - Changed Macro constants to C++ constants.
    // 2011-06-04/GGB - Function created.

    void CImageCalibrationDialog::saveUI()
    {
      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_MASTERDARK,
                                       QVariant(calibrateImage->darkFramePath.string().c_str()));
      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_MASTERBIAS,
                                       QVariant(calibrateImage->biasFramePath.string().c_str()));
      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_MASTERFLAT,
                                       QVariant(calibrateImage->flatFramePath.string().c_str()));

      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_USEBIAS, QVariant(groupBoxUseBias->isChecked()));

      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_USEFLAT,
                                       QVariant(groupBoxUseFlat->isChecked()));

      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_IGNORETEMPERATURE,
                                       QVariant(checkBoxIgnoreTemperature->isChecked()));
      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_IGNOREEXPOSURE,
                                       QVariant(checkBoxIgnoreExposure->isChecked()));

      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_SAVEORIGINAL,
                                       QVariant(checkBoxSaveOriginal->isChecked()));
      settings::astroManagerSettings->setValue(settings::DIALOG_SINGLEIMAGECALIBRATION_APPENDFRAMES,
                                       QVariant(checkBoxAppendFrames->isChecked()));
    }

    /// @brief Sets up all the UI elements based on the stored settings.
    /// @throws CRuntimeAssert
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2015-08-01/GGB - Updated to use macro ASSOCIATE_CONTROL
    /// @version 2015-01-04/GGB - Changed Macro constants to C++ constants.
    /// @version 2011-06-04/GGB - Function created.

    void CImageCalibrationDialog::setupUI()
    {
         // Get the addresses of the widgets

      lineEditMasterDarkFrame = findChild<QLineEdit *>("lineEditMasterDarkFrame");
      ASSOCIATE_CONTROL(groupBoxUseFlat, dlg, "groupBoxUseFlat", QGroupBox);
      ASSOCIATE_CONTROL(lineEditMasterFlatFrame, dlg, "lineEditMasterFlatFrame", QLineEdit);

      ASSOCIATE_CONTROL(groupBoxUseBias, dlg, "groupBoxUseBias", QGroupBox);
        ASSOCIATE_CONTROL(lineEditMasterBiasFrame, dlg, "lineEditMasterBiasFrame", QLineEdit);

      ASSOCIATE_CONTROL(checkBoxIgnoreTemperature, dlg, "checkBoxIgnoreTemperature", QCheckBox);
      ASSOCIATE_CONTROL(checkBoxIgnoreExposure, dlg, "checkBoxIgnoreExposure", QCheckBox);
      ASSOCIATE_CONTROL(checkBoxSaveOriginal, dlg, "checkBoxSaveOriginal", QCheckBox);
      ASSOCIATE_CONTROL(checkBoxAppendFrames, dlg, "checkBoxAppendFrames", QCheckBox);

        // Set all the default values into the dialog box.

      lineEditMasterDarkFrame->setText(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_MASTERDARK,
        QVariant("")).toString());
      lineEditMasterBiasFrame->setText(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_MASTERBIAS, QVariant("")).toString());
      lineEditMasterFlatFrame->setText(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_MASTERFLAT, QVariant("")).toString());
      groupBoxUseBias->setChecked(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_USEBIAS, QVariant(false)).toBool());
      groupBoxUseFlat->setChecked(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_USEFLAT, QVariant(false)).toBool());

      checkBoxIgnoreTemperature->setChecked(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_IGNORETEMPERATURE,
                                                                          QVariant(false)).toBool());
      checkBoxIgnoreExposure->setChecked(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_IGNOREEXPOSURE,
                                                                       QVariant(false)).toBool());

      checkBoxSaveOriginal->setChecked(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_SAVEORIGINAL,
                                                                     QVariant(false)).toBool());
      checkBoxAppendFrames->setChecked(settings::astroManagerSettings->value(settings::DIALOG_SINGLEIMAGECALIBRATION_APPENDFRAMES,
                                                                     QVariant(false)).toBool());

      checkBoxIgnoreExposure->setEnabled(!groupBoxUseBias->isChecked());

        // Setup all the slots/signals

      connect(findChild<QPushButton *>("buttonDarkFrameName"), SIGNAL(clicked(bool)), this,
        SLOT(eventButtonSelectDarkFrame(bool)));
      connect(findChild<QPushButton *>("buttonBiasFrameName"), SIGNAL(clicked(bool)), this,
        SLOT(eventButtonSelectBiasFrame(bool)));
      connect(findChild<QPushButton *>("buttonFlatFrameName"), SIGNAL(clicked(bool)), this,
        SLOT(eventButtonSelectFlatFrame(bool)));
      connect(findChild<QPushButton *>("btnCalibrateImages"), SIGNAL(clicked(bool)), this,
        SLOT(eventButtonCalibrate(bool)));
      connect(findChild<QPushButton *>("buttonCancel"), SIGNAL(clicked(bool)), this,
        SLOT(eventButtonCancel(bool)));

      connect(groupBoxUseBias, SIGNAL(clicked(bool)), this, SLOT(eventGroupSelectBiasFrame(bool)));
    }

  }  // namespace dialogs
}  // namespace AstroManager
