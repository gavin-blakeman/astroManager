//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								CInstrumentDockWidget
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2011-2013, 2018 Gavin Blakeman.
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
// CLASSES INCLUDED:    CInstrumentDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//
// HISTORY:             2013-06-16/GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 Release
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../include/CInstrumentDockwidget.h"

  // Include files for astroManager

#include "../include/database/databaseARID.h"

  // Miscellaneous library header files.

#include <ACL>
#include <GCL>
#include <QCL>

namespace astroManager
{
  namespace dockwidgets
  {


    //*****************************************************************************************************************************
    //
    // CInstrumentDockWidget
    //
    //*****************************************************************************************************************************

    // Class constructor
    //
    // 2011-05-31/GGB - Function created.

    CInstrumentDockWidget::CInstrumentDockWidget(QAction *anAction, QWidget *aParent) : dwAction(anAction),
      QDockWidget(aParent), bInternalCheck(false)
    {
      setupUI();
    }

    // Updates the values as required when the user changes the aperture.
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::eventApertureChanged(int newValue)
    {
      if (!bInternalCheck)
      {
        bInternalCheck = true;
        if (cbFRatio->checkState() == Qt::Checked)
        {
          sbFocalLength->setValue((int) floor(newValue * sbFRatio->value()));
        }
        else
        {
          sbFRatio->setValue(sbFocalLength->value() / newValue);
        };
        bInternalCheck = false;
      };
    }

    // Updates the values as required when the user changes the F-Ratio.
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::eventFRatioChanged(double newValue)
    {
      if (!bInternalCheck)
      {
        bInternalCheck = true;
        if (cbAperture->checkState() == Qt::Checked)
        {
          sbFocalLength->setValue((int) floor(newValue * sbAperture->value()));
        }
        else
        {
          sbAperture->setValue( (int) floor(sbFocalLength->value() / newValue));
        };
        bInternalCheck = false;
      };
    }

    // Handles the state change message from the aperture check box.
    // At any one time, two of the three items (Aperture, F-Ratio, Focal Length) must be checked.
    // This function is to ensure that this is correct.
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::eventCheckAperture(int newState)
    {
      if (!bInternalCheck)
      {
        bInternalCheck = true;
        if (newState == Qt::Checked)
        {
          sbAperture->setEnabled(true);

            // Automatically uncheck the FocalLength box.

          cbFocalLength->setChecked(false);
          sbFocalLength->setEnabled(false);
        }
        else
        {
          sbAperture->setEnabled(false);

          if (cbFocalLength->checkState() == Qt::Unchecked)
          {
            cbFocalLength->setChecked(true);
            sbFocalLength->setEnabled(true);
          }
          else
          {
            cbFRatio->setChecked(true);
            sbFRatio->setEnabled(true);
          };
        };
        bInternalCheck = false;
      };
    }

    // Handles the state change message from the aperture check box.
    // At any one time, two of the three items (Aperture, F-Ratio, Focal Length) must be checked.
    // This function is to ensure that this is correct.
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::eventCheckFocalLength(int newState)
    {
      if (!bInternalCheck)
      {
        bInternalCheck = true;
        if (newState == Qt::Checked)
        {
          sbFocalLength->setEnabled(true);

            // Automatically uncheck the focal lenght box

          cbFRatio->setChecked(false);
          sbFRatio->setEnabled(false);
        }
        else
        {
          sbFocalLength->setEnabled(false);

          if (cbFRatio->checkState() == Qt::Unchecked)
          {
            cbFRatio->setChecked(true);
            sbFRatio->setEnabled(true);
          }
          else
          {
            cbAperture->setChecked(true);
            sbAperture->setEnabled(true);
          };
        };
        bInternalCheck = false;
      };
    };

    // Handles the state change message from the aperture check box.
    // At any one time, two of the three items (Aperture, F-Ratio, Focal Length) must be checked.
    // This function is to ensure that this is correct.
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::eventCheckFRatio(int newState)
    {
      if (!bInternalCheck)
      {
        bInternalCheck = true;
        if (newState == Qt::Checked)
        {
          sbFRatio->setEnabled(true);

            // Automatically uncheck the focal lenght box

          cbFocalLength->setChecked(false);
          sbFocalLength->setEnabled(false);
        }
        else
        {
          sbFRatio->setEnabled(false);

          if (cbFocalLength->checkState() == Qt::Unchecked)
          {
            cbFocalLength->setChecked(true);
            sbFocalLength->setEnabled(true);
          }
          else
          {
            cbAperture->setChecked(true);
            sbAperture->setEnabled(true);
          };
        };
        bInternalCheck = false;
      };
    };

    // Handles a change of the selected camera model in the combo box.
    // The gain, sensor size and pixel sizes must be updated.
    //
    // 2010-11-07/GGB - Function created.

    void CInstrumentDockWidget::eventComboCameraIndexChanged(int nIndex)
    {
      float PixelX, PixelY, Gain;
      long SensorX, SensorY;

      long long ID = comboCamera->itemData(nIndex).toLongLong();

      try
      {
        //database::GetInstrumentDetails(ID, PixelX, PixelY, SensorX, SensorY, Gain);
        sbPixelX->setValue(PixelX);
        sbPixelY->setValue(PixelY);
        sbGain->setValue(Gain);
        sbSensorX->setValue(SensorX);
        sbSensorY->setValue(SensorY);
        UpdateFOV();
      }
      catch (GCL::CError &)
      {
        // Name not found.
      };
    }

