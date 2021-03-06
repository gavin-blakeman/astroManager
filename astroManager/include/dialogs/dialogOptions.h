﻿//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								DialogOptions
// SUBSYSTEM:						Options Dialog for Application
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt, boost.
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2020 Gavin Blakeman.
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
// OVERVIEW:            Implements the options dialog for the application.
//
// CLASSES INCLUDED:    CDialogOptions
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CDialogOptions
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-29 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

  // Standard C++ library header files

#include <list>
#include <tuple>

  // Miscellaneous library header files

#include <QCL>

namespace astroManager
{
  typedef std::tuple<int, int, QString, QWidget *> TWidgetEntry;
  typedef std::list<TWidgetEntry> DWidgetStore;

  namespace dialogs
  {
    class CDialogOptions : public QCL::CDialog
    {
      Q_OBJECT

    private:
      QListWidget *listWidget;
      QStackedWidget *stackedWidget;

        // General - General Tab

      QLineEdit *lineEditUserName;
      QSpinBox *spinBoxMaximumThreads;
      QRadioButton *radioButtonLocalTime;
      QRadioButton *radioButtonUTC;
      QSpinBox *spinBoxLastOpenedFiles;
      QSpinBox *spinBoxHistogramBins;
      QSpinBox *spinBoxToolbarHeight;

        // General - Directories

      QLineEdit *lineEditLogfileDirectory;
      QLineEdit *lineEditDataDirectory;
      QLineEdit *lineEditDarkDirectory;
      QLineEdit *lineEditFlatDirectory;
      QLineEdit *lineEditBiasDirectory;
      QLineEdit *lineEditMasterDarkDirectory;
      QLineEdit *lineEditMasterFlatDirectory;
      QLineEdit *lineEditMasterBiasDirectory;
      QCheckBox *checkBoxDarkDirectory;
      QCheckBox *checkBoxFlatDirectory;
      QCheckBox *checkBoxBiasDirectory;

        // General Data files

      QLineEdit *lineEditTAIUTC;
      QLineEdit *lineEditUTCUT1;
      QLineEdit *lineEditMPCORB;
      QLineEdit *lineEditCometEls;

        // General - Source Extraction

      QComboBox *comboBoxSEAlgorithm;
      QCheckBox *checkBoxSEAstrometry;
      QCheckBox *checkBoxSEPhotometry;

        // Astrometry

      QSpinBox *spinBoxCentroidRadius;
      QSpinBox *spinBoxCentroidSensitivity;

      QComboBox *comboBoxAstrometryColour;
      QComboBox *comboBoxAstrometrySelectedColour;

        // Photometry

      QCheckBox *checkBoxZMAG;
      QSpinBox *spinBoxApertureRadius;
      QSpinBox *spinBoxInnerSkyRadius;
      QSpinBox *spinBoxOuterSkyRadius;
      QComboBox *comboBoxPhotometryColour;
      QComboBox *comboBoxPhotometrySelectedColour;
      QLineEdit *lineEditCSVDirectory;
      QCheckBox *checkBoxCSVDirectory;


        // ARID Database

      QGroupBox *groupBoxARID;
        QCheckBox *checkBoxEnableARID;
        QComboBox *comboBoxARIDDatabaseType;
        QStackedWidget *stackedWidgetARID;
        QPushButton *pushButtonARIDTestConnection;
        QTextEdit *textEditARIDTestConnection;

            // ARID MySQL

          QLineEdit *lineEditARIDMySQLHost;
          QLineEdit *lineEditARIDMySQLDatabase;
          QSpinBox *spinBoxARIDMySQLPort;
          QLineEdit *lineEditARIDMySQLUser;
          QLineEdit *lineEditARIDMySQLPassword;

            // ARID Postgre

          QLineEdit *lineEditARIDPostgreSQLHost;
          QLineEdit *lineEditARIDPostgreSQLDatabase;
          QSpinBox *spinBoxARIDPostgreSQLPort;
          QLineEdit *lineEditARIDPostgreSQLUser;
          QLineEdit *lineEditARIDPostgreSQLPassword;

            // ARID Oracle

          QLineEdit *lineEditARIDOracleHost;
          QLineEdit *lineEditARIDOracleSchema;
          QSpinBox *spinBoxARIDOraclePort;
          QLineEdit *lineEditARIDOracleUser;
          QLineEdit *lineEditARIDOraclePassword;

        // ATID Database

