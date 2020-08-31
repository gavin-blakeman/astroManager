//*********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:								selectImageQueryModel
// SUBSYSTEM:						Subclass for selection of images from the database.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2020 Gavin Blakeman.
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
// CLASSES INCLUDED:    CSelectImageQueryModel
//
// CLASS HIERARCHY:     QSqlQueryModel
//                        CSQLQueryModel
//                          CSelectImageQueryModel
//
// HISTORY:             2017-08-01/GGB - File Created
//
//*********************************************************************************************************************************

#include "include/qtExtensions/selectImageQueryModel.h"

#include "include/database/databaseARID.h"
#include "include/error.h"

  // Miscellanous libraries

#include <ACL>

namespace astroManager
{
  namespace QTE
  {

    /// @brief Constructor for the imageQuery model.
    /// @throws
    /// @version 2017-08-19/GGB - Function created.

    CSelectImageQueryModel::CSelectImageQueryModel()
    {
      sqlWriter_.resetQuery();
      sqlWriter_.select({"TBL_IMAGES.TARGET", "TBL_FILTERS.SHORTTEXT", "TBL_SITES.SHORTTEXT",
                         "TBL_TELESCOPES.SHORTTEXT", "TBL_IMAGES.IMAGEDATE", "TBL_IMAGES.IMAGETIME", "TBL_IMAGES.RA",
                         "TBL_IMAGES.DECLINATION", "TBL_IMAGES.QUALITY", "TBL_IMAGES.COMMENTS", "TBL_IMAGES.IMAGE_ID"})
          .from({"TBL_IMAGES"})
          .join({std::make_tuple("TBL_IMAGES", "FILTER_ID", GCL::sqlWriter::JOIN_LEFT, "TBL_FILTERS", "FILTER_ID"),
                 std::make_tuple("TBL_IMAGES", "SITE_ID", GCL::sqlWriter::JOIN_LEFT, "TBL_SITES", "SITE_ID"),
                 std::make_tuple("TBL_IMAGES", "TELESCOPE_ID", GCL::sqlWriter::JOIN_LEFT, "TBL_TELESCOPES", "TELESCOPE_ID")});

      setQuery(QString::fromStdString(sqlWriter_.string()), database::databaseARID->database());

      DEBUGMESSAGE(sqlWriter_.string());

      int columnIndex = 0;
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Target"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Filter"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Observing Site"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Telescope"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Date"));
      setHeaderData(columnIndex++, Qt::Horizontal, QObject::tr("Time"));
      setHeaderData(ra_c, Qt::Horizontal, QObject::tr("RA"));
      setHeaderData(dec_c, Qt::Horizontal, QObject::tr("DEC"));
      setHeaderData(quality_c, Qt::Horizontal, QObject::tr("Quality"));
      setHeaderData(comments_c, Qt::Horizontal, QObject::tr("Comments"));
      setHeaderData(imageID_c, Qt::Horizontal, QObject::tr("Image ID"));
    }

    /// @brief Function to ensure that the data is formatted correctly before being displayed in the tableModel.
    /// @param[in] item - Index of the item that needs to be displayed.
    /// @param[in] role - The role being queried.
    /// @returns QVariant with the data or information needed.
    /// @throws None.
    /// @version 2017-08-01/GGB - Function created.

    QVariant CSelectImageQueryModel::data(QModelIndex const &item, int role) const
    {
      QVariant returnValue;

      switch (role)
      {
        case Qt::DisplayRole:           // The key data to be rendered in the form of text. (QString)
        {
          returnValue = CSQLQueryModel::data(item, role);

          switch (item.column())
          {
            case target_c:   // Target

            case filter_c:   // Filter

            case observingSite_c:   // Observing Site
            case telescope_c:   // Telescope
              break;
            case date_c:     /* Date */
            {
              ACL::TJD JD(returnValue.toDouble());
              returnValue = QString::fromStdString(JD.gregorianDate());
              break;
            };
            case time_c:     /* Time */
            {
              returnValue = QString::fromStdString(GCL::sprintfHMS(returnValue.toUInt()));
              break;
            }
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
            case quality_c:     /* Quality */
            {
              switch (returnValue.toUInt())
              {
                case 1:
                {
                  returnValue = QString("*");
                  break;
                };
                case 2:
                {
                  returnValue = QString("**");
                  break;
                };
                case 3:
                {
                  returnValue = QString("***");
                  break;
                };
                case 4:
                {
                  returnValue = QString("****");
                  break;
                };
                case 5:
                {
                  returnValue = QString("*****");
                  break;
                };
                default:
                {
                  returnValue = QString(" ");
                  break;
                };
              };
              break;
            };
            case comments_c:
            case imageID_c:
            {
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
          if (item.column() == quality_c)
          {
            returnValue = Qt::AlignCenter;
          };
          break;
        }
        case Qt::BackgroundRole:        // The background brush used for items rendered with the default delegate. (QBrush)
        {
          if (item.column() == quality_c)
          {
            switch(CSQLQueryModel::data(item, Qt::DisplayRole).toUInt())
            {
              case 1:
              {
                returnValue = QBrush(Qt::red);
                break;
              }
              case 2:
              {
                returnValue = QBrush(Qt::darkRed);
                break;
              }
              case 3:
              {
                returnValue = QBrush(Qt::darkGray);
                break;
              }
              case 4:
              {
                returnValue = QBrush(Qt::darkGreen);
                break;
              }
              case 5:
              {
                returnValue = QBrush(Qt::green);
                break;
              }
              default:
              {
                break;
              }
            }
            break;
          };
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

    void CSelectImageQueryModel::resetQuery()
    {
      setQuery(QString::fromStdString(sqlWriter_.string()), database::databaseARID->database());
    }

  } // namespace QTRE
} // namespace AstroManager
