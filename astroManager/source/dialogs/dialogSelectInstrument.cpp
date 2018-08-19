//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogSelectInstrument
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
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
// OVERVIEW:            Implements the MDI Frame window for the application. There is a single class implemented:
//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CDialog
//                        - CSelectInstrumentDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-05-27 GGB - Moved all dialogs into seperate files.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-03 GGB - Moved CMdiSubWindow to own file and renamed.
//                      2012-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dialogs/dialogSelectInstrument.h"

#include "../../include/database/databaseATID.h"
#include "../../include/Settings.h"

namespace astroManager
{
  namespace dialogs
  {
    #define ROLE_INSTRUMENTID   Qt::UserRole + 0

    //*****************************************************************************************************************************
    //
    // CSelectInstrumentDialog
    //
    //*****************************************************************************************************************************

    // Class constructor
    //
    // 2011-06-25/GGB - Function created.

    CSelectInstrumentDialog::CSelectInstrumentDialog() : CDialog(":/forms/dialogSelectInstrument.ui")
    {
      setupUI();
    }

    // Handles the Ok button.
    //
    // Revision:  1.00
    // Date:      25 June 2011

    void CSelectInstrumentDialog::btnOk(bool)
    {
      int nRow = tableWidget->currentRow();
      QVariant vInstrumentID;

      if (nRow != -1)
      {
        vInstrumentID = tableWidget->item(nRow, 0)->data(ROLE_INSTRUMENTID);
        //settings::VSOPSettings->setValue(SETTINGS_INSTRUMENT, vInstrumentID);
        dlg->accept();
      };
    }

    // Sets up the user interface elements.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-06-25/GGB - Function created.

    void CSelectInstrumentDialog::setupUI()
    {
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nRow = 0, nColumn;

      tableWidget = dlg->findChild<QTableWidget *>("tableWidget");

      connect(dlg->findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));

      szSQL = QString( \
        "SELECT i.INSTRUMENT_ID, i.SHORTTEXT, i.MANUFACTURER, i.MODEL " \
        "FROM TBL_INSTRUMENTS i "
        "WHERE i.DONTDISPLAY = 0");

      query.exec(szSQL);

      while (query.next())
      {
        nColumn = 0;

        tableWidget->insertRow(nRow);

        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(query.value(1).toString()));
        tableWidget->item(nRow, nColumn++)->setData(ROLE_INSTRUMENTID, query.value(0));

        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(2).toString()));  // Manufacturer
        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(query.value(3).toString()));    // Model

        nRow++;
      };
    };
  };
}
