//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								dialogSelectImages
// SUBSYSTEM:						Dialogs classes for the application
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						AstroManager::dialogs
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018, 2020 Gavin Blakeman.
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
// OVERVIEW:            Dialog for selecting an image version to downLoad
//
// CLASSES INCLUDED:    CDialogSelectImageVersion
//
// CLASS HIERARCHY:     QObject
//                        Dialog
//                          CDialogSelectImages
//
// HISTORY:             2017-08-19 GGB - File created
//
//*********************************************************************************************************************************

#include "include/dialogs/dialogSelectImages.h"

#include "include/database/databaseARID.h"

namespace astroManager
{
  namespace dialogs
  {
    /// @brief Constructor for the class.
    /// @param[in] parent - Pointer to the parent object.
    /// @throws std::bad_alloc
    /// @version 2017-08-19/GGB - Function created.

    CDialogSelectImages::CDialogSelectImages(QObject *parent, std::vector<database::imageID_t> &imageList)
      : QCL::CDialog(":/dialogs/dialogSelectImages.ui", parent), imageList_(imageList), queryModel()
    {
      setupUI();
    }

    /// @brief Processes the select push button press.
    /// @throws None.
    /// @version 2017-08-26/GGB - Function created.

    void CDialogSelectImages::eventPushButonSelectImages(bool)
    {
      imageList_.clear();

      QModelIndexList itemIndexList = tableViewImages->selectionModel()->selectedRows();

      for (auto index = itemIndexList.begin(); index != itemIndexList.end(); ++index)
      {
        QModelIndex item = (*index).sibling((*index).row(), QTE::CSelectImageQueryModel::imageID_c);
        imageList_.emplace_back(queryModel.data(item, Qt::DisplayRole).toUInt());
      };

      dlg->accept();
    }

    void CDialogSelectImages::eventPushButtonRefreshData(bool)
    {

    }

    /// @brief Populates the filters filter combo box.
    /// @throws
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::populateFilters()
    {
      GCL::sqlWriter sqlWriter;

      sqlWriter.select({"TBL_FILTERS.FILTER_ID", "TBL_FILTERS.SHORTTEXT", "TBL_FILTERS.DESCRIPTION"})
          .distinct()
          .from({"TBL_FILTERS"})
          .join({std::make_tuple("TBL_FILTERS", "FILTER_ID", GCL::sqlWriter::JOIN_INNER, "TBL_IMAGES", "FILTER_ID")})
          .orderBy({std::make_pair("TBL_FILTERS.SHORTTEXT", GCL::sqlWriter::ASC)});

      QSqlQuery sqlQuery(database::databaseARID->database());

      if (sqlQuery.exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery.next())
        {
          comboBoxFilter->addItem(sqlQuery.value(1).toString() + " - " +sqlQuery.value(2).toString(), sqlQuery.value(0));
        };
      }
      else
      {
        QSqlError error = sqlQuery.lastError();

        ERRORMESSAGE("Error while executing query: " + sqlWriter.string());
        ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      };
    }

