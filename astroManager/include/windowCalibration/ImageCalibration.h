//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageCalibration.h
// SUBSYSTEM:						Classes to interface image calibration
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	libAstroImages
//                      Qt
// NAMESPACE:						AstroManager::imageCalibration
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
// CLASSES INCLUDED:    CCreateMasterDarkWindow - User interface window for creating a master dark frame
//                      CCreateMasterFlatWindow - User interface for creating a master flat frame.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CARPAASubWindow
//                          - CCreateMasterDarkWindow
//                          - CCreateMasterFlatWindow
//                          - CCreateMasterBiasWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-12 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef IMAGECALIBRATION_H
#define IMAGECALIBRATION_H

#include "include/qtExtensions/qt.h"
#include "windowCalibration.h"

namespace astroManager
{
  namespace calibration
  {
    class CCreateMasterDarkWindow : public CWindowCalibration
    {
      Q_OBJECT
    private:
      QStringList darkImageList;
      QStringList biasImageList;

      QString biasFileName;
      QString darkFileName;

      QString masterBiasSaveAs;

      virtual void closeEvent(QCloseEvent *);
      void setupUI();

      virtual bool save() {return false;}
      virtual bool saveAs() {return false;}

    protected:
    public:
      CCreateMasterDarkWindow(QWidget * = 0);

      virtual ESubWindowType getWindowType() const {return SWT_MASTERDARKWINDOW;}

    private slots:
      virtual void windowActivating();
      void eventSelectDarkFramesClicked(bool);
      void eventBtnRemoveImagesClicked(bool);

      void eventSelectMasterNameClicked(bool);

      void eventSelectBiasFrameClicked(bool);
      void eventSaveMasterBiasAsClicked(bool);

      void eventSelectBiasFramesClicked(bool);
      void eventRemoveBiasFramesClicked(bool);

      void eventGroupBoxMasterBiasFrameSelected(bool);
      void eventGroupBoxBiasFramesSelection(bool);

      void eventDarkListSelectionChanged();
      void eventBiasListSelectionChanged();

      void eventBtnCreateClicked(bool);

    };

    class CCreateMasterFlatWindow : public CWindowCalibration
    {
      Q_OBJECT
    private:
      QStringList flatImageList;
      QStringList darkImageList;
      QString flatFileName;
      QString darkFileName;
      QString darkSaveAs;

      virtual void closeEvent(QCloseEvent *);
      void setupUI();

      virtual bool save() {return false;}
      virtual bool saveAs() {return false;}

    protected:
    public:
      CCreateMasterFlatWindow(QWidget * = 0);

      virtual ESubWindowType getWindowType() const {return SWT_MASTERFLATWINDOW;}

    private slots:
      virtual void windowActivating();

      void eventSelectFlatFramesClicked(bool);
      void eventBtnRemoveFlatFramesClicked(bool);
      void eventSelectMasterFlatNameClicked(bool);

      void eventBtnSelectMasterDarkNameClicked(bool);

      void eventBtnAddDarkFramesClicked(bool);
      void eventBtnRemoveDarkFramesClicked(bool);

      void eventGroupBoxMasterDarkClicked(bool);
      void eventGroupBoxDarkFramesClicked(bool);

      void eventBtnSaveDarkAsClicked(bool);

      void eventFlatListSelectionChanged();

      void eventBtnCreateClicked(bool);
    };

    class CCreateMasterBiasWindow : public CWindowCalibration
    {
      Q_OBJECT
    private:
      QStringList biasImageList;

      QString masterBiasFileName;

      void setupUI();

      virtual bool save() {return false;};
      virtual bool saveAs() {return false;};

    protected:
    public:
      CCreateMasterBiasWindow(QWidget * = 0);

      virtual ESubWindowType getWindowType() const {return SWT_MASTERBIASWINDOW;};

    private slots:
      virtual void windowActivating();

      void eventSelectBiasFramesClicked(bool);
      void eventRemoveBiasFramesClicked(bool);
      void eventBiasListSelectionChanged();

      void eventSaveMasterBiasAs(bool);

      void eventBtnCreateClicked(bool);
    };

    class CImageCalibrationMultipleWindow : public CWindowCalibration
    {
      Q_OBJECT

    private:
      QStringList imagesList;
      QString masterDarkFrame;
      QString masterBiasFrame;
      QString masterFlatFrame;
      QString saveDirectory;

      virtual bool save() {return (false);};		// No function needed.
      virtual bool saveAs() {return (false);};

      virtual void windowActivating();
      virtual void closeEvent(QCloseEvent *);

      void setupUI();

    protected:
    public:
      CImageCalibrationMultipleWindow(QWidget * = 0);
      virtual ~CImageCalibrationMultipleWindow() {;};

      virtual ESubWindowType getWindowType() const {return SWT_CALIBRATION_MULTIPLE;};

    private slots:
      void eventSelectFilesClicked(bool);
      void eventRemoveFilesClicked(bool);
      void eventListSelectionChanged();

      void eventDarkFrameName(bool);
      void eventBiasFrameName(bool);
      void eventFlatFrameName(bool);

      void eventSaveDirectory(bool);

      void eventCalibrateImages(bool);
    };

  }  // namespace imagecalibration
}  // namespace AstroManager


#endif // IMAGECALIBRATION_H
