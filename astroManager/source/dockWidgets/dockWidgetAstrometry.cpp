//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetAstrometry
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
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
//
// OVERVIEW:
//
// CLASSES INCLUDED:    CAstrometryDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into this file
//
//*********************************************************************************************************************************

#include "../../include/dockWidgets/dockWidgetAstrometry.h"

  // astroManager Include files

#include "../../include/database/databaseATID.h"
#include "../../include/dialogs/dialogSelectObject.h"
#include "../../include/ImageComparison.h"
#include "../../include/windowImage/windowImageDisplay.h"
#include "../../include/qtExtensions/MdiSubWindow.h"
#include "../../include/settings.h"

  // Miscellaneous library header files

#include <QCL>

namespace astroManager
{
  namespace dockwidgets
  {

    //*****************************************************************************************************************************
    //
    // CAstromentryImageDockWidget
    //
    //*****************************************************************************************************************************

#define ROLE_OBJECTINDEX  Qt::UserRole + 0

    /// @brief Class constructor.
    /// @param[in] action: Pointer to the action
    /// @param[in] parent: Pointer to the parent object.
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of parameters. Updated heirarchy for weather window.
    /// @version 2013-07-27/GGB - Added objectName.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-06-12/GGB - Function created.

    CAstrometryDockWidget::CAstrometryDockWidget(QWidget *parent, QAction *action) :
      CDockWidgetImage(DW_ASTROMETRY_NAME, parent, action, settings::DW_IMAGE_ASTROMETRY_VISIBLE), bReference(false)
    {
      parentObject = dynamic_cast<mdiframe::CFrameWindow *>(parent);
      setupUI();
      setObjectName(DW_ASTROMETRY_NAME);
    }

    /// @brief Adds a new object into the object list.
    /// @param[in] newObject - The new object being created.
    /// @throws None.
    /// @version 2015-01-04/GGB - Function created.

    void CAstrometryDockWidget::addNewObject(astrometry::PAstrometryObservation newObject)
    {
      // Add the object to the table view.

      insertRow(tableWidgetAstrometry->rowCount(), newObject);
    }

    /// @brief Displays astrometry information when an astrometry object is selected.
    /// @param[in] ao - The astrometry observation to display the data for.
    /// @throws
    /// @version

    void CAstrometryDockWidget::displayAstrometry(astrometry::PAstrometryObservation ao)
    {
    }

    /// @brief Procedure called when an image is activating.
    /// @throws None.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-06-26/GGB - Function created.

    void CAstrometryDockWidget::imageActivating()
    {
      redraw();
    }

    /// @brief Procedure called when the image that the dock widget refers to has been changed.
    /// @throws None.
    /// @version 2013-08-10/GGB - Function created.

    void CAstrometryDockWidget::imageChanged()
    {
      redraw();
    }

    /// @brief  Procedure called when an image is deactivating.
    /// @throws
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-06-26/GGB - Function created.

    void CAstrometryDockWidget::imageDeactivating()
    {
      redraw();
    }

    /// @brief Allows the user to select an object to associate with the currently selected reference or target.
    /// @throws C
    // 2013-08-11/GGB - 1) Added code to initialise the name of the object. (Bug #1210914)
    //                  2) Added code to update dirty status of astroFile. (Bug #1210749)
    // 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    // 2011-06-27/GGB - Function created.