    /// @brief Populates the observing sites combo box.
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::populateSites()
    {
      GCL::sqlWriter sqlWriter;

      sqlWriter.select({"TBL_SITES.SITE_ID", "TBL_SITES.SHORTTEXT"})
          .distinct()
          .from({"TBL_SITES"})
          .join({std::make_tuple("TBL_SITES", "SITE_ID", GCL::sqlWriter::JOIN_INNER, "TBL_IMAGES", "SITE_ID")})
          .orderBy({std::make_pair("TBL_SITES.SHORTTEXT", GCL::sqlWriter::ASC)});

      QSqlQuery sqlQuery(database::databaseARID->database());

      if (sqlQuery.exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery.next())
        {
          comboBoxObservingSite->addItem(sqlQuery.value(1).toString(), sqlQuery.value(0));
        };
      }
      else
      {
        QSqlError error = sqlQuery.lastError();

        ERRORMESSAGE("Error while executing query: " + sqlWriter.string());
        ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      };
    }

    /// @brief Populates the targets comboBox.
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::populateTargets()
    {
      GCL::sqlWriter sqlWriter;

      sqlWriter.select({"TARGET"})
          .distinct()
          .from({"TBL_IMAGES"})
          .orderBy({std::make_pair("TARGET", GCL::sqlWriter::ASC)});

      QSqlQuery sqlQuery(database::databaseARID->database());

      if (sqlQuery.exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery.next())
        {
          comboBoxTarget->addItem(sqlQuery.value(0).toString());
        };
      }
      else
      {
        QSqlError error = sqlQuery.lastError();

        ERRORMESSAGE("Error while executing query: " + sqlWriter.string());
        ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      };
    }

    /// @brief Populates the telescope comboBox
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::populateTelescopes()
    {
      GCL::sqlWriter sqlWriter;

      sqlWriter.select({"TBL_TELESCOPES.TELESCOPE_ID", "TBL_TELESCOPES.SHORTTEXT"})
          .distinct()
          .from({"TBL_TELESCOPES"})
          .join({std::make_tuple("TBL_TELESCOPES", "TELESCOPE_ID", GCL::sqlWriter::JOIN_INNER, "TBL_IMAGES", "TELESCOPE_ID")})
          .orderBy({std::make_pair("TBL_TELESCOPES.SHORTTEXT", GCL::sqlWriter::ASC)});

      QSqlQuery sqlQuery(database::databaseARID->database());

      if (sqlQuery.exec(QString::fromStdString(sqlWriter.string())))
      {
        while (sqlQuery.next())
        {
          comboBoxTelescope->addItem(sqlQuery.value(1).toString(), sqlQuery.value(0));
        };
      }
      else
      {
        QSqlError error = sqlQuery.lastError();

        ERRORMESSAGE("Error while executing query: " + sqlWriter.string());
        ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      };
    }

    /// @brief Sets the date ranges for the two date controls.
    /// @throws None.
    /// @version 2017-08-20/GGB - Function created.

    void CDialogSelectImages::setDates()
    {
      GCL::sqlWriter sqlWriter;

      sqlWriter.select()
          .max("IMAGEDATE")
          .min("IMAGEDATE")
          .from("TBL_IMAGES");

      QSqlQuery sqlQuery(database::databaseARID->database());

      if (sqlQuery.exec(QString::fromStdString(sqlWriter.string())))
      {
        sqlQuery.first();
        if (sqlQuery.isValid())
        {
          ACL::TJD JDMax(sqlQuery.value(0).toDouble());
          ACL::TJD JDMin(sqlQuery.value(1).toDouble());

          dateEditStart->setDateRange(QDate::fromJulianDay(static_cast<std::uint64_t>(JDMin)),
                                      QDate::fromJulianDay(static_cast<std::uint64_t>(JDMax)));

          dateEditEnd->setDateRange(QDate::fromJulianDay(static_cast<std::uint64_t>(JDMin)),
                                    QDate::fromJulianDay(static_cast<std::uint64_t>(JDMax)));
        }
        else
        {
            // No values in the query.
        };
      }
      else
      {
        QSqlError error = sqlQuery.lastError();

        ERRORMESSAGE("Error while executing query: " + sqlWriter.string());
        ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
        ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
        ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
      };

    }

    /// @brief Sets up the user interface elements.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::setupUI()
    {
      tableViewImages = findChild<QTableView *>("tableViewImages");
      groupBoxTarget = findChild<QGroupBox *>("groupBoxTarget");
        comboBoxTarget = findChild<QComboBox *>("comboBoxTarget");
      groupBoxFilter = findChild<QGroupBox *>("groupBoxFilter");
        comboBoxFilter = findChild<QComboBox *>("comboBoxFilter");
      groupBoxObservingSite = findChild<QGroupBox *>("groupBoxObservingSite");
        comboBoxObservingSite = findChild<QComboBox *>("comboBoxObservingSite");
      groupBoxTelescope = findChild<QGroupBox *>("groupBoxTelescope");
        comboBoxTelescope = findChild<QComboBox *>("comboBoxTelescope");
      groupBoxDate = findChild<QGroupBox *>("groupBoxDate");
        dateEditStart = findChild<QDateEdit *>("dateEditStart");
        dateEditEnd = findChild<QDateEdit *>("dateEditEnd");
      groupBoxQuality = findChild<QGroupBox *>("groupBoxQuality");
        listWidgetQuality = findChild<QListWidget *>("listWidgetQuality");

      pushButtonSelectImages = findChild<QPushButton *>("pushButtonSelectImages");
      pushButtonCancel = findChild<QPushButton *>("pushButtonCancel");
      pushButtonRefreshData = findChild<QPushButton *>("pushButtonRefreshData");
      pushButtonApplyFilters = findChild<QPushButton *>("pushButtonApplyFilters");

      tableViewImages->setModel(&queryModel);
      tableViewImages->setSortingEnabled(true);
      tableViewImages->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
      tableViewImages->setColumnHidden(9, true);

      populateTargets();
      populateFilters();
      populateSites();
      populateTelescopes();
      setDates();

      connect(pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(eventPushButtonCancel(bool)));
      connect(pushButtonSelectImages, SIGNAL(clicked(bool)), this, SLOT(eventPushButonSelectImages(bool)));
      connect(groupBoxTarget, SIGNAL(toggled(bool)), this, SLOT(eventUpdateFilterString(bool)));
      connect(comboBoxTarget, SIGNAL(currentIndexChanged(int)), this, SLOT(eventUpdateFilterString(int)));
      connect(groupBoxFilter, SIGNAL(toggled(bool)), this, SLOT(eventUpdateFilterString(bool)));
      connect(comboBoxFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(eventUpdateFilterString(int)));
      connect(groupBoxObservingSite, SIGNAL(toggled(bool)), this, SLOT(eventUpdateFilterString(bool)));
      connect(comboBoxObservingSite, SIGNAL(currentIndexChanged(int)), this, SLOT(eventUpdateFilterString(int)));
      connect(groupBoxTelescope, SIGNAL(toggled(bool)), this, SLOT(eventUpdateFilterString(bool)));
      connect(comboBoxTelescope, SIGNAL(currentIndexChanged(int)), this, SLOT(eventUpdateFilterString(int)));
    }

    /// @brief Manages the group boxes being checked/unchecked.
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::eventUpdateFilterString(bool)
    {
      eventUpdateFilterString(1);
    }

    /// @brief Updates the filter string for the dialog.
    /// @throws None.
    /// @version 2017-08-19/GGB - Function created.

    void CDialogSelectImages::eventUpdateFilterString(int)
    {
      GCL::sqlWriter &sqlWriter = queryModel.sqlWriter();

      sqlWriter.resetWhere();

      if (groupBoxTarget->isChecked())
      {
        sqlWriter.where({GCL::sqlWriter::parameterTriple(std::string("TBL_IMAGES.TARGET"), std::string("="),
                         comboBoxTarget->currentText().toStdString())});
      };

      if (groupBoxFilter->isChecked())
      {
        sqlWriter.where({GCL::sqlWriter::parameterTriple(std::string("TBL_IMAGES.FILTER_ID"), std::string("="),
                         comboBoxFilter->itemData(comboBoxFilter->currentIndex()).toUInt())});
      };

      if (groupBoxObservingSite->isChecked())
      {
        sqlWriter.where({GCL::sqlWriter::parameterTriple(std::string("TBL_IMAGES.SITE_ID"), std::string("="),
                         comboBoxObservingSite->itemData(comboBoxObservingSite->currentIndex()).toUInt())});
      };

      if (groupBoxTelescope->isChecked())
      {
        sqlWriter.where({GCL::sqlWriter::parameterTriple(std::string("TBL_IMAGES.TELESCOPE_ID"), std::string("="),
                         comboBoxTelescope->itemData(comboBoxTelescope->currentIndex()).toUInt())});
      };


      queryModel.resetQuery();
    }

  } // namespace dialogs
} // namespace AstroManager
