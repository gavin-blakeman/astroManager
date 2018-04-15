//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Manager)
// FILE:								dialogEditResources
// SUBSYSTEM:						Edit/Add Resources (Sites, Observers, Telescopes, Imagers)
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
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
// OVERVIEW:            Dialog to allow editing of resources (master data) for various parts of the application.
//
// CLASSES INCLUDED:    CDialogEditResources
//
// CLASS HIERARCHY:     CDialog
//                        - CDialogEditResources
//                        - CSelectLocationDialog
//						            - CSearchSelectObjectDialog
//                        - CImageFloatDialog
//                        - CImageResampleDialog
//
// HISTORY:             2018-02-01 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef DIALOGEDITRESOURCES_H
#define DIALOGEDITRESOURCES_H

#include "dialogs.h"

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogEditResources : public CDialog
    {
      Q_OBJECT

    private:
      QListWidget *listWidgetSiteNames;
      QLineEdit *lineEditSiteName;
      QSpinBox *spinBoxLatitudeDegrees;
      QSpinBox *spinBoxLatitudeMinutes;
      QDoubleSpinBox *doubleSpinBoxLatitudeSeconds;
      QComboBox *comboBoxNS;
      QSpinBox *spinBoxLongitudeDegrees;
      QSpinBox *spinBoxLongitudeMinutes;
      QDoubleSpinBox *doubleSpinBoxLongitudeSeconds;
      QComboBox *comboBoxEW;
      QSpinBox *spinBoxAltitude;
      QDoubleSpinBox *doubleSpinBoxTimeOffset;
      QComboBox *comboBoxDaylightSaving;
      QLineEdit *lineEditIAUCode;
      QCheckBox *checkBoxDontDisplay;
      QPushButton *pushButtonFilterDeletedSites;
      QPushButton *pushButtonSiteSaveChanges;
      QPushButton *pushButtonSiteRevertChanges;
      bool bSitesDirty = false;

      QListWidget *listWidgetTelescopes;
      QLineEdit *lineEditTelescopeName;

      QLineEdit *lineEditObserverName;
      QLineEdit *lineEditObserverEMail;
      QLineEdit *lineEditObserverAAVSO;

      QListWidget *listWidgetInstruments;
      QPushButton *pushButtonFilterDeletedInstruments;

      void setupUI();
        void setupSite();
        void setupTelescope();
        void setupObserver();
        void setupInstruments();

    protected:
    public:
      CDialogEditResources(QWidget * = nullptr);

    public slots:
      void pushButtonFilterDeletedSitesClicked(bool);
      void pushButtonFilterDeletedInstrumentsClicked(bool);
      void listWidgetSiteNamesSelectionChanged();
      void eventSitesDataChanged();
    };
  }
}

#endif // DIALOGEDITRESOURCES_H

