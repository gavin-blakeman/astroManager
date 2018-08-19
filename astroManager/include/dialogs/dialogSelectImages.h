//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogSelectImages
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
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
// OVERVIEW:            Dialog for selecting an image version to downLoad
//
// CLASSES INCLUDED:    CDialogSelectImageVersion
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogSelectImages
//
// HISTORY:             2017-08-19 GGB - File created
//
//*********************************************************************************************************************************

#ifndef DIALOGSELECTIMAGES
#define DIALOGSELECTIMAGES

#include "dialogs.h"
#include "../qtExtensions/selectImageQueryModel.h"
#include "../qtExtensions/qt.h"
#include "../astroManager.h"

  // Standard C++ headers

#include <cstdint>
#include <vector>

namespace astroManager
{
  namespace dialogs
  {
    class CDialogSelectImages : public CDialog
    {
      Q_OBJECT

    private:
      bool selectMultiple;
      std::vector<imageID_t> &imageList_;

      QTableView *tableViewImages;
      QGroupBox *groupBoxTarget;
        QComboBox *comboBoxTarget;
      QGroupBox *groupBoxFilter;
        QComboBox *comboBoxFilter;
      QGroupBox *groupBoxObservingSite;
        QComboBox *comboBoxObservingSite;
      QGroupBox *groupBoxTelescope;
        QComboBox *comboBoxTelescope;
      QGroupBox *groupBoxDate;
        QDateEdit *dateEditStart;
        QDateEdit *dateEditEnd;
      QGroupBox *groupBoxQuality;
        QListWidget *listWidgetQuality;

      QPushButton *pushButtonSelectImages;
      QPushButton *pushButtonCancel;
      QPushButton *pushButtonRefreshData;
      QPushButton *pushButtonApplyFilters;

      QTE::CSelectImageQueryModel queryModel;

      void populateFilters();
      void populateSites();
      void populateTargets();
      void populateTelescopes();
      void setDates();

    protected:
      void setupUI();

    public:
      CDialogSelectImages(QObject *, std::vector<imageID_t> &);
      virtual ~CDialogSelectImages() {}

    public slots:
      void eventPushButtonCancel(bool) { dlg->reject(); }
      void eventPushButonSelectImages(bool);
      void eventPushButtonRefreshData(bool);

      void eventUpdateFilterString(int);
      void eventUpdateFilterString(bool);

    };
  } // namespace dialogs
} // namespace AstroManager

#endif // DIALOGSELECTIMAGES
