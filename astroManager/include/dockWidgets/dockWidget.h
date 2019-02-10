//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidget
// SUBSYSTEM:						The dockWidget class derived from QDockWidget
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
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
// CLASSES INCLUDED:    CDockWidget
//
// CLASS HIERARCHY:     QDockWidget
//                        - CDockWidget
//                          - CDockWidgetImage
//                            - CAstrometryDockWidget
//                            - CPhotometryDockWidget
//                            - CInstrumentDockwidget
//                            - CMessageWidget
//                            - CDockWidgetMagnify
//                          - CDockWidgetWeather
//                            - CDockWidgetWeatherScale
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

#ifndef DOCKWIDGET
#define DOCKWIDGET

  // astroManager header files

#include "../qtExtensions/MdiSubWindow.h"

  // Miscellaneous library header files

#include <QCL>

namespace astroManager
{
  namespace dockwidgets
  {
    class CDockWidget : public QDockWidget
    {
      Q_OBJECT

    private:
      QAction *dockWidgetAction;
      QString visibleSettingString;

    protected:
      virtual void closeEvent(QCloseEvent *);
      virtual void hideEvent(QHideEvent *);
      virtual void showEvent(QShowEvent *);

    public:
      CDockWidget(QString const &, QWidget *, QAction *, QString const &);

      virtual void mdiWindowActivating(CMdiSubWindow *) = 0;

    };

  } // namespace dockwidgets
} // namespace astroManager

#endif // DOCKWIDGET

