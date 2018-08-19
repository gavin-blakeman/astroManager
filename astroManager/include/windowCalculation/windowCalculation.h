//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowCalculation
// SUBSYSTEM:						Subclass for calculation windows
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CWindowCalculation
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        CMdiSubWindow
//                          CWindowCalculation
//
// HISTORY:             2017-07-06/GGB - File Created
//
//*********************************************************************************************************************************

#ifndef WINDOWCALCULATION
#define WINDOWCALCULATION

#include "../qtExtensions/MdiSubWindow.h"

namespace astroManager
{
  namespace calculation
  {
    class CWindowCalculation : public CMdiSubWindow
    {
    private:
      virtual bool save() { return false; }
      virtual bool saveAs() {return false; }

    protected:
      virtual void closeEvent(QCloseEvent *);

    public:
      CWindowCalculation(QWidget *);
      virtual EWindowClass getWindowClass() const { return WC_NULL; }
    };

  } // namespace calculation
} // namespace AstroManager

#endif // WINDOWCALCULATION

