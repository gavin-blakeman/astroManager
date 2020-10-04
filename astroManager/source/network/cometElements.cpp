//*********************************************************************************************************************************
//
// PROJECT:             astroManager
// FILE:                cometElements
// SUBSYSTEM:           Download of comet elements file
// LANGUAGE:            C++
// TARGET OS:           WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:  Qt
// NAMESPACE:           astroManager::network
// AUTHOR:              Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2020 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:
//
// HISTORY:             2020-10-03 GGB - File Created.
//
//*********************************************************************************************************************************

#include "include/network/cometElements.h"

  // Standard C++ library header files

#include <cstdint>
#include <string>

  // Miscellaneous library header files

#include "boost/filesystem.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/locale.hpp"
#include <GCL>
#include <QCL>

  // astroManager header files

#include "include/network/network.h"
#include "include/settings.h"

namespace astroManager::network
{
  QUrl const COMETELS_URL("https://www.minorplanetcenter.net/iau/MPCORB/CometEls.txt");

  void CCometElements::downloadCometElements()
  {
    INFOMESSAGE(boost::locale::translate("Checking if CometEls.txt needs to be downloaded..."));

      // Read the data from the settings file

    boost::gregorian::date_duration updateFrequency(settings::astroManagerSettings->value(settings::FILE_COMETELS_UPDATE,
                                                                                          QVariant(7)).toInt());
    std::string lastUpdate = settings::astroManagerSettings->value(settings::FILE_COMETELS_LASTUPDATE,
                                                                   QVariant("2020-01-01")).toString().toStdString();
    boost::filesystem::path filename(settings::astroManagerSettings->value(settings::FILE_COMETELS_LOCATION,
                                                                           QVariant("")).toString().toStdString());

      // Convert to dates and check if an update is required.

    boost::gregorian::date lastDate(boost::gregorian::from_simple_string(lastUpdate));
    lastDate += updateFrequency;

    boost::gregorian::date today(boost::gregorian::day_clock::local_day());

    if (today >= lastDate)
    {
      // Need to update.

      INFOMESSAGE(boost::locale::translate("Updating CometEls.txt, attempting download from Minor Planet Center..."));

      fileDownloadManager.startDownload(COMETELS_URL, filename, &CCometElements::success, &CCometElements::error);

    }
    else
    {
      INFOMESSAGE(boost::locale::translate("MPCORB.DAT does not need to be downloaded."));
    };

  }

  /// @brief      Function called if the download failed.
  /// @param[in]  : not used
  /// @param[in]  networkReply: The network reply containing the error.
  /// @throws
  /// @version    2020-10-04/GGB - Function created.

  void CCometElements::error(std::uint32_t, QNetworkReply *networkReply)
  {
    ERRORMESSAGE(boost::locale::translate("CometEls.txt download was not successfull!"));
    ERRORMESSAGE(networkReply->errorString().toStdString());
  }

  /// @brief       Function called if the download succeeded.
  /// @param[in]  : not used
  /// @param[in]  : not used
  /// @throws
  /// @version    2020-10-04/GGB - Function created.

  void CCometElements::success(std::uint32_t, QNetworkReply *)
  {
    boost::gregorian::date today(boost::gregorian::day_clock::local_day());

    std::string lastUpdate(boost::gregorian::to_iso_extended_string(today));

    settings::astroManagerSettings->setValue(settings::FILE_COMETELS_LASTUPDATE, QString::fromStdString(lastUpdate));

    INFOMESSAGE(boost::locale::translate("CometEls.txt download succeeded."));
  }

} // namespace astroManager::network
