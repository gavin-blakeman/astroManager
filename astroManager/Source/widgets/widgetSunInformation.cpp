//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:                widgetSunInformation
// SUBSYSTEM:           Widget to show sun information
// LANGUAGE:            C++17
// TARGET OS:           WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:  Qt
// NAMESPACE:           AstroManager::widgets
// AUTHOR:              Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
//                      This file is part of the AstroManager software.
//
//                      AstroManager is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      AstroManager is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with AstroManager.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            The widget displays the sun information. IE twilight times and rise/set times,
//
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:     CWidget
//
// HISTORY:             2018-02-01 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../Include/widgets/widgetSunInformation.h"

namespace AstroManager
{
  namespace widgets
  {
    /// @brief Class constructor
    /// @param[in] parent - The parent of the widget.
    /// @throws std::bad_alloc
    /// @version 2018-02-03/GGB - Function created.

    CWidgetSunInformation::CWidgetSunInformation(QWidget *parent) : CWidget(parent, "Sun Information")
    {
      setupUI();
    }

    /// @brief Sets up the user interface of the widget.
    /// @throws GCL::CRuntimeError(AstroManager)
    /// @version 2018-02-03/GGB - Function created.

    void CWidgetSunInformation::setupUI()
    {

    }

  } // namespace widgets
} // namespace AstroManager
