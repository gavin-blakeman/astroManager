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
#include "../qtExtensions/qt.h"
#include "../qtExtensions/queryModelPlanning.h"

#include <ACL>

namespace astroManager
{
  namespace windowPlanning
  {
    class CWindowPlanning : public CMdiSubWindow
    {
      Q_OBJECT

    private:
      QComboBox *comboBoxPlans = nullptr;
      QComboBox *comboBoxSites = nullptr;
      QDateEdit *dateEditSelectedDate = nullptr;
      QTimeEdit *timeEditSelectedTime = nullptr;
      QRadioButton *radioButtonUT = nullptr;
      QRadioButton *radioButtonLT = nullptr;
      QRadioButton *radioButtonST = nullptr;
      QPushButton *pushButtonTimeMinutePlus = nullptr;
      QPushButton *pushButtonTimeMinuteMinus = nullptr;
      QPushButton *pushButtonTimeHourPlus = nullptr;
      QPushButton *pushButtonTimeHourMinus = nullptr;
      QPushButton *pushButtonTimeDayPlus = nullptr;
      QPushButton *pushButtonTimeDayMinus = nullptr;
      QPushButton *pushButtonRealTime = nullptr;

      QTableView *tableViewPlanning = nullptr;

      QTE::CQueryModelPlanning queryModel;

      QTimer *timer1s = nullptr;        ///< 1s Timer used for updating the time as required.

      ACL::DTargetAstronomy planTargets;
      std::int_least32_t timeZoneOffset = 0;

      void setupUI();

    protected:
    public:
      CWindowPlanning(QWidget *);
      virtual ~CWindowPlanning() {}

      virtual EWindowClass getWindowClass() const { return WC_PLANNING; }
      virtual ESubWindowType getWindowType() const { return SWT_UTILITY_PLANNING; }
      virtual bool save() {}
      virtual bool saveAs() {}

    public slots:
      void eventTimer1s();

      virtual void windowActivating() {}
      virtual void currentIndexChangedPlans(int);

      virtual void radioButtonLTClicked(bool);
      virtual void radioButtonUTClicked(bool);
      virtual void radioButtonLSTClicked(bool);

      virtual void pushButtonRealTimeClicked(bool);

      virtual void comboBoxSiteCurrentIndexChanged(int);
    };
  }
}

#endif // WINDOWPLANNING_H

