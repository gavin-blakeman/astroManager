//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dockWidgetNavigator
// SUBSYSTEM:						Provides an overview of the current viewport on the image.
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
// CLASSES INCLUDED:    CDockwidgetNavigator
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CPhotometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//                        - CDockWidgetMagnify
//                        - CDockwidgetNavigator
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-05-23 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef DOCKWIDGETNAVIGATOR_H
#define DOCKWIDGETNAVIGATOR_H

#include "dockWidgetImage.h"

namespace astroManager
{
  namespace dockwidgets
  {
    class CDockWidgetNavigator : public CDockWidgetImage
    {
    private:
      QGraphicsView *graphicsView;
      QGraphicsScene *graphicsScene;
      QGraphicsRectItem *viewRectangle;

      void setupUI();

    protected:

    public:
      CDockWidgetNavigator(QWidget *, QAction *);
      virtual ~CDockWidgetNavigator();

      virtual void mdiWindowActivating(CMdiSubWindow *) override;
      virtual void imageChanged();
      void updateLimits(QRectF const &limits);

      virtual void setEnabled(bool);
    };
  }  // namespace dockwidgets
}  // namespace AstroManager

#endif // DOCKWIDGETNAVIGATOR_H
