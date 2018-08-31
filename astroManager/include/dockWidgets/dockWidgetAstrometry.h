//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetAstrometry
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CAstrometryDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into this file
//
//*********************************************************************************************************************************

#ifndef DOCKWIDGETASTROMETRY_H
#define DOCKWIDGETASTROMETRY_H

#include "../astrometry/astrometryObservation.h"
#include "dockWidgetImage.h"
#include "../FrameWindow.h"

  // Standard libraries

#include <memory>

  // libAstroImages Library

#include <ACL>
#include <QCL>

namespace astroManager
{
  namespace dockwidgets
  {
      /// Dock widget that controls direct astrometry on an image.

      /// This dock widget allows the user to select references and targets on the image, calculate the plate constants,
      /// show the template for further stellar objects within the field.
      /// The dock widget automatically writes the information into the astroFile using the astrometry functions. When
      /// The user wishes to calculate plate constants, or observed positions of target objects, the astrometry functions of the
      /// CAstroFile class are also used.

    class CAstrometryDockWidget: public CDockWidgetImage
    {
      Q_OBJECT

    private:
      QTableWidget *tableWidgetAstrometry;
      QPushButton *pushButtonReferenceSelect;
      QPushButton *pushButtonReferenceEdit;
      QPushButton *pushButtonReferenceDelete;
      QPushButton *pushButtonObjectInformation;
      QPushButton *pushButtonPlateConstants;
      QLabel *labelObjectName;
      QLabel *labelCCDCoordinates;
      QLabel *labelObjectRA;
      QLabel *labelObjectDec;
      QLabel *labelObjectType;

      mdiframe::CFrameWindow *parentObject;

      bool bReference;

      void setupUI();

      void redraw();    // Redraws all the information in the window.

    protected:

      void insertRow(int, astrometry::PAstrometryObservation);

    public:
      CAstrometryDockWidget(QWidget *, QAction *);

      void imageActivating();
      void imageDeactivating();
      virtual void imageChanged();

      virtual void setEnabled(bool);

      void referenceCompleted(astrometry::PAstrometryObservation);
      void addNewObject(astrometry::PAstrometryObservation);
      void displayAstrometry(astrometry::PAstrometryObservation);

    private slots:
      void eventButtonReferenceSelect(bool);
      void eventTableRowReferenceSelected(int, int);
      void eventButtonReferenceDelete(bool);

      void eventAssociateObject(bool);
      void eventPlateConstants(bool);
    };

  }  // namespace dockwidgets
}  // namespace AstroManager

#endif // DOCKWIDGETASTROMETRY_H
