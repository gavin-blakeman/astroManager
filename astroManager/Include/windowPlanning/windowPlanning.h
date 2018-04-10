//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:                windowPlanning
// SUBSYSTEM:           Window for observation planning.
// LANGUAGE:            C++
// TARGET OS:           WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:  Qt
// NAMESPACE:           AstroManager::widgets
// AUTHOR:              Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
//                      This file is part of the AstroManager software.
//
//                      AstroManager is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      AstroManager is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with AstroManager.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:
//
// CLASSES INCLUDED:    CWindowPlanning
//
// CLASS HIERARCHY:     CMdiSubWindow
//                        - CWindowPlanning
//
// HISTORY:             2018-02-03 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef WINDOWPLANNING_H
#define WINDOWPLANNING_H

#include "../qtExtensions/MdiSubWindow.h"

#include <ACL>

namespace AstroManager
{
  namespace windowPlanning
  {
    class CWindowPlanning : public CMdiSubWindow
    {
    private:
      QComboBox *comboBoxPlans = nullptr;
      QComboBox *comboBoxSites = nullptr;

      QTableView *tableViewPlanning = nullptr;

      ACL::DTargetAstronomy planTargets;

      void setupUI();

    protected:
    public:
      CWindowPlanning(QWidget *);

      virtual EWindowClass getWindowClass() const { return WC_PLANNING; }
      virtual ESubWindowType getWindowType() const { return SWT_WEATHER_HISTORY; }
      virtual bool save() {}
      virtual bool saveAs() {}

    public slots:
      virtual void windowActivating() {}
      virtual void currentIndexChangedPlans(int);
    };
  }
}

#endif // WINDOWPLANNING_H

