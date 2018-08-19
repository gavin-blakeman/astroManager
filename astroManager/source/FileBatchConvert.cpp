//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								FileBatchConvert
// SUBSYSTEM:						File | Batch Convert - Menu Handler
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::file
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
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
// OVERVIEW:            Implements the File | Batch convert menu option. Allows a number of files to be opened and converted to
//                      FITS, with attributes being applied and the file being registered.
//
// CLASSES INCLUDED:    CARPAASubWindow
//                      CFrameWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-01-23 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../include/FileBatchConvert.h"

  // astroManager include files

#include "../include/Settings.h"

  // Boost library

#include "boost/filesystem.hpp"

namespace astroManager
{
  namespace file
  {

      // Defined flag values for storing the state of the window in the registry.

    const int SITE_ENABLED  = 0x0001;
    const int SITE_DEFAULT  = 0x0002;
    const int SITE_MANUAL   = 0x0004;

    const int WEATHER_ENABLED(0x0001);
    const int WEATHER_USEDATABASE(0x0002);
    const int WEATHER_MANUAL(0x0004);

    const int OBSERVER_ENABLED  = 0x0001;
    const int OBSERVER_DEFAULT  = 0x0002;
    const int OBSERVER_MANUAL   = 0X0004;

    const int ROLE_PATH(Qt::UserRole + 0);

    /// Class constructor
    //
    // 2012-02-02/GGB - Function created.

    CFileBatchConvertDialog::CFileBatchConvertDialog() : CDialog(":/forms/dialogBatchConvert.ui")
    {
      setupUI();
    }

    void CFileBatchConvertDialog::eventConvertImages(bool)
    {
    }

    void CFileBatchConvertDialog::eventEnterObserver(bool)
    {
    }


    /// Bring up a site selection dialog to allow a site to be selected.
    //
    // 2012-02-12/GGB - Function created.

    void CFileBatchConvertDialog::eventEnterSite(bool)
    {
      dialogs::CSelectSiteDialog siteDialog(siteID);

      if (siteDialog.exec())
      {
          // Update the display of the site information.
      }
    }

    /// Bring up a dialog to allow the weather to be entered.
    //
    //

    void CFileBatchConvertDialog::eventEnterWeather(bool)
    {
    }

    /// Remove the selected files from the list of files.
    //
    //

    void CFileBatchConvertDialog::eventRemoveFiles(bool)
    {
    }

    /// Allows the user to select the directory for the output of the files.
    //
    // 2015-01-04/GGB - Changed Macro constants to C++ constants.
    // 2012-02-04/GGB - Function created.

