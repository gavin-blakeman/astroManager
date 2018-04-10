//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageResample
// SUBSYSTEM:						Dialog for entering data for resampling images.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2016 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CImageResampleDialog      - Allow entry of parameters for resampling an image.
//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-07-21/GGB - Renamed from CImageResampleDialog to DialogImageResample
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-27 GGB - Moved code from ImageDisplay
//
//*********************************************************************************************************************************

#ifndef DIALOGIMAGERESAMPLE_H
#define DIALOGIMAGERESAMPLE_H

#include "dialogs.h"

#include <ACL>

namespace AstroManager
{
  namespace dialogs
  {
    class CImageResampleDialog : public CDialog
    {
      Q_OBJECT

    private:
      bool bInternalCall;
      ACL::CAstroImage *astroImage;
      ACL::AXIS_t lWidth;
      ACL::AXIS_t lHeight;

      void setupUI();
      void saveUI();

    protected:
    public:
      CImageResampleDialog(ACL::CAstroImage *);

      inline ACL::AXIS_t getWidth() const { return lWidth;}
      inline ACL::AXIS_t getHeight() const { return lHeight;}

    private slots:
      void eventButtonContinue(bool);
      void eventButtonCancel(bool) { dlg->reject(); }

      void eventWidthValueChanged(int);
      void eventHeightValueChanged(int);
      void eventAspectRatioClicked(bool);
    };

  }  // namespace dialogs
}  // namespace AstroManager

#endif // DIALLOGIMAGERESAMPLE_H
