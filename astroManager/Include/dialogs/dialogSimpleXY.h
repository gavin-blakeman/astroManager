//*********************************************************************************************************************************
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
// OVERVIEW:						Implements a dialog to allow the user to set paramters for the findstars algorithm.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-02-16 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef DIALOGSIMPLEXY_H
#define DIALOGSIMPLEXY_H

#include "dialogs.h"

#include <Include/simplexy.h>

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogSimpleXY : public CDialog
    {
      Q_OBJECT

    private:
      QDoubleSpinBox *doubleSpinBoxDPSF;
      QDoubleSpinBox *doubleSpinBoxPLIM;
      QDoubleSpinBox *doubleSpinBoxDLIM;
      QDoubleSpinBox *doubleSpinBoxSaddle;
      QSpinBox *spinBoxMaxper;
      QSlider *horizontalSliderMaxPeaks;
      QSpinBox *spinBoxMaxSize;
      QSpinBox *spinBoxHalfBox;
      QDoubleSpinBox *doubleSpinBoxSigma;

      libAstrometry::SSimpleXY &sourceParameters;
      bool &returnValue;

      void setupUI();

    protected:
    public:
      CDialogSimpleXY(libAstrometry::SSimpleXY &nsp, bool &nrv);

    public slots:
      void eventButtonExtract(bool);
      void eventButtonAccept(bool);
      void eventButtonCancel(bool) { returnValue = false; dlg->reject(); }
    };

  }   // namespace dialogs
}     // namespace AstroManager



#endif // DIALOGSIMPLEXY_H
