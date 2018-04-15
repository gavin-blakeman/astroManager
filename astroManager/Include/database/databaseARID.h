﻿//*********************************************************************************************************************************
//
// PROJECT:							astroManager
// FILE:			          DatabaseARID
// SUBSYSTEM:           Data support functions
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// AUTHOR:              Gavin BLakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2012-2018 Gavin Blakeman.
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
// OVERVIEW:            Data support functions.
//
// CLASSES INCLUDED:    CARID
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                       - CdatabaseWeather
//
// HISTORY:             2018-02-03 GGB - Changed application name to AstroManager.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-03-01 GGB - Split CARID into it's own file.
//                      2013-02-24 GGB - Split CATID into it's own file
//                      2013-01-20 GGB - Release of AIRDAS 0000.00
//                      2012-01-04 GGB - Classes developed for AIRDAS
//
//*********************************************************************************************************************************

#ifndef AIRDAS_DATABASEARID_H
#define AIRDAS_DATABASEARID_H

#include "database.h"
#include "../ACL/astroFile.h"
#include "../ACL/observatoryInformation.h"
#include "../ACL/telescope.h"
#include "../astroManager.h"

  // Standard C++ libraries

#include <memory>

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogImageDetails;
  }

  namespace database
  {
    class CARID : public CDatabase
    {
    private:
      bool ARIDdisabled_;
      std::unique_ptr<QSqlQuery> sqlQuery;       ///< Pointer to the query that will be used

      virtual bool ODBC();
      virtual bool Oracle();
      virtual bool MySQL();
      virtual bool SQLite();
      virtual bool PostgreSQL() {}

    protected:
      void loadPhotometryFilterData();

      void processErrorInformation() const;
     
    public:
      CARID();
      virtual ~CARID();

      void connectToDatabase();

      void loadDefaultData();

        // Observing site functions.

      bool getObservingSite(std::uint32_t, CObservatory *);
      bool findObservingSite(CObservatory *);
      bool registerObservingSite(CObservatory *);

        // Telescope functions

      bool findTelescope(CTelescope *);
      bool registerTelescope(CTelescope *);

        // Image functions

      bool isImageNameRegistered(std::string const &);
      bool isImageNameRegistered(std::string const &, QString &);
      bool isImageNameRegistered(std::string const &, imageID_t &);
      bool isImageUUIDRegistered(QUuid const &, imageID_t &);
      bool getImageName(imageID_t, std::string &);
      bool updateImageComments(imageID_t, QString const &);
      bool updateImageQuality(imageID_t, std::uint8_t);
      bool updateImageFlags(imageID_t, bool, bool);
      bool registerImage(CAstroFile *);
      void saveOriginalImage(CAstroFile *);
      bool registerUploadImage(CAstroFile *, boost::filesystem::path const &);

      bool downLoadImage(imageID_t, imageVersion_t, QByteArray &);
      void uploadImage(QByteArray const &, imageID_t, imageVersion_t, QString const &);
      void uploadImage(QString const &, imageID_t, imageVersion_t, QString const &);
      imageVersion_t versionCount(imageID_t);
      bool versionLatest(imageID_t, imageVersion_t &);

        // Widget functions

      void PopulateFiltersAvailableCombo(QComboBox *);
      void PopulateInstrumentCombo(QComboBox *, bool);

      void populateComboSite(QComboBox *, bool);
      void PopulateObserverCombo(QComboBox *, bool);
      void populateStandardFiltersCombo(QComboBox *);
      void PopulateTelescopeCombo(QComboBox *, bool);
      void populateComboObservingPlans(QComboBox *, bool);

      void populateFiltersListWidget(QListWidget *);
      void populateListInstrumentNames(QListWidget *, bool);
      void populateListSiteNames(QListWidget *, bool);
      void populateListTelescopeNames(QListWidget *, bool);

        // Dialog functions

      void populateDialogImageDetails(dialogs::CDialogImageDetails *);
    };

    extern CARID *databaseARID;


  }  // namespace database
}  // namespace AstroManager

#endif // AIRDAS_DATABASEARID_H
