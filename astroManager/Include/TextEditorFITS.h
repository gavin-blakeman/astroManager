//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								TextEditorFITS
// SUBSYSTEM:						View/Edit FITS HDU Directly
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::texteditor
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2015, 2017-2018 Gavin Blakeman.
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
// OVERVIEW:            Implements the MDI Frame window for the application. There is a single class implemented:
//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CTextEditorFITS
//
// CLASS HIERARCHY:     QMainWindow
//                        - CMdiSubWindow
//                          - CTextEditor
//                            - CTextEditorFITS
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2015-08-03/GGB - File Created.
//
//*********************************************************************************************************************************

#ifndef TEXTEDITORFITS_H
#define TEXTEDITORFITS_H

/// @brief The CTextEditorFITS class allows a FITS file to be opened and hilighted as a text file rather than as a FITS file.
/// details By opening the file as a text file with illustrative hilighting, errors in the file can be identified and possibly
/// corrected. This can be used when opening/editing non-compliant FITS files to make them compliant. For instance if blank keywords
/// are used as comments, the offending line can be edited or deleted and the structure of the file corrected to allow it to be
/// opened.

#include "TextEditor.h"

  // Miscellaneous libraries

#include <ACL>

namespace AstroManager
{
  namespace texteditor
  {
    class CTextEditorFITS : public CTextEditorWindow
    {
      Q_OBJECT

    private:
      boost::filesystem::path filePath;
      ACL::CFITSInspector inspector;

      QTableWidget *tableWidget;
      QComboBox *comboBoxHDU;

      CTextEditorFITS() = delete;
      CTextEditorFITS(CTextEditorFITS const &) = delete;
      CTextEditorFITS &operator=(CTextEditorFITS const &) = delete;

      void setupUI();

    protected:
      void setupComboBoxHDU();


    public:
      CTextEditorFITS(QWidget *parent, boost::filesystem::path &);

      void closeEvent(QCloseEvent *event);

      virtual ESubWindowType getWindowType() const { return SWT_TEXTEDITORFITSWINDOW;}

    private slots:
      void eventDisplayHeaderData(int);
    };


  } // namespace texteditor
} // namespace AstroManager

#endif // TEXTEDITORFITS_H
