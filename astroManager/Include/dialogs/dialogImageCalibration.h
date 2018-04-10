//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageCalibration
// SUBSYSTEM:						Image Calibration Dialog
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2012-2016 Gavin Blakeman.
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
// OVERVIEW:            Dialog for Image Calibration - Allows the user to select dark frame, flat frame etc.
//
// CLASSES INCLUDED:    CImageCalibrationDialog   - A dialog to allow the user to enter the information required to calibrate a
//                                                  frame.
//
// CLASS HIERARCHY:     CDialog
//                        - CImageCalibrationDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-20 GGB - AIRDAS 2013.03 release.
//                      2013-01-22 GGB - AIRDAS 0000.00 release.
//                      2012-01-28 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef DIALOGIMAGECALIBRATION_H
#define DIALOGIMAGECALIBRATION_H

#include "dialogs.h"

#include <ACL>

namespace AstroManager
{
  namespace dialogs
  {
    class CImageCalibrationDialog : public CDialog
    {
      Q_OBJECT

    private:
      QLineEdit *lineEditMasterDarkFrame;
      QGroupBox *groupBoxUseBias;
      QLineEdit *lineEditMasterBiasFrame;
      QGroupBox *groupBoxUseFlat;
      QLineEdit *lineEditMasterFlatFrame;
      QCheckBox *checkBoxIgnoreTemperature;
      QCheckBox *checkBoxIgnoreExposure;
      QCheckBox *checkBoxSaveOriginal;
      QCheckBox *checkBoxAppendFrames;

      ACL::SCalibrateImage_Ptr calibrateImage;

      void setupUI();
      void saveUI();

    protected:
    public:
      CImageCalibrationDialog(ACL::SCalibrateImage_Ptr);
      virtual ~CImageCalibrationDialog() {;}

    private slots:
      void eventButtonCalibrate(bool);
      void eventButtonCancel(bool) { dlg->reject(); }

      void eventButtonSelectDarkFrame(bool);
      void eventButtonSelectFlatFrame(bool);
      void eventButtonSelectBiasFrame(bool);

      void eventGroupSelectBiasFrame(bool);
    };

  }  // namespace dialogs
}  // namespace AstroManager

#endif
