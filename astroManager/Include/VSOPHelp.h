//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:			          VSOPHelp.h
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imagedisplay
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2009-2017 Gavin Blakeman.
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
// OVERVIEW:	          Classes for the Help Menu Items. The Qt API is used to create system independent code.
//
//
// CLASSES INCLUDED:    CHelpAIP          - Help About the application
//                      CHelpATID         - Help about the ATID database
//                      CHelpAboutLibRaw  - Help about the LibRaw Library
//
// CLASS HIERARCHY:     CDialog
//                        - CHelpAIP
//                        - CHelpAboutLibRaw
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2012-01-01 GGB - Development of classes for openAIP
//
//*********************************************************************************************************************************

#ifndef VSOPHELP_H
#define VSOPHELP_H

#include <Qt>

  // AIRDAS Include Files

#include "dialogs/dialogs.h"
#include "Error.h"

namespace AstroManager
{
  namespace help
  {
    class CHelpAboutAIP : public dialogs::CDialog
    {
    private:
      void setupUI();

    protected:
    public:
      CHelpAboutAIP();
    };

    class CHelpATID : public QObject
    {
      Q_OBJECT

    private:
      QDialog *p;

      void setupDialog(void);
      QTableWidget *twSummary;
      QTableWidget *twVersion;
      QTableWidget *twObjects;

      void PopulateCatalogInfo(void);
      void PopulateSourceSummary(void);
      void PopulateObjectSummary(void);

    protected:
    public:
      CHelpATID(void);
      virtual ~CHelpATID(void);
      void setParent(QDialog *parent);

    public slots:
      void setData(void);
    };

  }   // namespace help
}		// namespace AstroManager


#endif // VSOPHELP_H
