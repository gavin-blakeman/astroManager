//*********************************************************************************************************************************
//
// PROJECT:							AstroManager (Astronomy Observation Manager)
// FILE:								DockWidgetHistogram
// SUBSYSTEM:						All the dockable widget classes used by the applicaiton
// LANGUAGE:						C++
// TARGET OS:						WINDOWS/UNIX/LINUX/MAC
// LIBRARY DEPENDANCE:	Qt, ACL, QWT, Boost
// NAMESPACE:						AstroManager::dockwidgets
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2013-2018 Gavin Blakeman.
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
// OVERVIEW:            The Histogram widget controls the way the image is displayed. It always applies to the currently selected
//                      image. If Histogram widget allows the black point and white point of an image to be changed. It also allows
//                      the transfer function being used for display to be changed.
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
//                      2013-02-02 GGB - Created class and file for the astroManager application
//
//*********************************************************************************************************************************

#include "../../Include/dockWidgets/dockWidgetHistogram.h"

  // astroManager includes

#include "../../Include/windowImage/windowImage.h"
#include "../../Include/dockWidgets/dockWidgetMagnify.h"
#include "../../Include/dockWidgets/dockWidgetNavigator.h"
#include "../../Include/Settings.h"

  // QWT Library

#include <qwt_abstract_scale_draw.h>
#include <qwt_scale_draw.h>
#include <qwt_series_data.h>
#include "qwt_scale_engine.h"

  // Miscellaneous Library

#include <ACL>
#include <GCL>

  // Qt Include files

#include <QtUiTools/QtUiTools>

namespace AstroManager
{
  namespace dockwidgets
  {
    int const HISTOGRAMBINS(128);

    QString const QS_LINEAR     ("LINEAR");
    QString const QS_GAMMA      ("GAMMA");
    QString const QS_SQRT       ("SQRT");
    QString const QS_CBRT       ("CBRT");
    QString const QS_SIGMOID    ("SIGMOID");
    QString const QS_SQUARE     ("SQUARE");
    QString const QS_GAMMALOG   ("GAMMALOG");
    QString const QS_LOG        ("LOG");

    int const BW_SLIDER_LOWER   = 0;          // 0%
    int const BW_SLIDER_UPPER   = 1000;       // 100.0%

    //*****************************************************************************************************************************
    //
    // CHistogram
    //
    //*****************************************************************************************************************************

    /// @brief Constructor for the class. Calls the function to setup the user interface.
    /// @param[in] parent - The parent widget. (Frame Window)
    /// @param[in] action - The menu action that is associated with this dock widget.
    /// @throws None.
    /// @version 2017-07-01/GGB - Changed order of parameters.
    /// @version 2016-03-26/GGB - Changed default transfer function to ETF_LINEAR (was ETF_NONE)
    /// @version 2013-07-27/GGB - Added objectName for restoreState() support.
    /// @version 2013-06-21/GGB - Added settings support.
    /// @version 2013-05-26/GGB - Added support for transfer functions.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-02-02/GGB - Function created.

    CHistogram::CHistogram(QWidget *parent, QAction *action)
      : CDockWidgetImage(tr("Histogram"), parent, action, settings::DW_IMAGE_HISTOGRAM_VISIBLE), histogramData(),
        histogramPlot(nullptr), transferFunction(ACL::ETF_LINEAR), gammaValue(1)
    {
      gammaValue = settings::astroManagerSettings->value(settings::DW_HOTOGRAM_GAMMA, QVariant(1)).toDouble();
      QString tempTransferFunction = settings::astroManagerSettings->value(settings::DW_HISTOGRAM_TRANSFERFUNCTION,
                                                                   QVariant(QS_LINEAR)).toString();

      if ( tempTransferFunction == QS_LINEAR )
      {
        transferFunction = ACL::ETF_LINEAR;
      }
      else if ( tempTransferFunction == QS_GAMMA )
      {
        transferFunction = ACL::ETF_GAMMA;
      }
      else if ( tempTransferFunction == QS_SQRT )
      {
        transferFunction = ACL::ETF_SQRT;
      }
      else if ( tempTransferFunction == QS_CBRT )
      {
        transferFunction = ACL::ETF_CBRT;
      }
      else if ( tempTransferFunction == QS_SIGMOID )
      {
        transferFunction = ACL::ETF_SIGMOID;
      }
      else if ( tempTransferFunction == QS_SQUARE )
      {
        transferFunction = ACL::ETF_SQUARE;
      }
      else if ( tempTransferFunction == QS_GAMMALOG )
      {
        transferFunction = ACL::ETF_GAMMALOG;
      }
      else if ( tempTransferFunction == QS_LOG )
      {
        transferFunction = ACL::ETF_LOG;
      }
      else
      {
        transferFunction = ACL::ETF_LINEAR;
      }

      setupUI();

      setObjectName(DW_HISTOGRAM_NAME);
    }

