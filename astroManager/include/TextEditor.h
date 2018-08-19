//*************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								TextEditor
// SUBSYSTEM:						A Text editor window for use with the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::texteditor
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CTextEditorWindow
//
// CLASS HIERARCHY:     Q
//                        - CReferenceImageDockWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2011-06-09 GGB - Development of classes for astroManager
//
//*************************************************************************************************

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QtGui/QtGui>

#include "../include/FrameWindow.h"

#include "boost/filesystem.hpp"

namespace astroManager
{
  namespace texteditor
  {
    class CTextEditorWindow : public CMdiSubWindow
    {
    private:
      bool isDirty;
      bool hasData;
      boost::filesystem::path filePath;

      virtual void windowActivating();
    protected:
    public:
      CTextEditorWindow(QWidget * = 0);

      virtual ESubWindowType getWindowType() const { return SWT_TEXTEDITORWINDOW;}

      virtual bool save();
      virtual bool saveAs();
    };


  }  // namespace texteditor
}  // namespace AstroManager

#endif  // TEXTEDITOR_H
