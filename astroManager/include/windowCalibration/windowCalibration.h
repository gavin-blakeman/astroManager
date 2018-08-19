#ifndef WINDOWCALIBRATION_H
#define WINDOWCALIBRATION_H

#include "../qtExtensions/MdiSubWindow.h"

namespace astroManager
{
  namespace calibration
  {

    class CWindowCalibration : public CMdiSubWindow
    {
    private:
    protected:
    public:
      CWindowCalibration(QWidget *parent) : CMdiSubWindow(parent) {}
      virtual EWindowClass getWindowClass() const { return WC_CALIBRATE; }
    };

  } // namespace calibration
} // namespace AstroManager

#endif // WINDOWCALIBRATION_H

