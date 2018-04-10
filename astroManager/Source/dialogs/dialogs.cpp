//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Dialogs.h
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
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
// OVERVIEW:            Including all dialog classes.
//
// CLASSES INCLUDED:    CDialog                   - Virtual base class for dialogs.
//						          CSearchSelectObjectDialog -
//                      CImageFloatDialog         - Allow entry of parameters for floating an image.
//                      CImageResampleDialog      - Allow entry of parameters for resampling an image.
//
// CLASS HIERARCHY:     CDialog
//                        - CSelectLocationDialog
//						            - CSearchSelectObjectDialog
//                        - CImageFloatDialog
//                        - CImageResampleDialog
//                        - CDialogEditResources
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2012-01-28 GGB - Move CImageCalibrationDialog to it's own file.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogs.h"

  // AIRDAS include files

#include "../../Include/database/database.h"
#include "../../Include/database/databaseARID.h"
#include "../../Include/database/databaseATID.h"
#include "../../Include/Settings.h"
#include "../../Include/VSOP.h"

  // Qt Framework

#include <QtSql/QtSql>
#include <QtUiTools/QtUiTools>

  // Standard library

#include <cfloat>
#include <cmath>

  // Boost library

#include "boost/filesystem.hpp"

  // Other libraries

#include <GCL>

namespace AstroManager
{
  namespace dialogs
  {

    //*****************************************************************************************************************************
    //
    // CDialog
    //
    //*****************************************************************************************************************************

    /// Constructor taking no argument. The calling function/derived dialog must load the dialog template.
    //
    // 2013-05-10/GGB - Function created.

    CDialog::CDialog(QObject *parent) : QObject(parent)
    {
    }

    /// @brief Constructor for the CDialog class.
    /// @param[in] szDialog - String value representing the dialog resource name.
    /// @param[in] parent - The parent object
    /// @throws CError(AIRDAS, 0x0001)
    /// @throws CRuntimeAssert
    /// @version 2014-02-10/GGB - Added check and exception after opening the file and loading the dialog.
    /// @version 2010-06-21/GGB - Function created.

    CDialog::CDialog(QString szDialog, QObject *parent) : QObject(parent)
    {
      QUiLoader loader;

      QFile file(szDialog);

      if (!QFile::exists(szDialog))
      {
        ERRORMESSAGE("Resource does not exist: " + szDialog.toStdString());
        ERROR(AIRDAS, 0x0001);
      }

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Could not open the resource: " + szDialog.toStdString());
        ERROR(AIRDAS, 0x0001);
      };

      dlg = static_cast<QDialog *>(loader.load(&file));

      RUNTIME_ASSERT(AIRDAS, dlg != nullptr, "The dialog template could not be loaded.")

      file.close();
    }

    /// @brief Default destructor for the class.
    /// @version 2010-06-21/GGB - Function created.

    CDialog::~CDialog()
    {
    }

    /// @brief Exec function
    /// @version 2010-06-21/GGB - Function created.

    int CDialog::exec()
    {
      return ( dlg->exec() );
    }

  //************************************************************************************************
  //
  // CSearchSelectObjectDialog
  //
  //************************************************************************************************

  int CSearchSelectObjectDialog::startTab = 0;

  /// @brief Class constructor.
  /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
  /// @version 2010-11-27/GGB - Function created.

