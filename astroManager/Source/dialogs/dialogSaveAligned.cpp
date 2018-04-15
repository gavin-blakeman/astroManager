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
//
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CDialogSaveAligned
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//                          - CDialogSaveAligned
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-06-12 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogSaveAligned.h"

  // AIRDAS includes

#include "../../Include/ImageComparison.h"

  // Standard Library

#include <cstdint>

namespace AstroManager
{
  namespace dialogs
  {
    /// @brief Class constructor
    /// @param[in] lwi - Pointer to the list widget for the images to be saved.
    /// @returns None.
    /// @throws None.
    /// @version 2011-06-04/GGB - Function created.

    CDialogSaveAligned::CDialogSaveAligned(QListWidget *lwi) : CDialog(":/forms/dialogSaveAligned.ui"), listWidgetImages(lwi)
    {
      setupUI();
    }

    /// @brief Function to save the images. Each image is saved in turn. Only the checked images are saved.
    /// @parm None.
    /// @returns None.
    /// @throws None.
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2013-06-23/GGB - Function created.

    void CDialogSaveAligned::eventButtonSave()
    {
      int rows;
      int index;
      QListWidgetItem *lwi;
      imaging::SControlBlock *controlBlock;

        // Save the input Images.

      rows = listWidgetOriginal->count();

      for (index = 0; index < rows; ++index)
      {
        lwi = listWidgetOriginal->item(index);
        if (lwi->checkState() == Qt::Checked)
        {
          controlBlock = reinterpret_cast<imaging::SControlBlock *>(lwi->data(imaging::ROLE_CONTROLBLOCK).toULongLong());
//          controlBlock->inputImage.astroFile->saveToFile();
        };
      };

        // Save the output images.

      rows = listWidgetAligned->count();

      for (index = 0; index < rows; ++index)
      {
        lwi = listWidgetAligned->item(index);
        if (lwi->checkState() == Qt::Checked)
        {
          controlBlock = reinterpret_cast<imaging::SControlBlock *>(lwi->data(imaging::ROLE_CONTROLBLOCK).toULongLong());
//          controlBlock->outputImage.astroFile->save();
        };
      };

      dlg->accept();
    }

    /// @brief Select all the aligned files.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2013-06-13/GGB - Function created.

    void CDialogSaveAligned::eventSelectAligned(bool)
    {
      int rows = listWidgetAligned->count();
      int index;

      for (index = 0; index < rows; ++index)
      {
        listWidgetAligned->item(index)->setCheckState(Qt::Checked);
      };
    }

    /// @brief Select all the original files.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2013-06-13/GGB - Function created.

    void CDialogSaveAligned::eventSelectOriginal(bool)
    {
      int rows = listWidgetOriginal->count();
      int index;

      for (index = 0; index < rows; ++index)
      {
        listWidgetOriginal->item(index)->setCheckState(Qt::Checked);
      };
    }

    /// @brief Unchecks all the aligned files.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2013-06-13/GGB - Function created.

    void CDialogSaveAligned::eventUnselectAligned(bool)
    {
      int rows = listWidgetAligned->count();
      int index;

      for (index = 0; index < rows; ++index)
      {
        listWidgetAligned->item(index)->setCheckState(Qt::Unchecked);
      }
    }

    /// @brief Unchecks all the original files.
    /// @param None.
    /// @returns None.
    /// @throws None.
    /// @version 2013-06-13/GGB - Function created.

    void CDialogSaveAligned::eventUnselectOriginal(bool)
    {
      int rows = listWidgetOriginal->count();
      int index;

      for (index = 0; index < rows; ++index)
      {
        listWidgetOriginal->item(index)->setCheckState(Qt::Unchecked);
      };
    }

    /// @brief Sets up the user interface.
    /// @param None.
    /// @returns None.
    /// @throws GCL::CRuntimeAssert
    /// @version 2016-04-23/GGB - Update to reflect the use of a single data role and SCcontrolBlock
    /// @version 2015-08-01/GGB - Changed to use macro ASSOCIATE_CONTROL
    /// @version 2013-06-12/GGB - Function created.

    void CDialogSaveAligned::setupUI()
    {
      imaging::SControlBlock *controlBlock;
      QDialogButtonBox *buttonBox;

      ASSOCIATE_CONTROL(listWidgetOriginal, dlg, "listWidgetOriginal", QListWidget);
      ASSOCIATE_CONTROL(listWidgetAligned, dlg, "listWidgetAligned", QListWidget);

      ASSOCIATE_CONTROL(pushButtonSelectOriginal, dlg, "pushButtonSelectOriginal", QPushButton);
      ASSOCIATE_CONTROL(pushButtonUnselectOriginal, dlg, "pushButtonUnselectOriginal", QPushButton);
      ASSOCIATE_CONTROL(pushButtonSelectAligned, dlg, "pushButtonSelectAligned", QPushButton);
      ASSOCIATE_CONTROL(pushButtonUnselectAligned, dlg, "pushButtonUnselectAligned", QPushButton);

      ASSOCIATE_CONTROL(buttonBox, dlg, "buttonBox", QDialogButtonBox);

      connect(pushButtonSelectOriginal, SIGNAL(clicked(bool)), this, SLOT(eventSelectOriginal(bool)));
      connect(pushButtonUnselectOriginal, SIGNAL(clicked(bool)), this, SLOT(eventUnselectOriginal(bool)));
      connect(pushButtonSelectAligned, SIGNAL(clicked(bool)), this, SLOT(eventSelectAligned(bool)));
      connect(pushButtonUnselectAligned, SIGNAL(clicked(bool)), this, SLOT(eventUnselectAligned(bool)));
      connect(buttonBox, SIGNAL(accepted()), this, SLOT(eventButtonSave()));
      connect(buttonBox, SIGNAL(rejected()), this, SLOT(eventButtonCancel()));

      int index, rows = listWidgetImages->count();

      for (index = 0; index < rows; ++index)
      {
        QListWidgetItem *lwi = listWidgetImages->item(index);
        controlBlock = reinterpret_cast<imaging::SControlBlock *>(lwi->data(imaging::ROLE_CONTROLBLOCK).toULongLong());
        QListWidgetItem *newItem;

          // Data for the original images.

        newItem = new QListWidgetItem(controlBlock->inputFilename);
        newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        newItem->setData(imaging::ROLE_CONTROLBLOCK, QVariant( (qulonglong) controlBlock));
        if (controlBlock->inputImage.astroFile->isDirty())
        {
          newItem->setCheckState(Qt::Checked);
        }
        else
        {
          newItem->setCheckState(Qt::Unchecked);
        };
        listWidgetOriginal->addItem(newItem);

        if (controlBlock->outputImageValid)
        {
          newItem = new QListWidgetItem(controlBlock->outputFilename);
          newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
          newItem->setData(imaging::ROLE_CONTROLBLOCK, QVariant( (qulonglong) controlBlock));
          if (controlBlock->outputImage.astroFile->isDirty())
          {
              newItem->setCheckState(Qt::Checked);
          }
          else
          {
            newItem->setCheckState(Qt::Unchecked);
          };
          listWidgetAligned->addItem(newItem);
        };
      };
    }

  }  // namespace dialogs
} // namespace AstroManager
