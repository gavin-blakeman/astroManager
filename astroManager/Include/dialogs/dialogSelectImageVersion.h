//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogSelectImageVersion
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017 Gavin Blakeman.
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
// OVERVIEW:            Dialog for selecting an image version to downLoad
//
// CLASSES INCLUDED:    CDialogSelectImageVersion
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogSelectImageVersion
//
// HISTORY:             2017-08-12 GGB - File created
//
//*********************************************************************************************************************************

#ifndef DIALOGSELECTIMAGEVERSION
#define DIALOGSELECTIMAGEVERSION

#include "dialogs.h"
#include "../qtExtensions/qt.h"
#include "../qtExtensions/selectImageVersionQueryModel.h"
#include "../VSOP.h"

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogSelectImageVersion : public CDialog
    {
      Q_OBJECT

    private:
      imageVersion_t &imageVersion_;
      QTableView *tableViewVersions;
      QCheckBox *checkBoxMostRecent;
      QPushButton *pushButtonOpenImage_;

      QTE::CSelectImageVersionQueryModel queryModel;

    protected:
      void setupUI();

    public:
      CDialogSelectImageVersion(imageID_t, imageVersion_t &, QObject *);
      ~CDialogSelectImageVersion() {}

    private slots:
      void pushButtonOpenImage(bool);
      void pushButtonCancel(bool) { dlg->reject(); }
      void eventRowActivated(const QModelIndex &);
      void eventRowDoubleClick(const QModelIndex &);
    };


  } // namespace dialogs
} // namespace AstroManager


#endif // DIALOGSELECTIMAGEVERSION

