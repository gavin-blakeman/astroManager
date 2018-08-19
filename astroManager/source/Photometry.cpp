//*************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:			          VSOPPhotometry.cpp
// SUBSYSTEM:           Photometry Management Windows
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// NAMESPACE:						AstroManager::photometry
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2010-2018 Gavin Blakeman.
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
// OVERVIEW:	          Classes for photometry windows.
//
// CLASSES:		          CProgramDisplayEdit - Class for displaying and editing a Photometry Program
//						          CWindowLightCurves - Class for displaying and analysing light curves.
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 Release.
//                      2013-01-20 GGB - astroManager 0000.00 Release
//
//*************************************************************************************************

#include "../include/Photometry.h"

#include <Qt>
#include <QtUiTools/QtUiTools>
#include <QtCore/QtCore>

#include "qwt_legend.h"
#include "qwt_plot_marker.h"
#include "qwt_symbol.h"

#include "boost/scoped_ptr.hpp"

#include <ACL>
#include <GCL>

#include "../include/astroManager.h"
#include "../include/database/databaseARID.h"
#include "../include/database/databaseATID.h"
#include "../include/FrameWindow.h"
#include "../include/Settings.h"

#include "float.h"

namespace astroManager
{
  namespace photometry
  {

    // Helper class for storing information about graphs.

    struct SLCData
    {
      char szFilter[7];		// Filter that this data applies to.
      long lItemCount;		// Number of items already stored.
      long lArraySize;		// Current size of the array
      double *JD;					// Array of X values.
      double *Mag;				// Array of Y values.
      double *MagErr;

      SLCData(void);
      ~SLCData(void);
    };

    // Constructor for the structure class.
    //
    // 2010-07-07/GGB - Function created.

    SLCData::SLCData(void)
    {
      szFilter[0] = '\0';
      lItemCount = 0;
      lArraySize = 500;		// Initial array size;
      JD = new double[lArraySize];
      Mag = new double[lArraySize];
      MagErr = new double[lArraySize];
    }

    // Class destructor. Clean up after class has been used.
    //
    // 2010-07-07/GGB - Function created.

    SLCData::~SLCData(void)
    {
      if (JD)
      {
        delete [] JD;
        JD = NULL;
      };
      if (Mag)
      {
        delete [] Mag;
        Mag = NULL;
      };
      if (MagErr)
      {
        delete [] MagErr;
        MagErr = NULL;
      };
      szFilter[0] = '\0';
      lItemCount = 0;
      lArraySize = 0;
    }

    //*****************************************************************************************************************************
    //
    // CWindowLightCurves
    //
    // Window for displaying and analysing light curves.
    //
    //*****************************************************************************************************************************

    // Class constructor. Loads the .ui information and creates the window.
    //
    // Revision:		1.00
    // Date:				6 July 2010
    // Written by:	Gavin Blakeman

    CWindowLightCurves::CWindowLightCurves(QWidget *parent) : QMdiSubWindow(parent)
    {
//      QGridLayout *glayout;
//      QUiLoader loader;
//      QwtSymbol *symbol;
//      int nIndex;
//      QPen pen;

//      LCData = NULL;
//      LCCount = 0;

//      QFile file(":/forms/windowPhotometryLightCurves.ui");
//      file.open(QFile::ReadOnly);

//      QWidget *formWidget = loader.load(&file, this);
//      file.close();

//      glayout = (QGridLayout *) formWidget->layout();

//      cbObject = qFindChild<QComboBox *>(formWidget, "cbObject");
//      connect(cbObject, SIGNAL(currenINDEX_tChanged(int)), this, SLOT(eventComboIndexChanged(int)));

//      cbU = qFindChild<QCheckBox *>(formWidget, "cbU");
//      connect(cbU, SIGNAL(stateChanged(int)), this, SLOT(eventCheckUChanged(int)));

//      cbB = qFindChild<QCheckBox *>(formWidget, "cbB");
//      connect(cbB, SIGNAL(stateChanged(int)), this, SLOT(eventCheckBChanged(int)));

//      cbV = qFindChild<QCheckBox *>(formWidget, "cbV");
//      connect(cbV, SIGNAL(stateChanged(int)), this, SLOT(eventCheckVChanged(int)));

//      cbR = qFindChild<QCheckBox *>(formWidget, "cbR");
//      connect(cbR, SIGNAL(stateChanged(int)), this, SLOT(eventCheckRChanged(int)));

//      cbI = qFindChild<QCheckBox *>(formWidget, "cbI");
//      connect(cbI, SIGNAL(stateChanged(int)), this, SLOT(eventCheckIChanged(int)));

//      sbMin = qFindChild<QDoubleSpinBox *>(formWidget, "sbMin");
//      connect(sbMin, SIGNAL(valueChanged(double)), this, SLOT(eventJDMinChanged(double)));

//      sbMax = qFindChild<QDoubleSpinBox *>(formWidget, "sbMax");
//      connect(sbMax, SIGNAL(valueChanged(double)), this, SLOT(eventJDMaxChanged(double)));

//      plot = new QwtPlot();
//      glayout->addWidget(plot, 1, 0, 10, 5);

//      plot->setTitle(tr("Light Curve"));

//      plot->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

//      series = new QwtPlotCurve[5];		// Create 5 curves
//      symbol = new QwtSymbol();
//      symbol->setStyle(QwtSymbol::Ellipse);
//      symbol->setSize(5,5);

//      for (nIndex=0; nIndex < 5; nIndex++)
//      {
//        series[nIndex].attach(plot);
//        series[nIndex].setStyle(QwtPlotCurve::Dots);
//      };

//      pen.setColor(Qt::darkBlue);
//      symbol->setPen(pen);
//      series[0].setTitle(*new QString("U"));
//      series[0].setSymbol(symbol);

//      pen.setColor(Qt::blue);
//      symbol->setPen(pen);
//      series[1].setTitle(*new QString("B"));
//      series[1].setSymbol(symbol);

//      pen.setColor(Qt::green);
//      symbol->setPen(pen);
//      series[2].setTitle(*new QString("V"));
//      series[2].setSymbol(symbol);

//      pen.setColor(Qt::darkRed);
//      symbol->setPen(pen);
//      series[3].setTitle(*new QString("R"));
//      series[3].setSymbol(symbol);

//      pen.setColor(Qt::red);
//      symbol->setPen(pen);
//      series[4].setTitle(*new QString("I"));
//      series[4].setSymbol(symbol);

//      setWidget(formWidget);

//      PopulateCombo();

    };

