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
// CLASSES INCLUDED:    CSelectImageVersionQueryModel
//
// CLASS HIERARCHY:     QSqlQueryModel
//                        CSQLQueryModel
//                          CSelectImageVersionQueryModel
//
// HISTORY:             2017-08-12/GGB - File Created
//
//*********************************************************************************************************************************

#ifndef SELECTIMAGEVERSIONQUERYMODEL
#define SELECTIMAGEVERSIONQUERYMODEL

#include "sqlQueryModel.h"
#include "../astroManager.h"

namespace astroManager
{
  namespace QTE
  {
    class CSelectImageVersionQueryModel : public CSQLQueryModel
    {
      Q_OBJECT

    private:
      database::imageID_t imageID_;


      CSelectImageVersionQueryModel(CSelectImageVersionQueryModel const &) = delete;

    protected:
    public:
      CSelectImageVersionQueryModel(database::imageID_t, QObject * = nullptr);
      virtual ~CSelectImageVersionQueryModel() {}

      virtual QVariant data(QModelIndex const &item, int role = Qt::DisplayRole) const;
      virtual void resetQuery();
    };

  } // namespace QTE
} // namespace AstroManager

#endif // SELECTIMAGEVERSIONQUERYMODEL

