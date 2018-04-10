#ifndef DOCKWIDGETIMAGE
#define DOCKWIDGETIMAGE

#include "../../Include/windowImage/windowImage.h"
#include "dockWidget.h"
#include "../imaging/imageControl.h"
#include "../VSOP.h"

#include <ACL>

namespace AstroManager
{
  namespace dockwidgets
  {
    class CDockWidgetImage : public CDockWidget
    {
      Q_OBJECT

    private:
    protected:
      static imaging::SControlImage *currentImage;

    public:
      CDockWidgetImage(QString const &, QWidget *, QAction *, QString const &);

      static void setCurrentImage(imaging::SControlImage *);

      virtual void imageChanged() = 0;
      virtual void imageActivating() {}
      virtual void imageDeactivating() {}

    };

  } // namespace dockwidgets

} // namespace AstroManager

#endif // DOCKWIDGETIMAGE

