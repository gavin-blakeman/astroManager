//*********************************************************************************************************************************
//
// PROJECT:							AIRDAS
// FILE:								FITSMemoryFileArray
// SUBSYSTEM:						Class for in-memory FITS files using QByteArray
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						AIRDAS
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2017 Gavin Blakeman.
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
// OVERVIEW:						A class for in-memory management of FITS files.
//
// CLASSES INCLUDED:		CFITSMemoryFileArray
//
// CLASS HIERARCHY:     ACL::CFITSMemoryFile
//                        AstroManager::CFITSMemoryFileArray
//
// HISTORY:             2017-08-13 GGB - Started development of classes.
//
//*********************************************************************************************************************************

#ifndef FITSMEMORYFILEARRAY
#define FITSMEMORYFILEARRAY

  // AIRDAS includes

#include "../qtExtensions/qt.h"

  // Miscellaneous libraries

#include <ACL>

namespace AstroManager
{
  class CFITSMemoryFileArray : public ACL::CFITSMemoryFile
  {
  private:
    QByteArray byteArray_;

  protected:
    virtual void memory_allocate(std::size_t);
    virtual void memory_reallocate(std::size_t);
    virtual void memory_free();

  public:
    CFITSMemoryFileArray();
    CFITSMemoryFileArray(boost::filesystem::path const &);
    CFITSMemoryFileArray(std::size_t);

    QByteArray const &byteArray() const { return byteArray_; }
  };

} // namespace AstroManager

#endif // FITSMEMORYFILEARRAY