    /// @brief Deactivates the passed Control Image.
    /// @param[in] enabled - The enabled value
    /// @throws None.
    /// @version 2013-06-22/GGB - Changed to function setEnabled()
    /// @version 2013-05-29/GGB - Disabling of elements enabled.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2011-05-29/GGB - Function created.

    void CHistogram::setEnabled(bool enabledValue)
    {
      CDockWidget::setEnabled(enabledValue);

      spanSlider->setEnabled(enabledValue);
      doubleSpinBoxGamma->setEnabled(enabledValue);
      comboBoxTransferFunction->setEnabled(enabledValue);
      checkBoxInvert->setEnabled(enabledValue);
      sbBlack->setEnabled(enabledValue);
      sbWhite->setEnabled(enabledValue);
      histogramPlot->setEnabled(enabledValue);
    }

    /// @brief Thread function for doing the histogram calculation.
    /// @throws CCodeError(astroManager)
    /// @version 2015-09-05/GGB - Changed second parameter to std::tuple, from boost::tuple.
    /// @version 2015-07-31/GGB - Updated code for new astro image interface.
    /// @version 2013-04-13/GGB - Function created.

    void CHistogram::displayHistogramThread(ACL::CAstroImage *astroImage, std::tuple<ACL::INDEX_t, ACL::INDEX_t,
                                            ACL::FP_t, ACL::FP_t> values,
                                            PHistogramSlice histogramSlice)
    {
      ACL::INDEX_t lIndex;
      size_t nBinNo;

        // Count the samples and assign each sample to a bin.

      for(lIndex = std::get<0>(values); lIndex < std::get<1>(values); lIndex++)
      {
        nBinNo = static_cast<size_t>(std::floor((astroImage->getValue(lIndex) - std::get<2>(values)) / std::get<3>(values)));
        if ( (nBinNo < 0) || (nBinNo >= HISTOGRAMBINS) )
        {
          CODE_ERROR(astroManager);
        }
        else
        {
          (*histogramSlice)[nBinNo]++;        // Add the weighted pixel count to the bin.
        };
      };
    }

    /// @brief Performs the calculations and display of the histogram
    /// @throws std::bad_alloc
    /// @version 2015-07-03/GGB - Changed double to FP_t and long to size_t. Resolved bug 1471220
    /// @version 2013-07-13/GGB - Removed SControlImage::(min, max, mean)
    /// @version 2013-04-13/GGB - Converted to a multi-threaded calculation mechanism.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-02-02/GGB - Changed method of calculating histogram.
    /// @version 2011-06-07/GGB - Function created.