      QGroupBox *groupBoxATID;
      QCheckBox *checkBoxUseSimbad;
      QCheckBox *checkBoxEnableATID;
      QComboBox *comboBoxATIDDatabaseType;
      QStackedWidget *stackedWidgetATID;
      QPushButton *pushButtonATIDTestConnection;
      QTextEdit *textEditATIDTestConnection;

          // ARID MySQL

        QLineEdit *lineEditATIDMySQLHost;
        QLineEdit *lineEditATIDMySQLDatabase;
        QSpinBox *spinBoxATIDMySQLPort;
        QLineEdit *lineEditATIDMySQLUser;
        QLineEdit *lineEditATIDMySQLPassword;

          // ARID Postgre

        QLineEdit *lineEditATIDPostgreSQLHost;
        QLineEdit *lineEditATIDPostgreSQLDatabase;
        QSpinBox *spinBoxATIDPostgreSQLPort;
        QLineEdit *lineEditATIDPostgreSQLUser;
        QLineEdit *lineEditATIDPostgreSQLPassword;

          // ARID Oracle

        QLineEdit *lineEditATIDOracleHost;
        QLineEdit *lineEditATIDOracleSchema;
        QSpinBox *spinBoxATIDOraclePort;
        QLineEdit *lineEditATIDOracleUser;
        QLineEdit *lineEditATIDOraclePassword;

        // Weather Database

      QGroupBox *groupBoxWeather;
        QComboBox *comboBoxWeatherDatabaseType;
        QStackedWidget *stackedWidgetWeather;
        QPushButton *pushButtonWeatherTestConnection;
        QTextEdit *textEditWeatherTestConnection;

          // MySQL

        QLineEdit *lineEditWeatherMySQLHost;
        QLineEdit *lineEditWeatherMySQLDatabase;
        QSpinBox *spinBoxWeatherMySQLPort;
        QLineEdit *lineEditWeatherMySQLUser;
        QLineEdit *lineEditWeatherMySQLPassword;

          // Postgre SQL

        QLineEdit *lineEditWeatherPostgreSQLHost;
        QLineEdit *lineEditWeatherPostgreSQLDatabase;
        QSpinBox *spinBoxWeatherPostgreSQLPort;
        QLineEdit *lineEditWeatherPostgreSQLUser;
        QLineEdit *lineEditWeatherPostgreSQLPassword;

          // Oracle

        QLineEdit *lineEditWeatherOracleHost;
        QLineEdit *lineEditWeatherOracleSchema;
        QSpinBox *spinBoxWeatherOraclePort;
        QLineEdit *lineEditWeatherOracleUser;
        QLineEdit *lineEditWeatherOraclePassword;

          // Image Manager

        QCheckBox *checkBoxUploadARID;
        QCheckBox *checkBoxRegisterOnOpen;
        QCheckBox *checkBoxUploadOriginal;
        QGroupBox *groupBoxStoreVersions;
        QSpinBox *spinBoxMaximumVersions;

      int lastItem;
      bool dirty;

      void setupUI();
      void setupGeneral();
      void setupAstrometry();
      void setupPhotometry();
      void setupDatabase();
        void setupARIDDatabase();
        void setupATIDDatabase();
        void setupWeatherDatabase();
      void setupImageManager();

      void saveGeneral();
      void saveAstrometry();
      void savePhotometry();
      void saveDatabase();
        void saveARIDDatabase();
        void saveATIDDatabase();
        void saveWeatherDatabase();
      void saveImageManager();

      void populateComboBoxColour(QComboBox *);

    protected:
    public:
      CDialogOptions();

    private slots:
      void save();

        // Directories

      void eventLogFileDirectory(bool);
      void eventDataDirectory();
      void eventDarkDirectory(bool);
      void eventFlatDirectory(bool);
      void eventBiasDirectory(bool);
      void eventMasterDarkDirectory(bool);
      void eventMasterFlatDirectory(bool);
      void eventMasterBiasDirectory(bool);
      void eventCSVDirectory(bool);

        // Data files

      void eventTAIUTC();
      void eventUTCUT1();
      void eventMPCORB();
      void eventCometEls();

        // Database

      void eventATIDUseSimbad(bool);
      void eventATIDUseATID(bool);
      void eventARIDDatabaseCombo(int);
      void eventATIDDatabaseCombo(int);
      void eventWeatherDatabaseCombo(int);
      void eventWeatherTestConnection(bool);
      void eventARIDTestConnection(bool);
      void eventATIDTestConnection();

        // Other

      void eventResetWarnings(bool);
    };
  }
}

#endif // DIALOGOPTIONS_H
