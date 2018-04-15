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
//                      Copyright 2013-2018 Gavin Blakeman.
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

#include "../../Include/dialogs/dialogOptions.h"

  // astroManager headers

#include "../../Include/database/database.h"
#include "../../Include/Error.h"
#include "../../Include/qtExtensions/qt.h"
#include "../../Include/Settings.h"
#include "../../Include/astroManager.h"

  // Standard C++ headers

#include <algorithm>
#include <thread>
#include <tuple>

  // Class Libraries

#include <GCL>

namespace AstroManager
{
  namespace dialogs
  {

    struct SColourRecord
    {
      QColor colourValue;
      QString colourString;
    };

    SColourRecord colourArray[] =
    {
      {Qt::white, QString(QObject::tr("White"))},
      {Qt::black, QString(QObject::tr("Black"))},
      {Qt::red, QString(QObject::tr("Red"))},
      {Qt::darkRed, QString(QObject::tr("Dark Red"))},
      {Qt::green, QString(QObject::tr("Green"))},
      {Qt::darkGreen, QString(QObject::tr("Dark Green"))},
      {Qt::blue, QString(QObject::tr("Blue"))},
      {Qt::darkBlue, QString(QObject::tr("Dark Blue"))},
      {Qt::cyan, QString(QObject::tr("Cyan"))},
      {Qt::darkCyan, QString(QObject::tr("Dark Cyan"))},
      {Qt::magenta, QString(QObject::tr("Magenta"))},
      {Qt::darkMagenta, QString(QObject::tr("Dark Magenta"))},
      {Qt::yellow, QString(QObject::tr("Yellow"))},
      {Qt::darkYellow, QString(QObject::tr("Dark Yellow"))},
      {Qt::gray, QString(QObject::tr("Gray"))},
      {Qt::darkGray, QString(QObject::tr("Dark Gray"))},
      {Qt::lightGray, QString(QObject::tr("Light Gray"))}
    };

    /// @brief Constructor for the Options Dialog class.
    /// @throws None.
    /// @version 2017-06-18/GGB - Added variable initialisation.
    /// @version 2013-03-29/GGB - Function created.

    CDialogOptions::CDialogOptions() : CDialog(":/dialogs/dialogOptions.ui"), dirty(false)
    {
      setupUI();
    }

    /// @brief Processes the event when a database type is chosen.
    /// @param[in] index - The index of the currently selected option.
    /// @throws None.
    /// @version 2015-06-14/GGB - Function created.

    void CDialogOptions::eventARIDDatabaseCombo(int index)
    {
      stackedWidgetARID->setCurrentIndex(comboBoxARIDDatabaseType->itemData(index, Qt::UserRole).toInt() );
    }

