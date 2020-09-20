//*********************************************************************************************************************************
//
// PROJECT:		          AstroManager (Astronomy Observation Manager)
// FILE:                DialogExportAsJPEG
// SUBSYSTEM:						Dialog to allow the user to select some JPEG save parameters when exporting as JPEG.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						AstroManager::dialogs
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
// OVERVIEW:						Implements a dialog class to allow the user to select some JPEG parameters when an image is being exported
//                      as JPEG.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-07-21 GGB - File created.
//
//*********************************************************************************************************************************

#ifndef DIALOGEXPORTASJPEG_H
#define DIALOGEXPORTASJPEG_H

// Miscellanous library header files

#include <QCL>

#include <ACL>

namespace astroManager
{
  namespace dialogs
  {
    class CDialogExportAsJPEG : public QCL::CDialog
    {
      Q_OBJECT

    private:
      QCheckBox *checkBoxAspectRatio;
      QSpinBox *spinBoxImageWidth;
      QSpinBox *spinBoxImageHeight;
      QSpinBox *spinBoxImageQuality;
      QDialogButtonBox *buttonBox;

      bool bInternalCall;
      ACL::AXIS_t &imageWidth, &imageHeight;
      int &imageQuality;
      double aspectRatio;

      void setupUI();

    protected:
    public:
      CDialogExportAsJPEG(ACL::AXIS_t &, ACL::AXIS_t &, int &);

    private slots:
      void save();
      void eventWidthValueChanged(int);
      void eventHeightValueChanged(int);
      void eventAspectRatioClicked(bool);
    };

  }   // namespace dialogs
}   // namespace AstroManager

#endif // DIALOGEXPORTASJPEG_H
