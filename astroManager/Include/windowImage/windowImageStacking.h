//*************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								ImageStacking.h
// SUBSYSTEM:						Image/Image File Viewing and Manipulation Classes
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, CFitsIO, gsl.
// NAMESPACE:						AstroManager::imagingstacking
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
// OVERVIEW:						Implements the classes for displaying and analysing images.
//
// CLASSES INCLUDED:    CStackImagesWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CARPAASubWindow
//                          - CAstroImageWindow
//                            - CStackImagesWindow
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2011-06-11 GGB - Development of classes for AIRDAS
//
//*************************************************************************************************

#ifndef WINDOWIMAGESTACKING_H
#define WINDOWIMAGESTACKING_H

  // AIRDAS header files

#include "../astrometry/astrometryObservation.h"
#include "windowImage.h"
#include "../imaging/imageControl.h"
#include "../photometry/photometryObservation.h"
#include "../qtExtensions/qt.h"

namespace AstroManager
{
  namespace imaging
  {
      // Struct used for keeping track of the alignment point drawing components.

    struct SAlignGraphics
    {
    public:
      QGraphicsEllipseItem *ellipse;
      QGraphicsSimpleTextItem *text;

      SAlignGraphics() : ellipse(nullptr), text(nullptr) {}
    };

    class CStackImagesWindow : public CAstroImageWindow
    {
      Q_OBJECT

    private:
      std::vector<imageID_t> imageIDList;
      QStringList imageList;
      std::string darkFrameFilename;
      std::string flatFrameFilename;
      std::string biasFrameFilename;

      imaging::CAstroGraphicsView *gvImageInput;
      imaging::CAstroGraphicsView *gvImageOutput;
      QGraphicsScene *gsImageInput;
      QGraphicsScene *gsImageOutput;

      QTabWidget *tabWidget;
      QListWidget *listImages;
      QPushButton *pbOpenInWindow;
      QLabel *labelCount;
      QLabel *labelAlign1, *labelAlign2;
      QRadioButton *radioMeanCombine, *radioMedianCombine;
      QPushButton *btnAlign1, *btnAlign2;
      QPushButton *btnImageRemoveAll;
      QPushButton *btnImageRemove;
      QToolButton *toolButtonAddImages;
      QPushButton *pushButtonAutoProcess;

      QBitmap cursorAstrometry;
      QBitmap maskAstrometry;
      QCursor astrometry;

      QAction *actionSelectFromFolder;
      QAction *actionSelectFromDatabase;

      QMenu *addImagesMenu;

      ACL::CImageStack imageStack;

      imaging::SControlImage outputControlImage;
      bool outputControlImageValid_ = false;

      bool isDirty;						// true if the resulting image has not been saved.
      std::string fileName;		// Filename of the resulting stacked images. Used by save and saveAs

      SAlignGraphics alignPoint1;
      SAlignGraphics alignPoint2;

      void setupUI();
      virtual bool save();
      virtual bool saveDatabase();
      virtual bool saveAs();

      virtual void windowActivating();

      void toggleImageWidgets(bool);

      void displayAlign1(const QPointF &);
      void displayAlign2(const QPointF &);
      void displayAlignmentGraphics(QListWidgetItem *);
      void removeAlignmentGraphics();

      void clearImageList() noexcept;
      void deleteListItem(QListWidgetItem *);
      imaging::SControlImage *loadImage(QListWidgetItem *);
      void stackImages();

      void createActions();

    protected:
      virtual void closeEvent(QCloseEvent *);

    public:
      CStackImagesWindow(QWidget * = 0);
      virtual ~CStackImagesWindow();

      virtual ESubWindowType getWindowType() const {return SWT_STACKIMAGESWINDOW;}

      virtual void setMode(EMode) {}

      void eventMousePress(QMouseEvent *);
      virtual void repaintImage();

         // Zoom functions

      void zoomAll();
      void zoomIn();
      void zoomOut();
      void zoomSelection();
      void zoom11();

      virtual imaging::SControlImage *getControlImage();

        // Astrometry functions

      virtual void changeAstrometrySelection(astrometry::PAstrometryObservation) {}

        // Photometry functions

      virtual void changePhotometrySelection(photometry::PPhotometryObservation) {}

    private slots:
      void eventButtonAddImagesFromFolder(bool);
      void eventButtonAddImagesFromDatabase(bool);
      void eventButtonImageRemove(bool);
      void eventButtonImageRemoveAll(bool);

      void eventButtonSelectBias(bool);
      void eventButtonSelectDark(bool);
      void eventButtonSelectFlat(bool);

      void eventButtonAlignment1(bool);
      void eventButtonAlignment2(bool);

      void signalItemClickedImageList(QListWidgetItem *);

      void eventButtonCombineImages(bool);
      void eventButtonAutoStack(bool);
      void eventOpenInWindow(bool);

      void eventTabChanged(int);
    };

  }  // namespace imagestacking
}  // namespace AstroManager

#endif // WINDOWIMAGESTACKING_H
