//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogConfigureTelescope
// SUBSYSTEM:						Dialogs classes for the application
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
// OVERVIEW:            Dialog for entering/changing/registering data for a telescope
//
// CLASSES INCLUDED:    CDialogConfigureTelescope
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogConfigureTelescope
//
// HISTORY:             2017-08-05 GGB - Class moved into seperate file from configure.h(cpp)
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogConfigureTelescope.h"

namespace AstroManager
{
  namespace dialogs
  {
    /// @brief Class constructor
    /// @param[in] telescope - Pointer to an AstroManager::CTelescope instance.
    /// @throws std::bad_alloc
    /// @version 2017-08-05/GGB - Updated to use CTelescope.
    /// @version 2011-06-25/GGB - Function created.

    CDialogConfigureTelescope::CDialogConfigureTelescope(CTelescope *telescope) : CDialog(":/forms/dialogConfigureTelescope.ui"),
      telescope_(telescope)
    {
      setupUI();
    }

    /// @brief Handles the OK button press and saves all the values.
    /// @throws None.
    /// @version 2017-08-05/GGB - Updated to use CTelescope.
    /// @version 2011-06-25/GGB - Function created.

    void CDialogConfigureTelescope::btnOk(bool)
    {
      if (lineEditShortText->text().isEmpty())
      {
        dlg->reject();
      }
      else
      {
        telescope_->telescopeName() = lineEditShortText->text().toStdString();
        telescope_->manufacturer() = lineEditManufacturer->text().toStdString();
        telescope_->model() = lineEditModel->text().toStdString();
        telescope_->aperture() = spinBoxAperture->value();
        telescope_->focalLength() = doubleSpinBoxFocalLength->value();
        telescope_->obstruction() = doubleSpinBoxObstruction->value();

        dlg->accept();
      };
    }

    /// @brief Sets up the user interface as required.
    /// @throws GCL::CRuntimeError()
    /// @version 2017-08-05/GGB - Updated to use CTelescope.
    /// @version 2017-06-15/GGB - Function created
    /// @version 2011-06-25/GGB - Function created.

    void CDialogConfigureTelescope::setupUI()
    {
        // Get all the addressed of the child objects

      lineEditShortText = findChild<QLineEdit *>("lineEditShortText");
      lineEditManufacturer = findChild<QLineEdit *>("lineEditManufacturer");
      lineEditModel = findChild<QLineEdit *>("lineEditModel");
      spinBoxAperture = findChild<QSpinBox *>("spinBoxAperture");
      doubleSpinBoxFocalLength = findChild<QDoubleSpinBox *>("doubleSpinBoxFocalLength");
      doubleSpinBoxObstruction = findChild<QDoubleSpinBox *>("doubleSpinBoxObstruction");

        // Set all the values.

      lineEditShortText->setText(QString::fromStdString(telescope_->telescopeName()));
      lineEditManufacturer->setText(QString::fromStdString(telescope_->manufacturer()));
      lineEditModel->setText(QString::fromStdString(telescope_->model()));
      spinBoxAperture->setValue(telescope_->aperture());
      doubleSpinBoxFocalLength->setValue(telescope_->focalLength());
      doubleSpinBoxObstruction->setValue(telescope_->obstruction());

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));
    }
  } // namespace dialogs
} // namespace AstroManager
