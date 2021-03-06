﻿//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								DialogBinPixels
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imagedisplay
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2011-2020 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CBinPixelsDialog          - Allows selection of binning size for images.
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-07 GGB - Moved CCBinPixelsDialog into it's own file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file.
//                      2013-01-20 GGB - Release of astroManager 0000.00
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef DIALOGBINPIXELS_H
#define DIALOGBINPIXELS_H

  // Miscellaneous library header files

#include <QCL>

namespace astroManager
{
  namespace dialogs
  {
    class CBinPixelsDialog : public QCL::CDialog
    {
      Q_OBJECT

    private:
      int nsize;

      void setupUI();
      void saveUI();

    protected:
    public:
      CBinPixelsDialog();

      inline long getBinSize() const { return nsize;}

    private slots:
      void eventButtonContinue(bool);
      void eventButtonCancel(bool) { dlg->reject(); }
    };

  } // namespace dialogs
} // namespace AstroManager

#endif // DIALOGBINPIXELS_H
