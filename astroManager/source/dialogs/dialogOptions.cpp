//*********************************************************************************************************************************
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

#include "include/dialogs/dialogOptions.h"

  // Standard C++ library header files

#include <algorithm>
#include <thread>

  // Miscellaneous library header files.

#include <ACL>
#include "boost/locale.hpp"
#include <GCL>
#include <QCL>

  // astroManager application header files

#include "include/error.h"
#include "include/settings.h"
#include "include/astroManager.h"

namespace astroManager
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
      {Qt::white, QString::fromStdString(boost::locale::translate("White").str())},
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
    /// @param[in] index: The index of the currently selected option.
    /// @throws None.
    /// @version 2015-06-14/GGB - Function created.

    void CDialogOptions::eventARIDDatabaseCombo(int index)
    {
      stackedWidgetARID->setCurrentIndex(comboBoxARIDDatabaseType->itemData(index, Qt::UserRole).toInt() );
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
        case QCL::CDatabase::SQLDB_MYSQL:
        {
          szDBMS = "QMYSQL";
          szHostName = lineEditARIDMySQLHost->text();
          szPortNumber = spinBoxARIDMySQLPort->value();
          szDatabaseName = lineEditARIDMySQLDatabase->text();
          szUserName = lineEditARIDMySQLUser->text();
          szPassword = lineEditARIDMySQLPassword->text();
          break;
        };
        case QCL::CDatabase::SQLDB_ODBC:
        {
          szDBMS = "QODBC";
          break;
        };
        case QCL::CDatabase::SQLDB_PSQL:
        {
          szDBMS = "QPSQL";
          szHostName = lineEditARIDPostgreSQLHost->text();
          szPortNumber = spinBoxARIDPostgreSQLPort->value();
          szDatabaseName = lineEditARIDPostgreSQLDatabase->text();
          szUserName = lineEditARIDPostgreSQLUser->text();
          szPassword = lineEditARIDPostgreSQLPassword->text();
          break;
        };
        case QCL::CDatabase::SQLDB_QOCI:
        {
          szDBMS = "QOCI";
          szHostName = lineEditARIDOracleHost->text();
          szPortNumber = spinBoxARIDOraclePort->value();
          szDatabaseName = lineEditARIDOracleSchema->text();
          szUserName = lineEditARIDOracleUser->text();
          szPassword = lineEditARIDOraclePassword->text();
          break;
        }
        case QCL::CDatabase::SQLDB_SQLITE:
        {
          szDBMS = "QSQLITE";
          break;
        }
        default:
        {
          CODE_ERROR;
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

    /// @brief      Processes the event when a database type is chosen.
    /// @param[in]  index: The index of the currently selected option.
    /// @throws     None.
    /// @version    2018-09-24/GGB - Function created.

    void CDialogOptions::eventATIDDatabaseCombo(int index)
    {
      stackedWidgetATID->setCurrentIndex(comboBoxATIDDatabaseType->itemData(index, Qt::UserRole).toInt() );
    }

    /// @brief Test the connection to the ATID database.
    /// @throws None.
    /// @version 2018-09-22/GGB - Function created.

    void CDialogOptions::eventATIDTestConnection()
    {
      QString szDBMS;
      QString szHostName;
      std::uint16_t szPortNumber;
      QString szDatabaseName;
      QString szUserName;
      QString szPassword;

        // Let the user know that we are beginning

      textEditATIDTestConnection->clear();
      textEditATIDTestConnection->setTextColor(QColor("black"));
      textEditATIDTestConnection->append(tr("Starting Test..."));

        // Determine the strings that we need.

      szDBMS = comboBoxATIDDatabaseType->currentText();

      switch (comboBoxATIDDatabaseType->itemData(comboBoxATIDDatabaseType->currentIndex(), Qt::UserRole).toInt())
      {
        case QCL::CDatabase::SQLDB_MYSQL:
        {
          szDBMS = "QMYSQL";
          szHostName = lineEditATIDMySQLHost->text();
          szPortNumber = spinBoxATIDMySQLPort->value();
          szDatabaseName = lineEditATIDMySQLDatabase->text();
          szUserName = lineEditATIDMySQLUser->text();
          szPassword = lineEditATIDMySQLPassword->text();
          break;
        };
        case QCL::CDatabase::SQLDB_ODBC:
        {
          szDBMS = "QODBC";
          break;
        };
        case QCL::CDatabase::SQLDB_PSQL:
        {
          szDBMS = "QPSQL";
          szHostName = lineEditATIDPostgreSQLHost->text();
          szPortNumber = spinBoxATIDPostgreSQLPort->value();
          szDatabaseName = lineEditATIDPostgreSQLDatabase->text();
          szUserName = lineEditATIDPostgreSQLUser->text();
          szPassword = lineEditATIDPostgreSQLPassword->text();
          break;
        };
        case QCL::CDatabase::SQLDB_QOCI:
        {
          szDBMS = "QOCI";
          szHostName = lineEditATIDOracleHost->text();
          szPortNumber = spinBoxATIDOraclePort->value();
          szDatabaseName = lineEditATIDOracleSchema->text();
          szUserName = lineEditATIDOracleUser->text();
          szPassword = lineEditATIDOraclePassword->text();
          break;
        }
        case QCL::CDatabase::SQLDB_SQLITE:
        {
          szDBMS = "QSQLITE";
          break;
        }
        default:
        {
          CODE_ERROR;
          break;
        }
      };

      textEditATIDTestConnection->setTextColor(QColor("darkBlue"));
      textEditATIDTestConnection->append(tr("Driver name: ") + szDBMS);
      textEditATIDTestConnection->append(tr("Host name: ") + szHostName);
      textEditATIDTestConnection->append(QString(tr("Host port: %1")).arg(szPortNumber));
      textEditATIDTestConnection->append(tr("Database name: ") + szDatabaseName);
      textEditATIDTestConnection->append(tr("User name: ") + szUserName);
      textEditATIDTestConnection->append(tr("Password: ") + szPassword);
      textEditATIDTestConnection->setTextColor(QColor("magenta"));
      textEditATIDTestConnection->append(tr("Attempting connection..."));

      {
        QSqlDatabase testConnection = QSqlDatabase::addDatabase(szDBMS, "testConnection");
        testConnection.setHostName(szHostName);
        testConnection.setPort(szPortNumber);
        testConnection.setDatabaseName(szDatabaseName);
        testConnection.setUserName(szUserName);
        testConnection.setPassword(szPassword);

        if (testConnection.open())
        {
          textEditATIDTestConnection->setTextColor(QColor("darkGreen"));
          textEditATIDTestConnection->append(QString::fromStdString(boost::locale::translate("Connection succesful.").str()));

          testConnection.close();
        }
        else
        {
          textEditATIDTestConnection->setTextColor(QColor("darkRed"));
          textEditATIDTestConnection->append(tr("Connection failed."));

            // List the error strings.

          QSqlError error = testConnection.lastError();

          textEditATIDTestConnection->append(tr("Error while connecting to Database: ") + szHostName + "." + szDatabaseName);
          textEditATIDTestConnection->append(tr("Error returned by driver: ") + error.nativeErrorCode());
          textEditATIDTestConnection->append(tr("Text returned by driver: ") + error.driverText());
          textEditATIDTestConnection->append(tr("Text returned by database: ") + error.databaseText());
        };
      };

      QSqlDatabase::removeDatabase("testConnection");

      textEditATIDTestConnection->setTextColor(QColor("black"));
      textEditATIDTestConnection->append(tr("Test Complete."));
    }

    /// @brief Responds to the useSimbad checkbox being pressed.
    /// @throws None.
    /// @version 2018-09-27/GGB - Function created.

    void CDialogOptions::eventATIDUseSimbad(bool isChecked)
    {
      if (isChecked)
      {
        groupBoxATID->setChecked(false);
        pushButtonATIDTestConnection->setEnabled(false);
      }
      else
      {
        groupBoxATID->setChecked(true);
        pushButtonATIDTestConnection->setEnabled(false);
      }
    }

    /// @brief Responds to the useSimbad checkbox being pressed.
    /// @throws None.
    /// @version 2018-09-27/GGB - Function created.

    void CDialogOptions::eventATIDUseATID(bool isChecked)
    {
      if (isChecked)
      {
        checkBoxUseSimbad->setChecked(false);
        pushButtonATIDTestConnection->setEnabled(true);
      }
      else
      {
        checkBoxUseSimbad->setChecked(false);
        pushButtonATIDTestConnection->setEnabled(true);
      }
    }

    /// @brief      Processes the event to choose the coment elements file.
    /// @throws     None.
    /// @version    2018-09-16/GGB - Function created.

    void CDialogOptions::eventCometEls()
    {
      QString fileName = lineEditCometEls->text();
      fileName = QFileDialog::getOpenFileName(dlg, tr("Comet Elements File"), fileName);

      if (!fileName.isNull())
      {
        lineEditCometEls->setText(fileName);
      };
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
    /// @throws None.
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

    /// @brief Responds to the button press for the data directory.
    /// @throws None.
    /// @version 2018-09-16/GGB - Function created.
    /// @todo Can these be converted into a single function with lookups?

    void CDialogOptions::eventDataDirectory()
    {
      QString directory = lineEditDataDirectory->text();
      directory = QFileDialog::getExistingDirectory(dlg, tr("Data File directory"), directory);

      if (!directory.isNull())
      {
        lineEditDataDirectory->setText(directory);
      }
    }

    /// @brief Responds to the button press for the flat frame directory.
    /// @throws None
    /// @version 2013-06-01/GGB - Function created.

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

    /// @brief Processes the event to choose the MPCORB file.
    /// @throws None.
    /// @version 2018-09-16/GGB - Function created.

    void CDialogOptions::eventMPCORB()
    {
      QString fileName = lineEditMPCORB->text();
      fileName = QFileDialog::getOpenFileName(dlg, tr("MPC Orbitals File"), fileName);

      if (!fileName.isNull())
      {
        lineEditMPCORB->setText(fileName);
      };
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

    /// @brief Processes the event to choose the coment elements file.
    /// @throws None.
    /// @version 2018-09-16/GGB - Function created.

    void CDialogOptions::eventTAIUTC()
    {
      QString fileName = lineEditTAIUTC->text();
      fileName = QFileDialog::getOpenFileName(dlg, tr("TAI-UTC data file"), fileName);

      if (!fileName.isNull())
      {
        lineEditTAIUTC->setText(fileName);
      };
    }

    /// @brief Processes the event to choose the coment elements file.
    /// @throws None.
    /// @version 2018-09-16/GGB - Function created.

    void CDialogOptions::eventUTCUT1()
    {
      QString fileName = lineEditUTCUT1->text();
      fileName = QFileDialog::getOpenFileName(dlg, tr("UTC-UT1 data file"), fileName);

      if (!fileName.isNull())
      {
        lineEditUTCUT1->setText(fileName);
      };
    }

    /// @brief Processes the event when a weather database type is chosen.
    /// @param[in] index - The index of the item chosen.
    /// @throws None.
    /// @version 2017-06-18/GGB - Function created.

    void CDialogOptions::eventWeatherDatabaseCombo(int index)
    {
      stackedWidgetWeather->setCurrentIndex(comboBoxWeatherDatabaseType->itemData(index, Qt::UserRole).toInt() );
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
        case QCL::CDatabase::SQLDB_MYSQL:
        {
          szDBMS = "QMYSQL";
          szHostName = lineEditWeatherMySQLHost->text();
          szPortNumber = spinBoxWeatherMySQLPort->value();
          szDatabaseName = lineEditWeatherMySQLDatabase->text();
          szUserName = lineEditWeatherMySQLUser->text();
          szPassword = lineEditWeatherMySQLPassword->text();
          break;
        };
        case QCL::CDatabase::SQLDB_ODBC:
        {
          szDBMS = "QODBC";
          break;
        };
        case QCL::CDatabase::SQLDB_PSQL:
        {
          szDBMS = "QPSQL";
          szHostName = lineEditWeatherPostgreSQLHost->text();
          szPortNumber = spinBoxWeatherPostgreSQLPort->value();
          szDatabaseName = lineEditWeatherPostgreSQLDatabase->text();
          szUserName = lineEditWeatherPostgreSQLUser->text();
          szPassword = lineEditWeatherPostgreSQLPassword->text();
          break;
        };
        case QCL::CDatabase::SQLDB_QOCI:
        {
          szDBMS = "QOCI";
          szHostName = lineEditWeatherOracleHost->text();
          szPortNumber = spinBoxWeatherOraclePort->value();
          szDatabaseName = lineEditWeatherOracleSchema->text();
          szUserName = lineEditWeatherOracleUser->text();
          szPassword = lineEditWeatherOraclePassword->text();
          break;
        }
        case QCL::CDatabase::SQLDB_SQLITE:
        {
          szDBMS = "QSQLITE";
          break;
        }
        default:
        {
          CODE_ERROR;
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

    /// @brief      Processes the save button.
    /// @throws     None.
    /// @version    2013-05-31/GGB - Function created.

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
        settings::astroManagerSettings->setValue(settings::ARID_DATABASE_DBMS, QVariant(comboBoxARIDDatabaseType->currentText()));

        switch (comboBoxARIDDatabaseType->itemData(comboBoxARIDDatabaseType->currentIndex(), Qt::UserRole).toInt())
        {
          case QCL::CDatabase::SQLDB_MYSQL:
          {
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_HOSTADDRESS, lineEditARIDMySQLHost->text());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_PORT, spinBoxARIDMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_DATABASENAME, lineEditARIDMySQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_USERNAME, lineEditARIDMySQLUser->text());
            settings::astroManagerSettings->setValue(settings::ARID_MYSQL_PASSWORD, lineEditARIDMySQLPassword->text());
            break;
          };
          case QCL::CDatabase::SQLDB_ODBC:
          {
            break;
          };
          case QCL::CDatabase::SQLDB_PSQL:
          {
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_HOSTADDRESS, lineEditARIDPostgreSQLHost->text());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_PORT, spinBoxARIDMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_DATABASENAME, lineEditARIDPostgreSQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_USERNAME, lineEditARIDPostgreSQLUser->text());
            settings::astroManagerSettings->setValue(settings::ARID_POSTGRESQL_PASSWORD, lineEditARIDPostgreSQLPassword->text());
            break;
          };
          case QCL::CDatabase::SQLDB_QOCI:
          {
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_HOSTADDRESS, lineEditARIDOracleHost->text());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_PORT, spinBoxARIDOraclePort->value());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_SCHEMANAME, lineEditARIDOracleSchema->text());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_USERNAME, lineEditARIDOracleUser->text());
            settings::astroManagerSettings->setValue(settings::ARID_ORACLE_PASSWORD, lineEditARIDOraclePassword->text());
            break;
          }
          case QCL::CDatabase::SQLDB_SQLITE:
          {
            break;
          }
          default:
          {
            CODE_ERROR;
            break;
          }
        };
      }
    }
    /// @brief Saves the ATID database options.
    /// @throws GCL::CCodeError(...)
    /// @version 2019-09-24/GGB - Function created.

    void CDialogOptions::saveATIDDatabase()
    {
      settings::astroManagerSettings->setValue(settings::ATID_DATABASE_USESIMBAD, checkBoxUseSimbad->isChecked());

      if ( groupBoxATID->isChecked() )
      {
        settings::astroManagerSettings->setValue(settings::ATID_DATABASE_DBMS, QVariant(comboBoxATIDDatabaseType->currentText()));

        switch (comboBoxATIDDatabaseType->itemData(comboBoxATIDDatabaseType->currentIndex(), Qt::UserRole).toInt())
        {
          case QCL::CDatabase::SQLDB_MYSQL:
          {
            settings::astroManagerSettings->setValue(settings::ATID_MYSQL_HOSTADDRESS, lineEditATIDMySQLHost->text());
            settings::astroManagerSettings->setValue(settings::ATID_MYSQL_PORT, spinBoxATIDMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::ATID_MYSQL_DATABASENAME, lineEditATIDMySQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::ATID_MYSQL_USERNAME, lineEditATIDMySQLUser->text());
            settings::astroManagerSettings->setValue(settings::ATID_MYSQL_PASSWORD, lineEditATIDMySQLPassword->text());
            break;
          };
          case QCL::CDatabase::SQLDB_ODBC:
          {
            break;
          };
          case QCL::CDatabase::SQLDB_PSQL:
          {
            settings::astroManagerSettings->setValue(settings::ATID_POSTGRESQL_HOSTADDRESS, lineEditATIDPostgreSQLHost->text());
            settings::astroManagerSettings->setValue(settings::ATID_POSTGRESQL_PORT, spinBoxATIDMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::ATID_POSTGRESQL_DATABASENAME, lineEditATIDPostgreSQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::ATID_POSTGRESQL_USERNAME, lineEditATIDPostgreSQLUser->text());
            settings::astroManagerSettings->setValue(settings::ATID_POSTGRESQL_PASSWORD, lineEditATIDPostgreSQLPassword->text());
            break;
          };
          case QCL::CDatabase::SQLDB_QOCI:
          {
            settings::astroManagerSettings->setValue(settings::ATID_ORACLE_HOSTADDRESS, lineEditATIDOracleHost->text());
            settings::astroManagerSettings->setValue(settings::ATID_ORACLE_PORT, spinBoxATIDOraclePort->value());
            settings::astroManagerSettings->setValue(settings::ATID_ORACLE_SCHEMANAME, lineEditATIDOracleSchema->text());
            settings::astroManagerSettings->setValue(settings::ATID_ORACLE_USERNAME, lineEditATIDOracleUser->text());
            settings::astroManagerSettings->setValue(settings::ATID_ORACLE_PASSWORD, lineEditATIDOraclePassword->text());
            break;
          }
          case QCL::CDatabase::SQLDB_SQLITE:
          {
            break;
          }
          default:
          {
            CODE_ERROR;
            break;
          }
        };
      }
    }

    /// @brief Saves the database values.
    /// @throws None.
    /// @version 2018-09-24/GGB - Added support for ARID database.
    /// @version 2017-06-18/GGB - Added support for the weather database.
    /// @version 2015-06-14/GGB - Moved ARID into saveARIDDatabase() function.
    /// @version 2014-02-16/GGB - Added source extraction tab.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::saveDatabase()
    {
      saveARIDDatabase();
      saveATIDDatabase();
      saveWeatherDatabase();
    }

    /// @brief      Writes the general values to the settings.
    /// @version    2020-09-19/GGB - Update the CTargetMinorPlanet and CTargetComets file names.
    /// @version    2018-09-16/GGB - Added support for data directory
    /// @version    2017-06-25/GGB - Updates the number of threads used. (Bug #72)
    /// @version    2013-06-02/GGB - Added toolbar height.
    /// @version    2013-06-01/GGB - Function created.

    void CDialogOptions::saveGeneral()
    {
        // General

      settings::astroManagerSettings->setValue(settings::SETTINGS_OBSERVER, QVariant(lineEditUserName->text()));
      settings::astroManagerSettings->setValue(settings::MAX_THREADS, QVariant(spinBoxMaximumThreads->value()));
      settings::astroManagerSettings->setValue(settings::SETTINGS_LT, QVariant(radioButtonLocalTime->isChecked()));
      settings::astroManagerSettings->setValue(settings::FILE_LASTOPENEDDEPTH, QVariant(spinBoxLastOpenedFiles->value()));
      settings::astroManagerSettings->setValue(settings::DW_HISTOGRAM_BINS, QVariant(spinBoxHistogramBins->value()));
      settings::astroManagerSettings->setValue(settings::TOOLBAR_HEIGHT, QVariant(spinBoxToolbarHeight->value()));

        // Directories

      settings::astroManagerSettings->setValue(settings::DIRECTORY_LOGDIR, QVariant(lineEditLogfileDirectory->text()));
      settings::astroManagerSettings->setValue(settings::DIRECTORY_DATA, QVariant(lineEditDataDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant(lineEditDarkDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant(lineEditFlatDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant(lineEditBiasDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant(lineEditMasterDarkDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERFLAT_DIRECTORY, QVariant(lineEditMasterFlatDirectory->text()));
      settings::astroManagerSettings->setValue(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant(lineEditMasterBiasDirectory->text()));

      settings::astroManagerSettings->setValue(settings::DARKFRAME_DIRECTORY_AUTOUPDATE, QVariant(checkBoxDarkDirectory->isChecked()));
      settings::astroManagerSettings->setValue(settings::FLATFRAME_DIRECTORY_AUTOUPDATE, QVariant(checkBoxFlatDirectory->isChecked()));
      settings::astroManagerSettings->setValue(settings::BIASFRAME_DIRECTORY_AUTOUPDATE, QVariant(checkBoxBiasDirectory->isChecked()));

        // Data Files

      settings::astroManagerSettings->setValue(settings::FILE_TAIUTC, QVariant(lineEditTAIUTC->text()));
      settings::astroManagerSettings->setValue(settings::FILE_UTCUT1, QVariant(lineEditUTCUT1->text()));
      settings::astroManagerSettings->setValue(settings::FILE_MPCORB, QVariant(lineEditMPCORB->text()));
      ACL::CTargetMinorPlanet::setFileName(lineEditMPCORB->text().toStdString());
      settings::astroManagerSettings->setValue(settings::FILE_COMETELS, QVariant(lineEditCometEls->text()));
      ACL::CTargetComet::setFileName(lineEditCometEls->text().toStdString());

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
          case QCL::CDatabase::SQLDB_MYSQL:
          {
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_HOSTADDRESS, lineEditWeatherMySQLHost->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_PORT, spinBoxWeatherMySQLPort->value());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_DATABASENAME, lineEditWeatherMySQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_USERNAME, lineEditWeatherMySQLUser->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_MYSQL_PASSWORD, lineEditWeatherMySQLPassword->text());
            break;
          };
          case QCL::CDatabase::SQLDB_ODBC:
          {
            break;
          };
          case QCL::CDatabase::SQLDB_PSQL:
          {
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_HOSTADDRESS, lineEditWeatherPostgreSQLHost->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_PORT, spinBoxWeatherPostgreSQLPort->value());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_DATABASENAME, lineEditWeatherPostgreSQLDatabase->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_USERNAME, lineEditWeatherPostgreSQLUser->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_POSTGRESQL_PASSWORD, lineEditWeatherPostgreSQLPassword->text());
            break;
          };
          case QCL::CDatabase::SQLDB_QOCI:
          {
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_HOSTADDRESS, lineEditWeatherOracleHost->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_PORT, spinBoxWeatherOraclePort->value());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_SCHEMANAME, lineEditWeatherOracleSchema->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_USERNAME, lineEditWeatherOracleUser->text());
            settings::astroManagerSettings->setValue(settings::WEATHER_ORACLE_PASSWORD, lineEditWeatherOraclePassword->text());
            break;
          }
          case QCL::CDatabase::SQLDB_SQLITE:
          {
            break;
          }
          default:
          {
            CODE_ERROR;
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
        lineEditARIDMySQLDatabase = findChild<QLineEdit *>("lineEditARIDMySQLDatabase");
        lineEditARIDMySQLUser = findChild<QLineEdit *>("lineEditARIDMySQLUser");
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

      std::for_each(QCL::CDatabase::databaseDrivers.begin(),QCL::CDatabase::databaseDrivers.end(),
                    [&](std::pair<const int, QCL::CDatabase::SDatabaseDriver> &dt)
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
          stackedWidgetARID->setCurrentIndex(QCL::CDatabase::SQLDB_MYSQL);
        }
        else if (szDatabase == "Oracle")
        {
          stackedWidgetARID->setCurrentIndex(QCL::CDatabase::SQLDB_QOCI);
        }
        else if (szDatabase == "SQLite")
        {
          stackedWidgetARID->setCurrentIndex(QCL::CDatabase::SQLDB_SQLITE);
        }
        else if (szDatabase == "PostgreSQL")
        {
          stackedWidgetARID->setCurrentIndex(QCL::CDatabase::SQLDB_PSQL);
        }
        else if (szDatabase == "ODBC")
        {
          stackedWidgetARID->setCurrentIndex(QCL::CDatabase::SQLDB_ODBC);
        }
        else
        {
          CODE_ERROR;
        }
      }
      else
      {
        stackedWidgetARID->setCurrentIndex(QCL::CDatabase::SQLDB_MYSQL);
      }
      connect(pushButtonARIDTestConnection, SIGNAL(clicked(bool)), this, SLOT(eventARIDTestConnection(bool)));
    }

    /// @brief Sets up the ATID database options.
    /// @throws CRuntimeAssert
    /// @version 2018-09-24/GGB - Function created.

    void CDialogOptions::setupATIDDatabase()
    {
      checkBoxUseSimbad = findChild<QCheckBox *>("checkBoxUseSimbad");
      groupBoxATID = findChild<QGroupBox *>("groupBoxATID");
        comboBoxATIDDatabaseType = findChild<QComboBox *>("comboBoxATIDDatabaseType");
        stackedWidgetATID = findChild<QStackedWidget *>("stackedWidgetATID");
        pushButtonATIDTestConnection = findChild<QPushButton *>("pushButtonATIDTestConnection");
        textEditATIDTestConnection = findChild<QTextEdit *>("textEditATIDTestConnection");

          // MySQL

        lineEditATIDMySQLHost = findChild<QLineEdit *>("lineEditATIDMySQLHost");
        spinBoxATIDMySQLPort = findChild<QSpinBox *>("spinBoxATIDMySQLPort");
        lineEditATIDMySQLDatabase = findChild<QLineEdit *>("lineEditATIDMySQLDatabase");
        lineEditATIDMySQLUser = findChild<QLineEdit *>("lineEditATIDMySQLUser");
        lineEditATIDMySQLPassword = findChild<QLineEdit *>("lineEditATIDMySQLPassword");

          // PostgreSQL

        lineEditATIDPostgreSQLHost = findChild<QLineEdit *>("lineEditATIDPostgreHost");
        spinBoxATIDPostgreSQLPort = findChild<QSpinBox *>("spinBoxATIDPostgreSQLPort");
        lineEditATIDPostgreSQLDatabase = findChild<QLineEdit *>("lineEditATIDPostgreSQLDatabase");
        lineEditATIDPostgreSQLUser = findChild<QLineEdit *>("lineEditATIDPostgreSQLUser");
        lineEditATIDPostgreSQLPassword = findChild<QLineEdit *>("lineEditATIDPostgreSQLPassword");

          // Oracle

        lineEditATIDOracleHost = findChild<QLineEdit *>("lineEditATIDOracleHost");
        spinBoxATIDOraclePort = findChild<QSpinBox *>("spinBoxATIDOraclePort");
        lineEditATIDOracleSchema = findChild<QLineEdit *>("lineEditATIDOracleSchema");
        lineEditATIDOracleUser = findChild<QLineEdit *>("lineEditATIDOracleUser");
        lineEditATIDOraclePassword = findChild<QLineEdit *>("lineEditATIDOraclePassword");

        // Add the items to the combo box. The driver map key value is used as user data to identify the driver selected.

      std::for_each(QCL::CDatabase::databaseDrivers.begin(),QCL::CDatabase::databaseDrivers.end(),
                    [&](std::pair<const int, QCL::CDatabase::SDatabaseDriver> &dt)
                       {comboBoxATIDDatabaseType->addItem(dt.second.driverText, QVariant(dt.first));} );

        // Set the current/default values from the settings file.
        // MySQL

      lineEditATIDMySQLHost->setText(settings::astroManagerSettings->value(settings::ATID_MYSQL_HOSTADDRESS, "localhost").toString());
      spinBoxATIDMySQLPort->setValue(settings::astroManagerSettings->value(settings::ATID_MYSQL_PORT, QVariant(3306)).toInt());
      lineEditATIDMySQLDatabase->setText(settings::astroManagerSettings->value(settings::ATID_MYSQL_DATABASENAME, "").toString());
      lineEditATIDMySQLUser->setText(settings::astroManagerSettings->value(settings::ATID_MYSQL_USERNAME, "").toString());
      lineEditATIDMySQLPassword->setText(settings::astroManagerSettings->value(settings::ATID_MYSQL_PASSWORD, "").toString());

        // PostgreSQL

      lineEditATIDPostgreSQLHost->setText(settings::astroManagerSettings->value(settings::ATID_POSTGRESQL_HOSTADDRESS, "localhost").toString());
      spinBoxATIDPostgreSQLPort->setValue(settings::astroManagerSettings->value(settings::ATID_POSTGRESQL_PORT, QVariant(5432)).toInt());
      lineEditATIDPostgreSQLDatabase->setText(settings::astroManagerSettings->value(settings::ATID_POSTGRESQL_DATABASENAME, "").toString());
      lineEditATIDPostgreSQLUser->setText(settings::astroManagerSettings->value(settings::ATID_POSTGRESQL_USERNAME, "").toString());
      lineEditATIDPostgreSQLPassword->setText(settings::astroManagerSettings->value(settings::ATID_POSTGRESQL_PASSWORD, "").toString());

        // Oracle

      lineEditATIDOracleHost->setText(settings::astroManagerSettings->value(settings::ATID_ORACLE_HOSTADDRESS, "localhost").toString());
      spinBoxATIDOraclePort->setValue(settings::astroManagerSettings->value(settings::ATID_ORACLE_PORT, QVariant(1521)).toInt());
      lineEditATIDOracleSchema->setText(settings::astroManagerSettings->value(settings::ATID_ORACLE_SCHEMANAME, "").toString());
      lineEditATIDOracleUser->setText(settings::astroManagerSettings->value(settings::ATID_ORACLE_USERNAME, "").toString());
      lineEditATIDOraclePassword->setText(settings::astroManagerSettings->value(settings::ATID_ORACLE_PASSWORD, "").toString());

      connect(comboBoxATIDDatabaseType, SIGNAL(currentIndexChanged(int)), this, SLOT(eventATIDDatabaseCombo(int)));

        // Select the current driver.

      if (groupBoxATID->isChecked())
      {
        std::string szDatabase = settings::astroManagerSettings->value(settings::ATID_DATABASE_DBMS, QVariant("MySQL")).toString().toStdString();

        if (szDatabase == "MySQL")
        {
          stackedWidgetATID->setCurrentIndex(QCL::CDatabase::SQLDB_MYSQL);
        }
        else if (szDatabase == "Oracle")
        {
          stackedWidgetATID->setCurrentIndex(QCL::CDatabase::SQLDB_QOCI);
        }
        else if (szDatabase == "SQLite")
        {
          stackedWidgetATID->setCurrentIndex(QCL::CDatabase::SQLDB_SQLITE);
        }
        else if (szDatabase == "PostgreSQL")
        {
          stackedWidgetATID->setCurrentIndex(QCL::CDatabase::SQLDB_PSQL);
        }
        else if (szDatabase == "ODBC")
        {
          stackedWidgetATID->setCurrentIndex(QCL::CDatabase::SQLDB_ODBC);
        }
        else
        {
          CODE_ERROR;
        }
      }
      else
      {
        stackedWidgetATID->setCurrentIndex(QCL::CDatabase::SQLDB_MYSQL);
      };

      if (settings::astroManagerSettings->value(settings::ATID_DATABASE_USESIMBAD, false).toBool())
      {
        groupBoxATID->setChecked(false);
        checkBoxUseSimbad->setChecked(true);
        pushButtonATIDTestConnection->setEnabled(false);
      }
      else
      {
        groupBoxATID->setChecked(true);
        checkBoxUseSimbad->setChecked(false);
        pushButtonATIDTestConnection->setEnabled(true);
      }

      connect(checkBoxUseSimbad, SIGNAL(clicked(bool)), this, SLOT(eventATIDUseSimbad(bool)));
      connect(groupBoxATID, SIGNAL(clicked(bool)), this, SLOT(eventATIDUseATID(bool)));

      connect(pushButtonATIDTestConnection, SIGNAL(clicked()), this, SLOT(eventATIDTestConnection()));
    }

    /// @brief Sets up the database information.
    /// @throws CRuntimeAssert
    /// @version 2018-09-24/GGB - Added support for the ATID database (Bug #161)
    /// @version 2017-07-01/GGB - Added support for the weather database.
    /// @version 2017-06-18/GGB - Use the CDialog findChild(). (Bug #67)
    /// @version 2015-06-11/GGB - Moved ARID setup to a seperate function.
    /// @version 2013-06-01/GGB - Function created.

    void CDialogOptions::setupDatabase()
    {
      setupARIDDatabase();
      groupBoxATID = findChild<QGroupBox *>("groupBoxATID");

      setupATIDDatabase();

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
    /// @version 2018-09-16/GGB - Added Data directory support
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
      ASSOCIATE_LINEEDIT(lineEditDataDirectory, dlg, "lineEditDataDirectory");
      lineEditDarkDirectory = findChild<QLineEdit *>("lineEditDarkDirectory");
      lineEditFlatDirectory = findChild<QLineEdit *>("lineEditFlatDirectory");
      lineEditBiasDirectory = findChild<QLineEdit *>("lineEditBiasDirectory");
      lineEditMasterDarkDirectory = findChild<QLineEdit *>("lineEditMasterDarkDirectory");
      lineEditMasterFlatDirectory = findChild<QLineEdit *>("lineEditMasterFlatDirectory");
      lineEditMasterBiasDirectory = findChild<QLineEdit *>("lineEditMasterBiasDirectory");
      checkBoxDarkDirectory = findChild<QCheckBox *>("checkBoxDarkDirectory");
      checkBoxFlatDirectory = findChild<QCheckBox *>("checkBoxFlatDirectory");
      checkBoxBiasDirectory = findChild<QCheckBox *>("checkBoxBiasDirectory");

      lineEditLogfileDirectory->setText(settings::astroManagerSettings->value(settings::DIRECTORY_LOGDIR, QVariant("./log")).toString());
      lineEditDataDirectory->setText(settings::astroManagerSettings->value(settings::DIRECTORY_DATA, QVariant("data")).toString());
      lineEditDarkDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_DARKFRAME_DIRECTORY, QVariant("")).toString());
      lineEditFlatDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_FLATFRAME_DIRECTORY, QVariant("")).toString());
      lineEditBiasDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_BIASFRAME_DIRECTORY, QVariant("")).toString());
      lineEditMasterDarkDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERDARK_DIRECTORY, QVariant("")).toString());
      lineEditMasterFlatDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERFLAT_DIRECTORY, QVariant("")).toString());
      lineEditMasterBiasDirectory->setText(settings::astroManagerSettings->value(settings::IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY, QVariant("")).toString());
      checkBoxDarkDirectory->setChecked(settings::astroManagerSettings->value(settings::DARKFRAME_DIRECTORY_AUTOUPDATE, QVariant(true)).toBool());
      checkBoxFlatDirectory->setChecked(settings::astroManagerSettings->value(settings::FLATFRAME_DIRECTORY_AUTOUPDATE, QVariant(true)).toBool());
      checkBoxBiasDirectory->setChecked(settings::astroManagerSettings->value(settings::BIASFRAME_DIRECTORY_AUTOUPDATE, QVariant(true)).toBool());

        // Data Files

      ASSOCIATE_LINEEDIT(lineEditTAIUTC, dlg, "lineEditTAIUTC");
      ASSOCIATE_LINEEDIT(lineEditUTCUT1, dlg, "lineEditUTCUT1");
      ASSOCIATE_LINEEDIT(lineEditMPCORB, dlg, "lineEditMPCORB");
      ASSOCIATE_LINEEDIT(lineEditCometEls, dlg, "lineEditCometEls");

      lineEditTAIUTC->setText(settings::astroManagerSettings->value(settings::FILE_TAIUTC, QVariant("TAI-UTC.csv")).toString());
      lineEditUTCUT1->setText(settings::astroManagerSettings->value(settings::FILE_UTCUT1, QVariant("finals2000A.data.csv")).toString());
      lineEditMPCORB->setText(settings::astroManagerSettings->value(settings::FILE_MPCORB, QVariant("MPCORB.DAT")).toString());
      lineEditCometEls->setText(settings::astroManagerSettings->value(settings::FILE_COMETELS, QVariant("CometEls.txt")).toString());

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

        // Directories

      connect(findChild<QPushButton *>("pushButtonLogfileDirectory"), SIGNAL(clicked(bool)), this, SLOT(eventLogFileDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonDataDirectory"), SIGNAL(clicked()), this, SLOT(eventDataDirectory()));
      connect(findChild<QPushButton *>("pushButtonDarkDirectory"), SIGNAL(clicked(bool)), this, SLOT(eventDarkDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonFlatDirectory"), SIGNAL(clicked(bool)), this, SLOT(eventFlatDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonBiasDirectory"), SIGNAL(clicked(bool)), this, SLOT(eventBiasDirectory(bool)));
      connect(findChild<QPushButton *>("pushButtonCSVDirectory"), SIGNAL(clicked(bool)), this, SLOT(eventCSVDirectory(bool)));

        // Data files

      connect(findChild<QPushButton *>("pushButtonTAIUTC"), SIGNAL(clicked()), this, SLOT(eventTAIUTC()));
      connect(findChild<QPushButton *>("pushButtonUTCUT1"), SIGNAL(clicked()), this, SLOT(eventUTCUT1()));
      connect(findChild<QPushButton *>("pushButtonMPCORB"), SIGNAL(clicked()), this, SLOT(eventMPCORB()));
      connect(findChild<QPushButton *>("pushButtonCometEls"), SIGNAL(clicked()), this, SLOT(eventCometEls()));
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

      std::for_each(QCL::CDatabase::databaseDrivers.begin(), QCL::CDatabase::databaseDrivers.end(),
                    [&](std::pair<const int, QCL::CDatabase::SDatabaseDriver> &dt)
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

        if ( (szDatabase == "MySQL") && (QCL::CDatabase::isDriverAvailable(QCL::CDatabase::SQLDB_MYSQL)) )
        {
          stackedWidgetWeather->setCurrentIndex(QCL::CDatabase::SQLDB_MYSQL);
          driverValid = true;
        }
        else if ( (szDatabase == "Oracle") && (QCL::CDatabase::isDriverAvailable(QCL::CDatabase::SQLDB_QOCI)) )
        {
          stackedWidgetWeather->setCurrentIndex(QCL::CDatabase::SQLDB_QOCI);
          driverValid = true;
        }
        else if ( (szDatabase == "SQLite") && (QCL::CDatabase::isDriverAvailable(QCL::CDatabase::SQLDB_SQLITE)) )
        {
          stackedWidgetWeather->setCurrentIndex(QCL::CDatabase::SQLDB_SQLITE);
          driverValid = true;
        }
        else if ( (szDatabase == "PostgreSQL") && (QCL::CDatabase::isDriverAvailable(QCL::CDatabase::SQLDB_PSQL)) )
        {
          stackedWidgetWeather->setCurrentIndex(QCL::CDatabase::SQLDB_PSQL);
          driverValid = true;
        }
        else if ( (szDatabase == "ODBC") && (QCL::CDatabase::isDriverAvailable(QCL::CDatabase::SQLDB_ODBC)) )
        {
          stackedWidgetWeather->setCurrentIndex(QCL::CDatabase::SQLDB_ODBC);
          driverValid = true;
        }
        else
        {
          CODE_ERROR;
        }

        if (!driverValid)
        {
            // The currently selected driver is not available for this system.
        }
      }
      else
      {
        stackedWidgetWeather->setCurrentIndex(QCL::CDatabase::SQLDB_MYSQL);
      }

      connect(pushButtonWeatherTestConnection, SIGNAL(clicked(bool)), this, SLOT(eventWeatherTestConnection(bool)));
    }

  }  // namespace dialogs
}  // namespace astroManager
