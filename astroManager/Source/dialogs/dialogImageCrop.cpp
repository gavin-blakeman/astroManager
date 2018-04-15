//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageCrop
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
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
// CLASSES INCLUDED:    CDialogImageCrop -Dialog for setting cropping of images.

//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-02-07 GGB - Moved dialog CDailogImageCrop into seperate file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file
//                      2013-01-20 GGB - Release of astroManager 0000.00
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogImageCrop.h"

namespace AstroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CDialogImageCrop
    //
    //*****************************************************************************************************************************

    // Class constructor
    //
    // 2011-08-14/GGB - Function created.

    CDialogImageCrop::CDialogImageCrop(MCL::TPoint2D<ACL::AXIS_t> &o, MCL::TPoint2D<ACL::AXIS_t> &d)
      : CDialog(":/forms/dialogImageCrop.ui"), origin(o), dims(d)
    {
      setupUI();
    }

    /// @brief Handles the OK button press.
    /// @throws None.
    /// @version 2015-09-21/GGB - (Bug 67) Added code to accept dialog and check extents.
    /// @version 2011-08-14/GGB - Function created.

    void CDialogImageCrop::btnOk(bool)
    {
      MCL::TPoint2D<ACL::AXIS_t> o(spinBoxXO->value(), spinBoxYO->value());
      MCL::TPoint2D<ACL::AXIS_t> d(spinBoxXD->value(), spinBoxYD->value());

      ACL::AXIS_t dimX = o.x() + d.x();
      ACL::AXIS_t dimY = o.y() + d.y();

      if (dimX < dims.x() && dimY < dims.y())
      {
        origin = o;
        dims = d;

        dlg->accept();
      }
      else
      {
        QMessageBox msgBox;

        msgBox.setWindowTitle(tr("Error: Dimensions larger than original image dimensions."));
        msgBox.setText(tr("The dimensions of the requested image are greater than the dimensions of the original image."));
        msgBox.setInformativeText(tr("Please update the origin and dimensions to ensure that the cropped image will fit inside the original image."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
      };
    }

    // If the X Dimension changes and the checkbox for the aspect ratio is selected, then the Y dimanesion must be updated.
    //
    // 2011-08-14/GGB - Function created.

    void CDialogImageCrop::eventXDChanged()
    {
      const float ar = dims.y() / dims.x();

      if (checkBoxAspectRatio->isChecked())
      {
        spinBoxYD->setValue(spinBoxXD->value() * ar);
      };
    }

    // If the Y Dimension changes and the checkbox for the aspect ratio is selected, then the X dimanesion must be updated.
    //
    // 2011-08-14/GGB - Function created.

    void CDialogImageCrop::eventYDChanged()
    {
      const float ar = dims.x() / dims.y();

      if (checkBoxAspectRatio->isChecked())
      {
        spinBoxXD->setValue(spinBoxYD->value() * ar);
      };
    }

    /// @brief Links and sets all the UI elements.
    /// @throws GCL::CRuntimeAssert(astroManager)
    /// @version 2015-09-21/GGB - Updated to use ASSOCIATE_CONTROL() macro and check for null.
    /// @version 2013-09-16/GGB - Changed logic flow to correct bug. (Bug #1219189)
    /// @version 2011-08-15/GGB - Function created.

    void CDialogImageCrop::setupUI()
    {
      ASSOCIATE_CONTROL(spinBoxXO, dlg, "spinBoxXO", QSpinBox);
      ASSOCIATE_CONTROL(spinBoxYO, dlg, "spinBoxYO", QSpinBox);
      ASSOCIATE_CONTROL(spinBoxXD, dlg, "spinBoxXD", QSpinBox);
      ASSOCIATE_CONTROL(spinBoxYD, dlg, "spinBoxYD", QSpinBox);
      ASSOCIATE_CONTROL(checkBoxAspectRatio, dlg, "checkBoxAspectRatio", QCheckBox);

      spinBoxXO->setMaximum(dims.x());
      spinBoxYO->setMaximum(dims.y());
      spinBoxXD->setMaximum(dims.x());
      spinBoxYD->setMaximum(dims.y());

      spinBoxXO->setValue(origin.x());
      spinBoxYO->setValue(origin.y());
      spinBoxXD->setValue(dims.x());
      spinBoxYD->setValue(dims.y());

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));
    }

  } // namespace dialogs

} // namespace AstroManager
