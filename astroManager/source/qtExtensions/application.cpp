﻿//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                Application
// SUBSYSTEM:						QApplication subclassing
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2018 Gavin Blakeman.
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
//
// OVERVIEW:						Implements a subclass of the QApplication class. Notibly, the notify() function is subclassed to enable
//                      exception handling.
//                      The exceptions are caught and handled in this class. Any exceptions caught will be logged and/or displayed.
//
// CLASSES INCLUDED:    CApplication
//
// CLASS HIERARCHY:     QApplication
//                        - CApplication
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-07-27 GGB - File created.
//
//*********************************************************************************************************************************

#include "../../include/qtExtensions/application.h"
#include "../../include/error.h"

#include <iostream>
#include <fstream>

#include "boost/lexical_cast.hpp"

#include <GCL>
#include <ACL>

namespace astroManager
{

  //*******************************************************************************************************************************
  //
  // astroManagerApplication
  //
  //*******************************************************************************************************************************

  /// @brief Event handler to ensure that unhandled exceptions are caught and handled.
  /// @throws None.
  /// @version 2020-08-31/GGB - Remove code that logged to std::clog. This would repeat the message logging.
  /// @version 2017-07-10/GGB - Fixed Bug #38 - Use of GCL rather than library name.
  /// @version 2017-06-20/GGB - Updating to use GCL error handling. (Bug #70)
  /// @version 2017-06-18/GGB - Removed dead code (Bug #38)
  /// @version 2016-04-07/GGB - Added catch() for cfitsio errors.
  /// @version 2013-08-11/GGB - Function created.

  bool CApplication::notify(QObject *receiver, QEvent *event)
  {
    try
    {
      return QApplication::notify(receiver, event);
    }
    catch (GCL::CCodeError &codeError)
    {
      QMessageBox msgBox;

      msgBox.setText(QString::fromStdString("Code Error"));
      msgBox.setStandardButtons(QMessageBox::Abort);
      msgBox.setDefaultButton(QMessageBox::Abort);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setInformativeText(QString::fromStdString(codeError.what()));

      msgBox.exec();

      exit(-1);
    }
    catch (GCL::CError &error)
    {
      std::clog << error.library() << " unhandled exception: " << error.errorCode() << std::endl;
      std::clog << error.errorMessage() << std::endl;

      exit(error.errorCode());
    }
    catch (GCL::CRuntimeAssert &error)
    {
      std::clog << error.what() << std::endl;

      exit(-1);
    }

    catch(...)
    {
      std::clog << "Unhandled exception, unknown error." << std::endl;

      exit( -1 );
    };
  }

}   // namespace AstroManager
