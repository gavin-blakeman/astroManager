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

#ifndef AIRDAS_SIMBAD_H
#define AIRDAS_SIMBAD_H

#include <string>
#include <vector>

#include "qtExtensions/qt.h"

namespace AstroManager
{
  namespace SIMBAD
  {

    class CSIMBAD : QObject
    {
      Q_OBJECT

    private:
      QNetworkAccessManager networkAccessManager;
      QNetworkReply *networkReply;

      CSIMBAD(CSIMBAD const &) = delete;

    protected:
    public:
      CSIMBAD();
      virtual ~CSIMBAD();

      virtual bool queryObject(std::string const &);

    private slots:
      void httpReadyRead();
    };

  }   // namespace SIMBAD

}   // namespace AstroManager

#endif // AIRDAS_SIMBAD_H
