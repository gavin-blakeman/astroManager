//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								PhotometryDockWidgets
// SUBSYSTEM:						The dock widget for single image photometry.
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
//
// OVERVIEW:
//
// CLASSES INCLUDED:    CPhotometryDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CPhotometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - astroManager 0000.00.000 release.
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//
//*********************************************************************************************************************************

#include "../../include/dockWidgets/dockWidgetPhotometry.h"

  // astroManager include files

#include "../../include/database/database.h"
#include "../../include/database/databaseATID.h"
#include "../../include/dialogs/dialogSelectObject.h"
#include "../../include/ImageComparison.h"
#include "../../include/windowImage/windowImageDisplay.h"
#include "../../include/Settings.h"
#include "../../include/astroManager.h"

  // Boost Library

#include "boost/algorithm/string.hpp"

  // Qt Library

#include <QtUiTools/QtUiTools>

  // Qwt Library

#include <qwt_symbol.h>

namespace astroManager
{
  namespace dockwidgets
  {

    int const ROLE_OBJECINDEX_t  = Qt::UserRole + 0;

    //*****************************************************************************************************************************
    //
    // CPhotometryDockWidget
    //
    //*****************************************************************************************************************************

    /// @brief Class constructor.
    /// @param[in] parent - The parent object
    /// @param[in] action - The action object.
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of parameters and updated hierarchy.
    /// @version 2013-07-27/GGB - Added objectName to support restoreState.
    /// @version 2013-05-10/GGB - Removed support for the objectStore.
    /// @version 2013-03-28/GGB - Added the objectStore.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-06-04/GGB - Function created.

    CPhotometryDockWidget::CPhotometryDockWidget(QWidget *parent, QAction *action)
      : CDockWidgetImage(DW_PHOTOMETRY_NAME, parent, action, settings::DW_IMAGE_PHOTOMETRY_VISIBLE)
    {
      parentObject = dynamic_cast<mdiframe::CFrameWindow *>(parent);

      // Load the radii from the registry

      uiRadius1 = settings::astroManagerSettings->value(settings::PHOTOMETRY_RADIUS1, QVariant(5)).toUInt();
      uiRadius2 = settings::astroManagerSettings->value(settings::PHOTOMETRY_RADIUS2, QVariant(7)).toUInt();
      uiRadius3 = settings::astroManagerSettings->value(settings::PHOTOMETRY_RADIUS3, QVariant(10)).toUInt();

      setupUI();
      setObjectName(DW_PHOTOMETRY_NAME);
    }

    /// @brief Function to plot the profile of the selected object.
    /// @param[in] centroid - The centroid of the object.
    //
    // 2013-05-18/GGB - Function created.

    void CPhotometryDockWidget::drawProfile(MCL::TPoint2D<ACL::FP_t> const &centroid)
    {
      std::vector<boost::tuple<ACL::FP_t, ACL::FP_t> > data;
      QVector<FP_t> xData;
      QVector<FP_t> yData;
      FP_t fMax = -1e9, fMin = 1e9;
      std::vector<boost::tuple<ACL::FP_t, ACL::FP_t> >::const_iterator iterator;

      currentImage->astroFile->objectProfile(0, centroid, uiRadius2, data);

      for (iterator = data.begin(); iterator!= data.end(); iterator++)
      {
        xData.push_back( (*iterator).get<0>() );
        yData.push_back( (*iterator).get<1>() );

        fMax = std::max((*iterator).get<1>(), fMax);
        fMin = std::min((*iterator).get<1>(), fMin);
      };

      profilePlot->setAxisScale( QwtPlot::xBottom, 0.0, uiRadius2 );
      profilePlot->setAxisScale( QwtPlot::yLeft, fMin, fMax);
      profileCurve->setSamples(xData, yData);
      profilePlot->replot();
    }

    /// Function called when the image is activating. IE, the image has not changed, but simply requires to be activated.
    //

    void CPhotometryDockWidget::imageActivating()
    {
    }

    /// @brief Function called when a new image is activated
    /// @throws None.
    /// @version 2013-08-18/GGB - Added support for ZMAG.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2012-11-09/GGB - Function created.

