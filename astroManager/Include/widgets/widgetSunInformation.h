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
// OVERVIEW:            The widget displays the sun information. IE twilight times and rise/set times,
//
// CLASSES INCLUDED:
//
// CLASS HIERARCHY:     CWidget
//
// HISTORY:             2018-02-01 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef WIDGETSUNINFORMATION_H
#define WIDGETSUNINFORMATION_H

#include "widget.h"

namespace AstroManager
{
  namespace widgets
  {
    class CWidgetSunInformation : public CWidget
    {
    private:
      void setupUI();
    protected:
    public:
      CWidgetSunInformation(QWidget * = nullptr);
    };
  } // namespace widgets
} // namespace AstroManager

#endif // WIDGETSUNINFORMATION_H

