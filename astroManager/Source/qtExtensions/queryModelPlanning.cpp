//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:                queryModelPlanning
// SUBSYSTEM:           Query Model for the planning window.
// LANGUAGE:            C++
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
//
// OVERVIEW:
//
// CLASSES INCLUDED:    CWindowPlanning
//
// CLASS HIERARCHY:     CMdiSubWindow
//                        - CWindowPlanning
//
// HISTORY:             2018-02-03 GGB - File Created.
//
//*********************************************************************************************************************************

#include "../../Include/qtExtensions/queryModelPlanning.h"

#include "../../Include/database/databaseARID.h"

#include <ACL>

namespace AstroManager
{
  namespace QTE
  {

    /// @brief Constructor for the Planning Query model.
    /// @throws
    /// @version 2018-02-03/GGB - Function created.

    CQueryModelPlanning::CQueryModelPlanning(planID_t planID) : planID(planID)
    {
      sqlWriter_.resetQuery();
      sqlWriter_.select({"TBL_TARGETS.TARGET_ID", "TBL_TARGETS.RANK", "ATID.TBL_NAMES.NAME"})
          .from({"TBL_TARGETS"})
          .join({std::make_tuple("TBL_TARGETS", "NAME_ID", GCL::sqlwriter::CSQLWriter::JOIN_LEFT, "ATID.TBL_NAMES", "NAME_ID")})
//                 std::make_tuple("TBL_IMAGES", "SITE_ID", GCL::sqlwriter::CSQLWriter::JOIN_LEFT, "TBL_SITES", "SITE_ID"),
//                std::make_tuple("TBL_IMAGES", "TELESCOPE_ID", GCL::sqlwriter::CSQLWriter::JOIN_LEFT, "TBL_TELESCOPES", "TELESCOPE_ID")})
           .where({GCL::sqlwriter::parameterTriple(std::string("PLAN_ID"), std::string("="), planID)});

      DEBUGMESSAGE(sqlWriter_.string());

      setQuery(QString::fromStdString(sqlWriter_.string()), database::databaseARID->database());

        /// @todo Change this code to check for an error before printing the error.

      QSqlError error = lastError();
      ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
      ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
      ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());

      setHeaderData(id_c, Qt::Horizontal, QObject::tr("ID"));
      setHeaderData(rank_c, Qt::Horizontal, QObject::tr("Rank"));
      setHeaderData(name_c, Qt::Horizontal, QObject::tr("Name"));
      setHeaderData(type_c, Qt::Horizontal, QObject::tr("Type"));
      //setHeaderData(ra_c, Qt::Horizontal, QObject::tr("RA"));
      //setHeaderData(dec_c, Qt::Horizontal, QObject::tr("DEC"));
      //setHeaderData(altitude_c, Qt::Horizontal, QObject::tr("Altitude"));
      //setHeaderData(azimuth_c, Qt::Horizontal, QObject::tr("Azimuth"));
      //setHeaderData(airmass_c, Qt::Horizontal, QObject::tr("Airmass"));
      //setHeaderData(appMag_c, Qt::Horizontal, QObject::tr("Apparent Magnitude"));
      //setHeaderData(constellation_c, Qt::Horizontal, QObject::tr("Constallation"));
      //setHeaderData(extinction_c, Qt::Horizontal, QObject::tr("Magnitude"));
    }

    /// @brief Function to ensure that the data is formatted correctly before being displayed in the tableModel.
    /// @param[in] item - Index of the item that needs to be displayed.
    /// @param[in] role - The role being queried.
    /// @returns QVariant with the data or information needed.
    /// @throws None.
    /// @version 2018-03-02/GGB - Function created.

    QVariant CQueryModelPlanning::data(QModelIndex const &item, int role) const
    {
      QVariant returnValue;

      switch (role)
      {
        case Qt::DisplayRole:           // The key data to be rendered in the form of text. (QString)
        {
          returnValue = CSQLQueryModel::data(item, role);

          switch (item.column())
          {
            case id_c:            // Target Identifier
            case rank_c:
            case name_c:          // Target Name
            case type_c:          // Target Type
            case altitude_c:
            case azimuth_c:
            case airmass_c:
              break;
            case ra_c:     /* RA */
            {
              returnValue = QString::fromStdString(GCL::sprintfHMS(returnValue.toDouble(), 2));
              break;
            };
            case dec_c:     /* DEC */
            {
              returnValue = QString::fromStdString(GCL::sprintfDMS(returnValue.toDouble(), true, 2));
              break;
            };
            default:
            {
              ASTROMANAGER_CODE_ERROR;
              break;
            };
          };
          break;
        }
        case Qt::DecorationRole:        // The data to be rendered as a decoration in the form of an icon. (QColor, QIcon or QPixmap)
        case Qt::EditRole:              // The data in a form suitable for editing in an editor. (QString)
        case Qt::ToolTipRole:           // The data displayed in the item's tooltip. (QString)
        case Qt::StatusTipRole:         // The data displayed in the status bar. (QString)
        case Qt::WhatsThisRole:         // The data displayed for the item in "What's This?" mode. (QString)
        case Qt::SizeHintRole:          // The size hint for the item that will be supplied to views. (QSize)
        case Qt::FontRole:              // The font used for items rendered with the default delegate. (QFont)
          break;
        case Qt::TextAlignmentRole:     // The alignment of the text for items rendered with the default delegate. (Qt::AlignmentFlag)
        {
          break;
        }
        case Qt::BackgroundRole:        // The background brush used for items rendered with the default delegate. (QBrush)
        {
          break;
        };
        case Qt::ForegroundRole:        // The foreground brush (text color, typically) used for items rendered with the default delegate. (QBrush)
        case Qt::CheckStateRole:        // This role is used to obtain the checked state of an item. (Qt::CheckState)
        case Qt::InitialSortOrderRole:  // This role is used to obtain the initial sort order of a header view section. (Qt::SortOrder). This role was introduced in Qt 4.8.
          break;
        default:
        {
          ASTROMANAGER_CODE_ERROR;
          break;
        }
      };

      return returnValue;
    }

    /// @brief Resets the query of the query model.
    /// @throws None.
    /// @version 2018-02-03/GGB - Function created.

    void CQueryModelPlanning::resetQuery()
    {
      setQuery(QString::fromStdString(sqlWriter_.string()), database::databaseARID->database());
    }

  } // namespace QTE
} // namespace AstroManager