    void CPhotometryDockWidget::imageChanged()
    {
      if (currentImage)
      {
        if (currentImage->astroFile->keywordExists(currentImage->currentHDB, ACL::HEASARC_FILTER))
        {
          std::string data, comment;

          currentImage->astroFile->keywordData(currentImage->currentHDB, ACL::HEASARC_FILTER, data, comment);

          labelFilter->setText(QString::fromStdString(data));

          boost::to_upper(data);

          if (data == "U")
            labelFilter->setStyleSheet("QLabel { color: violet }");
          else if (data == "B")
            labelFilter->setStyleSheet("QLabel { color: blue }");
          else if (data == "V")
            labelFilter->setStyleSheet("QLabel { color: green }");
          else if (data == "R")
            labelFilter->setStyleSheet("QLabel { color: red }");
          else if (data == "I")
            labelFilter->setStyleSheet("QLabel { color: darkred }");
          else if (data == "R")
            labelFilter->setStyleSheet("QLabel { color: red }");
          else if (data == "G")
            labelFilter->setStyleSheet("QLabel { color: green }");
          else if (data == "B")
            labelFilter->setStyleSheet("QLabel { color: blue }");
          else if (data == "CLEAR")
            labelFilter->setStyleSheet("QLabel { color: black }");
          else
            labelFilter->setStyleSheet("QLabel { color: black }");
        }
        else
        {
          labelFilter->setText("Not specified");
          labelFilter->setStyleSheet("QLabel { color : gray");
        };

        if (currentImage->astroFile->keywordExists(currentImage->currentHDB, ACL::astroManager_ZMAG))
        {
          FP_t zmag = static_cast<FP_t>(currentImage->astroFile->keywordData(currentImage->currentHDB, ACL::astroManager_ZMAG));
          labelZMAG->setText(QString("%1").arg(zmag));
        }
        else
        {
          labelZMAG->setText("0");
        }

        redraw();
      };
    }

    /// @brief Adds a new object into the object list.
    //
    // 2013-05-12/GGB - Removed support for export csv button.
    // 2013-05-10/GGB - Removed support for the objectStore.
    // 2013-03-28/GGB - Added support for the objectStore.
    // 2012-11-11/GGB - Function created.

    void CPhotometryDockWidget::addNewObject(photometry::PPhotometryObservation newObject)
    {
      // Add the object to the table view.

      insertRow(tableWidgetPhotometry->rowCount(), newObject);
    }

    /// @brief Function called when an image is deactivated.
    //
    /// @version 2017-09-23/GGB - Updated to use CAngle.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2012-11-09/GGB - Function created.

    void CPhotometryDockWidget::imageDeactivating()
    {
      redraw();
    }

    /// @brief Displays all the information for a selected object.
    /// @param[in] po - The photometry observation to display the data for.
    /// @throws CCodeError(astroManager)
    /// @version 2012-11-10/GGB - Function created.

    void CPhotometryDockWidget::displayPhotometry(photometry::PPhotometryObservation po)
    {
      TRACEENTER;

      ACL::CHDB *currentHDB;
      FP_t zmag;

      if (!po)
      {
        // There is no object selected. Hide all the information.
      }
      else
      {
          // Object is selected. Show the relevant information.

        currentHDB = currentImage->astroFile->getHDB(currentImage->currentHDB).get();

        if (settings::astroManagerSettings->value(settings::PHOTOMETRY_USEZMAG, QVariant(false)).toBool())
        {
          if (currentHDB->HDBType() == ACL::BT_IMAGE)
          {
            if (currentHDB->keywordExists(ACL::astroManager_ZMAG))
            {
              zmag = static_cast<FP_t>(currentHDB->keywordData(ACL::astroManager_ZMAG));
            }
            else
            {
              zmag = 0;
            }
          }
          else
          {
            CODE_ERROR(astroManager);
          };
        }
        else
        {
          zmag = 0;
        };

        infoTab->setEnabled(true);

        tlObjectName->setText(QString::fromStdString(po->objectName()));
        if (po->observedCoordinates())
        {
          labelRA->setText(QString::fromStdString(po->observedCoordinates()->RA().A2SHMS()));
          labelDec->setText(QString::fromStdString(po->observedCoordinates()->DEC().A2SDMS()));
        }
        else
        {
          labelRA->setText("--h--'--""");
          labelDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
        };

        tlADUAperture->setText(QString("%1").arg(po->sourceADU()));
        tlAreaAperture->setText(QString("%1").arg(po->sourceArea()));
        tlADUAnnulus->setText(QString("%1").arg(po->skyADU()));
        tlAreaAnnulus->setText(QString("%1").arg(po->skyArea()));

        tlStar->setText(QString("%1").arg(po->getStarADU()));
        tlStarSky->setText(QString("%1").arg(po->getSkyADU()));
        tlSky->setText(QString("%1").arg(po->getStarSkyADU()));
        tlMagnitude->setText(QString("%1").arg(zmag + *(po->instrumentMagnitude())));
        labelMagnitudeError->setText(QString("%1").arg(po->magnitudeError()));
        if ( po->FWHM() )
        {
          labelFWHM->setText(QString("%1").arg(*(po->FWHM())));
        }
        else
        {
          labelFWHM->setText("-.--");
        };

        tlStarE->setText(QString("%1").arg(po->getStarE()));
        tlSNR->setText(QString("%1").arg(po->getSNR()));
      };

      drawProfile(po->CCDCoordinates());

      TRACEEXIT;
    }

