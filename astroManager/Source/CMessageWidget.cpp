//*********************************************************************************************************************************
//
// PROJECT:							AIRDAS (Astronomical Image Reduction and Data Analysis Software)
// FILE:								CMessageWidget
// SUBSYSTEM:						Messaging window
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AIRDAS::dockwidgets
// AUTHOR:							Gavin Blakeman.
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
//
// CLASSES INCLUDED:    CMessageWidget - Widget to display application messages. Redirects the std::cour, std::cerr and std::clog 
//                                        to the screen.
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - Removed from file DockWidgets.h
//
//*********************************************************************************************************************************

#include "../Include/CMessageWidget.h"

  // Standard libraries

#include <fstream>

namespace AIRDAS
{
  namespace dockwidgets
  {
    //*****************************************************************************************************************************
    //
    // CMessageWidget
    //
    //*****************************************************************************************************************************

    /// Constructor for the class.
    //
    // 2012-07-28/GGB - Function created.

    CMessageWidget::CMessageWidget(QAction *action, QWidget *aParent) : QDockWidget(aParent), dwAction(action), textEdit(nullptr),
      debugStream(nullptr), clogCopy(std::clog.rdbuf()), widgetCopy(std::clog.rdbuf()), logTee(widgetCopy, clogCopy),
      logTeeStream(logTee)
    {
      originalRdBuf = std::clog.rdbuf(logTeeStream.rdbuf());

      setupUI();
    }

    /// Destructor - Must restore the std::clog rdbuffer.
    //
    // 2012-01-20/GGB - Function created.

    CMessageWidget::~CMessageWidget()
    {
      std::clog.rdbuf(originalRdBuf);
    }

    /// Called when the widget is hiding.
    //
    // 2013-07-02/GGB - Added check before calling the action.
    // 2013-01-20/GGB - Function created.

    void CMessageWidget::hideEvent(QHideEvent *event)
    {
      if (!event->spontaneous())
        if (dwAction)
          dwAction->setChecked(false);

      QDockWidget::hideEvent(event);
    }

    /// Sets up the user interface for the widget.
    //
    // 2012-07-28/GGB - Function created.

    void CMessageWidget::setupUI()
    {

      textEdit = new QTextEdit();

      setWidget(textEdit);

      debugStream = new QDebugStream(widgetCopy, textEdit);
    }

    /// Handles a show event and checks the widget action.
    //
    // 2013-01-20/GGB - Function created.

    void CMessageWidget::showEvent(QShowEvent *event)
    {
      if (!event->spontaneous())
        dwAction->setChecked(true);

      QDockWidget::showEvent(event);
    }

  }  // namespace dockwidgets

}  // namespace AIRDAS
