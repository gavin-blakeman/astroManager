//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								Dialogs.h
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
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
// OVERVIEW:            Including all dialog classes.
//
// CLASSES INCLUDED:    CDialog                   - Virtual base class for dialogs.
//						          CSearchSelectObjectDialog -
//                      CImageFloatDialog         - Allow entry of parameters for floating an image.
//                      CImageResampleDialog      - Allow entry of parameters for resampling an image.
//
// CLASS HIERARCHY:     CDialog
//                        - CDialogEditResources
//                        - CSelectLocationDialog
//						            - CSearchSelectObjectDialog
//                        - CImageFloatDialog
//                        - CImageResampleDialog
//
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-03-22 GGB - AIRDAS 2013.03 release.
//                      2013-01-20 GGB - AIRDAS 0000.00 release.
//                      2012-01-28 GGB - Move CImageCalibrationDialog to it's own file.
//                      2011-06-04 GGB - Development of classes for AIRDAS
//
//*********************************************************************************************************************************

#ifndef DIALOGS_H
#define DIALOGS_H

  // Qt

#include "../qtExtensions/qt.h"

#include <string>
#include <GCL>

namespace AstroManager
{
  namespace dialogs
  {

  class CDialog : public QObject
  {
    Q_OBJECT

  private:
  protected:
    QDialog *dlg;

    template<typename T>
    T findChild(const QString &childName)
    {
      T returnValue = dlg->findChild<T>(childName);
      RUNTIME_ASSERT(AIRDAS, returnValue != nullptr, "Control " + childName.toStdString() + " not found." );

      return returnValue;
    }

  public:
    CDialog(QObject * = nullptr);
    CDialog(QString, QObject * = nullptr);
    ~CDialog();

    virtual int exec();

//  public slots:
//    virtual void accepted() {}
//    virtual void rejected() { std::cout << "CDialog::rejected called" << std::endl; }
  };

  enum ESSODOptions
  {
    SSOD_NONE = 0x00,
    SSOD_LIST = 0x01,						// From list tab
    SSOD_PHOTOMETRY = 0x02,			// From Photometry Programs tab
    SSOD_RECENT = 0x04					// From recently selected tab
  };

  class CSearchSelectObjectDialog : public CDialog
  {
    Q_OBJECT

  private:
    int dialogOptions;
    QVariant &vNameID;

    QTabWidget *tabWidget;
      QWidget *fromList;
        QCheckBox *checkObjectType;
        QComboBox *comboObjectType;
        QCheckBox *checkSpectralClass;
        QComboBox *comboSpectralClass;
        QCheckBox *checkConstellation;
        QComboBox *comboConstellation;
        QCheckBox *checkCatalog;
        QComboBox *comboCatalog;
        QCheckBox *checkMultiple;
        QCheckBox *checkExoPlanets;
        QLineEdit *editObjectName;
        QGroupBox *groupStarType;
          QRadioButton *radioPrimary;
          QRadioButton *radioJohnson;
          QRadioButton *radioCousins;
          QRadioButton *radioLandolt;
          QRadioButton *radioAAVSO;
          QRadioButton *radioVariable;
        QTableWidget *tableObjects;
        QPushButton *btnSearch;
      QWidget *fromPhotometry;
        QComboBox *comboPhotometry;
        QTableWidget *tablePhotometryObjects;
      QWidget *fromRecent;
        QTableWidget *tableRecentObjects;
    QPushButton *buttonSelect;
    QPushButton *buttonCancel;
    QPushButton *buttonHelp;

    void PopulateRecentObjects(void);

  protected:
  public:
    static int startTab;

    CSearchSelectObjectDialog(QVariant &, int);

  private slots:
    void eventBtnSearchClicked(bool);

    void eventCheckObjectTypeChanged(int);
    void eventCheckSpectralClassChanged(int);
    void eventCheckConstellationChanged(int);
    void eventCheckCatalogChanged(int);

    void eventButtonHelpClicked(bool);
    void eventButtonCancelClicked(bool);
    void eventButtonSelectClicked(bool);

    void eventTableObjectsCellClicked(int, int);

    void eventComboPhotometryCurrenINDEX_tChanged(int);
    void eventTablePhotometryCellClicked(int, int);

    void eventTableRecentCellClicked(int, int);

  };

    class CRotateImageDialog : public CDialog
    {
      Q_OBJECT

    private:
      double *angle;

    protected:
    public:
      CRotateImageDialog(double *);

    private slots:
      void btnOk(void);
      void btnCancel(void) { dlg->reject(); }
   };

    class CSelectSiteDialog : public dialogs::CDialog
    {
      Q_OBJECT
    private:
      QTableWidget *tableWidget;
      QVariant &siteID;

      void setupUI();

    protected:
    public:
      CSelectSiteDialog(QVariant &);

    private slots:
      void btnOk(bool);
      void btnCancel(bool) { dlg->reject(); }
    };

  }   // namespace dialogs
}   // namespace AstroManager

#endif // DIALOGS_H