  CSearchSelectObjectDialog::CSearchSelectObjectDialog(QVariant &nameID, int dlgOptions)
    : CDialog(":/forms/dialogSelectStar.ui"), dialogOptions(dlgOptions), vNameID(nameID)
  {
    int nIndex = 1;

    tabWidget = findChild<QTabWidget *>("tabWidget");
      fromList = tabWidget->widget(nIndex++);

      if (dialogOptions & SSOD_LIST)
      {
        checkObjectType = findChild<QCheckBox *>("checkObjectType");
        comboObjectType = findChild<QComboBox *>("comboObjectType");
        checkSpectralClass = findChild<QCheckBox *>("checkSpectralClass");
        comboSpectralClass = findChild<QComboBox *>("comboSpectralClass");
        checkConstellation = findChild<QCheckBox *>("checkConstellation");
        comboConstellation = findChild<QComboBox *>("comboConstellation");
        checkCatalog = findChild<QCheckBox *>("checkCatalog");
        comboCatalog = findChild<QComboBox *>("comboCatalog");
        checkMultiple = findChild<QCheckBox *>("checkMultiple");
        checkExoPlanets = findChild<QCheckBox *>("checkExoPlanets");
        editObjectName = findChild<QLineEdit *>("editObjectName");
        groupStarType = findChild<QGroupBox *>("groupStarType");
          radioPrimary = findChild<QRadioButton *>("radioPrimary");
          radioJohnson = findChild<QRadioButton *>("radioJohnson");
          radioCousins = findChild<QRadioButton *>("radioCousins");
          radioLandolt = findChild<QRadioButton *>("radioLandolt");
          radioAAVSO = findChild<QRadioButton *>("radioAAVSO");
          radioVariable = findChild<QRadioButton *>("radioVariable");
        tableObjects = findChild<QTableWidget *>("tableObjects");
        btnSearch = findChild<QPushButton *>("btnSearch");
      };
      fromPhotometry = tabWidget->widget(nIndex++);
        comboPhotometry = findChild<QComboBox *>("comboPhotometry");
        tablePhotometryObjects = findChild<QTableWidget *>("tablePhotometryObjects");
      fromRecent = tabWidget->widget(nIndex++);
        tableRecentObjects = findChild<QTableWidget *>("tableRecentObjects");
    buttonSelect = findChild<QPushButton *>("buttonSelect");
    buttonCancel = findChild<QPushButton *>("buttonCancel");
    buttonHelp = findChild<QPushButton *>("buttonHelp");

    connect(buttonSelect, SIGNAL(clicked(bool)), this, SLOT(eventButtonSelectClicked(bool)));
    connect(buttonCancel, SIGNAL(clicked(bool)), this, SLOT(eventButtonCancelClicked(bool)));
    connect(buttonHelp, SIGNAL(clicked(bool)), this, SLOT(eventButtonHelpClicked(bool)));

        // Populate all the relevant combo boxes

    if (dialogOptions & SSOD_LIST)
    {
      database::databaseATID->PopulateObjectTypeCombo(comboObjectType);
      database::databaseATID->PopulateSpectralClassCombo(comboSpectralClass);
      database::databaseATID->PopulateConstellationsCombo(comboConstellation);
      database::databaseATID->PopulateCatalogCombo(comboCatalog);
    };

    if (dialogOptions & SSOD_PHOTOMETRY)
    {
      database::databaseATID->PopulatePhotometryProgramCombo(comboPhotometry);

      eventComboPhotometryCurrenINDEX_tChanged(0);

      connect(comboPhotometry, SIGNAL(currenINDEX_tChanged(int)), this, SLOT(eventComboPhotometryCurrenINDEX_tChanged(int)));
      connect(tablePhotometryObjects, SIGNAL(cellClicked(int, int)), this, SLOT(eventTablePhotometryCellClicked(int, int)));
    };

        // Connect all items to functions

      connect(btnSearch, SIGNAL(clicked(bool)), this, SLOT(eventBtnSearchClicked(bool)));
      connect(checkObjectType, SIGNAL(stateChanged(int)), this, SLOT(eventCheckObjectTypeChanged(int)));
      connect(checkSpectralClass, SIGNAL(stateChanged(int)), this, SLOT(eventCheckSpectralClassChanged(int)));
      connect(checkConstellation, SIGNAL(stateChanged(int)), this, SLOT(eventCheckConstellationChanged(int)));
      connect(checkCatalog, SIGNAL(stateChanged(int)), this, SLOT(eventCheckCatalogChanged(int)));

      connect(tableObjects, SIGNAL(cellClicked(int, int)), this, SLOT(eventTableObjectsCellClicked(int, int)));

      connect(tableRecentObjects, SIGNAL(cellClicked(int, int)), this, SLOT(eventTableRecentCellClicked(int, int)));

        // Set all unselected combo boxes to disabled.

      comboObjectType->setEnabled(false);
      comboSpectralClass->setEnabled(false);
      comboConstellation->setEnabled(false);
      comboCatalog->setEnabled(false);

      tabWidget->setCurrentIndex(startTab);

      PopulateRecentObjects();

  }

  /// @brief When the user requests a search, this function performs the search and displays the results.
  /// @details The SQL query needs to be constructed using the data the user has set up.
  //
  // 2010-11-15/GGB - Function created.

  void CSearchSelectObjectDialog::eventBtnSearchClicked(bool)
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

      if (checkSpectralClass->checkState() == Qt::Checked)
      {
        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;
        szWhile += QString("(TBL_SPECTRALCTYPE.SHORTTEXT='%1')").arg(comboSpectralClass->currentText());
      };

      if (checkConstellation->checkState() == Qt::Checked)
      {
        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;
        szWhile += QString("(TBL_CONSTELLATIONS.CONSTELLATIONNAME='%1')").arg(comboConstellation->currentText());
      };

