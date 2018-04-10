﻿//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                DialogSimpleXY
// SUBSYSTEM:						Dialog manage the automatic source finding process with the simpleXY algorithm.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2014-2016 Gavin Blakeman.
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
// OVERVIEW:						Implements a dialog to allow the user to set paramters for the findstars algorithm.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-02-16 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogSimpleXY.h"

#include "../../Include/Error.h"
#include "../../Include/Settings.h"

namespace AstroManager
{
  namespace dialogs
  {

    /// Constructor for the class.
    //
    // 2014-02-16/GGB - Function created.

    CDialogSimpleXY::CDialogSimpleXY(libAstrometry::SSimpleXY &nsp, bool &nrv) : CDialog(":/forms/dialogSimpleXY.ui"), sourceParameters(nsp),
      returnValue(nrv)
    {
      returnValue = false;
      setupUI();
    }

    void CDialogSimpleXY::eventButtonExtract(bool)
    {

    }

    /// Function to respond to the accept solution button.
    //
    // 2014-02-23/GGB - Function created.

    void CDialogSimpleXY::eventButtonAccept(bool)
    {
      returnValue = true;
      dlg->reject();
    }

    /// Sets up the UI elements of the dialog.
    //
    // 2014-02-16/GGB - Function created.

    void CDialogSimpleXY::setupUI()
    {
      doubleSpinBoxDPSF = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxDPSF");
      doubleSpinBoxPLIM = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxPLIM");
      doubleSpinBoxDLIM = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxDLIM");
      doubleSpinBoxSaddle = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxSaddle");
      spinBoxMaxper = dlg->findChild<QSpinBox *>("spinBoxMaxper");
      horizontalSliderMaxPeaks = dlg->findChild<QSlider *>("horizontalSliderMaxPeaks");
      spinBoxMaxSize = dlg->findChild<QSpinBox *>("spinBoxMaxSize");
      spinBoxHalfBox = dlg->findChild<QSpinBox *>("spinBoxHalfBox");
      doubleSpinBoxSigma = dlg->findChild<QDoubleSpinBox *>("spinBoxSigma");

      if (!doubleSpinBoxDPSF || !doubleSpinBoxPLIM || !doubleSpinBoxDLIM || !doubleSpinBoxSaddle || !spinBoxMaxper ||
          !horizontalSliderMaxPeaks || !spinBoxMaxSize || !spinBoxHalfBox || !doubleSpinBoxSigma)
      {
        CODE_ERROR(AIRDAS);
      };

      doubleSpinBoxDPSF->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_DPSF,
                                                                QVariant(sourceParameters.dpsf)).toDouble());
      doubleSpinBoxPLIM->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_PLIM,
                                                                QVariant(sourceParameters.plim)).toDouble());
      doubleSpinBoxDLIM->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_DLIM,
                                                                QVariant(sourceParameters.dlim)).toDouble());
      doubleSpinBoxSaddle->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_SADDLE,
                                                                  QVariant(sourceParameters.saddle)).toDouble());
      spinBoxMaxper->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_MAXPER,
                                                            QVariant(sourceParameters.maxper)).toLongLong());
      horizontalSliderMaxPeaks->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_MAXNPEAKS,
                                                                       QVariant(static_cast<qlonglong>(sourceParameters.maxnpeaks))).toULongLong());
      spinBoxMaxSize->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_MAXSIZE,
                                                             QVariant(sourceParameters.maxsize)).toLongLong());
      spinBoxHalfBox->setValue(settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_HALFBOX,
                                                             QVariant(sourceParameters.halfbox)).toLongLong());
      doubleSpinBoxSigma->setValue((settings::VSOPSettings->value(settings::SOURCE_EXTRACTION_SIMPLEXY_SIGMA,
                                                                  QVariant(sourceParameters.sigma))).toDouble());

      connect(dlg->findChild<QPushButton *>("pushButtonExtract"), SIGNAL(clicked(bool)), this, SLOT(eventButtonExtract(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonAccept"), SIGNAL(clicked(bool)), this, SLOT(eventButtonAccept(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventButtonCancel(bool)));
    }


  } // namespace dialogs
} // namespace AstroManager
