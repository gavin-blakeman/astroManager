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
//                      Copyright 2018-2020 Gavin Blakeman.
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

// Miscellaneous library header files

#include <QCL>

namespace astroManager
{
  namespace dialogs
  {
    class CDialogEditResources : public QCL::CDialog
    {
      Q_OBJECT

    private:
      QListWidget *listWidgetSiteNames = nullptr;
      QLineEdit *lineEditSiteName = nullptr;
      QSpinBox *spinBoxLatitudeDegrees = nullptr;
      QSpinBox *spinBoxLatitudeMinutes = nullptr;
      QDoubleSpinBox *doubleSpinBoxLatitudeSeconds = nullptr;
      QComboBox *comboBoxNS = nullptr;
      QSpinBox *spinBoxLongitudeDegrees = nullptr;
      QSpinBox *spinBoxLongitudeMinutes = nullptr;
      QDoubleSpinBox *doubleSpinBoxLongitudeSeconds = nullptr;
      QComboBox *comboBoxEW = nullptr;
      QSpinBox *spinBoxAltitude = nullptr;
      QDoubleSpinBox *doubleSpinBoxTimeOffset = nullptr;
      QComboBox *comboBoxDaylightSaving = nullptr;
      QLineEdit *lineEditIAUCode = nullptr;
      QCheckBox *checkBoxDontDisplay = nullptr;
      QPushButton *pushButtonFilterDeletedSites = nullptr;
      QPushButton *pushButtonSiteSaveChanges = nullptr;
      QPushButton *pushButtonSiteRevertChanges = nullptr;
      bool bSitesDirty = false;

      QListWidget *listWidgetTelescopes = nullptr;
      QLineEdit *lineEditTelescopeName = nullptr;

      QLineEdit *lineEditObserverName = nullptr;
      QLineEdit *lineEditObserverEMail = nullptr;
      QLineEdit *lineEditObserverAAVSO = nullptr;

      QListWidget *listWidgetInstruments = nullptr;
      QPushButton *pushButtonFilterDeletedInstruments = nullptr;

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

