//*********************************************************************************************************************************
//
// PROJECT:							AIRDAS (Astronomical Image Reduction and Data Analysis Software)
// FILE:								CImageResampleDialog
// SUBSYSTEM:						Dialog for entering data for resampling images.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AIRDAS::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2015 Gavin Blakeman.
//                      This file is part of the Open Astronomical Image Reduction and Data Analysis Software (AIRDAS)
//
//                      AIRDAS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      AIRDAS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//                      License for more details.
//
//                      You should have received a copy of the GNU General Public License along with AIRDAS.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CImageResampleDialog      - Allow entry of parameters for resampling an image.
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-27 GGB - Moved code from ImageDisplay
//
//*********************************************************************************************************************************

#include "../Include/CImageResampleDialog.h"
#include "../Include/Error.h"
#include "../Include/Settings.h"

namespace AIRDAS
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CImageResampleDialog
    //
    //*****************************************************************************************************************************

    // Constructor for the class.
    //
    // 2011-06-04/GGB - Function created.

    CImageResampleDialog::CImageResampleDialog(libAstroClass::CAstroImage *newImage) : CDialog(":/forms/dialogResampleImage.ui"), 
      astroImage(newImage), bInternalCall(false), lWidth(0), lHeight(0)
    {
      setupUI();
    }

    // Ensures that the aspect ratio is maintained if the button is clicked.
    // Change the height to match the width.
    //
    // 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventAspectRatioClicked(bool)
    {
      double aspectRatio = astroImage->width() / astroImage->height();
      long width, height;
      double actualAspect;
      long newHeight;

      width = qFindChild<QSpinBox *>(dlg, "spinBoxWidth")->value();
      height = qFindChild<QSpinBox *>(dlg, "spinBoxHeight")->value();

      actualAspect =(double)  width / (double) height;

      if (actualAspect != aspectRatio)
      {
        newHeight = (long) (width / aspectRatio);
        bInternalCall = true;
        if (newHeight < astroImage->height())
          qFindChild<QSpinBox *>(dlg, "spinBoxHeight")->setValue(astroImage->height());
        else
          qFindChild<QSpinBox *>(dlg, "spinBoxHeight")->setValue(newHeight);
        bInternalCall = false;
      };
    };

    // Handles the Continue button click
    //
    // 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventButtonContinue(bool)
    {
      double percentage;

      switch(qFindChild<QTabWidget *>(dlg, "tabWidget")->currentIndex() )
      {
      case 0:
          // Using the percentage.

        percentage = qFindChild<QDoubleSpinBox *>(dlg, "doubleSpinBoxPercentage")->value();
        lWidth = (long) (astroImage->width() * percentage / 100);
        lHeight = (long) (astroImage->height() * percentage / 100);
        break;
      case 1:
        lWidth = qFindChild<QSpinBox *>(dlg, "spinBoxWidth")->value();
        lHeight = qFindChild<QSpinBox *>(dlg, "spinBoxHeight")->value();
        break;
      default:
        CODE_ERROR;   // Should not be able to get here.
        break;
      };

      saveUI();
      dlg->accept();
    };

    // Manages the spin box value changing to maintain the aspect ratio if required.
    //
    // 2013-01-27/GGB - Added the internal call parameter to allow correct use.
    // 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventHeightValueChanged(int height)
    {
      double aspectRatio;
      long newWidth;

      if (!bInternalCall)
      {
        if (qFindChild<QCheckBox *>(dlg, "checkBoxAspectRatio")->isChecked())
        {
          aspectRatio = (double) astroImage->width() / (double) astroImage->height();

          newWidth = (long) (height * aspectRatio);
          bInternalCall = true;
          if (newWidth < astroImage->width())
            qFindChild<QSpinBox *>(dlg, "spinBoxWidth")->setValue(astroImage->width());
          else
            qFindChild<QSpinBox *>(dlg, "spinBoxWidth")->setValue(newWidth);

          bInternalCall = false;
        };
      };
    };

    // Manages the spin box value changing to maintain the aspect ratio if required.
    //
    // 2013-01-27/GGB - Added the internal call parameter to allow correct use.
    // 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventWidthValueChanged(int width)
    {
      double aspectRatio;
      long newHeight;

      if (!bInternalCall)
      {
        if (qFindChild<QCheckBox *>(dlg, "checkBoxAspectRatio")->isChecked())
        {
          aspectRatio = (double) astroImage->height() / (double) astroImage->width();

          newHeight = (long) (width * aspectRatio);
          bInternalCall = true;
          if (newHeight < astroImage->height())
            qFindChild<QSpinBox *>(dlg, "spinBoxHeight")->setValue(astroImage->height());
          else
            qFindChild<QSpinBox *>(dlg, "spinBoxHeight")->setValue(newHeight);

          bInternalCall = false;
        };
      };
    }

    // Saves the relevant UI settings.
    //
    // 2011-06-04/GGB - Function created.

    void CImageResampleDialog::saveUI()
    {
      settings::VSOPSettings->setValue(DIALOG_IMAGERESAMPLE_ASPECT,
        QVariant(qFindChild<QCheckBox *>(dlg, "checkBoxAspectRatio")->isChecked()));
    }

    // Sets up the UI as required.
    //
    // 2011-06-04/GGB - Function created.

    void CImageResampleDialog::setupUI()
    {
      QSpinBox *sb;

      qFindChild<QCheckBox *>(dlg, "checkBoxAspectRatio")->setChecked(settings::VSOPSettings->value(DIALOG_IMAGERESAMPLE_ASPECT,
        QVariant(true)).toBool());

      qFindChild<QLabel *>(dlg, "labelWidth")->setText(QString("%1").arg(astroImage->width()));
      qFindChild<QLabel *>(dlg, "labelHeight")->setText(QString("%1").arg(astroImage->height()));

      sb = qFindChild<QSpinBox *>(dlg, "spinBoxWidth");
      sb->setValue(astroImage->width());
      sb->setMinimum(astroImage->width());
      sb->setMaximum(astroImage->width() * 5);
      connect(sb, SIGNAL(valueChanged(int)), this, SLOT(eventWidthValueChanged(int)));

      sb = qFindChild<QSpinBox *>(dlg, "spinBoxHeight");
      sb->setValue(astroImage->height());
      sb->setMinimum(astroImage->height());
      sb->setMaximum(astroImage->height() * 5);

      connect(sb, SIGNAL(valueChanged(int)), this, SLOT(eventHeightValueChanged(int)));

      connect(qFindChild<QCheckBox *>(dlg, "checkBoxAspectRatio"), SIGNAL(clicked(bool)),
        this, SLOT(eventAspectRatioClicked(bool)));

      connect(qFindChild<QPushButton *>(dlg, "buttonContinue"), SIGNAL(clicked(bool)), this, SLOT(eventButtonContinue(bool)));
      connect(qFindChild<QPushButton *>(dlg, "buttonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCancel(bool)));
    }

  }  // namespace dialogs
}  // namespace AIRDAS
