//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogConfigureSite
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2017 Gavin Blakeman.
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
// OVERVIEW:            Dialog for entering/changing/registering data for an observing site.
//
// CLASSES INCLUDED:    CDialogConfigureSite
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogConfigureSite
//
// HISTORY:             2017-08-04 GGB - Class moved into seperate file from configure.h(cpp)
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogConfigureSite.h"

#include "../../Include/Error.h"
#include "../../Include/VSOP.h"

#include <cmath>

namespace AstroManager
{
  namespace dialogs
  {
    /// @brief Constructor for the class.
    /// @param[in] observatory - Pointer to the observatory class containing the information.
    /// @throws std::bad_alloc
    /// @throws GCL::CRuntimeAssert(AIRDAS)
    /// @version 2017-08-04/GGB - Updated to work with ARID and registerObservingSite()
    /// @version 2011-06-20/GGB - Function created.

    CDialogConfigureSite::CDialogConfigureSite(CObservatory *observatory) : CDialog(":/forms/dialogConfigureSite.ui"),
      observatory_(observatory)
    {
      RUNTIME_ASSERT("AIRDAS", observatory != nullptr, "parameter observatory cannot be nullptr.");
      setupUI();
    }

    /// @brief Handles the button for closing and saving the data.
    /// @throws None.
    /// @version 2017-08-04/GGB - Updated to reflect passing CObservatory instance.
    /// @version 2011-06-21/GGB - Function created.

    void CDialogConfigureSite::btnOk(bool)
    {
      if (lineEditShortText->text().isEmpty())
      {
        dlg->reject();
      }
      else
      {
        observatory_->siteName() = lineEditShortText->text().toStdString();
        observatory_->latitude() = (spinBoxLatitudeD->value() + static_cast<FP_t>(spinBoxLatitudeM->value()) / 60
            + doubleSpinBoxLatitudeS->value() / 3600) * ((comboBoxLatitude->currentIndex() == 0) ? 1 : -1);

        observatory_->longitude() = (spinBoxLongitudeD->value() + static_cast<double>(spinBoxLongitudeM->value()) / 60
                                     + doubleSpinBoxLongitudeS->value() / 3600) * ((comboBoxLongitude->currentIndex() == 0) ? 1 : -1);
        observatory_->altitude() = spinBoxAltitude->value();
        observatory_->timeZone() = spinBoxTimeZone->value();
        observatory_->daylightSaving() = checkBoxDaylightSaving->isChecked();
        observatory_->IAUCode() = lineEditIAUCode->text().toStdString();
        dlg->accept();
      };
    }

    /// @brief Sets up the user interface for the class.
    /// @throws GCL::CRuntimeAssert()
    /// @version 2017-08-04/GGB - Updated to reflect use of CObservatory.
    /// @version 2017-06-15/GGB - Updated to Qt5
    /// @version 2011-06-20/GGB - Function created.

    void CDialogConfigureSite::setupUI()
    {
      double dVal;
      int iVal;

        // Get all the children addresses

      lineEditShortText = findChild<QLineEdit *>("lineEditShortText");
      spinBoxLatitudeD = findChild<QSpinBox *>("spinBoxLatitudeD");
      spinBoxLatitudeM = findChild<QSpinBox *>("spinBoxLatitudeM");
      doubleSpinBoxLatitudeS = findChild<QDoubleSpinBox *>("doubleSpinBoxLatitudeS");
      comboBoxLatitude = findChild<QComboBox *>("comboBoxLatitude");
      spinBoxLongitudeD = findChild<QSpinBox *>("spinBoxLongitudeD");
      spinBoxLongitudeM = findChild<QSpinBox *>("spinBoxLongitudeM");
      doubleSpinBoxLongitudeS = findChild<QDoubleSpinBox *>("doubleSpinBoxLongitudeS");
      comboBoxLongitude = findChild<QComboBox *>("comboBoxLongitude");
      spinBoxAltitude = findChild<QSpinBox *>("spinBoxAltitude");
      spinBoxTimeZone = findChild<QSpinBox *>("spinBoxTimeZone");
      checkBoxDaylightSaving = findChild<QCheckBox *>("checkBoxDaylightSaving");
      lineEditIAUCode = findChild<QLineEdit *>("lineEditIAUCode");

        // Set up the N/S combo box.

      comboBoxLatitude->addItem(QString(tr("N")));
      comboBoxLatitude->addItem(QString(tr("S")));

        // Setup the E/W combo box

      comboBoxLongitude->addItem(QString(tr("E")));
      comboBoxLongitude->addItem(QString(tr("W")));

      lineEditShortText->setText(QString::fromStdString(observatory_->siteName()));

      dVal = observatory_->latitude();

      if (dVal >= 0)
      {
        comboBoxLatitude->setCurrentIndex(0);
      }
      else
      {
        comboBoxLatitude->setCurrentIndex(1);
      }

      dVal = std::abs(dVal);

      iVal = static_cast<int>(std::floor(dVal));
      spinBoxLatitudeD->setValue(iVal);
      dVal -= iVal;
      dVal *= 60;
      iVal = static_cast<int>(std::floor(dVal));
      spinBoxLatitudeM->setValue(iVal);
      dVal -= iVal;
      dVal *= 60;
      doubleSpinBoxLatitudeS->setValue(dVal);

      dVal = observatory_->longitude();

      if (dVal >= 0)
      {
        comboBoxLongitude->setCurrentIndex(0);
       }
      else
      {
        comboBoxLongitude->setCurrentIndex(1);
      };

      dVal = std::abs(dVal);

      iVal = static_cast<int>(std::floor(dVal));
      spinBoxLongitudeD->setValue(iVal);
      dVal -= iVal;
      dVal *= 60;
      iVal = static_cast<int>(std::floor(dVal));
      spinBoxLongitudeM->setValue(iVal);
      dVal -= iVal;
      dVal *= 60;
      doubleSpinBoxLongitudeS->setValue(dVal);

      spinBoxAltitude->setValue(observatory_->altitude());
      spinBoxTimeZone->setValue(observatory_->timeZone());

      if (observatory_->daylightSaving())
      {
        checkBoxDaylightSaving->setChecked(false);
      }
      else
      {
        checkBoxDaylightSaving->setChecked(true);
      };

      lineEditIAUCode->setText(QString::fromStdString(observatory_->IAUCode()));

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));
    }


  } // namespace dialogs
} // namespace AstroManager
