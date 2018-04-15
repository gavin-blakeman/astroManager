//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogSelectObserver
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
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
// OVERVIEW:            Implements the MDI Frame window for the application. There is a single class implemented:
//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CSelectObserverDialog
//
// CLASS HIERARCHY:     CDialog
//                        - CSelectObserverDialog
//                        - CSelectInstrumentDialog
//                        - CSelectTelescopeDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-05-27 GGB - Moved CSelectObserverDialog into own file.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-02-03 GGB - Moved CMdiSubWindow to own file and renamed.
//                      2012-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef DIALOGSELECTOBSERVER_H
#define DIALOGSELECTOBSERVER_H

#include "dialogs.h"

namespace AstroManager
{
  namespace dialogs
  {
    class CSelectObserverDialog : public CDialog
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;

      void setupUI();

    protected:
    public:
      CSelectObserverDialog();

    private slots:
      void btnOk(bool);
      void btnCancel(bool) { dlg->reject(); };
    };
  };
};  // namespace AstroManager

#endif // DIALOGSELECTOBSERVER_H
