//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogImageDetails
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// OVERVIEW:            Dialog for viewing/editing image details
//
// CLASSES INCLUDED:    CDialogImageDetails
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogImageDetails
//
// HISTORY:             2017-08-15 GGB - File created
//
//*********************************************************************************************************************************

#ifndef DIALOGIMAGEDETAILS
#define DIALOGIMAGEDETAILS

// Miscellaneous library header files

#include <QCL>


#include "../database/databaseARID.h"
#include "../qtExtensions/selectImageVersionQueryModel.h"
#include "../astroManager.h"

namespace astroManager
{
  namespace dialogs
  {
    class CDialogImageDetails : public QCL::CDialog
    {
      Q_OBJECT
    private:
      imageID_t imageID_;

      QLabel *labelImageID;
      QLabel *labelImageUUID;
      QLabel *labelFilePath;
      QLabel *labelImageDate;
      QLabel *labelImageTime;
      QLabel *labelObservingSite;
      QLabel *labelTelescope;
      QLabel *labelTarget;
      QLabel *labelCoordinates;
      QLabel *labelFilter;
      QComboBox *comboBoxQuality;
      QPlainTextEdit *plainTextEditComments;
      QCheckBox *checkBoxAstrometric;
      QCheckBox *checkBoxPhotometric;
      QGroupBox *groupBoxSyntheticImage;
      QListView *listViewSynthetic;
      QTableView *tableViewVersions;
      QPushButton *pushButtonOK;
      QPushButton *pushButtonDeleteVersion;
      QPushButton *pushButtonDeleteImageData;

      QTE::CSelectImageVersionQueryModel queryModelVersion;


    protected:
      void setupUI();

    public:
      CDialogImageDetails(QObject *, imageID_t);
      virtual ~CDialogImageDetails() {}

    public slots:
      void eventPushButtonOK(bool);
      void eventPushButtonCancel(bool) { emit dlg->reject(); }
      void eventPushButtonDeleteVersion(bool);
      void eventPushButtonDeleteImageData(bool);

      friend class database::CARID;

    };

  }
}

#endif // DIALOGIMAGEDETAILS

