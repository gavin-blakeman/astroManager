//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogSaveAligned
// SUBSYSTEM:						Dialog to save aligned images. (Special requirements)
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imagedisplay
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CDialogSaveAligned
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CDialogSaveAligned
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-06-12 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef DIALOGIMAGEALIGNED_H
#define DIALOGIMAGEALIGNED_H

#include "dialogs.h"

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogSaveAligned : public dialogs::CDialog
    {
      Q_OBJECT

    private:
      QListWidget *listWidgetOriginal;
      QListWidget *listWidgetAligned;
      QPushButton *pushButtonSelectOriginal;
      QPushButton *pushButtonUnselectOriginal;
      QPushButton *pushButtonSelectAligned;
      QPushButton *pushButtonUnselectAligned;

      QListWidget *listWidgetImages;

      void setupUI();

    protected:
    public:
      CDialogSaveAligned(QListWidget *);

    private slots:
      void eventSelectOriginal(bool);
      void eventUnselectOriginal(bool);
      void eventSelectAligned(bool);
      void eventUnselectAligned(bool);
      void eventButtonCancel() { dlg->reject(); }
      void eventButtonSave();
    };
  }
}

#endif // DIALOGIMAGEALIGNED_H