    // Deletes any memory dynamically allocated by the class.
    //
    // Revision:		1.00
    // Date:				6 July 2010
    // Written by:	Gavin Blakeman

    CWindowLightCurves::~CWindowLightCurves(void)
    {
      if (LCData)
      {
        delete [] LCData;
        LCData = NULL;
        LCCount = 0;
      };
      if (series)
      {
        delete [] series;
        series = NULL;
      };
    };

    // Procedure to populate the combo box with the object names.
    // Only objects that have light curves are populated into the combo box.
    //
    // Revision:		1.00
    // Date:				6 July 2010
    // Written by:	Gavin Blakeman

    void CWindowLightCurves::PopulateCombo(void)
    {
      QString szSQL("SELECT DISTINCT TBL_NAMES.Name, TBL_STELLAROBJECTS.OBJECT_ID FROM TBL_FILTERS INNER JOIN ((TBL_NAMES RIGHT JOIN TBL_STELLAROBJECTS ON TBL_NAMES.NAME_ID = TBL_STELLAROBJECTS.PreferredName) INNER JOIN TBL_PHOT_LIGHTCURVE ON TBL_STELLAROBJECTS.OBJECT_ID = TBL_PHOT_LIGHTCURVE.OBJECT_ID) ON TBL_FILTERS.FILTER_ID = TBL_PHOT_LIGHTCURVE.FILTER_ID");
      QSqlQuery query(database::databaseATID->database());
      QSqlQuery query2(database::databaseATID->database());
      QVariant variant;

      query.exec(szSQL);

      while (query.next())
      {
        variant = query.value(0);
        if (variant.isNull())
        {		// Get the first name out of the preferred name list.
          szSQL = QString("SELECT na.Name FROM TBL_NAMES na WHERE na.OBJECT_ID = %1").arg(query.value(1).toString());
          query2.exec(szSQL);
          variant = query2.value(0);
        };
        cbObject->addItem(*new QString(variant.toString()), *new QVariant(query.value(1)));
      };
    };

    // Called when the index of the item in the combo box changes.
    // This means that the data for the graphs needs to be updated and that the
    // graphs also need to be updated.
    //
    // Note:	All dates are automatically converted to HJD dates on the fly.
    //
    // Revision:		1.00
    // Date:
    // Written by:

