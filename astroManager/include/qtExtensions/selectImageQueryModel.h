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
// CLASSES INCLUDED:    CSelectImageQueryModel
//
// CLASS HIERARCHY:     QSqlQueryModel
//                        CSQLQueryModel
//                          CSelectImageQueryModel
//
// HISTORY:             2017-08-01/GGB - File Created
//
//*********************************************************************************************************************************

#ifndef SELECTIMAGEQUERYMODEL
#define SELECTIMAGEQUERYMODEL

#include "sqlQueryModel.h"

  // Miscellaneous libraries.

#include <GCL>

namespace astroManager
{
  namespace QTE
  {
    class CSelectImageQueryModel : public CSQLQueryModel
    {
      Q_OBJECT

    public:
      enum
      {
        target_c = 0,
        filter_c,
        observingSite_c,
        telescope_c,
        date_c,
        time_c,
        ra_c,
        dec_c,
        quality_c,
        comments_c,
        imageID_c,
      };

    private:
      CSelectImageQueryModel(CSelectImageQueryModel const &) = delete;
      GCL::sqlwriter::CSQLWriter sqlWriter_;

    protected:
    public:
      CSelectImageQueryModel();
      virtual ~CSelectImageQueryModel() {}

      virtual QVariant data(QModelIndex const &item, int role = Qt::DisplayRole) const;
      virtual void resetQuery();

      virtual GCL::sqlwriter::CSQLWriter &sqlWriter() { return sqlWriter_; }
    };

  } // namespace QTE
} // namespace AstroManager

#endif // SELECTIMAGEQUERYMODEL

