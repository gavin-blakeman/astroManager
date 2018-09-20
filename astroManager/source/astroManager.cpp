//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Manager)
// FILE:								astroManager
// SUBSYSTEM:						Application and Window Creation.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Boost, GCL, Qt
// NAMESPACE:						AstroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2009-2018 Gavin Blakeman.
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
// OVERVIEW:	          Main() function and Qt API entry point implemented in this file.
//
// HISTORY:		          2018-02-03 GGB - Rename Project to astroManager
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//						          2010-05-05 GGB - Change application to ARPAA and use Qt API.
//                      2009-08-22 GGB - File created.
//
//*********************************************************************************************************************************

  // Standard C++ library header files

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <sstream>

  // astroManager application header files

#include "../include/qtExtensions/application.h"
#include "../include/database/databaseARID.h"
#include "../include/database/databaseATID.h"
#include "../include/database/databaseWeather.h"
#include "../include/error.h"
#include "../include/FrameWindow.h"
#include "../include/settings.h"

    // Miscellaneous include files

#include <ACL>
#include "boost/lexical_cast.hpp"
#include <MCL>
#include <SCL>
#include <GCL>
#include <QCL>
#include <QxtGui/QxtConfirmationMessage>

  // These do not need to be defined in C++

#ifdef max
#undef max
#endif

/// @brief Main Windows Procedure
/// @details Create the application window, displays the application window and manages the message loop.
/// @version 2016-05-07/GGB
///   @li Added support for log severity "critical"
///   @li Added logger shutdown calls before exiting.
/// @version 2017-06-20/GGB - Updated error handling to use new GCL classes.
/// @version 2015-09-12/GGB - (Bug 74) - Added check for logFile directory. If not existing will use the application directory.
/// @version 2015-07-01/GGB - (Bug 38) SCL Error message load removed following update to SCL error class to autoload error messages.
/// @version 2015-06-30/GGB - ACL Error message load removed following update to ACL error class to autoload error messages.
/// @version 2014-12-23/GGB - Changed logging to use the GCL::logger classes. Removed GPL notice from initial startup logging.
/// @version 2013-09-22/GGB - Removed call to tzset() before time(...)
/// @version 2013-07-27/GGB - Added support for displaying the license and disclaimer after the splash screen.
/// @version 2013-04-26/GGB - Added support for MCL errors and exception catching for SCL and GCL errors.
/// @version 2013-03-22/GGB - Added exception catching for libAstroclass and MCL.
/// @version 2013-02-24/GGB - Added code for Xerces support for XML decoding.
/// @version 2013-01-26/GGB - Added suppport for GCL errors.
/// @version 2013-01-20/GGB - Added logfile support.
/// @version 2011-12-18/GGB - Added code to load the settings on start-up.
/// @version 2011-02-24/GGB - Added code to catch the exception from the CATID::CATID() constructor and terminate the application
///                           with an error.
/// @version 2010-12-09/GGB - Moved database initialisation code into CATID class. Only create the class here.
/// @version 2010-05-12/GGB - Added code to initialise database into this procedure. Database uses a global variable.
/// @version 2010-05-05/GGB - Modified to use Qt library rather than native windows library.
/// @version 2009-10-14/GGB - Created CApplication and CVSOPApplication classes
/// @version 2009-08-22/GGB - Function created.

