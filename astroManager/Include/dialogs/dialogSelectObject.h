﻿//*********************************************************************************************************************************
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
//                      1. Enter the object name directly.
//                      2. Enter the object name and verify/expand against the SIMBAD database.
//                      3. Select the object name from the ATID database.
//
// CLASSES INCLUDED:    CSelectObjectDialog
//
// CLASS HIERARCHY:     CDialog
//                        - CSelectObjectDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-03-17 GGB - Removed CSelectObjectDialog into file DialogSelectObject.
//                      2013-03-17 GGB - Removed CImageControlWidget into file DockWidgetImageControl.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into it's own file
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - AIRDAS 0000.00.000 release.
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef __DIALOG_SELECTOBJECT_H
#define __DIALOG_SELECTOBJECT_H

  // AIRDAS files

#include "dialogs.h"

namespace AstroManager
{
  namespace dialogs
  {
    class CSelectObjectDialog : public dialogs::CDialog
    {
      Q_OBJECT

    private:
      QTabWidget *tabWidget;
        QWidget *tabManual;
          QLineEdit *lineEditObjectName;
        QWidget *tabATID;
          QCheckBox *checkObjectType;
          QComboBox *comboObjectType;
          QCheckBox *checkConstellation;
          QComboBox *comboConstellation;
          QCheckBox *checkMultiple;
          QCheckBox *checkExoPlanets;
          QLineEdit *editObjectName;
          QTableWidget *tableObjects;
          QPushButton *btnSearch;
        QWidget *tabSIMBAD;
        QWidget *tabRecent;
          QTableWidget *tableRecentObjects;
      QDialogButtonBox *buttonBox;

      QWidget *fromSimbad;

      QString &nameID_;

      void setupUI();
      void PopulateRecentObjects(void);

      void searchATID();
      void searchSIMBAD() {}

      void selectATID();
      void selectManual();
      void selectRecent() {}
      void selectSIMBAD();

    protected:
    public:
      static int startTab;

      CSelectObjectDialog(QString &, QWidget *);

    private slots:
      void eventBtnSearchClicked(bool);

      void eventCheckObjectTypeChanged(int);
      void eventCheckConstellationChanged(int);

      void eventButtonCancelClicked();
      void eventButtonSelectClicked();

      void eventTableObjectsCellClicked(int, int);

      void eventTableRecentCellClicked(int, int);
    };
  }
}

#endif
