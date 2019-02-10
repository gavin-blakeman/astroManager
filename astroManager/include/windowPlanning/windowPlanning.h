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

  // Standard C++ library header files.

#include <memory>
#include <vector>

  // astroManager header files.

#include "../ACL/targetAstronomy.h"
#include "../ACL/telescope.h"
#include "../qtExtensions/MdiSubWindow.h"

  // Miscellaneous library header files.

#include <ACL>
#include <QCL>

namespace astroManager
{
  namespace windowPlanning
  {
    class CWindowPlanning final : public CMdiSubWindow
    {
      Q_OBJECT

    private:
      std::vector<std::unique_ptr<CTargetAstronomy>> targetList;
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

      QTableWidget *tableWidgetPlanning = nullptr;

      QTimer *timer1s = nullptr;        ///< 1s Timer used for updating the time as required.

      std::int_least32_t timeZoneOffset = 0;

      void setupUI();
      void loadPlanData();

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

      virtual void windowActivating() override;
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

