//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:			          SIMBAD
// SUBSYSTEM:           Support for the SIMBAD online astronomical database.
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// AUTHOR:              Gavin BLakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015 Gavin Blakeman.
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
// OVERVIEW:            Implements web access to the SIMBAD database for the purposes of querying object and field information from
//                      the SIMBAD database.
//
// CLASSES INCLUDED:    CSIMBAD
//
// HIERARCHY:           CSIMBAD
//
// HISTORY:             2015-09-22 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../Include/SIMBAD.h"

namespace AstroManager
{
  namespace SIMBAD
  {

    std::string coord = "http://simbad.u-strasbg.fr/simbad/sim-coo";


    /// @brief Constructor for the class.
    /// @throws None.
    /// @version 2015-09-22/GGB - Function created.

    CSIMBAD::CSIMBAD()
    {

    }

    /// @brief Destructor for the class.
    /// @throws None.
    /// @version 2015-09-22/GGB - Function created.

    CSIMBAD::~CSIMBAD()
    {
    }


    /// @brief Queries SIMBAD for information on an object.
    /// @param[in] objectID - The ID (or name) of the object to query.
    /// @returns true - Object found on SIMBAD
    /// @returns false - Object not found.
    /// @throws None.
    /// @version 2015-09-25/GGB - Function created.

    bool CSIMBAD::queryObject(std::string const &objectID)
    {
      QUrl url("http://simbad.u-strasbg.fr/simbad/sim-id");
      QUrlQuery urlQuery;

      urlQuery.addQueryItem("id", QString::fromStdString(objectID));

      url.setQuery(urlQuery);

      networkReply = networkAccessManager.get(QNetworkRequest(url));
      connect(networkReply, SIGNAL(finished()), this, SLOT(httpFinished()));
      connect(networkReply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
      connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDataReadProgress(qint64,qint64)));
    }

    void CSIMBAD::httpReadyRead()
    {

    }



  }   // namespace SIMBAD

}   // namespace AstroManager