    void CWindowLightCurves::eventComboIndexChanged(int index)
    {
      double dRa, dDec;
      QString szSQL = QString("SELECT o.RA, o.DEC FROM TBL_STELLAROBJECTS o WHERE o.OBJECT_ID = %1").arg(cbObject->itemData(index).toString());
      QString szSQL1 = QString("SELECT TBL_FILTERS.SHORTTEXT, TBL_PHOT_LIGHTCURVE.JD, TBL_PHOT_LIGHTCURVE.Magnitude, TBL_PHOT_LIGHTCURVE.Mag_error " \
                                "FROM TBL_FILTERS INNER JOIN TBL_PHOT_LIGHTCURVE ON TBL_FILTERS.FILTER_ID = TBL_PHOT_LIGHTCURVE.FILTER_ID " \
                                "WHERE (TBL_PHOT_LIGHTCURVE.OBJECT_ID=%1) ORDER BY TBL_FILTERS.SHORTTEXT, TBL_PHOT_LIGHTCURVE.JD").arg(cbObject->itemData(index).toString());
      int nIndex, nIndex2;
      SLCData *newLCData = NULL;
      int newLCCount = 0;
      double *newJDArray, *newMagArray, *newMagErrArray;
      double mMin=0, mMax=0;
      bool bFirst = true;
      bool bU, bB, bV, bR, bI;
      double JDMin, JDMax;

      QSqlQuery query(database::databaseATID->database());

        // Delete any curve data that is already stored.

      if (LCData)
      {
        delete [] LCData;
        LCData = NULL;
        LCCount = NULL;

      };

        // Get the RA and declination for finding the HJD.

      query.exec(szSQL);
      query.first();
      dRa = ACL::hms2deg(query.value(0).toDouble() / 10000);
      dDec = ACL::dms2deg(query.value(1).toDouble() / 10000);

        // Open the main query for getting the light curve values to load into the array.

      query.exec(szSQL1);

      JDMin = JDMax = 0;

      while (query.next())
      {
        if (bFirst)
        {
          mMax = mMin = query.value(2).toDouble();
          JDMin = JDMax = query.value(1).toDouble();
          bFirst = false;
        }
        else
        {

          if (query.value(2).toDouble() < mMin)
            mMin = query.value(2).toDouble();

          if (query.value(2).toDouble() > mMax)
            mMax = query.value(2).toDouble();

          if (query.value(1).toDouble() > JDMax)
            JDMax = query.value(1).toDouble();

          if (query.value(1).toDouble() < JDMin)
            JDMin = query.value(1).toDouble();
        };

        if (LCCount == 0)
        {
          LCData = new SLCData[30];		// 24 Known filters when file created. Allow some expansion.
          LCCount = 30;
        };
        for (nIndex = 0; nIndex < LCCount; nIndex++)
        {
          if (LCData[nIndex].szFilter[0] == '\0')
          {	// Filter not existing in this slot.
            strcpy(LCData[nIndex].szFilter, query.value(0).toString().toLatin1().constData());
            break;		// exit for loop.
          }
          else if (strcmp(LCData[nIndex].szFilter, query.value(0).toString().toLatin1().constData()) == 0)
          {
            break;		// Exit the for loop.
          }
          else if (nIndex == (LCCount-1))
          {	// Exhausted all items in array. Need to increase array size.

            newLCCount = LCCount +10;		// Increase by 10 items.
            newLCData = new SLCData[newLCCount];

              // Copy the data to the new array.

            for (nIndex2 = 0; nIndex2 < LCCount; nIndex2++)
              newLCData[nIndex2] = LCData[nIndex2];

              // Delete the old array

            delete [] LCData;
            LCData = newLCData;
            newLCData = NULL;
            LCCount = newLCCount;
            newLCCount = 0;

            nIndex--;		// Make the loop run one more time
          };
        };		// end of for loop.
        if (nIndex < LCCount)
        {	// break from loop. nIndex points to correct entry.
          // insert the data from the database into the entry.

          if (LCData[nIndex].lItemCount+1 == LCData[nIndex].lArraySize)  // Increament array size before we get to the maximum size.
          {	// Increase array size.
            LCData[nIndex].lArraySize += 500;		// Increment size

            newJDArray = new double[LCData[nIndex].lArraySize];
            newMagArray = new double[LCData[nIndex].lArraySize];
            newMagErrArray = new double[LCData[nIndex].lArraySize];

            for (nIndex2 = 0; nIndex2 <= LCData[nIndex].lItemCount; nIndex2++)
            {
              newJDArray[nIndex2] = LCData[nIndex].JD[nIndex2];
              newMagArray[nIndex2] = LCData[nIndex].Mag[nIndex2];
              newMagErrArray[nIndex2] = LCData[nIndex].MagErr[nIndex2];
            };// for loop
            delete [] LCData[nIndex].JD;
            delete [] LCData[nIndex].Mag;
            delete [] LCData[nIndex].MagErr;
            LCData[nIndex].JD = newJDArray;
            LCData[nIndex].Mag = newMagArray;
            LCData[nIndex].MagErr = newMagErrArray;
            newJDArray = NULL;
            newMagArray = NULL;
            newMagErrArray = NULL;
          }; // endif
            // lItemCount is pointing to a valid Array address.
          LCData[nIndex].JD[LCData[nIndex].lItemCount] = query.value(1).toDouble(); //JD2HJD(query.value(1).toDouble(), dRa, dDec);
          LCData[nIndex].Mag[LCData[nIndex].lItemCount] = query.value(2).toDouble();
          LCData[nIndex].MagErr[LCData[nIndex].lItemCount] = query.value(3).toDouble();
          LCData[nIndex].lItemCount++;
        };
      }; // end while

        // Use the data and create the U, B, V, R, I curves from the data.

      bU = bB = bV = bR = bI = false;		// None found yet.

        // NULL all the data arrays

      for (nIndex = 0; nIndex < 5; nIndex++)
        series[nIndex].setRawSamples(NULL, NULL, 0);

      nIndex = 0;

      while ( (nIndex < LCCount) && (!(bU && bB && bV && bR && bI)) )
      {
        if (strcmp(LCData[nIndex].szFilter, "U") == 0)
        {
          series[0].setRawSamples(LCData[nIndex].JD, LCData[nIndex].Mag, LCData[nIndex].lItemCount);
          bU = true;
        }
        else if (strcmp(LCData[nIndex].szFilter, "B") == 0)
        {
          series[1].setRawSamples(LCData[nIndex].JD, LCData[nIndex].Mag, LCData[nIndex].lItemCount);
          bB = true;
        }
        else if (strcmp(LCData[nIndex].szFilter, "V") == 0)
        {
          series[2].setRawSamples(LCData[nIndex].JD, LCData[nIndex].Mag, LCData[nIndex].lItemCount);
          bV = true;
        }
        else if (strcmp(LCData[nIndex].szFilter, "R") == 0)
        {
          series[3].setRawSamples(LCData[nIndex].JD, LCData[nIndex].Mag, LCData[nIndex].lItemCount);
          bR = true;
        }
        else if (strcmp(LCData[nIndex].szFilter, "I") == 0)
        {
          series[4].setRawSamples(LCData[nIndex].JD, LCData[nIndex].Mag, LCData[nIndex].lItemCount);
          bI = true;
        };

        nIndex++;
      };

      if (!bU)
        cbU->setEnabled(false);
      else
        cbU->setEnabled(true);

      if (!bB)
        cbB->setEnabled(false);
      else
        cbB->setEnabled(true);

      if (!bV)
        cbV->setEnabled(false);
      else
        cbV->setEnabled(true);

      if (!bR)
        cbR->setEnabled(false);
      else
        cbR->setEnabled(true);

      if (!bI)
        cbI->setEnabled(false);
      else
        cbI->setEnabled(true);


      if (mMax < 0)
        mMax = floor(mMax);
      else
        mMax = ceil(mMax);

      if (mMin < 0)
        mMin = ceil(mMin);
      else
        mMin = floor(mMin);

      sbMin->setMinimum(JDMin);
      sbMin->setMaximum(JDMax);
      sbMin->setValue(JDMin);

      sbMax->setMinimum(JDMin);
      sbMax->setMaximum(JDMax);
      sbMax->setValue(JDMax);

      plot->setAxisScale(QwtPlot::xBottom, JDMin, JDMax);

      plot->setAxisScale(QwtPlot::yLeft, mMax, mMin);


      if (cbU->isChecked())
        series[0].show();
      else
        series[0].hide();

      if (cbB->isChecked())
        series[1].show();
      else
        series[1].hide();

      if (cbV->isChecked())
        series[2].show();
      else
        series[2].hide();

      if (cbR->isChecked())
        series[3].show();
      else
        series[3].hide();

      if (cbI->isChecked())
        series[4].show();
      else
        series[4].hide();

      plot->replot();

    };

    // Event handler for the U checkbox.
    //
    // Revision:		1.00
    // Date:				17 July 2010
    // Written by:	Gavin Blakeman

    void CWindowLightCurves::eventCheckUChanged(int state)
    {
      if (state == Qt::Checked)
        series[0].show();
      else
        series[0].hide();

      plot->replot();
    };

    // Event handler for the B checkbox.
    //
    // Revision:		1.00
    // Date:				17 July 2010
    // Written by:	Gavin Blakeman

    void CWindowLightCurves::eventCheckBChanged(int state)
    {
      if (state == Qt::Checked)
        series[1].show();
      else
        series[1].hide();

      plot->replot();
    };

    // Event handler for the V checkbox.
    //
    // Revision:		1.00
    // Date:				17 July 2010
    // Written by:	Gavin Blakeman

    void CWindowLightCurves::eventCheckVChanged(int state)
    {
      if (state == Qt::Checked)
        series[2].show();
      else
        series[2].hide();

      plot->replot();
    };

    // Event handler for the R checkbox.
    //
    // Revision:		1.00
    // Date:				17 July 2010
    // Written by:	Gavin Blakeman

    void CWindowLightCurves::eventCheckRChanged(int state)
    {
      if (state == Qt::Checked)
        series[3].show();
      else
        series[3].hide();

      plot->replot();
    };

    // Event Handler for the I checkbox.
    //
    // Revision:		1.00
    // Date:				17 July 2010
    // Written by:	Gavin Blakeman

    void CWindowLightCurves::eventCheckIChanged(int state)
    {
      if (state == Qt::Checked)
        series[4].show();
      else
        series[4].hide();

      plot->replot();
    };

    void CWindowLightCurves::eventJDMinChanged(double d)
    {
      double JDMax = sbMax->value();
      plot->setAxisScale(QwtPlot::xBottom, d, JDMax);
      plot->replot();
    };