    /// @brief Function to update the CSV directory.
    /// @throws None.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventCSVDirectory(bool)
    {
      QString directory = lineEditCSVDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Photometry Export CSV directory"), directory);

      if (!directory.isNull())
      {
        lineEditCSVDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the log file directory.
    /// @throws None.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventLogFileDirectory(bool)
    {
      QString directory = lineEditLogfileDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Logfile directory"), directory);

      if (!directory.isNull())
      {
        lineEditLogfileDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the dark frame directory.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventDarkDirectory(bool)
    {
      QString directory = lineEditDarkDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Dark frame directory"), directory);

      if (!directory.isNull())
      {
        lineEditDarkDirectory->setText(directory);
      }
    }

    /// Responds to the button press for the flat frame directory.
    //
    // 2013-06-01/GGB - Function created.

    void CDialogOptions::eventFlatDirectory(bool)
    {
      QString directory = lineEditFlatDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Flat frame directory"), directory);

      if (!directory.isNull())
      {
        lineEditFlatDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the bias frame directory.
    /// @throws None.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventBiasDirectory(bool)
    {
      QString directory = lineEditBiasDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Bias frame directory"), directory);

      if (!directory.isNull())
      {
        lineEditBiasDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the Master Dark frame directory.
    /// @throws None.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventMasterDarkDirectory(bool)
    {
      QString directory = lineEditMasterDarkDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Master Dark frame directory"), directory);

      if (!directory.isNull())
      {
        lineEditMasterDarkDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the Master Flat frame directory.
    /// @throws None.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventMasterFlatDirectory(bool)
    {
      QString directory = lineEditMasterFlatDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Master Flat frame directory"), directory);

      if (!directory.isNull())
      {
        lineEditMasterFlatDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the Master Bias frame directory.
    /// @throws None.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::eventMasterBiasDirectory(bool)
    {
      QString directory = lineEditMasterBiasDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Master Bias frame directory"), directory);

      if (!directory.isNull())
      {
        lineEditMasterBiasDirectory->setText(directory);
      }
    }

    /// @brief Reset all the warning dialogs.
    /// @throws None.
    /// @version 2015-01-03/GGB - Function created.

    void CDialogOptions::eventResetWarnings(bool)
    {
      settings::astroManagerSettings->setValue(settings::CM_DISCLAIMER, QVariant(-1));
      settings::astroManagerSettings->setValue(settings::CM_IMAGECOMPARISON_REMOVE, QVariant(-1));
      settings::astroManagerSettings->setValue(settings::CM_IMAGECOMPARISON_REMOVEALL, QVariant(-1));
      settings::astroManagerSettings->setValue(settings::CM_IMAGESTACKING_REMOVE, QVariant(-1));
      settings::astroManagerSettings->setValue(settings::CM_IMAGESTACKING_REMOVEALL, QVariant(-1));
    }

    /// @brief Processes the event when a weather database type is chosen.
    /// @param[in] index - The index of the item chosen.
    /// @throws None.
    /// @version 2017-06-18/GGB - Function created.

    void CDialogOptions::eventWeatherDatabaseCombo(int index)
    {
      stackedWidgetWeather->setCurrentIndex(comboBoxWeatherDatabaseType->itemData(index, Qt::UserRole).toInt() );
    }

    /// @brief Test the connection to the ARID database.
    /// @throws None.
    /// @version 2017-07-21/GGB - Function created.

    void CDialogOptions::eventARIDTestConnection(bool)
    {
      QString szDBMS;
      QString szHostName;
      std::uint16_t szPortNumber;
      QString szDatabaseName;
      QString szUserName;
      QString szPassword;

        // Let the user know that we are beginning

      textEditARIDTestConnection->clear();
      textEditARIDTestConnection->setTextColor(QColor("black"));
      textEditARIDTestConnection->append(tr("Starting Test..."));

        // Determine the strings that we need.

      szDBMS = comboBoxARIDDatabaseType->currentText();

      switch (comboBoxARIDDatabaseType->itemData(comboBoxARIDDatabaseType->currentIndex(), Qt::UserRole).toInt())
      {
        case database::CDatabase::SQLDB_MYSQL:
        {
          szDBMS = "QMYSQL";
          szHostName = lineEditARIDMySQLHost->text();
          szPortNumber = spinBoxARIDMySQLPort->value();
          szDatabaseName = lineEditARIDMySQLDatabase->text();
          szUserName = lineEditARIDMySQLUser->text();
          szPassword = lineEditARIDMySQLPassword->text();
          break;
        };
        case database::CDatabase::SQLDB_ODBC:
        {
          szDBMS = "QODBC";
          break;
        };
        case database::CDatabase::SQLDB_PSQL:
        {
          szDBMS = "QPSQL";
          szHostName = lineEditARIDPostgreSQLHost->text();
          szPortNumber = spinBoxARIDPostgreSQLPort->value();
          szDatabaseName = lineEditARIDPostgreSQLDatabase->text();
          szUserName = lineEditARIDPostgreSQLUser->text();
          szPassword = lineEditARIDPostgreSQLPassword->text();
          break;
        };
        case database::CDatabase::SQLDB_QOCI:
        {
          szDBMS = "QOCI";
          szHostName = lineEditARIDOracleHost->text();
          szPortNumber = spinBoxARIDOraclePort->value();
          szDatabaseName = lineEditARIDOracleSchema->text();
          szUserName = lineEditARIDOracleUser->text();
          szPassword = lineEditARIDOraclePassword->text();
          break;
        }
        case database::CDatabase::SQLDB_SQLITE:
        {
          szDBMS = "QSQLITE";
          break;
        }
        default:
        {
          CODE_ERROR(astroManager);
          break;
        }
      };

      textEditARIDTestConnection->setTextColor(QColor("darkBlue"));
      textEditARIDTestConnection->append(tr("Driver name: ") + szDBMS);
      textEditARIDTestConnection->append(tr("Host name: ") + szHostName);
      textEditARIDTestConnection->append(QString(tr("Host port: %1")).arg(szPortNumber));
      textEditARIDTestConnection->append(tr("Database name: ") + szDatabaseName);
      textEditARIDTestConnection->append(tr("User name: ") + szUserName);
      textEditARIDTestConnection->append(tr("Password: ") + szPassword);
      textEditARIDTestConnection->setTextColor(QColor("magenta"));
      textEditARIDTestConnection->append(tr("Attempting connection..."));

      {
        QSqlDatabase testConnection = QSqlDatabase::addDatabase(szDBMS, "testConnection");
        testConnection.setHostName(szHostName);
        testConnection.setPort(szPortNumber);
        testConnection.setDatabaseName(szDatabaseName);
        testConnection.setUserName(szUserName);
        testConnection.setPassword(szPassword);

        if (testConnection.open())
        {
          textEditARIDTestConnection->setTextColor(QColor("darkGreen"));
          textEditARIDTestConnection->append(tr("Connection succesful."));

          testConnection.close();
        }
        else
        {
          textEditARIDTestConnection->setTextColor(QColor("darkRed"));
          textEditARIDTestConnection->append(tr("Connection failed."));

            // List the error strings.

          QSqlError error = testConnection.lastError();

          textEditARIDTestConnection->append(tr("Error while connecting to Database: ") + szHostName + "." + szDatabaseName);
          textEditARIDTestConnection->append(tr("Error returned by driver: ") + error.nativeErrorCode());
          textEditARIDTestConnection->append(tr("Text returned by driver: ") + error.driverText());
          textEditARIDTestConnection->append(tr("Text returned by database: ") + error.databaseText());
        };
      };

      QSqlDatabase::removeDatabase("testConnection");

      textEditARIDTestConnection->setTextColor(QColor("black"));
      textEditARIDTestConnection->append(tr("Test Complete."));
    }

    /// @brief Processes the user request to test the weather database connection.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    void CDialogOptions::eventWeatherTestConnection(bool)
    {
      QString szDBMS;
      QString szHostName;
      std::uint16_t szPortNumber;
      QString szDatabaseName;
      QString szUserName;
      QString szPassword;

        // Let the user know that we are beginning

      textEditWeatherTestConnection->clear();
      textEditWeatherTestConnection->setTextColor(QColor("black"));
      textEditWeatherTestConnection->append(tr("Starting Test..."));

        // Determine the strings that we need.

      szDBMS = comboBoxWeatherDatabaseType->currentText();

      switch (comboBoxWeatherDatabaseType->itemData(comboBoxWeatherDatabaseType->currentIndex(), Qt::UserRole).toInt())
      {
        case database::CDatabase::SQLDB_MYSQL:
        {
          szDBMS = "QMYSQL";
          szHostName = lineEditWeatherMySQLHost->text();
          szPortNumber = spinBoxWeatherMySQLPort->value();
          szDatabaseName = lineEditWeatherMySQLDatabase->text();
          szUserName = lineEditWeatherMySQLUser->text();
          szPassword = lineEditWeatherMySQLPassword->text();
          break;
        };
        case database::CDatabase::SQLDB_ODBC:
        {
          szDBMS = "QODBC";
          break;
        };
        case database::CDatabase::SQLDB_PSQL:
        {
          szDBMS = "QPSQL";
          szHostName = lineEditWeatherPostgreSQLHost->text();
          szPortNumber = spinBoxWeatherPostgreSQLPort->value();
          szDatabaseName = lineEditWeatherPostgreSQLDatabase->text();
          szUserName = lineEditWeatherPostgreSQLUser->text();
          szPassword = lineEditWeatherPostgreSQLPassword->text();
          break;
        };
        case database::CDatabase::SQLDB_QOCI:
        {
          szDBMS = "QOCI";
          szHostName = lineEditWeatherOracleHost->text();
          szPortNumber = spinBoxWeatherOraclePort->value();
          szDatabaseName = lineEditWeatherOracleSchema->text();
          szUserName = lineEditWeatherOracleUser->text();
          szPassword = lineEditWeatherOraclePassword->text();
          break;
        }
        case database::CDatabase::SQLDB_SQLITE:
        {
          szDBMS = "QSQLITE";
          break;
        }
        default:
        {
          CODE_ERROR(astroManager);
          break;
        }
      };

      textEditWeatherTestConnection->setTextColor(QColor("darkBlue"));
      textEditWeatherTestConnection->append(tr("Driver name: ") + szDBMS);
      textEditWeatherTestConnection->append(tr("Host name: ") + szHostName);
      textEditWeatherTestConnection->append(QString(tr("Host port: %1")).arg(szPortNumber));
      textEditWeatherTestConnection->append(tr("Database name: ") + szDatabaseName);
      textEditWeatherTestConnection->append(tr("User name: ") + szUserName);
      textEditWeatherTestConnection->append(tr("Password: ") + szPassword);
      textEditWeatherTestConnection->setTextColor(QColor("magenta"));
      textEditWeatherTestConnection->append(tr("Attempting connection..."));

      {
        QSqlDatabase testConnection = QSqlDatabase::addDatabase(szDBMS, "testConnection");
        testConnection.setHostName(szHostName);
        testConnection.setPort(szPortNumber);
        testConnection.setDatabaseName(szDatabaseName);
        testConnection.setUserName(szUserName);
        testConnection.setPassword(szPassword);

        if (testConnection.open())
        {
          textEditWeatherTestConnection->setTextColor(QColor("darkGreen"));
          textEditWeatherTestConnection->append(tr("Connection succesful."));

          testConnection.close();
        }
        else
        {
          textEditWeatherTestConnection->setTextColor(QColor("darkRed"));
          textEditWeatherTestConnection->append(tr("Connection failed."));

            // List the error strings.

          QSqlError error = testConnection.lastError();

          textEditWeatherTestConnection->append(tr("Error while connecting to Database: ") + szHostName + "." + szDatabaseName);
          textEditWeatherTestConnection->append(tr("Error returned by driver: ") + error.nativeErrorCode());
          textEditWeatherTestConnection->append(tr("Text returned by driver: ") + error.driverText());
          textEditWeatherTestConnection->append(tr("Text returned by database: ") + error.databaseText());
        };
      };

      QSqlDatabase::removeDatabase("testConnection");

      textEditWeatherTestConnection->setTextColor(QColor("black"));
      textEditWeatherTestConnection->append(tr("Test Complete."));
    }

    /// @brief Populates a combobox with the colour values.
    /// @param[in] comboBox - The combo box to populate.
    /// @throws None.
    /// @version 2013-08-24/GGB - Function created.

    void CDialogOptions::populateComboBoxColour(QComboBox *comboBox)
    {
      size_t elements = sizeof(colourArray) / sizeof(SColourRecord);   // Work out how many elements stored.
      size_t index;

      for (index = 0; index < elements; ++index)
      {
        comboBox->addItem(colourArray[index].colourString, QVariant(colourArray[index].colourValue));
      }
    }

    /// @brief Processes the save button.
    /// @throws None.
    /// @version 2013-05-31/GGB - Function created.

    void CDialogOptions::save()
    {
      saveGeneral();
      saveAstrometry();
      savePhotometry();
      saveDatabase();
      saveImageManager();

      dlg->accept();
    }

    /// @brief Saves the astrometry related information.
    /// @throws None.
    /// @version 2013-08-24/GGB - Added colour functions
    /// @version 2013-08-02/GGB - Function created.

    void CDialogOptions::saveAstrometry()
    {
      settings::astroManagerSettings->setValue(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS, QVariant(spinBoxCentroidRadius->value()));
      settings::astroManagerSettings->setValue(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                       QVariant(spinBoxCentroidSensitivity->value()));

      settings::astroManagerSettings->setValue(settings::ASTROMETRY_INDICATOR_COLOUR,
                                       comboBoxAstrometryColour->itemData(comboBoxAstrometryColour->currentIndex()));
      settings::astroManagerSettings->setValue(settings::ASTROMETRY_INDICATOR_SELECTEDCOLOUR,
                                       comboBoxAstrometrySelectedColour->itemData(comboBoxAstrometrySelectedColour->currentIndex()));
    }

    /// @brief Saves the ARID database options.
    /// @throws GCL::CCodeError(...)
    /// @version 2017-07-01/GGB - Updated to reflect new approach to testing and databases.
    /// @version 2015-06-14/GGB - Function created.

    void CDialogOptions::saveARIDDatabase()
    {
      settings::astroManagerSettings->setValue(settings::ARID_DATABASE_DISABLE, QVariant(!groupBoxARID->isChecked()));
      if ( groupBoxARID->isChecked() )
      {
        settings::astroManagerSettings->setValue(settings::WEATHER_DATABASE_DBMS, QVariant(comboBoxWeatherDatabaseType->currentText()));
        settings::astroManagerSettings->setValue(settings::ARID_DATABASE_DBMS, QVariant(comboBoxARIDDatabaseType->currentText()));

        switch (comboBoxARIDDatabaseType->itemData(comboBoxARIDDatabaseType->currentIndex(), Qt::UserRole).toInt())
        {
          case database::CDatabase::SQLDB_MYSQL:
          {
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_HOSTADDRESS, lineEditARIDMySQLHost->text());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_PORT, spinBoxARIDMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_DATABASENAME, lineEditARIDMySQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_USERNAME, lineEditARIDMySQLUser->text());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_PASSWORD, lineEditARIDMySQLPassword->text());
            break;
          };
          case database::CDatabase::SQLDB_ODBC:
          {
            break;
          };
          case database::CDatabase::SQLDB_PSQL:
          {
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_HOSTADDRESS, lineEditARIDPostgreSQLHost->text());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_PORT, spinBoxARIDMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_DATABASENAME, lineEditARIDPostgreSQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_USERNAME, lineEditARIDPostgreSQLUser->text());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_PASSWORD, lineEditARIDPostgreSQLPassword->text());
            break;
          };
          case database::CDatabase::SQLDB_QOCI:
          {
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_HOSTADDRESS, lineEditARIDOracleHost->text());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_PORT, spinBoxARIDOraclePort->value());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_SCHEMANAME, lineEditARIDOracleSchema->text());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_USERNAME, lineEditARIDOracleUser->text());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_PASSWORD, lineEditARIDOraclePassword->text());
            break;
          }
          case database::CDatabase::SQLDB_SQLITE:
          {
            break;
          }
          default:
          {
            CODE_ERROR(astroManager);
            break;
          }
        };
      }
    }

    /// @brief Saves the database values.
    /// @throws None.
    /// @version 2017-06-18/GGB - Added support for the weather database.
    /// @version 2015-06-14/GGB - Moved ARID into saveARIDDatabase() function.
    /// @version 2014-02-16/GGB - Added source extraction tab.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::saveDatabase()
    {
      settings::astroManagerSettings->setValue(settings::ATID_DATABASE_DISABLE, QVariant(!groupBoxATID->isChecked()));
      saveARIDDatabase();
      settings::astroManagerSettings->setValue(settings::WEATHER_DATABASE_DISABLE, QVariant(!groupBoxWeather->isChecked()));

      saveWeatherDatabase();
    }

    /// @brief Writes the general values to the settings.
    /// @version 2017-06-25/GGB - Updates the number of threads used. (Bug #72)
    /// @version 2013-06-02/GGB - Added toolbar height.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::saveGeneral()
    {
      settings::astroManagerSettings->setValue(settings::SETTINGS_OBSERVER, QVariant(lineEditUserName->text()));
      settings::astroManagerSettings->setValue(settings::MAX_THREADS, QVariant(spinBoxMaximumThreads->value()));
      settings::astroManagerSettings->setValue(settings::SETTINGS_LT, QVariant(radioButtonLocalTime->isChecked()));
      settings::astroManagerSettings->setValue(settings::FILE_LASTOPENEDDEPTH, QVariant(spinBoxLastOpenedFiles->value()));
      settings::astroManagerSettings->setValue(settings::DW_HISTOGRAM_BINS, QVariant(spinBoxHistogramBins->value()));
      settings::astroManagerSettings->setValue(settings::TOOLBAR_HEIGHT, QVariant(spinBoxToolbarHeight->value()));

      settings::astroManagerSettings->setValue(settings::SETTINGS_LOGDIR, QVariant(lineEditLogfileDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(lineEditDarkDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(lineEditFlatDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(lineEditBiasDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant(lineEditMasterDarkDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERFLAT_DIRECTORY, QVariant(lineEditMasterFlatDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant(lineEditMasterBiasDirectory->text()));

      settings::astroManagerSettings->setValue(settings::DARKFRAME_DIRECTORY_AUTOUPDATE, QVariant(checkBoxDarkDirectory->isChecked()));
      settings::astroManagerSettings->setValue(settings::FLATFRAME_DIRECTORY_AUTOUPDATE, QVariant(checkBoxFlatDirectory->isChecked()));
      settings::astroManagerSettings->setValue(settings::BIASFRAME_DIRECTORY_AUTOUPDATE, QVariant(checkBoxBiasDirectory->isChecked()));

        // Source Extraction Data

      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_ALGORITHM,
                                       QVariant(comboBoxSEAlgorithm->itemData(comboBoxSEAlgorithm->currentIndex())));
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_ADD_ASTROMETRY, QVariant(checkBoxSEAstrometry->isChecked()));
      settings::astroManagerSettings->setValue(settings::SOURCE_EXTRACTION_ADD_PHOTOMETRY, QVariant(checkBoxSEPhotometry->isChecked()));

      setThreads(spinBoxMaximumThreads->value());
    }

    /// @brief Saves the image management values.
    /// @throws None.
    /// @version 2017-08-24/GGB - Function created.

    void CDialogOptions::saveImageManager()
    {
      settings::astroManagerSettings->setValue(settings::IMAGING_DATABASE_REGISTERONOPEN, checkBoxRegisterOnOpen->isChecked());
      settings::astroManagerSettings->setValue(settings::IMAGING_DATABSE_SAVE, checkBoxUploadARID->isChecked());
      settings::astroManagerSettings->setValue(settings::IMAGING_DATABASE_SAVEORIGINAL, checkBoxUploadOriginal->isChecked());
      settings::astroManagerSettings->setValue(settings::IMAGING_DATABASE_SAVEVERSIONS, groupBoxStoreVersions->isChecked());
      settings::astroManagerSettings->setValue(settings::IMAGING_DATABASE_MAXVERSIONS, spinBoxMaximumVersions->value());
    }


    /// @brief Saves the photometry values when required.
    /// @throws None.
    /// @version 2013-08-24/GGB - Added support for colour selection.
    /// @version 2013-08-18/GGB - Added support for ZMAG.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::savePhotometry()
    {
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_USEZMAG, QVariant(checkBoxZMAG->isChecked()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_RADIUS1, QVariant(spinBoxApertureRadius->value()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_RADIUS2, QVariant(spinBoxInnerSkyRadius->value()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_RADIUS3, QVariant(spinBoxOuterSkyRadius->value()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_INDICATOR_COLOUR,
                                       comboBoxPhotometryColour->itemData(comboBoxPhotometryColour->currentIndex()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_INDICATOR_SELECTEDCOLOUR,
                                       comboBoxPhotometrySelectedColour->itemData(comboBoxPhotometrySelectedColour->currentIndex()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_CSVDIRECTORY, QVariant(lineEditCSVDirectory->text()));
      settings::astroManagerSettings->setValue(settings::PHOTOMETRY_CSVDIRECTORY_UPDATE, QVariant(checkBoxCSVDirectory->isChecked()));
    }

    /// @brief Saves the data in the weather database.
    /// @throws GCL::CCodeError(...)
    /// @version 2017-07-01/GGB - Function Created.

    void CDialogOptions::saveWeatherDatabase()
    {
      if (groupBoxWeather->isChecked())
      {
        settings::astroManagerSettings->setValue(settings::WEATHER_DATABASE_DISABLE, QVariant(false));
        settings::astroManagerSettings->setValue(settings::WEATHER_DATABASE_DBMS, QVariant(comboBoxWeatherDatabaseType->currentText()));

        switch (comboBoxWeatherDatabaseType->itemData(comboBoxWeatherDatabaseType->currentIndex(), Qt::UserRole).toInt())
        {
          case database::CDatabase::SQLDB_MYSQL:
          {
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_HOSTADDRESS, lineEditWeatherMySQLHost->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_PORT, spinBoxWeatherMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_DATABASENAME, lineEditWeatherMySQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_USERNAME, lineEditWeatherMySQLUser->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_PASSWORD, lineEditWeatherMySQLPassword->text());
            break;
          };
          case database::CDatabase::SQLDB_ODBC:
          {
            break;
          };
          case database::CDatabase::SQLDB_PSQL:
          {
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_HOSTADDRESS, lineEditWeatherPostgreSQLHost->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_PORT, spinBoxWeatherPostgreSQLPort->value());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_DATABASENAME, lineEditWeatherPostgreSQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_USERNAME, lineEditWeatherPostgreSQLUser->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_PASSWORD, lineEditWeatherPostgreSQLPassword->text());
            break;
          };
          case database::CDatabase::SQLDB_QOCI:
          {
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_HOSTADDRESS, lineEditWeatherOracleHost->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_PORT, spinBoxWeatherOraclePort->value());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_SCHEMANAME, lineEditWeatherOracleSchema->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_USERNAME, lineEditWeatherOracleUser->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_PASSWORD, lineEditWeatherOraclePassword->text());
            break;
          }
          case database::CDatabase::SQLDB_SQLITE:
          {
            break;
          }
          default:
          {
            CODE_ERROR(astroManager);
            break;
          }
        };
      }
      else
      {
        settings::astroManagerSettings->setValue(settings::WEATHER_DATABASE_DISABLE, QVariant(true));
      }
    }

    /// @brief Sets up the ARID database options.
    /// @throws CRuntimeAssert
    /// @version 2017-07-01/GGB - Updated to reflect new approach to testing and databases.
    /// @version 2017-06-18/GGB - Use new findChild() funtion in CDialog. (Bug #67)
    /// @version 2015-06-11/GGB - Function created.

    void CDialogOptions::setupARIDDatabase()
    {
      groupBoxARID = findChild<QGroupBox *>("groupBoxARID");
        comboBoxARIDDatabaseType = findChild<QComboBox *>("comboBoxARIDDatabaseType");
        stackedWidgetARID = findChild<QStackedWidget *>("stackedWidgetARID");
        pushButtonARIDTestConnection = findChild<QPushButton *>("pushButtonARIDTestConnection");
        textEditARIDTestConnection = findChild<QTextEdit *>("textEditARIDTestConnection");

          // MySQL

        lineEditARIDMySQLHost = findChild<QLineEdit *>("lineEditARIDMySQLHost");
        spinBoxARIDMySQLPort = findChild<QSpinBox *>("spinBoxARIDMySQLPort");
        lineEditARIDMySQLDatabase = findChild<QLineEdit *>("lineEditMySQLDatabase");
        lineEditARIDMySQLUser = findChild<QLineEdit *>("lineEditMySQLUser");
        lineEditARIDMySQLPassword = findChild<QLineEdit *>("lineEditARIDMySQLPassword");

          // PostgreSQL

        lineEditARIDPostgreSQLHost = findChild<QLineEdit *>("lineEditARIDPostgreHost");
        spinBoxARIDPostgreSQLPort = findChild<QSpinBox *>("spinBoxARIDPostgreSQLPort");
        lineEditARIDPostgreSQLDatabase = findChild<QLineEdit *>("lineEditARIDPostgreSQLDatabase");
        lineEditARIDPostgreSQLUser = findChild<QLineEdit *>("lineEditARIDPostgreSQLUser");
        lineEditARIDPostgreSQLPassword = findChild<QLineEdit *>("lineEditARIDPostgreSQLPassword");

          // Oracle

        lineEditARIDOracleHost = findChild<QLineEdit *>("lineEditARIDOracleHost");
        spinBoxARIDOraclePort = findChild<QSpinBox *>("spinBoxARIDOraclePort");
        lineEditARIDOracleSchema = findChild<QLineEdit *>("lineEditARIDOracleSchema");
        lineEditARIDOracleUser = findChild<QLineEdit *>("lineEditARIDOracleUser");
        lineEditARIDOraclePassword = findChild<QLineEdit *>("lineEditARIDOraclePassword");

      groupBoxARID->setChecked(!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE, QVariant(true)).toBool());

        // Add the items to the combo box. The driver map key value is used as user data to identify the driver selected.

      std::for_each(database::CDatabase::databaseDrivers.begin(),database::CDatabase::databaseDrivers.end(),
                    [&](std::pair<const int, database::CDatabase::SDatabaseDriver> &dt)
                       {comboBoxARIDDatabaseType->addItem(dt.second.driverText, QVariant(dt.first));} );

        // Set the current/default values from the settings file.
        // MySQL

      lineEditARIDMySQLHost->setText(settings::astroManagerSettings->value(settings::ARID_MYSQL_HOSTADDRESS, "localhost").toString());
      spinBoxARIDMySQLPort->setValue(settings::astroManagerSettings->value(settings::ARID_MYSQL_PORT, QVariant(3306)).toInt());
      lineEditARIDMySQLDatabase->setText(settings::astroManagerSettings->value(settings::ARID_MYSQL_DATABASENAME, "").toString());
      lineEditARIDMySQLUser->setText(settings::astroManagerSettings->value(settings::ARID_MYSQL_USERNAME, "").toString());
      lineEditARIDMySQLPassword->setText(settings::astroManagerSettings->value(settings::ARID_MYSQL_PASSWORD, "").toString());

        // PostgreSQL

      lineEditARIDPostgreSQLHost->setText(settings::astroManagerSettings->value(settings::ARID_POSTGRESQL_HOSTADDRESS, "localhost").toString());
      spinBoxARIDPostgreSQLPort->setValue(settings::astroManagerSettings->value(settings::ARID_POSTGRESQL_PORT, QVariant(5432)).toInt());
      lineEditARIDPostgreSQLDatabase->setText(settings::astroManagerSettings->value(settings::ARID_POSTGRESQL_DATABASENAME, "").toString());
      lineEditARIDPostgreSQLUser->setText(settings::astroManagerSettings->value(settings::ARID_POSTGRESQL_USERNAME, "").toString());
      lineEditARIDPostgreSQLPassword->setText(settings::astroManagerSettings->value(settings::ARID_POSTGRESQL_PASSWORD, "").toString());

        // Oracle

      lineEditARIDOracleHost->setText(settings::astroManagerSettings->value(settings::ARID_ORACLE_HOSTADDRESS, "localhost").toString());
      spinBoxARIDOraclePort->setValue(settings::astroManagerSettings->value(settings::ARID_ORACLE_PORT, QVariant(1521)).toInt());
      lineEditARIDOracleSchema->setText(settings::astroManagerSettings->value(settings::ARID_ORACLE_SCHEMANAME, "").toString());
      lineEditARIDOracleUser->setText(settings::astroManagerSettings->value(settings::ARID_ORACLE_USERNAME, "").toString());
      lineEditARIDOraclePassword->setText(settings::astroManagerSettings->value(settings::ARID_ORACLE_PASSWORD, "").toString());

      connect(comboBoxARIDDatabaseType, SIGNAL(currentIndexChanged(int)), this, SLOT(eventARIDDatabaseCombo(int)));

      // Select the current driver.

      if (groupBoxARID->isChecked())
      {
        std::string szDatabase = settings::astroManagerSettings->value(settings::ARID_DATABASE_DBMS, QVariant("MySQL")).toString().toStdString();

        if (szDatabase == "MySQL")
        {
          stackedWidgetARID->setCurrentIndex(database::CDatabase::SQLDB_MYSQL);
        }
        else if (szDatabase == "Oracle")
        {
          stackedWidgetARID->setCurrentIndex(database::CDatabase::SQLDB_QOCI);
        }
        else if (szDatabase == "SQLite")
        {
          stackedWidgetARID->setCurrentIndex(database::CDatabase::SQLDB_SQLITE);
        }
        else if (szDatabase == "PostgreSQL")
        {
          stackedWidgetARID->setCurrentIndex(database::CDatabase::SQLDB_PSQL);
        }
        else if (szDatabase == "ODBC")
        {
          stackedWidgetARID->setCurrentIndex(database::CDatabase::SQLDB_ODBC);
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        stackedWidgetARID->setCurrentIndex(database::CDatabase::SQLDB_MYSQL);
      }
      connect(pushButtonARIDTestConnection, SIGNAL(clicked(bool)), this, SLOT(eventARIDTestConnection(bool)));
    }

    /// @brief Sets up the database information.
    /// @throws CRuntimeAssert
    /// @version 2017-07-01/GGB - Added support for the weather database.
    /// @version 2017-06-18/GGB - Use the CDialog findChild(). (Bug #67)
    /// @version 2015-06-11/GGB - Moved ARID setup to a seperate function.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::setupDatabase()
    {
      setupARIDDatabase();
      groupBoxATID = findChild<QGroupBox *>("groupBoxATID");
      groupBoxATID->setChecked(!settings::astroManagerSettings->value(settings::ATID_DATABASE_DISABLE, QVariant(true)).toBool());

      setupWeatherDatabase();
    }

    /// Sets up the astrometry information.
    /// @version 2017-06-18/GGB - Update to CDialog style findChild (Bug #67)
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2013-08-24/GGB - Added colour functions.
    /// @version 2013-08-02/GGB - Function created.

    void CDialogOptions::setupAstrometry()
    {
      int colourIndex;
      QColor currentColour;

      spinBoxCentroidRadius = findChild<QSpinBox *>("spinBoxCentroidRadius");
      spinBoxCentroidSensitivity = findChild<QSpinBox *>("spinBoxCentroidSensitivity");
      comboBoxAstrometryColour = findChild<QComboBox *>("comboBoxAstrometryColour");
      comboBoxAstrometrySelectedColour = findChild<QComboBox *>("comboBoxAstrometrySelectedColour");

      spinBoxCentroidRadius->setValue(settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_RADIUS, QVariant(16)).toInt());
      spinBoxCentroidSensitivity->setValue(settings::astroManagerSettings->value(settings::ASTROMETRY_CENTROIDSEARCH_SENSITIVITY,
                                                                         QVariant(3)).toInt());

      populateComboBoxColour(comboBoxAstrometryColour);
      currentColour = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_COLOUR, QVariant(QColor(Qt::red))).value<QColor>();
      colourIndex = comboBoxAstrometryColour->findData(currentColour);
      comboBoxAstrometryColour->setCurrentIndex(colourIndex);

      populateComboBoxColour(comboBoxAstrometrySelectedColour);
      currentColour = settings::astroManagerSettings->value(settings::ASTROMETRY_INDICATOR_SELECTEDCOLOUR,
                                                    QVariant(QColor(Qt::yellow))).value<QColor>();
      colourIndex = comboBoxAstrometrySelectedColour->findData(currentColour);
      comboBoxAstrometrySelectedColour->setCurrentIndex(colourIndex);
    }

    /// @brief Gets the window handles and sets up the settings into the edit controls.
    /// @throws CRuntimeAssert.
    /// @version 2017-06-25/GGB - Added code to set the maximum number of threads allowed. (Bug #72)
    /// @version 2017-06-18/GGB - Updated to use CDialog::findChild(). (Bug #67)
    /// @version 2014-02-16/GGB - Added source extraction tab.
    /// @version 2013-06-02/GGB - Added toolbar height.
    /// @version 2013-05-31/GGB - Function created.

    void CDialogOptions::setupGeneral()
    {
        // General Tab

      lineEditUserName = findChild<QLineEdit *>("lineEditUserName");
      spinBoxMaximumThreads = findChild<QSpinBox *>("spinBoxMaximumThreads");
      radioButtonLocalTime = findChild<QRadioButton *>("radioButtonLocalTime");
      radioButtonUTC = findChild<QRadioButton *>("radioButtonUTC");
      spinBoxLastOpenedFiles = findChild<QSpinBox *>("spinBoxLastOpenedFiles");
      spinBoxHistogramBins = findChild<QSpinBox *>("spinBoxHistogramBins");
      spinBoxToolbarHeight = findChild<QSpinBox *>("spinBoxToolbarHeight");

      lineEditUserName->setText(settings::astroManagerSettings->value(settings::SETTINGS_OBSERVER, QVariant("")).toString());
      spinBoxMaximumThreads->setValue(settings::astroManagerSettings->value(settings::MAX_THREADS, QVariant(2)).toInt());
      spinBoxMaximumThreads->setMaximum(std::thread::hardware_concurrency());   // Set the maximum number of threads.
      if (settings::astroManagerSettings->value(settings::SETTINGS_LT, QVariant(false)).toBool())
      {
        radioButtonLocalTime->setChecked(true);
      }
      else
      {
        radioButtonUTC->setChecked(true);
      }
      spinBoxLastOpenedFiles->setValue(settings::astroManagerSettings->value(settings::FILE_LASTOPENEDDEPTH, QVariant(5)).toInt());
      spinBoxHistogramBins->setValue(settings::astroManagerSettings->value(settings::DW_HISTOGRAM_BINS, QVariant(128)).toInt());
      spinBoxToolbarHeight->setValue(settings::astroManagerSettings->value(settings::TOOLBAR_HEIGHT, QVariant(16)).toInt());

        // Directories Tab

      lineEditLogfileDirectory = findChild<QLineEdit *>("lineEditLogfileDirectory");
      lineEditDarkDirectory = findChild<QLineEdit *>("lineEditDarkDirectory");
      lineEditFlatDirectory = findChild<QLineEdit *>("lineEditFlatDirectory");
      lineEditBiasDirectory = findChild<QLineEdit *>("lineEditBiasDirectory");
      lineEditMasterDarkDirectory = findChild<QLineEdit *>("lineEditMasterDarkDirectory");
      lineEditMasterFlatDirectory = findChild<QLineEdit *>("lineEditMasterFlatDirectory");
      lineEditMasterBiasDirectory = findChild<QLineEdit *>("lineEditMasterBiasDirectory");
      checkBoxDarkDirectory = findChild<QCheckBox *>("checkBoxDarkDirectory");
      checkBoxFlatDirectory = findChild<QCheckBox *>("checkBoxFlatDirectory");
      checkBoxBiasDirectory = findChild<QCheckBox *>("checkBoxBiasDirectory");

      lineEditLogfileDirectory->setText(settings::astroManagerSettings->value(settings::SETTINGS_LOGDIR, QVariant("./log")).toString());
      lineEditDarkDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant("")).toString());
      lineEditFlatDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant("")).toString());
      lineEditBiasDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant("")).toString());
      lineEditMasterDarkDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant("")).toString());
      lineEditMasterFlatDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERFLAT_DIRECTORY, QVariant("")).toString());
      lineEditMasterBiasDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant("")).toString());
      checkBoxDarkDirectory->setChecked(settings::astroManagerSettings->value(settings::DARKFRAME_DIRECTORY_AUTOUPDATE, QVariant(true)).toBool());
      checkBoxFlatDirectory->setChecked(settings::astroManagerSettings->value(settings::FLATFRAME_DIRECTORY_AUTOUPDATE, QVariant(true)).toBool());
      checkBoxBiasDirectory->setChecked(settings::astroManagerSettings->value(settings::BIASFRAME_DIRECTORY_AUTOUPDATE, QVariant(true)).toBool());

        // Source Extraction tab

      comboBoxSEAlgorithm = findChild<QComboBox *>("comboBoxSEAlgorithm");
      checkBoxSEAstrometry = findChild<QCheckBox *>("checkBoxSEAstrometry");
      checkBoxSEPhotometry = findChild<QCheckBox *>("checkBoxSEPhotometry");

      for(const auto iter : SEAlgorithms)
      {
        comboBoxSEAlgorithm->addItem(QString::fromStdString(iter.second), QVariant(iter.first));
      }

      int SEA = settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_ALGORITHM, QVariant(1)).toInt();

      int index = comboBoxSEAlgorithm->findData(SEA);
      if (index != -1)
      {
        comboBoxSEAlgorithm->setCurrentIndex(index);
      }

      checkBoxSEAstrometry->setChecked(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_ADD_ASTROMETRY, QVariant(true)).toBool());
      checkBoxSEPhotometry->setChecked(settings::astroManagerSettings->value(settings::SOURCE_EXTRACTION_ADD_PHOTOMETRY, QVariant(false)).toBool());
    }

    /// @brief Sets up the data for the image managementoptions.
    /// @throws GCL::CRuntimeAssert
    /// @version 2017-08-24/GGB - Function created.

    void CDialogOptions::setupImageManager()
    {
      checkBoxRegisterOnOpen = findChild<QCheckBox *>("checkBoxRegisterOnOpen");
      checkBoxUploadARID = findChild<QCheckBox *>("checkBoxUploadARID");
      checkBoxUploadOriginal = findChild<QCheckBox *>("checkBoxUploadOriginal");
      groupBoxStoreVersions = findChild<QGroupBox *>("groupBoxStoreVersions");
      spinBoxMaximumVersions = findChild<QSpinBox *>("spinBoxMaximumVersions");

      checkBoxRegisterOnOpen->setChecked(settings::astroManagerSettings->value(settings::IMAGING_DATABASE_REGISTERONOPEN, QVariant(true)).toBool());
      checkBoxUploadARID->setChecked(settings::astroManagerSettings->value(settings::IMAGING_DATABSE_SAVE, QVariant(true)).toBool());
      checkBoxUploadOriginal->setChecked(settings::astroManagerSettings->value(settings::IMAGING_DATABASE_SAVEORIGINAL, QVariant(true)).toBool());
      groupBoxStoreVersions->setChecked(settings::astroManagerSettings->value(settings::IMAGING_DATABASE_SAVEVERSIONS, QVariant(true)).toBool());
      spinBoxMaximumVersions->setValue(settings::astroManagerSettings->value(settings::IMAGING_DATABASE_MAXVERSIONS, QVariant(0)).toUInt());
    }

    /// @brief Sets up the photometry options.
    /// @throws CRuntimeAssert
    /// @version 2017-06-14/GGB - Update to Qt5
    /// @version 2015-07-31/GGB - Added runtime assertions.
    /// @version 2013-08-18/GGB - Added support for the ZMAG.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::setupPhotometry()
    {
      int colourIndex;
      QColor currentColour;

      checkBoxZMAG = findChild<QCheckBox *>("checkBoxZMAG");
      spinBoxApertureRadius = findChild<QSpinBox *>("spinBoxApertureRadius");
      spinBoxInnerSkyRadius = findChild<QSpinBox *>("spinBoxInnerSkyRadius");
      spinBoxOuterSkyRadius = findChild<QSpinBox *>("spinBoxOuterSkyRadius");
      comboBoxPhotometryColour = findChild<QComboBox *>("comboBoxPhotometryColour");
      comboBoxPhotometrySelectedColour = findChild<QComboBox *>("comboBoxPhotometrySelectedColour");
      lineEditCSVDirectory = findChild<QLineEdit *>("lineEditCSVDirectory");
      checkBoxCSVDirectory = findChild<QCheckBox *>("checkBoxCSVDirectory");

      checkBoxZMAG->setChecked(settings::astroManagerSettings->value(settings::PHOTOMETRY_USEZMAG, QVariant(false)).toBool());
      spinBoxApertureRadius->setValue(settings::astroManagerSettings->value(settings::PHOTOMETRY_RADIUS1, QVariant((int) 6)).toInt());
      spinBoxInnerSkyRadius->setValue(settings::astroManagerSettings->value(settings::PHOTOMETRY_RADIUS2, QVariant((int) 10)).toInt());
      spinBoxOuterSkyRadius->setValue(settings::astroManagerSettings->value(settings::PHOTOMETRY_RADIUS3, QVariant((int) 15)).toInt());

      populateComboBoxColour(comboBoxPhotometryColour);
      currentColour = settings::astroManagerSettings->value(settings::PHOTOMETRY_INDICATOR_COLOUR, QVariant(QColor(Qt::red))).value<QColor>();
      colourIndex = comboBoxPhotometryColour->findData(currentColour);
      comboBoxPhotometryColour->setCurrentIndex(colourIndex);

      populateComboBoxColour(comboBoxPhotometrySelectedColour);
      currentColour = settings::astroManagerSettings->value(settings::PHOTOMETRY_INDICATOR_SELECTEDCOLOUR,
                                                    QVariant(QColor(Qt::yellow))).value<QColor>();
      colourIndex = comboBoxPhotometrySelectedColour->findData(currentColour);
      comboBoxPhotometrySelectedColour->setCurrentIndex(colourIndex);

      lineEditCSVDirectory->setText(settings::astroManagerSettings->value(settings::PHOTOMETRY_CSVDIRECTORY, QVariant("")).toString());
      checkBoxCSVDirectory->setChecked(settings::astroManagerSettings->value(settings::PHOTOMETRY_CSVDIRECTORY_UPDATE, QVariant(true)).toBool());
    }

    /// @brief Sets up the dialog
    /// @throws CRuntimeAssert.
    /// @version 2017-06-18/GGB - Use CDialog::findChild(). (Bug #67)
    /// @version 2014-02-09/GGB - Added Source extraction page.
    /// @version 2013-03-30/GGB - Function created.

    void CDialogOptions::setupUI()
    {
      QListWidgetItem *listWidgetItem;
      QDialogButtonBox *buttonBox;

      listWidget = findChild<QListWidget *>("listWidget");
      stackedWidget = findChild<QStackedWidget *>("stackedWidget");
      buttonBox = findChild<QDialogButtonBox *>("buttonBox");

      listWidgetItem = new QListWidgetItem(QIcon(":/images/Gear.png"), tr("General"));
      listWidget->addItem(listWidgetItem);
      listWidgetItem = new QListWidgetItem(QIcon(":/images/astrometry.png"), tr("Astrometry"));
      listWidget->addItem(listWidgetItem);
      listWidgetItem = new QListWidgetItem(QIcon(":/images/photometry.png"), tr("Photometry"));
      listWidget->addItem(listWidgetItem);
      listWidgetItem = new QListWidgetItem(QIcon(":/icons/database/database_gear.png"), tr("Database"));
      listWidget->addItem(listWidgetItem);
      listWidgetItem = new QListWidgetItem(QIcon(":/images/file_manager.png"), tr("Image Management"));
      listWidget->addItem(listWidgetItem);
      listWidgetItem = nullptr;

      connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
      connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
      connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));

      connect(findChild<QPushButton *>("pushButtonLogfileDirectory"), SIGNAL(clicked(bool)), this,
        SLOT(eventLogFileDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonDarkDirectory"), SIGNAL(clicked(bool)), this,
        SLOT(eventDarkDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonFlatDirectory"), SIGNAL(clicked(bool)), this,
        SLOT(eventFlatDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonBiasDirectory"), SIGNAL(clicked(bool)), this,
        SLOT(eventBiasDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonCSVDirectory"), SIGNAL(clicked(bool)), this,
        SLOT(eventCSVDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonResetWarnings"), SIGNAL(clicked(bool)), this,
              SLOT(eventResetWarnings(bool)));

      setupGeneral();
      setupAstrometry();
      setupPhotometry();
      setupDatabase();
      setupImageManager();
    }

    /// @brief Sets up the data and widget pointers for the Weather Database section.
    /// @throws CRuntimeAssert
    /// @version 2017-06-18/GGB - Function created.

    void CDialogOptions::setupWeatherDatabase()
    {
      groupBoxWeather = findChild<QGroupBox *>("groupBoxWeather");
        comboBoxWeatherDatabaseType = findChild<QComboBox *>("comboBoxWeatherDatabaseType");
        stackedWidgetWeather = findChild<QStackedWidget *>("stackedWidgetWeather");
        pushButtonWeatherTestConnection = findChild<QPushButton *>("pushButtonWeatherTestConnection");
        textEditWeatherTestConnection = findChild<QTextEdit *>("textEditWeatherTestConnection");

            // MySQL

          lineEditWeatherMySQLHost = findChild<QLineEdit *>("lineEditWeatherMySQLHost");
          lineEditWeatherMySQLDatabase = findChild<QLineEdit *>("lineEditWeatherMySQLDatabase");
          spinBoxWeatherMySQLPort = findChild<QSpinBox *>("spinBoxWeatherMySQLPort");
          lineEditWeatherMySQLUser = findChild<QLineEdit *>("lineEditWeatherMySQLUser");
          lineEditWeatherMySQLPassword = findChild<QLineEdit *>("lineEditWeatherMySQLPassword");

            // PostgreSQL

          lineEditWeatherPostgreSQLHost = findChild<QLineEdit *>("lineEditWeatherPostgreSQLHost");
          lineEditWeatherPostgreSQLDatabase = findChild<QLineEdit *>("lineEditWeatherPostgreSQLDatabase");
          spinBoxWeatherPostgreSQLPort = findChild<QSpinBox *>("spinBoxWeatherPostgreSQLPort");
          lineEditWeatherPostgreSQLUser = findChild<QLineEdit *>("lineEditWeatherPostgreSQLUser");
          lineEditWeatherPostgreSQLPassword = findChild<QLineEdit *>("lineEditWeatherPostgreSQLPassword");

            // Oracle

          lineEditWeatherOracleHost = findChild<QLineEdit *>("lineEditWeatherOracleHost");
          lineEditWeatherOracleSchema = findChild<QLineEdit *>("lineEditWeatherOracleSchema");
          spinBoxWeatherOraclePort = findChild<QSpinBox *>("spinBoxWeatherOraclePort");
          lineEditWeatherOracleUser = findChild<QLineEdit *>("lineEditWeatherOracleUser");
          lineEditWeatherOraclePassword = findChild<QLineEdit *>("lineEditWeatherOraclePassword");

      groupBoxWeather->setChecked(!settings::astroManagerSettings->value(settings::WEATHER_DATABASE_DISABLE, QVariant(true)).toBool());

        // Add the items to the combo box. The driver map key value is used as user data to identify the driver selected.

      std::for_each(database::CDatabase::databaseDrivers.begin(),database::CDatabase::databaseDrivers.end(),
                    [&](std::pair<const int, database::CDatabase::SDatabaseDriver> &dt)
                       {comboBoxWeatherDatabaseType->addItem(dt.second.driverText, QVariant(dt.first));} );

        // MySQL

      lineEditWeatherMySQLHost->setText(settings::astroManagerSettings->value(settings::WEATHER_MYSQL_HOSTADDRESS, "localhost").toString());
      lineEditWeatherMySQLDatabase->setText(settings::astroManagerSettings->value(settings::WEATHER_MYSQL_DATABASENAME, "").toString());
      spinBoxWeatherMySQLPort->setValue(settings::astroManagerSettings->value(settings::WEATHER_MYSQL_PORT, QVariant(3306)).toInt());
      lineEditWeatherMySQLUser->setText(settings::astroManagerSettings->value(settings::WEATHER_MYSQL_USERNAME, "").toString());
      lineEditWeatherMySQLPassword->setText(settings::astroManagerSettings->value(settings::WEATHER_MYSQL_PASSWORD, "").toString());

        // PostgreSQL

      lineEditWeatherPostgreSQLHost->setText(settings::astroManagerSettings->value(settings::WEATHER_POSTGRESQL_HOSTADDRESS, "localhost").toString());
      lineEditWeatherPostgreSQLDatabase->setText(settings::astroManagerSettings->value(settings::WEATHER_POSTGRESQL_DATABASENAME, "").toString());
      spinBoxWeatherPostgreSQLPort->setValue(settings::astroManagerSettings->value(settings::WEATHER_POSTGRESQL_PORT, QVariant(5432)).toInt());
      lineEditWeatherPostgreSQLUser->setText(settings::astroManagerSettings->value(settings::WEATHER_POSTGRESQL_USERNAME, "").toString());
      lineEditWeatherPostgreSQLPassword->setText(settings::astroManagerSettings->value(settings::WEATHER_POSTGRESQL_PASSWORD, "").toString());

        // Oracle

      lineEditWeatherOracleHost->setText(settings::astroManagerSettings->value(settings::WEATHER_ORACLE_HOSTADDRESS, "localhost").toString());
      lineEditWeatherOracleSchema->setText(settings::astroManagerSettings->value(settings::WEATHER_ORACLE_SCHEMANAME, "").toString());
      spinBoxWeatherOraclePort->setValue(settings::astroManagerSettings->value(settings::WEATHER_ORACLE_PORT, QVariant(1521)).toInt());
      lineEditWeatherOracleUser->setText(settings::astroManagerSettings->value(settings::WEATHER_ORACLE_USERNAME, "").toString());
      lineEditWeatherOraclePassword->setText(settings::astroManagerSettings->value(settings::WEATHER_ORACLE_PASSWORD, "").toString());

      connect(comboBoxWeatherDatabaseType, SIGNAL(currentIndexChanged(int)), this, SLOT(eventWeatherDatabaseCombo(int)));

        // Select the current driver.

      if (groupBoxWeather->isChecked())
      {
        bool driverValid = false;

        std::string szDatabase = settings::astroManagerSettings->value(settings::WEATHER_DATABASE_DBMS).toString().toStdString();

        if ( (szDatabase == "MySQL") && (database::CDatabase::isDriverAvailable(database::CDatabase::SQLDB_MYSQL)) )
        {
          stackedWidgetWeather->setCurrentIndex(database::CDatabase::SQLDB_MYSQL);
          driverValid = true;
        }
        else if ( (szDatabase == "Oracle") && (database::CDatabase::isDriverAvailable(database::CDatabase::SQLDB_QOCI)) )
        {
          stackedWidgetWeather->setCurrentIndex(database::CDatabase::SQLDB_QOCI);
          driverValid = true;
        }
        else if ( (szDatabase == "SQLite") && (database::CDatabase::isDriverAvailable(database::CDatabase::SQLDB_SQLITE)) )
        {
          stackedWidgetWeather->setCurrentIndex(database::CDatabase::SQLDB_SQLITE);
          driverValid = true;
        }
        else if ( (szDatabase == "PostgreSQL") && (database::CDatabase::isDriverAvailable(database::CDatabase::SQLDB_PSQL)) )
        {
          stackedWidgetWeather->setCurrentIndex(database::CDatabase::SQLDB_PSQL);
          driverValid = true;
        }
        else if ( (szDatabase == "ODBC") && (database::CDatabase::isDriverAvailable(database::CDatabase::SQLDB_ODBC)) )
        {
          stackedWidgetWeather->setCurrentIndex(database::CDatabase::SQLDB_ODBC);
          driverValid = true;
        }
        else
        {
          CODE_ERROR(astroManager);
        }

        if (!driverValid)
        {
            // The currently selected driver is not available for this system.
        }
      }
      else
      {
        stackedWidgetWeather->setCurrentIndex(database::CDatabase::SQLDB_MYSQL);
      }

      connect(pushButtonWeatherTestConnection, SIGNAL(clicked(bool)), this, SLOT(eventWeatherTestConnection(bool)));
    }


  }  // namespace dialogs
}  // namespace AstroManager

