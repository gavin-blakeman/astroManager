//*********************************************************************************************************************************
//
// PROJECT:             astroManager
// FILE:                TextEditSink
// SUBSYSTEM:           Logging Library
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, GCL
// NAMESPACE:						AstroManager::logger
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2014-2018 Gavin Blakeman.
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
// OVERVIEW:            This file implements a logging class to be used to log messages to file.
//                      The class has been designed as a lightweight easy to use class.
//
// CLASSES INCLUDED:    CTextEditSink
//
// HISTORY:             2016-05-07/GGB - Moved file and class from GCL to astroManager.
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2014-12-25 GGB - Development of class for "Observatory Weather System - Service"
//
//*********************************************************************************************************************************

#include "../../include/qtExtensions/TextEditSink.h"

namespace astroManager
{
  namespace logger
  {
    //******************************************************************************************************************************
    //
    // CTextEditSink
    //
    //******************************************************************************************************************************

    /// @brief Default constructor for class.
    /// @param[in] te - The text edit control used to display the log messages.
    /// @throws None.
    /// @version 2016-05-07/GGB - Bug# 109. Added functionality to update from non-gui thread.
    /// @version 2014-12-25/GGB - Function created.

    CTextEditSink::CTextEditSink(QTextEdit *te) : GCL::logger::CLoggerSink(), textEdit(te)
    {
      connect(this, SIGNAL(signalLogColour(QColor)), textEdit, SLOT(setTextColor(QColor)));
      connect(this, SIGNAL(signalLogAppend(QString)), textEdit, SLOT(append(QString)));
    }

    /// @brief Writes the text to the device.
    /// @param[in] s - The log string to display.
    /// @throws GCL::CError(GCL, 0x1002) - LOGGER: Text Edit not assignd.
    /// @version 2017-07-01/GGB - Added colour to the Bug #83
    /// @version 2016-05-07/GGB - Bug# 109. Added functionality to update from non-gui thread.
    /// @version 2014-12-25/GGB - Function created.

    void CTextEditSink::write(std::string const &s)
    {
      QColor textColour("black");

      if (textEdit)
      {
        if (s.find("[critical]") != std::string::npos)
        {
          textColour.setNamedColor("red");
        }
        else if (s.find("[error]") != std::string::npos)
        {
          textColour.setNamedColor("darkRed");
        }
        else if (s.find("[warning]") != std::string::npos)
        {
          textColour.setNamedColor("darkYellow");
        }
        else if (s.find("[notice]") != std::string::npos)
        {
          textColour.setNamedColor("black");
        }
        else if (s.find("[information]") != std::string::npos)
        {
          textColour.setNamedColor("green");
        }
        else if (s.find("[debug]") != std::string::npos)
        {
          textColour.setNamedColor("darkBlue");
        }
        else if (s.find("[trace]") != std::string::npos)
        {
          textColour.setNamedColor("cyan");
        };

        emit signalLogColour(textColour);
        emit signalLogAppend(QString::fromStdString(s));

        QEvent *event = new QEvent(QEvent::UpdateRequest);

        QApplication::postEvent(textEdit, event);
      }
      else
      {
        ERROR(GCL, 0x1002);     // LOGGER: Text Edit not assignd.
      };
    }

  }   // namespace logger
}   // namespace GCL
