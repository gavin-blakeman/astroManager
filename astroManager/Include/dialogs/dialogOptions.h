//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogOptions
// SUBSYSTEM:						Options Dialog for Application
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt, boost.
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2017 Gavin Blakeman.
//                      This file is part of the Astronomical Image Reduction and Data Analysis Software (AIRDAS)
//
//                      AIRDAS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      AIRDAS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with AIRDAS.  If not,
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
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-29 GGB - File Created
//
//*********************************************************************************************************************************

#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include "dialogs.h"

  // Standard libraries

#include <list>

  // Boost Library

#include "boost/tuple/tuple.hpp"

namespace AstroManager
{
  typedef boost::tuple<int, int, QString, QWidget *> TWidgetEntry;
  typedef std::list<TWidgetEntry> DWidgetStore;

  namespace dialogs
  {
    class CDialogOptions : public CDialog
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
      QLineEdit *lineEditDarkDirectory;
      QLineEdit *lineEditFlatDirectory;
      QLineEdit *lineEditBiasDirectory;
      QLineEdit *lineEditMasterDarkDirectory;
      QLineEdit *lineEditMasterFlatDirectory;
      QLineEdit *lineEditMasterBiasDirectory;
      QCheckBox *checkBoxDarkDirectory;
      QCheckBox *checkBoxFlatDirectory;
      QCheckBox *checkBoxBiasDirectory;

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

      QGroupBox *groupBoxATID;


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
        void setupWeatherDatabase();
      void setupImageManager();

      void saveGeneral();
      void saveAstrometry();
      void savePhotometry();
      void saveDatabase();
        void saveARIDDatabase();
        void saveWeatherDatabase();
      void saveImageManager();

      void populateComboBoxColour(QComboBox *);

    protected:
    public:
      CDialogOptions();

    private slots:
      void save();
      void eventLogFileDirectory(bool);
      void eventDarkDirectory(bool);
      void eventFlatDirectory(bool);
      void eventBiasDirectory(bool);
      void eventMasterDarkDirectory(bool);
      void eventMasterFlatDirectory(bool);
      void eventMasterBiasDirectory(bool);
      void eventCSVDirectory(bool);
      void eventResetWarnings(bool);
      void eventARIDDatabaseCombo(int);
      void eventWeatherDatabaseCombo(int);
      void eventWeatherTestConnection(bool);
      void eventARIDTestConnection(bool);
    };
  }
}

#endif // DIALOGOPTIONS_H
