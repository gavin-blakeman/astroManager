//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								windowSelectImage
// SUBSYSTEM:						Subclass for selection of images from the database.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, boost
// NAMESPACE:						AstroManager
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
// CLASSES INCLUDED:    CWindowSelectImage
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        CMdiSubWindow
//                          CWindowSelectImage
//
// HISTORY:             2017-07-28/GGB - File Created
//
//*********************************************************************************************************************************

#ifndef WINDOWSELECTIMAGE
#define WINDOWSELECTIMAGE

#include "../Include/qtExtensions/selectImageQueryModel.h"
#include "../Include/qtExtensions/MdiSubWindow.h"

namespace AstroManager
{
  class CWindowSelectImage : public CMdiSubWindow
  {
    Q_OBJECT

  private:
    QTableView *tableViewImages;
    QPushButton *pushButtonOpenImage;
    QPushButton *pushButtonEditData;
    QPushButton *pushButtonRefreshData;

    QTE::CSelectImageQueryModel queryModel;

    void setupUI();
  protected:
  public:
    CWindowSelectImage(QWidget *parent = 0);

    virtual ESubWindowType getWindowType() const { return SWT_FILE_SEARCH; }

  public slots:
    virtual void windowActivating() {}

    virtual void eventRowDoubleClick(const QModelIndex &);
    virtual void eventRowActivated(const QModelIndex &);
    virtual void eventOpenImage(bool);
    virtual void eventEditImageData(bool);
    virtual void eventRefreshData(bool);\
  };

} // namespace AstroManager

#endif // WINDOWSELECTIMAGE

