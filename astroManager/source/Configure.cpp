//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Configure.cpp
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2020 Gavin Blakeman.
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
//						          Implements classes used for configuration, specifically configuration of database parameters.
//
// CLASSES INCLUDED:    CConfigureSitesWindow - Window to allow user to edit can configure observing sites.
//                      CConfigureObserversWindow - Window to allow user to add/edit observer information.
//                      CConfigureInstrumentsWindow -
//                      CConfigureTelescopesWindow -
//                      CConfigureSiteDialog - Dialog used with the above window for data entry.
//                      CConfigureObserverDialog - Dialog used to configure observer information.
//                      CConfigureInstrumentDialog -
//                      CConfigureTelescopeDialog -
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CARPAASubWindow
//                          - CConfigureSitesWindow
//                          - CConfigureObserversWindow
//                          - CConfigureTelescopesWindow
//                          - CConfigureInstrumentsWindow
//
//                      QDialog
//                        - CDialog
//                          - CConfigureSiteDialog
//                          - CConfigureObserverDialog
//                          - CConfigureInstrumentDialog
//                          - CConfigureTelescopeDialog
//
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-19 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "include/Configure.h"

  // astroManager application header files.

#include "include/database/databaseATID.h"
#include "include/settings.h"

  // Miscellaneous library header files.

#include <ACL>
#include <QCL>

namespace astroManager
{
  namespace configure
  {
    int const ROLE_INSTRUMENTID = Qt::UserRole + 0;
    int const ROLE_CATALOGUEORDERID = Qt::UserRole + 0;
    int const ROLE_OBSERVERID  = Qt::UserRole + 0;
    int const ROLE_TELESCOPEID = Qt::UserRole + 0;

    //*****************************************************************************************************************************
    //
    // CConfigureCatalogWindow
    //
    //*****************************************************************************************************************************

    /// @brief Class constructor.
    /// @param[in] aParent - Pointer to the parent window
    /// @throws None
    /// @version 2011-06-25/GGB - Function created.

    CConfigureCatalogWindow::CConfigureCatalogWindow(QWidget *aParent) : CMdiSubWindow(aParent)
    {
      setupUI();
    }

    /// @brief Moves a catalog down in the sort list.
    /// @throws None
    /// @version 2011-06-25/GGB - Function created.

    void CConfigureCatalogWindow::eventMoveDown(bool)
    {
      QSqlTableModel model;
      QSqlRecord record;
      int nRow;
      QVariant vObserverID;
      QVariant vCatalogOrderID;
      QVariant vSortOrder;
      unsigned int nSortOrder;
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      QTableWidgetItem *twi[4];
      int nColumn;

      nRow = tableWidget->currentRow();

      if ( nRow < (tableWidget->rowCount()-1) )
      {
        vCatalogOrderID = tableWidget->item(nRow, 0)->data(ROLE_CATALOGUEORDERID);

        szSQL = QString( \
          "SELECT c.OBSERVER_ID, c.SORTORDER " \
          "FROM TBL_CATALOGORDER c " \
          "WHERE c.CATALOGORDER_ID = %1").arg(vCatalogOrderID.toString());

        query.exec(szSQL);

        query.first();

        vObserverID = query.value(0);
        vSortOrder = query.value(1);
        nSortOrder = vSortOrder.toUInt();

        //database::CreateRecord_TBL_CATALOGORDER(record);

        model.setTable("TBL_CATALOGORDER");
        model.setEditStrategy(QSqlTableModel::OnRowChange);
        model.setFilter(QString("(SORTORDER = %1 AND OBSERVER_ID = %3) OR (SORTORDER = %2 AND OBSERVER_ID = %3)") \
          .arg(nSortOrder).arg(nSortOrder+1).arg(vObserverID.toString()));
        model.setSort(3, Qt::AscendingOrder);
        model.select();

        record = model.record(0);
        record.setValue("SORTORDER", QVariant(nSortOrder + 1));
        model.setRecord(0, record);
        model.submit();

        record = model.record(1);
        record.setValue("SORTORDER", QVariant(nSortOrder));
        model.setRecord(1, record);
        model.submit();

        for (nColumn = 0; nColumn < 4; nColumn++)
          twi[nColumn] = tableWidget->takeItem(nRow, nColumn);

        tableWidget->removeRow(nRow);
        nRow++;
        tableWidget->insertRow(nRow);

        for (nColumn = 0; nColumn < 4; nColumn++)
        {
          tableWidget->setItem(nRow, nColumn, twi[nColumn]);
          twi[nColumn] = NULL;
        };

        pushButtonMoveDown->setEnabled(false);
        pushButtonMoveUp->setEnabled(false);
      };
    }

    /// @brief Moves a catalog up in the sort list.
    /// @throws None
    /// @version 2011-06-25/GGB - Function created.

