//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								JD2Gregorian
// SUBSYSTEM:						Subclass for calculation windows
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CWindowCalculation
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        CMdiSubWindow
//                          CWindowCalculation
//                            CWindowCalculationGregorian2JD
//
// HISTORY:             2017-07-06/GGB - File Created
//
//*********************************************************************************************************************************

#include "../../Include/windowCalculation/JD2Gregorian.h"

  // AIRDAS include files

#include "../../Include/Settings.h"

  // Miscellaneous include files.

#include <ACL>

namespace AstroManager
{
  namespace calculation
  {

    /// @brief Default class constructor.
    /// @param[in] parent - The parent Widget.
    /// @version 2017-07-09/GGB - Converted to use Qt.
    /// @version 2010-01-02/GGB - Function created.

    CJD2Gregorian::CJD2Gregorian(QWidget *parent) : CWindowCalculation(parent)
    {
      setupUI();

      setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    }

    /// @brief Calculates the date from the JD and displays it in the relevant control.
    /// @details Calls the TJD functions to perform the conversion.
    /// @parm[in] julianDayValue - The value from the spin box.
    /// @throws CCodeError.
    /// @version 2017-07-09/GGB - Converted to use Qt.
    /// @version 2010-01-02/GGB - Function created.

    void CJD2Gregorian::calculateDate()
    {
      ACL::TJD JD(static_cast<double>(spinBoxJulianDay->value()));

      if (radioButtonJD->isChecked())
      {
        labelDescription->setText(tr("Modified Julian Day (MJD)"));
        labelMJD->setText(QString("%1").arg(static_cast<double>(JD) - ACL::MJD0));
      }
      else if (radioButtonMJD->isChecked())
      {
        JD += ACL::MJD0;
        labelDescription->setText(tr("Julian Day (JD)"));
        labelMJD->setText(QString("%1").arg(static_cast<double>(JD)));
      }
      else
      {
        CODE_ERROR(AIRDAS);
      }

      labelGregorianDate->setText(QString::fromStdString(JD.gregorianDate()));

    }

    /// @brief Responds to a radio button being pressed.
    /// @throws None.
    /// @version 2017-07-09/GGB - Function created.

    void CJD2Gregorian::eventCalculateDate(int)
    {
      calculateDate();
    }

    /// @brief Responds to a radio button being pressed.
    /// @throws None.
    /// @version 2017-07-09/GGB - Function created.

    void CJD2Gregorian::eventRadioButtonClicked(bool)
    {
      if (radioButtonJD->isChecked())
      {
        settings::astroManagerSettings->setValue(settings::WINDOW_CALCULATION_JD2GREG_JDSELECTED, QVariant(true));
      }
      else
      {
        settings::astroManagerSettings->setValue(settings::WINDOW_CALCULATION_JD2GREG_JDSELECTED, QVariant(false));
      }
      calculateDate();
    }

    /// @brief Sets up the UI
    /// @throws GCL::CRuntimeError(AIRDAS)
    /// @throws GCL::CError(AIRDAS, 0x0001)
    /// @version 2017-07-09/GGB - Converted to use Qt.
    /// @version 2010-01-02/GGB - Function created.

    void CJD2Gregorian::setupUI()
    {
      QUiLoader loader;

      QFile file(":/windows/JD2Gregorian.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource file "":/windows/JD2Gregorian.ui"" ");
        ERROR(AIRDAS, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      ASSOCIATE_LABEL(labelGregorianDate, formWidget, "labelGregorianDate");
      ASSOCIATE_SPINBOX(spinBoxJulianDay, formWidget, "spinBoxJulianDay");
      ASSOCIATE_LABEL(labelDescription, formWidget, "labelDescription");
      ASSOCIATE_LABEL(labelMJD, formWidget, "labelMJD");
      ASSOCIATE_RADIOBUTTON(radioButtonJD, formWidget, "radioButtonJD");
      ASSOCIATE_RADIOBUTTON(radioButtonMJD, formWidget, "radioButtonMJD")

      if (settings::astroManagerSettings->value(settings::WINDOW_CALCULATION_JD2GREG_JDSELECTED, QVariant(true)).toBool())
      {
        radioButtonMJD->setChecked(false);
        radioButtonJD->setChecked(true);
      }
      else
      {
        radioButtonJD->setChecked(false);
        radioButtonMJD->setChecked(true);
      }

      connect(spinBoxJulianDay, SIGNAL(valueChanged(int)), this, SLOT(eventCalculateDate(int)));
      connect(radioButtonJD, SIGNAL(clicked(bool)), this, SLOT(eventRadioButtonClicked(bool)));
      connect(radioButtonMJD, SIGNAL(clicked(bool)), this, SLOT(eventRadioButtonClicked(bool)));
    }


  } // namespace calculation
} // namespace AstroManager
