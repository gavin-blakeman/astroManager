//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageComparison.h
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::imagedisplay
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CImageComparisonWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CAstroImageWindow
//                          - CImageComparisonWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef IMAGECOMPARISON_H
#define IMAGECOMPARISON_H

  // astroManager includes

#include "ACL/astroFile.h"
#include "astrometry/astrometryObservation.h"
#include "FrameWindow.h"
#include "windowImage/windowImageStacking.h"
#include "photometry/photometryObservation.h"
#include "qtExtensions/qt.h"

  // Other Headers

#include <ACL>
#include <MCL>

  // Standard Library

#include <string>

  // Boost Library

#include "boost/filesystem.hpp"
#include "boost/optional.hpp"

namespace astroManager
{
  namespace imaging
  {
    int const ROLE_CONTROLBLOCK   = Qt::UserRole + 7;

    struct SControlBlock
    {
      QString inputFilename = "";
      QString outputFilename = "";
      bool alignmentPointsComplete = false;
      bool outputImageValid = false;
      bool inputImageValid = false;
      imaging::SControlImage inputImage;
      imaging::SControlImage outputImage;
      boost::optional<MCL::TPoint2D<FP_t>> alignmentPoint1;
      boost::optional<MCL::TPoint2D<FP_t>> alignmentPoint2;
      ACL::CRegisterImageInformation registerImageInformation;

      SControlBlock(imaging::CAstroImageWindow *);
    };

    /// @brief Class for displaying the comparison images.
    /// The listWidgetImages member is used for storing the list of images. Each of the controlImage structures for the input image,
    /// output image and the transform data is stored as additional data on the data member of the list widget items.
    /// @version 2016-04-21/GGB - Added the SControlBlock class member and remove all the different roles. IE reduce the number of
    /// data items that need to be accessed and allow easier and better control over the data.

    class CImageComparisonWindow : public imaging::CAstroImageWindow
    {
      Q_OBJECT

    private:
      QStringList imageList;                      ///< Used to track the filenames that have been added to the list.
      QListWidget *listWidgetImages;
      QRadioButton *radioButtonBlink;
      QRadioButton *radioButtonSubtract;
      QRadioButton *radioButtonColour;
      QDoubleSpinBox *spinBoxInterval;
      QPushButton *pushButtonAdd;
      QPushButton *pushButtonRemove;
      QPushButton *pushButtonRemoveAll;
      QPushButton *pushButtonManual;
      QPushButton *pushButtonAutomatic;
      QPushButton *pushButtonAlign1;
      QPushButton *pushButtonAlign2;
      QPushButton *pushButtonPrepare;
      QLabel *labelAlign1;
      QLabel *labelAlign2;
      QTabWidget *tabWidget;
      QLabel *labelCurrentFile;

      QTimer *blinkTimer;

      QGraphicsScene *graphicsSceneImageInput;
      QGraphicsScene *graphicsSceneImageOutput;

      CAstroGraphicsView *graphicsViewImageInput;
      CAstroGraphicsView *graphicsViewImageOutput;

      QBitmap cursorAstrometry;
      QBitmap maskAstrometry;
      QCursor astrometry;

      imaging::SAlignGraphics alignPoint1;
      imaging::SAlignGraphics alignPoint2;

      bool bAllowSave;
      int imageNumber;

      void setupUI();

      virtual void windowActivating();

      virtual bool save();
      virtual bool saveAs();

      virtual void eventMousePress(QMouseEvent *);

      void toggleImageWidgets(bool);

      void displayAlign1(MCL::TPoint2D<ACL::FP_t> const &);
      void displayAlign2(MCL::TPoint2D<ACL::FP_t> const &);

      void clearImageList();

      void mousePressAstrometry(QMouseEvent *);
      void mousePressPhotometry(QMouseEvent *);

      astrometry::PAstrometryObservation astrometryAdd(imaging::SControlImage *, MCL::TPoint2D<ACL::FP_t> const &, QString const &);
      photometry::PPhotometryObservation photometryAdd(imaging::SControlImage *, MCL::TPoint2D<ACL::FP_t> const &, QString const &);

    protected:
      virtual void closeEvent(QCloseEvent *);

    public:
      CImageComparisonWindow(QWidget * = 0);
      virtual ~CImageComparisonWindow();

      virtual ESubWindowType getWindowType() const {return SWT_IMAGE_COMPARE;}

      virtual void setMode(EMode);

      virtual imaging::SControlImage *getControlImage();

      void redrawImage();
      virtual void repaintImage();

      void zoomAll();
      void zoomIn();
      void zoomOut();
      void zoomSelection();
      void zoom11();

        // Astrometry functions

      virtual void changeAstrometrySelection(astrometry::PAstrometryObservation) {}

        // Photometry functions

      virtual void changePhotometrySelection(photometry::PPhotometryObservation) {}

    private slots:
      void eventButtonAddImages(bool);
      void eventButtonRemoveImages(bool);
      void eventButtonImagesRemoveAll(bool);

      void eventButtonAlignment1(bool);
      void eventButtonAlignment2(bool);

      void eventRadioButtonBlinkImages(bool);
      void eventRadioButtonSubtractImages(bool);

      void eventButtonPrepareImages(bool);

      void signalItemClickedImageList(QListWidgetItem *);

      void eventTabCurrentChanged(int);

      void eventIntervalChange(double);

      void eventBlink();
      void eventButtonManual(bool);
      void eventButtonAutomatic(bool);
    };

  }  // namespace imaging
}  // namespace AstroManager

#endif  // IMAGECOMPARISON_H
