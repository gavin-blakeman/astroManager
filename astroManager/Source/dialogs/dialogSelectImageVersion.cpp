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

#include "../../Include/dialogs/dialogSelectImageVersion.h"

#include "../../Include/database/databaseARID.h"
#include "../../Include/qtExtensions/qt.h"

  // Miscellaneous libraries

#include <GCL>

namespace AstroManager
{
  namespace dialogs
  {
    /// @brief Class constructor.
    /// @param[in] imageID - The ID of the image to list the versions.
    /// @param[out] imageVersion - Reference to the variable to set to the selected imageVersion.
    /// @throws std::bad_alloc
    /// @version 2017-08-12/GGB - Function created.

    CDialogSelectImageVersion::CDialogSelectImageVersion(imageID_t imageID, imageVersion_t &imageVersion, QObject *parent) :
      CDialog(":/dialogs/dialogSelectImageVersion.ui", parent), imageVersion_(imageVersion), queryModel(imageID)
    {
      setupUI();
    }

    /// @brief Activates the push buttons when a row is activated.
    /// @throws None.
    /// @version 2017-08-14/GGB - Function created.

    void CDialogSelectImageVersion::eventRowActivated(const QModelIndex &)
    {
      pushButtonOpenImage_->setEnabled(true);
    }

    /// @brief Reponds to a selected row being double clicked. Same as opening the image.
    /// @param[in] index - The index of the item that is double clicked.
    /// @throws
    /// @version 2017-08-11/GGB - Function created.

    void CDialogSelectImageVersion::eventRowDoubleClick(const QModelIndex &index)
    {
      QModelIndex indexCopy = index.sibling(index.row(), 0);

      imageVersion_ = queryModel.data(indexCopy, Qt::DisplayRole).toUInt();

      dlg->accept();
    }

    /// @brief Processes the Open Image push button.
    /// @throws None.
    /// @version 2017-08-14/GGB - Function created.

    void CDialogSelectImageVersion::pushButtonOpenImage(bool)
    {
      eventRowDoubleClick(tableViewVersions->currentIndex());
    }

    /// @brief Sets up the initial dialog.
    /// @throws GCL::CRuntimeAssert
    /// @version 2017-08-12/GGB - Function created.

    void CDialogSelectImageVersion::setupUI()
    {
      tableViewVersions = findChild<QTableView *>("tableViewVersions");
      checkBoxMostRecent = findChild<QCheckBox *>("checkBoxMostRecent");
      pushButtonOpenImage_ = findChild<QPushButton *>("pushButtonOpenImage");

      tableViewVersions->setModel(&queryModel);
      tableViewVersions->setSortingEnabled(true);
      tableViewVersions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
      tableViewVersions->setColumnHidden(9, true);

      pushButtonOpenImage_->setEnabled(false);

      connect(pushButtonOpenImage_, SIGNAL(clicked(bool)), this, SLOT(pushButtonOpenImage(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(pushButtonCancel(bool)));

      connect(tableViewVersions, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(eventRowDoubleClick(const QModelIndex &)));
      connect(tableViewVersions, SIGNAL(activated(const QModelIndex &)), this, SLOT(eventRowActivated(const QModelIndex &)));
      connect(tableViewVersions, SIGNAL(clicked(const QModelIndex &)), this, SLOT(eventRowActivated(const QModelIndex &)));
    }

  } // namespace dialogs
} // namespace AstroManager
