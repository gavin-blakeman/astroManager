//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                application
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
// OVERVIEW:						Implements a subclass of the QApplication class. Notibly, the notify() function is subclassed to enable
//                      exception handling.
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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCL>

namespace astroManager
{
  class CApplication : public QApplication
  {
  private:
  protected:
  public:
    CApplication(int &argc, char *argv[]) : QApplication(argc, argv) {}

    virtual bool notify(QObject *, QEvent *);
  };

} // namespace

#endif // APPLICATION_H
