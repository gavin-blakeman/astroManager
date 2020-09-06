//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogBinPixels
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imagedisplay
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
// CLASSES INCLUDED:    CBinPixelsDialog          - Allows selection of binning size for images.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//                          - CAstroImageWindow
//                      QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-07 GGB - Moved CCBinPixelsDialog into it's own file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file.
//                      2013-01-20 GGB - Release of astroManager 0000.00
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dialogs/dialogBinPixels.h"
#include "../../include/error.h"
#include "../../include/settings.h"

#include <GCL>

namespace astroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CBinPixelsDialog
    //
    //*****************************************************************************************************************************

    /// @brief Class constructor
    //
    // 2011-06-10/GGB - Function created.

    CBinPixelsDialog::CBinPixelsDialog() : CDialog(":/forms/dialogBinPixels.ui")
    {
      setupUI();
    }

    /// @brief Handles the continue button press.
    /// @version 2017-06-14/GGB - Updated to Qt5
    // 2011-06-10/GGB - Function created.

    void CBinPixelsDialog::eventButtonContinue(bool)
    {
      if (findChild<QRadioButton *>("radioButton2x2")->isChecked())
      {
        nsize = 2;
      }
      else if (findChild<QRadioButton *>("radioButton3x3")->isChecked())
      {
        nsize = 3;
      }
      else
      {
        CODE_ERROR;
      };

      saveUI();

      dlg->accept();
    }

    /// @brief Saves the UI settings
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2011-06-10/GGB - Function created.

    void CBinPixelsDialog::saveUI()
    {
      if (findChild<QRadioButton *>("radioButton2x2")->isChecked())
      {
        settings::astroManagerSettings->value(settings::DIALOG_BINPIXELS_BINSIZE, QVariant(2));
      }
      else if (findChild<QRadioButton *>("radioButton3x3")->isChecked())
      {
        settings::astroManagerSettings->value(settings::DIALOG_BINPIXELS_BINSIZE, QVariant(3));
      }
      else
      {
        CODE_ERROR;
      }
    }

    /// @brief Sets up the UI as required
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2011-06-10/GGB - Function created.

    void CBinPixelsDialog::setupUI()
    {
      switch(settings::astroManagerSettings->value(settings::DIALOG_BINPIXELS_BINSIZE, QVariant(2)).toInt())
      {
      case 2:
        findChild<QRadioButton *>("radioButton2x2")->setChecked(true);
        break;
      case 3:
        findChild<QRadioButton *>("radioButton3x3")->setChecked(true);
        break;
      default:
        CODE_ERROR;
        break;
      };

      connect(findChild<QPushButton *>("pushButtonContinue"), SIGNAL(clicked(bool)), this, SLOT(eventButtonContinue(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCancel(bool)));
    }
  } // namespace dialogs
} // namespace AstroManager