    /// @brief Push button to allow the object name to be selected for the object.
    /// @param[in] - unused.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-08-11/GGB - 1) Added code to initialise the object name. (Bug #1210914)
    /// @version                  2) Added code to allow save and make the image dirty.
    /// @version 2013-08-09/GGB - Added support for the compare image window.
    /// @version 2013-05-10/GGB - Function created.

    void CPhotometryDockWidget::eventButtonObjectName(bool)
    {
      QString szName;
      int nRow = tableWidgetPhotometry->currentRow();

      szName = QString::fromStdString(currentImage->photometryObservations[nRow]->objectName());

      dialogs::CSelectObjectDialog *dlg = new dialogs::CSelectObjectDialog(szName, this);

      if (dlg->exec() == QDialog::Accepted)
      {
          // Update the string in the table, as well as the displayed string on the image
          // as well as the strings for the object information.

        currentImage->photometryObservations[nRow]->objectName(szName.toStdString());

        tableWidgetPhotometry->item(nRow, 0)->setText(szName);

        tlObjectName->setText(szName);

        currentImage->astroFile->isDirty(true);

        if (currentImage->parent_->getWindowType() == SWT_IMAGEWINDOW)
        {
          imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(currentImage->parent_);
          if (!iw)
            CODE_ERROR(astroManager);

          iw->repaintImage();
          iw->updateWindowTitle();
        }
        else if (currentImage->parent_->getWindowType() == SWT_IMAGE_COMPARE)
        {
          dynamic_cast<imaging::CImageComparisonWindow *>(currentImage->parent_)->redrawImage();
        }
        else
        {
          CODE_ERROR(astroManager);   // Dock widget should be grayed.
        };
      };
    }

    /// @brief Allows the user to delete an object from the current photometry list. The currently selected item is deleted. The
    ///        graphics item group also needs to be deleted.
    /// @throws
    /// @version 2015-01-01/GGB - Added code to delete the text and group and also to reset the current selection. (Bug #1406768)
    /// @version 2013-08-17/GGB - Function created.

