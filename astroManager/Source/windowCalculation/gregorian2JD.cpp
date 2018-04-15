//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowCalculationGregorian2JD
// SUBSYSTEM:						Subclass for calculation windows
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CWindowCalculation
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        CMdiSubWindow
//                          CWindowCalculation
//                            CWindowCalculationGregorian2JD
//
// HISTORY:             2017-07-06/GGB - File Created
//
//*********************************************************************************************************************************

#include "../../Include/windowCalculation/gregorian2JD.h"

  // AIRDAS includes

#include "../../Include/FrameWindow.h"
#include "../../Include/qtExtensions/qt.h"

  // Miscellaneous libraries

#include <ACL>

namespace AstroManager
{
  namespace calculation
  {
    //*************************************************************************************************
    //
    // Window to calculate the julian day from a gregorian date
    //
    //*************************************************************************************************

      /// @brief Constructor. Open window from stored resource
      /// @param[in] parent - The parent object for the instance.
      /// @throws None.
      /// @version 2017-07-06/GGB - Updated to use setupUI() and added size hint.
      /// @version 2010-05-21/GGB - Function created.

      CGregorian2JD::CGregorian2JD(QWidget *parent) : CWindowCalculation(parent)
      {
        setupUI();

        setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
      }

      /// @brief Calculates and displays the JD from the date time gregorian date.
      /// @param[in] date - The date to convert to a JD.
      /// @throws None.
      /// @version 2010-05-21/GGB - Converted to use Qt 4.7.4.
      /// @version 2010-01-02/GGB - Function created.

      void CGregorian2JD::eventCalculateJD(const QDate &date)
      {
        char szJD[30];
        ACL::TJD JD;

        JD = date.toJulianDay();
        JD.JD(szJD, sizeof(szJD), 1);
        tbJulianDay->setText(szJD);
      }

      /// @brief Sets up the UI of the file.
      /// @throws GCL::CRuntimeError()
      /// @throws GCL::CError(AIRDAS, 0x0001)
      /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
      /// @version 2017-07-06/GGB - Function created.

      void CGregorian2JD::setupUI()
      {
        QUiLoader loader;

        QFile file(":/forms/windowGregorian2Julian.ui");

        if (!file.open(QFile::ReadOnly))
        {
          ERRORMESSAGE("Unable to open resource :/forms/windowGregorian2Julian.ui.");
          ERROR(AIRDAS, 0x0001);
        }

        QWidget *formWidget = loader.load(&file, this);
        file.close();

        setWidget(formWidget);

        ASSOCIATE_LABEL(tbJulianDay, formWidget, "tbJulianDay");
        ASSOCIATE_CONTROL(dwGregorian, formWidget, "dwGregorian", QDateEdit);

          // Set the end time to the time now.

        dwGregorian->setDateTime(QDateTime::currentDateTime());

        connect(dwGregorian, SIGNAL(dateChanged(const QDate &)), this, SLOT(eventCalculateJD(const QDate &)));


      }

  } // namespace calculation
} // namespace AstroManager
