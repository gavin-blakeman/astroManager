//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								VSOPView
// SUBSYSTEM:						Object Information Child Window Class
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2009-2018 Gavin Blakeman.
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
// OVERVIEW:            Implements classes for the View Menu in the astroManager application
//
// CLASSES INCLUDED:    CObjectInfoWindow
//
// CLASS HIERARCHY:     QMdiSubWindow
//                        - CObjectInfoWindow
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-20 GGB - astroManager 2013.03 release.
//                      2013-01-22 GGB - astroManager 0000.00 release.
//                      2009-10-25 GGB - File created
//
//*********************************************************************************************************************************

#include "../Include/VSOPView.h"

  // Qt Includes

#include <QUiLoader>

  // astroManager files

#include "../Include/database/database.h"
#include "../Include/database/databaseATID.h"

  // Astronomy Class Library

#include <ACL>

namespace AstroManager
{

  /// @brief Constructor for the class.
  /// @param[in] parent - The parent that owns this instance.
  /// @throws None.
  /// @version 2009-10-25/GGB - Function created.

  CObjectInfoWindow::CObjectInfoWindow(QWidget *parent) : CMdiSubWindow(parent)
  {
    QUiLoader loader;
    int nRow;

    QFile file(":/forms/windowObjectInfo.ui");
    file.open(QFile::ReadOnly);

    QWidget *formWidget = loader.load(&file, this);
    file.close();

    setWidget(formWidget);

    buttonSearch = formWidget->findChild<QPushButton *>("buttonSearch");
    connect(buttonSearch, SIGNAL(clicked()), this, SLOT(eventSearch()));

      // General info tab items

    //ASSOCIATE_CONTROL(lineEditObjectName, formWidget, lineEditObjectName, QLineEdit);
    tbSelectedObject = formWidget->findChild<QLabel *>("tbSelectedObject");
    tbConstellation = formWidget->findChild<QLabel *>("tbConstellation");
    tbObjectType = formWidget->findChild<QLabel *>("tbObjectType");
    tbSpectralType = formWidget->findChild<QLabel *>("tbSpectralType");
    tbSpectrum = formWidget->findChild<QLabel *>("tbSpectrum");
    tbRedShift = formWidget->findChild<QLabel *>("tbRedShift");
    tbIsVariable = formWidget->findChild<QLabel *>("tbIsVariable");
    tbIsMultiple = formWidget->findChild<QLabel *>("tbIsMultiple");
    tbHasExoplanets = formWidget->findChild<QLabel *>("tbHasExoplanets");
    tbComments = formWidget->findChild<QLabel *>("tbComments");

      // Position information

    twPosition = formWidget->findChild<QTableWidget *>("twPosition");
    for (nRow = 0; nRow < 7; nRow++)
    {
      twPosition->setRowHeight(nRow, 20);
    };

      // Photometry tab items

    tbIsPrimaryStandard = formWidget->findChild<QLabel *>("tbIsPrimaryStandard");
    tbIsLandoltStandard = formWidget->findChild<QLabel *>("tbIsLandoltStandard");
    tbIsAAVSOStandard = formWidget->findChild<QLabel *>("tbIsAAVSOStandard");
    tbIsJohnsonStandard = formWidget->findChild<QLabel *>("tbIsJohnsonStandard");
    tbIsCousinsStandard = formWidget->findChild<QLabel *>("tbIsCousinsStandard");
    tbAAVSOLabel = formWidget->findChild<QLabel *>("tbAAVSOLabel");
    twMagnitudes = formWidget->findChild<QTableWidget *>("twMagnitudes");

    for (nRow = 0; nRow < 8; nRow++)
    {
      twMagnitudes->setRowHeight(nRow, 20);
    };

      // Alternate names

    twAlternateNames = formWidget->findChild<QTableWidget *>("twAlternateNames");

      // Variable information

    tbPeriod = formWidget->findChild<QLabel *>("tbPeriod");
    tbEpoch = formWidget->findChild<QLabel *>("tbEpoch");
    twVarMagnitudes = formWidget->findChild<QTableWidget *>("twVarMagnitudes");
    tbVarComments = formWidget->findChild<QLabel *>("tbVarComments");

    ClearFields();

    setWindowTitle(tr("View Target Information"));
    //setWindowIcon(QIcon(":/icons/iconDarknessHours.png"));
  }


