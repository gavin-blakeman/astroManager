//*********************************************************************************************************************************
//
// PROJECT:							astroManager
// FILE:			          astroManagerHelp
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2009-2018 Gavin Blakeman.
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
// OVERVIEW:	          Classes for the Help Menu Items. The Qt API is used to create system independent code.
//
// CLASSES INCLUDED:    CHelpAIP          - Help About the application
//                      CHelpATID         - Help about the ATID database
//
// CLASS HIERARCHY:     CDialog
//                        - CHelpAIP
//
// HISTORY:		          2018-02-03 GGB - Rename Project to astroManager
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-06-26 GGG - Removed class CHelpAboutLibRaw
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release
//                      2012-01-01 GGB - Development of classes for openAIP
//
//*********************************************************************************************************************************

#include "../Include/astroManagerHelp.h"

  // Qt Framework files

#include "../Include/qtExtensions/qt.h"

  // AIRDAS include files

#include "../Include/database/database.h"
#include "../Include/database/databaseATID.h"
#include "../Include/astroManager.h"

  // Standard header files

#include <cstdio>

  // Other libraries

#include <GCL>

// The following are defined in cfitsio and need to be undefined to be able to include libraw (winnt.h).
// As cfitsio is never accessed directly by the code in this library, this will not be a concern.

#ifdef WIN32
#undef TBYTE
#endif

  // LibRaw support library

#ifdef USE_LIBRAW
#include "libraw/libraw.h"
#endif

namespace AstroManager
{
  namespace help
  {

    /// @brief Sets the parent window of the dialog to allow the class to update the table controls in the dialog.
    /// @throws None.
    /// @version 2010-05-10/GGB - Function created.

    void CHelpATID::setParent(QDialog *parent)
    {
      p = parent;
      setupDialog();
    }

    void CHelpATID::setupDialog(void)
    {
      twVersion = p->findChild<QTableWidget *>("twVersion");
      twSummary = p->findChild<QTableWidget *>("twSummary");
      twObjects = p->findChild<QTableWidget *>("twObjects");

      PopulateCatalogInfo();
      PopulateSourceSummary();
      PopulateObjectSummary();
    }

    // Function called to set the data in the table controls of the dialog.
    //
    // Revision:		1.00
    // Date:
    // Written by:

    void CHelpATID::setData()
    {
    }

    /// @brief Class constructor.
    /// @throws None.
    /// @version 2010-05-11/GGB - Function created,

    CHelpATID::CHelpATID(void)
    {
      twVersion = twSummary = twObjects = nullptr;
    }

    /// @brief Class Destructor.
    /// @details Ensures that the parent window registers this window as closed.
    /// @throws None.
    /// @version 2010-05-11/GGB - Function created.

    CHelpATID::~CHelpATID(void)
    {
      twVersion = twSummary = twObjects = nullptr;
    }


  // Populates the catalogue information page.
  // The information populated includes:
  //																			The Version of the catalogue
  //																			The status of the catalogue
  //																			The publish date of the catalogue
  //																			The number of objects in the catalogue
  //
  // 2009-11-18/GGB - Function created.

  void CHelpATID::PopulateCatalogInfo()
  {
    if (!database::databaseATID->usingSIMBAD())
    {
      QSqlQuery query(database::databaseATID->database());
      QTableWidgetItem *newItem;

      query.exec(*new QString("SELECT vi.REVISION, vi.STATUS, vi.PUBLISHDATE, vi.TARGETCOUNT, vi.IDENTIFIERCOUNT FROM TBL_VERSIONINFO vi ORDER BY vi.REVISION DESC"));

      query.next();			// Move to first row of query

      twVersion->setRowHeight(0, 20);
      newItem = new QTableWidgetItem(tr("Catalog Version"));
      twVersion->setItem(0, 0, newItem);
      newItem = new QTableWidgetItem(*(new QString(query.value(0).toString())));
      twVersion->setItem(0, 1, newItem);

      twVersion->setRowHeight(1, 20);
      newItem = new QTableWidgetItem(tr("Catalog Status"));
      twVersion->setItem(1, 0, newItem);
      newItem = new QTableWidgetItem(*(new QString(query.value(1).toString())));
      twVersion->setItem(1, 1, newItem);

      twVersion->setRowHeight(2, 20);
      newItem = new QTableWidgetItem(tr("Publish Date"));
      twVersion->setItem(2, 0, newItem);
      newItem = new QTableWidgetItem(*(new QString(query.value(2).toDateTime().toString("dd-MMM-yyyy"))));
      twVersion->setItem(2, 1, newItem);

      twVersion->setRowHeight(3, 20);
      newItem = new QTableWidgetItem(tr("Number of Targets"));
      twVersion->setItem(3, 0, newItem);
      newItem = new QTableWidgetItem(*(new QString(query.value(3).toString())));
      twVersion->setItem(3, 1, newItem);

      twVersion->setRowHeight(4, 20);
      newItem = new QTableWidgetItem(tr("Total Identifiers"));
      twVersion->setItem(4, 0, newItem);
      newItem = new QTableWidgetItem(*(new QString(query.value(4).toString())));
      twVersion->setItem(4, 1, newItem);
    };
  }

