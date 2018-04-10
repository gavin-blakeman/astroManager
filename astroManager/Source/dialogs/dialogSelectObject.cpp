//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogSelectObjects
// SUBSYSTEM:						Object Name Selection
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2016 Gavin Blakeman.
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
// OVERVIEW:            This dialog provides a means for the user to select an object name. There are three methods of entering data
//                      for the object name.
//                      1. Enter the object name directly. (There is no data validation in this model)
//                      2. Enter the object name and verify/expand against the SIMBAD database.
//                      3. Select the object name from the ATID database.
//
// CLASSES INCLUDED:    CSelectObjectDialog
//
// CLASS HIERARCHY:     CDialog
//                        - CSelectObjectDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-03-17 GGB - Removed CSelectObjectDialog into file DialogSelectObject.
//                      2013-01-20 GGB - AIRDAS 0000.00.000 release.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogSelectObject.h"

  // AIRDAS includes

#include "../../Include/database/database.h"
#include "../../Include/database/databaseATID.h"
#include "../../Include/Settings.h"

namespace AstroManager
{
  namespace dialogs
  {
    //*****************************************************************************************************************************
    //
    // CSelectObjectDialog
    //
    //*****************************************************************************************************************************

    int CSelectObjectDialog::startTab = 0;

    // Class constructor.
    //
    // 2013-07-29/GGB - Code moved to setupUI() when updating class to handle three input methods.
    // 2011-06-30/GGB - Function created.

    CSelectObjectDialog::CSelectObjectDialog(QString &nameID, QWidget *parent) : CDialog(":/forms/dialogSelectObject.ui", parent),
      nameID_(nameID)
    {
      setupUI();
    }

    /// @brief When the user requests a search, this function performs the search and displays the results.
    /// @details The SQL query needs to be constructed using the data the user has set up.
    /// @version 2013-05-10/GGB - Function created.

    void CSelectObjectDialog::eventBtnSearchClicked(bool)
    {
      if (settings::VSOPSettings->value(settings::ATID_DATABASE_USESIMBAD, QVariant(true)).toBool())
      {
        searchSIMBAD();
      }
      else
      {
        searchATID();
      }
    }

    // Handles the cancel button click.
    //
    // 2010-11-17/GGB - Function created.

    void CSelectObjectDialog::eventButtonCancelClicked()
    {
      startTab = tabWidget->currentIndex();

      dlg->reject();
    }

    /// @brief Handles the select click. Calls the appropriate select handler based on the current tab.
    //
    // 2013-07-29/GGB - Added support for manual name entry.
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::eventButtonSelectClicked()
    {
      startTab = tabWidget->currentIndex();

      switch (startTab)
      {
      case 0:
        selectManual();
        break;
      case 1:
        selectATID();
        break;
      case 2:
        selectSIMBAD();
        break;
      case 3:
        selectRecent();
        break;
      default:
        CODE_ERROR(AIRDAS);
      }
    }

    // Handles the check event from the Constellation Check
    //
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::eventCheckConstellationChanged(int newState)
    {
      if (newState == Qt::Checked)
        comboConstellation->setEnabled(true);
      else
        comboConstellation->setEnabled(false);
    }

    // Handles the check event from the ObjectType check box.
    //
    // 2010-11-14/GGB - Function created.

    void CSelectObjectDialog::eventCheckObjectTypeChanged(int newState)
    {
      if (newState == Qt::Checked)
        comboObjectType->setEnabled(true);
      else
        comboObjectType->setEnabled(false);
    }

    // Enables the selection button when an item is selected.
    //
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::eventTableObjectsCellClicked(int, int)
    {
      //buttonSelect->setEnabled(true);
    }

    // Enables the selection button when an item is selected.
    //
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::eventTableRecentCellClicked(int, int)
    {
      //buttonSelect->setEnabled(true);
    }

