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

#include "../../include/dialogs/dialogEditResources.h"

#include "../../include/ACL/observatoryInformation.h"
#include "../../include/database/databaseARID.h"

#include <cmath>

namespace astroManager
{
  namespace dialogs
  {
    /// @brief Class constructor
    /// @throws GCL::CError()
    /// @version 2018-02-02/GGB - Function created.

    CDialogEditResources::CDialogEditResources(QWidget *parent) : CDialog(":/dialogs/dialogEditResources.ui", parent)
    {
      setupUI();
    }

    /// @brief Function is called when any data changes on the sites tab. This function then updates button states.
    /// @throws None.
    /// @version 2018-02-03/GGB - Function created.

    void CDialogEditResources::eventSitesDataChanged()
    {
      bSitesDirty = true;
      pushButtonSiteSaveChanges->setEnabled(true);
      pushButtonSiteRevertChanges->setEnabled(true);
    }

    /// @brief Event handler for the site item being changed.
    /// @throws None.
    /// @version 2018-02-02/GGB - Function created.

    void CDialogEditResources::listWidgetSiteNamesSelectionChanged()
    {
      std::uint_least16_t dd;
      FP_t ss;

      CObservatory site;
      database::databaseARID->getObservingSite(listWidgetSiteNames->currentItem()->data(Qt::UserRole).toULongLong(), &site);

        // Disable the save and revert buttons

      pushButtonSiteSaveChanges->setEnabled(false);
      pushButtonSiteRevertChanges->setEnabled(false);

      lineEditSiteName->setText(QString::fromStdString(site.siteName()));

      ss = std::abs(site.latitude());
      dd = static_cast<std::uint_least16_t>(std::floor(ss));
      ss -= dd;
      ss *= 60;

      spinBoxLatitudeDegrees->setValue(dd);

      dd = static_cast<std::uint_least16_t>(std::floor(ss));
      ss -= dd;
      ss *= 60;

      spinBoxLatitudeMinutes->setValue(dd);
      doubleSpinBoxLatitudeSeconds->setValue(ss);

      if (site.latitude() >= 0)
      {
        comboBoxNS->setCurrentText("N");
      }
      else
      {
        comboBoxNS->setCurrentText("S");
      }

      ss = std::abs(site.longitude());
      dd = static_cast<std::uint_least16_t>(std::floor(ss));
      ss -= dd;
      ss *= 60;

      spinBoxLongitudeDegrees->setValue(dd);

      dd = static_cast<std::uint_least16_t>(std::floor(ss));
      ss -= dd;
      ss *= 60;

      spinBoxLongitudeMinutes->setValue(dd);
      doubleSpinBoxLongitudeSeconds->setValue(ss);

      if (site.longitude() >= 0)
      {
        comboBoxEW->setCurrentText("E");
      }
      else
      {
        comboBoxEW->setCurrentText("W");
      };
      spinBoxAltitude->setValue(site.altitude());
      doubleSpinBoxTimeOffset->setValue(site.timeZone());
      //QComboBox *comboBoxDaylightSaving;
      lineEditIAUCode->setText(QString::fromStdString(site.IAUCode()));
      checkBoxDontDisplay->setChecked(site.dontDisplay());
    }

    /// @brief Responds to the filter deleted Instruments
    /// @param[in] isChecked - Details if the filter button is checked or not.
    /// @throws None.
    /// @version 2018-02-03/GGB - Function created.

    void CDialogEditResources::pushButtonFilterDeletedInstrumentsClicked(bool isChecked)
    {

      listWidgetInstruments->clear();

        // Now fill the listWidget with the data for the sites

      database::databaseARID->populateListInstrumentNames(listWidgetInstruments, isChecked);
    }

    /// @brief Responds to the filter deleted sites
    /// @param[in] isChecked - Details if the filter button is checked or not.
    /// @throws None.
    /// @version 2018-02-02/GGB - Function created.

    void CDialogEditResources::pushButtonFilterDeletedSitesClicked(bool isChecked)
    {

      listWidgetSiteNames->clear();

        // Now fill the listWidget with the data for the sites

      database::databaseARID->populateListSiteNames(listWidgetSiteNames, isChecked);
    }

    /// @brief Sets up the information in the observer tab.
    /// @throws None.
    /// @version 2018-02-03/GGB - Function created.

    void CDialogEditResources::setupInstruments()
    {
        // Setup the pointers to the various widgets

      listWidgetInstruments = findChild<QListWidget *>("listWidgetInstruments");
      pushButtonFilterDeletedInstruments = findChild<QPushButton *>("pushButtonFilterDeletedInstruments");

        // Now fill the listWidget with the data for the sites.

      database::databaseARID->populateListInstrumentNames(listWidgetInstruments, pushButtonFilterDeletedInstruments->isChecked());
    }

