//*************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:			          Photometry.H
// SUBSYSTEM:           Photometry Management Windows
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// NAMESPACE:						AstroManager::photometry
// AUTHOR:              Gavin Blakeman
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

#ifndef PHOTOMETRY_H
#define PHOTOMETRY_H

#include <QCL>

#include "qwt_plot.h"
#include "qwt_plot_curve.h"

#include "dialogs/dialogs.h"

namespace astroManager
{
  namespace photometry
  {

    //************************************************************************************************
    //
    // Class for plotting light curves
    //
    //************************************************************************************************

    struct SLCData;

    class CWindowLightCurves : public QMdiSubWindow
    {
      Q_OBJECT

    private:
      QwtPlot *plot;
      QwtPlotCurve *series;
      QComboBox *cbObject;
      QCheckBox *cbU, *cbB, *cbV, *cbR, *cbI;
      QDoubleSpinBox *sbMin, *sbMax;
      SLCData *LCData;
      int LCCount;

      void PopulateCombo(void);


    protected:
    public:
      CWindowLightCurves(QWidget * = NULL);
      virtual ~CWindowLightCurves(void);

    public slots:
      void eventComboIndexChanged(int);
      void eventCheckUChanged(int);
      void eventCheckBChanged(int);
      void eventCheckVChanged(int);
      void eventCheckRChanged(int);
      void eventCheckIChanged(int);
      void eventJDMinChanged(double);
      void eventJDMaxChanged(double);
    };


    // **********************************************************************************************
    //
    // Dialogs dedicated to Photometry
    //
    //***********************************************************************************************

    class CPhotometryObjectEditDialog : public dialogs::CDialog
    {
      Q_OBJECT

      struct SFilter
      {
        int FilterID;
        bool selected;
      };

    private:
      bool isDirty;
      QVariant *vPhotObjID;           // PHOTOBJECT_ID of the item to edit.
      QVariant *vProgram;         // OBSPROG_ID of the program being edited.
      QComboBox *cbComparison;
      QComboBox *cbCheck;
      QSpinBox *sbObservationInterval;
      QListWidget *lwFilters;
      QLineEdit *leLastObservation;
      QDateEdit *deNextObservation;

      void setupDialog();
      void setupUI();

    protected:
    public:
      CPhotometryObjectEditDialog(QVariant *, QVariant *);
      virtual ~CPhotometryObjectEditDialog(void);

    private slots:
      void eventCloseClicked(void);
      void eventSaveClicked(void);
      void eventFilterSelected(QListWidgetItem *);
    };

    class CPhotometryObjectDialog : public dialogs::CDialog
    {
    private:
      //_variant_t *vtRankAfter;
      //_variant_t *vtPhotProgID;

      //void IDBSearch(void);
      //void IDBAddAll(void);
      //void IDBAddSelected(void);

    protected:
    public:
      CPhotometryObjectDialog();
      virtual ~CPhotometryObjectDialog(void);

      //virtual LRESULT WMInitDialog(HWND, UINT, WPARAM, LPARAM);
      //virtual LRESULT WMCommand(HWND, UINT, WPARAM, LPARAM);
    };

    class CDialogPhotometryObservationEntry : public QObject
    {
      Q_OBJECT

    private:
      static bool bLT;
      bool isDirty;

      QLineEdit *leJD;
      QComboBox *cbSite;
      QComboBox *cbObserver;
      QComboBox *cbTelescope;
      QComboBox *cbInstrument;
      QComboBox *cbFilter;
      QComboBox *cbStandardFilters;
      QDateTimeEdit *dtDate;
      QLineEdit *leTarget;
      QDialog *parent;
      QRadioButton *rbLT;
      QRadioButton *rbUTC;
      QComboBox *cbComparison;
      QComboBox *cbCheck;

      QLineEdit *leSMTarget;
      QLineEdit *leSMComparison;
      QLineEdit *leSMCheck;
      QLineEdit *leSETarget;
      QLineEdit *leSEComparison;
      QLineEdit *leSECheck;

      QLineEdit *leIMTarget;
      QLineEdit *leIMComparison;
      QLineEdit *leIMCheck;
      QLineEdit *leIETarget;
      QLineEdit *leIEComparison;
      QLineEdit *leIECheck;

      QPushButton *btnSave;
      QPushButton *btnClose;
      QPushButton *btnClear;
      QPushButton *btnHelp;

      QVariant *targetObject;

      void setupDialog(void);
      void PopulateStandardsCombo(QComboBox *);

      void SaveData(void);

    protected:
    public:
      CDialogPhotometryObservationEntry(void);
      CDialogPhotometryObservationEntry(QVariant *);

      ~CDialogPhotometryObservationEntry(void);

      void setParent(QDialog *);

      private slots:
        void eventUTCButton(bool);
        void eventSaveClicked(void);
        void eventCloseClicked(void);
        void eventClearClicked(void);
        void eventHelpClicked(void);
        void eventDateChanged(const QDateTime &);

    };

  }  // namespace photometry
}  // namespace AstroManager

#endif // PHOTOMETRY_H
