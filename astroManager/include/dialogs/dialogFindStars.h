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
// OVERVIEW:						Implements a dialog to allow the user to set paramters for the findstars algorithm.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2014-02-02 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef DIALOGFINDSTARS_H
#define DIALOGFINDSTARS_H

#include "dialogs.h"

#include <ACL>

namespace astroManager
{
  namespace dialogs
  {
    int const DialogExtract = 2;

    struct SDialogFindStars : public ACL::SFindSources
    {
    public:
      ACL::AXIS_t minBorder;
    };

    class CDialogFindStars : public CDialog
    {
      Q_OBJECT

    private:
      QSpinBox *spinBoxBorder;
      QSpinBox *spinBoxRNOISE;
      QDoubleSpinBox *doubleSpinBoxMinimumPeak;
      QDoubleSpinBox *doubleSpinBoxStarSigma;
      QSpinBox *spinBoxISTATPIX;
      QSpinBox *spinBoxNSTATPIX;
      QSpinBox *spinBoxMaxWalk;
      QDoubleSpinBox *doubleSpinBoxBurnedOut;
      QSpinBox *spinBoxMinSep;
      QSpinBox *spinBoxMaxRad;
      QSpinBox *spinBoxMinRad;

      SDialogFindStars &sourceParameters;

      void setupUI();

    protected:
    public:
      CDialogFindStars(SDialogFindStars &nsp);
    public slots:
      void eventButtonExtract(bool);
    };

  }   // namespace dialogs
}     // namespace AstroManager

#endif // DIALOGFINDSTARS_H
