//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Configure.h
// SUBSYSTEM:						MDI Frame Window Class
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LANGUAGE:						C++
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::mdiframe
// AUTHOR:							Gavin Blakeman.
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
// OVERVIEW:
//						          Implements classes used for configuration, specifically configuration of database parameters.
//
// CLASSES INCLUDED:    CConfigureSitesWindow - Window to allow user to edit can configure observing sites.
//                      CConfigureObserversWindow - Window to allow user to add/edit observer information.
//                      CConfigureInstrumentsWindow -
//                      CConfigureTelescopesWindow -
//                      CConfigureSiteDialog - Dialog used with the above window for data entry.
//                      CConfigureObserverDialog - Dialog used to configure observer information.
//                      CConfigureInstrumentDialog -
//                      CConfigureTelescopeDialog -
//                      CConfigureDatabaseDriverDialog - Dialog used to configure the database driver parameters.
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CARPAASubWindow
//                          - CConfigureSitesWindow
//                          - CConfigureObserversWindow
//                          - CConfigureTelescopesWindow
//                          - CConfigureInstrumentsWindow
//
//                      QDialog
//                        - CDialog
//                          - CConfigureSiteDialog
//                          - CConfigureObserverDialog
//                          - CConfigureInstrumentDialog
//                          - CConfigureTelescopeDialog
//                          - CConfigureDatabaseDriverDialog
//
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-06-19 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "dialogs/dialogs.h"
#include "Error.h"
#include "FrameWindow.h"

  // Miscellaneous library header files

#include <GCL>
#include <QCL>

namespace astroManager
{
  namespace configure
  {
    class CConfigureCatalogWindow : public CMdiSubWindow
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;
      QPushButton *pushButtonMoveUp;
      QPushButton *pushButtonMoveDown;

      inline virtual bool save() {CODE_ERROR(astroManager); }
      inline virtual bool saveAs() {CODE_ERROR(astroManager); }

      void setupUI();

    protected:
    public:
      CConfigureCatalogWindow(QWidget * = 0);

      inline virtual ESubWindowType getWindowType() const { return SWT_CONFIGURE_CATALOGPREFS;}
      virtual void windowActivating() {}

    private slots:
      void eventMoveUp(bool);
      void eventMoveDown(bool);
      void eventRowSelected(int, int);

    };

    class CConfigureInstrumentsWindow : public CMdiSubWindow
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;

      inline virtual bool save() {CODE_ERROR(astroManager);}
      inline virtual bool saveAs() {CODE_ERROR(astroManager);}

      virtual void closeEvent(QCloseEvent *);

      void setupUI();

      void insertRow(int, QSqlQuery &);
      void redrawRow(int);

    protected:
    public:
      CConfigureInstrumentsWindow(QWidget * = 0);

      virtual void windowActivating();
      inline virtual ESubWindowType getWindowType() const { return SWT_CONFIGURE_INSTRUMENTS;}

    private slots:
      void eventRowSelected(int, int);

      void eventDontDisplay(bool);
      void eventCreateInstrument(bool);
    };

    class CConfigureObserversWindow : public CMdiSubWindow
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;

      inline virtual bool save() {CODE_ERROR(astroManager); }
      inline virtual bool saveAs() {CODE_ERROR(astroManager); }

      virtual void closeEvent(QCloseEvent *);

      void setupUI();

      void insertRow(int, QSqlQuery &);
      void redrawRow(int);

    protected:
    public:
      CConfigureObserversWindow(QWidget * = 0);

      inline virtual ESubWindowType getWindowType() const { return SWT_CONFIGURE_OBSERVERS;}
      virtual void windowActivating() {}

    private slots:
      void eventRowSelected(int, int);

      void eventDontDisplay(bool);
      void eventCreateObserver(bool);
    };

    class CConfigureSitesWindow : public CMdiSubWindow
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;

      void setupUI();

      inline virtual bool save() {CODE_ERROR(astroManager); }
      inline virtual bool saveAs() {CODE_ERROR(astroManager); }

      void insertRow(int, QSqlQuery &);

      virtual void closeEvent(QCloseEvent *);

      void redrawRow(int);

    protected:
    public:
      CConfigureSitesWindow(QWidget * = 0);

      inline virtual ESubWindowType getWindowType() const { return SWT_CONFIGURE_SITES;}
      virtual void windowActivating();

    private slots:
      void eventRowSelected(int, int);

      void eventDontDisplay(bool);
      void eventCreateSite(bool);
    };

    class CConfigureTelescopesWindow : public CMdiSubWindow
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;

      inline virtual bool save() {CODE_ERROR(astroManager); }
      inline virtual bool saveAs() {CODE_ERROR(astroManager); }

      virtual void closeEvent(QCloseEvent *);

      void setupUI();

      void insertRow(int, QSqlQuery &);
      void redrawRow(int);

    protected:
    public:
      CConfigureTelescopesWindow(QWidget * = 0);

      virtual void windowActivating() {}
      inline virtual ESubWindowType getWindowType() const { return SWT_CONFIGURE_TELESCOPES;}

    private slots:
      void eventRowSelected(int, int);

      void eventDontDisplay(bool);
      void eventCreateTelescope(bool);
    };

    class CConfigureInstrumentDialog : public dialogs::CDialog
    {
      Q_OBJECT
    private:
      QLineEdit *lineEditShortText;
      QLineEdit *lineEditManufacturer;
      QLineEdit *lineEditModel;
      QSpinBox *spinBoxNX;
      QSpinBox *spinBoxNY;
      QDoubleSpinBox *doubleSpinBoxDX;
      QDoubleSpinBox *doubleSpinBoxDY;
      QDoubleSpinBox *doubleSpinBoxADU;
      QCheckBox *checkBoxDontDisplay;

      QVariant &vInstrumentID;
      bool newRecord;
      void setupUI();

    protected:
    public:
      CConfigureInstrumentDialog(QVariant &, bool);

    private slots:
      void btnOk(bool);
      void btnCancel(bool) { dlg->reject(); }
    };

    class CConfigureObserverDialog : public dialogs::CDialog
    {
      Q_OBJECT

    private:
      QVariant &vObserverID;
      bool newRecord;

      QLineEdit *lineEditShortText;
      QLineEdit *lineEditAAVSOInitials;
      QCheckBox *checkBoxDontDisplay;

      void setupUI();

    protected:
    public:
      CConfigureObserverDialog(QVariant &, bool);

    private slots:
      void btnOk(bool);
      void btnCancel(bool) { dlg->reject(); }
    };

  }    // namespace configure
}    // namespace AstroManager

#endif // CONFIGURE_H
