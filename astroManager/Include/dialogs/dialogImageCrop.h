﻿//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageCrop
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman.
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
// CLASSES INCLUDED:    CDialogImageCrop -Dialog for setting cropping of images.

//
// CLASS HIERARCHY:     QDialog
//                        - CDialog
//                          - CImageFloatDialog
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-02-07 GGB - Moved dialog CDailogImageCrop into seperate file.
//                      2013-01-27 GGB - Moved CImageResampleDialog into it's own file
//                      2013-01-20 GGB - Release of AIRDAS 0000.00
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef AIRDAS_DIALOGIMAGECROP_H
#define AIRDAS_DIALOGIMAGECROP_H

#include "dialogs.h"

#include <ACL>
#include <MCL>

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogImageCrop : public dialogs::CDialog
    {
      Q_OBJECT

    private:
      MCL::TPoint2D<ACL::AXIS_t> &origin;
      MCL::TPoint2D<ACL::AXIS_t> &dims;

      QSpinBox *spinBoxXO;
      QSpinBox *spinBoxYO;
      QSpinBox *spinBoxXD;
      QSpinBox *spinBoxYD;

      QCheckBox *checkBoxAspectRatio;

      void setupUI();

    protected:
    public:
      CDialogImageCrop(MCL::TPoint2D<ACL::AXIS_t>&, MCL::TPoint2D<ACL::AXIS_t> &);

    private slots:
      void btnOk(bool);
      void btnCancel(bool) { dlg->reject(); }

      void eventXDChanged();
      void eventYDChanged();
    };

  } // namespace dialogs

}   // namespace AstroManager

#endif // AIRDAS_DIALOGIMAGECROP_H
