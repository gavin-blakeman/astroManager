//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								CInstrumentDockwidget.h
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2010-2013, 2018 Gavin Blakeman.
//                      This file is part of the Astronomy Manager software(astroManager)
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
// CLASSES INCLUDED:    CInstrumentDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//
// HISTORY:             2012-11-18  GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04  GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef CINSTRUMENTDOCKWIDGET_H
#define CINSTRUMENTDOCKWIDGET_H

#include "dockWidgets/dockWidget.h"

#include "qtExtensions/qt.h"

namespace AstroManager
{
  namespace dockwidgets
  {

    class CInstrumentDockWidget : public QDockWidget
    {
      Q_OBJECT
    private:
      QAction *dwAction;

      QTabWidget *setupTab;
        //QWidget *tabPhotometrySetup;
        //	QComboBox *comboFilter;
        QWidget *tabTelescope;
          QCheckBox *cbAperture;
          QCheckBox *cbFRatio;
          QCheckBox *cbFocalLength;
          QSpinBox *sbAperture;
          QDoubleSpinBox *sbFRatio;
          QSpinBox *sbFocalLength;
        QWidget *tabInstrument;
          QCheckBox *cbCamera;
          QComboBox *comboCamera;
          QDoubleSpinBox *sbGain;
          QDoubleSpinBox *sbPixelX;
          QDoubleSpinBox *sbPixelY;
          QSpinBox *sbSensorX;
          QSpinBox *sbSensorY;
        QWidget *tabFOV;
          QLabel *tlPixX;
          QLabel *tlPixY;
          QLabel *tlFOVx;
          QLabel *tlFOVy;

      bool bInternalCheck;

      void setupUI();
      void UpdateFOV();

    protected:
    public:
      CInstrumentDockWidget(QAction *, QWidget * = 0);

    private slots:
      void eventFocalLengthChanged(int);
      void eventCheckAperture(int);
      void eventCheckFRatio(int);
      void eventCheckFocalLength(int);

      void eventApertureChanged(int);
      void eventFRatioChanged(double);

      void eventComboCameraIndexChanged(int);
      void eventSetupTabChanged(int);
    };

  } // namespace dockwidgets

}  // namespace AstroManager

#endif // CINSTRUMENTDOCKWIDGET_H
