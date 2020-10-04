//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowSelectImage
// SUBSYSTEM:						Subclass for selection of images from the database.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2020 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CWindowSelectImage
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        CMdiSubWindow
//                          CWindowSelectImage
//
// HISTORY:             2017-07-28/GGB - File Created
//
//*********************************************************************************************************************************

#include "include/windowSelectImage.h"

  // Qxt Library

#include <QxtGui/QxtConfirmationMessage>

  // astroManager header files

#include "include/database/databaseARID.h"
#include "include/dialogs/dialogImageDetails.h"
#include "include/FrameWindow.h"
#include "include/settings.h"

#include <GCL>

namespace astroManager
{
  /// @brief Default constructor for the class.
  /// @param[in] parent - The parent or owner window.
  /// @throws std::bad_alloc
  /// @version 2017-07-28/GGB - Function created.

  CWindowSelectImage::CWindowSelectImage(QWidget *parent) : CMdiSubWindow(parent), queryModel()
  {
    setupUI();

    setWindowTitle(tr("Search Images"));
  }

  /// @brief Opens the dialog to allow all the image data to be viewed or edited.
  /// @throws None.
  /// @version 2017-08-16/GGB - Function created.

  void CWindowSelectImage::eventEditImageData(bool)
  {
    QModelIndex indexCopy = tableViewImages->currentIndex().sibling(tableViewImages->currentIndex().row(),
                                                                    QTE::CSelectImageQueryModel::imageID_c);

    database::imageID_t imageID = queryModel.data(indexCopy, Qt::DisplayRole).toUInt();

    dialogs::CDialogImageDetails dialogImageDetails(this, imageID);

    dialogImageDetails.exec();
  }

  /// @brief Responds to the openImage button being pressed.
  /// @throws None.
  /// @version 2017-08-12/GGB - Function created.

  void CWindowSelectImage::eventOpenImage(bool)
  {
    eventRowDoubleClick(tableViewImages->currentIndex());
  }

  /// @brief Function called to delete an image.
  /// @note 1. This function deletes the image data, as well as the meta-data.
  /// @throws None.
  /// @version 2018-05-12/GGB - Function created.

  void CWindowSelectImage::eventPushButtonDelete(bool)
  {
    QxtConfirmationMessage msgBox;

    QModelIndex indexCopy = tableViewImages->currentIndex().sibling(tableViewImages->currentIndex().row(),
                                                                    QTE::CSelectImageQueryModel::imageID_c);

    database::imageID_t imageID = queryModel.data(indexCopy, Qt::DisplayRole).toUInt();

    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(tr("Are you sure you want to delete."));
    QIcon messageIcon(":/images/user_judge.png");

    msgBox.setIconPixmap(messageIcon.pixmap(32, 32));
    msgBox.setInformativeText(tr("The data associated with all versions of this image will be deleted. The metadata for the " \
                                 "image will not be deleted."));
    msgBox.setConfirmationText(tr("Do not show again."));
    msgBox.addButton("Accept", QMessageBox::AcceptRole);
    msgBox.setDefaultButton(msgBox.addButton("Reject", QMessageBox::RejectRole));
    msgBox.setOverrideSettingsKey(settings::CM_IMAGE_DELETE_DELETEIMAGE);

    if (msgBox.exec() == QMessageBox::AcceptRole)
    {
      database::databaseARID->imageDeleteImage(imageID);   // Delete the image.

      GCL::logger::defaultLogger().logMessage(GCL::logger::info, "Image Data Deleted. imageID = " + std::to_string(imageID));
    };

    queryModel.resetQuery();
  }

  /// @brief Refreshes the data by requerying the underlying model.
  /// @throws None.
  /// @version 2018-05-12/GGB - Added button to delete images. (Bug #132)
  /// @version 2017-08-14/GGB - Function created.

  void CWindowSelectImage::eventRefreshData(bool)
  {
    pushButtonOpenImage->setEnabled(false);
    pushButtonEditData->setEnabled(false);
    pushButtonDeleteImage->setEnabled(false);

    queryModel.resetQuery();
  }

  /// @brief Activates the push buttons when a row is activated.
  /// @throws None.
  /// @version 2018-05-12/GGB - Added button to delete images. (Bug #132)
  /// @version 2017-08-12/GGB - Function created.

  void CWindowSelectImage::eventRowActivated(const QModelIndex &)
  {
    pushButtonOpenImage->setEnabled(true);
    pushButtonEditData->setEnabled(true);
    pushButtonDeleteImage->setEnabled(true);
  }

  /// @brief Reponds to a selected row being double clicked. Same as opening the image.
  /// @param[in] index - The index of the item that is double clicked.
  /// @throws
  /// @version 2017-08-11/GGB - Function created.

  void CWindowSelectImage::eventRowDoubleClick(const QModelIndex &index)
  {
    QModelIndex indexCopy = index.sibling(index.row(), QTE::CSelectImageQueryModel::imageID_c);

    database::imageID_t imageID = queryModel.data(indexCopy, Qt::DisplayRole).toUInt();

    dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->imageOpenFromDatabase(imageID);
  }

  /// @brief Setup up the user interface elements.
  /// @throws GCL::CRuntimeError(astroManager, ...)
  /// @version 2018-05-12/GGB - Added button to delete images. (Bug #132)
  /// @version 2017-07-28/GGB -Function created.

  void CWindowSelectImage::setupUI()
  {
    QUiLoader loader;

      // Create the window details from the template

    QFile file(":/windows/windowSelectImage.ui");

    if (!file.open(QFile::ReadOnly))
    {
      ERRORMESSAGE("Unable to open resource :/windows/windowSelectImage.ui");
      ERROR(astroManager, 0x0001);
    }

    QWidget *formWidget = loader.load(&file, this);
    file.close();

    setWidget(formWidget);
    ASSOCIATE_TABLEVIEW(tableViewImages, formWidget, "tableViewImages");
    ASSOCIATE_PUSHBUTTON(pushButtonOpenImage, formWidget, "pushButtonOpenImage");
    ASSOCIATE_PUSHBUTTON(pushButtonEditData, formWidget, "pushButtonEditData");
    ASSOCIATE_PUSHBUTTON(pushButtonRefreshData, formWidget, "pushButtonRefreshData");
    ASSOCIATE_PUSHBUTTON(pushButtonDeleteImage, formWidget, "pushButtonDeleteImage");

    tableViewImages->setModel(&queryModel);
    tableViewImages->setSortingEnabled(true);
    tableViewImages->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableViewImages->setColumnHidden(QTE::CSelectImageQueryModel::imageID_c, true);

    pushButtonOpenImage->setEnabled(false);
    pushButtonEditData->setEnabled(false);
    pushButtonDeleteImage->setEnabled(false);

    connect(tableViewImages, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(eventRowDoubleClick(const QModelIndex &)));
    connect(tableViewImages, SIGNAL(activated(const QModelIndex &)), this, SLOT(eventRowActivated(const QModelIndex &)));
    connect(tableViewImages, SIGNAL(clicked(const QModelIndex &)), this, SLOT(eventRowActivated(const QModelIndex &)));
    connect(pushButtonOpenImage, SIGNAL(clicked(bool)), this, SLOT(eventOpenImage(bool)));
    connect(pushButtonEditData, SIGNAL(clicked(bool)), this, SLOT(eventEditImageData(bool)));
    connect(pushButtonRefreshData, SIGNAL(clicked(bool)), this, SLOT(eventRefreshData(bool)));
    connect(pushButtonDeleteImage, SIGNAL(clicked(bool)), this, SLOT(eventPushButtonDelete(bool)));
  }

} // namespace AstroManager