int main(int argc, char *argv[])
{
  int returnValue;
  std::string ifn;

    // Set the log level

  GCL::logger::CSeverity logSeverity;
  int logLevel = astroManager::settings::astroManagerSettings->value(astroManager::settings::SETTINGS_LOGLEVEL, QVariant(0)).toInt();
  if (logLevel != 0)
  {
    logSeverity.fCritical = logLevel & astroManager::settings::LL_CRITICAL;
    logSeverity.fError = logLevel & astroManager::settings::LL_ERROR;
    logSeverity.fWarning = logLevel & astroManager::settings::LL_WARNING;
    logSeverity.fNotice = logLevel & astroManager::settings::LL_NOTICE;
    logSeverity.fInfo = logLevel & astroManager::settings::LL_INFO;
    logSeverity.fDebug = logLevel & astroManager::settings::LL_DEBUG;
    logSeverity.fTrace = logLevel & astroManager::settings::LL_TRACE;
  }
  else
  {
    logSeverity.fCritical = true;
    logSeverity.fError = true;
    logSeverity.fWarning = true;
    logSeverity.fNotice = true;
    logSeverity.fInfo = true;
    logSeverity.fDebug = true;
    logSeverity.fTrace = true;
  }

  std::string logFilePath = astroManager::settings::astroManagerSettings->value(astroManager::settings::DIRECTORY_LOGDIR,
                                                                                QVariant(QString(""))).
      toString().toStdString();

    // If the logfile directory does not exist, default to the current directory.

  if (!boost::filesystem::is_directory(logFilePath))
  {
    logFilePath = "";
  };

  GCL::logger::PLoggerSink fileLogger(new GCL::logger::CFileSink(logFilePath, "astroManager"));
  std::dynamic_pointer_cast<GCL::logger::CFileSink>(fileLogger)->setRotationPolicyUse(10);
  fileLogger->setLogLevel(logSeverity);

  try
  {
    std::dynamic_pointer_cast<GCL::logger::CFileSink>(fileLogger)->openLogFile();
  }
  catch(...)
  {
    std::clog << "Error while creating logfile. Does the directory exist?" << std::endl;
  }

  GCL::logger::defaultLogger().addSink(fileLogger);

  logSeverity.fTrace = false;
  GCL::logger::PLoggerSink coutLogger(new GCL::logger::CStreamSink(std::cout));
  coutLogger->setLogLevel(logSeverity);
  GCL::logger::defaultLogger().addSink(coutLogger);

  astroManager::CApplication app(argc, argv);

  GCL::logger::defaultLogger().logMessage(GCL::logger::notice, "Application Started.");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Release Number: " + astroManager::getReleaseString() + ". Release Date: " + astroManager::getReleaseDate() + ".");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "int size: " + boost::lexical_cast<std::string>(sizeof(int)) + " bytes. Maximum Value: " + boost::lexical_cast<std::string>(std::numeric_limits<int>::max()) + ".");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "long size: " + boost::lexical_cast<std::string>(sizeof(long)) + " bytes. Maximum Value: " + boost::lexical_cast<std::string>(std::numeric_limits<long>::max()) + ".");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "float size: " + boost::lexical_cast<std::string>(sizeof(float)) + " bytes.");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "double size: " + boost::lexical_cast<std::string>(sizeof(double)) + " bytes.");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "AXIS_t size: " + boost::lexical_cast<std::string>(sizeof(ACL::AXIS_t)) + " bytes.");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "INDEX_t size: " + boost::lexical_cast<std::string>(sizeof(ACL::INDEX_t)) + " bytes.");
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "FP_t size: " + boost::lexical_cast<std::string>(sizeof(ACL::FP_t)) + " bytes.");

  QPixmap pixmap(":/images/splash.jpg");

  QSplashScreen splash(pixmap);
  splash.show();
  splash.showMessage(QString("Starting Application"), Qt::AlignTop | Qt::AlignHCenter, Qt::white);

  try
  {
    app.setOrganizationName(astroManager::settings::ORG_NAME);
    app.setApplicationName(astroManager::settings::APPL_NAME);

    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Loading error messages...");
    splash.showMessage(QString("Loading error messages..."), Qt::AlignTop | Qt::AlignHCenter, Qt::white);
    astroManager::loadErrorMessages();

    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Initialising settings...");
    splash.showMessage(QString("Initialising settings..."), Qt::AlignTop | Qt::AlignHCenter, Qt::white);
    astroManager::settings::InitialiseStartupSettings();

      // Identify the available database drivers and initialise the list of drivers.

    INFOMESSAGE("Available database drivers: " + QSqlDatabase::drivers().join(", ").toStdString());
    QCL::CDatabase::initialiseDrivers();

    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Creating database Connections...");
    splash.showMessage("Creating database objects...", Qt::AlignTop | Qt::AlignHCenter, Qt::white);

    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Creating ATID Database Connection...");
    astroManager::database::databaseATID = new astroManager::database::CATID();		// Create the database connection
    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Loading ATID SQL mapping file...");
    astroManager::database::databaseATID->readMapFile("SQLMap/ATID.map");
    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Connecting to ATID database...");
    astroManager::database::databaseATID->connectToDatabase();

    if (!astroManager::settings::astroManagerSettings->value(astroManager::settings::ARID_DATABASE_DISABLE, QVariant(true)).toBool())
    {
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Creating ARID Database Connection...");
      astroManager::database::databaseARID = new astroManager::database::CARID();   // Create the database connection.
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Loading ARID SQL mapping file...");
      astroManager::database::databaseATID->readMapFile("SQLMap/ARID.map");
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Connecting to ARID database...");
      astroManager::database::databaseARID->connectToDatabase();
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Loading default ARID data...");
      astroManager::database::databaseARID->loadDefaultData();
    }
    else
    {
      astroManager::database::databaseARID = new astroManager::database::CARID();   // Create the database connection.
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "ARID Database is disabled.");
    }

    if (!astroManager::settings::astroManagerSettings->value(astroManager::settings::WEATHER_DATABASE_DISABLE, QVariant(true)).toBool())
    {
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Creating Weather Database Connection...");
      astroManager::database::databaseWeather = new astroManager::database::CDatabaseWeather();  // Create the database connection.
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Loading WEATHER SQL mapping file...");
      astroManager::database::databaseATID->readMapFile("SQLMap/WEATHER.map");
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Connecting to WEATHER database...");
      splash.showMessage(QString("Connecting to WEATHER database"), Qt::AlignTop | Qt::AlignHCenter, Qt::white);
      astroManager::database::databaseWeather->connectToDatabase();
    }
    else
    {
      astroManager::database::databaseWeather = new astroManager::database::CDatabaseWeather();  // Create the database connection.
      GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Weather Database is disabled.");
    }

    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Loading data...");
    splash.showMessage(QString("Loading data..."), Qt::AlignTop | Qt::AlignHCenter, Qt::white);
    ifn = astroManager::settings::astroManagerSettings->value(astroManager::settings::FILE_TAIUTC, QVariant("data/TAI-UTC.csv")).toString().toStdString();
    ACL::CAstroTime::load_dAT(ifn);
    ifn = astroManager::settings::astroManagerSettings->value(astroManager::settings::FILE_TAIUTC, QVariant("data/finals2000A.data.csv")).toString().toStdString();
    ACL::CAstroTime::load_dUT1(ifn);

    GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Creating main window...");
    splash.showMessage(QString("Creating main window"), Qt::AlignTop | Qt::AlignHCenter, Qt::white);

    astroManager::mdiframe::CFrameWindow vsopWin;

    vsopWin.show();

    splash.finish(&vsopWin);

    {
      QxtConfirmationMessage msgBox;
      QFile file(":/text/disclaimer.txt");

      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setText("License and Disclaimer.");

      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        CODE_ERROR(astroManager);
      };

      QTextStream data(&file);
      QString disclaimer = data.readAll();

      QIcon messageIcon(":/images/user_judge.png");

      msgBox.setIconPixmap(messageIcon.pixmap(32, 32));
      msgBox.setInformativeText(disclaimer);
      msgBox.setConfirmationText("Do not show again.");
      msgBox.addButton("Accept", QMessageBox::AcceptRole);
      msgBox.setDefaultButton(msgBox.addButton("Reject", QMessageBox::RejectRole));
      msgBox.setOverrideSettingsKey(astroManager::settings::CM_DISCLAIMER);

      if (msgBox.exec() == QMessageBox::AcceptRole)
      {
        GCL::logger::defaultLogger().logMessage(GCL::logger::notice, "License and Disclaimer accepted.");
        returnValue = app.exec();
      }
      else
      {
        GCL::logger::defaultLogger().logMessage(GCL::logger::notice, "License and Disclaimer not accepted.");
      };
    };

    //xercesc::XMLPlatformUtils::Terminate();

    GCL::logger::defaultLogger().logMessage(GCL::logger::notice,
                                         "Application Terminated. Return Value: " + std::to_string(returnValue));

    GCL::logger::defaultLogger().shutDown();    // Attempt to shutdown the logger.
    return returnValue;
  }
  catch(GCL::CCodeError &codeError)
  {
    codeError.logErrorMessage();

    QMessageBox msgBox;

    msgBox.setText(QString::fromStdString("Code Error in " + codeError.library() + " Library"));
    msgBox.setStandardButtons(QMessageBox::Abort);
    msgBox.setDefaultButton(QMessageBox::Abort);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setInformativeText(QString::fromStdString(codeError.errorMessage()));
    msgBox.exec();

    GCL::logger::defaultLogger().logMessage(GCL::logger::notice,
                                         "Application Terminated. Return Value: -1");
    GCL::logger::defaultLogger().shutDown();    // Attempt to shutdown the logger.
    return -1;
  }
  catch (GCL::CError &error)
  {
    error.logErrorMessage();
    GCL::logger::defaultLogger().logMessage(GCL::logger::notice,
                                         "Application Terminated. Return Value: " +
                                         boost::lexical_cast<std::string>(error.errorCode()));
    GCL::logger::defaultLogger().shutDown();    // Attempt to shutdown the logger.
    return ( error.errorCode() );
  }
  catch (GCL::CRuntimeAssert &error)
  {
    GCL::logger::defaultLogger().logMessage(GCL::logger::error,
                                            "Application terminated for a runtime assert in library" + error.library());
    GCL::logger::defaultLogger().shutDown();
    GCL::logger::defaultLogger().shutDown();    // Attempt to shutdown the logger.
    return (-1);
  }
  catch(...)
  {
    GCL::logger::defaultLogger().logMessage(GCL::logger::error, "Unhandled exception, unknown error.");
    GCL::logger::defaultLogger().logMessage(GCL::logger::notice, "Application Terminated. Return Value: -1.");
    GCL::logger::defaultLogger().shutDown();    // Attempt to shutdown the logger.
    return ( -1 );
  };
}

