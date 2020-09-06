//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetHistogram
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2020 Gavin Blakeman.
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
// CLASSES INCLUDED:    CDockWidgetHistogram - Used to display the image histogram.
//
// CLASS HIERARCHY:     QDockWidget
//                        - CAstrometryDockWidget
//                        - CInstrumentDockwidget
//                        - CMessageWidget
//                        - CDockwidgetHistogram
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-02-02 GGB - Created class and file for the astroManager application
//
//*********************************************************************************************************************************

#ifndef ASTROMANAGER_DOCKWIDGETHISTOGRAM_H
#define ASTROMANAGER_DOCKWIDGETHISTOGRAM_H

  // astroManager include files

#include "dockWidgetImage.h"
#include "../FrameWindow.h"

  // Standard library

#include <memory>
#include <tuple>
#include <vector>

  // Qxt Library

#include "../Qxt/include/QxtGui/QxtSpanSlider"

  // Qwt Library

#include <qwt_plot.h>
#include <qwt_plot_histogram.h>

  // Boost library

#include "boost/tuple/tuple.hpp"
#include <QCL>

namespace astroManager
{
  namespace dockwidgets
  {
    typedef std::shared_ptr<std::vector<size_t> > PHistogramSlice;

    class CHistogram : public CDockWidgetImage
    {
      Q_OBJECT

    private:
      ACL::ETransferFunction transferFunction;
      double gammaValue;
      QDoubleSpinBox *sbBlack;
      QDoubleSpinBox *sbWhite;
      QxtSpanSlider *spanSlider;            // Used for setting the black and white points.
      QCheckBox *checkBoxInvert;
      QComboBox *comboBoxTransferFunction;
      QDoubleSpinBox *doubleSpinBoxGamma;

      bool bInternalCall;

      QwtPlotHistogram histogramData;
      QwtPlot *histogramPlot;

      void setupUI();

      void displayHistogramThread(ACL::CAstroImage *, std::tuple<ACL::INDEX_t, ACL::INDEX_t, ACL::FP_t, ACL::FP_t>, PHistogramSlice);
      void DisplayHistogram();

    protected:
      void redrawImage();

    public:
      CHistogram(QWidget *, QAction *);
      virtual ~CHistogram() {}

      void resetValues(double, double);
      void updateMinMaxValues();
      void updateValues();

      virtual void setEnabled(bool enabledValue);
      virtual void imageChanged();
      virtual void mdiWindowActivating(CMdiSubWindow *) override;

    private slots:
      void eventBlackChanged(double);
      void eventWhiteChanged(double);
      void eventBlackReset(bool);
      void eventWhiteReset(bool);
      void eventBlackSliderChanged(int);
      void eventWhiteSliderChanged(int);
      void eventInvertChanged(int);
      void eventGammaChanged(double);
      void eventTranferFunctionChanged(QString const &);
    };
  }
}  // namespace AstroManager


#endif // DOCKWIDGETHISTOGRAM_H
