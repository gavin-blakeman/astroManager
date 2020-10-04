//*********************************************************************************************************************************
//
// PROJECT:             astroManager
// FILE:                Network management functions/interface.
// SUBSYSTEM:           Instanciation of network download manager.
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
// HISTORY:             2020-10-04 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef NETWORK_H
#define NETWORK_H

  // Miscellaneous library header files

#include <QCL>

namespace astroManager::network
{
  extern QCL::CFileDownloadManager fileDownloadManager;
}

#endif // NETWORK_H
