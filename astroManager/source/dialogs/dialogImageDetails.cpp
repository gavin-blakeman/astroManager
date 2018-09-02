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

#include "../../include/dialogs/dialogImageDetails.h"

#include "../../include/settings.h"

#include "../../include/qtExtensions/selectImageVersionQueryModel.h"

  // Qxt Library

#include <QxtGui/QxtConfirmationMessage>

namespace astroManager
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

    /// @brief Function to delete an image. Only the image data is deleted. The image metadata is not deleted.
    /// @note 1. This function leaves "overhanging" TBL_IMAGES records with no TBL_IMAGESTORAGE records.
    /// @throws None.
    /// @version 2018-05-12/GGB - Function created.

    void CDialogImageDetails::eventPushButtonDeleteImageData(bool)
    {
      QxtConfirmationMessage msgBox;

      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setText(tr("Are you sure you want to delete."));
      QIcon messageIcon(":/images/user_judge.png");

      msgBox.setIconPixmap(messageIcon.pixmap(32, 32));
      msgBox.setInformativeText(tr("The data associated with all versions of this image will be deleted. The metadata for the " \
                                   "image will not be deleted."));
      msgBox.setConfirmationText(tr("Do not show again."));
      msgBox.addButton("Accept", QMessageBox::AcceptRole);
      msgBox.setDefaultButton(msgBox.addButton("Reject", QMessageBox::RejectRole));
      msgBox.setOverrideSettingsKey(settings::CM_IMAGE_DELETE_DELETEALL);

      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        database::databaseARID->imageDeleteImageData(imageID_);   // Delete all the image data.

        GCL::logger::defaultLogger().logMessage(GCL::logger::info, "Image Data Deleted. imageID = " + std::to_string(imageID_));
      };

      queryModelVersion.resetQuery();
    }

    void CDialogImageDetails::eventPushButtonDeleteVersion(bool)
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
      pushButtonDeleteVersion = findChild<QPushButton *>("pushButtonDeleteVersion");
      pushButtonDeleteImageData = findChild<QPushButton *>("pushButtonDeleteImageData");

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
      connect(pushButtonDeleteVersion, SIGNAL(clicked(bool)), this, SLOT(eventPushButtonDeleteVersion(bool)));
      connect(pushButtonDeleteImageData, SIGNAL(clicked(bool)), this, SLOT(eventPushButtonDeleteImageData(bool)));
    }

  }   // namespace dialogs
}   // namespace AstroManager