    void CHistogram::DisplayHistogram()
    {
      std::vector<PHistogramSlice> histogramSlice;
      FP_t fMin = currentImage->astroFile->imageMin(currentImage->currentHDB);
      FP_t fMax = currentImage->astroFile->imageMax(currentImage->currentHDB);
      FP_t fBinSize = (fMax - fMin) / (HISTOGRAMBINS - 1);
      FP_t lastPos, currentPos;
      size_t lIndex, lPixels;
      FP_t histMax = 0;
      std::vector<ACL::INDEX_t> histogram(HISTOGRAMBINS, 0);
      QwtIntervalSample intervalSample;
      QVector<QwtIntervalSample> intervalData;
      ACL::CAstroImage *astroImage;
      size_t numberOfThreads;
      size_t threadNumber;
      size_t indexStep;
      ACL::INDEX_t indexBegin = 0, indexEnd = 0;
      boost::thread_group threadGroup;
      boost::thread *thread;
      ACL::INDEX_t maxThreads = static_cast<size_t>(settings::astroManagerSettings->value(settings::MAX_THREADS, QVariant(2)).toLongLong());

      if (currentImage)
      {
        if (currentImage->astroFile)
        {
          astroImage = currentImage->astroFile->getAstroImage(currentImage->currentHDB);

          lPixels = astroImage->width() * astroImage->height();

            // Ensure that we are using a reasonable number of threads. Maximise the number of threads to the number of rows

          numberOfThreads = std::min(static_cast<ACL::INDEX_t>(astroImage->height()), std::max(ACL::INDEX_t(1), maxThreads));

            // Create the histogram slices

          for (lIndex = 0; lIndex < numberOfThreads; lIndex++)
          {
            PHistogramSlice hs(new std::vector<size_t>(HISTOGRAMBINS, 0));
            histogramSlice.push_back(hs);
          };

          indexStep = lPixels / numberOfThreads;

            // Spawn the threads.

          for (threadNumber = 0; threadNumber < numberOfThreads; threadNumber++)
          {
            indexBegin = indexEnd;
            if (threadNumber == (numberOfThreads -1) )
            {
              indexEnd = lPixels;
            }
            else
            {
              indexEnd += indexStep;
            };

            thread = new boost::thread(&CHistogram::displayHistogramThread, this, astroImage,
                                       std::make_tuple(indexBegin, indexEnd, fMin, fBinSize),
                                       histogramSlice[threadNumber]);
            threadGroup.add_thread(thread);
            thread = nullptr;
          };

          threadGroup.join_all();     // Wait for all the threads to finish.

            // Add the histogram slices together.

          for (lIndex = 0; lIndex < HISTOGRAMBINS; lIndex++)
          {
            ACL::INDEX_t count = (*histogramSlice[0])[lIndex];
            for (size_t slice = 1; slice < numberOfThreads; slice++)
            {
              count += (*histogramSlice[slice])[lIndex];
            };
            histogram[lIndex] = count;
          };

          currentPos = fMin;
          for(lIndex = 0; lIndex < HISTOGRAMBINS; lIndex++)
          {
            lastPos = currentPos;
            currentPos += fBinSize;

              // Remove invalid data

            if (histogram[lIndex] == 0)
            {
              intervalSample = QwtIntervalSample(LOG_MIN, lastPos, currentPos);
            }
            else
            {
              intervalSample = QwtIntervalSample(histogram[lIndex], lastPos, currentPos);
            };

            intervalData.push_back(intervalSample);
            if (histogram[lIndex] > histMax)
            {
              histMax = histogram[lIndex];
            };
          };

          histogramData.setSamples(intervalData);
          histogramData.setBaseline(LOG_MIN);          

          histogramPlot->setAxisScale(QwtPlot::yLeft, 1, histMax);
          histogramPlot->setAxisScale(QwtPlot::xBottom, fMin, fMax);

            // Disable drawing the labels on the scales.

          histogramPlot->axisScaleDraw(QwtPlot::yLeft)->enableComponent(QwtAbstractScaleDraw::Labels, true);
          histogramPlot->axisScaleDraw(QwtPlot::xBottom)->enableComponent(QwtAbstractScaleDraw::Labels, true);

          histogramPlot->replot();
        };
      };
    }

    /// @brief Manages the black point changes.
    /// @details The black point cannot change below the minimum value. The minimum for the white point must also be updated to be
    /// the black point value.
    /// @param[in] newVal - The new black point value.
    /// @throws CODE_ERROR
    /// @version 2016-03-26/GGB - Changed an implicit cast to a static_cast in setLowerValue()
    /// @version 2013-05-29/GGB - Added the spanSlider.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-01-26/GGB - Added call to function redrawImage.
    /// @version 2010-10-16/GGB - Function created.

