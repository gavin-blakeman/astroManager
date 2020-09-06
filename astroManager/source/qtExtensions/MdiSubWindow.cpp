﻿//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								MdiSubWindow
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
// OVERVIEW:            Implements the MDI Frame window for the application. There is a single class implemented:
//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CMdiSubWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CMdiSubWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-03 GGB - Moved code from FrameWindow
//                      2012-01-20 GGB - astroManager 0000.00 release
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/qtExtensions/MdiSubWindow.h"
#include "../../include/FrameWindow.h"

namespace astroManager
{
  //*****************************************************************************************************************************
  //
  // CMdiSubWindow
  //
  //*****************************************************************************************************************************

  /// @brief Class constructor
  /// @param[in] parent: The owner of this widget.
  /// @throws None.
  /// @version 2013-07-27/GGB - Removed connecting the aboutToActivate() signal to the windowActivating() slot.
  /// @version 2011-05-29/GGB - Function created.

  CMdiSubWindow::CMdiSubWindow(QWidget *parent) : QMdiSubWindow(parent)
  {
  }

  /// @brief Ensures that the window name is removed from the list of windows in the frame.
  /// @param[in] event: The event that is associated with the closeEvent.
  /// @throws None.
  /// @version 2011-05-15/GGB - Function created.

  void CMdiSubWindow::closeEvent(QCloseEvent *event)
  {
    dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
    QMdiSubWindow::closeEvent(event);		// Close the sub window.
  }

  /// @brief Function called when a mdi child window is activated.
  /// @details This function calls all the dockWidget windowActivating functions.
  /// @throws None.
  /// @note 1. The subwindow function is called from CFrameWindow. This function must be called by all inheriting classes, unless
  ///          they specifically replicate the functionality in this function.
  /// @note 2. This function calls the windowActivating function for all the dockwidgets.
  /// @version 2018-10-21/GGB - Function created.

  void CMdiSubWindow::windowActivating()
  {
    mdiframe::CFrameWindow *frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());
    RUNTIME_ASSERT(frameWindow != nullptr,  "Parent widget cannot be nullptr.");

    if (frameWindow)
    {
        // Iterate over all the dockwidgets, but check them for validity before calling.
        // Note it is not an error of the dockWidget does not exist.

      for (mdiframe::CFrameWindow::dockwidget_t index = mdiframe::IDDW_IMAGECONTROL; index < mdiframe::IDDW_COUNT; index++)
      {
        if (frameWindow->getDockWidget(index) != nullptr)
        {
          frameWindow->getDockWidget(index)->mdiWindowActivating(this);
        };
      };
    }
    else
    {
      CODE_ERROR;
    };
  }

}  // namespace AstroManager
