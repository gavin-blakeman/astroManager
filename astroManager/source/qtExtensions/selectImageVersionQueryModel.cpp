//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								selectImageVersionQueryModel
// SUBSYSTEM:						Subclass for selection of images from the database.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CSelectImageVersionQueryModel
//
// CLASS HIERARCHY:     QSqlQueryModel
//                        CSQLQueryModel
//                          CSelectImageVersionQueryModel
//
// HISTORY:             2017-08-12/GGB - File Created
//
//*********************************************************************************************************************************

#include "../../include/qtExtensions/selectImageVersionQueryModel.h"

#include "../../include/error.h"
#include "../../include/database/databaseARID.h"

  // Standard C++ libraries

#include <ctime>

  // Miscellaneous libraries

#include <ACL>
#include <GCL>

namespace astroManager
{
  namespace QTE
  {
    /// @brief Class constructor. Sets the query and header values.
    /// @param[in] parent - The parent instance
    /// @throws std::bad_alloc
    /// @version 2017-08-16/GGB - Function created.

    CSelectImageVersionQueryModel::CSelectImageVersionQueryModel(imageID_t imageID, QObject *parent) : CSQLQueryModel(parent),
      imageID_(imageID)
    {
      GCL::sqlwriter::CSQLWriter sqlWriter;

      sqlWriter.select({"IMAGE_VERSION", "DATETIME", "COMMENT"}).from({"TBL_IMAGESTORAGE"})
          .where({GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID)})
          .orderBy({std::make_pair("IMAGE_VERSION", GCL::sqlwriter::CSQLWriter::DESC)});

      setQuery(QString::fromStdString(sqlWriter.string()), database::databaseARID->database());

      int columnIndex = 0;
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Version"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Date/Time"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Comments"));
    }

    /// @brief Function to ensure that the data is formatted correctly before being displayed in the tableModel.
    /// @param[in] item - Index of the item that needs to be displayed.
    /// @param[in] role - The role being queried.
    /// @returns QVariant with the data or information needed.
    /// @throws None.
    /// @version 2017-08-01/GGB - Function created.

    QVariant CSelectImageVersionQueryModel::data(QModelIndex const &item, int role) const
    {
      QVariant returnValue;

      switch (role)
      {
        case Qt::DisplayRole:           // The key data to be rendered in the form of text. (QString)
        {
          returnValue = CSQLQueryModel::data(item, role);

          switch (item.column())
          {
            case 0:
            {
              break;
            }
            case 1:       // Date/Time - As JD
            {
              struct tm time;
              ACL::TJD JD(returnValue.toDouble());
              JD.gregorianDate(&time);
              returnValue = QString::fromStdString(GCL::sprintDateTime(&time));
              break;
            }
            case 2:
            {
              break;
            }
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
        case Qt::TextAlignmentRole:     // The alignment of the text for items rendered with the default delegate. (Qt::AlignmentFlag)
        case Qt::BackgroundRole:        // The background brush used for items rendered with the default delegate. (QBrush)
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

    /// @brief Forces a refresh of the query data.
    /// @throws None.
    /// @version 2018-05-12/GGB - Function created.

    void CSelectImageVersionQueryModel::resetQuery()
    {
      GCL::sqlwriter::CSQLWriter sqlWriter;

      sqlWriter.select({"IMAGE_VERSION", "DATETIME", "COMMENT"}).from({"TBL_IMAGESTORAGE"})
          .where({GCL::sqlwriter::parameterTriple(std::string("IMAGE_ID"), std::string("="), imageID_)})
          .orderBy({std::make_pair("IMAGE_VERSION", GCL::sqlwriter::CSQLWriter::DESC)});

      setQuery(QString::fromStdString(sqlWriter.string()), database::databaseARID->database());

    }

  } // namespace QTE
} // namespace AstroManager
