//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Utilities
// SUBSYSTEM:						Various Utility Classes
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman.
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
// OVERVIEW:            Implements functions required for the utility menu for openAIP
//
//
// CLASSES INCLUDED:
//
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2011-06-11 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_UTILITIES_H
#define ASTROMANAGER_UTILITIES_H

  // astroManager files

#include "dialogs/dialogs.h"
#include "error.h"

  // Miscellaneous library header files.

#include <QCL>

namespace astroManager
{
  namespace utilities
  {
    class CEnterDATDialog : public dialogs::CDialog
    {
    Q_OBJECT

    private:
      QTableWidget *tableWidget;
      QDateEdit *dateEditMJD;
      QSpinBox *spinBoxValue;

      void setupUI();

    protected:
    public:
      CEnterDATDialog();

    private slots:
      void btnCancel(bool) { dlg->reject(); }
      void pushButtonInsert(bool);
    };

  }  // namespace utilities

}  // namespace AstroManager

#endif  // UTILITIES_H