    void CFileBatchConvertDialog::eventSelectDirectory(bool)
    {
      QString dir = QFileDialog::getExistingDirectory(dlg, tr("Output Directory"),
                                                 settings::astroManagerSettings->value(settings::DIALOG_BATCHCONVERT_OUTPUTDIRECTORY).toString(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
      if (dir != "")
      {
        labelOutputDirectory->setText(dir);
        settings::astroManagerSettings->setValue(settings::DIALOG_BATCHCONVERT_OUTPUTDIRECTORY, dir);
      };
    }

    /// Function to allow the user to select files for conversion
    //
    // 2012-02-05/GGB - Function created.

    void CFileBatchConvertDialog::eventSelectFiles(bool)
    {
      QStringList newImages;
      QStringList::const_iterator constIterator;
      boost::filesystem::path filePath;
      bool firstPass = true;
      QListWidgetItem *lwi = NULL;

        // Bring up the dialog box to allow the user to select files.

      newImages = QFileDialog::getOpenFileNames(dlg, tr("Select Files"),
        settings::astroManagerSettings->value(settings::IMAGING_DIRECTORY, QVariant("")).toString(),
                    tr("Image Files (*.ST7 *.DNG)"));

         // Iterate the list of items and only add items that are not repeats.

      for (constIterator = newImages.constBegin(); constIterator != newImages.constEnd(); ++constIterator)
        if ( !(imageList.contains((*constIterator), Qt::CaseInsensitive)) )
        {
            // If on the first pass through the list, then the directory associated with the list
            // is saved if necessary to allow reopen to the same directory.

          filePath = (*constIterator).toStdString();

          if ( firstPass )
          {	// Get the directory details from the file name.
            settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(filePath.parent_path().string().c_str()));
            firstPass = false;
          };

          imageList << (*constIterator);	// Insert the string into the image list.

            // Add the filename into the list. Strip off the path and just use the filename.
            // Add the red icon to show that the image does not have alignment points marked at this stage.

          lwi = new QListWidgetItem(filePath.filename().string().c_str());
          lwi->setData(ROLE_PATH , QVariant(*constIterator) );		// Add the complete path as user data.
          listWidgetFiles->addItem(lwi);
          lwi = NULL;
        };
    };

    /// Function to handle the clicked message for the radio button.
    //
    // R/D/A: 1.00/2012-02-12/GGB

    void CFileBatchConvertDialog::radioButtonObserverDefaultClicked(bool)
    {
      pushButtonObserverSelect->setEnabled(false);
    };

    /// Function to handle the clicked message for the radio button.
    //
    // R/D/A: 1.00/2012-02-12/GGB

    void CFileBatchConvertDialog::radioButtonObserverManualClicked(bool)
    {
      pushButtonObserverSelect->setEnabled(true);
    };

    /// Function to handle the clicked message for the radio button.
    //
    // R/D/A: 1.00/2012-02-12/GGB

    void CFileBatchConvertDialog::radioButtonSiteDefaultClicked(bool)
    {
      pushButtonEnterSite->setEnabled(false);
    };

    /// Function to handle the clicked message for the radio button.
    //
    // 2012-02-12/GGB - Function created.

    void CFileBatchConvertDialog::radioButtonSiteManualClicked(bool)
    {
      pushButtonEnterSite->setEnabled(true);
    }

    /// Function to handle the clicked message for the radio button.
    //
    // 2012-02-12/GGB - Function created.

    void CFileBatchConvertDialog::radioButtonWeatherDatabaseClicked(bool)
    {
      pushButtonWeatherEnter->setEnabled(false);
    }

    /// Function to handle the clicked message for the radio button.
    //
    // 2012-02-12/GGB - Function created.

    void CFileBatchConvertDialog::radioButtonWeatherManualClicked(bool)
    {
      pushButtonWeatherEnter->setEnabled(true);
    }

    /// Function to setup the user interface elements.
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2015-01-04/GGB - Changed macro constants to C++ contants.
    /// @version 2012-02-02/GGB - Function created.

    void CFileBatchConvertDialog::setupUI()
    {
      int flagValue;

        // Link all the interface elements.

      listWidgetFiles = dlg->findChild<QListWidget *>("listWidgetFiles");
      labelOutputDirectory = dlg->findChild<QLabel *>("labelOutputDirectory");
      groupBoxObservationSite = dlg->findChild<QGroupBox *>("groupBoxObservationSite");
        radioButtonSiteDefault = dlg->findChild<QRadioButton *>("radioButtonSiteDefault");
        radioButtonSiteManual = dlg->findChild<QRadioButton *>("radioButtonSiteManual");
        pushButtonEnterSite = dlg->findChild<QPushButton *>("pushButtonEnterSite");
      groupBoxWeather = dlg->findChild<QGroupBox *>("groupBoxWeather");
        radioButtonWeatherDatabase = dlg->findChild<QRadioButton *>("radioButtonWeatherDatabase");
        radioButtonWeatherManual = dlg->findChild<QRadioButton *>("radioButtonWeatherManual");
        pushButtonWeatherEnter = dlg->findChild<QPushButton *>("pushButtonWeatherEnter");
      groupBoxObserver = dlg->findChild<QGroupBox *>("groupBoxObserver");
        radioButtonObserverDefault = dlg->findChild<QRadioButton *>("radioButtonObserverDefault");
        radioButtonObserverManual = dlg->findChild<QRadioButton *>("radioButtonObserverManual");
        pushButtonObserverSelect = dlg->findChild<QPushButton *>("pushButtonObserverSelect");

        // Load defaults from the registry and populate as required.

      labelOutputDirectory->setText(settings::astroManagerSettings->value(settings::DIALOG_BATCHCONVERT_OUTPUTDIRECTORY, QString("")).toString());

      flagValue = settings::astroManagerSettings->value(settings::DIALOG_BATCHCONVERT_SITE).toInt();
      if (flagValue & SITE_DEFAULT)
      {
        radioButtonSiteDefault->setChecked(true);
        pushButtonEnterSite->setEnabled(false);
      }
      else if (flagValue & SITE_MANUAL)
      {
        radioButtonSiteManual->setChecked(true);
        pushButtonEnterSite->setEnabled(true);
      }
      else
      {
        radioButtonSiteDefault->setChecked(true);
        pushButtonEnterSite->setEnabled(false);
      };
      groupBoxObservationSite->setChecked(flagValue & SITE_ENABLED);

      flagValue = settings::astroManagerSettings->value(settings::DIALOG_BATCHCONVERT_WEATHER).toInt();
      if (flagValue & WEATHER_USEDATABASE)
      {
        radioButtonWeatherDatabase->setChecked(true);
        pushButtonWeatherEnter->setEnabled(false);
      }
      else if (flagValue & WEATHER_MANUAL)
      {
        radioButtonWeatherManual->setChecked(true);
        pushButtonWeatherEnter->setEnabled(true);
      }
      else
      {
        radioButtonWeatherDatabase->setChecked(true);
        pushButtonWeatherEnter->setEnabled(false);
      };
      groupBoxWeather->setChecked(flagValue & WEATHER_ENABLED);

      flagValue = settings::astroManagerSettings->value(settings::DIALOG_BATCHCONVERT_OBSERVER).toInt();
      if (flagValue & OBSERVER_DEFAULT)
      {
        radioButtonObserverDefault->setChecked(true);
        pushButtonObserverSelect->setEnabled(false);
      }
      else if (flagValue & OBSERVER_MANUAL)
      {
        radioButtonObserverManual->setChecked(true);
        pushButtonObserverSelect->setEnabled(true);
      }
      else
      {
        radioButtonObserverDefault->setChecked(true);
        pushButtonObserverSelect->setEnabled(false);
      };
      groupBoxObserver->setChecked(flagValue & OBSERVER_ENABLED);

      connect(dlg->findChild<QPushButton *>("pushButtonSelectDirectory"), SIGNAL(clicked(bool)), this,
        SLOT(eventSelectDirectory(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonSelectFiles"), SIGNAL(clicked(bool)), this,
        SLOT(eventSelectFiles(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonRemoveFiles"), SIGNAL(clicked(bool)), this,
        SLOT(eventRemoveFiles(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonConvertImages"), SIGNAL(clicked(bool)), this,
        SLOT(eventConvertImages(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonEnterSite"), SIGNAL(clicked(bool)), this,
        SLOT(eventEnterSite(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonEnterWeather"), SIGNAL(clicked(bool)), this,
        SLOT(eventEnterWeather(bool)));
      connect(dlg->findChild<QPushButton *>("pushButtonEnterObserver"), SIGNAL(clicked(bool)), this,
        SLOT(eventEnterObserver(bool)));
      connect(radioButtonSiteDefault, SIGNAL(clicked(bool)), this, SLOT(radioButtonSiteDefaultClicked(bool)));
      connect(radioButtonSiteManual, SIGNAL(clicked(bool)), this, SLOT(radioButtonSiteManualClicked(bool)));
      connect(radioButtonWeatherDatabase, SIGNAL(clicked(bool)), this, SLOT(radioButtonWeatherDatabaseClicked(bool)));
      connect(radioButtonWeatherManual, SIGNAL(clicked(bool)), this, SLOT(radioButtonWeatherManualClicked(bool)));
      connect(radioButtonObserverDefault, SIGNAL(clicked(bool)), this, SLOT(radioButtonObserverDefaultClicked(bool)));
      connect(radioButtonObserverManual, SIGNAL(clicked(bool)), this, SLOT(radioButtonObserverManualClicked(bool)));
    }

  }    // namespace file
}    // namespace AstroManager
