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
//                      Copyright 2017-2020 Gavin Blakeman.
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

#ifndef ASTROFILE
#define ASTROFILE

#include "../astroManager.h"

  // Standard C++ Library header files.

#include <memory>
#include <vector>

  // Miscellaneous libraries

#include <ACL>
#include "boost/filesystem.hpp"

namespace astroManager
{
  class CAstroFile : public ACL::CAstroFile
  {
  private:
    enum ELastSave
    {
      LS_NONE,
      LS_FILE,
      LS_DATABASE,
    };

    QWidget *parent_;
    boost::filesystem::path fileName_;
    bool fileNameValid_;

    imageID_t imageID_;
    imageVersion_t imageVersion_;
    bool imageIDValid_;

    bool syntheticImage_ = false;

    ELastSave lastSaveAs_ = LS_NONE;


    CAstroFile() = delete;

    void cleanLocationKeywords();
    virtual void preLoadActions();
    virtual void postLoadActions();
    void registerImage();

  protected:
    void processFile();
    void loadFromDatabase();
    virtual void load();                          // Load file

    virtual bool saveToFile();
    virtual bool saveToDatabase();

  public:
    CAstroFile(QWidget *, boost::filesystem::path const &);
    CAstroFile(QWidget *, imageID_t, imageVersion_t);
    CAstroFile(QWidget *, ACL::CAstroFile const &);
    CAstroFile(CAstroFile const &);

    virtual std::unique_ptr<ACL::CAstroFile> createCopy() const;

    virtual bool save();                                      // Save file
    virtual bool saveAs();

    void fileNameValid(bool valid) { fileNameValid_ = valid; }
    bool fileNameValid() const { return fileNameValid_; }

    imageID_t imageID() const noexcept { return imageID_; }
    imageID_t &imageID() { return imageID_; }
    void imageID(imageID_t);
    void imageIDValid(bool valid) { imageIDValid_ = valid; }

    void imageVersion(imageVersion_t imageVersion) { imageVersion_ = imageVersion; }

    boost::filesystem::path getFileName() const;

      // Image Functions

    bool syntheticImage() const { return syntheticImage_; }
    void syntheticImage(bool synthetic) { syntheticImage_ = synthetic; }

  };
} // namespace AstroManager

#endif // ASTROFILE

