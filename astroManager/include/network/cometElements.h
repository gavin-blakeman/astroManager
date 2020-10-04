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

#ifndef COMETELEMENTS_H
#define COMETELEMENTS_H

  // Stndard C++ library header files

#include <cstdint>

  // Miscellaneous library header files

#include "boost/filesystem.hpp"
#include <QCL>

namespace astroManager::network
{
  class CCometElements final
  {
  public:
    static void downloadCometElements();
    static void error(std::uint32_t, QNetworkReply *);
    static void success(std::uint32_t, QNetworkReply *);
  };

} // namespace astroManager::network

#endif // COMETELEMENTS_H
