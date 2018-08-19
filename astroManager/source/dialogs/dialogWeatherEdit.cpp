//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogWeatherEdit
// SUBSYSTEM:						Dialog for editing/entering weather information
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2016-2018 Gavin Blakeman.
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
//                          - CDialogWeatherEdit
//
// HISTORY:             2016-03-26 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../include/dialogs/dialogWeatherEdit.h"

  // astroManager header files

#include "../../include/Error.h"
#include "../../include/Settings.h"

#include <GCL>

namespace astroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CDialogWeatherEdit
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @throws None.
    /// @version 2016-03-26/GGB - Function created.

    CDialogWeatherEdit::CDialogWeatherEdit(ACL::CWeather &wd) : CDialog(":/forms/dialogWeatherEdit.ui"), weatherData(wd)
    {
      setupUI();
    }

    /// @brief Handles the Continue button click
    /// @throws None
    /// @version 2016-03-26/GGB - Function created.

    void CDialogWeatherEdit::eventButtonContinue(bool)
    {
      weatherData.temperature(PCL::CTemperature(spinBoxTemperature->value(), PCL::TU_C));
      weatherData.pressure(PCL::CPressure(spinBoxPressure->value() * 100, PCL::PU::PA));
      weatherData.RH(static_cast<float>(spinBoxRH->value()) / 100);     // Must be converted to a %

      dlg->accept();
    }


    /// @brief Sets up the UI for the dialog
    /// @throws CCodeError
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2016-03-26/GGB - Function created.

    void CDialogWeatherEdit::setupUI()
    {
      ASSOCIATE_CONTROL(spinBoxTemperature, dlg, "spinBoxTemperature", QSpinBox);
      ASSOCIATE_CONTROL(spinBoxPressure, dlg, "spinBoxPressure", QSpinBox);
      ASSOCIATE_CONTROL(spinBoxRH, dlg, "spinBoxRH", QSpinBox);

      if (weatherData.temperature())
      {
        spinBoxTemperature->setValue((*weatherData.temperature())(PCL::TU_C));
      };

      if (weatherData.pressure())
      {
        spinBoxPressure->setValue((*weatherData.pressure())(PCL::PU::PA) / 100);
      };

      if (weatherData.RH())
      {
        spinBoxRH->setValue(static_cast<int>(*weatherData.RH() * 100));   // RH in %
      };

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(eventButtonContinue(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCancel(bool)));
    }

  }  // namespace dialogs
}  // namespace AstroManager
