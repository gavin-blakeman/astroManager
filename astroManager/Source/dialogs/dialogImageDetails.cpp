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

#include "../../Include/dialogs/dialogImageDetails.h"

#include "../../Include/qtExtensions/selectImageVersionQueryModel.h"

namespace AstroManager
{
  namespace dialogs
  {
    /// @brief Constructor
    /// @param[in] parent - The parent object.
    /// @param[in] imageID - The ID of the image.
    /// @throws std::bad_alloc
    /// @version 2017-08-15/GGB - Function created.

    CDialogImageDetails::CDialogImageDetails(QObject *parent, imageID_t imageID) :
      CDialog(":/dialogs/dialogImageDetails.ui", parent), imageID_(imageID), queryModelVersion(imageID, parent)
    {
      setupUI();
    }

    void CDialogImageDetails::eventPushButtonDelete(bool)
    {
    }

    /// @brief Processes the OK Push button event.
    /// @throws None.
    /// @version 2017-08-20/GGB - Function created.

    void CDialogImageDetails::eventPushButtonOK(bool)
    {      
      if (true)
      {
        database::databaseARID->updateImageQuality(imageID_, comboBoxQuality->itemData(comboBoxQuality->currentIndex()).toUInt());
      };

      if (true)
      {
        database::databaseARID->updateImageComments(imageID_, plainTextEditComments->toPlainText());
      };

      database::databaseARID->updateImageFlags(imageID_, checkBoxAstrometric->isChecked(), checkBoxPhotometric->isChecked());

      dlg->accept();
    }

    /// @brief Setup the user interface elements.
    /// @throws GCL::CCodeError()
    /// @version 2017-08-15/GGB - Function created.

    void CDialogImageDetails::setupUI()
    {
      labelImageID = findChild<QLabel *>("labelImageID");
      labelImageUUID = findChild<QLabel *>("labelImageUUID");
      labelFilePath = findChild<QLabel *>("labelFilePath");
      labelImageDate = findChild<QLabel *>("labelImageDate");
      labelImageTime = findChild<QLabel *>("labelImageTime");
      labelObservingSite = findChild<QLabel *>("labelObservingSite");
      labelTelescope = findChild<QLabel *>("labelTelescope");
      labelTarget = findChild<QLabel *>("labelTarget");
      labelCoordinates = findChild<QLabel *>("labelCoordinates");
      labelFilter = findChild<QLabel *>("labelFilter");
      comboBoxQuality = findChild<QComboBox *>("comboBoxQuality");
      plainTextEditComments = findChild<QPlainTextEdit *>("plainTextEditComments");
      checkBoxAstrometric = findChild<QCheckBox *>("checkBoxAstrometric");
      checkBoxPhotometric = findChild<QCheckBox *>("checkBoxPhotometric");
      groupBoxSyntheticImage = findChild<QGroupBox *>("groupBoxSyntheticImage");
      listViewSynthetic = findChild<QListView *>("listViewSynthetic");
      tableViewVersions = findChild<QTableView *>("tableViewVersions");
      pushButtonDeleteVersion = findChild<QPushButton *>("pushButtonDeleteVersion");
      pushButtonOK = findChild<QPushButton *>("pushButtonOK");
      pushButtonDelete = findChild<QPushButton *>("pushButtonDelete");

      tableViewVersions->setModel(&queryModelVersion);
      tableViewVersions->setSortingEnabled(true);
      tableViewVersions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
      //tableViewImages->setColumnHidden(10, true);

      comboBoxQuality->addItem("*", 1);
      comboBoxQuality->addItem("**", 2);
      comboBoxQuality->addItem("***", 3);
      comboBoxQuality->addItem("****", 4);
      comboBoxQuality->addItem("*****", 5);

      database::databaseARID->populateDialogImageDetails(this);

      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(eventPushButtonCancel(bool)));
      connect(pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(eventPushButtonOK(bool)));
      connect(pushButtonDelete, SIGNAL(clicked(bool)), this, SLOT(eventPushButtonDelete(bool)));
    }

  }   // namespace dialogs
}   // namespace AstroManager