    void CWindowLightCurves::eventJDMaxChanged(double d)
    {
      double JDMin = sbMin->value();
      plot->setAxisScale(QwtPlot::xBottom, JDMin, d);
      plot->replot();
    };

    //*****************************************************************************************************************************
    //
    // CPhotometryObjectEditDialog
    // Displays information on the objects in the Photometry Program
    //
    //*****************************************************************************************************************************

#define ROLE_FILTERSELECT  Qt::UserRole + 1

    // Class constructor. Creates and associates the widget and then calls the setupDialog
    // function.
    //
    // Revision:		1.00
    // Written by:	Gavin Blakeman
    // Date:				27 June 2010

    CPhotometryObjectEditDialog::CPhotometryObjectEditDialog(QVariant *vProg, QVariant *vEdit)
      : CDialog(":/forms/dialogPhotometryEditTarget.ui"), vPhotObjID(vEdit), vProgram(vProg)
    {
      setupUI();

      setupDialog();
    };

    CPhotometryObjectEditDialog::~CPhotometryObjectEditDialog(void)
    {
    };

    // Responds when the user selects an filter for the observation.
    //
    // Revision:  1.00
    // Date:      18 June 2011

    void CPhotometryObjectEditDialog::eventFilterSelected(QListWidgetItem *lwi)
    {
      QIcon iconChecked(":/images/BMP_CHECKED.bmp");
      QIcon iconUnchecked(":/images/BMP_UNCHECKED.bmp");

      QVariant vBool = lwi->data(ROLE_FILTERSELECT);

      if (vBool.toBool())
      {
        lwi->setIcon(iconUnchecked);
        vBool = QVariant((bool) false);
      }
      else
      {
        lwi->setIcon(iconChecked);
        vBool = QVariant((bool) true);
      };
      lwi->setData(ROLE_FILTERSELECT, vBool);

    };

    // Adds the relevant information into the fields of the dialog.
    //
    // Revision:		1.00
    // Date:
    // Written by:	Gavin Blakeman

    void CPhotometryObjectEditDialog::setupDialog(void)
    {
      QString szSQL = QString(
        "SELECT TBL_PHOTOBJECTS.OBSINTERVAL, TBL_PHOTOBJECTS.COMP_ID, TBL_PHOTOBJECTS.CHECK_ID, TBL_NAMES.NAME " \
        "FROM TBL_PHOTOBJECTS INNER JOIN TBL_NAMES ON TBL_PHOTOBJECTS.NAME_ID = TBL_NAMES.NAME_ID " \
        "WHERE TBL_PHOTOBJECTS.PHOTOBJECT_ID = %1").arg(vPhotObjID->toString());
      std::auto_ptr<QSqlQuery> query(new QSqlQuery(database::databaseATID->database()));
      QDate date;
      QList<QListWidgetItem *> lwItems;
      QList<QListWidgetItem *>::iterator iter;
      QIcon iconChecked(":/images/BMP_CHECKED.bmp");
      QIcon iconUnchecked(":/images/BMP_UNCHECKED.bmp");

      query->exec(* new QString(szSQL));

      query->first();

        // Target, comparison and check stars

      szSQL = query->value(3).toString();

      dlg->findChild<QLineEdit *>("leTargetObject")->setText(query->value(3).toString());
      //database::databaseATID->PopulatePhotometryStandardCombo(cbComparison, vPhotObjID, &query->value(1));
      //database::databaseATID->PopulatePhotometryStandardCombo(cbCheck, vPhotObjID, &query->value(2));

      sbObservationInterval->setValue(query->value(0).toInt());

        // Count the observations

      szSQL = QString("SELECT TBL_PHOT_OBSRECORD.JD " \
                      "FROM TBL_PHOTOBJECTS INNER JOIN TBL_PHOT_OBSRECORD ON TBL_PHOTOBJECTS.PHOTOBJ_ID = TBL_PHOT_OBSRECORD.PHOTOBJ_ID " \
                      "WHERE (((TBL_PHOTOBJECTS.OBSPROG_ID)=%1) AND ((TBL_PHOTOBJECTS.TARGET_ID)=%2)) " \
                      "ORDER BY TBL_PHOT_OBSRECORD.JD DESC").arg(vProgram->toString()).arg(vPhotObjID->toString());
      query->exec(QString(szSQL));

      if (!query->first())
      {	// No records

        leLastObservation->setText(tr("No Observations"));
        date = QDate::currentDate();
        date = date.addDays(query->value(0).toInt());
        deNextObservation->setDate(date);
      }
      else
      {	// Records exist.
        date = QDate().fromJulianDay(query->value(0).toDouble());
        leLastObservation->setText(date.toString());
        date = date.addDays(query->value(0).toInt());
        deNextObservation->setDate(date);
      };

        // Now add the filters to the filter list.

      database::databaseATID->PopulateFiltersList(lwFilters, iconUnchecked);

      szSQL = QString("SELECT TBL_FILTERS.FILTER_ID, TBL_FILTERS.SHORTTEXT " \
        "FROM TBL_PHOTOBJFILTER INNER JOIN TBL_FILTERS ON TBL_PHOTOBJFILTER.FILTER_ID = TBL_FILTERS.FILTER_ID " \
        "WHERE TBL_PHOTOBJFILTER.PHOTOBJECT_ID = %1 " \
        "ORDER BY TBL_FILTERS.FILTER_ID").arg(vPhotObjID->toString());
      query->exec(szSQL);

      while (query->next())
      {
        lwItems = lwFilters->findItems(QString(query->value(1).toString()), Qt::MatchCaseSensitive | Qt::MatchFixedString);
        if (lwItems.size() > 1)
        {
          CODE_ERROR(astroManager);
        }
        else
        {
          iter = lwItems.begin();
          (*iter)->setIcon(iconChecked);
          (*iter)->setData(ROLE_FILTERSELECT, QVariant(true));
        };
      };
    }

    /// @brief Data in the dialog must be validated and saved.
    //
    // 2011-06-18/GGB - Function created.

