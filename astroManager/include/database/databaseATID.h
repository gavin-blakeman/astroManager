//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:			          DatabaseATID
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
// CLASSES INCLUDED:    CATID
//
// HIERARCHY:           CDatabase
//                        - CATID
//                        - CARID
//                        - CDatabaseWeather
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-24 GGB - Split CATID into it's own file
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-01-04 GGB - Classes developed for astroManager
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_DATABASEATID_H
#define ASTROMANAGER_DATABASEATID_H

  // astroManager application header files.

#include "../astroManager.h"

  // Miscellaneous library header files.

#include <ACL>
#include <QCL>

namespace astroManager
{
  namespace database
  {
    const int ROLE_FILTERID  = Qt::UserRole + 0;

    // Class for the Astronomical Target Database

    class CATID final : public QCL::CDatabase
    {
      Q_OBJECT

    private:
      bool useSIMBAD;                         ///< If true, the SIMBAD lookups are used, not the ATID database.
      virtual bool ODBC();
      virtual bool Oracle();
      virtual bool MySQL();
      virtual bool SQLite();
      virtual bool PostgreSQL() {return false;}

      bool queryByCoordinatesSIMBAD(ACL::CAstronomicalCoordinates const &, double, ACL::DTargetAstronomy &);
      bool queryByCoordinatesATID(ACL::CAstronomicalCoordinates const &, double, ACL::DTargetAstronomy &);
      bool queryByCoordinatesSIMBAD(ACL::CAstronomicalCoordinates const &, ACL::CAstronomicalCoordinates const &, ACL::DTargetAstronomy &);
      bool queryByCoordinatesATID(ACL::CAstronomicalCoordinates const &, ACL::CAstronomicalCoordinates const &, ACL::DTargetAstronomy &);

      bool queryStellarObjectByName_ATID(std::string const &, ACL::CTargetStellar *);
      bool queryStellarObjectByName_SIMBAD(std::string const &, ACL::CTargetStellar *);

      void readStellarObjectInformation_ATID(objectID_t, ACL::CTargetStellar *);
      void readStellarObjectInformation_SIMBAD(objectID_t, ACL::CTargetStellar *);

        // Deleted functions

      CATID(CATID const &) = delete;

    protected:
      bool parseSIMBADReply(std::string const &, ACL::DTargetAstronomy &);
      bool parseSIMBADObjectQuery(std::string const &, ACL::CTargetStellar &);

    public:
      enum EForce
      {
        FORCE_NONE,                 ///< Don't force the use of either ATID or SIMBAD. Rely on the settings.
        FORCE_ATID,                 ///< Ignore the settings and query ATID
        FORCE_SIMBAD,               ///< Ignore the settings and query SIMBAD
      };

      CATID();
      virtual ~CATID();

      void connectToDatabase();
      virtual void readMapFile(boost::filesystem::path const &);

      bool usingSIMBAD() const { return useSIMBAD; }

      long long GetObservationCount(const long long) const;
      void PopulateFiltersList(QListWidget *);
      void PopulateFiltersList(QListWidget *, QIcon &);
      void PopulateSpectralClassCombo(QComboBox *);
      void PopulateObjectTypeCombo(QComboBox *);
      void PopulatePhotometryStandardCombo(QComboBox *, QVariant *, QVariant * = nullptr);
      void PopulateConstellationsCombo(QComboBox *);
      void PopulateCatalogCombo(QComboBox *);
      void PopulatePhotometryProgramCombo(QComboBox *);
      bool populateStellarObject(QVariant const &, std::shared_ptr<ACL::CTargetStellar>);
      bool queryStellarObjectByName(std::string const &, ACL::CTargetStellar *, EForce = FORCE_NONE);

      void GetLatestMagnitude(objectID_t, const long long, float &, float &) const;

      bool queryByCoordinates(ACL::CAstronomicalCoordinates const &RADEC, double radius, ACL::DTargetAstronomy &);
      bool queryByCoordinates(ACL::CAstronomicalCoordinates const &, ACL::CAstronomicalCoordinates const &, ACL::DTargetAstronomy &);

      bool queryNamesFromATID(objectID_t, std::vector<std::string> &);

        // Functions handling stellarObject information

      void readStellarObjectInformation(objectID_t, ACL::CTargetStellar *);

    };

    extern CATID *databaseATID;

  }   // namespace database
}    // namespace AstroManager

#endif // ASTROMANAGER_DATABASEATID_H
