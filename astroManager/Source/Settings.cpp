//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Manager)
// FILE:								Settings
// SUBSYSTEM:						Registry Class
// LANGUAGE:						C++
// TARGET OS:						UNIX/LINUX/WINDOWS/MAC
// LIBRARY DEPENDANCE:	ACL, Qt
//                      Qt
// NAMESPACE:						AstroManager::settings
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2009-2017 Gavin Blakeman.
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
// OVERVIEW:            Implements the class for the Registry Access
//
// HISTORY:		          2018-02-03 GGB - Rename Project to AstroManager
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-12-18 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#include "../Include/Settings.h"

  // AIRDAS include files.

#include "../Include/VSOP.h"

  // Miscellaneous Libraries

#include <ACL>
#include <GCL>
#include <MCL>
#include <PCL>
#include <SCL>

  // Standard C++ libraries.

#include <thread>

namespace AstroManager
{
  namespace settings
  {
    QSettings *VSOPSettings = new QSettings(ORG_NAME, APPL_NAME);

    /// @brief Function to create default settings for ARID if there are no settings defined when the application starts.
    /// @throws None.
    /// @version 2013-04-26/GGB - Function created.

    void createDefaultARIDSettings()
    {
      VSOPSettings->setValue(ARID_DATABASE_DBMS, SQLITE);
      VSOPSettings->setValue(ARID_SQLITE_DRIVERNAME, "QSQLITE");
      VSOPSettings->setValue(ARID_SQLITE_DATABASENAME, "Data/ARID.sqlite");
    }

    /// @brief Copys the recent photometry values to one older and adds the new value at position 1.
    /// @param[in] mostRecent - The most recent entry to add.
    /// @throws None.
    /// @version 2010-11-27/GGB - Function created.

    void InsertRecentObject(const QString &mostRecent)
    {
      int nIndex;
      QString szKey1, szKey2;

      // Copy the old values down

      for (nIndex = 10; nIndex > 1; nIndex--)
      {
        szKey1 = QString(DIALOG_SELECTOBJECT_RECENT).arg(nIndex);
        szKey2 = QString(DIALOG_SELECTOBJECT_RECENT).arg(nIndex-1);

        settings::VSOPSettings->setValue(szKey1, settings::VSOPSettings->value(szKey2, QVariant("")));
      };

      szKey1 = QString(DIALOG_SELECTOBJECT_RECENT).arg(1);

      settings::VSOPSettings->setValue(szKey1, QVariant(mostRecent));
    }

    /// @brief Loads any settings that need to be initialised on startup
    /// @note Any additional settings that need to be initialised on startup can go in this routine.
    /// @throws None.
    /// @version 2017-06-25/GGB - Updatd thread handling
    ///   @li Added automatic selection of the number of threads. (Bug #72)
    ///   @li Added call to function setThreads()
    /// @version 2015-08-08/GGB - Added SCL support (Bug 1470503)
    /// @version 2012-11-29/GGB - Added support for multi-threading in MCL
    /// @version 2011-12-18/GGB - Function created.

    void InitialiseStartupSettings()
    {
      QVariant vMaxThreads = settings::VSOPSettings->value(MAX_THREADS, QVariant(std::thread::hardware_concurrency()));

      setThreads(vMaxThreads.toUInt());
    }

  }  // namespace settings
}	// namespace AstroManager
