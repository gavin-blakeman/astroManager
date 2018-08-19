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

#ifndef DIALOGWEATHEREDIT_H
#define DIALOGWEATHEREDIT_H

#include "dialogs.h"

#include <ACL>

namespace astroManager
{
  namespace dialogs
  {
    class CDialogWeatherEdit : public CDialog
    {
      Q_OBJECT

    private:
      QSpinBox *spinBoxTemperature;
      QSpinBox *spinBoxPressure;
      QSpinBox *spinBoxRH;
      bool bInternalCall;
      ACL::CWeather &weatherData;

      void setupUI();

    protected:
    public:
      CDialogWeatherEdit(ACL::CWeather &);

    private slots:
      void eventButtonContinue(bool);
      void eventButtonCancel(bool) { dlg->reject(); }
    };

  }  // namespace dialogs
}  // namespace AstroManager

#endif // DIALOGWEATHEREDIT_H
