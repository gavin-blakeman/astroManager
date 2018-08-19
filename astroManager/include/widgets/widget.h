//*********************************************************************************************************************************
//
// PROJECT:							AstroManager
// FILE:								widget
// SUBSYSTEM:						Parent for all general widget inheretence
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:           AstroManager::widgets
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
// OVERVIEW:            Dialog to allow editing of resources (master data) for various parts of the application.
//
// CLASSES INCLUDED:    CWidget
//
// CLASS HIERARCHY:     CWidget
//
// HISTORY:             2018-02-01 GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef WIDGET_H
#define WIDGET_H

#include "../qtExtensions/qt.h"

namespace astroManager
{
  namespace widgets
  {
    class CWidget : public QWidget
    {
    private:
    protected:
    public:
      CWidget(QWidget *parent = 0, const char *name = 0) : QWidget(parent) {}
      ~CWidget() {}
    };
  }
}

#endif // WIDGET_H

