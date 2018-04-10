//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetMessage
// SUBSYSTEM:						Messaging window
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	ACL, Boost, GCL, Qt
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2012-2017 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CMessageWidget - Widget to display application messages.
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:		          2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-25/GGB - Changed logging to using the GCL::loggert functionality.
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-08-11 GGB - Changed filename to DockWidgetMessage
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - Removed from file DockWidgets.h
//
//*********************************************************************************************************************************

#ifndef DOCKWIDGETMESSAGE_H
#define DOCKWIDGETMESSAGE_H

#include "dockWidget.h"
#include "../qtExtensions/TextEditSink.h"

#include <GCL>

namespace AstroManager
{
  namespace dockwidgets
  {
    class CDockWidgetMessage : public CDockWidget
    {
      Q_OBJECT

    private:
      QTextEdit *textEdit;
      GCL::logger::PLoggerSink textEditSink;

      void setupUI();

    protected:

    public:
      CDockWidgetMessage(QWidget *, QAction *);
      virtual ~CDockWidgetMessage();
    };

  }  // namespace dockwidgets
}  // namespace AstroManager

#endif  // DOCKWIDGETMESSAGE_H