    void CAstrometryDockWidget::eventAssociateObject(bool)
    {
      int nRow;

      if (bReference)
      {
          // Get the current name of the object.

        nRow = tableWidgetAstrometry->currentRow();
        QString szName = QString::fromStdString(currentImage->astrometryObservations[nRow]->objectName());

        dialogs::CSelectObjectDialog *dlg = new dialogs::CSelectObjectDialog(szName, this);

        if (dlg->exec() == QDialog::Accepted)
        {
            // Update the string in the table, as well as the displayed string on the image
            // as well as the strings for the object information.

          nRow = tableWidgetAstrometry->currentRow();

          currentImage->astrometryObservations[nRow]->objectName(szName.toStdString());

          tableWidgetAstrometry->item(nRow, 0)->setText(szName);

          labelObjectName->setText(szName);

          //libAstroClass::sprintfHMS(szNumber, libAstroClass::dms2deg(query.value(1).toDouble() / 10000), 4);
          //labelObjectRA->setText(QString(szNumber));

          //libAstroClass::sprintfDMS(szNumber, libAstroClass::dms2deg(query.value(2).toDouble() / 10000), true, 3);
          //labelObjectDec->setText(QString(szNumber));

          //labelObjectType->setText(query.value(3).toString());

          //if (astrometry->astrometryReferences.size() >= 3)
          //  pushButtonPlateConstants->setEnabled(true);

          currentImage->astroFile->isDirty(true);

          if (currentImage->parent_->getWindowType() == SWT_IMAGEWINDOW)
          {
            imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(currentImage->parent_);
            if (!iw)
            {
              CODE_ERROR(astroManager);
            }

            iw->repaintImage();
            iw->updateWindowTitle();
          }
          else if (currentImage->parent_->getWindowType() == SWT_IMAGE_COMPARE)
          {
            dynamic_cast<imaging::CImageComparisonWindow *>(currentImage->parent_)->redrawImage();
          }
          else
          {
            CODE_ERROR(astroManager);   // Dock widget should be grayed.
          }
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// Allows the user to delete a reference object from the list of objects.
    //
    // 2015-01-01/GGB - Added code to delete the text and group and also to reset the current selection. (Bug #1406897)
    // 2013-08-11/GGB - Added code to delete the reference from the astroFile. (Bug #1210750)
    // 2011-06-29/GGB - Function created.

    void CAstrometryDockWidget::eventButtonReferenceDelete(bool)
    {
      int nRow;
      QGraphicsScene *scene;

      nRow = tableWidgetAstrometry->currentRow();

      if (nRow != -1 && currentImage)
      {
        // Delete the object and annotation from the scene.

        scene = currentImage->astrometryObservations[nRow]->group->scene();
        scene->removeItem(currentImage->astrometryObservations[nRow]->group);
        delete currentImage->astrometryObservations[nRow]->group;
        currentImage->astrometryObservations[nRow]->group = nullptr;
        currentImage->astrometryObservations[nRow]->text = nullptr;

          // Delete the object from the list.

        currentImage->currentAstrometrySelection.reset();   // Remove the selection link.

        currentImage->astroFile->astrometryObjectRemove(currentImage->astrometryObservations[nRow]->objectName());
        currentImage->astrometryObservations.erase(currentImage->astrometryObservations.begin() + nRow);

        tableWidgetAstrometry->removeRow(nRow);

        pushButtonObjectInformation->setEnabled(false);
        pushButtonReferenceEdit->setEnabled(false);
        pushButtonReferenceDelete->setEnabled(false);

        currentImage->astroFile->isDirty(true);

        if (currentImage->parent_->getWindowType() == SWT_IMAGEWINDOW)
        {
          imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(currentImage->parent_);
          if (!iw)
            CODE_ERROR(astroManager);

          iw->updateWindowTitle();
        };
      }
      else
        CODE_ERROR(astroManager);
    }

    /// @briefFunction called when the user wishes to reference an object on an image.
    /// @details Sends a message to the active child window (if it is the correct type of window) to allow the image to be
    ///          referenced.
    /// @param[in] toReference - Indicates if the button has been pressed.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-08-04/GGB - Added support for the comparison image window.
    /// @version 2011-06-14/GGB - Function created.

    void CAstrometryDockWidget::eventButtonReferenceSelect(bool toReference)
    {
      CMdiSubWindow *subWindow;
      mdiframe::CFrameWindow *frameWindow;

      frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(parentObject);
      subWindow = dynamic_cast<CMdiSubWindow *>(frameWindow->activeMdiChild());

      if (toReference)
      {
          // Let the image know that it is now going to reference an object.

        if (subWindow->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_ASTROMETRY);
        }
        else if (subWindow->getWindowType() == SWT_IMAGE_COMPARE)
        {
          dynamic_cast<imaging::CImageComparisonWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_ASTROMETRY);
        }
        else
        {
          CODE_ERROR(astroManager);   // Dock widget should be grayed.
        }
      }
      else
      {
        if (subWindow->getWindowType() == SWT_IMAGEWINDOW)
        {
          dynamic_cast<imaging::CImageWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_NONE);
        }
        else if (subWindow->getWindowType() == SWT_IMAGE_COMPARE)
        {
          dynamic_cast<imaging::CImageComparisonWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_NONE);
        }
        else
        {
          CODE_ERROR(astroManager);   // Dock widget should be grayed.
        };
      };
    }

