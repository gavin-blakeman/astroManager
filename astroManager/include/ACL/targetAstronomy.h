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
//                      Copyright 2018-2020 Gavin Blakeman.
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

#ifndef TARGETASTRONOMY_H
#define TARGETASTRONOMY_H

  // Standard C++ library header files

#include <cstdint>
#include <memory>
#include <vector>

// Miscellaneous library header files

#include <ACL>
#include <QCL>

  // astroManager application header files

#include "../astroManager.h"

namespace astroManager
{
  /* Note this is not derived from ACL::CTargetAstronomy as it needs to be able to work with any of the types of
   * astronomy targets. If we derived this, then there would need to be three or four classes.
   */

  class CTargetAstronomy final
  {
  public:
    enum
    {
      column_start = 0,
      column_rank = 0,
      column_name,
      column_type,
      column_ra,
      column_dec,
      column_altitude,
      column_azimuth,
      column_airmass,
      column_appMag,
      column_constellation,
      column_extinction,
      column_hourAngle,
      column_magnitude,
      column_observationCount,
      column_opposition,
      column_riseTime,
      column_setTime,
      column_transitTime,
      column_transitAltitude,
      column_angularSize,
      column_catalogue,
      column_end,              // Always leave this one at the end as it contains the total column count.
    };
  private:
    std::vector<QTableWidgetItem *> columnMap;
    std::unique_ptr<ACL::CTargetAstronomy> targetAstronomy_;

    CTargetAstronomy() = delete;

  protected:
    void updateColumnName();
    void updateColumnType();
    void updateColumnRA();
    void updateColumnDEC();
    void updateColumnAltitude();
    void updateColumnAzimuth();
    void updateColumnAirmass();
    void updateColumnApparentMagnitude();
    void updateColumnConstellation();
    void updateColumnExtinction();
    void updateColumnHourAngle();
    void updateColumnMagnitude();
    void updateColumnObservationCount();
    void updateColumnRiseTime();
    void updateColumnSetTime();
    void updateColumnTransitTime();
    void updateColumnTransitAltitude();
    void updateColumnAngularCatalogue();

  public:
    CTargetAstronomy(std::unique_ptr<ACL::CTargetAstronomy>);

    ACL::CTargetAstronomy *targetAstronomy() const;

    void setColumnWidgets(QTableWidget *);
    void updateAllColumnValues();
  };

} // namespace astroManager

#endif // TARGETASTRONOMY_H
