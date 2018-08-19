//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								photometryObservations
// SUBSYSTEM:						Photometry Observations
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
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
// CLASSES INCLUDED:    CPhotometryObservation
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-03-17 GGB - Removed CSelectObjectDialog into file DialogSelectObject.
//                      2013-03-17 GGB - Removed CImageControlWidget into file DockWidgetImageControl.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into it's own file
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/photometry/photometryObservation.h"

#include "../../include/Settings.h"
#include "../../include/astroManager.h"

namespace astroManager
{
  namespace photometry
  {
    //*****************************************************************************************************************************
    //
    // CPhotometryObservation
    //
    //*****************************************************************************************************************************

    /// @brief Default constructor for the class.
    /// @throws None.
    /// @version 2013-08-25/GGB - Function created.

    CPhotometryObservation::CPhotometryObservation() : ACL::CPhotometryObservation(std::string("")), group(nullptr),
      text(nullptr)
    {
    }

    /// @brief Class constuctor. Must null the group and text.
    /// @param[in] po - The photometry observation to copy
    /// @throws None.
    /// @version 2013-08-25/GGB - Function created.

    CPhotometryObservation::CPhotometryObservation(ACL::CPhotometryObservation const &po) :
      ACL::CPhotometryObservation(po), group(nullptr), text(nullptr)
    {
    }

    /// @brief Constructor taking an astronomy target.
    /// @param[in] ta - The astronomy target object.
    /// @throws None.
    /// @version 2016-05-05/GGB - Function created.

    CPhotometryObservation::CPhotometryObservation(ACL::SPTargetAstronomy &ta) : ACL::CPhotometryObservation(ta), group(nullptr), text(nullptr)
    {

    }

    /// Class destructor. Required to ensure dynamically allocated items are deleted.
    //
    // 2015-01-01/GGB - Function created.

    CPhotometryObservation::~CPhotometryObservation()
    {
      if (text)
        text = nullptr;
      if (group)
      {
        delete group;
        group = nullptr;
      }
    }

    /// @brief Friend function to output the object as comma delimited text.
    /// @param[in] os - The output stream.
    /// @param[in] po - The photometry observation.
    /// @returns The output stream.
    /// @throws None.
    /// @version 2017-09-23/GGB - Updated to use CAngle
    /// @version 2015-01-04/GGB - Removed assertion of (po != nullptr) ... Not needed with a reference.
    /// @version 2013-08-22/GGB - Added degrees output to the function.
    /// @version 2013-08-18/GGB - Added the RA/Dec if available to the output.
    /// @version 2012-11-16/GGB - Function created.

    std::ostream &operator << (std::ostream &os, CPhotometryObservation &po)
    {
      std::string szRA, szDec;

      os << std::string(po.objectName()) << ",";

      if (po.observedCoordinates())
      {
        if (settings::astroManagerSettings->value(settings::PHOTOMETRY_CSV_OUTPUTDEGREES, QVariant(true)).toBool())
        {
          os << po.observedCoordinates()->RA() << ", " << po.observedCoordinates()->DEC() << ", ";
        }
        else
        {
          szRA = po.observedCoordinates()->RA().A2SHMS();
          szDec = po.observedCoordinates()->DEC().A2SDMS();
          os << szRA << "," << szDec << ",";
        };
      }
      else
      {
        if (settings::astroManagerSettings->value(settings::PHOTOMETRY_CSV_OUTPUTDEGREES, QVariant(true)).toBool())
          os << "0, 0, ";
        else
        {
          szRA = std::string("--h--'--.--s""");
          szDec = std::string("±--°--'--""");
          os << szRA << "," << szDec << ",";
        };
      }

      os << po.CCDCoordinates().x() << "," << po.CCDCoordinates().y() << ",";
      os << *(po.instrumentMagnitude()) << ", " << po.magnitudeError();
      return os;
    }

  } // namespace photometry
} // namespace AstroManager
