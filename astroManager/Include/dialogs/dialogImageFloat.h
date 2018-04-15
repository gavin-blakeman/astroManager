//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DialogImageFloat
// SUBSYSTEM:						Dialog Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman.
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
// OVERVIEW:						Implements a dialog for user input of the parameters for floating an image.
//
// CLASSES INCLUDED:    CDialogImageFloat         - Allow entry of parameters for floating an image.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//                          - CAstroImageWindow
//                      QDialog
//                        - CDialog
//                          - CDialogImageFloat
//                          - CImageResampleDialog
//                          - CBinPixelsDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-02-07 GGB - Moved CDialogImageCrop, CBinPixelsDialog, CImageFloatDialog into it's own file.
//
//*********************************************************************************************************************************

#ifndef DIALOGIMAGEFLOAT_H
#define DIALOGIMAGEFLOAT_H

#include "dialogs.h"

#include <ACL>

namespace AstroManager
{
  namespace dialogs
  {
    class CDialogImageFloat : public dialogs::CDialog
    {
      Q_OBJECT

    private:
      ACL::CAstroImage *astroImage;
      long lWidth;
      long lHeight;
      long  dBackgroundValue;

      void setupUI();
      void saveUI();

    protected:
    public:
      CDialogImageFloat(ACL::CAstroImage *);

      inline long getWidth() const {return lWidth;}
      inline long getHeight() const {return lHeight;}
      inline long getBackground() const {return dBackgroundValue;}

    private slots:
      void eventButtonContinue(bool);
      void eventButtonCancel(bool) { dlg->reject(); }

      void eventManualSelected(bool);
    };

  } // namespace dialogs
} // namespace AstroManager

#endif // DIALOGIMAGEFLOAT_H
