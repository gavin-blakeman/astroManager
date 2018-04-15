//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								FileBatchConvert
// SUBSYSTEM:						File | Batch Convert - Menu Handler
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::file
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
//                      This file is part of the Astronomy Manager software(astroManager)
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
// OVERVIEW:            Implements the File | Batch convert menu option. Allows a number of files to be opened and converted to
//                      FITS, with attributes being applied and the file being registered.
//						          Allows the user to enter all relevant information using a dialog and then starts a thread running to
//                      perform the conversions. Once the thread is running, the status of the thread is shown on the main window,
//                      and the user can kill the thread (and conversion) if required.
//
// CLASSES INCLUDED:    CARPAASubWindow
//                      CFrameWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        -CARPAASubWindow
//
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2012-01-23 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef FILEBATCHCONVERT_H
#define FILEBATCHCONVERT_H

  // AIRDAS include files

#include "dialogs/dialogs.h"

  // Qt Library

#include <Qt>

namespace AstroManager
{
  namespace file
  {
    class CFileBatchConvertDialog : public dialogs::CDialog
    {
      Q_OBJECT

    private:
      QListWidget *listWidgetFiles;
      QLabel *labelOutputDirectory;
      QGroupBox *groupBoxObservationSite;
        QRadioButton *radioButtonSiteDefault;
        QRadioButton *radioButtonSiteManual;
        QPushButton *pushButtonEnterSite;
      QGroupBox *groupBoxWeather;
        QRadioButton *radioButtonWeatherDatabase;
        QRadioButton *radioButtonWeatherManual;
        QPushButton *pushButtonWeatherEnter;
      QGroupBox *groupBoxObserver;
        QRadioButton *radioButtonObserverDefault;
        QRadioButton *radioButtonObserverManual;
        QPushButton *pushButtonObserverSelect;

      QStringList imageList;
      QVariant siteID;

      void setupUI();

    protected:
    public:
      CFileBatchConvertDialog();

    private slots:
      void eventSelectDirectory(bool);
      void eventSelectFiles(bool);
      void eventRemoveFiles(bool);
      void eventConvertImages(bool);
      void eventEnterSite(bool);
      void eventEnterWeather(bool);
      void eventEnterObserver(bool);
      void radioButtonSiteManualClicked(bool);
      void radioButtonSiteDefaultClicked(bool);
      void radioButtonWeatherDatabaseClicked(bool);
      void radioButtonWeatherManualClicked(bool);
      void radioButtonObserverDefaultClicked(bool);
      void radioButtonObserverManualClicked(bool);
    };

  }  // namespace file
}  // namespace AstroManager

#endif // FILEBATCHCONVERT_H
