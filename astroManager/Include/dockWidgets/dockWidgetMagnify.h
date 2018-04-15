//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetMagnify
// SUBSYSTEM:						Provides a magnified view of the current cursor position.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2018 Gavin Blakeman.
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
// OVERVIEW:            Provides a dock widget showing a magnified view of the current cursor position.
//
// CLASSES INCLUDED:    CMagnifyDockWidget
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CPhotometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//                        - CDockWidgetMagnify
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-05-19 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef DOCKWIDGETMAGNIFY_H
#define DOCKWIDGETMAGNIFY_H

#include "dockWidgetImage.h"

namespace AstroManager
{
  namespace dockwidgets
  {
    int const IDA_DW_MAGNIFY_11               = 0;
    int const IDA_DW_MAGNIFY_21               = 1;
    int const IDA_DW_MAGNIFY_41               = 2;
    int const IDA_DW_MAGNIFY_MAX              = 3;

    class CDockWidgetMagnify : public CDockWidgetImage
    {
      Q_OBJECT

    private:
      QGraphicsView *graphicsView;
      QGraphicsScene *graphicsScene;
      QMenu *popupMenu;
      QAction *menuActions[IDA_DW_MAGNIFY_MAX];

      void createActions();
      void createPopupMenu();

      void setupUI();

    protected:
      void mousePressEvent(QMouseEvent *);

    public:
      CDockWidgetMagnify(QWidget *, QAction *);
      virtual ~CDockWidgetMagnify();

      virtual void imageActivating();
      virtual void imageDeactivating();
      virtual void imageChanged();
      void updateCenter(QPointF const &);

      private slots:
        void eventMagnify11();
        void eventMagnify21();
        void eventMagnify41();
    };

  }  // namespace dockwidgets
}  // namespace AstroManager

#endif // DOCKWIDGETMAGNIFY_H
