//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								MdiSubWindow
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2017 Gavin Blakeman.
//                      This file is part of the Astronomical Image Reduction and Data Analysis Software (AIRDAS)
//
//                      AIRDAS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
//                      License as published by the Free Software Foundation, either version 2 of the License, or (at your option)
//                      any later version.
//
//                      AIRDAS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
//                      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//                      more details.
//
//                      You should have received a copy of the GNU General Public License along with AIRDAS.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Implements the MDI Frame window for the application. There is a single class implemented:
//							          - class CFrameWindow
//
// CLASSES INCLUDED:    CMdiSubWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CMdiSubWindow
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-02-03 GGB - Moved code from FrameWindow
//                      2012-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

  // Qt Library

#include "qt.h"

namespace AstroManager
{
  enum ESubWindowType
  {
    SWT_NONE,
    SWT_FILE_SEARCH,
    SWT_IMAGEWINDOW,
    SWT_MASTERBIASWINDOW,
    SWT_MASTERDARKWINDOW,
    SWT_MASTERFLATWINDOW,
    SWT_CALIBRATION_MULTIPLE,
    SWT_STACKIMAGESWINDOW,
    SWT_CONFIGURE_SITES,
    SWT_CONFIGURE_OBSERVERS,
    SWT_CONFIGURE_INSTRUMENTS,
    SWT_CONFIGURE_TELESCOPES,
    SWT_CONFIGURE_CATALOGPREFS,
    SWT_TEXTEDITORWINDOW,
    SWT_TEXTEDITORFITSWINDOW,
    SWT_OBJECT_INFORMATION,
    SWT_WEATHER_HISTORY,
    SWT_IMAGE_COMPARE,
    SWT_CALC_GREG2JD,
    SWT_CALC_JD2GREG,
    SWT_UTILITY_PLANNING,
  };

  class CMdiSubWindow : public QMdiSubWindow
  {
    Q_OBJECT

  public:
    enum EWindowClass
    {
      WC_NULL,
      WC_IMAGE,
      WC_WEATHER,
      WC_ANALYSE,
      WC_CALIBRATE,
      WC_PLANNING,
    };

  protected:
    virtual void closeEvent(QCloseEvent *);

  public:
    CMdiSubWindow(QWidget *parent = 0);

    virtual ESubWindowType getWindowType() const = 0;
    virtual EWindowClass getWindowClass() const { return WC_NULL; }

    virtual bool save() {}
    virtual bool saveAs() {}
    virtual void clipboardCopy() {}

  public slots:
    virtual void windowActivating() = 0;
  };
}

#endif // MDISUBWINDOW_H