namespace astroManager
{
  // Software version information

  int const MAJORVERSION	= 2015;       // Major version (year)
  int const MINORVERSION	= 9;          // Minor version (month)
  std::uint16_t const BUILDNUMBER = 0x0326;
  std::string const BUILDDATE(__DATE__);

  std::vector<std::pair<int, std::string>> SEAlgorithms = { {1, std::string("Find Stars") },
                                                            {2, std::string("SimpleXY") } };

  // Returns the application description string.

  QString getDescription()
  {
    QFile file(":/text/description.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      CODE_ERROR(astroManager);
    }
    else
    {
      QTextStream data(&file);
      QString descriptionString = data.readAll();
      return descriptionString;
    };
  }

  /// @brief Returns the copyright string.
  /// @returns The copyright string as a std::string.
  /// @version 2013-06-01/GGB - Changed return type to QString
  /// @version 2013-05-11/GGB - Modified to use text file in the resources.
  /// @version 2012-01-01/GGB - Function created.

  QString getCopyrightString()
  {
    QFile file(":/text/copyright_licence.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      CODE_ERROR(astroManager);
    }
    else
    {
      QTextStream data(&file);
      QString copyrightString = data.readAll();
      return copyrightString;
    };
  }

  /// @brief Function to return the release string.
  /// @returns The release string as a std::string
  /// @throws None.
  /// @version 2012-01-01/GGB - Function created.
  /// @todo Change to use boost::format