    // Updates the values as required when the user changes the Focal Lenght
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::eventFocalLengthChanged(int newValue)
    {
      if (!bInternalCheck)
      {
        bInternalCheck = true;
        if (cbAperture->checkState() == Qt::Checked)
        {
          sbFRatio->setValue(newValue / sbAperture->value());
        }
        else
        {
          sbAperture->setValue((int) floor(newValue / sbFRatio->value()));
        };
      };
      bInternalCheck = false;
    };

    // Ensures the FOV data is updated when the tab is selected.
    //
    // 2010-11-09/GGB - Function created.

    void CInstrumentDockWidget::eventSetupTabChanged(int nTab)
    {
      if (nTab == 2)
        UpdateFOV();
    };

    // Sets up the user interface elements.
    //
    // 2010-11-02/GGB - Function created.

    void CInstrumentDockWidget::setupUI()
    {
      QUiLoader loader;
      int nIndex = 0;

      QFile file(":/forms/dwInstrumentInformation.ui");
      file.open(QFile::ReadOnly);

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

//      setupTab = qFindChild<QTabWidget *>(formWidget, "setupTab");
//        tabTelescope = setupTab->widget(++nIndex);
//          cbAperture = qFindChild<QCheckBox *>(formWidget, "cbAperture");
//          cbFRatio = qFindChild<QCheckBox *>(formWidget, "cbFRatio");
//          cbFocalLength = qFindChild<QCheckBox *>(formWidget, "cbFocalLength");
//          sbAperture = qFindChild<QSpinBox *>(formWidget, "sbAperture");
//          sbFRatio = qFindChild<QDoubleSpinBox *>(formWidget, "sbFRatio");
//          sbFocalLength = qFindChild<QSpinBox *>(formWidget, "sbFocalLength");
//        tabInstrument = setupTab->widget(++nIndex);
//          cbCamera = qFindChild<QCheckBox *>(formWidget, "cbCamera");
//          comboCamera = qFindChild<QComboBox *>(formWidget, "comboCamera");
//          sbGain = qFindChild<QDoubleSpinBox *>(formWidget, "sbGain");
//          sbPixelX = qFindChild<QDoubleSpinBox *>(formWidget, "sbPixelX");
//          sbPixelY = qFindChild<QDoubleSpinBox *>(formWidget, "sbPixelY");
//          sbSensorX = qFindChild<QSpinBox *>(formWidget, "sbSensorX");
//          sbSensorY = qFindChild<QSpinBox *>(formWidget, "sbSensorY");
//        tabFOV = setupTab->widget(++nIndex);
//          tlPixX = qFindChild<QLabel *>(formWidget, "tlPixX");
//          tlPixY = qFindChild<QLabel *>(formWidget, "tlPixY");
//          tlFOVx = qFindChild<QLabel *>(formWidget, "tlFOVx");
//          tlFOVy = qFindChild<QLabel *>(formWidget, "tlFOVy");

      tabInstrument = setupTab->widget(2);

      connect(setupTab, SIGNAL(currentChanged(int)), this, SLOT(eventSetupTabChanged(int)));

      connect(cbAperture, SIGNAL(stateChanged(int)), this, SLOT(eventCheckAperture(int)));
      connect(cbFRatio, SIGNAL(stateChanged(int)), this, SLOT(eventCheckFRatio(int)));
      connect(cbFocalLength, SIGNAL(stateChanged(int)), this, SLOT(eventCheckFocalLength(int)));

      connect(sbAperture, SIGNAL(valueChanged(int)), this, SLOT(eventApertureChanged(int)));
      connect(sbFRatio, SIGNAL(valueChanged(double)), this, SLOT(eventFRatioChanged(double)));
      connect(sbFocalLength, SIGNAL(valueChanged(int)), this, SLOT(eventFocalLengthChanged(int)));

      if (database::databaseARID)
        database::databaseARID->PopulateInstrumentCombo(comboCamera, false);

      cbAperture->setChecked(true);
      sbAperture->setEnabled(true);

      cbFRatio->setChecked(true);
      sbFRatio->setEnabled(true);

      cbFocalLength->setChecked(false);
      sbFocalLength->setEnabled(false);

      cbCamera->setChecked(false);

    };

    // Updates the FOV information in the tab
    //
    // 2010-11-09/GGB - Function created.

    void CInstrumentDockWidget::UpdateFOV()
    {
      double pX, pY;					// Size of pixels (arcsecs)
      double FOVx, FOVy;			// Field of view (arcsecs)
      double FL, dX, dY;			// Focal Length, pixel size in microns
      long PixelX, PixelY;	// Size of the sensor in pixels
      char szNumber[50];

      dX = sbPixelX->value();
      dY = sbPixelY->value();
      FL = sbFocalLength->value();
      PixelX = sbSensorX->value();
      PixelY = sbSensorY->value();

        // Get the focal length in mm and the pixel dimensions

      pX = 206.265 / FL * dX;
      pY = 206.265 / FL * dY;

      tlPixX->setText(QString("%1").arg(pX));
      tlPixY->setText(QString("%1").arg(pY));

      FOVx = pX * PixelX;
      FOVy = pY * PixelY;

      //ACL::sprintfDMS(szNumber, FOVx / 3600, false, 1);
      tlFOVx->setText(QString(szNumber));
      //ACL::sprintfDMS(szNumber, FOVy / 3600, false, 1);
      tlFOVy->setText(QString(szNumber));
    }

  }  // namespace dockwidgets

}  // namespace AstroManager