    void CConfigureCatalogWindow::eventMoveUp(bool)
    {
      QSqlTableModel model;
      QSqlRecord record;
      int nRow;
      QVariant vObserverID;
      QVariant vCatalogOrderID;
      QVariant vSortOrder;
      unsigned int nSortOrder;
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      QTableWidgetItem *twi[4];
      int nColumn;

      nRow = tableWidget->currentRow();

      if ( nRow > 0 )
      {
        vCatalogOrderID = tableWidget->item(nRow, 0)->data(ROLE_CATALOGUEORDERID);

        szSQL = QString( \
          "SELECT c.OBSERVER_ID, c.SORTORDER " \
          "FROM TBL_CATALOGORDER c " \
          "WHERE c.CATALOGORDER_ID = %1").arg(vCatalogOrderID.toString());

        query.exec(szSQL);

        query.first();

        vObserverID = query.value(0);
        vSortOrder = query.value(1);
        nSortOrder = vSortOrder.toUInt();

        //database::CreateRecord_TBL_CATALOGORDER(record);

        model.setTable("TBL_CATALOGORDER");
        model.setEditStrategy(QSqlTableModel::OnRowChange);
        model.setFilter(QString("(SORTORDER = %1 AND OBSERVER_ID = %3) OR (SORTORDER = %2 AND OBSERVER_ID = %3)") \
          .arg(nSortOrder-1).arg(nSortOrder).arg(vObserverID.toString()));
        model.setSort(3, Qt::AscendingOrder);
        model.select();

        record = model.record(0);
        record.setValue("SORTORDER", QVariant(nSortOrder));
        model.setRecord(0, record);
        model.submit();

        record = model.record(1);
        record.setValue("SORTORDER", QVariant(nSortOrder - 1));
        model.setRecord(1, record);
        model.submit();

        for (nColumn = 0; nColumn < 4; nColumn++)
          twi[nColumn] = tableWidget->takeItem(nRow, nColumn);

        tableWidget->removeRow(nRow);
        nRow--;
        tableWidget->insertRow(nRow);

        for (nColumn = 0; nColumn < 4; nColumn++)
        {
          tableWidget->setItem(nRow, nColumn, twi[nColumn]);
          twi[nColumn] = NULL;
        };

        pushButtonMoveUp->setEnabled(false);
        pushButtonMoveDown->setEnabled(false);
      };
    };

    // Manages button selection when the user selects a row.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureCatalogWindow::eventRowSelected(int nRow, int)
    {
      if (nRow == 0)
        pushButtonMoveUp->setEnabled(false);
      else
        pushButtonMoveUp->setEnabled(true);

      if (nRow == (tableWidget->rowCount() - 1) )
        pushButtonMoveDown->setEnabled(false);
      else
        pushButtonMoveDown->setEnabled(true);
    }

