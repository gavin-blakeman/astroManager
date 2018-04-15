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

#include "../Include/TextEditorFITS.h"

  // Standard C++ libraries

#include <algorithm>

namespace AstroManager
{
  namespace texteditor
  {

    /// @brief Class constructor.
    /// @param[in] parent - The parent object.
    /// @param[in] np - The new file path.
    /// @throws None.
    /// @version 2015-08-04/GGB - Function created.

    CTextEditorFITS::CTextEditorFITS(QWidget *parent, boost::filesystem::path &np) : CTextEditorWindow(parent), filePath(np),
      inspector(np)
    {
      setupUI();

      inspector.loadHeaderData();

      setupComboBoxHDU();
    }

    /// @brief Responds to the close event event.
    /// @param[in] event - The close event.
    /// @throws None.
    /// @version 2015-08-07/GGB - Function created.

    void CTextEditorFITS::closeEvent(QCloseEvent *event)
    {
      dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget())->childClosing(this);  // Let the parent know.
      event->accept();
    }

    /// @brief Function to display the data in the text display area.
    /// @param[in] currentHDU - The new selected HDU.
    /// @throws None.
    /// @version 2015-08-08/GGB - Function created.

    void CTextEditorFITS::eventDisplayHeaderData(int currentHDU)
    {
      int nRow = 0;
      tableWidget->clearContents();     // Clear the existing text.

      tableWidget->setRowCount(inspector.keywordCount(currentHDU));

      std::for_each(inspector.begin(currentHDU), inspector.end(currentHDU),
                    [this, &nRow] (ACL::CFITSInspector::CHDU::SFITSKeyword kwd)
      {
        QTableWidgetItem *twi;
        int nColumn = 0;

        twi = new QTableWidgetItem(QString::fromStdString(kwd.keyword));
        tableWidget->setItem(nRow, nColumn++, twi);

        twi = new QTableWidgetItem(QString::fromStdString(kwd.value));
        tableWidget->setItem(nRow, nColumn++, twi);

        twi = new QTableWidgetItem(QString::fromStdString(kwd.comment));
        tableWidget->setItem(nRow++, nColumn, twi);
      });
    }

    /// @brief Sets the data in the combo box.
    /// @throws None.
    /// @version 2015-08-07/GGB - Function created.

    void CTextEditorFITS::setupComboBoxHDU()
    {
      std::for_each(inspector.begin(), inspector.end(),
                    [this] (ACL::CFITSInspector::CHDU hdu)
      {comboBoxHDU->insertItem(hdu.hduNumber, QString::fromStdString(hdu.hduName));} );
    }


    /// @brief Function to setup the window.
    /// @throws GCL::CRuntimeAssert(AIRDAS)
    /// @throws GCL::CError(AIRDAS, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2015-08-07/GGB - Function created.

    void CTextEditorFITS::setupUI()
    {
      QUiLoader loader;

        // Create the window details from the template

      QFile file(":/forms/frameFITSInspect.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/frameFITSInspect.ui.");
        ERROR(AIRDAS, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      ASSOCIATE_CONTROL(comboBoxHDU, formWidget, "comboBoxHDU", QComboBox);
      ASSOCIATE_CONTROL(tableWidget, formWidget, "tableWidget", QTableWidget);

      tableWidget->setColumnWidth(0, 200);
      tableWidget->setColumnWidth(1, 400);
      tableWidget->setColumnWidth(2, 600);

      connect(comboBoxHDU, SIGNAL(currentIndexChanged(int)), this, SLOT(eventDisplayHeaderData(int)));
    }

  } // namespace texteditor
} //  namespace AstroManager
