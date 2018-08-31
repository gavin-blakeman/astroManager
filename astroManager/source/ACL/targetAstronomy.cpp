//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								targetAstronomy
// SUBSYSTEM:						Display weather data
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	ACL, Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:
//
// HISTORY:             2018-08-30 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../include/ACL/targetAstronomy.h"

  // Standard C++ library header files

#include <utility>

namespace astroManager
{

  /// @brief Constructor for the class. Also set the size of the vector.
  /// @param[in] parent: The table widget associated with this target.
  /// @param[in] newTarget: The target to assign to this instance.
  /// @throws std::bad_alloc
  /// @version 2018-08-31/GGB - Function created

  CTargetAstronomy::CTargetAstronomy(QTableWidget *parent, std::unique_ptr<ACL::CTargetAstronomy> newTarget)
    : parent_(parent), columnMap(col_columnCount), targetAstronomy(std::move(newTarget))
  {

  }

  /// @brief Sets the widget for the specified column.
  /// @param[in] columnNumber: The column to set the widget for.
  /// @param[in] widget: The widget associated with the column.
  /// @throws None.
  /// @version 2018-08-31/GGB - Function created.

  void CTargetAstronomy::setColumnWidget(std::uint8_t columnNumber, QTableWidgetItem *widget)
  {
    columnMap.at(columnNumber) = widget;
  }

} // namespace astroManager
