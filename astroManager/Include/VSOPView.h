//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								VSOPView.h
// SUBSYSTEM:						Object Information Child Window Class
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AIRDAS
// AUTHOR:							Gavin Blakeman.
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
// OVERVIEW:            Implements classes for the View Menu in the ARPAA application
//
// CLASSES INCLUDED:    CObjectInfoWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CObjectInfoWindow
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef AIRDAS_VSOPVIEW_H
#define AIRDAS_VSOPVIEW_H

  // Includes

#include "FrameWindow.h"
#include "qtExtensions/qt.h"

namespace AstroManager
{

  class CObjectInfoWindow : public CMdiSubWindow
  {
    Q_OBJECT

  private:

      // General Information Tab

    QPushButton *buttonSearch;
    QLineEdit *lineEditObjectName;
    QLabel *tbSelectedObject;
    QLabel *tbConstellation;
    QLabel *tbObjectType;
    QLabel *tbSpectralType;
    QLabel *tbSpectrum;
    QLabel *tbRedShift;
    QLabel *tbIsVariable;
    QLabel *tbIsMultiple;
    QLabel *tbHasExoplanets;
    QLabel *tbComments;

      // Position Information Tab

    QTableWidget *twPosition;

      // Photometry Tab

    //QLabel *tbIsPhotometryCheck;
    QLabel *tbIsPrimaryStandard;
    QLabel *tbIsLandoltStandard;
    QLabel *tbIsAAVSOStandard;
    QLabel *tbIsJohnsonStandard;
    QLabel *tbIsCousinsStandard;
    QLabel *tbAAVSOLabel;
    QTableWidget *twMagnitudes;

      // Alternate names

    QTableWidget *twAlternateNames;

      // Variable information

    QLabel *tbEpoch;
    QLabel *tbPeriod;
    QTableWidget *twVarMagnitudes;
    QLabel *tbVarComments;


    void ClearFields();
    void SetVariableTab(QVariant *);

  protected:
  public:
    CObjectInfoWindow(QWidget * = nullptr);
    virtual ~CObjectInfoWindow();

    virtual ESubWindowType getWindowType() const {return SWT_OBJECT_INFORMATION;}

    virtual bool save() { return false;}
    virtual bool saveAs() { return false;}

  private slots:
    void eventSearch();	// Processes the search command.
    virtual void windowActivating() {}

  };

}		// namespace

#endif  // AIRDAS_VSOPVIEW_H