    void CPhotometryObjectEditDialog::eventSaveClicked(void)
    {
      QString szComp;
      QString szCheck;
      QVariant vCheck, vComp;
      QDate dNext;
      int nInterval;
      int filterCount = lwFilters->count();
      int index;
      QListWidgetItem *lwi;
      QVariant vFilter, vBool;
      QString szSQL;
      boost::scoped_ptr<QSqlQuery> query(new QSqlQuery(database::databaseATID->database()));

        // Get the comparison and check stars (if any)

      szComp = cbComparison->currentText();
      szCheck = cbCheck->currentText();
      vComp = database::GetObjectID(szComp);
      vCheck = database::GetObjectID(szCheck);
      nInterval = sbObservationInterval->value();
      dNext = deNextObservation->date();

        // Save the data. Update the record.

      szSQL = QString( \
        "UPDATE TBL_PHOTOBJECTS " \
        "SET TBL_PHOTOBJECTS.OBSINTERVAL = %1, TBL_PHOTOBJECTS.COMP_ID = '%2', TBL_PHOTOBJECTS.CHECK_ID = '%3'" \
        "WHERE TBL_PHOTOBJECTS.PHOTOBJECT_ID = %4"). \
        arg(nInterval).arg(vComp.toString()).arg(vCheck.toString()).arg(vPhotObjID->toString());
      query->exec(szSQL);

        // Get and save the filters that have been selected.
        // Filters that are being used must be checked that they are in the list.
        // Filters that are not being used must be checked that they are not in the list!

      for (index = 0; index < filterCount; index++)
      {
        lwi = lwFilters->takeItem(0);   // Take the first row.
        vBool = lwi->data(ROLE_FILTERSELECT);
        vFilter = lwi->data(database::ROLE_FILTERID);

        szSQL = QString( \
          "SELECT pof.PHOTOBJECT_ID " \
          "FROM TBL_PHOTOBJFILTER pof " \
          "WHERE pof.PHOTOBJECT_ID = %1 AND pof.FILTER_ID = %2").arg(vPhotObjID->toString()).arg(vFilter.toString());
        query->exec(szSQL);

        if (vBool.toBool())
        {
            // Filter should be in the list
            // Check if the filter and PhotObjID combination already exists.

          if (!query->first())
          {
              // Row does not exist.

            szSQL = QString( \
              "INSERT INTO TBL_PHOTOBJFILTER ( PHOTOBJECT_ID, FILTER_ID ) " \
              "SELECT %1 AS Expr1, %2 AS Expr2").arg(vPhotObjID->toString()).arg(vFilter.toString());
            query->exec(szSQL);
          };
        }
        else
        {
            // Filter should not be in the list.
            // Check that the filter combination does not exist.

          if (query->first())
          {
              // Row does exist. Should be deleted.

            szSQL = QString( \
              "DELETE TBL_PHOTOBJFILTER " \
              "FROM TBL_PHOTOBJFILTER " \
              "WHERE TBL_PHOTOBJFILTER.PHOTOBJECT_ID=%1 AND TBL_PHOTOBJFILTER.FILTER_ID=%2"). \
              arg(vPhotObjID->toString()).arg(vFilter.toString());
            query->exec(szSQL);
          };
        };

        delete lwi;
        lwi = NULL;
      };
      dlg->accept();
    };

    // Processes the close push button.
    //
    // Revision:		1.00
    // Date:				10 July 2010