    /// @brief Sets up the information for the observer tab
    /// @throws None.
    /// @version 2018-02-03/GGB - Function created.

    void CDialogEditResources::setupObserver()
    {
        // Setup the pointers to the various widgets

      lineEditObserverName = findChild<QLineEdit *>("lineEditObserverName");
      lineEditObserverEMail = findChild<QLineEdit *>("lineEditObserverEMail");
      lineEditObserverAAVSO = findChild<QLineEdit *>("lineEditObserverAAVSO");

        // Now fill the listWidget with the data for the sites.
    }

    /// @brief Setsup the initial data for the site information.
    /// @throws GCL::CRuntimeError
    /// @version 2018-02-02/GGB - Function created.

    void CDialogEditResources::setupSite()
    {
        // Setup the pointers to the various widgets

      listWidgetSiteNames = findChild<QListWidget *>("listWidgetSiteNames");
      lineEditSiteName = findChild<QLineEdit *>("lineEditSiteName");
      spinBoxLatitudeDegrees = findChild<QSpinBox *>("spinBoxLatitudeDegrees");
      spinBoxLatitudeMinutes = findChild<QSpinBox *>("spinBoxLatitudeMinutes");
      doubleSpinBoxLatitudeSeconds = findChild<QDoubleSpinBox *>("doubleSpinBoxLatitudeSeconds");
      comboBoxNS = findChild<QComboBox *>("comboBoxNS");
      spinBoxLongitudeDegrees = findChild<QSpinBox *>("spinBoxLongitudeDegrees");
      spinBoxLongitudeMinutes = findChild<QSpinBox *>("spinBoxLongitudeMinutes");
      doubleSpinBoxLongitudeSeconds = findChild<QDoubleSpinBox *>("doubleSpinBoxLongitudeSeconds");
      comboBoxEW = findChild<QComboBox *>("comboBoxEW");
      spinBoxAltitude = findChild<QSpinBox *>("spinBoxAltitude");
      doubleSpinBoxTimeOffset = findChild<QDoubleSpinBox *>("doubleSpinBoxTimeOffset");
      comboBoxDaylightSaving = findChild<QComboBox *>("comboBoxDaylightSaving");
      lineEditIAUCode = findChild<QLineEdit *>("lineEditIAUCode");
      checkBoxDontDisplay = findChild<QCheckBox *>("checkBoxDontDisplay");
      pushButtonFilterDeletedSites = findChild<QPushButton *>("pushButtonFilterDeletedSites");
      pushButtonSiteSaveChanges = findChild<QPushButton *>("pushButtonSiteSaveChanges");
      pushButtonSiteRevertChanges = findChild<QPushButton *>("pushButtonSiteRevertChanges");

        // Now fill the listWidget with the data for the sites.

      database::databaseARID->populateListSiteNames(listWidgetSiteNames, pushButtonFilterDeletedSites->isChecked());

        // Set button states

      pushButtonSiteSaveChanges->setEnabled(false);
      pushButtonSiteRevertChanges->setEnabled(false);

        // Now connect any signals

      connect(pushButtonFilterDeletedSites, SIGNAL(clicked(bool)), this, SLOT(pushButtonFilterDeletedSitesClicked(bool)));
      connect(listWidgetSiteNames, SIGNAL(itemSelectionChanged()), this, SLOT(listWidgetSiteNamesSelectionChanged()));

      connect(lineEditSiteName, SIGNAL(textEdited(const QString &)), this, SLOT(eventSitesDataChanged()));
      connect(spinBoxLatitudeDegrees, SIGNAL(editingFinished()), this, SLOT(eventSitesDataChanged()));
    }

    /// @brief Setsup the initial data for the telescope information.
    /// @throws GCL::CRuntimeError
    /// @version 2018-02-02/GGB - Function created.

    void CDialogEditResources::setupTelescope()
    {
        // Setup the pointers to the various widgets

      listWidgetTelescopes = findChild<QListWidget *>("listWidgetTelescopes");
      lineEditTelescopeName = findChild<QLineEdit *>("lineEditTelescopeName");

        // Now fill the listWidget with the data for the sites.

      database::databaseARID->populateListTelescopeNames(listWidgetTelescopes, false);
    }

    /// @brief Function to ensure that the initial information and state is correctly set.
    /// @throws GCL::CCodeError
    /// @version 2018-02-02/GGB - Function created.

    void CDialogEditResources::setupUI()
    {
      setupSite();
      setupObserver();
      setupTelescope();
      setupInstruments();
    }
  } // namespace dialogs
} // namespace AstroManager