    void CHistogram::eventBlackChanged(double newVal)
    {
      ACL::CAstroImage *astroImage = nullptr;

      if (!bInternalCall)
      {
        if (currentImage)
        {
          astroImage = currentImage->astroFile->getAstroImage(currentImage->currentHDB);

          if (astroImage)
          {

            currentImage->blackPoint = newVal;

              // Set the white point minimum value.

            if (sbWhite->value() < newVal)
            {
              sbWhite->setValue(newVal);
            }

            redrawImage();
          }
          else
          {
            CODE_ERROR(astroManager);
          }
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      };

        // Transform the value to correct range and set the slider up.

      spanSlider->setLowerValue(static_cast<int>(std::floor(( (newVal - currentImage->astroFile->imageMin(currentImage->currentHDB)) /
                                                            (currentImage->astroFile->imageMax(currentImage->currentHDB)
                                                             - currentImage->astroFile->imageMin(currentImage->currentHDB)) *
                                                            (BW_SLIDER_UPPER - BW_SLIDER_LOWER)) + 0.5)));
    }

    /// @brief Processes the button press to reset the black point.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2017-08-28/GGB - Function created.

    void CHistogram::eventBlackReset(bool)
    {
      if (currentImage)
      {
        FP_t blackPoint = ACL::blackPoint(currentImage->astroFile->imageMean(currentImage->currentHDB),
                                          currentImage->astroFile->imageStdev(currentImage->currentHDB));
        eventBlackChanged(blackPoint);
      }
      else
      {
        astroManager_CODE_ERROR;
      }
    }

    /// @brief Routine to handle the slider movement.
    /// @details The spinbox must also be updated.  As the black value is changed the spin box must also be updated. Also the
    ///          limiting values must be changed.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-05-29/GGB - Added spanSlider.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2010-10-30/GGB - Function created.

    void CHistogram::eventBlackSliderChanged(int newVal)
    {
      ACL::CAstroImage *astroImage = nullptr;

      if (currentImage)
      {
        astroImage = currentImage->astroFile->getAstroImage(currentImage->currentHDB);

        if (astroImage)
        {
          if (!bInternalCall)
          {
             // Set the value in the spin box.

            sbBlack->setValue(newVal * ((astroImage->getMax() - astroImage->getMin()) / (BW_SLIDER_UPPER - BW_SLIDER_LOWER)) + astroImage->getMin());
          };
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      }
      else
      {
        CODE_ERROR(astroManager);
      }
    }

    /// @brief Function to manage a change in the gamma value.
    /// @throws None.
    /// @version 2013-05-26/GGB - Function created.

    void CHistogram::eventGammaChanged(double newGamma)
    {
      gammaValue = newGamma;

      redrawImage();
    }

    /// @brief Event handler called when the invert button is pressed.
    /// @throws None.
    /// @version 2013-01-26/GGB - Function created.

    void CHistogram::eventInvertChanged(int)
    {
      redrawImage();
    }

    /// @brief Sets the transferFunction value to the correct value when a new transfer function is selected.
    /// @param[in] tempTransferFunction - String representing the name of the transfer function.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-05-26/GGB - Function created.

    void CHistogram::eventTranferFunctionChanged(QString const &tempTransferFunction)
    {
      if ( tempTransferFunction == QS_LINEAR )
      {
        transferFunction = ACL::ETF_LINEAR;
        doubleSpinBoxGamma->setEnabled(false);
      }
      else if ( tempTransferFunction == QS_GAMMA )
      {
        transferFunction = ACL::ETF_GAMMA;
        doubleSpinBoxGamma->setEnabled(true);
      }
      else if ( tempTransferFunction == QS_SQRT )
      {
        transferFunction = ACL::ETF_SQRT;
        doubleSpinBoxGamma->setEnabled(false);
      }
      else if ( tempTransferFunction == QS_CBRT )
      {
        transferFunction = ACL::ETF_CBRT;
        doubleSpinBoxGamma->setEnabled(false);
      }
      else if ( tempTransferFunction == QS_SIGMOID )
      {
        transferFunction = ACL::ETF_SIGMOID;
        doubleSpinBoxGamma->setEnabled(false);
      }
      else if ( tempTransferFunction == QS_SQUARE )
      {
        transferFunction = ACL::ETF_SQUARE;
        doubleSpinBoxGamma->setEnabled(false);
      }
      else if ( tempTransferFunction == QS_GAMMALOG )
      {
        transferFunction = ACL::ETF_GAMMALOG;
        doubleSpinBoxGamma->setEnabled(true);
      }
      else if ( tempTransferFunction == QS_LOG )
      {
        transferFunction = ACL::ETF_LOG;
        doubleSpinBoxGamma->setEnabled(false);
      }
      else
      {
        CODE_ERROR(astroManager);
      };

      redrawImage();
    }

    /// @brief Manages the white point changes.
    /// @note The white point cannot be above the minimum value.
    /// @note The maximum for the black point must also be updates to the white point value.
    /// @version 2016-03-26/GGB - Changed implicit cast to static_cast in setUpperValue()
    /// @version 2013-05-29/GGB - Added spanSlider
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-01-26/GGB - Added call to function redrawImage.
    /// @version 2010-10-17/GGB - Function created.

    void CHistogram::eventWhiteChanged(double newVal)
    {
      ACL::CAstroImage *astroImage = nullptr;

      if (!bInternalCall)
      {
        if (currentImage)
        {

          astroImage = currentImage->astroFile->getAstroImage(currentImage->currentHDB);

          if (astroImage)
          {
            currentImage->whitePoint = newVal;    // Store the value.

              // Set the black point maximum value.

            if (sbBlack->value() > newVal)
            {
              sbBlack->setValue(newVal);
            }

            redrawImage();
          }
          else
          {
            CODE_ERROR(astroManager);
          }
        }
        else
        {
          CODE_ERROR(astroManager);
        }
      };

        // Transform the value to correct range and set the slider up.

      spanSlider->setUpperValue(static_cast<int>(std::floor( ( (newVal - currentImage->astroFile->imageMin(currentImage->currentHDB)) /
                                                (currentImage->astroFile->imageMax(currentImage->currentHDB)
                                                 - currentImage->astroFile->imageMin(currentImage->currentHDB)) *
                                                (BW_SLIDER_UPPER - BW_SLIDER_LOWER)) + 0.5)));
    }

    /// @brief Processes the button press to reset the white point.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2017-08-28/GGB - Function created.

    void CHistogram::eventWhiteReset(bool)
    {
      if (currentImage)
      {
        FP_t whitePoint = ACL::whitePoint(currentImage->astroFile->imageMean(currentImage->currentHDB),
                                          currentImage->astroFile->imageStdev(currentImage->currentHDB));
        eventWhiteChanged(whitePoint);
      }
      else
      {
        astroManager_CODE_ERROR;
      }
    }

    /// @brief Routine to handle the slider movement.
    /// @param[in] newVal - The new white value
    /// @details The spinbox must also be updated. As the white value is changed the spin box must also be updated. Also the
    ///          limiting values must be changed.
    /// @throws GCL::CCodeError(astroManager)
    /// @version 2013-05-29/GGB - Added the spanSlider.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2010-10-30/GGB - Function created

    void CHistogram::eventWhiteSliderChanged(int newVal)
    {
      RUNTIME_ASSERT(astroManager, currentImage, "CurrentImage should not be null.");

      ACL::CAstroImage *astroImage = nullptr;

      astroImage = currentImage->astroFile->getAstroImage(currentImage->currentHDB);

      if (astroImage)
      {
        if (!bInternalCall)
        {
            // Set the value in the spin box.

          sbWhite->setValue(newVal * ((astroImage->getMax() - astroImage->getMin()) / (BW_SLIDER_UPPER - BW_SLIDER_LOWER)) + astroImage->getMin());
        };
      }
      else
      {
        CODE_ERROR(astroManager);
      };
    }

    /// @brief This function must be called when the owner of a controlImage object changes the image in the controlImage object.
    /// @throws GCL::CodeError
    /// @details This will then re-read all the data associated with the controlImage and update all the fields to ensure that only
    /// valid information is displayed.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-03-09/GGB - Converted whitePoint and blackPoint to boost::optional<double>
    /// @version 2011-06-05/GGB - Function created.

    void CHistogram::imageChanged()
    {
      long lIndex;

        // Prepare the output image.

      if (currentImage)
      {
          // Check if the image already has a black point and white point
          // Black point and white point are determined by the calls to the image.

        if (!currentImage->blackPoint)
        {
          currentImage->blackPoint = currentImage->astroFile->blackPoint();
        };

        if (!currentImage->whitePoint)
        {
          currentImage->whitePoint = currentImage->astroFile->whitePoint();
        };

          // Render the image to the current settings

        currentImage->astroFile->setImagePlaneRenderFunction(currentImage->currentHDB, 0, *(currentImage->blackPoint),
                                                             *(currentImage->whitePoint), checkBoxInvert->isChecked(),
                                                             transferFunction, gammaValue);
        currentImage->astroFile->renderImage(currentImage->currentHDB, ACL::RM_GREY8);


        if (currentImage->ScreenImage)
        {
          delete currentImage->ScreenImage;
          currentImage->ScreenImage = nullptr;
        };

        if (currentImage->astroFile->isMonoImage(currentImage->currentHDB))
        {

            // Working with a monochrome image

          currentImage->ScreenImage = new QImage((uchar *) currentImage->astroFile->getRenderedImage(currentImage->currentHDB),
                                                 currentImage->astroFile->imageWidth(currentImage->currentHDB),
                                                 currentImage->astroFile->imageHeight(currentImage->currentHDB),
                                                  sizeof(uchar) * currentImage->astroFile->imageWidth(currentImage->currentHDB),
                                                 QImage::Format_Indexed8);

          currentImage->ScreenImage->setColorCount(256);

            // Create the gray scale colour table

          for(lIndex = 0; lIndex <= 255; lIndex++)
          {
            currentImage->ScreenImage->setColor(lIndex, qRgb(lIndex, lIndex, lIndex));
          };
        }
        else if (currentImage->astroFile->isPolyImage(currentImage->currentHDB))
        {
            // Working with a colour image.

          //currentImage->ScreenImage = new QImage((uchar *) currentImage->astroFile->getOutputImage(currentImage->currentHDB),
//            currentImage->astroFile->imageWidth(currentImage->currentHDB), currentImage->astroFile->imageHeight(currentImage->currentHDB),
  //          sizeof(ACL::SColourRGB) * currentImage->astroFile->imageWidth(currentImage->currentHDB), QImage::Format_RGB888);
        }
        else
        {
          CODE_ERROR(astroManager);
        };

        currentImage->pixmap->convertFromImage(*currentImage->ScreenImage, Qt::MonoOnly);
        currentImage->parent_->repaintImage();    // Added to ensure the image is repainted. (2016-03-26)

        bInternalCall = true;   // Do not update the image.

        sbWhite->setMaximum(currentImage->astroFile->imageMax(currentImage->currentHDB));
        sbWhite->setMinimum(currentImage->astroFile->imageMin(currentImage->currentHDB));
        sbWhite->setValue(*currentImage->whitePoint);

        sbBlack->setMinimum(currentImage->astroFile->imageMin(currentImage->currentHDB));
        sbBlack->setMaximum(currentImage->astroFile->imageMax(currentImage->currentHDB));
        sbBlack->setValue(*currentImage->blackPoint);

        bInternalCall = false;

        DisplayHistogram();

        spanSlider->setEnabled(true);
        doubleSpinBoxGamma->setEnabled(true);
        comboBoxTransferFunction->setEnabled(true);
        checkBoxInvert->setEnabled(true);
        sbBlack->setEnabled(true);
        sbWhite->setEnabled(true);
        histogramPlot->setEnabled(true);
      };
    }

    /// @brief Redraws the image.
    /// @throws None.
    /// @version 2013-05-26/GGB - Corrected program flow for new renderOutputImage function.
    /// @version 2013-05-20/GGB - Added the pixmap member.
    /// @version 2013-03-17/GGB - Function flow cleaned up with introduction of CDockWidget.
    /// @version 2013-01-26/GGB - Function created

    void CHistogram::redrawImage()
    {
      if (currentImage)
      {
          // Do the transform

        currentImage->astroFile->setImagePlaneRenderFunction(currentImage->currentHDB, 0, sbBlack->value(),
                                                             sbWhite->value(), checkBoxInvert->isChecked(),
                                                             transferFunction, gammaValue);
        currentImage->astroFile->renderImage(currentImage->currentHDB, ACL::RM_GREY8);

        if (currentImage->ScreenImage)
        {
          delete currentImage->ScreenImage;
          currentImage->ScreenImage = nullptr;
        };

        if (currentImage->astroFile->isMonoImage(currentImage->currentHDB))
        {

            // Working with a monochrome image

          currentImage->ScreenImage = new QImage(static_cast<uchar *>(currentImage->astroFile->getRenderedImage(currentImage->currentHDB)),
                                                 currentImage->astroFile->imageWidth(currentImage->currentHDB),
                                                 currentImage->astroFile->imageHeight(currentImage->currentHDB),
                                                 sizeof(uchar) * currentImage->astroFile->imageWidth(currentImage->currentHDB),
                                                 QImage::Format_Indexed8);

          currentImage->ScreenImage->setColorCount(256);

            // Create the gray scale colour table

          for(int lIndex=0; lIndex <= 255; lIndex++)
          {
            currentImage->ScreenImage->setColor(lIndex, qRgb(lIndex, lIndex, lIndex));
          };
        }
        else if (currentImage->astroFile->isPolyImage(currentImage->currentHDB))
        {
            // Working with a colour image.

          //currentImage->ScreenImage = new QImage((uchar *) currentImage->astroFile->getOutputImage(currentImage->currentHDB),
//            currentImage->astroFile->imageWidth(currentImage->currentHDB), currentImage->astroFile->imageHeight(currentImage->currentHDB),
  //          sizeof(ACL::SColourRGB) * currentImage->astroFile->imageWidth(currentImage->currentHDB), QImage::Format_RGB888);
        }
        else
        {
          CODE_ERROR(astroManager);
        };

        //currentImage->pixmap->convertFromImage(*currentImage->ScreenImage, Qt::MonoOnly);

        currentImage->pixmap->convertFromImage(*currentImage->ScreenImage);
        currentImage->parent_->repaintImage();      // This ensures that the image is redrawn

        auto frameWindow = dynamic_cast<mdiframe::CFrameWindow *>(nativeParentWidget());

        dynamic_cast<dockwidgets::CDockWidgetMagnify &>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_MAGNIFY)).imageChanged();
        dynamic_cast<dockwidgets::CDockWidgetNavigator &>(frameWindow->getDockWidget(mdiframe::IDDW_VIEW_NAVIGATOR)).imageChanged();
      };
    }