  /// @brief Destructor for the class.
  /// @throws None.
  /// @details Ensures that all memory objects are properly destroyed.
  /// @version 2009-11-07/GGB - Function created.

  CObjectInfoWindow::~CObjectInfoWindow(void)
  {
  }

  /// @brief Manages the search routine and searching the database for the relevant entry.
  /// @throws None.
  /// @details Fills all the relevant data into the controls in the window.
  /// @version 2009-11-05/GGB - Function created.

  void CObjectInfoWindow::eventSearch()
  {
    //QString szSQL  = "SELECT [TBL_CONSTELLATIONS].ConstellationName, [TBL_OBJECTTYPES].ObjectType, [TBL_SPECTRALCLASS].SHORTTEXT, [TBL_STELLAROBJECTS].SPECTRUM, [TBL_STELLAROBJECTS].RedShift, [TBL_STELLAROBJECTS].RA, [TBL_STELLAROBJECTS].DEC, [tbl_Epoch].Epoch, [TBL_STELLAROBJECTS].pmRA, [TBL_STELLAROBJECTS].pmDec, [TBL_STELLAROBJECTS].Parallax, [TBL_STELLAROBJECTS].Variable, [TBL_STELLAROBJECTS].Multiple, [TBL_STELLAROBJECTS].ExoPlanets, [TBL_STELLAROBJECTS].Comments, [TBL_STELLAROBJECTS].RadialVelocity, [TBL_STELLAROBJECTS].LandoltStandard, [TBL_STELLAROBJECTS].AAVSOStandard, [TBL_STELLAROBJECTS].AAVSOLabel, [TBL_STELLAROBJECTS].PrimaryStandard, [TBL_STELLAROBJECTS].JohnsonStandard, [TBL_STELLAROBJECTS].CousinsStandard, [TBL_STELLAROBJECTS].[U-Magnitude], [TBL_STELLAROBJECTS].[B-Magnitude], [TBL_STELLAROBJECTS].[V-Magnitude], [TBL_STELLAROBJECTS].[R-Magnitude], [TBL_STELLAROBJECTS].[I-Magnitude], [TBL_STELLAROBJECTS].[J-Magnitude], [TBL_STELLAROBJECTS].[H-Magnitude], [TBL_STELLAROBJECTS].[K-Magnitude], [TBL_STELLAROBJECTS].OBJECT_ID, [TBL_STELLAROBJECTS].ObjectType  \
    //                  FROM (([TBL_OBJECTTYPES] RIGHT JOIN ([TBL_CONSTELLATIONS] RIGHT JOIN [TBL_STELLAROBJECTS] ON [TBL_CONSTELLATIONS].CONSTELLATION_ID = [TBL_STELLAROBJECTS].CONSTELLATION_ID) ON [TBL_OBJECTTYPES].OBJECTTYPE_ID = [TBL_STELLAROBJECTS].ObjectType) INNER JOIN [TBL_NAMES] ON [TBL_STELLAROBJECTS].OBJECT_ID = [TBL_NAMES].OBJECT_ID) LEFT JOIN tbl_Epoch ON [TBL_STELLAROBJECTS].Epoch_ID = tbl_Epoch.Epoch_ID \
    //                  WHERE [TBL_NAMES].Name = '";
    //QString sObjectName;
    //QSqlQuery query(database::databaseATID->database());
    //QSqlQuery query2(database::databaseATID->database());
    //QTableWidgetItem *newItem;
    //int nRow;
    //double dPosition;
    //double RA1, Dec1, pmRA1, pmDec1, RV1, Parallax1, Epoch1, Epoch2;
    //char szPosition[50];
    //libAstroClass::TJD JD;		// Initialise with current date/time
    //libAstroClass::CStellarObject *aoObject;
    //libAstroClass::CAstronomicalCoordinates *CatalogueCoordinates;

    //sObjectName = tbObjectName->text();

    //if ( sObjectName.length() == 0)
    //{		// Nothing entered by User.

    //  QMessageBox msgBox;
    //  msgBox.setText(tr("No Name entered, please enter name."));
    //  msgBox.exec();
    //}
    //else
    //{
    //  szSQL += sObjectName + "'";		// add the necessary.
    //  query.exec(szSQL);
    //  if ( query.next() )
    //  {	// Object found
    //    ClearFields();
    //    tbSelectedObject->setText(sObjectName);
    //    tbConstellation->setText(query.value(0).toString());
    //    tbObjectType->setText(query.value(1).toString());
    //    tbSpectralType->setText(query.value(2).toString());
    //    tbSpectrum->setText(query.value(4).toString());
    //    tbRedShift->setText(query.value(3).toString());

    //    if (query.value(11).toBool())
    //      tbIsVariable->setText(tr("Yes"));
    //    else
    //      tbIsVariable->setText(tr("No"));

    //    if (query.value(12).toBool())
    //      tbIsMultiple->setText(tr("Yes"));
    //    else
    //      tbIsMultiple->setText("No");

    //    if (query.value(13).toBool())
    //      tbHasExoplanets->setText("Yes");
    //    else
    //      tbHasExoplanets->setText("No");

    //    tbComments->setText(query.value(14).toString());

    //    if (query.value(19).toBool())
    //      tbIsPrimaryStandard->setText("Yes");
    //    else
    //      tbIsPrimaryStandard->setText("No");

    //    if (query.value(16).toBool())
    //      tbIsLandoltStandard->setText("Yes");
    //    else
    //      tbIsLandoltStandard->setText("No");

    //    if (query.value(17).toBool())
    //      tbIsAAVSOStandard->setText("Yes");
    //    else
    //      tbIsAAVSOStandard->setText("No");
    //    tbAAVSOLabel->setText(query.value(18).toString());

    //    if (query.value(20).toBool())
    //      tbIsJohnsonStandard->setText("Yes");
    //    else
    //      tbIsJohnsonStandard->setText("No");

    //    if (query.value(21).toBool())
    //      tbIsCousinsStandard->setText("Yes");
    //    else
    //      tbIsCousinsStandard->setText("No");

    //    for (nRow = 0; nRow < 8; nRow++)
    //    {
    //      newItem = new QTableWidgetItem(query.value(22+nRow).toString());
    //      twMagnitudes->setItem(nRow, 0, newItem);
    //    };

    //    dPosition = query.value(5).toDouble();
    //    libAstroClass::sprintfHMS(szPosition, RA1 = libAstroClass::deg2dms((double) dPosition/10000), 4);
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(0, 0, newItem);

    //    libAstroClass::sprintfDMS(szPosition, Dec1 = libAstroClass::deg2dms(query.value(6).toDouble()/10000), true, 3);
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(1, 0, newItem);

    //    if ( libAstroClass::ConvertEpoch(query.value(7).toString().toAscii().data(), &Epoch1, &Epoch2) )
    //        Epoch1 = 0;
    //    newItem = new QTableWidgetItem(query.value(7).toString());
    //    twPosition->setItem(2, 0, newItem);

    //    pmRA1 = query.value(8).toDouble();
    //    sprintf(szPosition, "%.2f", pmRA1);
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(3, 0, newItem);

    //    pmDec1 = query.value(9).toDouble();
    //    sprintf(szPosition, "%.2f", pmDec1);
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(4, 0, newItem);

    //    Parallax1 = query.value(10).toDouble();
    //    sprintf(szPosition, "%.2f", Parallax1);
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(5, 0, newItem);

    //    RV1 = query.value(11).toDouble();
    //    sprintf(szPosition, "%.1f", RV1);
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(6, 0, newItem);

    //    sprintf(szPosition, "J%.2f", JD.Epoch());
    //    newItem = new QTableWidgetItem(szPosition);
    //    twPosition->setItem(2, 1, newItem);

    //    CatalogueCoordinates = new libAstroClass::CAstronomicalCoordinates(RA1 * 15, Dec1 );

    //    switch (query.value(31).toInt())
    //    {
    //    case libAstroClass::AOT_NONE:
    //      break;
    //    case libAstroClass::AOT_STAR:
    //    case libAstroClass::AOT_WHITEDWARF:
    //    case libAstroClass::AOT_NOVA:
    //      // The above three cases may all have proper motion.

    //      pmRA1 = pmRA1 / 1000;		// Convert to arcseconds
    //      pmDec1 = pmDec1 / 1000;  // Convert to arcseconds

    //      //aoObject = new CStellarObject(CatalogueCoordinates, ((EAstronomicalObjectType) query->value(31).toInt()), Epoch1, pmRA1, pmDec1, RV1, Parallax1 / 1000);
    //      aoObject->properMotion(JD);

    //      //sprintfHMS(szPosition, aoObject->pmCoordinates.RA()/15, 4 );
    //      newItem = new QTableWidgetItem(szPosition);
    //      twPosition->setItem(0, 1, newItem);

    //      //sprintfDMS(szPosition, aoObject->pmCoordinates.DEC(), true, 3 );
    //      newItem = new QTableWidgetItem(szPosition);
    //      twPosition->setItem(1, 1, newItem);

    //      //sprintf(szPosition, "%.2f", aoObject->pmRA2 * 1000 * DR2AS);
    //      newItem = new QTableWidgetItem(szPosition);
    //      twPosition->setItem(3, 1, newItem);

    //      //sprintf(szPosition, "%.2f", aoObject->pmDEC2 *  1000 * DR2AS);
    //      newItem = new QTableWidgetItem(szPosition);
    //      twPosition->setItem(4, 1, newItem);

    //      //sprintf(szPosition, "%.2f", aoObject->Parallax2 * 1000);
    //      newItem = new QTableWidgetItem(szPosition);
    //      twPosition->setItem(5, 1, newItem);

    //      //sprintf(szPosition, "%.1f", aoObject->RadialVelocity2);
    //      newItem = new QTableWidgetItem(szPosition);
    //      twPosition->setItem(6, 1, newItem);
    //      break;
    //    case libAstroClass::AOT_EXOPLANET:
    //    case libAstroClass::AOT_GALAXY:
    //    case libAstroClass::AOT_PLANETARYNEBULAR:
    //    case libAstroClass::AOT_EMMISIONNEBULA:
    //    case libAstroClass::AOT_SOLARSYSTEMOBJECT:
    //    case libAstroClass::AOT_MINORPLANET:
    //    case libAstroClass::AOT_UNUSED4:
    //    case libAstroClass::AOT_ARTIFICIALSATELLITE:
    //    case libAstroClass::AOT_QUASAR:
    //    case libAstroClass::AOT_BLACKHOLE:
    //    case libAstroClass::AOT_NEUTRONSTAR:
    //    case libAstroClass::AOT_SUPERNOVAREMNANT:
    //    case libAstroClass::AOT_COMET:
    //    case libAstroClass::AOT_OPENCLUSTER:
    //    case libAstroClass::AOT_GLOBULARCLUSTER:
    //    case libAstroClass::AOT_DARKNEBULA:
    //    case libAstroClass::AOT_REFLECTIONNEBULA:
    //    case libAstroClass::AOT_UNUSED5:
    //    case libAstroClass::AOT_ASTERISM:
    //    default:
    //      break;
    //    };

    //      // Alternate name information

    //    szSQL = "SELECT [TBL_NAMES].Name, [TBL_CATALOGUE].[Catalogue Name] " \
    //            "FROM [TBL_CATALOGUE] INNER JOIN [TBL_NAMES] ON [TBL_CATALOGUE].CATALOGUE_ID = [TBL_NAMES].CATALOGUE_ID " \
    //            "WHERE ((([TBL_NAMES].OBJECT_ID)=";
    //    szSQL.append(query.value(30).toString());
    //    szSQL.append("))");
    //    query2.exec(szSQL);


    //    nRow = 0;
    //    while (query2.next())
    //    {
    //      twAlternateNames->insertRow(nRow);
    //      twAlternateNames->setRowHeight(nRow, 20);
    //      newItem = new QTableWidgetItem(*(new QString(query2.value(0).toString())));
    //      twAlternateNames->setItem(nRow, 0, newItem);
    //      newItem = new QTableWidgetItem(*(new QString(query2.value(1).toString())));
    //      twAlternateNames->setItem(nRow, 1, newItem);
    //      nRow++;
    //    };
    //    //if (query.value(11).toBool())
    //    //  SetVariableTab(&(query.value(30)));
    //  }
    //  else
    //  {	// Object not found.
    //    QMessageBox msgBox;
    //    msgBox.setText("Object not found in ATID database.");
    //    msgBox.exec();
    //  };
    //};
  }

