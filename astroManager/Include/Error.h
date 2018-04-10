﻿//*************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Error.h
// SUBSYSTEM:						Error Class for ARPAA
// LANGUAGE:						C++
// TARGET OS:						WINDOWS, LINUX, UNIX, MAC
// LIBRARY DEPENDANCE:	Qt, GCL
// NAMESPACE:						AIRDAS
// AUTHOR:							Gavin Blakeman. (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2016 Gavin Blakeman.
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
// OVERVIEW:            Error class for defining runtime errors. This class is used as a base class for the class thrown during
//                      exceptions. The class also includes error reporting.
//
// CLASSES INCLUDED:    CError - Error class used for critical, recoverable and information messages.
//					            CCodeError  - Error class for reporting jumps into unreachable code.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-02-25 GGB - File created.
//
//*************************************************************************************************

#ifndef ERROR_H
#define ERROR_H

#include <map>
#include <string>

#include <stdexcept>

#include <GCL>

namespace AstroManager
{

  void loadErrorMessages();

#define AIRDAS_ERROR(ERR) (ERROR(AIRDAS, ERR))
#define AIRDAS_CODE_ERROR CODE_ERROR(AIRDAS)

} // namespace AstroManager

#endif // ERROR_H