      if (checkCatalog->checkState() == Qt::Checked)
      {
        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;
        szWhile += QString("(TBL_CATALOG.CATALOGUE_NAME='%1')").arg(comboCatalog->currentText());
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

      if (groupStarType->isChecked())
      {
        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;

        if (radioPrimary->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.PRIMARYSTANDARD <> 0)");
        }
        else if (radioJohnson->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.JOHNSONSTANDARD <> 0)");
        }
        else if (radioCousins->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.COUSINSSTANDARD <> 0)");
        }
        else if (radioLandolt->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.LANDOLTSTANDARD <> 0)");
        }
        else if (radioAAVSO->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.AAVSOSTANDARD <> 0)");
        }
        else if (radioVariable->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.VARIABLE <> 0)");
        };
      };

      if (bWhile)
        szWhile += " AND ";
      else
        bWhile = true;
      szWhile += QString("(TBL_NAMES.NAME Like '%1')").arg(editObjectName->text());

      if (bWhile)
        szSQL += "WHERE " + szWhile;

      szSQL += " LIMIT 100";

      query.exec(*new QString(szSQL));

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

      if (checkSpectralClass->checkState() == Qt::Checked)
      {
        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;
        szWhile += QString("(TBL_SPECTRALCLASS.SHORTTEXT='%1')").arg(comboSpectralClass->currentText());
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

      if (groupStarType->isChecked())
      {
        if (bWhile)
          szWhile += " AND ";
        else
          bWhile = true;

        if (radioPrimary->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.ISPRIMARYSTANDARD <> 0)");
        }
        else if (radioJohnson->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.ISJOHNSONSTANDARD <> 0)");
        }
        else if (radioCousins->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.ISCOUSINSSTANDARD <> 0)");
        }
        else if (radioLandolt->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.ISLANDOLTSTANDARD <> 0)");
        }
        else if (radioAAVSO->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.ISAAVSOSTANDARD <> 0)");
        }
        else if (radioVariable->isChecked())
        {
          szWhile += QString("(TBL_STELLAROBJECTS.ISVARIABLE <> 0)");
        };
      };

      if (bWhile)
        szSQL += "WHERE " + szWhile;

      query.exec(*new QString(szSQL));

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

  // Handles the check event from the ObjectType check box.
  //
  // 2010-11-14/GGB - Function created.

  void CSearchSelectObjectDialog::eventCheckObjectTypeChanged(int newState)
  {
    if (newState == Qt::Checked)
    {
      comboObjectType->setEnabled(true);
    }
    else
    {
      comboObjectType->setEnabled(false);
    };
  };

  // Handles the check event from the Spectral Class.
  //
  // Revision:		1.00
  // Date:				14 November 2010

  void CSearchSelectObjectDialog::eventCheckSpectralClassChanged(int newState)
  {
    if (newState == Qt::Checked)
    {
      comboSpectralClass->setEnabled(true);
    }
    else
    {
      comboSpectralClass->setEnabled(false);
    };
  };

  // Handles the check event from the Constellation Check
  //
  // Revision:		1.00
  // Date:				14 November 2010

  void CSearchSelectObjectDialog::eventCheckConstellationChanged(int newState)
  {
    if (newState == Qt::Checked)
    {
      comboConstellation->setEnabled(true);
    }
    else
    {
      comboConstellation->setEnabled(false);
    };
  };

  // Handles the check event from the catalog check
  //
  // Revision:	1.00
  // Date:			14 November 2010

  void CSearchSelectObjectDialog::eventCheckCatalogChanged(int newState)
  {
    if (newState == Qt::Checked)
    {
      comboCatalog->setEnabled(true);
    }
    else
    {
      comboCatalog->setEnabled(false);
    };
  };

  void CSearchSelectObjectDialog::eventButtonHelpClicked(bool)
  {
  };

  // Handles the cancel button click.
  //
  // Revision:	1.00
  // Date:			17 November 2010

  void CSearchSelectObjectDialog::eventButtonCancelClicked(bool)
  {
    startTab = tabWidget->currentIndex();

    dlg->reject();
  }

  // Handles the select click.
  // Revision:	1.00
  // Date:			27 November 2010

  void CSearchSelectObjectDialog::eventButtonSelectClicked(bool)
  {
    int nRow;
    QString szString;

    startTab = tabWidget->currentIndex();

    switch (startTab)
    {
    case 0:	// From List
      nRow = tableObjects->currentRow();
      vNameID = tableObjects->item(nRow, 0)->data(Qt::UserRole);
      szString = tableObjects->item(nRow, 0)->text();
      break;
    case 1:	// From Photometry
      nRow = tablePhotometryObjects->currentRow();
      vNameID = tablePhotometryObjects->item(nRow, 0)->data(Qt::UserRole);
      szString = tablePhotometryObjects->item(nRow, 0)->text();
      break;
    case 2:
      nRow = tableRecentObjects->currentRow();
      vNameID = tableRecentObjects->item(nRow, 0)->data(Qt::UserRole);
      szString = tableRecentObjects->item(nRow, 0)->text();
      break;
    };

      // Update the recent objects list.

    settings::InsertRecentObject(szString);

    dlg->accept();

  };

  // Enables the selection button when an item is selected.
  //
  // Revision:		1.00
  // Date:				17 November 2010

  void CSearchSelectObjectDialog::eventTableObjectsCellClicked(int, int)
  {
    buttonSelect->setEnabled(true);
  };

  // Enables the selection button when an item is selected.
  //
  // Revision:		1.00
  // Date:				26 November 2010

  void CSearchSelectObjectDialog::eventTablePhotometryCellClicked(int, int)
  {
    buttonSelect->setEnabled(true);
  }

  // Enables the selection button when an item is selected.
  //
  // Revision:		1.00
  // Date:				27 November 2010

  void CSearchSelectObjectDialog::eventTableRecentCellClicked(int, int)
  {
    buttonSelect->setEnabled(true);
  }

  // Responds to an item being selected in the Photometry box.
  //
  // 2010-11-17/GGB - Function created.

  void CSearchSelectObjectDialog::eventComboPhotometryCurrenINDEX_tChanged(int itemIndex)
  {
    QString szSQLProgram =  QString("SELECT TBL_NAMES.Name, TBL_OBJECTTYPES.ShortText, TBL_CONSTELLATIONS.LONGTEXT, TBL_STELLAROBJECTS.[V-Magnitude], TBL_SPECTRALCLASS.ShortText, TBL_CATALOGUE.CATALOGUENAME, TBL_NAMES.NAME_ID " \
                                    "FROM TBL_OBJECTTYPES INNER JOIN (TBL_CATALOGUE INNER JOIN (TBL_SPECTRALCLASS INNER JOIN (TBL_CONSTELLATIONS INNER JOIN (TBL_STELLAROBJECTS INNER JOIN (TBL_NAMES INNER JOIN TBL_PHOTOBJECTS ON TBL_NAMES.NAME_ID = TBL_PHOTOBJECTS.NAME_ID) ON TBL_STELLAROBJECTS.OBJECT_ID = TBL_NAMES.STELLAROBJECT_ID) ON TBL_CONSTELLATIONS.CONSTELLATION_ID = TBL_STELLAROBJECTS.Constellation_ID) ON TBL_SPECTRALCLASS.SPECTRALCLASS_ID = TBL_STELLAROBJECTS.SPECTRALCLASS_ID) ON TBL_CATALOGUE.CATALOGUE_ID = TBL_NAMES.CATALOGUE_ID) ON TBL_OBJECTTYPES.OBJECTTYPE_ID = TBL_STELLAROBJECTS.ObjectType_id " \
                                    "WHERE (((TBL_PHOTOBJECTS.OBSPROG_ID)=%1))").arg(comboPhotometry->itemData(itemIndex).toString());
    QSqlQuery queryProgram(database::databaseATID->database());
    int nRow;
    int nColumn;

      // Clear the list of objects

    for (nRow = tablePhotometryObjects->rowCount()-1; nRow >= 0; nRow--)
      tablePhotometryObjects->removeRow(nRow);

    queryProgram.exec(szSQLProgram);

      // Add the objects to the list.

    nRow = 0;
    while (queryProgram.next())
    {
      nColumn = 0;
      tablePhotometryObjects->insertRow(nRow);
      tablePhotometryObjects->setRowHeight(nRow, 20);

      tablePhotometryObjects->setItem(nRow, nColumn, new QTableWidgetItem(*new QString(queryProgram.value(0).toString())));
      tablePhotometryObjects->item(nRow, nColumn++)->setData(Qt::UserRole, *new QVariant(queryProgram.value(6)));
      tablePhotometryObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(queryProgram.value(1).toString())));
      tablePhotometryObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(queryProgram.value(2).toString())));
      tablePhotometryObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(QString("%1").arg(queryProgram.value(3).toDouble(), 4, 'f', 3, '0'))));
      tablePhotometryObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(queryProgram.value(4).toString())));
      tablePhotometryObjects->setItem(nRow, nColumn++, new QTableWidgetItem(*new QString(queryProgram.value(5).toString())));
      nRow++;
    };
  }

  // Populates the recent object table.
  //
  // 2010-11027/GGB - Function created.

  void CSearchSelectObjectDialog::PopulateRecentObjects(void)
  {
    int nRow, nColumn;
    QString szSQL, szKey;
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
  }

  //*******************************************************************************************************************************
  //
  // CRotateImageDialog
  //
  //*******************************************************************************************************************************

  // Constructor for the class.
  // Sets up the values in the class.
  /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
  /// @version 2011-05-31/GGB - Function created.

  CRotateImageDialog::CRotateImageDialog(double *newAngle) : CDialog(":/forms/dialogRotateImage.ui"), angle(newAngle)
  {
    (*angle) = settings::VSOPSettings->value(settings::DIALOG_ROTATEIMAGE_LASTVALUE, QVariant(0)).toDouble();

#ifdef _MSC_VER
    if (!_isnan(*angle))
#else
    if (!std::isnan(*angle))
#endif
    {
      if (angle < 0)
      {
        findChild<QRadioButton *>("radioButtonLeft")->setChecked(true);
      }
      else
      {
        findChild<QRadioButton *>("radioButtonRight")->setChecked(true);
      }

      findChild<QDoubleSpinBox *>("doubleSpinBoxAngle")->setValue(abs(*angle));
    }
    else
    {
      findChild<QDoubleSpinBox *>("doubleSpinBoxAngle")->setValue(0);
    }

    connect(findChild<QPushButton *>("buttonOk"), SIGNAL(clicked()), this, SLOT(btnOk()));

    connect(findChild<QPushButton *>("buttonCancel"), SIGNAL(clicked()), this, SLOT(btnCancel()));
  }

    // BtnOk pressed
    /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
    /// @version 2011-05-31/GGB - Function created.

    void CRotateImageDialog::btnOk(void)
    {
      (*angle) = findChild<QDoubleSpinBox *>("doubleSpinBoxAngle")->value();
      if (findChild<QRadioButton *>("radioButtonLeft")->isChecked())
      {
        (*angle) = -(*angle);
      }

      settings::VSOPSettings->setValue(settings::DIALOG_ROTATEIMAGE_LASTVALUE, QVariant((double) *angle));

      dlg->accept();
    }

    //*****************************************************************************************************************************
    //
    // CSelectSiteDialog
    //
    //*****************************************************************************************************************************

    // Class constructor
    //
    // 2011-06-26/GGB - Function created.

    CSelectSiteDialog::CSelectSiteDialog(QVariant &id) : CDialog(":/forms/dialogSelectSite.ui"), siteID(id)
    {
      setupUI();
    }

    // Handles the select and close button
    //
    // 2011-06-30/GGB - Function created.

    void CSelectSiteDialog::btnOk(bool)
    {
      int nRow = tableWidget->currentRow();

      if (nRow != -1)
      {
        siteID = tableWidget->item(nRow, 0)->data(ROLE_SITEID);
        dlg->accept();
      };
    }

    // Sets up the user interface.
    /// @version 2017-06-14/GGB - Update to use CDialog::findChild(...) function
    /// @version 2011-06-30/GGB - Function created.

    void CSelectSiteDialog::setupUI()
    {
      QSqlQuery query(database::databaseARID->database());
      QString szSQL;
      int nRow = 0, nColumn;
      std::string valueString;

      tableWidget = findChild<QTableWidget *>("tableWidget");

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));

      szSQL = QString( \
        "SELECT s.SITE_ID, s.SHORTTEXT, s.LATITUDE, s.LONGITUDE, s.ALTITUDE, s.TIMEZONE, s.IAUCODE " \
        "FROM TBL_SITES s " \
        "WHERE s.DONTDISPLAY = 0");
      query.exec(szSQL);

      while (query.next())
      {
        nColumn = 0;

        tableWidget->insertRow(nRow);

        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(query.value(1).toString()));
        tableWidget->item(nRow, nColumn++)->setData(ROLE_SITEID, query.value(0));

           // Site Latitude

        valueString = ACL::sprintfLMS(ACL::dms2deg(query.value(2).toDouble()), 2, 0);
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(valueString.c_str())));

          // Site Longitude

        valueString = ACL::sprintfLMS(ACL::dms2deg(query.value(3).toDouble()), 2, 1);
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(valueString.c_str())));

        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(4).toString())));  // Site Altitude
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(5).toString())));  // Time Zone
        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(QString(query.value(6).toString())));    // IAU code

        nRow++;
      };
    }

  } // namespace dialogs
}	// namespace AstroManager