  /// @brief Sets all the text controls to a empty string.
  /// @throws None.
  /// @details Deletes all the items in the TableWidgets.
  /// @version 2010-05-14/GGB - Function created.

  void CObjectInfoWindow::ClearFields()
  {
    int nRow;

    tbSelectedObject->clear();
    tbConstellation->clear();
    tbObjectType->clear();
    tbSpectralType->clear();
    tbSpectrum->clear();
    tbRedShift->clear();
    tbIsVariable->clear();
    tbIsMultiple->clear();
    tbHasExoplanets->clear();
    tbComments->clear();

      // Position Information Tab

    twPosition->clearContents();

        // Photometry Tab

    tbIsPrimaryStandard->clear();
    tbIsLandoltStandard->clear();
    tbIsAAVSOStandard->clear();
    tbIsJohnsonStandard->clear();
    tbIsCousinsStandard->clear();
    tbAAVSOLabel->clear();
    twMagnitudes->clearContents();

        // Alternate names

    twAlternateNames->clearContents();
    for (nRow = twAlternateNames->rowCount()-1; nRow >= 0; nRow--)
    {
      twAlternateNames->removeRow(nRow);
    };

        // Variable informaiton

    tbEpoch->clear();
    tbPeriod->clear();
    twVarMagnitudes->clearContents();
    tbVarComments->clear();

  }

