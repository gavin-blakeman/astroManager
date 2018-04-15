//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogSelectTelescope
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CSelectTelescopeDialog
//
// CLASS HIERARCHY:     CDialog
//                        - CSelectInstrumentDialog
//                        - CSelectTelescopeDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-05-27 GGB - Moved all dialogs into seperate files.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-02-03 GGB - Moved CMdiSubWindow to own file and renamed.
//                      2012-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#include "../../Include/dialogs/dialogSelectTelescope.h"

#include "../../Include/database/databaseATID.h"
#include "../../Include/Settings.h"

namespace AstroManager
{
  namespace dialogs
  {
    int const ROLE_TELESCOPEID    = Qt::UserRole + 0;

    //*****************************************************************************************************************************
    //
    // CSelectTelescopeDialog
    //
    //*****************************************************************************************************************************

    /// @brief Class constructor
    /// @throws None
    /// @version 2011-06-25/GGB - Function created.

    CSelectTelescopeDialog::CSelectTelescopeDialog() : CDialog(":/forms/dialogSelectTelescope.ui")
    {
      setupUI();
    }

    // Handles the select and close button
    //
    // 2011-06-25/GGB - Function created.

    void CSelectTelescopeDialog::btnOk(bool)
    {
      int nRow = tableWidget->currentRow();
      QVariant vTelescopeID;

      if (nRow != -1)
      {
        vTelescopeID = tableWidget->item(nRow, 0)->data(ROLE_TELESCOPEID);
        //settings::VSOPSettings->setValue(SETTINGS_TELESCOPE, vTelescopeID);
        dlg->accept();
      };
    }

    /// @brief Sets up the user interface elements.
    /// @throws None.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2011-06-25/GGB - Function created.

    void CSelectTelescopeDialog::setupUI()
    {
      QSqlQuery query(database::databaseATID->database());
      QString szSQL;
      int nRow = 0, nColumn;

      tableWidget = dlg->findChild<QTableWidget *>("tableWidget");

      connect(dlg->findChild<QPushButton *>("pushButtonOk"), SIGNAL(clicked(bool)), this, SLOT(btnOk(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonCancel"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));

      szSQL = QString( \
        "SELECT t.TELESCOPE_ID, t.SHORTTEXT, t.MANUFACTURER, t.MODEL, t.APERTURE, t.FRATIO, t.OBSTRUCTION " \
        "FROM TBL_TELESCOPES t " \
        "WHERE t.DONTDISPLAY = 0");

      query.exec(szSQL);

      while (query.next())
      {
        nColumn = 0;

        tableWidget->insertRow(nRow);

        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(query.value(1).toString()));
        tableWidget->item(nRow, nColumn++)->setData(ROLE_TELESCOPEID, query.value(0));

        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(2).toString()));   // Manufacturer
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(3).toString()));   // Model
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(4).toString()));   // Aperture
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(5).toString()));   // F-Ratio
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(6).toString()));   // Obstruction

        nRow++;
      };
    };

  };
}
