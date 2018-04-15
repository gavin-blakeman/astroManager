//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                DialogFindStars
// SUBSYSTEM:						Dialog manage the automatic source finding process with the findstars algorithm.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2014, 2018 Gavin Blakeman.
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
// OVERVIEW:						Implements a dialog to allow the user to set paramters for the findstars algorithm.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2014-02-02 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogFindStars.h"

#include "../../Include/Error.h"
#include "../../Include/Settings.h"

#include <ACL>

namespace AstroManager
{
  namespace dialogs
  {
    /// Class constructor
    //
    // 2014-02-02/GGB - Function created.

    CDialogFindStars::CDialogFindStars(SDialogFindStars &nsp) : CDialog(":/forms/dialogFindStars.ui"),
      sourceParameters(nsp)
    {
      setupUI();
    }

    /// Function to respond to the extract button.
    //
    // 2014-02-16/GGB - Function created.

    void CDialogFindStars::eventButtonExtract(bool)
    {
      sourceParameters.fsborder = spinBoxBorder->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_BORDER,
                                       QVariant(static_cast<qint64>(sourceParameters.fsborder)));

      sourceParameters.rnoise = spinBoxRNOISE->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_RNOISE,
                                       QVariant(static_cast<qint64>(sourceParameters.rnoise)));

      sourceParameters.bmin = doubleSpinBoxMinimumPeak->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_MINPEAK, QVariant(sourceParameters.bmin));

      sourceParameters.starsig = doubleSpinBoxStarSigma->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_STARSIGMA,QVariant(sourceParameters.starsig));

      sourceParameters.ispix = spinBoxISTATPIX->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_ISTATPIX,
                                       QVariant(static_cast<qint64>(sourceParameters.ispix)));

      sourceParameters.nspix = spinBoxNSTATPIX->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_NSTATPIX,
                                       QVariant(static_cast<qint64>(sourceParameters.nspix)));

      sourceParameters.maxw = spinBoxMaxWalk->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_MAXWALK,
                                       QVariant(static_cast<qint64>(sourceParameters.maxw)));

      sourceParameters.burnedout = doubleSpinBoxBurnedOut->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_BURNEDOUT,
                                       QVariant(static_cast<qint64>(sourceParameters.burnedout)));

      sourceParameters.minsep = spinBoxMinSep->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_MINSEP,
                                       QVariant(static_cast<qint64>(sourceParameters.minsep)));

      sourceParameters.maxrad = spinBoxMaxRad->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_MAXRAD,
                                       QVariant(static_cast<qint64>(sourceParameters.maxrad)));

      sourceParameters.minrad = spinBoxMinRad->value();
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_FINDSTARS_MINRAD,
                                       QVariant(static_cast<qint64>(sourceParameters.minrad)));

      dlg->done(DialogExtract);
    }

    /// Sets up all the data for the dialog.
    /// EXCEPTIONS: CODE_ERROR
    //
    // 2014-12-29/GGB - Added code to control the minimum border width. (Bug #1406196)
    // 2013-02-09/GGB - Function created.

    void CDialogFindStars::setupUI()
    {
      spinBoxBorder = dlg->findChild<QSpinBox *>("spinBoxBorder");
      spinBoxRNOISE = dlg->findChild<QSpinBox *>("spinBoxRNOISE");
      doubleSpinBoxMinimumPeak = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxMinimumPeak");
      doubleSpinBoxStarSigma = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxStarSigma");
      spinBoxISTATPIX = dlg->findChild<QSpinBox *>("spinBoxISTATPIX");
      spinBoxNSTATPIX = dlg->findChild<QSpinBox *>("spinBoxNSTATPIX");
      spinBoxMaxWalk = dlg->findChild<QSpinBox *>("spinBoxMaxWalk");
      doubleSpinBoxBurnedOut = dlg->findChild<QDoubleSpinBox *>("doubleSpinBoxBurnedOut");
      spinBoxMinSep = dlg->findChild<QSpinBox *>("spinBoxMinSep");
      spinBoxMaxRad = dlg->findChild<QSpinBox *>("spinBoxMaxRad");
      spinBoxMinRad = dlg->findChild<QSpinBox *>("spinBoxMinRad");

      if (!spinBoxBorder || !spinBoxRNOISE || !doubleSpinBoxMinimumPeak || !doubleSpinBoxStarSigma || !spinBoxISTATPIX ||
          !spinBoxNSTATPIX || !spinBoxMaxWalk || !doubleSpinBoxBurnedOut || !spinBoxMinSep || !spinBoxMaxRad || !spinBoxMinRad)
      {
        CODE_ERROR(astroManager);
      };

      spinBoxBorder->setMinimum(sourceParameters.minBorder);
      spinBoxBorder->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_BORDER,
                                                            QVariant(static_cast<qint64>(sourceParameters.fsborder))).toLongLong());
      spinBoxRNOISE->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_RNOISE,
                                                            QVariant(static_cast<qint64>(sourceParameters.rnoise))).toLongLong());
      doubleSpinBoxMinimumPeak->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_MINPEAK,
                                                                       QVariant(sourceParameters.bmin)).toDouble());
      doubleSpinBoxStarSigma->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_STARSIGMA,
                                                                     QVariant(sourceParameters.starsig)).toDouble());
      spinBoxISTATPIX->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_ISTATPIX,
                                                              QVariant(static_cast<qint64>(sourceParameters.ispix))).toLongLong());
      spinBoxNSTATPIX->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_NSTATPIX,
                                                              QVariant(static_cast<qint64>(sourceParameters.nspix))).toLongLong());
      spinBoxMaxWalk->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_MAXWALK,
                                                             QVariant(static_cast<qint64>(sourceParameters.maxw))).toLongLong());
      doubleSpinBoxBurnedOut->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_BURNEDOUT,
                                                                     QVariant(sourceParameters.burnedout)).toDouble());
      spinBoxMinSep->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_MINSEP,
                                                            QVariant(static_cast<qint64>(sourceParameters.minsep))).toLongLong());
      spinBoxMaxRad->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_MAXRAD,
                                                            QVariant(static_cast<qint64>(sourceParameters.maxrad))).toLongLong());
      spinBoxMinRad->setValue(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_FINDSTARS_MINRAD,
                                                            QVariant(static_cast<qint64>(sourceParameters.minrad))).toLongLong());

      connect(dlg->findChild<QPushButton *>("pushButtonExtract"), SIGNAL(clicked(bool)), this, SLOT(eventButtonExtract(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonAccept"), SIGNAL(clicked()), dlg, SLOT(accept()));
      connect(dlg->findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked()), dlg, SLOT(reject()));
    }

  }   // namespace dialogs
}     // namespace AstroManager
