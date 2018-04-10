//*********************************************************************************************************************************
//
// PROJECT:             AstroManager (Astronomy Observation Manager)
// FILE:			          VSOP
// SUBSYSTEM:           Definitions for the AIRDAS application.
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2009-2018 Gavin Blakeman.
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
// OVERVIEW:            Defines macros that are used in conjunction with error reporting as well as
//                      for user information reporting.
//
// CLASSES INCLUDED:    NONE.
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//
//*********************************************************************************************************************************

#ifndef VSOP_H
#define VSOP_H

#include "qtExtensions/qt.h"

#include <ACL>

#include <string>
#include <utility>
#include <vector>

namespace AstroManager
{
    // Application constants

  int const ROLE_SITEID                     (Qt::UserRole + 0);

  std::string const AIRDAS_EDITEDBY         ("Edited by: Astronomical Image Reduction and Data Analysis Software. (AIRDAS)");

    // Functions below are found in VSOP.cpp

  std::string getReleaseString();
  QString getCopyrightString();
  std::string getReleaseDate();
  QString getDescription();
  void setThreads(size_t);

    // String constants used by the application

  std::string const CREATOR();
  std::string const getDate();

  QChar const UTF16_DEGREESIGN                (0x00b0);
  QChar const UTF16_MULTIPLYSIGN              (0x00d7);
  QChar const UTF16_PLUSMINUSSIGN             (0x00b1);

  QString const QS_ASTROMETRY                 (QObject::tr("Astrometry"));
  QString const QS_PHOTOMETRY                 (QObject::tr("Photometry"));

  QVariant const QV_ASTROMETRY                (1);
  QVariant const QV_PHOTOMETRY                (2);

  typedef ACL::FP_t FP_t;
  typedef ACL::AXIS_t AXIS_t;
  typedef std::uint32_t imageID_t;                                      ///< The imageID type. 32 bit == 4 billion images.
  typedef std::uint16_t imageVersion_t;                                 ///< The type to use with image versions = 65536 versions.

  QString const EXTENSION_CSV                 ("Comma Delimited (*.csv)");

#ifdef USE_LIBRAW
  QString const EXTENSION_IMAGE = QString(QObject::tr("Image Files (*.fts *.fit *.ST7 *.DNG)"));
#else
  QString const EXTENSION_IMAGE = QString(QObject::tr("Image Files (*.fts *.fit *.fits *.ST7)"));
#endif

  QString const EXTENSION_FITS = QString(QObject::tr("FITS Files (*.fts *.fit *.fits"));

    // Source Extraction Algorithms

  extern std::vector<std::pair<int, std::string>> SEAlgorithms;

  QString const DW_MESSAGE_NAME       ("Message");        ///< Name of the Message dock widget
  QString const DW_NAVIGATOR_NAME     ("Navigator");      ///< Name of the Navigator dock widget
  QString const DW_PHOTOMETRY_NAME    ("Photometry");     ///< Name of the photometry dock widget
  QString const DW_MAGNIFIER_NAME     ("Magnifier");      ///< Name of the magnifier dock widget
  QString const DW_ASTROMETRY_NAME    ("Astrometry");     ///< Name of the astrometry dock widget
  QString const DW_HISTOGRAM_NAME     ("Histogram");      ///< Name of the histogram dock widget
  QString const DW_INFORMATION_NAME   ("Information");    ///< Name of the information dock widget
  QString const DW_WEATHER_SCALE      ("Weather Scale");
  QString const DW_WEATHER_PARAMETERS ("Weather Paramaters");

} // namespace AstroManager

#endif  // VSOP_H
