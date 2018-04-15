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
//
// OVERVIEW:
//
// CLASSES INCLUDED:    CTextEditorWindow
//
// CLASS HIERARCHY:     CReferenceImageDockWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2011-06-29 GGB - Development of classes for astroManager
//
//*************************************************************************************************

#include "../Include/TextEditor.h"

/*!
 * @brief The CTextEditorWindow class is used to view and edit text files.
 *
 * @details
 */

namespace AstroManager
{
  namespace texteditor
  {

    //*****************************************************************************************************************************
    //
    // CTextEditorWindow
    //
    //*****************************************************************************************************************************

    /// @brief Class constructor.
    /// @param[in] aParent - The parent of this window.
    ///
    //
    // 2011-06-29/GGB - Function created.

    CTextEditorWindow::CTextEditorWindow(QWidget *aParent) : CMdiSubWindow(aParent), isDirty(false), hasData(false), filePath()
    {
    }

    /// @brief Performs the save function.
    //

    bool CTextEditorWindow::save()
    {
      return false;
    }

    /// @brief Performs the saveAs function
    //


    bool CTextEditorWindow::saveAs()
    {
      return true;
    }

    /// @brief Called before the window activates.
    /// @brief Allows the window to change the look of the menu when it activates.
    //

    void CTextEditorWindow::windowActivating()
    {
    }


  }  // namespace texteditor
}  // namespace AstroManager
