//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Utilities.cpp
// SUBSYSTEM:						Various Utility Classes
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2011-2020 Gavin Blakeman.
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
// OVERVIEW:            Implements a number of utility functions, mostly associated with the Utility Menu.
//                      These are functions rather than classes.
//
// CLASSES INCLUDED:
//
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2011-06-11 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "include/Utilities.h"

  // Standard C++ library header files

#include <iostream>
#include <sstream>

  // Miscellaneous library headers

#include <ACL>
#include <PCL>
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include <QCL>

  // astroManager header files

#include "include/database/databaseATID.h"

namespace astroManager
{
  namespace utilities
  {

    //*****************************************************************************************************************************
    //
    // CEnterDATDialog
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @throws None.
    /// @version 2011-12-31/GGB - Function created.

    CEnterDATDialog::CEnterDATDialog() : QCL::CDialog(":/forms/dialogEnterdAT.ui")
    {
      setupUI();
    }

    // Procedure to insert the new value into the table.
    //
    //

    void CEnterDATDialog::pushButtonInsert(bool)
    {
      QDate date = dateEditMJD->date();
      QSqlTableModel model(0, database::databaseATID->database());
      QSqlRecord record;
      int nRow = tableWidget->rowCount();
      int nColumn = 0;
      int cumulative = tableWidget->item(nRow-1, 3)->text().toInt();

      QMessageBox msgBox;

      msgBox.setWindowTitle(QString::fromStdString(boost::locale::translate("Changes to dAT (TAI-UTC)")));
      msgBox.setText(QString::fromStdString(boost::locale::translate("Are you sure that you wish to add the entry?")));
      msgBox.setInformativeText(QString::fromStdString(boost::locale::translate("Data should only be added to this table when the IERS indicates that a leap second will occur. Are you sure  that you wish to add the entry into the dAT (TAI-UTC) table?")));
      msgBox.setIcon(QMessageBox::Question);
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton(QMessageBox::No);

      if (msgBox.exec() == QMessageBox::Yes)
      {

        model.setTable("TBL_LEAPSECONDS");
        model.setEditStrategy(QSqlTableModel::OnRowChange);
        model.select();

        //database::CreateRecord_TBL_LEAPSECONDS(record);

        ACL::TJD JD((double) date.toJulianDay());

        record.setValue("MJD", QVariant((double) JD.MJD()));
        record.setValue("VALUE", QVariant(spinBoxValue->value()));

        cumulative += spinBoxValue->value();

        model.insertRecord(-1, record);

        record = model.record(model.rowCount()-1);

        tableWidget->insertRow(nRow);
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(record.value("MJD").toString()));
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(JD.gregorianDate().c_str())));
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(record.value("VALUE").toString()));
        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(QString("%1").arg(cumulative)));
      };
    }

    // Function to "fill" the dialog
    //
    // 2011-12-31/GGB - Function created.

    void CEnterDATDialog::setupUI()
    {
      int nRow = 0, nColumn;
      int cumulative = 10;
      QSqlQuery query(database::databaseATID->database());
      QString szSQL("SELECT ls.MJD, ls.VALUE FROM TBL_LEAPSECONDS ls ORDER BY ls.MJD");
      ACL::TJD JD;   // Initialises today.

        // Get all the objects

      ASSOCIATE_CONTROL(tableWidget, dlg, "tableWidget", QTableWidget);
      ASSOCIATE_CONTROL(dateEditMJD, dlg, "dateEditMJD", QDateEdit);
      ASSOCIATE_CONTROL(spinBoxValue, dlg, "spinBoxValue", QSpinBox);

      dateEditMJD->setDate(QDate::currentDate());

        // Add the data into the tableWidget

      query.exec(szSQL);

      while (query.next())
      {
        nColumn = 0;
        tableWidget->insertRow(nRow);

        cumulative += query.value(1).toInt();
        JD = ACL::TJD(ACL::MJD0, query.value(0).toDouble());

        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(0).toString()));
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(QString(JD.gregorianDate().c_str())));
        tableWidget->setItem(nRow, nColumn++, new QTableWidgetItem(query.value(1).toString()));
        tableWidget->setItem(nRow, nColumn, new QTableWidgetItem(QString("%1").arg(cumulative)));

        nRow++;
      };

      spinBoxValue->setValue(1);

      connect(dlg->findChild<QPushButton *>("pushButtonClose"), SIGNAL(clicked(bool)), this, SLOT(btnCancel(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonInsert"), SIGNAL(clicked(bool)), this, SLOT(pushButtonInsert(bool)));
    }

  } // namespace utilities
} // namespace AstroManager
