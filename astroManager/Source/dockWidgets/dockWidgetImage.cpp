#include "../../Include/dockWidgets/dockWidgetImage.h"

namespace AstroManager
{
  namespace dockwidgets
  {
    imaging::SControlImage *CDockWidgetImage::currentImage = nullptr;

    /// @brief Constructor for the CDockWidgetImage Class.
    /// @param[in] title - The window title.
    /// @param[in] parent - The window parent.
    /// @param[in] action - The action for the window.
    /// @throws None.
    /// @version 2017-07-01/GGB - Function created.

    CDockWidgetImage::CDockWidgetImage(QString const &title, QWidget *parent, QAction *action, QString const &settingString)
      : CDockWidget(title, parent, action, settingString)
    {
    }

    /// @brief This function needs to be called when a window containing a SControlImage record becomes the active window.
    /// @details This allows the class on the other side to know if it should continue to update the record
    /// @param[in] activating - The controlImage structure for the image that is activating.
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
