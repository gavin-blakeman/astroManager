//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowSelectImage
// SUBSYSTEM:						Subclass for selection of images from the database.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager::dockwidgets
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

#include "../Include/windowSelectImage.h"

#include "../Include/database/databaseARID.h"
#include "../Include/dialogs/dialogImageDetails.h"
#include "../Include/FrameWindow.h"

#include <GCL>

namespace AstroManager
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
    QModelIndex indexCopy = tableViewImages->currentIndex().sibling(tableViewImages->currentIndex().row(), QTE::CSelectImageQueryModel::imageID_c);

    imageID_t imageID = queryModel.data(indexCopy, Qt::DisplayRole).toUInt();

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

  /// @brief Refreshes the data by requerying the underlying model.
  /// @throws None.
  /// @version 2017-08-14/GGB - Function created.

  void CWindowSelectImage::eventRefreshData(bool)
  {
    pushButtonOpenImage->setEnabled(false);
    pushButtonEditData->setEnabled(false);

    queryModel.resetQuery();
  }

  /// @brief Activates the push buttons when a row is activated.
  /// @throws None.
  /// @version 2017-08-12/GGB - Function created.

  void CWindowSelectImage::eventRowActivated(const QModelIndex &)
  {
    pushButtonOpenImage->setEnabled(true);
    pushButtonEditData->setEnabled(true);
  }

  /// @brief Reponds to a selected row being double clicked. Same as opening the image.
  /// @param[in] index - The index of the item that is double clicked.
  /// @throws
  /// @version 2017-08-11/GGB - Function created.

  void CWindowSelectImage::eventRowDoubleClick(const QModelIndex &index)
  {
    QModelIndex indexCopy = index.sibling(index.row(), QTE::CSelectImageQueryModel::imageID_c);

    imageID_t imageID = queryModel.data(indexCopy, Qt::DisplayRole).toUInt();

    dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->imageOpenFromDatabase(imageID);
  }

  /// @brief Setup up the user interface elements.
  /// @throws GCL::CRuntimeError(AIRDAS, ...)
  /// @version 2017-07-28/GGB -Function created.

  void CWindowSelectImage::setupUI()
  {
    QUiLoader loader;

      // Create the window details from the template

    QFile file(":/windows/windowSelectImage.ui");

    if (!file.open(QFile::ReadOnly))
    {
      ERRORMESSAGE("Unable to open resource :/windows/windowSelectImage.ui");
      ERROR(AIRDAS, 0x0001);
    }

    QWidget *formWidget = loader.load(&file, this);
    file.close();

    setWidget(formWidget);
    ASSOCIATE_TABLEVIEW(tableViewImages, formWidget, "tableViewImages");
    ASSOCIATE_PUSHBUTTON(pushButtonOpenImage, formWidget, "pushButtonOpenImage");
    ASSOCIATE_PUSHBUTTON(pushButtonEditData, formWidget, "pushButtonEditData");
    ASSOCIATE_PUSHBUTTON(pushButtonRefreshData, formWidget, "pushButtonRefreshData");

    tableViewImages->setModel(&queryModel);
    tableViewImages->setSortingEnabled(true);
    tableViewImages->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableViewImages->setColumnHidden(QTE::CSelectImageQueryModel::imageID_c, true);

    pushButtonOpenImage->setEnabled(false);
    pushButtonEditData->setEnabled(false);

    connect(tableViewImages, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(eventRowDoubleClick(const QModelIndex &)));
    connect(tableViewImages, SIGNAL(activated(const QModelIndex &)), this, SLOT(eventRowActivated(const QModelIndex &)));
    connect(tableViewImages, SIGNAL(clicked(const QModelIndex &)), this, SLOT(eventRowActivated(const QModelIndex &)));
    connect(pushButtonOpenImage, SIGNAL(clicked(bool)), this, SLOT(eventOpenImage(bool)));
    connect(pushButtonEditData, SIGNAL(clicked(bool)), this, SLOT(eventEditImageData(bool)));
    connect(pushButtonRefreshData, SIGNAL(clicked(bool)), this, SLOT(eventRefreshData(bool)));
  }

} // namespace AstroManager
