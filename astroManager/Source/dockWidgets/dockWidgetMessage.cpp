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
//                      Copyright 2012-2018 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CMessageWidget - Widget to display application messages.
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2014-12-25/GGB - Changed logging to using the GCL::loggert functionality.
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-08-11 GGB - Changed filename to DockWidgetMessage
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - Removed from file DockWidgets.h
//
//*********************************************************************************************************************************

#include "../../Include/dockWidgets/dockWidgetMessage.h"

#include "../../Include/Settings.h"
#include "../../Include/astroManager.h"

namespace AstroManager
{
  namespace dockwidgets
  {
    //*****************************************************************************************************************************
    //
    // CMessageWidget
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class.
    /// @param[in] parent - The parent object.
    /// @param[in] action - The action object.
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of parameters.
    /// @version 2012-07-28/GGB - Function created.

    CDockWidgetMessage::CDockWidgetMessage(QWidget *parent, QAction *action)
      : CDockWidget(DW_MESSAGE_NAME, parent, action, settings::DW_MESSAGE_VISIBLE), textEdit(nullptr),
        textEditSink(nullptr)
    {
      setupUI();
      setObjectName(DW_MESSAGE_NAME);
    }

    /// @brief Destructor - Must remove the logger Sink.
    /// @throws None.
    /// @version 2012-01-20/GGB - Function created.

    CDockWidgetMessage::~CDockWidgetMessage()
    {
      if (!GCL::logger::defaultLogger().removeSink(textEditSink))
      {
        GCL::logger::defaultLogger().logMessage(GCL::logger::error, "Unable to remove log sink after closing message widget.");
      };
    }

    /// @brief Sets up the user interface for the widget.
    /// @throws std::bad_alloc
    /// @version 2013-08-11/GGB - Changed the text edit to read only. (Bug #1210899 )
    /// @version 2012-07-28/GGB - Function created.

    void CDockWidgetMessage::setupUI()
    {
      textEdit = new QTextEdit();

      setWidget(textEdit);
      textEdit->setReadOnly(true);    // Cannot edit the text.

      GCL::logger::CSeverity sev{true, true, true, true, true, false, false};
      textEditSink.reset(new logger::CTextEditSink(textEdit));   // Create the logger for the text edit sink.
      textEditSink->setLogLevel(sev);
      textEditSink->timeStamp(false);
      GCL::logger::defaultLogger().addSink(textEditSink);
    }

  }  // namespace dockwidgets
}  // namespace AstroManager