  // Populates the Source Summary List View by counting the items in the catalogue.
  //
  // 2010-05-12/GGB - Changed to use the Qt API.
  // 2009-11-17/GGB - Function created.

  void CHelpATID::PopulateSourceSummary(void)
  {
    if (!database::databaseATID->usingSIMBAD())
    {
      QString szSQL("SELECT TBL_CATALOG.CATALOG_NAME, TBL_CATALOG.ABBREVIATION, TBL_CATALOG.MASK, Count(TBL_NAMES.Name) AS CountOfName " \
                    "FROM TBL_CATALOG INNER JOIN TBL_NAMES ON TBL_CATALOG.CATALOG_ID = TBL_NAMES.CATALOG_ID " \
                    "GROUP BY TBL_CATALOG.Catalog_Name, TBL_CATALOG.Abbreviation, TBL_CATALOG.Mask " \
                    "ORDER BY TBL_CATALOG.Catalog_Name");
      QSqlQuery query(database::databaseATID->database());
      QTableWidgetItem *newItem;
      int nRow = 0;

      query.exec(szSQL);

      QSqlError err = query.lastError();

      while (	query.next() )
      {
        twSummary->insertRow(nRow);
        twSummary->setRowHeight(nRow, 20);
        newItem = new QTableWidgetItem(*(new QString(query.value(0).toString())));
        twSummary->setItem(nRow, 0, newItem);
        newItem = new QTableWidgetItem(*(new QString(query.value(1).toString())));
        twSummary->setItem(nRow, 1, newItem);
        newItem = new QTableWidgetItem(*(new QString(query.value(2).toString())));
        twSummary->setItem(nRow, 2, newItem);
        newItem = new QTableWidgetItem(*(new QString(query.value(3).toString())));
        twSummary->setItem(nRow, 3, newItem);
        nRow++;
      };
    };
  }

  // Populates the List View with the count of object types.
  //
  // 2010-05-12/GGB - Changed to use the Qt API.
  // 2009-11-19/GGB - Function created.

  void CHelpATID::PopulateObjectSummary(void)
  {
    if (!database::databaseATID->usingSIMBAD())
    {
      QString szSQL("SELECT TBL_OBJECTTYPES.ObjectType, Count(TBL_STELLAROBJECTS.OBJECT_ID) AS CountOfOBJECT_ID " \
                    "FROM TBL_OBJECTTYPES INNER JOIN TBL_STELLAROBJECTS ON TBL_OBJECTTYPES.OBJECTTYPE_ID = TBL_STELLAROBJECTS.ObjectType_ID " \
                    "GROUP BY TBL_OBJECTTYPES.ObjectType");
      QSqlQuery query(database::databaseATID->database());
      QTableWidgetItem *newItem;
      int nRow = 0;

      query.exec(*new QString(szSQL));

      while (	query.next() )
      {
        twObjects->insertRow(nRow);
        twObjects->setRowHeight(nRow, 20);
        newItem = new QTableWidgetItem(*(new QString(query.value(0).toString())));
        twObjects->setItem(nRow, 0, newItem);
        newItem = new QTableWidgetItem(*(new QString(query.value(1).toString())));
        twObjects->setItem(nRow, 1, newItem);
        nRow++;
      };
    };
  };

    //*****************************************************************************************************************************
    //
    // CHelpAboutAstroManager
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @throws None.
    /// @version 2012-01-01/GGB - Function created.

    CHelpAboutAstroManager::CHelpAboutAstroManager() : dialogs::CDialog(":/forms/dialogAboutAIP.ui")
    {
      setupUI();
    }

    /// @brief Sets up the UI. Displays version information as well as license information.
    /// @throws CCodeError(AIRDAS)
    /// @version 2017-06-25/GGB - Use new findChild<>() function in CDialog.
    /// @version 2013-06-01/GGB - Added support for a dynamic description.
    /// @version 2013-03-03/GGB - Function created.

    void CHelpAboutAstroManager::setupUI()
    {
      QFile file(":/text/GPLv2.htm");

      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        CODE_ERROR(AIRDAS);
      }
      else
      {
        QTextStream data(&file);
        QString gpl3 = data.readAll();
        dlg->findChild<QTextBrowser *>("textBrowserLicense")->setText(gpl3);
       };

      findChild<QLabel *>("labelRelease")->setText(QString::fromStdString(getReleaseString()));
      findChild<QTextBrowser *>("textBrowserCopyright")->setText(getCopyrightString());
      findChild<QTextBrowser *>("textBrowserDescription")->setText(getDescription());
      findChild<QLabel *>("labelReleaseDate")->setText(QString::fromStdString(getReleaseDate()));
    }

  }	// namespace help
}	// namespace AstroManager
