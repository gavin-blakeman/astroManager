//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgets.h
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CAstrometryDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-09-30 GGB - AIRDAS 2013.09 release.
//                      2013-03-17 GGB - Removed CImageControlWidget into file DockWidgetImageControl.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into it's own file
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - Release 0000.00.000 of AIRDAS
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#include "../../Include/dockWidgets/dockWidgetImageInformation.h"

#include "../../Include/Settings.h"

  // Qt Library

#include <QVector>
#include <QtUiTools/QtUiTools>

namespace AstroManager
{
  namespace dockwidgets
  {
  //*****************************************************************************************************************************
    //
    // CImageControlWidget
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class. Calls the function to setup the user interface.
    /// @param[in] parent - Pointer to the parent object
    /// @param[in] action - The action associated with the control
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of parameters. Updated class to suit weather controls.
    /// @version 2013-07-27/GGB - Added objectName to allow restoreState() to operate correctly.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-02-02/GGB - Split black/white point and histogram into it's own class.
    /// @version 2011-05-28/GGB - Function created

    CImageControlWidget::CImageControlWidget(QWidget *parent, QAction *action)
      : CDockWidgetImage(DW_INFORMATION_NAME, parent, action, settings::DW_IMAGE_INFORMATION_VISIBLE)
    {
      setupUI();
      setObjectName(DW_INFORMATION_NAME);
    }

    /// @brief Deactivates the passed Control Image.
    /// @param[in] enabledValue - value to apply to enable the dockwidget.
    /// @throws None.
    /// @version 2013-06-22/GGB - Changed function to setEnabled.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-05-29/GGB - Function created.

    void CImageControlWidget::setEnabled(bool enabledValue)
    {
      CDockWidget::setEnabled(enabledValue);
    }

    /// @brief This function must be called when the owner of a controlImage object changes the image in the controlImage object.
    ///        This will then re-read all the data associated with the controlImage and update all the fields to ensure that only
    ///        valid information is displayed.
    /// @throws None.
    /// @version 2013-07-13/GGB - Removed min, max, mean from SControlImage. Changed label lookups to variables.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-03-09/GGB - Use imageMin, imageMax, imageHeight, imageWidth and imageMean methods of CAstroFile.
    /// @version 2013-02-02/GGB - Moved all white/black point data to CHistogram
    /// @version 2012-07-18/GGB - Use boost::optional for the black point and white point.
    /// @version 2011-06-05/GGB - Function created.

    void CImageControlWidget::imageChanged()
    {
      if (currentImage)
      {
          // Prepare the output values.

        labelMinimum->setText(QString("%1").arg(currentImage->astroFile->imageMin(currentImage->currentHDB)));
        labelMaximum->setText(QString("%1").arg(currentImage->astroFile->imageMax(currentImage->currentHDB)));
        labelMean->setText(QString("%1").arg(currentImage->astroFile->imageMean(currentImage->currentHDB)));
        labelDimX->setText(QString("%1").arg(currentImage->astroFile->imageWidth(currentImage->currentHDB)));
        labelDimY->setText(QString("%1").arg(currentImage->astroFile->imageHeight(currentImage->currentHDB)));
      };
    }

    /// @brief Sets up the UI and all UI associated variable. Performs all connections.
    /// @throws GCL::CRuntimeAssert(...)
    /// @version 2017-07-10/GGB - Fixed Bug #90.
    /// @version 2017-07-02/GGB - Updated all findChild to use ASSOCIATE_CONTROL
    /// @version 2013-07-13/GGB - Added variables for labels and added check for labels being created.
    /// @version 2013-06-22/GGB - Changed qFindChild() to findChild()
    /// @version 2013-02-14/GGB - Added support for displaying the RA/DEC when WCS data is present.
    /// @version 2011-05-28/GGB - Function created

    void CImageControlWidget::setupUI()
    {
      QUiLoader loader;

      QFile file(":/forms/dwImageControl.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to open resource :/forms/dwImageControl.ui.");
        ERROR(AIRDAS, 0x0001);
      }

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);      

      ASSOCIATE_LABEL(tlCoords, formWidget, "tlCoords");
      ASSOCIATE_LABEL(tlPV, formWidget, "tlPV");
      ASSOCIATE_LABEL(labelRA, formWidget, "labelRA");
      ASSOCIATE_LABEL(labelDEC, formWidget, "labelDEC");
      ASSOCIATE_LABEL(labelMinimum, formWidget, "labelMinimum");
      ASSOCIATE_LABEL(labelMaximum, formWidget, "labelMaximum");
      ASSOCIATE_LABEL(labelMean, formWidget, "labelMean");
      ASSOCIATE_LABEL(labelDimX, formWidget, "labelDimX");
      ASSOCIATE_LABEL(labelDimY, formWidget, "labelDimY");
    }

    /// @brief Called when the widget must update the values shown in the window.
    /// @details This would typically be after the AstroFile has been changed.
    /// @throws None.
    /// @version 2013-07-13/GGB - Changed label lookups to variables and removed SControlImage::(min, mean, max)
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-05-28/GGB - Created function

    void CImageControlWidget::updateMinMaxValues()
    {
      QString szValue;

      if (currentImage)
      {
        if (currentImage->astroFile)
        {
          szValue = QString("%1").arg(currentImage->astroFile->getAstroImage(currentImage->currentHDB)->getMin());
          labelMinimum->setText(szValue);

          szValue = QString("%1").arg(currentImage->astroFile->getAstroImage(currentImage->currentHDB)->getMax());
          labelMaximum->setText(szValue);

          szValue = QString("%1").arg(currentImage->astroFile->getAstroImage(currentImage->currentHDB)->getMean());
          labelMean->setText(szValue);
        };
      };
    }

    /// @brief Called when a connected class wants to update the coordinate and PV values.
    /// @throws None.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-02-14/GGB - Added support for RA/DEC display.
    /// @version 2011-05-29/GGB - Function created.

    void CImageControlWidget::updateValues()
    {
      if (currentImage)
      {
        tlCoords->setText(currentImage->szCoords);
        tlPV->setText(currentImage->szPV);
        labelRA->setText(currentImage->szRA);
        labelDEC->setText(currentImage->szDEC);
      };
    }

  }	// namespace dockwidgets
}	// namespace AstroManager