    /// @brief Sets up the user interface as required.
    ///
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureCatalogWindow::setupUI()
    {
      QUiLoader loader;
      QSqlQuery query;
      QString szSQL;
      int nRow = 0;
      int nColumn = 0;

      QFile file(":/forms/windowConfigureCatalogPrefs.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tableWidget = formWidget->findChild<QTableWidget *>("tableWidget");
      pushButtonMoveUp = formWidget->findChild<QPushButton *>("pushButtonMoveUp");
      pushButtonMoveDown = formWidget->findChild<QPushButton *>("pushButtonMoveDown");

      connect(pushButtonMoveUp, SIGNAL(clicked(bool)), this, SLOT(eventMoveUp(bool)));
      connect(pushButtonMoveDown, SIGNAL(clicked(bool)), this, SLOT(eventMoveDown(bool)));
      connect(tableWidget, SIGNAL(cellPressed(int, int)), this, SLOT(eventRowSelected(int, int)));

      szSQL = QString( \
        "SELECT TBL_CATALOGORDER.CATALOGORDER_ID, TBL_CATALOG.CATALOG_NAME, TBL_CATALOG.ABBREVIATION, TBL_CATALOG.COMMENTS, TBL_CATALOG.MASK " \
        "FROM TBL_CATALOG INNER JOIN TBL_CATALOGORDER ON TBL_CATALOG.CATALOG_ID = TBL_CATALOGORDER.CATALOG_ID " \
        "WHERE (((TBL_CATALOGORDER.OBSERVER_ID)=%1)) " \
        "ORDER BY TBL_CATALOGORDER.SORTORDER").arg(settings::astroManagerSettings->value(settings::SETTINGS_OBSERVER).toString());

      query.exec(szSQL);

      while (query.next())
      {
        nColumn = 0;

        tableWidget->insertRow(nRow);

        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(query.value(1).toString()));  // CATALOG_NAME
        tableWidget->item(nRow, nColumn++)->setData(ROLE_CATALOGUEORDERID, query.value(0));

        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(2).toString()));  // Abbreviation
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(3).toString()));  // Comments
        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(query.value(4).toString()));  // Mask

        nRow++;
      };
    };

    //*****************************************************************************************************************************
    //
    // CConfigureInstrumentsWindow
    //
    //*****************************************************************************************************************************

    // Class constructor.
    //
    // 2011-06-23/GGB - Function created.

    CConfigureInstrumentsWindow::CConfigureInstrumentsWindow(QWidget *aParent) : CMdiSubWindow(aParent)
    {
      setupUI();
    }

    // Processes the close event. Lets the MDI window know that this sub-window has been closed.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    };

    // Creates a new instrument record by using the dialog.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::eventCreateInstrument(bool)
    {
      QVariant vInstrumentID;
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      int nRow;

      CConfigureInstrumentDialog dlg(vInstrumentID, true);

      if (dlg.exec() == QDialog::Accepted)
      {
        szSQL = QString( \
          "SELECT i.INSTRUMENT_ID, i.SHORTTEXT, i.MANUFACTURER, i.MODEL, i.nX, i.nY, i.dX, i.dY, i.ADU, i.DONTDISPLAY " \
          "FROM TBL_INSTRUMENTS i " \
          "WHERE i.INSTRUMENT_ID = %1").arg(vInstrumentID.toString());
        query.exec(szSQL);
        query.first();

        nRow = tableWidget->rowCount();
        insertRow(nRow, query);
      };
    }

    // Handles the don't display button being pressed.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::eventDontDisplay(bool)
    {
      QVariant vInstrumentID = tableWidget->item(tableWidget->currentRow(), 0)->data(ROLE_INSTRUMENTID);
      QSqlTableModel model(0,database::databaseATID->database());
      QSqlRecord record;

      //database::CreateRecord_TBL_INSTRUMENTS(record);

      model.setTable("TBL_INSTRUMENTS");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.setFilter(QString("INSTRUMENT_ID = %1").arg(vInstrumentID.toString()));
      model.select();

      record = model.record(0);
      record.setValue("DONTDISPLAY", QVariant(true));

      model.setRecord(0, record);
      model.submit();

      redrawRow(tableWidget->currentRow());
    }

    // Brings up the dialog to edit an observer if the table is double clicked.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::eventRowSelected(int nRow, int)
    {
      QVariant vInstrumentID = tableWidget->item(nRow, 0)->data(ROLE_INSTRUMENTID);

      CConfigureInstrumentDialog dlg(vInstrumentID, false);
      dlg.exec();

      redrawRow(nRow);
    };

    // Adds the row into the table.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::insertRow(int nRow, QSqlQuery &query)
    {
      QTableWidgetItem *twi;
      int nColumn = 0;

      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      tableWidget->insertRow(nRow);

        // SHORTTEXT

      twi = new QTableWidgetItem(QString(query.value(1).toString()));
      twi->setData(ROLE_INSTRUMENTID, QVariant(query.value(0)));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Manufacturer

      twi = new QTableWidgetItem(QString(query.value(2).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Model

      twi = new QTableWidgetItem(QString(query.value(3).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // nX

      twi = new QTableWidgetItem(QString(query.value(4).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // nY

      tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(5).toString())));

        // dX

      tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(6).toString())));

        // dY

      tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(7).toString())));

        // ADU

      tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(8).toString())));

        // Don't display

      if (query.value(9).toBool())
        twi = new QTableWidgetItem(QString(tr("True")));
      else
        twi = new QTableWidgetItem(QString(tr("False")));
      tableWidget->setItem(nRow, nColumn, twi);

      if (se)
        tableWidget->setSortingEnabled(true);
    };

    // Redraws a row after changes have been made.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::redrawRow(int nRow)
    {
      QVariant vInstrumentID = tableWidget->item(nRow, 0)->data(ROLE_INSTRUMENTID);
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nColumn = 0;
      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      szSQL = QString( \
        "SELECT i.SHORTTEXT, i.MANUFACTURER, i.MODEL, i.nX, i.nY, i.dX, i.dY, i.ADU, i.DONTDISPLAY " \
        "FROM TBL_INSTRUMENTS i " \
        "WHERE i.INSTRUMENT_ID = %1").arg(vInstrumentID.toString());;

      query.exec(szSQL);

      query.first();

      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(0).toString()));  // SHORTTEXT
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(1).toString()));  // Manufacturer
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(2).toString()));  // Model
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(3).toString()));  // nX
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(4).toString()));  // nY
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(5).toString()));  // dX
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(6).toString()));  // dY
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(7).toString()));  // ADU

      if (query.value(8).toBool())
        tableWidget->item(nRow, nColumn)->setText(QString(tr("True")));
      else
        tableWidget->item(nRow, nColumn)->setText(QString(tr("False")));

      tableWidget->repaint();

      if (se)
        tableWidget->setSortingEnabled(true);

    };

    // Sets up the user interface as required.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::setupUI()
    {
      QUiLoader loader;
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nRow = 0;

      QFile file(":/forms/windowConfigureInstruments.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tableWidget = formWidget->findChild<QTableWidget *>("tableWidget");

      connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(eventRowSelected(int, int)));
      connect(formWidget->findChild<QPushButton *>("pushButtonAddInstrument"), SIGNAL(clicked(bool)),
        this, SLOT(eventCreateInstrument(bool)));
      connect(formWidget->findChild<QPushButton *>("pushButtonDontDisplay"), SIGNAL(clicked(bool)),
        this, SLOT(eventDontDisplay(bool)));

      szSQL = QString( \
        "SELECT i.INSTRUMENT_ID, i.SHORTTEXT, i.MANUFACTURER, i.MODEL, i.nX, i.nY, i.dX, i.dY, i.ADU, i.DONTDISPLAY " \
        "FROM TBL_INSTRUMENTS i");
      query.exec(szSQL);

        // fill the table.

      while (query.next())
      {
        insertRow(nRow++, query);
      };
    }

    // Called when the window is about to activate. Can enable/disable actions as required.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentsWindow::windowActivating()
    {
    }

    //*****************************************************************************************************************************
    //
    // CConfigureObserversWindow
    //
    //*****************************************************************************************************************************

    // Class constructor
    //
    // 2011-06-22/GGB - Function created.

    CConfigureObserversWindow::CConfigureObserversWindow(QWidget *aParent) : CMdiSubWindow(aParent)
    {
      setupUI();
    }

    // Processes the close event. Lets the MDI window know that this sub-window has been closed.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

    // Creates a new observer record by using the dialog.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::eventCreateObserver(bool)
    {
      QVariant vObserverID;
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      int nRow;

      CConfigureObserverDialog dlg(vObserverID, true);

      if (dlg.exec() == QDialog::Accepted)
      {
        szSQL = QString( \
          "SELECT o.OBSERVER_ID, o.SHORTTEXT, o.AAVSOINITIALS, o.DONTDISPLAY " \
          "FROM TBL_OBSERVERS o " \
          "WHERE o.OBSERVER_ID = %1").arg(vObserverID.toString());
        query.exec(szSQL);
        query.first();

        nRow = tableWidget->rowCount();
        insertRow(nRow, query);
      };
    }

    // Handles the don't display button being pressed.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::eventDontDisplay(bool)
    {
      QVariant vObserverID = tableWidget->item(tableWidget->currentRow(), 0)->data(ROLE_OBSERVERID);
      QSqlTableModel model(0, database::databaseATID->database());
      QSqlRecord record;

      //database::CreateRecord_TBL_OBSERVERS(record);

      model.setTable("TBL_OBSERVERS");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.setFilter(QString("OBSERVER_ID = %1").arg(vObserverID.toString()));
      model.select();

      record = model.record(0);
      record.setValue("DONTDISPLAY", QVariant(true));

      model.setRecord(0, record);
      model.submit();

      redrawRow(tableWidget->currentRow());
    }

    // Brings up the dialog to edit an observer if the table is double clicked.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::eventRowSelected(int nRow, int)
    {
      QVariant vObserverID = tableWidget->item(nRow, 0)->data(ROLE_OBSERVERID);

      CConfigureObserverDialog dlg(vObserverID, false);
      dlg.exec();

      redrawRow(nRow);
    }

    // Adds the row into the table.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::insertRow(int nRow, QSqlQuery &query)
    {
      QTableWidgetItem *twi;
      int nColumn = 0;

      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      tableWidget->insertRow(nRow);

      twi = new QTableWidgetItem(QString(query.value(1).toString()));
      twi->setData(ROLE_OBSERVERID, QVariant(query.value(0)));
      tableWidget->setItem(nRow, nColumn++, twi);

      twi = new QTableWidgetItem(QString(query.value(2).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

      if (query.value(3).toBool())
        twi = new QTableWidgetItem(QString(tr("True")));
      else
        twi = new QTableWidgetItem(QString(tr("False")));
      tableWidget->setItem(nRow, nColumn, twi);

      if (se)
        tableWidget->setSortingEnabled(true);
    };

    // Redraws a row after changes have been made.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::redrawRow(int nRow)
    {
      QVariant vObserverID = tableWidget->item(nRow, 0)->data(ROLE_OBSERVERID);
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nColumn = 0;
      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      szSQL = QString( \
        "SELECT o.SHORTTEXT, o.AAVSOINITIALS, o.DONTDISPLAY " \
        "FROM TBL_OBSERVERS o " \
        "WHERE o.OBSERVER_ID = %1").arg(vObserverID.toString());

      query.exec(szSQL);

      query.first();

      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(0).toString()));
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(1).toString()));

      if (query.value(2).toBool())
        tableWidget->item(nRow, nColumn)->setText(QString(tr("True")));
      else
        tableWidget->item(nRow, nColumn)->setText(QString(tr("False")));

      tableWidget->repaint();

      if (se)
        tableWidget->setSortingEnabled(true);

    };

    // Sets up the user interface as required.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureObserversWindow::setupUI()
    {
      QUiLoader loader;
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nRow = 0;

      QFile file(":/forms/windowConfigureObservers.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tableWidget = formWidget->findChild<QTableWidget *>("tableWidget");

      connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(eventRowSelected(int, int)));
      connect(formWidget->findChild<QPushButton *>("pushButtonAddObserver"), SIGNAL(clicked(bool)),
        this, SLOT(eventCreateObserver(bool)));
      connect(formWidget->findChild<QPushButton *>("pushButtonDontDisplay"), SIGNAL(clicked(bool)),
        this, SLOT(eventDontDisplay(bool)));

      szSQL = QString( \
        "SELECT o.OBSERVER_ID, o.SHORTTEXT, o.AAVSOINITIALS, o.DONTDISPLAY " \
        "FROM TBL_OBSERVERS o");
      query.exec(szSQL);

        // fill the table.

      while (query.next())
      {
        insertRow(nRow++, query);
      };
    };

    //*****************************************************************************************************************************
    //
    // CConfigureSitesWindow
    //
    //*****************************************************************************************************************************

    // Constructor for the configuration of sites window.
    //
    // 2011-06-19/GGB - Function created.

    CConfigureSitesWindow::CConfigureSitesWindow(QWidget *aParent) : CMdiSubWindow(aParent)
    {
      setupUI();
    }

    // Processes the close event. Lets the MDI window know that this sub-window has been closed.
    //
    // 2011-06-19/GGB - Function created.

    void CConfigureSitesWindow::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

    // Function to create a new site.
    //
    // 2011-06-21/GGB - Function created.

    void CConfigureSitesWindow::eventCreateSite(bool)
    {
      QVariant vSiteID;
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      int nRow;

//      CConfigureSiteDialog dlg(vSiteID, true);

//      if (dlg.exec() == QDialog::Accepted)
//      {
//        szSQL = QString( \
//          "SELECT ts.SITE_ID, ts.SHORTTEXT, ts.LATITUDE, ts.LONGITUDE, ts.ALTITUDE, ts.TIMEZONE, ts.DAYLIGHTSAVING, ts.IAUCODE, ts.DONTDISPLAY " \
//          "FROM TBL_SITES ts " \
//          "WHERE ts.SITE_ID = %1").arg(vSiteID.toString());
//        query.exec(szSQL);
//        query.first();

//        nRow = tableWidget->rowCount();
//        insertRow(nRow, query);
//      };
    }

    // Quick way to flag that the item should not be displayed.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureSitesWindow::eventDontDisplay(bool)
    {
      QVariant vSiteID = tableWidget->item(tableWidget->currentRow(), 0)->data(ROLE_SITEID);
      QSqlTableModel model;
      QSqlRecord record;

      //database::CreateRecord_TBL_SITES(record);

      model.setTable("TBL_SITES");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.setFilter(QString("SITE_ID = %1").arg(vSiteID.toString()));
      model.select();

      record = model.record(0);
      record.setValue("DONTDISPLAY", QVariant(true));

      model.setRecord(0, record);
      model.submit();

      redrawRow(tableWidget->currentRow());
    }

    // Procedure to handle the user selecting a row.
    // Brings up the dialog to edit the site information.
    //
    // 2011-06-20/GGB - Function created.

    void CConfigureSitesWindow::eventRowSelected(int nRow, int)
    {
      QVariant vSiteID = tableWidget->item(nRow, 0)->data(ROLE_SITEID);

      //CConfigureSiteDialog dlg(vSiteID, false);
      //dlg.exec();

      redrawRow(nRow);
    }

    // Inserts a row into the table.
    //
    // 2011-06-21/GGB - Function created.

    void CConfigureSitesWindow::insertRow(int nRow, QSqlQuery &query)
    {
      int nColumn = 0;
      QTableWidgetItem *twi = NULL;
      char szNumber[30];
      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      tableWidget->insertRow(nRow);

        // Site Shorttext

      twi = new QTableWidgetItem(QString(query.value(1).toString()));
      twi->setData(ROLE_SITEID, query.value(0));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Site Latitude

      ACL::sprintfLMS(szNumber, ACL::dms2deg(query.value(2).toDouble()), 2, 0);
      twi = new QTableWidgetItem(QString(szNumber));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Site Longitude

      ACL::sprintfLMS(szNumber, ACL::dms2deg(query.value(3).toDouble()), 2, 1);
      twi = new QTableWidgetItem(QString(szNumber));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Site Altitude

      twi = new QTableWidgetItem(QString(query.value(4).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Time Zone

      twi = new QTableWidgetItem(QString(query.value(5).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Daylight saving

      if (query.value(6).toBool())
        twi = new QTableWidgetItem(QString(tr("Yes")));
      else
        twi = new QTableWidgetItem(QString(tr("No")));
      tableWidget->setItem(nRow, nColumn++, twi);

        // IAUCode

      twi = new QTableWidgetItem(QString(query.value(7).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Don't Display

      if (query.value(8).toBool())
        twi = new QTableWidgetItem(QString(tr("True")));
      else
        twi = new QTableWidgetItem(QString(tr("False")));
      tableWidget->setItem(nRow, nColumn, twi);

      if (se)
        tableWidget->setSortingEnabled(true);
    }

    // Redraws a row after changes to the row.
    //
    // 2011-06-22/GGB - Function created.

    void CConfigureSitesWindow::redrawRow(int nRow)
    {
      QVariant vSiteID = tableWidget->item(nRow, 0)->data(ROLE_SITEID);
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      char szNumber[30];
      int nColumn = 0;
      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      szSQL = QString( \
        "SELECT ts.SHORTTEXT, ts.LATITUDE, ts.LONGITUDE, ts.ALTITUDE, ts.TIMEZONE, ts.DAYLIGHTSAVING, ts.IAUCODE, ts.DONTDISPLAY " \
        "FROM TBL_SITES ts " \
        "WHERE ts.SITE_ID = %1").arg(vSiteID.toString());

      query.exec(szSQL);

      query.first();

        // Short Text

      tableWidget->item(nRow, nColumn++)->setText(query.value(0).toString());

        // Site Latitude

      ACL::sprintfLMS(szNumber, ACL::dms2deg(query.value(1).toDouble()), 2, 0);
      tableWidget->item(nRow, nColumn++)->setText(szNumber);

        // Site Longitude

      ACL::sprintfLMS(szNumber, ACL::dms2deg(query.value(2).toDouble()), 2, 1);
      tableWidget->item(nRow, nColumn++)->setText(szNumber);

        // Site Altitude

      tableWidget->item(nRow, nColumn++)->setText(query.value(3).toString());

        // Time Zone

      tableWidget->item(nRow, nColumn++)->setText(query.value(4).toString());

        // Daylight saving

      if (query.value(5).toBool())
        tableWidget->item(nRow, nColumn++)->setText(tr("Yes"));
      else
        tableWidget->item(nRow, nColumn++)->setText(tr("No"));

        // IAUCode

      tableWidget->item(nRow, nColumn++)->setText(query.value(6).toString());

        // Don't Display

      if (query.value(7).toBool())
        tableWidget->item(nRow, nColumn)->setText(tr("True"));
      else
        tableWidget->item(nRow, nColumn)->setText(tr("False"));

      tableWidget->repaint();

      if (se)
        tableWidget->setSortingEnabled(true);
    }

    // Sets up the user interface for the class.
    //
    // 2011-06-19/GGB - Function created.

    void CConfigureSitesWindow::setupUI()
    {
      QUiLoader loader;
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nRow = 0;

      QFile file(":/forms/windowConfigureSites.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tableWidget = formWidget->findChild<QTableWidget *>("tableWidget");

      connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(eventRowSelected(int, int)));
      connect(formWidget->findChild<QPushButton *>("pushButtonAddSite"), SIGNAL(clicked(bool)),
        this, SLOT(eventCreateSite(bool)));
      connect(formWidget->findChild<QPushButton *>("pushButtonDontDisplay"), SIGNAL(clicked(bool)),
        this, SLOT(eventDontDisplay(bool)));

      szSQL = QString( \
        "SELECT ts.SITE_ID, ts.SHORTTEXT, ts.LATITUDE, ts.LONGITUDE, ts.ALTITUDE, ts.TIMEZONE, ts.DAYLIGHTSAVING, ts.IAUCODE, ts.DONTDISPLAY " \
        "FROM TBL_SITES ts");
      query.exec(szSQL);

      while (query.next())
      {
        insertRow(nRow++, query);
      };

    }

    // Called when the window is about to activate. Can enable/disable actions as required.
    //
    // 2011-06-19/GGB - Function created.

    void CConfigureSitesWindow::windowActivating()
    {
    }

    //*****************************************************************************************************************************
    //
    // CConfigureTelescopesWindow
    //
    //*****************************************************************************************************************************

    // Class constructor.
    //
    // 2011-06-25/GGB - Function created.

    CConfigureTelescopesWindow::CConfigureTelescopesWindow(QWidget *aParent) : CMdiSubWindow(aParent)
    {
      setupUI();
    };

    // Processes the close event. Lets the MDI window know that this sub-window has been closed.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    };

    // Creates a new instrument record by using the dialog.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::eventCreateTelescope(bool)
    {
      QVariant vTelescopeID;
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      int nRow;

//      CConfigureTelescopeDialog dlg(vTelescopeID, true);

//      if (dlg.exec() == QDialog::Accepted)
//      {
//        szSQL = QString( \
//          "SELECT t.TELESCOPE_ID, t.SHORTTEXT, t.MANUFACTURER, t.MODEL, t.APERTURE, t.FRATIO, t.OBSTRUCTION, t.DONTDISPLAY " \
//          "FROM TBL_TELESCOPES t " \
//          "WHERE t.TELESCOPE_ID = %1").arg(vTelescopeID.toString());
//        query.exec(szSQL);
//        query.first();

//        nRow = tableWidget->rowCount();
//        insertRow(nRow, query);
//      };
    }

    // Handles the don't display button being pressed.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::eventDontDisplay(bool)
    {
      QVariant vTelescopeID = tableWidget->item(tableWidget->currentRow(), 0)->data(ROLE_TELESCOPEID);
      QSqlTableModel model(0, database::databaseATID->database());
      QSqlRecord record;

      //database::CreateRecord_TBL_TELESCOPES(record);

      model.setTable("TBL_TELESCOPES");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.setFilter(QString("TELESCOPE_ID = %1").arg(vTelescopeID.toString()));
      model.select();

      record = model.record(0);
      record.setValue("DONTDISPLAY", QVariant(true));

      model.setRecord(0, record);
      model.submit();

      redrawRow(tableWidget->currentRow());
    };

    // Brings up the dialog to edit an observer if the table is double clicked.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::eventRowSelected(int nRow, int)
    {
      QVariant vTelescopeID = tableWidget->item(nRow, 0)->data(ROLE_TELESCOPEID);

//      CConfigureTelescopeDialog dlg(vTelescopeID, false);
//      dlg.exec();

      redrawRow(nRow);
    }

    // Adds the row into the table.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::insertRow(int nRow, QSqlQuery &query)
    {
      QTableWidgetItem *twi;
      int nColumn = 0;

      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      tableWidget->insertRow(nRow);

        // SHORTTEXT

      twi = new QTableWidgetItem(QString(query.value(1).toString()));
      twi->setData(ROLE_INSTRUMENTID, QVariant(query.value(0)));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Manufacturer

      twi = new QTableWidgetItem(QString(query.value(2).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Model

      twi = new QTableWidgetItem(QString(query.value(3).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // Aperture

      twi = new QTableWidgetItem(QString(query.value(4).toString()));
      tableWidget->setItem(nRow, nColumn++, twi);

        // F-Ratio

      tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(5).toString())));

        // Obstruction

      tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(query.value(6).toString())));

        // Don't display

      if (query.value(7).toBool())
        twi = new QTableWidgetItem(QString(tr("True")));
      else
        twi = new QTableWidgetItem(QString(tr("False")));
      tableWidget->setItem(nRow, nColumn, twi);

      if (se)
        tableWidget->setSortingEnabled(true);
    };

    // Redraws a row after changes have been made.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::redrawRow(int nRow)
    {
      QVariant vTelescopeID = tableWidget->item(nRow, 0)->data(ROLE_TELESCOPEID);
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nColumn = 0;
      bool se = tableWidget->isSortingEnabled();

      if (se)
        tableWidget->setSortingEnabled(false);

      szSQL = QString( \
        "SELECT t.SHORTTEXT, t.MANUFACTURER, t.MODEL, t.APERTURE, t.FRATIO, t.OBSTRUCTION, t.DONTDISPLAY " \
        "FROM TBL_TELESCOPES t " \
        "WHERE t.TELESCOPE_ID = %1").arg(vTelescopeID.toString());

      query.exec(szSQL);

      query.first();

      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(0).toString()));  // SHORTTEXT
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(1).toString()));  // Manufacturer
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(2).toString()));  // Model
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(3).toString()));  // Aperture
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(4).toString()));  // F-Ratio
      tableWidget->item(nRow, nColumn++)->setText(QString(query.value(5).toString()));  // Obstruction

      if (query.value(6).toBool())
        tableWidget->item(nRow, nColumn)->setText(QString(tr("True")));
      else
        tableWidget->item(nRow, nColumn)->setText(QString(tr("False")));

      tableWidget->repaint();

      if (se)
        tableWidget->setSortingEnabled(true);

    };

    // Sets up the user interface as required.
    //
    // 2011-06-25/GGB - Function created.

    void CConfigureTelescopesWindow::setupUI()
    {
      QUiLoader loader;
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nRow = 0;

      QFile file(":/forms/windowConfigureTelescopes.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      tableWidget = formWidget->findChild<QTableWidget *>("tableWidget");

      connect(tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(eventRowSelected(int, int)));
      connect(formWidget->findChild<QPushButton *>("pushButtonAddTelescope"), SIGNAL(clicked(bool)),
        this, SLOT(eventCreateTelescope(bool)));
      connect(formWidget->findChild<QPushButton *>("pushButtonDontDisplay"), SIGNAL(clicked(bool)),
        this, SLOT(eventDontDisplay(bool)));

      szSQL = QString( \
        "SELECT t.TELESCOPE_ID, t.SHORTTEXT, t.MANUFACTURER, t.MODEL, t.APERTURE, t.FRATIO, t.OBSTRUCTION, t.DONTDISPLAY " \
        "FROM TBL_TELESCOPES t ");

      query.exec(szSQL);

        // fill the table.

      while (query.next())
      {
        insertRow(nRow++, query);
      };
    };

    //*****************************************************************************************************************************
    //
    // CConfigureInstrumentDialog
    //
    //*****************************************************************************************************************************

    // Class constructor
    //
    // 2011-06-23/GGB - Function created.

    CConfigureInstrumentDialog::CConfigureInstrumentDialog(QVariant &instrumentID, bool nr)
      : CDialog(":/forms/dialogConfigureInstrument.ui"), vInstrumentID(instrumentID), newRecord(nr)
    {
      setupUI();
    }

    // Handles the OK button press.
    //
    // 2011-06-23/GGB - Function created.

    void CConfigureInstrumentDialog::btnOk(bool)
    {
      QString szShortText;
      QString szManufacturer;
      QString szModel;
      unsigned int nX, nY;
      double dX, dY;
      double ADU;
      bool dontdisplay = checkBoxDontDisplay->isChecked();
      QSqlTableModel model(0, database::databaseATID->database());
      QSqlRecord record;

      szShortText = lineEditShortText->text();
      szManufacturer = lineEditManufacturer->text();
      szModel = lineEditModel->text();
      nX = spinBoxNX->value();
      nY = spinBoxNY->value();
      dX = doubleSpinBoxDX->value();
      dY = doubleSpinBoxDY->value();
      ADU = doubleSpinBoxADU->value();

      model.setTable("TBL_INSTRUMENTS");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.select();

      //database::CreateRecord_TBL_INSTRUMENTS(record);
      if (szShortText.isNull())
        dlg->reject();
      else
      {
        if (newRecord)
        {
            // Must create a new record.

          record.setValue("SHORTTEXT", QVariant(szShortText));
          record.setValue("MANUFACTURER", QVariant(szManufacturer));
          record.setValue("MODEL", QVariant(szModel));
          record.setValue("nX", QVariant(nX));
          record.setValue("nY", QVariant(nY));
          record.setValue("dX", QVariant(dX));
          record.setValue("dY", QVariant(dY));
          record.setValue("ADU", QVariant(ADU));
          record.setValue("DONTDISPLAY", QVariant(dontdisplay));

          model.insertRecord(-1, record);

          record = model.record(model.rowCount()-1);
          vInstrumentID = record.value("INSTRUMENT_ID");
        }
        else
        {
            // Must update the record.

          model.setFilter(QString("INSTRUMENT_ID = %1").arg(vInstrumentID.toString()));
          model.select();

          record = model.record(0);
          record.setValue("SHORTTEXT", QVariant(szShortText));
          record.setValue("MANUFACTURER", QVariant(szManufacturer));
          record.setValue("MODEL", QVariant(szModel));
          record.setValue("nX", QVariant(nX));
          record.setValue("nY", QVariant(nY));
          record.setValue("dX", QVariant(dX));
          record.setValue("dY", QVariant(dY));
          record.setValue("ADU", QVariant(ADU));
          record.setValue("DONTDISPLAY", QVariant(dontdisplay));

          model.setRecord(0, record);
          model.submit();
        };
        dlg->accept();
      };
    }

    // Sets up the user interface as required.
    /// @version 2017-06-15/GGB - Update to Qt5
    /// @version 2011-06-23/GGB - Function created.

    void CConfigureInstrumentDialog::setupUI()
    {
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());

        // Get all the addressed of the child objects

      lineEditShortText = findChild<QLineEdit *>("lineEditShortText");
      lineEditManufacturer = findChild<QLineEdit *>("lineEditManufacturer");
      lineEditModel = findChild<QLineEdit *>("lineEditModel");
      spinBoxNX = findChild<QSpinBox *>("spinBoxNX");
      spinBoxNY = findChild<QSpinBox *>("spinBoxNY");
      doubleSpinBoxDX = findChild<QDoubleSpinBox *>("doubleSpinBoxDX");
      doubleSpinBoxDY = findChild<QDoubleSpinBox *>("doubleSpinBoxDY");
      doubleSpinBoxADU = findChild<QDoubleSpinBox *>("doubleSpinBoxADU");
      checkBoxDontDisplay = findChild<QCheckBox *>("checkBoxDontDisplay");

      if (!newRecord)
      {
        szSQL = QString( \
          "SELECT i.SHORTTEXT, i.MANUFACTURER, i.MODEL, i.nX, i.nY, i.dX, i.dY, i.ADU, i.DONTDISPLAY " \
          "FROM TBL_INSTRUMENTS i " \
          "WHERE i.INSTRUMENT_ID = %1").arg(vInstrumentID.toString());
        query.exec(szSQL);
        query.first();

        lineEditShortText->setText(query.value(0).toString());
        lineEditManufacturer->setText(query.value(1).toString());
        lineEditModel->setText(query.value(2).toString());
        spinBoxNX->setValue(query.value(3).toUInt());
        spinBoxNY->setValue(query.value(4).toUInt());
        doubleSpinBoxDX->setValue(query.value(5).toDouble());
        doubleSpinBoxDY->setValue(query.value(6).toDouble());
        doubleSpinBoxADU->setValue(query.value(7).toDouble());
        checkBoxDontDisplay->setChecked(query.value(8).toInt() != 0);
      };

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));
    }

    //*****************************************************************************************************************************
    //
    // CConfigureObserverDialog
    //
    //*****************************************************************************************************************************

    // Constructor for the class.
    //
    // 2011-06-22/GGB - Function created.

    CConfigureObserverDialog::CConfigureObserverDialog(QVariant &observerID, bool nr) : CDialog(":/forms/dialogConfigureObserver.ui"),
      vObserverID(observerID), newRecord(nr)
    {
      setupUI();
    }

    // Responds to the save and close button.
    //
    // Revision:  1.00
    // Date:      22 June 2011

    void CConfigureObserverDialog::btnOk(bool)
    {
      QString szShortText;
      QString szAAVSOInitials;
      QSqlTableModel model(0, database::databaseATID->database());
      QSqlRecord record;
      bool dontdisplay = checkBoxDontDisplay->isChecked();

      szShortText = lineEditShortText->text();
      szAAVSOInitials = lineEditAAVSOInitials->text();

      model.setTable("TBL_OBSERVERS");
      model.setEditStrategy(QSqlTableModel::OnRowChange);
      model.select();

      //database::CreateRecord_TBL_OBSERVERS(record);

      if (szShortText.isNull())
        dlg->reject();
      else
      {
        if (newRecord)
        {
            // Must create a new record.

          //database::CreateObserver(QVariant(szShortText), QVariant(szAAVSOInitials), dontdisplay, vObserverID);
        }
        else
        {
            // Must update the record.

          model.setFilter(QString("OBSERVER_ID = %1").arg(vObserverID.toString()));
          model.select();

          record = model.record(0);
          record.setValue("SHORTTEXT", QVariant(szShortText));
          if (szAAVSOInitials.size() > 0)
            record.setValue("AAVSOINITIALS", QVariant(szAAVSOInitials));
          else
            record.setNull("AAVSOINITIALS");
          record.setValue("DONTDISPLAY", QVariant(dontdisplay));

          model.setRecord(0, record);
          model.submit();
        };
        dlg->accept();
      };
    }

    // Sets up the user interface as required.
    /// @version 2017-06-15/GGB - Update to QT5
    /// @version 2011-06-22/GGB - Function created.

    void CConfigureObserverDialog::setupUI()
    {
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());

      lineEditShortText = findChild<QLineEdit *>("lineEditShortText");
      lineEditAAVSOInitials = findChild<QLineEdit *>("lineEditAAVSOInitials");
      checkBoxDontDisplay = findChild<QCheckBox *>("checkBoxDontDisplay");

      if (!newRecord)
      {
        szSQL = QString( \
          "SELECT o.SHORTTEXT, o.AAVSOINITIALS, o.DONTDISPLAY " \
          "FROM TBL_OBSERVERS o " \
          "WHERE o.OBSERVER_ID = %1").arg(vObserverID.toString());
        query.exec(szSQL);
        query.first();

        lineEditShortText->setText(query.value(0).toString());
        lineEditAAVSOInitials->setText(query.value(1).toString());
        checkBoxDontDisplay->setChecked(query.value(2).toInt() != 0);
      };

      connect(findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));
    }



  }  // namespace configure
}  // namespace AstroManager
