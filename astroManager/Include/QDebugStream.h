//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								QDebugStream.h
// SUBSYSTEM:						
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						None
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
// Copyright 2012-2013 Gavin Blakeman.
// This file is part of the Astronomical Image Reduction and Data Analysis Software (AIRDAS)
//
// AIRDAS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as 
// published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
//
// AIRDAS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with AIRDAS.  If not, 
// see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:

//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CDebugStream
//
// CLASS HIERARCHY:     QDebugStream
//
// HISTORY:             0000.00.000 2012-07-28 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef Q_DEBUG_STREAM_H
#define Q_DEBUG_STREAM_H

#include <iostream>
#include <streambuf>
#include <string>

  // Qt framework

#include "Qt.h"

class QDebugStream : public std::basic_streambuf<char>
{
private:
  QDebugStream(QDebugStream const &);
  QDebugStream &operator=(QDebugStream const &);

public:
  QDebugStream(std::ostream &stream, QTextEdit* text_edit) : m_stream(stream)
  {
    log_window = text_edit;
    m_old_buf = stream.rdbuf();
    stream.rdbuf(this);
  };


  ~QDebugStream()
  {
      // output anything that is left
    if (!m_string.empty())
      log_window->append(m_string.c_str());

    m_stream.rdbuf(m_old_buf);
  };

protected:
  virtual int_type overflow(int_type v)
  {
    if (v == '\n')
    {
      log_window->append(m_string.c_str());
      m_string.erase(m_string.begin(), m_string.end());
    }
    else
      m_string += v;

    return v;
  }

  virtual std::streamsize xsputn(const char *p, std::streamsize n)
  {
    QString szText;
    size_t pos = 0;

    m_string.append(p, p + n);
    
    while (pos != std::string::npos)
    {
      pos = m_string.find('\n');
      if (pos != std::string::npos)
      {
        std::string tmp(m_string.begin(), m_string.begin() + pos);
        log_window->append(tmp.c_str());
        m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
      };
    };

    log_window->repaint();
    return n;
  }

private:
  std::ostream &m_stream;
  std::streambuf *m_old_buf;
  std::string m_string;
  QTextEdit* log_window;

};

#endif
