//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								astroFile
// SUBSYSTEM:						Additions to the ACL::astroFile class.
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman. (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017-2018 Gavin Blakeman.
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
// OVERVIEW:						Implements the class for opening and managing an astronomical file.
//
// CLASSES INCLUDED:    CAstroFile
//
// CLASS HIERARCHY:     ACL::CAstroFile
//                        AstroManager::CAstroFile
//
// HISTORY:             2017-07-24 GGB - File created
//
//*********************************************************************************************************************************


#include "../../Include/ACL/astroFile.h"

  // astroManager Files

#include "../../Include/database/databaseARID.h"
#include "../../Include/ACL/FITSMemoryFileArray.h"
#include "../../Include/ACL/observatoryInformation.h"
#include "../../Include/ACL/telescope.h"
#include "../../Include/qtExtensions/qt.h"
#include "../../Include/Settings.h"

namespace AstroManager
{
  /// @brief Copy constructor.
  /// @param[in] toCopy - The instance to copy from.
  /// @throws std::bad_alloc
  /// @version 2017-08-26/GGB - Function created.

  CAstroFile::CAstroFile(CAstroFile const &toCopy) : ACL::CAstroFile(toCopy), parent_(toCopy.parent_),
    fileNameValid_(toCopy.fileNameValid_), fileName_(toCopy.fileName_), imageIDValid_(toCopy.imageIDValid_),
    imageID_(toCopy.imageID_), imageVersion_(toCopy.imageVersion_)
  {
  }

  /// @brief Constructor for the class. Calls the parent constructor.
  /// @details  Substitutes the observaltionLocation to a AstroManager::CObservatory rather than a ACL::CGeographicLocation.
  /// @param[in] filename - The filename to associate with this file.
  /// @throws std::bad_alloc
  /// @version 2017-07-24/GGB - Function created.

  CAstroFile::CAstroFile(QWidget *parent, boost::filesystem::path const &filename) : ACL::CAstroFile(filename.filename().string()),
    parent_(parent), fileNameValid_(true), fileName_(filename), imageIDValid_(false), imageID_(0), imageVersion_(0)
  {
      // First change the object stored by the astroFile into a AstroManager::CObservatory type.
      // and the telescope into an astroManagerCTelescope() type.

    observationLocation.reset(new CObservatory());
    observationTelescope.reset(new CTelescope());

    load();
  }

  /// @brief Constructor to construct from a database object.
  /// @param[in] imageID - The imageID to load from the database.
  /// @param[in] imageVersion - The version of the image to load.
  /// @throws std::bad_alloc
  /// @version 2017-08-12/GGB - Function created.

  CAstroFile::CAstroFile(QWidget *parent, imageID_t imageID, imageVersion_t imageVersion) : ACL::CAstroFile(), parent_(parent),
    fileNameValid_(false), fileName_(), imageIDValid_(true), imageID_(imageID), imageVersion_(imageVersion)
  {
      // First change the object stored by the astroFile into a AstroManager::CObservatory type.
      // and the telescope into an astroManagerCTelescope() type.

    observationLocation.reset(new CObservatory());
    observationTelescope.reset(new CTelescope());

    load();
  }

  /// Constructor to construct from a ACL::CAstroFile
  /// @param[in] parent - The parent (owner) of the astroFile.
  /// @param[in] astroFile - The astroFile to construct from.
  /// @throws std::bad_alloc
  /// @version 2017-08-18/GGB - Function created.

  CAstroFile::CAstroFile(QWidget *parent, ACL::CAstroFile const &astroFile) : ACL::CAstroFile(astroFile), parent_(parent),
    fileNameValid_(false), fileName_(), imageIDValid_(false), imageID_(0), imageVersion_(0)
  {
      // First change the object stored by the astroFile into a AstroManager::CObservatory type.
      // and the telescope into an astroManagerCTelescope() type.

    observationLocation.reset(new CObservatory());
    observationTelescope.reset(new CTelescope());

    load();
  }

  /// @brief Removes unnecessary location keywords.
  /// @throws None.
  /// @version 2017-07-25/GGB - Function created.

  void CAstroFile::cleanLocationKeywords()
  {
    if (keywordExists(0, ACL::MAXIM_LATITUDE))
    {
        // The MAXIM one is the one we want.

      keywordDelete(0, ACL::UCOLICK_LATITUDE);
      keywordDelete(0, ACL::SBIG_LATITUDE);
    }
    else if ( (keywordExists(0, ACL::UCOLICK_LATITUDE)) ||
              (keywordExists(0, ACL::SBIG_LATITUDE)) )
    {
        // Wrong keyword. Lets first save the preferred keyword.

      keywordWrite(0, ACL::MAXIM_LATITUDE, getObservationLocation()->latitude(), ACL::astroManager_COMMENT_LATITUDE);

        // Delete the keywords that are not required.

      keywordDelete(0, ACL::UCOLICK_LATITUDE);
      keywordDelete(0, ACL::SBIG_LATITUDE);
    };

    if (keywordExists(0, ACL::MAXIM_LONGITUDE))
    {
        // The MAXIM one is the one we want.

      keywordDelete(0, ACL::UCOLICK_LONGITUDE);
      keywordDelete(0, ACL::SBIG_LONGITUDE);
    }
    else if ( (keywordExists(0, ACL::UCOLICK_LONGITUDE)) ||
              (keywordExists(0, ACL::SBIG_LONGITUDE)) )
    {
        // Wrong keyword. Lets first save the preferred keyword.

      keywordWrite(0, ACL::MAXIM_LATITUDE, getObservationLocation()->longitude(), ACL::astroManager_COMMENT_LONGITUDE);

        // Delete the keywords that are not required.

      keywordDelete(0, ACL::UCOLICK_LONGITUDE);
      keywordDelete(0, ACL::SBIG_LONGITUDE);
    };

    if (keywordExists(0, ACL::MAXIM_ALTITUDE))
    {
        // The MAXIM one is the one we want.

      keywordDelete(0, ACL::astroManager_ALTITUDE);
    }
    else if (keywordExists(0, ACL::astroManager_ALTITUDE))
    {
        // Wrong keyword. Lets first save the preferred keyword.

      keywordWrite(0, ACL::MAXIM_ALTITUDE, getObservationLocation()->altitude(), ACL::astroManager_COMMENT_ALTITUDE);

        // Delete the keywords that are not required.

      keywordDelete(0, ACL::astroManager_ALTITUDE);
    };
  }

  /// @brief Creates a copy of this astroFile.
  /// @returns A shared pointer to the newly created copy.
  /// @throws None.
  /// @details Simply creates a new copy by calling the copy constructor.
  /// @version 2017-08-26/GGB - Function created.

  ACL::CAstroFile *CAstroFile::createCopy() const
  {
    return (new CAstroFile(*this));
  }

  /// @brief Returns the file name and path.
  /// @returns The file name and path.
  /// @throws GCL::CCodeError(astroManager)
  /// @version 2017-08-12/GGB - Function created.

  boost::filesystem::path CAstroFile::getFileName() const
  {
    if (fileNameValid_)
    {
      return fileName_;
    }
    else
    {
      ASTROMANAGER_CODE_ERROR;
    }
  }

  /// @brief Sets the imageID. This also changes the save Method to database.
  /// @param[in] imageID - The new ID to assign to the image.
  /// @throws None.
  /// @version 2017-09-01/GGB - Function created.

  void CAstroFile::imageID(imageID_t imageID)
  {
    imageID_ = imageID;

    imageIDValid_ = true;
  }

  /// @brief Overloaded load() function to load the file contents.
  /// @details Calls preLoadActions() and postLoadAction() to allow additional actions to take place automatically.
  /// @throws GCL::CCodeError(astroManager)
  /// @version 2017-07-26/GGB - Function created.

  void CAstroFile::load()
  {
    preLoadActions();
    if (fileNameValid_)
    {
      ACL::CAstroFile::loadFromFile(fileName_);
    }
    else if (imageIDValid_)
    {
      loadFromDatabase();
    }
    else
    {
      ACL_CODE_ERROR;
    }
    postLoadActions();
  }

  /// @brief Loads an image from the database.
  /// @throws GCL::CCodeError(astroManager)
  /// @version 2017-08-12/GGB - Function created.

  void CAstroFile::loadFromDatabase()
  {
    QByteArray byteArray;


    if (database::databaseARID->downLoadImage(imageID_, imageVersion_, byteArray))
    {
        // The following only works if the memory file is opened READONLY.

      fitsfile *file;
      int status = 0;
      void *ptr = byteArray.data();
      std::size_t size = byteArray.size();

      CFITSIO_TEST(fits_open_memfile(&file, "", READONLY, &ptr, &size, ACL::FITS_BLOCK, nullptr, &status));
      loadFromFITS(file);
      CFITSIO_TEST(fits_close_file(file, &status));
    }
    else
    {
      ASTROMANAGER_CODE_ERROR;
    };
  }

  /// @brief Activities to perform after the file has been opened.
  /// @throws None.
  /// @version 2017-08-12/GGB - Function created.

  void CAstroFile::postLoadActions()
  {
     // If the original image needs to be saved, call the function to perform the saving.

    if (fileNameValid_ && settings::astroManagerSettings->value(settings::IMAGING_DATABASE_SAVEORIGINAL, QVariant(true)).toBool())
    {
      database::databaseARID->saveOriginalImage(this);
    };
  }

  /// @brief Actions that will be done before the image is loaded.
  /// @throws
  /// @version 2017-07-26/GGB - Function created.

  void CAstroFile::preLoadActions()
  {
  }


  /// @brief Processes a newly opened AstroFile.
  /// @details  1. The file is checked to see if it has been registered and if not, it will be registered.
  ///           2. The location details will be searched to check if there is an observing site associated
  ///           3. Checks will be made to see if there is any weather data.

  void CAstroFile::processFile()
  {
    bool imageRegistered = false;
    bool fileUpdated = false;
    QUuid _uuid;

      // Check if the keyword exists for the UUID

    if (keywordExists(0, ACL::astroManager_UUID) )
    {
        // If so, load the UUID value

      std::string suuid = static_cast<std::string>(keywordData(0, ACL::astroManager_UUID));

      _uuid = QUuid(QString::fromStdString(suuid));
      imageRegistered = true;
      fileUpdated = true;
    }
    else
    {
      QString uuid;

        // The image does not have a UUID. Check if the filename is known to the database.

      imageRegistered = database::databaseARID->isImageNameRegistered(fileName_.filename().string(), uuid);
      _uuid = QUuid(uuid);

    };

      // At this point, if the image is registered, then the _uuid value is defined and the imageRegistered value is true.

    if (!imageRegistered)
    {
      _uuid = QUuid::createUuid();    /* Should create a unique UUID. */
      ///@todo Check that the UUID created does not exist in the database and if it does, create another.
    };

    if (!fileUpdated)
    {
        /* Need to some simplifications on the AstroFile. This includes:
         *    Check if the site already exists in the database
         *    Check if the geographical location is coded multiple times (common)
         *    Remove unneeded fields.
         */

      if (settings::astroManagerSettings->value(settings::IMAGING_KEYWORDS_CLEAN, QVariant(false)).toBool())
      {
        cleanLocationKeywords();
      };

        // Try to associate the latiitude/longitude with an observing site.

      keywordWrite(0, ACL::astroManager_UUID, _uuid.toString().toUpper().toStdString(), ACL::astroManager_COMMENT_UUID);
    };

      // Search for the observatory that is closest.

    if (database::databaseARID->findObservingSite(dynamic_cast<CObservatory *>(&*getObservationLocation())))
    {

    }


    if (!imageRegistered)
    {
      database::databaseARID->registerImage(this);
    };
  }

  /// @brief Procedure that registers an image in the database.
  /// @throws
  /// @version 2017-07-26/GGB - Function created.

  void CAstroFile::registerImage()
  {

  }

  /// @brief Saves the image. The lastSaveAs_ variable is used to determine how to save the file.
  /// @throws GCL::CCodeError(astroManager)
  /// @version 2017-08-13/GGB - Function created.

  bool CAstroFile::save()
  {
    bool returnValue = false;

    switch (lastSaveAs_)
    {
      case LS_NONE:
      {
        returnValue = saveAs();
        break;
      };
      case LS_FILE:
      {
        returnValue = saveToFile();
        break;
      };
      case LS_DATABASE:
      {
        returnValue = saveToDatabase();
        break;
      };
      default:
      {
        ASTROMANAGER_CODE_ERROR;
        break;
      };
    };

    return returnValue;
  }

  /// @brief Performs the saveAs function.
  /// @details If the ARID database is enabled, then the
  /// @throws GCL::CCodeError(astroManager)
  /// @version 2017-09-01/GGB - Function created.

  bool CAstroFile::saveAs()
  {
    bool returnValue = false;

    if(!settings::astroManagerSettings->value(settings::ARID_DATABASE_DISABLE).toBool())
    {
        // Give the user the option to save to database or file.

      QMessageBox messageBox;

      messageBox.setWindowTitle(QObject::tr("Save File As..."));
      messageBox.setText(QObject::tr("Save the image to file or database."));
      QPushButton *fileButton = messageBox.addButton(QObject::tr("File"), QMessageBox::AcceptRole);
      QPushButton *databaseButton = messageBox.addButton(QObject::tr("Database"), QMessageBox::AcceptRole);
      messageBox.exec();

      if (messageBox.clickedButton() == fileButton)
      {
        fileNameValid_ = false;           // Causes a saveAs()
        returnValue = saveToFile();
        lastSaveAs_ = LS_FILE;
      }
      else if (messageBox.clickedButton() == databaseButton)
      {
        returnValue = saveToDatabase();
        lastSaveAs_ = LS_DATABASE;
      }
      else
      {
        ASTROMANAGER_CODE_ERROR;
      };
    }
    else
    {
      returnValue = saveToFile();
    };

    return returnValue;
  }

  /// @brief Saves the astroFile to file.
  /// @returns true = Sucess
  /// @returns false = failure.
  /// @throws None.
  /// @version 2017-09-01/GGB - Function created.

  bool CAstroFile::saveToFile()
  {
    bool returnValue = false;
    try
    {
      if (!fileNameValid_)
      {
          // Request a filename from the user.

        QString fileName = QFileDialog::getSaveFileName(parent_, QObject::tr("Save file as..."),
                                                        settings::astroManagerSettings->value(settings::IMAGING_DIRECTORY,
                                                                                      QVariant("")).toString(),
                                                        QObject::tr("FITS Files (*.fts)"));
        if (!fileName.isNull())
        {
          fileName_ = fileName.toStdString();
          fileName_.replace_extension(".fts");

        };
      };
      ACL::CAstroFile::save(fileName_);
      returnValue = true;
    }
    catch (GCL::CError &error)
    {
      if ( error.isThisError("ACL", 0x1001) )
      {
        QMessageBox messageBox;

        messageBox.setText(QObject::tr("Unable to save file."));
        messageBox.setInformativeText(QObject::tr("Error while saving the file."));
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();
      }
      else
      {
        QMessageBox messageBox;

        messageBox.setText(QObject::tr("Unable to save file."));
        messageBox.setInformativeText(QObject::tr("Unknown Error while saving the file."));
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();
      }
    }
    catch (ACL::CFITSException &exception)      // Catch a FITS exception and report it.
    {
      QMessageBox messageBox;

      exception.logErrorMessage();
      std::cout.flush();
      messageBox.setText(QObject::tr("cfitsio Error: Unable to save file."));
      messageBox.setInformativeText(QString::fromStdString(exception.errorMessage()));
      messageBox.setIcon(QMessageBox::Critical);
      messageBox.setStandardButtons(QMessageBox::Ok);
      messageBox.setDefaultButton(QMessageBox::Ok);
      messageBox.exec();
    }
    catch(...)
    {
      QMessageBox messageBox;

      messageBox.setText(QObject::tr("Default Error Handler: Unable to save file."));
      messageBox.setInformativeText(QObject::tr("Unknown Error while saving the file."));
      messageBox.setIcon(QMessageBox::Critical);
      messageBox.setStandardButtons(QMessageBox::Ok);
      messageBox.setDefaultButton(QMessageBox::Ok);
      messageBox.exec();
    };

    return returnValue;
  }

  /// @brief Saves the image to database. The image is automatically saved as the next version.
  /// @details Checks then need to be made to find the maximum allowable versions (zero is never considered a version) and deleting
  ///          any extraneous versions.
  /// @throws None.
  /// @version 2017-08-13/GGB - Function created.

  bool CAstroFile::saveToDatabase()
  {
    bool returnValue = false;

    CFITSMemoryFileArray memoryArray;

    if (!imageIDValid_)
    {
        // Need to register the image.
        // Check if the UUID exists.

      if (keywordExists(0, ACL::astroManager_UUID))
      {
        std::string szUUID = static_cast<std::string>(keywordData(0, ACL::astroManager_UUID));
        QUuid UUID(QString::fromStdString(szUUID));

        if (UUID.isNull())
        {
            // Create a unique UUID for the file.

          do
          {
            UUID = QUuid::createUuid();
          }
          while (database::databaseARID->isImageUUIDRegistered(UUID, imageID_));

          keywordWrite(0, ACL::astroManager_UUID, UUID.toString().toStdString(), ACL::astroManager_COMMENT_UUID);
        }
        else if (database::databaseARID->isImageUUIDRegistered(UUID, imageID_))
        {
          database::databaseARID->versionLatest(imageID_, imageVersion_);
          imageIDValid_ = true;
        };
      };

        // We have a valid UUID. We may also have a valid imageID

      if (!imageIDValid_)
      {
          // Register the image.

        imageVersion_ = 0;
        database::databaseARID->registerImage(this);
        imageIDValid_ = true;
      };
    };

      // Ask the user for a comment to include when saving. Note a null string is acceptable.

    QString comments = QInputDialog::getMultiLineText(parent_, QObject::tr("Version Comments"),
                                                      QObject::tr("Comments to include with version"));
    if (comments.isNull())
    {
      comments = "";
    };

    try
    {
      ACL::CAstroFile::save(memoryArray);
      database::databaseARID->uploadImage(memoryArray.byteArray(), imageID_, ++imageVersion_, comments);
      returnValue = true;
    }
    catch (GCL::CError &error)
    {
      if ( error.isThisError("ACL", 0x1001) )
      {
        QMessageBox messageBox;

        messageBox.setText(QObject::tr("Unable to save file."));
        messageBox.setInformativeText(QObject::tr("Error while saving the file."));
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();
      }
      else
      {
        QMessageBox messageBox;

        messageBox.setText(QObject::tr("Unable to save file."));
        messageBox.setInformativeText(QObject::tr("Unknown Error while saving the file."));
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();
      }
    }
    catch (ACL::CFITSException &exception)      // Catch a FITS exception and report it.
    {
      QMessageBox messageBox;

      exception.logErrorMessage();
      std::cout.flush();
      messageBox.setText(QObject::tr("cfitsio Error: Unable to save file."));
      messageBox.setInformativeText(QString::fromStdString(exception.errorMessage()));
      messageBox.setIcon(QMessageBox::Critical);
      messageBox.setStandardButtons(QMessageBox::Ok);
      messageBox.setDefaultButton(QMessageBox::Ok);
      messageBox.exec();
    }
    catch(...)
    {
      QMessageBox messageBox;

      messageBox.setText(QObject::tr("Default Error Handler: Unable to save file."));
      messageBox.setInformativeText(QObject::tr("Unknown Error while saving the file."));
      messageBox.setIcon(QMessageBox::Critical);
      messageBox.setStandardButtons(QMessageBox::Ok);
      messageBox.setDefaultButton(QMessageBox::Ok);
      messageBox.exec();
    };

    return returnValue;
  }

} // namespace AstroManager
