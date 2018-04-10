//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowCalculation
// SUBSYSTEM:						Subclass for calculation windows
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017 Gavin Blakeman.
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
// CLASSES INCLUDED:    CWindowCalculation
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        CMdiSubWindow
//                          CWindowCalculation
//
// HISTORY:             2017-07-06/GGB - File Created
//
//*********************************************************************************************************************************

#include "../../Include/windowCalculation/windowCalculation.h"

#include "../../Include/FrameWindow.h"

namespace AstroManager
{
  namespace calculation
  {
    /// @brief Class constructor.
    /// @param[in] widget - The owner widget.
    /// @throws None.
    /// @version 2017-07-06/GGB - Function created.

    CWindowCalculation::CWindowCalculation(QWidget *widget) : CMdiSubWindow(widget)
    {

    }

    /// @brief Processes the close event. Lets the MDI window know that this sub-window has been closed.
    /// @param[in] event - The close event information.
    /// @throws None.
    /// @version 2017-07-06/GGB - Updated to use dynamic_cast<>()
    /// @version 2010-05-21/GGB - Function created.

    void CWindowCalculation::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      QMdiSubWindow::closeEvent(event);		// Close the sub window.
    }

  } // namespace calculation
} // namespace AstroManager