    /// Calculates the plate constants. Displays the relevant error message if an error occurs.
    //
    // 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    // 2012-01-13/GGB - Function created.

    void CAstrometryDockWidget::eventPlateConstants(bool)
    {
      /*QMessageBox msgBox;

      try
      {
        currentImage->astrometryClass.calculatePlateConstants();
      }
      catch (libAstroClass::CACLError &error)
      {
        msgBox.setText(tr("Error while calculating plate constants."));
        msgBox.setInformativeText(QString(error.errorMessage().c_str()));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
      };*/
    }

    /// @brief A row in the table has been selected. Display the appropriate data and unhide the buttons.
    /// @param[in] nRow - The row that has been selected.
    /// @throws None.
    /// @version 2017-09-23/GGB - Updated to use CAngle.
    /// @version 2013-08-17/GGB - Corrected RA/Dec display (Bug #1213076)
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-06-27/GGB - Function created.

    void CAstrometryDockWidget::eventTableRowReferenceSelected(int nRow, int)
    {
      pushButtonObjectInformation->setEnabled(true);
      pushButtonReferenceEdit->setEnabled(true);
      pushButtonReferenceDelete->setEnabled(true);

      astrometry::PAstrometryObservation &ao(currentImage->astrometryObservations[nRow]);

      bReference = true;

      if (ao->objectName().empty())
      {
          // No object associated with the referenced position yet, just display blank data.

        labelObjectName->setText("No Name");
        labelCCDCoordinates->setText(QString("(%1, %2)").arg(ao->CCDCoordinates().x()).
                                     arg(ao->CCDCoordinates().y()));
        labelObjectRA->setText("--h--'--""");
        labelObjectDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectType->setText("");
      }
      else
      {
          // Search for the object information and display the raw object information. Do not correct RA/DEC for refraction etc.

      //  szSQL = QString( \
      //    "SELECT TBL_NAMES.NAME_ID, TBL_NAMES.Name, TBL_STELLAROBJECTS.RA, TBL_STELLAROBJECTS.DEC, TBL_OBJECTTYPES.OBJECTTYPE " \
      //    "FROM TBL_OBJECTTYPES INNER JOIN (TBL_NAMES INNER JOIN TBL_STELLAROBJECTS ON TBL_NAMES.STELLAROBJECT_ID = TBL_STELLAROBJECTS.OBJECT_ID) ON TBL_OBJECTTYPES.OBJECTTYPE_ID = TBL_STELLAROBJECTS.OBJECTTYPE_ID " \
      //    "WHERE TBL_NAMES.NAME_ID=%1").arg(astrometry->astrometryReferences[nRow].vNameID.toString());

      //  query.exec(szSQL);
      //  query.first();
        labelObjectName->setText(QString::fromStdString(currentImage->astrometryObservations[nRow]->objectName()));
        labelCCDCoordinates->setText(QString("(%1, %2)").arg(currentImage->astrometryObservations[nRow]->CCDCoordinates().x()).
                                     arg(currentImage->astrometryObservations[nRow]->CCDCoordinates().y()));
      }

      if (ao->observedCoordinates())
      {
        labelObjectRA->setText(QString::fromStdString(ao->observedCoordinates()->RA().A2SHMS()));
        labelObjectDec->setText(QString::fromStdString(ao->observedCoordinates()->DEC().A2SDMS()));
      }
      else
      {
        labelObjectRA->setText("--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
      };
      currentImage->parent_->changeAstrometrySelection(ao);
    }

    /// @brief Inserts a row into the table.
    //
    // 2015-01-04/GGB - Function created.

    void CAstrometryDockWidget::insertRow(int nRow, astrometry::PAstrometryObservation toInsert)
    {
      ACL::CHDB *currentHDB;

      currentHDB = currentImage->astroFile->getHDB(currentImage->currentHDB).get();

      tableWidgetAstrometry->insertRow(nRow);
      tableWidgetAstrometry->setRowHeight(nRow, 16);
      tableWidgetAstrometry->setItem(nRow, 0, new QTableWidgetItem(*new QString(QString("%1").arg(QString::fromStdString(toInsert->objectName())))));
      tableWidgetAstrometry->setCurrentCell(nRow, 0);
    }

    /// Redraws all the information in the dock widget.
    /// Is called after the astrometry image changes.
    //
    // 2013-03-17/GGB - Changed type of object stored to be descendant of SAstrometryObjectInformation
    // 2013-02-06/GGB - Removed all target code and have only one set of object code.
    // 2011-06-29/GGB - Function created.

    void CAstrometryDockWidget::redraw()
    {
      astrometry::DAstrometryObservationStore::const_iterator iter;
      int nRow;

      if (currentImage)
      {
          // Remove all the lines already in the reference tableWidget

        while (tableWidgetAstrometry->rowCount())
          tableWidgetAstrometry->removeRow(0);

        pushButtonReferenceSelect->setEnabled(true);
        pushButtonReferenceEdit->setEnabled(false);
        pushButtonReferenceDelete->setEnabled(false);
        pushButtonObjectInformation->setEnabled(false);

        bReference = false;

        labelObjectName->setText("");
        labelObjectRA->setText("--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectType->setText("");

        nRow = 0;
        for (iter = currentImage->astrometryObservations.begin(); iter != currentImage->astrometryObservations.end(); iter++)
        {
          tableWidgetAstrometry->insertRow(nRow);

          tableWidgetAstrometry->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString((*iter)->objectName())));
          nRow++;
        };
      }
      else
      {
        while (tableWidgetAstrometry->rowCount())
        {
          tableWidgetAstrometry->removeRow(0);
        };

        pushButtonReferenceSelect->setEnabled(false);
        pushButtonReferenceEdit->setEnabled(false);
        pushButtonReferenceDelete->setEnabled(false);
        pushButtonObjectInformation->setEnabled(false);

        labelObjectName->setText("");
        labelObjectRA->setText("--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectType->setText("");
      };
    }

    /// @brief Function called by image window when a reference has been completed.
    /// @param[in] astrometryObject -
    /// @throws None.
    /// @details
    ///   @li Uncheck the button
    ///   @li Ensure that the user selects an object to go with the reference. \n
    ///   @li Add the new item into the astroFile \n
    ///   @li Add the new item to the tableWidget \n
    /// @version 2017-09-23/GGB - Updated to use CAngle
    /// @version 2013-08-16/GGB - Corrected bug with displaying RA/DEC (Bug #1213076)
    /// @version 2013-08-03/GGB - Moved some code into CImageDisplay to align with the requirements of CImageComparison.
    /// @version 2013-07-18/GGB - Removed variable objectName and corrected code (Bug #1198191)
    /// @version 2013-04-12/GGB - Added code to set the creator and date of a new HDB.
    /// @version 2013-03-17/GGB - Changed type of object stored to be descendant of SAstrometryObjectInformation
    /// @version 2013-02-03/GGB - Moved code from CImageDisplay to this class.
    /// @version 2011-06-26/GGB - Function created.

    void CAstrometryDockWidget::referenceCompleted(astrometry::PAstrometryObservation astrometryObject)
    {
      int nRow = tableWidgetAstrometry->rowCount();

      tableWidgetAstrometry->insertRow(nRow);

      tableWidgetAstrometry->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(astrometryObject->objectName())));
      tableWidgetAstrometry->item(nRow, 0)->setData(ROLE_OBJECTINDEX, QVariant(nRow));

