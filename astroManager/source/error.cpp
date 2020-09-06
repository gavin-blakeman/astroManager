//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Error.cpp
// SUBSYSTEM:						Error Class for ARPAA
// LANGUAGE:						C++
// TARGET OS:						WINDOWS, LINUX, UNIX, MAC
// LIBRARY DEPENDANCE:	Qt, GCL
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2020 Gavin Blakeman.
//                      This file is part of the Astronomy Manager software(astroManager)
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
// OVERVIEW:            Error class for defining runtime errors. This class is used as a base class for the class
//                      thrown during exceptions. The class also includes error reporting.
//
// CLASSES INCLUDED:    CError - Error class used for critical, recoverable and information messages.
//					            CCodeError  - Error class for reporting jumps into unreachable code.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-02-25 GGB - File created.
//
//*********************************************************************************************************************************

#include "include/error.h"

  // astroManager header files.

#include "include/astroManager.h"

  // Miscellaneous library header files

#include "boost/lexical_cast.hpp"
#include <QCL>

namespace astroManager
{

  /// @brief Adds the astroManager library error codes into the GCL error object.
  /// @throws None.
  /// @version 2016-06-20/GGB - Function created.

  void loadErrorMessages()
  {
    std::vector<std::pair<GCL::TErrorCode, std::string>> errors =
    {
      {0x0001, std::string("Resource Failed to Load.")},
      {0x0002, std::string("")},
      {0x0003, std::string("")},
      {0x0004, std::string("ARID DATABASE: Database Driver not recognised.")},
      {0x0005, std::string("Invalid image plane")},
      {0x0006, std::string("Unable to open file, or file does not exist.")},
      {0x0007, std::string("")},
      {0x0008, std::string("")},   //
      {0x0009, std::string("DATABASE: Weather Database settings not complete.")},
      {0x000A, std::string("ARID DATABASE: Database Driver not recognised.")},
      {0x000B, std::string("Array index out of array bounds.")},
      {0x000C, std::string("FITS keyword type unknown.")},
      {0x000D, std::string("Unable to open file type selected.")},
      {0x000E, std::string("Error while opening file.")},
      {0x000F, std::string("cfitsio: cfitsioError")},
      {0x0100, std::string("ARID DATABASE: Magnitude not found.")},
      {0x0101, std::string("KEYWORD: Unable to cast value... Out of range.")},
      {0x0200, std::string("FITS: TIMESYS - timescale unknown or undefined.")},
      {0x0201, std::string("")},
      {0x0701, "SIMBAD: Invalid field name."},
      {0x0702, "SIMBAD: Invalid region type."},
    };

    std::for_each(errors.begin(), errors.end(),
                  [] (std::pair<GCL::TErrorCode, std::string> p) { GCL::CError::addErrorMessage("astroManager", p.first, p.second); });
  }

} // namespace AstroManager