  /// @brief Sets the variable information on the Variable Tab for the object passed into the function.
  /// @throws None.
  /// @version 2010-05-14/GGB - Function created.

  void CObjectInfoWindow::SetVariableTab(QVariant *objectID)
  {
    QString szSQL = "SELECT [t-VarStars].TypeUncertain, [t-VarStars].Period, [t-VarStars].Epoch, [t-VarStars].[U-Min], [t-VarStars].[U-Max], [t-VarStars].[B-Min], [t-VarStars].[B-Max], [t-VarStars].[V-Min], [t-VarStars].[V-Max], [t-VarStars].[R-Min], [t-VarStars].[R-Max], [t-VarStars].[I-Min], [t-VarStars].[I-Max], [t-VarTypes].LongText " \
                    "FROM [t-VarTypes] INNER JOIN ([t-VarStars] INNER JOIN TBL_VARSTAR_VARTYPE ON [t-VarStars].Key = TBL_VARSTAR_VARTYPE.VARSTAR_ID) ON [t-VarTypes].Key = TBL_VARSTAR_VARTYPE.VARTYPE_ID " \
                    "WHERE [t-VarStars].OBJECT_ID= " + objectID->toString();
    QSqlQuery *query;
    QTableWidgetItem *newItem;
    int nRow;

    query = new QSqlQuery(database::databaseATID->database());
    query->exec(szSQL);

    if (query->next())
    {
      if (query->value(1).isNull())
      {
        tbPeriod->setText("Not Specified");
      }
      else
      {
        tbPeriod->setText(query->value(1).toString());
      };

      if (query->value(2).isNull())
      {
        tbEpoch->setText("Not Specified");
      }
      else
      {
        tbEpoch->setText(query->value(2).toString());
      };

      for (nRow = 0; nRow < 5; nRow++)
      {
        if (query->value(3+(nRow*2)).isNull())
        {
          newItem = new QTableWidgetItem("Not Specified");
        }
        else
        {
          newItem = new QTableWidgetItem(query->value(3+(nRow*2)).toString());
        };
        twVarMagnitudes->setItem(nRow, 0, newItem);

        if (query->value(3+(nRow*2)+1).isNull())
        {
          newItem = new QTableWidgetItem("Not Specified");
        }
        else
        {
          newItem = new QTableWidgetItem(query->value(3+(nRow*2)+1).toString());
        };
        twVarMagnitudes->setItem(nRow, 1, newItem);
      };
    }
    else
    {	// Error, information not found.
    };
  }

}  // namespace AstroManager
