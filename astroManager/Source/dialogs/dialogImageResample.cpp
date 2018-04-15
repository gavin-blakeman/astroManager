//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageResample
// SUBSYSTEM:						Dialog for entering data for resampling images.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
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
// CLASSES INCLUDED:    CImageResampleDialog      - Allow entry of parameters for resampling an image.
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-07-21/GGB - Renamed from CImageResampleDialog to DialogImageResample
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-27 GGB - Moved code from ImageDisplay
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogImageResample.h"

  // astroManager header files

#include "../../Include/Error.h"
#include "../../Include/Settings.h"

#include <GCL>

namespace AstroManager
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

    CImageResampleDialog::CImageResampleDialog(ACL::CAstroImage *newImage) : CDialog(":/forms/dialogResampleImage.ui"),
      astroImage(newImage), bInternalCall(false), lWidth(0), lHeight(0)
    {
      setupUI();
    }

    // Ensures that the aspect ratio is maintained if the button is clicked.
    // Change the height to match the width.
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventAspectRatioClicked(bool)
    {
      double aspectRatio = astroImage->width() / astroImage->height();
      long width, height;
      double actualAspect;
      long newHeight;

      width = findChild<QSpinBox *>("spinBoxWidth")->value();
      height = findChild<QSpinBox *>("spinBoxHeight")->value();

      actualAspect = static_cast<double>(width) / static_cast<double>(height);

      if (actualAspect != aspectRatio)
      {
        newHeight = (long) (width / aspectRatio);
        bInternalCall = true;
        if (newHeight < astroImage->height())
        {
          findChild<QSpinBox *>("spinBoxHeight")->setValue(astroImage->height());
        }
        else
        {
          findChild<QSpinBox *>("spinBoxHeight")->setValue(newHeight);
        }
        bInternalCall = false;
      };
    }

    // Handles the Continue button click
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventButtonContinue(bool)
    {
      double percentage;

      switch(findChild<QTabWidget *>("tabWidget")->currentIndex() )
      {
      case 0:
          // Using the percentage.

        percentage = findChild<QDoubleSpinBox *>("doubleSpinBoxPercentage")->value();
        lWidth = (long) (astroImage->width() * percentage / 100);
        lHeight = (long) (astroImage->height() * percentage / 100);
        break;
      case 1:
        lWidth = findChild<QSpinBox *>("spinBoxWidth")->value();
        lHeight = findChild<QSpinBox *>("spinBoxHeight")->value();
        break;
      default:
        CODE_ERROR(astroManager);   // Should not be able to get here.
        break;
      };

      saveUI();
      dlg->accept();
    }

    // Manages the spin box value changing to maintain the aspect ratio if required.
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2015-08-01/GGB - Changed c-style casting to static_cast.
    /// @version 2013-01-27/GGB - Added the internal call parameter to allow correct use.
    /// @version 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventHeightValueChanged(int height)
    {
      double aspectRatio;
      long newWidth;

      if (!bInternalCall)
      {
        if (findChild<QCheckBox *>("checkBoxAspectRatio")->isChecked())
        {
          aspectRatio = static_cast<double>(astroImage->width()) / static_cast<double>(astroImage->height());

          newWidth = static_cast<long>(height * aspectRatio);
          bInternalCall = true;
          if (newWidth < astroImage->width())
          {
            findChild<QSpinBox *>("spinBoxWidth")->setValue(astroImage->width());
          }
          else
          {
            findChild<QSpinBox *>("spinBoxWidth")->setValue(newWidth);
          }

          bInternalCall = false;
        };
      };
    }

    /// @brief Manages the spin box value changing to maintain the aspect ratio if required.
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2015-08-01/GGB - Changed c-style casting to static_cast.
    /// @version 2013-01-27/GGB - Added the internal call parameter to allow correct use.
    /// @version 2011-06-04/GGB - Function created.

    void CImageResampleDialog::eventWidthValueChanged(int width)
    {
      double aspectRatio;
      long newHeight;

      if (!bInternalCall)
      {
        if (findChild<QCheckBox *>("checkBoxAspectRatio")->isChecked())
        {
          aspectRatio = static_cast<double>(astroImage->height()) / static_cast<double>(astroImage->width());

          newHeight = static_cast<long>((width * aspectRatio));
          bInternalCall = true;
          if (newHeight < astroImage->height())
          {
            findChild<QSpinBox *>("spinBoxHeight")->setValue(astroImage->height());
          }
          else
          {
            findChild<QSpinBox *>("spinBoxHeight")->setValue(newHeight);
          }

          bInternalCall = false;
        };
      };
    }

    // Saves the relevant UI settings.
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2011-06-04/GGB - Function created.

    void CImageResampleDialog::saveUI()
    {
      settings::astroManagerSettings->setValue(settings::DIALOG_IMAGERESAMPLE_ASPECT,
        QVariant(findChild<QCheckBox *>("checkBoxAspectRatio")->isChecked()));
    }

    // Sets up the UI as required.
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2011-06-04/GGB - Function created.

    void CImageResampleDialog::setupUI()
    {
      QSpinBox *sb;

      findChild<QCheckBox *>("checkBoxAspectRatio")->
          setChecked(settings::astroManagerSettings->value(settings::DIALOG_IMAGERESAMPLE_ASPECT,
                                                   QVariant(true)).toBool());

      findChild<QLabel *>("labelWidth")->setText(QString("%1").arg(astroImage->width()));
      findChild<QLabel *>("labelHeight")->setText(QString("%1").arg(astroImage->height()));

      sb = dlg->findChild<QSpinBox *>("spinBoxWidth");
      sb->setValue(astroImage->width());
      sb->setMinimum(astroImage->width());
      sb->setMaximum(astroImage->width() * 5);
      connect(sb, SIGNAL(valueChanged(int)), this, SLOT(eventWidthValueChanged(int)));

      sb = findChild<QSpinBox *>("spinBoxHeight");
      sb->setValue(astroImage->height());
      sb->setMinimum(astroImage->height());
      sb->setMaximum(astroImage->height() * 5);

      connect(sb, SIGNAL(valueChanged(int)), this, SLOT(eventHeightValueChanged(int)));

      connect(findChild<QCheckBox *>("checkBoxAspectRatio"), SIGNAL(clicked(bool)),
        this, SLOT(eventAspectRatioClicked(bool)));

      connect(findChild<QPushButton *>("buttonContinue"), SIGNAL(clicked(bool)), this, SLOT(eventButtonContinue(bool)));
      connect(findChild<QPushButton *>("buttonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCancel(bool)));
    }

  }  // namespace dialogs
}  // namespace AstroManager