    void CPhotometryDockWidget::eventButtonRemove(bool)
    {
      int nRow = tableWidgetPhotometry->currentRow();

      if (nRow != -1 && currentImage)
      {
          // Delete the object and annotation from the scene.

        QGraphicsScene *scene = currentImage->photometryObservations[nRow]->group->scene();
        scene->removeItem(currentImage->photometryObservations[nRow]->group);
        delete currentImage->photometryObservations[nRow]->group;
        currentImage->photometryObservations[nRow]->group = nullptr;
        currentImage->photometryObservations[nRow]->text = nullptr;

          // Delete the object from the photometry list in the astroFile.

        currentImage->currentPhotometrySelection.reset();   // Remove the selection link.

        currentImage->astroFile->photometryObjectRemove(currentImage->photometryObservations[nRow]->objectName());
        currentImage->photometryObservations.erase(currentImage->photometryObservations.begin() + nRow);

        tableWidgetPhotometry->removeRow(nRow);

        btnObjectName->setEnabled(false);
        pushButtonRemove->setEnabled(false);

        currentImage->astroFile->isDirty(true);

        if (currentImage->parent_->getWindowType() == SWT_IMAGEWINDOW)
        {
          imaging::CImageWindow *iw = dynamic_cast<imaging::CImageWindow *>(currentImage->parent_);
          if (!iw)
            CODE_ERROR(astroManager);

          iw->updateWindowTitle();
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// Function used when the user wishes to select an object for Photometry.
    /// Change the window mode to Photometry and select the photometry cursor.
    ///
    // 2012-11-08/GGB - Function created.

    void CPhotometryDockWidget::eventButtonSelect(bool btnState)
    {
      CMdiSubWindow *subWindow;
      mdiframe::CFrameWindow *frameWindow;

        // Let the image know that it is now going to reference an object.

      frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(parentObject);

      if (frameWindow)
        subWindow = dynamic_cast<CMdiSubWindow *>(frameWindow->activeMdiChild());
      else
        CODE_ERROR(astroManager);

      if (!subWindow)
        CODE_ERROR(astroManager);

      if ( (subWindow->getWindowType() == SWT_IMAGEWINDOW) ||
           (subWindow->getWindowType() == SWT_IMAGE_COMPARE) )
      {
        if (btnState)
        {
          dynamic_cast<imaging::CAstroImageWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_PHOTOMETRY);
          pushButtonSelect->setChecked(true);
        }
        else
        {
          dynamic_cast<imaging::CAstroImageWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_NONE);
          pushButtonSelect->setChecked(false);
        };
      }
      else
      {
        CODE_ERROR(astroManager);   // Dock widget should be grayed.
      }
    }

    // Handles the changed event from the Radius 1 check box.
    //
    // 2013-05-18/GGB - Added save to settings.
    // 2011-05-29/GGB - Function created.

    void CPhotometryDockWidget::eventRadius1Changed(int newValue)
    {
      if (newValue > 0)
      {
        sbRadius2->setMinimum(newValue + 1);

        uiRadius1 = newValue;

        settings::astroManagerSettings->setValue(settings::PHOTOMETRY_RADIUS1, QVariant(newValue) );
      };
    }

    // Handles the changed event from the Radius 2 check box.
    //
    // 2013-05-18/GGB - Saved new value to settings.
    // 2011-05-29/GGB - Function created.

    void CPhotometryDockWidget::eventRadius2Changed(int newValue)
    {
      if (newValue > 0)
      {
        sbRadius1->setMaximum(newValue);
        sbRadius3->setMinimum(newValue + 1);

        uiRadius2 = newValue;

        settings::astroManagerSettings->setValue(settings::PHOTOMETRY_RADIUS2, QVariant(newValue) );
      };
    }

    // Handles the changed event from the Radius 3 check box.
    //
    // 2013-05-18/GGB - Save new value to settings.
    // 2011-05-29/GGB - Function created.

    void CPhotometryDockWidget::eventRadius3Changed(int newValue)
    {
      if (newValue > 0)
      {
        sbRadius2->setMaximum(newValue);

        uiRadius3 = newValue;

        settings::astroManagerSettings->setValue( settings::PHOTOMETRY_RADIUS3, QVariant(newValue) );
      };
    }

    /// @brief Displays the information for the item that has been selected.
    //
    // 2013-08-24/GGB - Added code to allow the selected item to hilight.
    // 2013-03-29/GGB - Function created.

    void CPhotometryDockWidget::eventReferenceSelected(int row, int)
    {
      std::string objectText;
      photometry::PPhotometryObservation selectedObject;

      if (!currentImage->photometryObservations.empty())
      {
        objectText = tableWidgetPhotometry->item(row, 0)->text().toStdString();

        selectedObject = currentImage->photometryObservations[row];
        btnObjectName->setEnabled(true);
      }
      else
      {
        btnObjectName->setEnabled(false);
      }

      displayPhotometry(selectedObject);
      currentImage->parent_->changePhotometrySelection(selectedObject);

      pushButtonRemove->setEnabled(true);
    }

    /// @brief Inserts a row into the table.
    /// @param[in] nRow - The row number to insert the new row.
    /// @param[in] toInsert - The observation to insert.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2012-11-12/GGB - Function created.

    void CPhotometryDockWidget::insertRow(int nRow, photometry::PPhotometryObservation toInsert)
    {
      ACL::CHDB *currentHDB;
      FP_t zmag;

      currentHDB = currentImage->astroFile->getHDB(currentImage->currentHDB).get();

      if (settings::astroManagerSettings->value(settings::PHOTOMETRY_USEZMAG, QVariant(false)).toBool())
      {
        if (currentHDB->HDBType() == ACL::BT_IMAGE)
        {
          if (currentHDB->keywordExists(ACL::astroManager_ZMAG))
          {
            zmag = static_cast<FP_t>(currentHDB->keywordData(ACL::astroManager_ZMAG));
          }
          else
          {
            zmag = 0;
          }
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        zmag = 0;
      }

      tableWidgetPhotometry->insertRow(nRow);
      tableWidgetPhotometry->setRowHeight(nRow, 16);
      tableWidgetPhotometry->setItem(nRow, 0, new QTableWidgetItem(*new QString(QString("%1").arg(QString::fromStdString(toInsert->objectName())))));
      tableWidgetPhotometry->setItem(nRow, 1, new QTableWidgetItem(QString("%1").arg(zmag + *(toInsert->instrumentMagnitude()))));
      tableWidgetPhotometry->setItem(nRow, 2, new QTableWidgetItem(QString("%1").arg(toInsert->magnitudeError())));
      tableWidgetPhotometry->setCurrentCell(nRow, 0);
    }

    /// @brief Redraws all the information in the dock widget. Is called after the photometry image changes.
    //
    // 2013-05-12/GGB - Removed support for the editing push button and the export csv pushbutton.
    // 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    // 2012-11-12/GGB - Function created.

    void CPhotometryDockWidget::redraw()
    {
      int nRow;
      photometry::PPhotometryObservation currentObject;

      if (currentImage)
      {
          // Remove all the existing data

        while (tableWidgetPhotometry->rowCount())
          tableWidgetPhotometry->removeRow(0);

        if (isEnabled())
          pushButtonSelect->setEnabled(true);

        pushButtonRemove->setEnabled(false);
        btnObjectName->setEnabled(false);  // Nothing selected.

        tlObjectName->setText("");
        labelRA->setText("--h--'--""");
        labelDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
        tlVariable->setText("");

        tlADUAperture->setText("");
        tlAreaAperture->setText("");
        tlADUAnnulus->setText("");
        tlAreaAnnulus->setText("");

        tlStar->setText("");
        tlStarSky->setText("");
        tlSky->setText("");
        tlMagnitude->setText("--.--");
        labelFWHM->setText("--.--");

        tlStarE->setText("");
        tlSNR->setText("");

        nRow = 0;

        if (currentImage->astroFile->hasPhotometryHDB())
        {
          currentObject = std::dynamic_pointer_cast<photometry::CPhotometryObservation>(currentImage->astroFile->photometryObjectFirst());

          while (currentObject)
          {
            insertRow(nRow++, currentObject);
            currentObject = std::dynamic_pointer_cast<photometry::CPhotometryObservation>(currentImage->astroFile->photometryObjectNext());
          };
        };
      }
      else
      {
        labelFilter->setText("");


        while (tableWidgetPhotometry->rowCount())
          tableWidgetPhotometry->removeRow(0);

        pushButtonSelect->setEnabled(false);
        pushButtonRemove->setEnabled(false);

        tlObjectName->setText("");
        labelRA->setText("--h--'--""");
        labelDec->setText(UTF16_PLUSMINUSSIGN % "--" % UTF16_DEGREESIGN % "--'--""");
        tlVariable->setText("");

        tlADUAperture->setText("");
        tlAreaAperture->setText("");
        tlADUAnnulus->setText("");
        tlAreaAnnulus->setText("");

        tlStar->setText("---");
        tlStarSky->setText("---");
        tlSky->setText("---");
        tlMagnitude->setText("---");
        labelFWHM->setText("--.--");

        tlStarE->setText("");
        tlSNR->setText("");
      };
    }

    /// Function called by image window when a reference has been completed. \n
    /// 1. Uncheck the button \n
    /// 2. Ensure that the user selects an object to go with the reference. \n
    /// 3. Add the new item into the astroFile \n
    /// 4. Add the new item to the tableWidget \n
    //
    // 2013-08-05/GGB - Moved code into the window objects.
    // 2013-04-12/GGB - Added code to set the creator and date of a new HDB.
    // 2013-03-31/GGB - Function created.

    void CPhotometryDockWidget::referenceCompleted(photometry::PPhotometryObservation photometryObject)
    {
      imaging::CImageWindow *subWindow;
      mdiframe::CFrameWindow *frameWindow;
      int nRow = tableWidgetPhotometry->rowCount();
      pushButtonSelect->setChecked(false);

         // Let the image know that it has finished referencing.

      frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(parentObject);
      RUNTIME_ASSERT(astroManager, frameWindow != nullptr, "frameWindow dynamic_cast to nullptr");

      subWindow = dynamic_cast<imaging::CImageWindow *>(frameWindow->activeMdiChild());
      RUNTIME_ASSERT(astroManager, subWindow != nullptr, "subWindow dynamic_cast to nullptr");

      if ( (subWindow->getWindowType() == SWT_IMAGEWINDOW) || (subWindow->getWindowType() == SWT_IMAGE_COMPARE) )
      {
        dynamic_cast<imaging::CAstroImageWindow *>(subWindow)->setMode(imaging::CAstroImageWindow::M_NONE);
      }
      else
      {
        CODE_ERROR(astroManager);   // Dock widget should be grayed.
      }

      tableWidgetPhotometry->insertRow(nRow);

      tableWidgetPhotometry->setItem(nRow, 0, new QTableWidgetItem(QString::fromStdString(photometryObject->objectName())));
      tableWidgetPhotometry->item(nRow, 0)->setData(ROLE_OBJECINDEX_t, QVariant(nRow));
    }

    /// Called when the object needs to be enabled or disabled.
    //
    // 2013-07-18/GGB - Function created.

    void CPhotometryDockWidget::setEnabled(bool enabledValue)
    {
      labelFilter->setEnabled(enabledValue);
      infoTab->setEnabled(enabledValue);
      tableWidgetPhotometry->setEnabled(enabledValue);

      pushButtonSelect->setEnabled(enabledValue);
      pushButtonRemove->setEnabled(enabledValue);
    }

    /// Function to setup all the user interface elements
    /// @throws GCL::CError(astroManager, 0x0001)
    /// @version 2017-07-10/GGB - Bug #90 checking for resource opening succesfully.
    /// @version 2013-08-18/GGG - Added ZMAG support
    /// @version 2013-05-18/GGB - Added support for the profileCurve.
    /// @version 2013-05-12/GGB - Removed support for the edit and export csv push button.
    /// @version 2013-04-25/GGB - Added support for the FWHM display.
    /// @version 2013-03-29/GGB - Added support for selection of items in the list of objects.
    /// @version 2012-11-09/GGB - Function created.

    void CPhotometryDockWidget::setupUI()
    {
      QUiLoader loader;
      int nIndex = 0;
      QGridLayout *glayout;

      QFile file(":/forms/dwPhotometry.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Unable to load resource :/forms/dwPhotometry.ui");
        ERROR(astroManager, 0x0001);
      };

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWindowTitle(tr("Single Image Photometry"));

      labelFilter = formWidget->findChild<QLabel *>("labelFilter");
      labelZMAG = formWidget->findChild<QLabel *>("labelZMAG");

      tableWidgetPhotometry = formWidget->findChild<QTableWidget *>("tableWidgetPhotometry");

      infoTab = formWidget->findChild<QTabWidget *>("infoTab");
        tabAperture = infoTab->widget(nIndex);
          sbRadius1 = formWidget->findChild<QSpinBox *>("spinBoxRadius1");
          sbRadius2 = formWidget->findChild<QSpinBox *>("spinBoxRadius2");
          sbRadius3 = formWidget->findChild<QSpinBox *>("spinBoxRadius3");
        tabObjectInformation = infoTab->widget(++nIndex);
          btnObjectName = formWidget->findChild<QPushButton *>("btnObjectName");
          tlObjectName = formWidget->findChild<QLabel *>("tlObjectName");
          labelRA = formWidget->findChild<QLabel *>("labelRA");
          labelDec = formWidget->findChild<QLabel *>("labelDec");
          tlVariable = formWidget->findChild<QLabel *>("tlVariable");

      if (!btnObjectName || !tlObjectName || !labelRA || !labelDec || !tlVariable)
        CODE_ERROR(astroManager);

        tabCounts = infoTab->widget(++nIndex);
          tlADUAperture = formWidget->findChild<QLabel *>("tlADUAperture");
          tlAreaAperture = formWidget->findChild<QLabel *>("tlAreaAperture");
          tlADUAnnulus = formWidget->findChild<QLabel *>("tlADUAnnulus");
          tlAreaAnnulus = formWidget->findChild<QLabel *>("tlAreaAnnulus");
        tabPhotometry = infoTab->widget(++nIndex);
          tlStar = formWidget->findChild<QLabel *>("tlStar");
          tlSky = formWidget->findChild<QLabel *>("tlSky");
          tlStarSky = formWidget->findChild<QLabel *>("tlStarSky");
          tlMagnitude = formWidget->findChild<QLabel *>("tlMagnitude");
          labelMagnitudeError = formWidget->findChild<QLabel *>("labelMagnitudeError");
          labelFWHM = formWidget->findChild<QLabel *>("labelFWHM");
        tabElectrons = infoTab->widget(++nIndex);
          tlStarE = formWidget->findChild<QLabel *>("tlStarE");
          tlNoiseE = formWidget->findChild<QLabel *>("tlNoiseE");
          tlSNR = formWidget->findChild<QLabel *>("tlSNR");
        tabProfile = infoTab->widget(++nIndex);
        glayout = (QGridLayout *) tabProfile->layout();
          profilePlot = new QwtPlot();
          profileCurve = new QwtPlotCurve();
          profileCurve->setTitle( "Some Points" );
          profileCurve->setPen( QPen( Qt::black, 4 ) );
          QwtSymbol *symbol = new QwtSymbol( QwtSymbol::XCross, QBrush( Qt::black), QPen( Qt::black, 2 ), QSize( 8, 8 ) );
          profileCurve->setSymbol( symbol );
          profileCurve->setStyle(QwtPlotCurve::Dots);
          profileCurve->attach( profilePlot );
          glayout->addWidget(profilePlot, 0, 0, 1, 2);

      if (!tlStar || !tlSky || !tlStarSky || !tlMagnitude || !labelMagnitudeError || !labelFWHM)
        CODE_ERROR(astroManager);

      infoTab->setEnabled(false);		// Must not be useable until an object is chosen.

      pushButtonSelect = formWidget->findChild<QPushButton *>("pushButtonSelect");
      pushButtonRemove = formWidget->findChild<QPushButton *>("pushButtonRemove");

      setWidget(formWidget);

      sbRadius1->setValue(uiRadius1);
      sbRadius1->setMinimum(0);
      sbRadius1->setMaximum(uiRadius2);

      sbRadius2->setValue(uiRadius2);
      sbRadius2->setMinimum(uiRadius1);
      sbRadius2->setMaximum(uiRadius3);

      sbRadius3->setValue(uiRadius3);
      sbRadius3->setMinimum(uiRadius2);

      connect(tableWidgetPhotometry, SIGNAL(cellClicked(int, int)), this, SLOT(eventReferenceSelected(int, int)));

      connect(sbRadius1, SIGNAL(valueChanged(int)), this, SLOT(eventRadius1Changed(int)));
      connect(sbRadius2, SIGNAL(valueChanged(int)), this, SLOT(eventRadius2Changed(int)));
      connect(sbRadius3, SIGNAL(valueChanged(int)), this, SLOT(eventRadius3Changed(int)));

      connect(btnObjectName, SIGNAL(clicked(bool)), this, SLOT(eventButtonObjectName(bool)));

      connect(pushButtonSelect, SIGNAL(clicked(bool)), this, SLOT(eventButtonSelect(bool)));
      connect(pushButtonRemove, SIGNAL(clicked(bool)), this, SLOT(eventButtonRemove(bool)));

      setEnabled(false);
    }


  }  // namespace dockwidgets
}  // namespace AstroManager
