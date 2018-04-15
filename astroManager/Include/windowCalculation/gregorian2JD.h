//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowCalculationGregorian2JD
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
//                            CWindowCalculationGregorian2JD
//
// HISTORY:             2017-07-06/GGB - File Created
//
//*********************************************************************************************************************************

#ifndef WINDOWCALCULATIONGREGORIAN2JD
#define WINDOWCALCULATIONGREGORIAN2JD

#include "windowCalculation.h"

namespace AstroManager
{
  namespace calculation
  {
    class CGregorian2JD : public CWindowCalculation
    {
      Q_OBJECT

    private:
      QLabel *tbJulianDay;
      QDateEdit *dwGregorian;

      void setupUI();

    protected:    

    public:
      CGregorian2JD(QWidget * = nullptr);

      virtual ESubWindowType getWindowType() const {return SWT_CALC_GREG2JD;}

      private slots:
        void eventCalculateJD(const QDate &);
      public slots:
        virtual void windowActivating() {}
    };

  } // namespace calculation
} // namespace AstroManager

#endif // WINDOWCALCULATIONGREGORIAN2JD