    /// Populates the recent object table.
    //
    // 2013-05-10/GGB - Added functionality to use SIMBAD or the ATID database.
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::PopulateRecentObjects(void)
    {
      int nRow, nColumn;
      QString szKey;

      if (!database::databaseATID->usingATID())
      {
          // Not using ATID. Don't lookup and information on recent objects.

        for (nRow = 0; nRow < settings::RECENT_COUNT; nRow++)
        {
          szKey = QString(settings::DIALOG_SELECTOBJECT_RECENT).arg(nRow+1);

          nColumn = 0;
          tableRecentObjects->insertRow(nRow);
          tableRecentObjects->setRowHeight(nRow, 20);

          tableRecentObjects->setItem(nRow, nColumn, new QTableWidgetItem(settings::VSOPSettings->value(szKey, QVariant("")).toString()));
        };
      }
      else
      {
          // Using ATID. Lookup information on recent objects.

        QString szSQL;
        QSqlQuery query(database::databaseATID->database());

        for (nRow = 0; nRow < settings::RECENT_COUNT; nRow++)
        {
          szKey = QString(settings::DIALOG_SELECTOBJECT_RECENT).arg(nRow+1);

          szSQL = QString("SELECT TBL_NAMES.Name, TBL_OBJECTTYPES.ShortText, TBL_CONSTELLATIONS.LONGTEXT, TBL_STELLAROBJECTS.[V-Magnitude], TBL_SPECTRALCLASS.ShortText, TBL_CATALOGUE.CATALOGUENAME, TBL_NAMES.NAME_ID " \
                          "FROM (TBL_OBJECTTYPES INNER JOIN (TBL_CONSTELLATIONS INNER JOIN (TBL_SPECTRALCLASS INNER JOIN TBL_STELLAROBJECTS ON TBL_SPECTRALCLASS.SPECTRALCLASS_ID = TBL_STELLAROBJECTS.SPECTRALCLASS_ID) ON TBL_CONSTELLATIONS.CONSTELLATION_ID = TBL_STELLAROBJECTS.Constellation_ID) ON TBL_OBJECTTYPES.OBJECTTYPE_ID = TBL_STELLAROBJECTS.ObjectType_id) INNER JOIN (TBL_CATALOGUE INNER JOIN TBL_NAMES ON TBL_CATALOGUE.CATALOGUE_ID = TBL_NAMES.CATALOGUE_ID) ON TBL_STELLAROBJECTS.OBJECT_ID = TBL_NAMES.STELLAROBJECT_ID " \
                          "WHERE (((TBL_NAMES.Name)='%1'))").arg(settings::VSOPSettings->value(szKey, "").toString());
          query.exec(szSQL);
          query.first();

          nColumn = 0;
          tableRecentObjects->insertRow(nRow);
          tableRecentObjects->setRowHeight(nRow, 20);

          tableRecentObjects->setItem(nRow, nColumn, new QTableWidgetItem(*new QString(query.value(0).toString())));
          tableRecentObjects->item(nRow, nColumn++)->setData(Qt::UserRole, *new QVariant(query.value(6)));
          tableRecentObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(1).toString())));
          tableRecentObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(2).toString())));
          tableRecentObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(QString("%1").arg(query.value(3).toDouble(), 4, 'f', 3, '0'))));
          tableRecentObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(4).toString())));
          tableRecentObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(5).toString())));
        };
      };
    }

    /// Function to search the ATID database.
    //
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::searchATID()
    {
      QString szSQL;
      QString szWhile = "";
      QString szName;
      bool bWhile = false;
      QSqlQuery query(database::databaseATID->database());
      QSqlQuery queryName(database::databaseATID->database());
      int nRow, nColumn;
      QString szListItem;
      const int rowHeight = 20;

        // Clear out the table first

      tableObjects->clearContents();
      for (nRow = tableObjects->rowCount() -1; nRow >= 0; nRow--)
        tableObjects->removeRow(nRow);

      if (editObjectName->text().length() != 0)
      {
              // There is an object name present.

        szSQL = QString( \
          "SELECT TBL_NAMES.NAME, TBL_CATALOG.CATALOG_NAME, TBL_OBJECTTYPES.OBJECTTYPE, TBL_CONSTELLATIONS.CONSTELLATIONNAME, TBL_SPECTRALTYPES.ShortText, TBL_STELLAROBJECTS.VMAGNITUDE, TBL_NAMES.NAME_ID  " \
          "FROM (TBL_SPECTRALTYPES INNER JOIN (TBL_OBJECTTYPES INNER JOIN ((TBL_CATALOG INNER JOIN TBL_NAMES ON TBL_CATALOG.CATALOG_ID = TBL_NAMES.CATALOG_ID) INNER JOIN TBL_STELLAROBJECTS ON TBL_NAMES.STELLAROBJECT_ID = TBL_STELLAROBJECTS.OBJECT_ID) ON TBL_OBJECTTYPES.OBJECTTYPE_ID = TBL_STELLAROBJECTS.OBJECTTYPE_ID) ON TBL_SPECTRALTYPES.SPECTRALTYPE_ID = TBL_STELLAROBJECTS.SPECTRALTYPE_ID) INNER JOIN TBL_CONSTELLATIONS ON TBL_STELLAROBJECTS.Constellation_ID = TBL_CONSTELLATIONS.CONSTELLATION_ID ");

        if (checkObjectType->checkState() == Qt::Checked)
        {
          szWhile = QString("(TBL_OBJECTTYPES.OBJECTTYPE='%1')").arg(comboObjectType->currentText());
          bWhile = true;
        };

        if (checkConstellation->checkState() == Qt::Checked)
        {
          if (bWhile)
            szWhile += " AND ";
          else
            bWhile = true;
          szWhile += QString("(TBL_CONSTELLATIONS.CONSTELLATIONNAME='%1')").arg(comboConstellation->currentText());
        };

        if (checkMultiple->checkState() == Qt::Checked)
        {
          if (bWhile)
            szWhile += " AND ";
          else
            bWhile = true;
          szWhile += QString("(TBL_STELLAROBJECTS.MULTIPLE <> 0)");
        };

        if (checkExoPlanets->checkState() == Qt::Checked)
        {
          if (bWhile)
            szWhile += " AND ";
          else
            bWhile = true;
          szWhile += QString("(TBL_STELLAROBJECTS.EXOPLANETS <> 0)");
        };

        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;

        szWhile += QString("(TBL_NAMES.NAME Like '%1')").arg(editObjectName->text());

        if (bWhile)
          szSQL += "WHERE " + szWhile;

        szSQL += " LIMIT 100";

        query.exec(QString(szSQL));

          // Add all the selected items to the list box

        nRow = 0;
        while ( (query.next()) && (nRow < 100))
        {
          nColumn = 0;
          tableObjects->insertRow(nRow);
          tableObjects->setRowHeight(nRow, rowHeight);
          tableObjects->setItem(nRow, nColumn, new QTableWidgetItem(*new QString(query.value(0).toString())));
          tableObjects->item(nRow, nColumn++)->setData(Qt::UserRole, *new QVariant(query.value(6)));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(2).toString())));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(3).toString())));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(QString("%1").arg(query.value(5).toDouble(), 4, 'f', 3, '0'))));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(4).toString())));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(1).toString())));
          nRow++;
        };
      }
      else
      {
          // No name text selected.
          // Search for the other information and display the preferred names to ensure only one entry per object.

        szSQL = "SELECT TBL_OBJECTTYPES.SHORTTEXT, TBL_CONSTELLATIONS.LONGTEXT, TBL_SPECTRALCLASS.SHORTTEXT, TBL_STELLAROBJECTS.[V-MAGNITUDE], TBL_STELLAROBJECTS.PREFERREDNAME, TBL_STELLAROBJECTS.OBJECT_ID "\
                "FROM TBL_OBJECTTYPES INNER JOIN (TBL_CONSTELLATIONS INNER JOIN (TBL_SPECTRALCLASS INNER JOIN TBL_STELLAROBJECTS ON TBL_SPECTRALCLASS.SPECTRALCLASS_ID = TBL_STELLAROBJECTS.SPECTRALCLASS_ID) ON TBL_CONSTELLATIONS.CONSTELLATION_ID = TBL_STELLAROBJECTS.CONSTELLATION_ID) ON TBL_OBJECTTYPES.OBJECTTYPE_ID = TBL_STELLAROBJECTS.OBJECTTYPE_ID ";


        if (checkObjectType->checkState() == Qt::Checked)
        {
          szWhile = QString("(TBL_OBJECTTYPES.ShortText='%1')").arg(comboObjectType->currentText());
          bWhile = true;
        };

        if (checkConstellation->checkState() == Qt::Checked)
        {
          if (bWhile)
            szWhile += " AND ";
          else
            bWhile = true;
          szWhile += QString("(TBL_CONSTELLATIONS.CONSTELLATIONNAME='%1')").arg(comboConstellation->currentText());
        };

        if (checkMultiple->checkState() == Qt::Checked)
        {
          if (bWhile)
            szWhile += " AND ";
          else
            bWhile = true;
          szWhile += QString("(TBL_STELLAROBJECTS.ISMULTIPLE <> 0)");
        };

        if (checkExoPlanets->checkState() == Qt::Checked)
        {
          if (bWhile)
            szWhile += " AND ";
          else
            bWhile = true;
          szWhile += QString("(TBL_STELLAROBJECTS.ISEXOPLANETS <> 0)");
        };

        if (bWhile)
          szSQL += "WHERE " + szWhile;

        query.exec(QString(szSQL));

          // Add all the selected items to the list box

        nRow = 0;
        while ( (query.next()) && (nRow < 100))
        {
            // Get the name of the object.

          if (query.value(4).isNull())
          {		// No Preferred name. Use the name from the sor order.

  //					if (checkCatalog->checkState() == Qt::Checked)
  //					{
  //						if (bWhile)
  //							szWhile += " AND ";
  //						else
  //							bWhile = true;
  //						szWhile += QString("(TBL_CATALOGUE.[CATALOGUE NAME]='%1')").arg(comboCatalog->currentText());
  //					};

            szName = QString("SELECT TBL_NAMES.Name, TBL_CATALOGUE.CATALOGUENAME, TBL_NAMES.NAME_ID " \
                             "FROM (TBL_CATALOGUE INNER JOIN TBL_NAMES ON TBL_CATALOGUE.CATALOGUE_ID = TBL_NAMES.CATALOGUE_ID) INNER JOIN TBL_CATALOGUEORDER ON TBL_CATALOGUE.CATALOGUE_ID = TBL_CATALOGUEORDER.CATALOGUE_ID " \
                             "WHERE (((TBL_CATALOGUEORDER.OBSERVER_ID)=%1) AND ((TBL_NAMES.STELLAROBJECT_ID)=%2)) " \
                             "ORDER BY TBL_CATALOGUEORDER.SORTORDER").arg(settings::VSOPSettings->value(settings::SETTINGS_OBSERVER, QString("1")).toString()).arg(query.value(5).toLongLong());
          }
          else
          {		// Preferred name.
            szName = QString("SELECT TBL_NAMES.Name, TBL_CATALOGUE.CATALOGUENAME, TBL_NAMES.NAME_ID " \
                             "FROM TBL_CATALOGUE INNER JOIN TBL_NAMES ON TBL_CATALOGUE.CATALOGUE_ID = TBL_NAMES.CATALOGUE_ID " \
                             "WHERE TBL_NAMES.NAME_ID = %1").arg(query.value(4).toLongLong());
          };

          queryName.exec(szName);
          queryName.first();

          nColumn = 0;
          tableObjects->insertRow(nRow);
          tableObjects->setRowHeight(nRow, rowHeight);
          tableObjects->setItem(nRow, nColumn, new QTableWidgetItem(*new QString(queryName.value(0).toString())));
          tableObjects->item(nRow, nColumn++)->setData(Qt::UserRole, *new QVariant(queryName.value(2)));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(0).toString())));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(1).toString())));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(QString("%1").arg(query.value(3).toDouble(), 4, 'f', 3, '0'))));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(query.value(2).toString())));
          tableObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(queryName.value(1).toString())));
          nRow++;
        };
      };
    }

    // Handles the select click.
    //
    // 2011-06-30/GGB - Function created.

    void CSelectObjectDialog::selectATID()
    {
      int nRow;
      QString szString;

      startTab = tabWidget->currentIndex();

      switch (startTab)
      {
      case 0:	// From List
        nRow = tableObjects->currentRow();
        //nameID_ = tableObjects->item(nRow, 0)->data(Qt::UserRole);
        szString = tableObjects->item(nRow, 0)->text();
        break;
      case 1:
        nRow = tableRecentObjects->currentRow();
        //vNameID = tableRecentObjects->item(nRow, 0)->data(Qt::UserRole);
        szString = tableRecentObjects->item(nRow, 0)->text();
        break;
      };

        // Update the recent objects list.

      settings::InsertRecentObject(szString);

      dlg->accept();
    }

    /// Function to handle the selectManual operation. Returns the name of the object with no error checking, or validation
    /// of the name.
    //
    // 2013-07-29/GGB - Function created.

    void CSelectObjectDialog::selectManual()
    {
      QString szString = lineEditObjectName->text();

      if (!szString.isNull() && !szString.isEmpty())
      {
        nameID_ = szString;
        dlg->accept();
      }
    }

    /// Function to handle the select button when the SIMBAD option is in use.
    //
    // 2013-05-10/GGB - Function created.

    void CSelectObjectDialog::selectSIMBAD()
    {
      startTab = tabWidget->currentIndex();
      bool canExit = false;

      switch (startTab)
      {
      case 0: // From SIMBAD
        if (!editObjectName->text().isEmpty())
        {
          nameID_ = editObjectName->text();
          canExit = true;
        };
        break;
      case 1: // From recent.
        if (tableRecentObjects->currentRow() != -1)
        {
          nameID_ = tableRecentObjects->currentItem()->text();
          canExit = true;
        };
      };

      if (canExit)
        dlg->accept();
    }

    /// Function to set the dialog UI up.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2013-08-11/GGB - Added code to initialise the object Name. (Bug #1210914)
    /// @version 2013-07-29/GGB - Function created.

    void CSelectObjectDialog::setupUI()
    {
      int nIndex = 0;

      tabWidget = dlg->findChild<QTabWidget *>("tabWidget");
        tabManual = tabWidget->widget(nIndex++);
          lineEditObjectName = dlg->findChild<QLineEdit *>("lineEditObjectName");
        tabATID = tabWidget->widget(nIndex++);
          checkObjectType = dlg->findChild<QCheckBox *>("checkObjectType");
          comboObjectType = dlg->findChild<QComboBox *>("comboObjectType");
          checkConstellation = dlg->findChild<QCheckBox *>("checkConstellation");
          comboConstellation = dlg->findChild<QComboBox *>("comboConstellation");
          checkMultiple = dlg->findChild<QCheckBox *>("checkMultiple");
          checkExoPlanets = dlg->findChild<QCheckBox *>("checkExoPlanets");
          editObjectName = dlg->findChild<QLineEdit *>("editObjectName");
          tableObjects = dlg->findChild<QTableWidget *>("tableObjects");
          btnSearch = dlg->findChild<QPushButton *>("btnSearch");
        tabSIMBAD = tabWidget->widget(nIndex++);
        tabRecent = tabWidget->widget(nIndex++);
          tableRecentObjects = dlg->findChild<QTableWidget *>("tableRecentObjects");

      buttonBox = dlg->findChild<QDialogButtonBox *>("buttonBox");
      if (!buttonBox)
      {
        CODE_ERROR(AIRDAS);
      }

      connect(buttonBox, SIGNAL(accepted()), this, SLOT(eventButtonSelectClicked()));
      connect(buttonBox, SIGNAL(rejected()), this, SLOT(eventButtonCancelClicked()));

      tabWidget->setTabEnabled(1, false);
      tabWidget->setTabEnabled(2, false);

      if (database::databaseATID->usingATID())
      {

        tabWidget->setTabEnabled(1, true);

          // Populate combo boxes.

        database::databaseATID->PopulateObjectTypeCombo(comboObjectType);
        database::databaseATID->PopulateConstellationsCombo(comboConstellation);

        connect(btnSearch, SIGNAL(clicked(bool)), this, SLOT(eventBtnSearchClicked(bool)));
        connect(checkObjectType, SIGNAL(stateChanged(int)), this, SLOT(eventCheckObjectTypeChanged(int)));
        connect(checkConstellation, SIGNAL(stateChanged(int)), this, SLOT(eventCheckConstellationChanged(int)));

        connect(tableObjects, SIGNAL(cellClicked(int, int)), this, SLOT(eventTableObjectsCellClicked(int, int)));
      }
      else if (database::databaseATID->usingSIMBAD())
      {
        tabWidget->setTabEnabled(2, true);
      }
      else
      {
      };

      lineEditObjectName->setText(nameID_);

      connect(tableRecentObjects, SIGNAL(cellClicked(int, int)), this, SLOT(eventTableRecentCellClicked(int, int)));

        // Set all unselected combo boxes to disabled.

      comboObjectType->setEnabled(false);
      comboConstellation->setEnabled(false);

      tabWidget->setCurrentIndex(startTab);

      PopulateRecentObjects();
    }

  }  // namespace dialogs
}  // namespace AstroManager
