//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                DialogExportAsJPEG
// SUBSYSTEM:						Dialog to allow the user to select some JPEG save parameters when exporting as JPEG.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2016 Gavin Blakeman.
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
// OVERVIEW:						Implements a dialog class to allow the user to select some JPEG parameters when an image is being exported
//                      as JPEG.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-07-21 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogExportAsJPEG.h"

  // AIRDAS include files

#include "../../Include/Error.h"
#include "../../Include/Settings.h"

namespace AstroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CDialogExportAsJPEG
    //
    //*****************************************************************************************************************************

    /// Class constructor
    //
    // 2013-07-21/GGB - Function created.

    CDialogExportAsJPEG::CDialogExportAsJPEG(ACL::AXIS_t &width, ACL::AXIS_t &height, int &quality) :
      CDialog(":/forms/dialogExportAsJPEG.ui"), bInternalCall(false), imageWidth(width), imageHeight(height), imageQuality(quality)
    {
      aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);
      setupUI();
    }

    // Ensures that the aspect ratio is maintained if the button is clicked.
    // Change the height to match the width.
    //
    // 2013-07-21/GGB - Function created.

    void CDialogExportAsJPEG::eventAspectRatioClicked(bool)
    {
      long width, height;
      double actualAspect;
      long newHeight;

      width = spinBoxImageWidth->value();
      height = spinBoxImageHeight->value();

      actualAspect =(double)  width / (double) height;

      if (actualAspect != aspectRatio)
      {
        newHeight = (long) (width / aspectRatio);
        bInternalCall = true;
        if (newHeight < imageHeight)
          spinBoxImageHeight->setValue(imageHeight);
        else
          spinBoxImageHeight->setValue(newHeight);
        bInternalCall = false;
      };
    }

    // Manages the spin box value changing to maintain the aspect ratio if required.
    //
    // 2013-07-21/GGB - Function created.

    void CDialogExportAsJPEG::eventHeightValueChanged(int height)
    {
      long newWidth;

      if (!bInternalCall)
      {
        if (checkBoxAspectRatio->isChecked())
        {
          newWidth = (long) (height * aspectRatio);
          bInternalCall = true;
          if (newWidth > imageWidth)
            spinBoxImageWidth->setValue(imageWidth);
          else
            spinBoxImageWidth->setValue(newWidth);

          bInternalCall = false;
        };
      };
    }

    // Manages the spin box value changing to maintain the aspect ratio if required.
    //
    // 2013-07-21/GGB - Function created.

    void CDialogExportAsJPEG::eventWidthValueChanged(int width)
    {
      long newHeight;

      if (!bInternalCall)
      {
        if (checkBoxAspectRatio->isChecked())
        {
          newHeight = (long) (width * aspectRatio);
          bInternalCall = true;
          if (newHeight > imageHeight)
            spinBoxImageHeight->setValue(imageHeight);
          else
            spinBoxImageHeight->setValue(newHeight);

          bInternalCall = false;
        };
      };
    }

    /// Processes the save button.
    //
    // 2013-07-21/GGB - Function created.

    void CDialogExportAsJPEG::save()
    {
      settings::VSOPSettings->setValue(settings::DIALOG_EXPORTASJPEG_QUALITY, QVariant(spinBoxImageQuality->value()));

      imageWidth = spinBoxImageWidth->value();
      imageHeight = spinBoxImageHeight->value();
      imageQuality = spinBoxImageQuality->value();

      dlg->accept();
    }

    /// Sets up the user interface.
    //
    // 2013-07-21/GGB - Function created.

    void CDialogExportAsJPEG::setupUI()
    {
      checkBoxAspectRatio = dlg->findChild<QCheckBox *>("checkBoxAspectRatio");
      spinBoxImageWidth = dlg->findChild<QSpinBox *>("spinBoxImageWidth");
      spinBoxImageHeight = dlg->findChild<QSpinBox *>("spinBoxImageHeight");
      spinBoxImageQuality = dlg->findChild<QSpinBox *>("spinBoxImageQuality");
      buttonBox = dlg->findChild<QDialogButtonBox *>("buttonBox");

      if (!checkBoxAspectRatio || !spinBoxImageWidth || !spinBoxImageHeight || !spinBoxImageQuality || !buttonBox)
        CODE_ERROR(ACL);

      checkBoxAspectRatio->setChecked(true);

      spinBoxImageWidth->setMaximum(imageWidth);
      spinBoxImageWidth->setValue(imageWidth);

      spinBoxImageHeight->setMaximum(imageHeight);
      spinBoxImageHeight->setValue(imageHeight);

      spinBoxImageQuality->setValue(settings::VSOPSettings->value(settings::DIALOG_EXPORTASJPEG_QUALITY, QVariant(80)).toInt());

      connect(checkBoxAspectRatio, SIGNAL(clicked(bool)), this, SLOT(eventAspectRatioClicked(bool)));
      connect(spinBoxImageWidth, SIGNAL(valueChanged(int)), this, SLOT(eventWidthValueChanged(int)));
      connect(spinBoxImageHeight, SIGNAL(valueChanged(int)), this, SLOT(eventHeightValueChanged(int)));

      connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
      connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));
    }

  } // namespace dialogs
} // namespace AstroManager
