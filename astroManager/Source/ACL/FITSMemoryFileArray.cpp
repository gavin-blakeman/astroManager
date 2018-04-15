//*********************************************************************************************************************************
//
// PROJECT:							astroManager
// FILE:								FITSMemoryFileArray
// SUBSYSTEM:						Class for in-memory FITS files using QByteArray
// LANGUAGE:						C++
// TARGET OS:						None.
// LIBRARY DEPENDANCE:	Qt.
// NAMESPACE:						astroManager
// AUTHOR:							Gavin Blakeman (GGB)
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

#include "../../Include/ACL/FITSMemoryFileArray.h"

namespace AstroManager
{

  /// @brief Default constructor for the class.
  /// @throws std::bad_alloc
  /// @version 2017-08-13/GGB - Function created.

  CFITSMemoryFileArray::CFITSMemoryFileArray() : ACL::CFITSMemoryFile(), byteArray_()
  {
  }

  /// @brief Constructor from a file.
  /// @param[in] filePath - The path and filename
  /// @throws std::bad_alloc
  /// @version 2017-08-13/GGB - Function created.

  CFITSMemoryFileArray::CFITSMemoryFileArray(boost::filesystem::path const &filePath) : ACL::CFITSMemoryFile(), byteArray_()
  {
    readFromFile(filePath);
  }

  /// @brief Constructor for preallocated size.
  /// @param[in] memorySize - The size of the memory to allocate.
  /// @throws std::bad_alloc
  /// @version 2017-08-13/GGB - Function created.

  CFITSMemoryFileArray::CFITSMemoryFileArray(std::size_t memorySize) : ACL::CFITSMemoryFile(), byteArray_()
  {
    memory_allocate(memorySize);
  }

  /// @brief Function to allocate memory.
  /// @param[in] newMemorySize - The number of bytes to allocate.
  /// @throws std::bad_alloc
  /// @version 2017-08-13/GGB - Function created.

  void CFITSMemoryFileArray::memory_allocate(std::size_t newMemorySize)
  {
    byteArray_.resize(newMemorySize);
    memorySize(byteArray_.size());
    memoryPointer(byteArray_.data());
  }

  /// @brief Frees the allocated memory
  /// @throws None.
  /// @version 2017-08-13/GGB - Function created.

  void CFITSMemoryFileArray::memory_free()
  {
    byteArray_.resize(0);
    memorySize(byteArray_.size());
    memoryPointer(byteArray_.data());
  }

  /// @brief Reallocates memory
  /// @param[in] newMemorySize - Number of bytes to allocate.
  /// @throws std::bad_alloc
  /// @version 2017-08-13/GGB - Function created.

  void CFITSMemoryFileArray::memory_reallocate(std::size_t newMemorySize)
  {
    byteArray_.resize(newMemorySize);
    memorySize(byteArray_.size());
    memoryPointer(byteArray_.data());
  }
}