      labelObjectName->setText(QString::fromStdString(astrometryObject->objectName()));
      if (astrometryObject->observedCoordinates())
      {
        labelObjectRA->setText(QString::fromStdString(astrometryObject->observedCoordinates()->RA().A2SHMS()));
        labelObjectDec->setText(QString::fromStdString(astrometryObject->observedCoordinates()->DEC().A2SDMS()));
      }
      else
      {
        labelObjectRA->setText("--" % UTF16_DEGREESIGN % "--'--""");
        labelObjectDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
      };
      labelObjectType->setText(QString("------"));

      try
      {
        if (currentImage->astroFile->astrometryCheckRequisites())
        {
          pushButtonPlateConstants->setEnabled(true);
        };
      }
      catch(...)
      {
      };
    }

    /// @brief Enables the widget.
    /// @param[in] enabledValue - Is the dockWidget enabled or not.
    /// @throws None.
    /// @version 2013-07-18/GGB - Function created.

    void CAstrometryDockWidget::setEnabled(bool enabledValue)
    {
      tableWidgetAstrometry->setEnabled(enabledValue);
      pushButtonReferenceSelect->setEnabled(enabledValue);
      pushButtonReferenceEdit->setEnabled(enabledValue);
      pushButtonReferenceDelete->setEnabled(enabledValue);
      pushButtonObjectInformation->setEnabled(enabledValue);
      pushButtonPlateConstants->setEnabled(enabledValue);
      labelObjectName->setEnabled(enabledValue);
      labelObjectRA->setEnabled(enabledValue);
      labelObjectDec->setEnabled(enabledValue);
      labelObjectType->setEnabled(enabledValue);
    }

    /// @brief Sets up the UI
    /// @throws GCL::CRuntimeAssert(...)
    /// @version 2017-07-10/GGB - Fixed Bug #90.
    /// @version 2013-07-18/GGB - Added check that all the widget members are found.
    /// @version 2013-02-01/GGB - Removed all target referencing information.
    /// @version 2011-06-12/GGB - Function Created.

    void CAstrometryDockWidget::setupUI()
    {
      QUiLoader loader;

      QFile file(":/forms/dwAstrometry.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Failed to open resource :/forms/dwAstrometry.ui");
        ERROR(astroManager, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tableWidgetAstrometry = formWidget->findChild<QTableWidget *>("tableWidgetReference");
      pushButtonReferenceSelect = formWidget->findChild<QPushButton *>("pushButtonReferenceSelect");
      pushButtonReferenceEdit = formWidget->findChild<QPushButton *>("pushButtonReferenceEdit");
      pushButtonReferenceDelete = formWidget->findChild<QPushButton *>("pushButtonReferenceDelete");
      pushButtonObjectInformation = formWidget->findChild<QPushButton *>("pushButtonObjectInformation");
      pushButtonPlateConstants = formWidget->findChild<QPushButton *>("pushButtonPlateConstants");
      labelObjectName = formWidget->findChild<QLabel *>("labelObjectName");
      labelCCDCoordinates = formWidget->findChild<QLabel *>("labelCCDCoordinates");
      labelObjectRA = formWidget->findChild<QLabel *>("labelObjectRA");
      labelObjectDec = formWidget->findChild<QLabel *>("labelObjectDec");
      labelObjectType = formWidget->findChild<QLabel *>("labelObjectType");

      if (!tableWidgetAstrometry || !pushButtonReferenceSelect || !pushButtonReferenceEdit || !pushButtonReferenceDelete ||
          !pushButtonObjectInformation || !pushButtonPlateConstants || !labelObjectName || !labelCCDCoordinates || !labelObjectRA ||
          !labelObjectDec || !labelObjectType)
        CODE_ERROR(astroManager);

      connect(pushButtonReferenceSelect, SIGNAL(clicked(bool)), this, SLOT(eventButtonReferenceSelect(bool)));
      connect(pushButtonReferenceDelete, SIGNAL(clicked(bool)), this, SLOT(eventButtonReferenceDelete(bool)));
      connect(tableWidgetAstrometry, SIGNAL(cellClicked(int, int)), this, SLOT(eventTableRowReferenceSelected(int, int)));
      connect(pushButtonObjectInformation, SIGNAL(clicked(bool)), this, SLOT(eventAssociateObject(bool)));
      connect(pushButtonPlateConstants, SIGNAL(clicked(bool)), this, SLOT(eventPlateConstants(bool)));

      setEnabled(false);
    }

  }  // namespace dockwidgets
}  // namespace AstroManager
