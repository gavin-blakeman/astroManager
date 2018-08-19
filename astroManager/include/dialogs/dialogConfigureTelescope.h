//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogConfigureTelescope
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
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
// OVERVIEW:            Dialog for entering/changing/registering data for a telescope
//
// CLASSES INCLUDED:    CDialogConfigureTelescope
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogConfigureTelescope
//
// HISTORY:             2017-08-05 GGB - Class moved into seperate file from configure.h(cpp)
//
//*********************************************************************************************************************************

#ifndef DIALOGCONFIGURETELESCOPE
#define DIALOGCONFIGURETELESCOPE

#include "../ACL/telescope.h"
#include "dialogs.h"

namespace astroManager
{
  namespace dialogs
  {

    class CDialogConfigureTelescope : public CDialog
    {
      Q_OBJECT
    private:
      QLineEdit *lineEditShortText;
      QLineEdit *lineEditManufacturer;
      QLineEdit *lineEditModel;
      QSpinBox *spinBoxAperture;
      QDoubleSpinBox *doubleSpinBoxFocalLength;
      QDoubleSpinBox *doubleSpinBoxObstruction;

      CTelescope *telescope_;
      void setupUI();

    protected:
    public:
      CDialogConfigureTelescope(CTelescope *);

    private slots:
      void btnOk(bool);
      void btnCancel(bool) { dlg->reject(); }
    };

  } // namespace dialogs
} // namespace AstroManager

#endif // DIALOGCONFIGURETELESCOPE