  std::string getReleaseString()
  {
    char szString[50];

    sprintf(szString, "%04i.%02i.%04X", MAJORVERSION, MINORVERSION, BUILDNUMBER);

    return std::string(szString);
  }

  /// @brief Function to return the build date.
  /// @returns The build date as a string.
  /// @throws None.
  /// @version 2013-01-20/GGB - Function created.

  std::string getReleaseDate()
  {
    return BUILDDATE;
  }

  /// @brief Returns the name of the application that created an HDU
  /// @returns String of the application that created the HDU
  /// @throws None.
  /// @version 2018-04-13/GGB - Updated project name to astroManager.
  /// @version 2013-04-12/GGB - Function created.

  std::string const CREATOR()
  {
    return std::string("astroManager-") + getReleaseString();
  }

  /// @brief Returns the current date in the format YYYY-MM-DD.
  /// @returns current date as a string in YYYY-MM-DD format.
  /// @throws None.
  /// @version 2013-04-12/GGB - Function created.

  std::string const getDate()
  {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);

    return std::string(buffer);
  }

  /// @brief Sets the number of threads.
  /// @details There are a number of libraries that track the number of threads to use. This function will set the number
  ///          of threads to use for each of the libraries.
  /// @param[in] numThreads - The number of threads to use
  /// @throws None.
  /// @version 2017-06-25/GGB - Function created.

  void setThreads(size_t numThreads)
  {
    ACL::maxThreads = numThreads;
    //GCL::maxThreads = numThreads;
    MCL::maxThreads = numThreads;
    //PCL::maxThreads = numThreads;
    SCL::maxThreads = numThreads;
  }

}    // namespace astroManager