    /// @brief Resets the values for the dockWidget
    /// @param[in] fMin - The minimum value for the scales.
    /// @param[in] fMax - The maximum value for the scales.
    /// @throws None.
    /// @version 2011-05-29/GGB - Function created.

    void CHistogram::resetValues(double fMin, double fMax)
    {
      if (currentImage)
      {
        sbBlack->setMinimum(fMin);
        sbBlack->setMaximum(fMax);
        sbBlack->setValue(fMin);

        sbWhite->setMinimum(fMin);
        sbWhite->setMaximum(fMax);
        sbWhite->setValue(fMax);
      };
    }

    /// @brief Sets up the UI and all UI associated variable. Performs all connections.
    /// @throws None.
    /// @version 2017-07-09/GGB - Added check and error for opening the widget from the .ui file.
    /// @version 2013-05-29/GGB - Changed the black and white sliders to span sliders.
    /// @version 2013-05-11/GGB - Added transfer function and gamma.
    /// @version 2011-05-28/GGB - Function created.

    void CHistogram::setupUI()
    {
      QUiLoader loader;
      QGridLayout *gridLayout;
      QGroupBox *groupBox;

      QFile file(":/forms/dwHistogram.ui");

      if (!file.open(QFile::ReadOnly))
      {
        ERRORMESSAGE("Cannot open resource :/forms/dwHistogram.ui");
        ERROR(astroManager, 0x0001);
      };

      QWidget *formWidget = loader.load(&file, this);
      file.close();

      setWidget(formWidget);

      //setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

      ASSOCIATE_CONTROL(sbBlack, formWidget, "sbBlack", QDoubleSpinBox);
      ASSOCIATE_CONTROL(sbWhite, formWidget, "sbWhite", QDoubleSpinBox);
      ASSOCIATE_CONTROL(checkBoxInvert, formWidget, "checkBoxInvert", QCheckBox);
      ASSOCIATE_CONTROL(comboBoxTransferFunction, formWidget, "comboBoxTransferFunction", QComboBox);
      ASSOCIATE_CONTROL(doubleSpinBoxGamma, formWidget, "doubleSpinBoxGamma", QDoubleSpinBox);
      ASSOCIATE_CONTROL(groupBox, formWidget, "groupBox", QGroupBox);

      if (!(gridLayout = dynamic_cast<QGridLayout *>(groupBox->layout())))
      {
        CODE_ERROR(astroManager);
      };

        // Create and add the double slider to the layout.

      spanSlider = new QxtSpanSlider(Qt::Horizontal);
      spanSlider->setHandleMovementMode(QxtSpanSlider::NoOverlapping);
      spanSlider->setMinimum(BW_SLIDER_LOWER);
      spanSlider->setMaximum(BW_SLIDER_UPPER);
      spanSlider->setTickPosition(QSlider::TicksBothSides);
      spanSlider->setTickInterval( (BW_SLIDER_UPPER - BW_SLIDER_LOWER) / 10);
      gridLayout->addWidget(spanSlider, 2, 0, 1, 2);

      doubleSpinBoxGamma->setValue(gammaValue);

      comboBoxTransferFunction->clear();
      comboBoxTransferFunction->addItem(QS_LINEAR);
      comboBoxTransferFunction->addItem(QS_GAMMA);
      comboBoxTransferFunction->addItem(QS_SQRT);
      comboBoxTransferFunction->addItem(QS_CBRT);
      comboBoxTransferFunction->addItem(QS_SIGMOID);
      comboBoxTransferFunction->addItem(QS_SQUARE);
      comboBoxTransferFunction->addItem(QS_GAMMALOG);
      comboBoxTransferFunction->addItem(QS_LOG);

      comboBoxTransferFunction->setCurrentIndex(transferFunction-1);

      connect(sbWhite, SIGNAL(valueChanged(double)), this, SLOT(eventWhiteChanged(double)));
      connect(sbBlack, SIGNAL(valueChanged(double)), this, SLOT(eventBlackChanged(double)));

      connect(spanSlider, SIGNAL(lowerValueChanged(int)), this, SLOT(eventBlackSliderChanged(int)));
      connect(spanSlider, SIGNAL(upperValueChanged(int)), this, SLOT(eventWhiteSliderChanged(int)));

      connect(checkBoxInvert, SIGNAL(stateChanged(int)), this, SLOT(eventInvertChanged(int)));
      connect(doubleSpinBoxGamma, SIGNAL(valueChanged(double)), this, SLOT(eventGammaChanged(double)));
      connect(comboBoxTransferFunction, SIGNAL(currentIndexChanged(QString const &)), this, SLOT(eventTranferFunctionChanged(QString const &)));

      connect(formWidget->findChild<QPushButton *>("pushButtonBlackPointReset"), SIGNAL(clicked(bool)), this, SLOT(eventBlackReset(bool)));
      connect(formWidget->findChild<QPushButton *>("pushButtonWhitePointReset"), SIGNAL(clicked(bool)), this, SLOT(eventWhiteReset(bool)));

      if (!(gridLayout = dynamic_cast<QGridLayout *>(formWidget->layout())))
      {
        CODE_ERROR(astroManager);
      };

        // Create the Histogram Widget

      histogramPlot = new QwtPlot();
      histogramPlot->setFixedHeight(143);
      histogramPlot->setFixedWidth(316);
      histogramPlot->setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
      histogramPlot->setAxisMaxMajor(QwtPlot::yLeft, 5);
      histogramPlot->setAxisMaxMajor(QwtPlot::xBottom, 5);

      histogramData.setStyle(QwtPlotHistogram::Columns);

      histogramData.attach(histogramPlot);
      gridLayout->addWidget(histogramPlot, 0, 0);

      spanSlider->setEnabled(false);
      doubleSpinBoxGamma->setEnabled(false);
      comboBoxTransferFunction->setEnabled(false);
      checkBoxInvert->setEnabled(false);
      sbBlack->setEnabled(false);
      sbWhite->setEnabled(false);
      histogramPlot->setEnabled(false);
    }

  }  // namespace dockwidgets
}  // namespace AstroManager
