//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageFloat
// SUBSYSTEM:						Dialog Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman.
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
// CLASSES INCLUDED:    CAstroImageWindow         - Window for displaying and manipulating astronomical images.
//                      CImageFloatDialog         - Allow entry of parameters for floating an image.
//                      CBinPixelsDialog          - Allows selection of binning size for images.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//                          - CAstroImageWindow
//                      QDialog
//                        - CDialog
//                          - CDialogImageFloat
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-02-07 GGB - Moved CDialogImageCrop, CBinPixelsDialog, CImageFloatDialog into it's own file.
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogImageFloat.h"

#include "../../Include/Error.h"
#include "../../Include/Settings.h"

#include <GCL>

namespace AstroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CImageFloatDialog
    //
    //*****************************************************************************************************************************

#define IMAGE_MIN     1
#define IMAGE_MAX     2
#define IMAGE_MEAN    3
#define IMAGE_MEDIAN  4
#define MANUAL        5

    /// @brief Class constructor
    //
    // 2011-06-04/GGB - Function created.

    CDialogImageFloat::CDialogImageFloat(ACL::CAstroImage *newImage) : CDialog(":/forms/dialogFloatImage.ui"), astroImage(newImage)
    {
      setupUI();
    }

    /// @brief Manages the user pressing the continue button.
    /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
    /// @version 2011-06-04/GGB - Function created.

    void CDialogImageFloat::eventButtonContinue(bool)
    {
      lHeight = findChild<QSpinBox *>("spinBoxHeight")->value();
      lWidth = findChild<QSpinBox *>("spinBoxWidth")->value();

      if (findChild<QRadioButton *>("radioButtonMinimum")->isChecked())
      {
        dBackgroundValue = astroImage->getMin();
      }
      else if (findChild<QRadioButton *>("radioButtonMaximum")->isChecked())
      {
        dBackgroundValue = astroImage->getMax();
      }
      else if (findChild<QRadioButton *>("radioButtonMean")->isChecked())
      {
        dBackgroundValue = astroImage->getMean();
      }
      else if (findChild<QRadioButton *>("radioButtonMedian")->isChecked())
      {
        dBackgroundValue = astroImage->getMedian();
      }
      else if (findChild<QRadioButton *>("radioButtonManual")->isChecked())
      {
        dBackgroundValue = findChild<QSpinBox *>("spinBoxManual")->value();
      }
      else
      {
        CODE_ERROR(astroManager);
      }

      saveUI();
      dlg->accept();
    }

    /// @brief Ensures the spin box is updated when the Manual option is selected.
    //
    // 2011-06-04/GGB - Function created.

    void CDialogImageFloat::eventManualSelected(bool checked)
    {
      dlg->findChild<QSpinBox *>("spinBoxManual")->setEnabled(checked);
    }

    // Saves the presets for the user interface.
    /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
    /// @version 2011-06-04/GGB - Function created.

    void CDialogImageFloat::saveUI()
    {
      int bgnd;

      if (findChild<QRadioButton *>("radioButtonMinimum")->isChecked())
      {
        bgnd = IMAGE_MIN;
      }
      else if (findChild<QRadioButton *>("radioButtonMaximum")->isChecked())
      {
        bgnd = IMAGE_MAX;
      }
      else if (findChild<QRadioButton *>("radioButtonMean")->isChecked())
      {
        bgnd = IMAGE_MEAN;
      }
      else if (findChild<QRadioButton *>("radioButtonMedian")->isChecked())
      {
        bgnd = IMAGE_MEDIAN;
      }
      else if (findChild<QRadioButton *>("radioButtonManual")->isChecked())
      {
        bgnd = MANUAL;
        settings::astroManagerSettings->setValue(settings::DIALOG_IMAGEFLOAT_MANUAL, QVariant(findChild<QSpinBox *>("spinBoxManual")->value()));
      }
      else
      {
        CODE_ERROR(astroManager);
      }

      settings::astroManagerSettings->setValue(settings::DIALOG_IMAGEFLOAT_BACKGROUND, QVariant(bgnd));
    }

    // Sets up the user interface.
    /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
    /// @version 2011-06-04/GGB - Function created.

    void CDialogImageFloat::setupUI()
    {
      QSpinBox *sbHeight, *sbWidth;
      int bgnd = settings::astroManagerSettings->value(settings::DIALOG_IMAGEFLOAT_BACKGROUND, QVariant(IMAGE_MIN)).toInt();

        // Set the User interface up correctly.

      switch (bgnd)
      {
      case IMAGE_MIN:
        findChild<QRadioButton *>("radioButtonMinimum")->setChecked(true);
        break;
      case IMAGE_MAX:
        findChild<QRadioButton *>("radioButtonMaximum")->setChecked(true);
        break;
      case IMAGE_MEAN:
        findChild<QRadioButton *>("radioButtonMean")->setChecked(true);
        break;
      case IMAGE_MEDIAN:
        findChild<QRadioButton *>("radioButtonMedian")->setChecked(true);
        break;
      case MANUAL:
        findChild<QRadioButton *>("radioButtonManual")->setChecked(true);
        findChild<QSpinBox *>("radioButtonManual")->setEnabled(true);
        findChild<QSpinBox *>("radioButtonManual")->setValue(settings::astroManagerSettings->value(settings::DIALOG_IMAGEFLOAT_MANUAL,
          QVariant(0)).toInt());
        break;
      default:
        CODE_ERROR(astroManager);
        break;
      };

      sbWidth = findChild<QSpinBox *>("spinBoxWidth");
      sbHeight = findChild<QSpinBox *>("spinBoxHeight");

      sbWidth->setValue(astroImage->width());
      sbHeight->setValue(astroImage->height());

      sbWidth->setMinimum(astroImage->width());
      sbHeight->setMinimum(astroImage->height());

      connect(findChild<QRadioButton *>("radioButtonManual"), SIGNAL(clicked(bool)), this, SLOT(eventManualSelected(bool)));

      connect(findChild<QPushButton *>("pushButtonContinue"), SIGNAL(clicked(bool)), this, SLOT(eventButtonContinue(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCancel(bool)));
    }
  }
}
