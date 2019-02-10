//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Observation Manager)
// FILE:								dockWidgetImage
// SUBSYSTEM:						Ancestor class
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2011-2018 Gavin Blakeman.
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
// OVERVIEW:
//
// CLASSES INCLUDED:    CAstrometryDockWidget     - Used when referencing images
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-03-17 GGB - Removed CImageControlWidget into file DockWidgetImageControl.
//                      2013-02-01 GGB - Removed CAstrometryDockWidget into it's own file
//                      2013-01-28 GGB - Split PhotometryDockWidgets from DockWidgets.
//                      2013-01-20 GGB - Removed CMessageWidget to a seperate file.
//                      2013-01-20 GGB - Release 0000.00.000 of astroManager
//                      2012-11-18 GGB - CInstrumentDockwidget split into a new file and renamed.
//                      2011-06-04 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#include "../../include/dockWidgets/dockWidgetImage.h"

namespace astroManager
{
  namespace dockwidgets
  {
    imaging::SControlImage *CDockWidgetImage::currentImage = nullptr;

    /// @brief Constructor for the CDockWidgetImage Class.
    /// @param[in] title: The window title.
    /// @param[in] parent: The window parent.
    /// @param[in] action: The action for the window.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    CDockWidgetImage::CDockWidgetImage(QString const &title, QWidget *parent, QAction *action, QString const &settingString)
      : CDockWidget(title, parent, action, settingString)
    {
    }

    /// @brief This function needs to be called when a window containing a SControlImage record becomes the active window.
    /// @details This allows the class on the other side to know if it should continue to update the record
    /// @param[in] activating: The controlImage structure for the image that is activating.
    /// @throws None.
    /// @note This should not be called when the image is changed. (See imageChanging)
    /// @version 2013-06-22/GGB - Added else to update the isActive feature of the current image is the same as the activating image.
    /// @version 2013-03-17/GGB - Function flow updated and moved to CDockWidget.
    /// @version 2011-05-29/GGB - Function created

    void CDockWidgetImage::setCurrentImage(imaging::SControlImage *activating)
    {
      if (activating != currentImage)
      {
        if (currentImage)
        {
          currentImage->isActive = false;
        };

        currentImage = activating;

        if (currentImage)
        {
          currentImage->isActive = true;
        };
      }
      else if (currentImage)
      {
        currentImage->isActive = true;
      };
    }

  } // namespace dockwidgets
} // namespace AstroManager