    void CPhotometryObjectEditDialog::eventCloseClicked(void)
    {
      QMessageBox msgBox;

      if (isDirty)
      {
        msgBox.setText(tr("There is unsaved data."));
        msgBox.setInformativeText(tr("Save or Discard Data?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Save);
        if (msgBox.exec() == QMessageBox::Save)
        {
            // Save the data

          eventSaveClicked();
        }
        else
          dlg->reject();
      }
      else
        dlg->reject();
    }

    /// @brief Sets up the user interface as required.
    ///
    /// @version 2017-06-14/GGB - Updated to Qt5
    /// @version 2011-06-18/GGB - Function created.

    void CPhotometryObjectEditDialog::setupUI()
    {
      ASSOCIATE_CONTROL(sbObservationInterval, dlg, "sbObservationInterval", QSpinBox);
      deNextObservation = dlg->findChild<QDateEdit *>("deNextObservation");
      cbComparison = dlg->findChild<QComboBox *>("cbComparison");
      cbCheck = dlg->findChild<QComboBox *>("cbCheck");
      leLastObservation = dlg->findChild<QLineEdit *>("leLastObservation");
      lwFilters = dlg->findChild<QListWidget *>("listWidgetFilters");

      connect(dlg->findChild<QPushButton *>("btnClose"), SIGNAL(clicked(void)), this, SLOT(eventCloseClicked(void)));
      connect(dlg->findChild<QPushButton *>("btnSave"), SIGNAL(clicked(void)), this, SLOT(eventSaveClicked(void)));

      connect(lwFilters, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(eventFilterSelected(QListWidgetItem *)));
    }

  /*
    //****************************************************************************
    //
    // CVariableStarSelectionDialog
    // Allows the selection of a variable star or selection of variable stars to
    // be selected.
    // Any stars added are added directly into the database. The calling classes
    // must refresh their lists.
    //
    //***************************************************************************

    // Class constructor
    //
    // Revision:		1.00
    // Written by:	Gavin Blakeman
    // Date:				11 January 2010

    CVariableStarSelectionDialog::CVariableStarSelectionDialog(CWindow *aParent,
                                                               _variant_t *ppid,
                                                               _variant_t *rank) : CDialog(aParent)
    {
      vtRankAfter = rank;
      vtPhotProgID = ppid;
    };

    LRESULT CVariableStarSelectionDialog::WMInitDialog(HWND, UINT, WPARAM, LPARAM)
    {
      char szText[256];
      _RecordsetPtr pRst("ADODB.RECORDSET");
      char szSQL[1024] = "SELECT st.SITEDESCRIPTION FROM TBL_SITES st WHERE st.DONTDISPLAY <> 1";
      _variant_t vtSiteDescription;

      try
      {
        CheckDlgButton(IDB_RARANGE, BST_UNCHECKED);
        CheckDlgButton(IDB_DECRANGE, BST_UNCHECKED);
        CheckDlgButton(IDB_VMAGRANGE, BST_UNCHECKED);
        CheckDlgButton(IDB_NAMESEARCH, BST_UNCHECKED);
        CheckDlgButton(IDB_ALTMIN, BST_UNCHECKED);
        SetDlgItemTxt(IDE_RAMIN, "00:00:00");
        SetDlgItemTxt(IDE_RAMAX, "23:59:59");
        SetDlgItemTxt(IDE_DECMIN, "-90:00:00");
        SetDlgItemTxt(IDE_DECMAX, "90:00:00");
        SetDlgItemTxt(IDE_MAGMAX, "-2");
        SetDlgItemTxt(IDE_MAGMIN, "14");
        SetDlgItemTxt(IDE_NAME, "*");
        SetDlgItemTxt(IDE_ALTMIN, "45");

          // Now fill the combo box

        pRst->CursorLocation = adUseServer;
        pRst->Open(szSQL, _variant_t((IDispatch *)pAstroDBConnection,true), adOpenStatic, adLockReadOnly, adCmdText);

        while (!pRst->EndOfFile)
        {
          vtSiteDescription = pRst->Fields->GetItem((long) 0)->GetValue();
          strcpy_s(szText, sizeof(szText), (char *) ((_bstr_t) vtSiteDescription));
          SendDlgItemMsg(IDC_SITE, CB_ADDSTRING, (WPARAM) 0, (LPARAM) szText);
          pRst->MoveNext();
        };
        pRst->Close();
      }
      catch(_com_error &e)
      {
        // Notify the user of errors if any.
        // Pass a connection pointer accessed from the Connection.

        PrintComError(e);
      };
      return (TRUE);

    };

    // Processes the WM_COMMAND message.
    //
    // Revision:		1.00
    // Written by:	Gavin Blakeman
    // Date:				12 January 2010

    LRESULT CVariableStarSelectionDialog::WMCommand(HWND, UINT, WPARAM wParam, LPARAM)
    {

      switch (LOWORD(wParam))
      {
      case IDB_SEARCH:
        IDBSearch();
        return TRUE;
      case IDB_ADDALL:
        IDBAddAll();
        return TRUE;
      case IDB_ADDSELECTED:
        IDBAddSelected();
        return TRUE;
      case IDB_CLOSEWINDOW:
        EndDialog(FALSE);
        return TRUE;
      };
      return TRUE;
    };

    // Selects all variable stars based on the criteria specified by the user.
    //
    // Revision:		1.00
    // Written by:
    // Date:

    void  CVariableStarSelectionDialog::IDBSearch(void)
    {
      int nError = 0;
      //const char *(szColumns[7]);
      _RecordsetPtr pRst("ADODB.RECORDSET");
      char szSQL[1024] = "";
      _variant_t vtName;
      _variant_t vtRA;
      _variant_t vtObjectType;
      _variant_t vtDec;
      _variant_t vtVMag;
      _variant_t vtVariable;
      _variant_t vtExoPlanets;
      _variant_t vtPhotometryCheck;
      _variant_t vtMultiple;
      int nRALimits = 0;
      int nDecLimits = 0;
      int nMagLimits = 0;
      int nNameSpec = 0;
      int nSiteSpec = 0;
      char szBuffer[256] = "\0";
      double dRAMin, dRAMax;
      double dDecMin, dDecMax;
      float fMagMax, fMagMin;
      float fAltMin;
      char szName[256];
      char szSite[256];
      LRESULT lrCurrentSel;

      try
      {
        if (IsDlgButtonChecked(IDB_RARANGE))
        {
          nRALimits = -1;
          GetDlgItemTxt(IDE_RAMIN, szBuffer, sizeof(szBuffer));
          if (sscanfRA(szBuffer, &dRAMin) <= 0)
          {
            FieldError(IDE_RAMIN);
            nError = -1;
          }
          else
          {
            GetDlgItemTxt(IDE_RAMAX, szBuffer, sizeof(szBuffer));
            if (sscanfRA(szBuffer, &dRAMax) <= 0)
            {
              nError = -1;
              FieldError(IDE_RAMAX);
            }
            else if (dRAMin > dRAMax)
              MessageBox(hWindow, "Minimum RA must be less than Maximum RA.", "Error", MB_ICONERROR | MB_APPLMODAL | MB_OK);
          };
        };
        if ( (IsDlgButtonChecked(IDB_DECRANGE) && !nError) )
        {
          nDecLimits = -1;			// There are declination limits.
          GetDlgItemTxt(IDE_DECMIN, szBuffer, sizeof(szBuffer));
          if (sscanfDec(szBuffer, &dDecMin) <= 0)
          {
            FieldError(IDE_DECMIN);
            nError = -1;
          }
          else
          {
            GetDlgItemTxt(IDE_DECMAX, szBuffer, sizeof(szBuffer));
            if (sscanfDec(szBuffer, &dDecMax) <= 0)
            {
              FieldError(IDE_DECMIN);
              nError = -1;
            }
            else if (dDecMin > dDecMax)
              MessageBox(hWindow, "Minimum Dec must be less than Maximum Dec.", "Error", MB_ICONERROR | MB_APPLMODAL | MB_OK);
          };
        };
        if ( (IsDlgButtonChecked(IDB_VMAGRANGE) && !nError) )
        {
          nMagLimits = -1;
          if (!GetDlgItemFloat(IDE_MAGMAX, &fMagMax))
          {
            FieldError(IDE_MAGMAX);
            nError = -1;
          }
          else if (!GetDlgItemFloat(IDE_MAGMIN, &fMagMin))
          {
            FieldError(IDE_MAGMIN);
            nError = -1;
          }
          else if (fMagMax > fMagMin)
            MessageBox(hWindow, "Minimum Magnitude must be less that Maximum Magnitude.", "Error", MB_ICONERROR | MB_APPLMODAL | MB_OK);
        };
        if ( IsDlgButtonChecked(IDB_NAMESEARCH) && !nError)
        {
          GetDlgItemTxt(IDE_NAME, szName, sizeof(szName));
        };
        if ( (IsDlgButtonChecked(IDB_ALTMIN) && !nError))
        {
          lrCurrentSel = SendDlgItemMsg(IDC_SITE, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
          SendDlgItemMsg(IDC_SITE, CB_GETLBTEXT, (WPARAM) lrCurrentSel, (LPARAM) szSite);
          if (!GetDlgItemFloat(IDE_ALTMIN, &fAltMin))
          {
            FieldError(IDE_ALTMIN);
            nError = -1;
          };
        };
      }
      catch(_com_error &e)
      {
        // Notify the user of errors if any.
        // Pass a connection pointer accessed from the Connection.

        PrintComError(e);
      };
    };


    void  CVariableStarSelectionDialog::IDBAddAll(void)
    {
    };


    void  CVariableStarSelectionDialog::IDBAddSelected(void)
    {
    }; */

    //************************************************************************************************
    //
    // CDialogPhotometryObservationEntry
    //
    //************************************************************************************************

  bool CDialogPhotometryObservationEntry::bLT = true;		// default to local time.

    // Class constructor
    //
    // Revision:		2.00
    // Date:				28 May 2010
    // By:					Gavin Blakeman
    // Changes:			1) Changed to use Qt API.
    //
    // Revision:		1.00
    // Written by:	Gavin Blakeman
    // Date:				17 January 2010

    CDialogPhotometryObservationEntry::CDialogPhotometryObservationEntry(void)
    {
      cbSite = cbObserver = cbTelescope = NULL;
      leJD = NULL;
      cbInstrument = cbFilter = cbStandardFilters = NULL;
      dtDate = NULL;
      rbLT = rbUTC = NULL;
      parent = NULL;
      targetObject = NULL;
      isDirty = false;
    };

    // Constructor taking a target object.
    // Creates a new copy of the target object variant.
    //
    // Revision:		1.00
    // Date:				29 May 2010
    // Witten by:		Gavin Blakeman

    CDialogPhotometryObservationEntry::CDialogPhotometryObservationEntry(QVariant *to)
    {
      cbSite = cbObserver = cbTelescope = NULL;
      leJD = NULL;
      cbInstrument = cbFilter = cbStandardFilters = NULL;
      dtDate = NULL;
      rbLT = rbUTC = NULL;
      parent = NULL;
      targetObject = new QVariant(*to);
      isDirty = false;
    };

    // Ensures that any dynamically allocated memory is freed when the class is destroyed.
    //
    // Revision:		1.00
    // Date:				29 May 2010
    // Written by:	Gavin Blakeman

    CDialogPhotometryObservationEntry::~CDialogPhotometryObservationEntry(void)
    {
      if (targetObject)
      {
        delete targetObject;
        targetObject = NULL;
      };
      isDirty = false;
    };

    // Sets the parent of the instance.
    //
    // Revision:		1.00
    // Date:				29 May 2010
    // Written by:	Gavin Blakeman

    void CDialogPhotometryObservationEntry::setParent(QDialog *newParent)
    {
      parent = newParent;
      setupDialog();
    };

    // Sets up the dialog. Fills in all required fields.
    //
    // Revision:		1.00
    // Date:
    // Written by:	Gavin Blakeman

    void CDialogPhotometryObservationEntry::setupDialog(void)
    {
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      QVariant vObjectName;

      cbSite = parent->findChild<QComboBox *>("cbSite");
      cbObserver = parent->findChild<QComboBox *>("cbObserver");
      cbTelescope = parent->findChild<QComboBox *>("cbTelescope");
      cbInstrument = parent->findChild<QComboBox *>("cbInstrument");
      cbFilter = parent->findChild<QComboBox *>("cbFilter");
      cbStandardFilters = parent->findChild<QComboBox *>("cbStandardFilters");

      leJD = parent->findChild<QLineEdit *>("leJD");

      rbLT = parent->findChild<QRadioButton *>("rbLT");
      rbUTC = parent->findChild<QRadioButton *>("rbUTC");
      connect(rbUTC, SIGNAL(toggled(bool)), this, SLOT(eventUTCButton(bool)));

      leTarget = parent->findChild<QLineEdit *>("leTarget");

      cbCheck = parent->findChild<QComboBox *>("cbCheck");
      PopulateStandardsCombo(cbCheck);

      cbComparison = parent->findChild<QComboBox *>("cbComparison");
      PopulateStandardsCombo(cbComparison);

      leSMTarget = parent->findChild<QLineEdit *>("leSMTarget");
      leSMComparison = parent->findChild<QLineEdit *>("leSMComparison");
      leSMCheck = parent->findChild<QLineEdit *>("leSMCheck");
      leSETarget = parent->findChild<QLineEdit *>("leSETarget");
      leSEComparison = parent->findChild<QLineEdit *>("leSEComparison");
      leSECheck = parent->findChild<QLineEdit *>("leSECheck");

      leIMTarget = parent->findChild<QLineEdit *>("leIMTarget");
      leIMComparison = parent->findChild<QLineEdit *>("leIMComparison");
      leIMCheck = parent->findChild<QLineEdit *>("leIMCheck");
      leIETarget = parent->findChild<QLineEdit *>("leIETarget");
      leIEComparison = parent->findChild<QLineEdit *>("leIEComparison");
      leIECheck = parent->findChild<QLineEdit *>("leIECheck");

      btnSave = parent->findChild<QPushButton *>("btnSave");
      connect(btnSave, SIGNAL(clicked(void)), this, SLOT(eventSaveClicked(void)));

      btnClose = parent->findChild<QPushButton *>("btnClose");
      connect(btnClose, SIGNAL(clicked(void)), this, SLOT(eventCloseClicked(void)));

      btnHelp = parent->findChild<QPushButton *>("btnHelp");
      connect(btnHelp, SIGNAL(clicked(void)), this, SLOT(eventHelpClicked(void)));

      btnClear = parent->findChild<QPushButton *>("btnClear");
      connect(btnClear, SIGNAL(clicked(void)), this, SLOT(eventClearClicked(void)));


      dtDate = parent->findChild<QDateTimeEdit *>("dtDate");
      if (bLT)
      {
        rbLT->setChecked(true);
        dtDate->setDateTime(*new QDateTime(QDateTime::currentDateTime().toLocalTime()));
      }
      else
      {
        rbUTC->setChecked(true);
        dtDate->setDateTime(*new QDateTime(QDateTime::currentDateTime().toUTC()));
      };
      connect(dtDate, SIGNAL(dateTimeChanged(const QDateTime &)), this, SLOT(eventDateChanged(const QDateTime &)));

      if (database::databaseARID)
      {
        database::databaseARID->populateComboSite(cbSite, true);
        database::databaseARID->PopulateObserverCombo(cbObserver, true);
        database::databaseARID->PopulateTelescopeCombo(cbTelescope, true);
        database::databaseARID->PopulateInstrumentCombo(cbInstrument, true);
        database::databaseARID->PopulateFiltersAvailableCombo(cbFilter);
        database::databaseARID->populateStandardFiltersCombo(cbStandardFilters);
      };

        // If the target object is specified, search for the relevant name.

      if (targetObject)
      {
        szSQL = "SELECT ob.PreferredName FROM TBL_STELLAROBJECTS ob WHERE ob.OBJECT_ID= " + targetObject->toString();
        query.exec(*new QString(szSQL));
        if (!query.first())
        {	// No preferred name.
          szSQL = QString("SELECT TBL_NAMES.Name " \
                          "FROM TBL_CATALOGUEORDER INNER JOIN TBL_NAMES ON TBL_CATALOGUEORDER.CATALOGUE_ID = TBL_NAMES.CATALOGUE_ID " \
                          "WHERE ((TBL_NAMES.STELLAROBJECT_ID= %1) AND (TBL_CATALOGUEORDER.OBSERVER_ID=%2)) " \
                          "ORDER BY TBL_CATALOGUEORDER.SORTORDER").arg(targetObject->toString()).arg(settings::astroManagerSettings->value("Observer", QString("0")).toString());
        }
        else
        {	// Preferred name
          szSQL = "SELECT nm.NAME FROM TBL_NAMES nm WHERE nm.NAME_ID= " + query.value(0).toString();
        };
        query.exec(*new QString(szSQL));
        query.exec(*new QString(szSQL));
          if (query.first())
          {
            leTarget->setText(*new QString(query.value(0).toString()));
          }
          else
          {
            leTarget->setText(*new QString(""));
          };
      };
    };

    // Handles the event from the UTC radio button to indicate that it has been
    // checked/unchecked.
    //
    // Revision:		1.00
    // Date:				29 May 2010
    // Written by:	Gavin Blakeman

    void CDialogPhotometryObservationEntry::eventUTCButton(bool btnState)
    {
      if (btnState)
        bLT = false;
      else
        bLT = true;
    };

    // Fills the passed combo box with comparison/check stars within 1 degree of the target object.
    // Comparison/Check stars are:
    //	1) Primary Standards
    //	2) Johnson standards
    //	3) Cousings standards
    //	4) Landolt standards
    //	5) AAVSO standards
    //
    // Revision:		1.00
    // Date:
    // Written  by:

    void CDialogPhotometryObservationEntry::PopulateStandardsCombo(QComboBox *combo)
    {
      QString szSQL;
      QSqlQuery query(database::databaseATID->database());
      double dRA, dDEC;
      double dRAmax, dRAmin, dDECmax, dDECmin;
      QString arg1, arg2, arg3, arg4;

      if (targetObject)
      {
          // Get the coordinates of the target object. (Don't precess)

        szSQL = "SELECT o.RA, o.DEC FROM TBL_STELLAROBJECTS o WHERE o.OBJECT_ID = " + targetObject->toString();
        query.exec(*new QString(szSQL));
        query.first();
        dRA = query.value(0).toDouble();
        dDEC = query.value(1).toDouble();
        dRA /= 10000;
        dDEC /= 10000;
        dRA = ACL::dms2deg(dRA*15);		// Convert from dms to deg
        dDEC = ACL::dms2deg(dDEC);
        dRAmax = dRA + .5;			// 1 degree window.
        dRAmin = dRA - .5;
        dDECmax = dDEC + .5;
        dDECmin = dDEC - .5;
        if (dRAmax > 360)
          dRAmax -= 360;
        if (dRAmin < 0)
          (dRAmin += 360);
        dRAmax = ACL::deg2dms(dRAmax) * 10000/15;
        dRAmin = ACL::deg2dms(dRAmin) * 10000/15;
        dDECmax = ACL::deg2dms(dDECmax) * 10000;
        dDECmin = ACL::deg2dms(dDECmin) * 10000;

        arg1 = QVariant(dRAmin).toString();
        arg2 = QVariant(dRAmax).toString();
        arg3 = QVariant(dDECmin).toString();
        arg4 = QVariant(dDECmax).toString();

        szSQL = QString("SELECT TBL_NAMES.NAME, TBL_STELLAROBJECTS.OBJECT_ID " \
                      "FROM TBL_NAMES INNER JOIN TBL_STELLAROBJECTS ON TBL_NAMES.NAME_ID = TBL_STELLAROBJECTS.PreferredName " \
                      "WHERE (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.AAVSOStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.LandoltStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.PrimaryStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.JohnsonStandard)=True)) OR (((TBL_STELLAROBJECTS.RA)>%1 And (TBL_STELLAROBJECTS.RA)<%2) AND ((TBL_STELLAROBJECTS.DEC)>%3 And (TBL_STELLAROBJECTS.DEC)<%4) AND ((TBL_STELLAROBJECTS.CousinsStandard)=True))").arg(arg1).arg(arg2).arg(arg3).arg(arg4);
        query.exec(*new QString(szSQL));

        while (query.next())
          combo->addItem(*new QString(query.value(0).toString()), *new QVariant(query.value(1)));
      };

    };

    // Handles the event for the "Save Data" button being clicked.
    // Calls the SaveData() function.
    //
    // Revision:		1.00
    // Date:				5 June 2010
    // Written by:	Gavin Blakeman

    void CDialogPhotometryObservationEntry::eventSaveClicked(void)
    {
      SaveData();
    };

    // Called when the dialog is to be closed.
    // Checks if the dialog is dirty and asks the user if necessary if the data should be saved
    //
    // Revision:		1.00
    // Date:				5 June 2010
    // Written by:	Gavin Blakeman

    void CDialogPhotometryObservationEntry::eventCloseClicked(void)
    {
      QMessageBox msgBox;

      if (isDirty)
      {
        msgBox.setText(tr("There is unsaved data."));
        msgBox.setInformativeText(tr("Save or Discard Data?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Save);
        if (msgBox.exec() == QMessageBox::Save)
        {	// Save the data
          SaveData();
        };
      };
      parent->done(0);
    };

    void CDialogPhotometryObservationEntry::eventHelpClicked(void)
    {
    };

    void CDialogPhotometryObservationEntry::eventClearClicked(void)
    {
    }

    void CDialogPhotometryObservationEntry::SaveData(void)
    {
    };

    // Updates the JulianDay number if the datetime control is changed.
    //
    // 2010-06-05/GGB - Function created.

    void CDialogPhotometryObservationEntry::eventDateChanged(const QDateTime &dateTime)
    {
      time_t time = dateTime.toTime_t();
      ACL::TJD JD(gmtime(&time));		// Convert to JD.
      char szJD[100];

      JD.JD(szJD, sizeof(szJD), 6);
      leJD->setText(QString(szJD));
    }

    //***********************************************************************************************
    //
    // CPhotometryObjectDialog
    //
    //***********************************************************************************************

    CPhotometryObjectDialog::CPhotometryObjectDialog() : CDialog("forms/dialogPhotometryObject.ui")
    {
    };

    CPhotometryObjectDialog::~CPhotometryObjectDialog(void)
    {
    };

  };	// namespace photometry

};  // namespace AstroManager
