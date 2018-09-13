#-----------------------------------------------------------------------------------------------------------------------------------
#
# PROJECT:            Astronomy Class Library
# FILE:								ACL.pro
# SUBSYSTEM:          Project File
# LANGUAGE:						C++
# TARGET OS:          WINDOWS/UNIX/LINUX/MAC
# LIBRARY DEPENDANCE:	None.
# NAMESPACE:          N/A
# AUTHOR:							Gavin Blakeman.
# LICENSE:            GPLv2
#
#                     Copyright 2013-2018 Gavin Blakeman.
#                     This file is part of the Astronomy Class Library (ACL).
#
#                     ACL is free software: you can redistribute it and/or modify it under the terms of the GNU General
#                     Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
#                     option) any later version.
#
#                     ACL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
#                     implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#                     for more details.
#
#                     You should have received a copy of the GNU General Public License along with ACL.  If not, see
#                     <http://www.gnu.org/licenses/>.
#
# OVERVIEW:						Project file for compiling the project
#
# HISTORY:            2013-06-15/GGB - Development of classes for astroManager
#
#-----------------------------------------------------------------------------------------------------------------------------------

TARGET = astroManager
TEMPLATE = app
CONFIG += qt thread debug qxt
QXT += core gui
DEFINES += BOOST_THREAD_USE_LIB QT_GUI_LIB QT_CORE_LIB USE_SOFA

QT += core gui sql network printsupport uitools widgets svg

QTPLUGIN += qsqlmysql \
#            qtiff \
#            qsqlite \
#            qsqloci \
#            qsqlodbc \
#            qsqlpsql
#           qjpeg \

QMAKE_CXXFLAGS += -std=c++17 -static -static-libgcc #-save-temps=obj
QMAKE_LFLAGS += -fopenmp

#DESTDIR = ""
OBJECTS_DIR = "objects"
UI_DIR = "objects/ui"
MOC_DIR = "objects/moc"
RCC_DIR = "objects/rcc"


INCLUDEPATH += \
  "../boost 1.62" \
  "../cfitsio" \
  "../ACL" \
  "../astrometry" \
  "../GCL" \
  #"../../Library/LibRaw" \
  "../MCL" \
  "../QCL" \
  "../PCL" \
  "../qwt-6.1.3/src" \
  "../Qxt/include" \
  "../SCL" \
  "../SOFA/src" \
  "../GeographicLib-1.48/include" \
  "../astrometry++"

SOURCES += \
    source/FrameWindow.cpp \
    source/VSOPView.cpp \
    source/Utilities.cpp \
    source/TextEditor.cpp \
    source/Photometry.cpp \
    source/ImageComparison.cpp \
    source/FileBatchConvert.cpp \
    source/Configure.cpp \
    source/AstroGraphicsView.cpp \
    source/TextEditorFITS.cpp \
    source/StellarObjects.cpp \
    source/dockWidgets/dockWidgetWeatherScale.cpp \
    source/dockWidgets/dockWidget.cpp \
    source/dockWidgets/dockWidgetImage.cpp \
    source/dockWidgets/dockWidgetImageInformation.cpp \
    source/dockWidgets/dockWidgetAstrometry.cpp \
    source/dockWidgets/dockWidgetHistogram.cpp \
    source/dockWidgets/dockWidgetMagnify.cpp \
    source/dockWidgets/dockWidgetMessage.cpp \
    source/dockWidgets/dockWidgetNavigator.cpp \
    source/dockWidgets/dockWidgetPhotometry.cpp \
    source/imaging/imageControl.cpp \
    source/astrometry/astrometryObservation.cpp \
    source/photometry/photometryObservation.cpp \
    source/dockWidgets/dockWidgetWeather.cpp \
    source/windowWeather/windowWeatherHistory.cpp \
    source/windowWeather/windowWeather.cpp \
    source/windowImage/windowImageDisplay.cpp \
    source/windowImage/windowImage.cpp \
    source/windowCalibration/windowCalibration.cpp \
    source/windowCalibration/ImageCalibration.cpp \
    source/database/databaseATID.cpp \
    source/database/databaseWeather.cpp \
    source/database/databaseARID.cpp \
    source/dialogs/dialogBinPixels.cpp \
    source/dialogs/dialogOptions.cpp \
    source/dialogs/dialogExportAsJPEG.cpp \
    source/dialogs/dialogExportAsPNG.cpp \
    source/dialogs/dialogFindStars.cpp \
    source/dialogs/dialogImageCalibration.cpp \
    source/dialogs/dialogImageCrop.cpp \
    source/dialogs/dialogImageFloat.cpp \
    source/dialogs/dialogImageResample.cpp \
    source/dialogs/dialogSaveAligned.cpp \
    source/dialogs/dialogSelectInstrument.cpp \
    source/dialogs/dialogSelectObject.cpp \
    source/dialogs/dialogSelectObserver.cpp \
    source/dialogs/dialogSelectTelescope.cpp \
    source/dialogs/dialogSimpleXY.cpp \
    source/dialogs/dialogs.cpp \
    source/dialogs/dialogWeatherEdit.cpp \
    source/dockWidgets/dockWidgetWeatherParameters.cpp \
    source/windowCalculation/windowCalculation.cpp \
    source/windowCalculation/gregorian2JD.cpp \
    source/windowCalculation/JD2Gregorian.cpp \
    source/ACL/astroFile.cpp \
    source/windowSelectImage.cpp \
    source/qtExtensions/application.cpp \
    source/qtExtensions/TextEditSink.cpp \
    source/qtExtensions/MdiSubWindow.cpp \
    source/qtExtensions/sqlQueryModel.cpp \
    source/qtExtensions/selectImageQueryModel.cpp \
    source/dialogs/dialogConfigureSite.cpp \
    source/dialogs/dialogConfigureTelescope.cpp \
    source/dialogs/dialogSelectImageVersion.cpp \
    source/qtExtensions/selectImageVersionQueryModel.cpp \
    source/ACL/FITSMemoryFileArray.cpp \
    source/dialogs/dialogImageDetails.cpp \
    source/dialogs/dialogSelectImages.cpp \
    source/windowImage/windowImageStacking.cpp \
    source/ACL/observatoryInformation.cpp \
    source/CInstrumentDockwidget.cpp \
    source/SIMBAD.cpp \
    source/widgets/widget.cpp \
    source/dialogs/dialogEditResources.cpp \
    source/widgets/widgetSunInformation.cpp \
    source/windowPlanning/windowPlanning.cpp \
    source/astroManager.cpp \
    source/astroManagerHelp.cpp \
    source/ACL/targetAstronomy.cpp \
    source/error.cpp \
    source/settings.cpp

HEADERS  += \
    include/FrameWindow.h \
    include/FileBatchConvert.h \
    include/ImageComparison.h \
    include/AstroGraphicsView.h \
    include/VSOPView.h \
    include/Utilities.h \
    include/TextEditor.h \
    include/Photometry.h \
    include/Configure.h \
    include/TextEditorFITS.h \
    include/StellarObjects.h \
    include/dockWidgets/dockWidgetWeatherScale.h \
    include/dockWidgets/dockWidget.h \
    include/dockWidgets/dockWidgetImage.h \
    include/dockWidgets/dockWidgetImageInformation.h \
    include/dockWidgets/dockWidgetAstrometry.h \
    include/dockWidgets/dockWidgetHistogram.h \
    include/dockWidgets/dockWidgetMagnify.h \
    include/dockWidgets/dockWidgetMessage.h \
    include/dockWidgets/dockWidgetNavigator.h \
    include/dockWidgets/dockWidgetPhotometry.h \
    include/imaging/imageControl.h \
    include/astrometry/astrometryObservation.h \
    include/photometry/photometryObservation.h \
    include/dockWidgets/dockWidgetWeather.h \
    include/windowWeather/windowWeatherHistory.h \
    include/windowWeather/windowWeather.h \
    include/windowImage/windowImage.h \
    include/windowImage/windowImageDisplay.h \
    include/windowCalibration/windowCalibration.h \
    include/windowCalibration/ImageCalibration.h \
    include/database/databaseARID.h \
    include/database/databaseATID.h \
    include/database/databaseWeather.h \
    include/dialogs/dialogExportAsJPEG.h \
    include/dialogs/dialogExportAsPNG.h \
    include/dialogs/dialogFindStars.h \
    include/dialogs/dialogImageCalibration.h \
    include/dialogs/dialogImageCrop.h \
    include/dialogs/dialogImageFloat.h \
    include/dialogs/dialogImageResample.h \
    include/dialogs/dialogs.h \
    include/dialogs/dialogSaveAligned.h \
    include/dialogs/dialogSelectInstrument.h \
    include/dialogs/dialogSelectObject.h \
    include/dialogs/dialogSelectObserver.h \
    include/dialogs/dialogSelectTelescope.h \
    include/dialogs/dialogSimpleXY.h \
    include/dialogs/dialogWeatherEdit.h \
    include/dialogs/dialogBinPixels.h \
    include/dialogs/dialogOptions.h \
    include/dockWidgets/dockWidgetWeatherParameters.h \
    include/windowCalculation/windowCalculation.h \
    include/windowCalculation/gregorian2JD.h \
    include/windowCalculation/JD2Gregorian.h \
    include/ACL/astroFile.h \
    include/windowSelectImage.h \
    include/qtExtensions/application.h \
    include/qtExtensions/TextEditSink.h \
    include/qtExtensions/MdiSubWindow.h \
    include/qtExtensions/sqlQueryModel.h \
    include/qtExtensions/selectImageQueryModel.h \
    include/dialogs/dialogConfigureSite.h \
    include/dialogs/dialogConfigureTelescope.h \
    include/ACL/telescope.h \
    include/dialogs/dialogSelectImageVersion.h \
    include/qtExtensions/selectImageVersionQueryModel.h \
    include/ACL/FITSMemoryFileArray.h \
    include/dialogs/dialogImageDetails.h \
    include/dialogs/dialogSelectImages.h \
    include/windowImage/windowImageStacking.h \
    include/ACL/observatoryInformation.h \
    include/CInstrumentDockwidget.h \
    include/SIMBAD.h \
    include/widgets/widget.h \
    include/dialogs/dialogEditResources.h \
    include/widgets/widgetDateTimePanel.h \
    include/widgets/widgetSunInformation.h \
    include/widgets/widgetMoonInformation.h \
    include/widgets/widgetShortTermVisibility.h \
    include/windowPlanning/windowPlanning.h \
    include/astroManager.h \
    include/astroManagerHelp.h \
    include/ACL/targetAstronomy.h \
    include/error.h \
    include/settings.h


RESOURCES += \
    Resource/VSOP.qrc

win32:CONFIG(release, debug|release) {
  LIBS += -L../../Library/Library/win32/release/ -lGCL
  LIBS += -L../../Library/Library/win32/release -lAstroClass
}
else:win32:CONFIG(debug, debug|release) {
  LIBS += -L../../Library/Library/win32/debug -lACL
  LIBS += -L../../Library/Library/win32/debug -lCCFits
  LIBS += -L../../Library/Library/win32/debug -lSBIG
  LIBS += -L../../Library/Library/win32/debug -lCfitsio
  LIBS += -L../../Library/Library/win32/debug -lGCL
  LIBS += -L../../Library/Library/win32/debug -lMCL
  LIBS += -L../../Library/Library/win32/debug -lNOVAS
  LIBS += -L../../Library/Library/win32/debug -lPCL
  LIBS += -L../../Library/Library/win32/debug -lQwt
  #LIBS += -L../../Library/Library/win32/debug -lRaw
  LIBS += -L../../Library/Library/win32/debug -lSCL
  LIBS += -L../../Library/Library/win32/debug -lWCS
  LIBS += -L../../Library/Library/win32/debug -lWeatherLink
  LIBS += -L../../Library/Library/win32/debug -lboost_filesystem
  LIBS += -L../../Library/Library/win32/debug -lboost_system
  LIBS += -L../../Library/Library/win32/debug -lboost_thread
  LIBS += -L../../Library/Library/win32/debug -lboost_chrono
  LIBS += -L../../Library/Library/win32/debug -lSOFA
  LIBS += -L../../Library/Library/win32/debug -lQxt
}
else:unix:CONFIG(debug, debug|release) {
  LIBS += -L../ACL -lACL
  LIBS += -L../../Library/Library -lSBIG
  LIBS += -L../../Library/Library -lcfitsio
  LIBS += -L../GCL -lGCL
  LIBS += -L../MCL -lMCL
  LIBS += -L../NOVAS -lNOVAS
  LIBS += -L../PCL -lPCL
  LIBS += -L../qwt-6.1.3/lib -lqwt
  #LIBS += -L../../Library/Library -lRaw
  LIBS += -L../SCL -lSCL
  LIBS += -L../../Library/Library -lWCS
  LIBS += -L../QCL -lQCL
  LIBS += -L../WCL -lWCL
  LIBS += -L../../../Library/Library -lboost_filesystem
  LIBS += -L../../../Library/Library/unix/debug -lboost_system
  LIBS += -L../../Library/Library/unix/debug -lboost_thread
  LIBS += -L../../Library/Library/unix/debug -lboost_chrono
  LIBS += -L../SOFA -lSOFA
  LIBS += -L../../Library/Library -lQxt
  LIBS += -L../GeographicLib-1.48 -lGeographicLib
}
else:unix:CONFIG(release, debug|release) {
  LIBS += -L../ACL -lACL
  LIBS += -L../../Library/Library/unix/release -lSBIG
  LIBS += -L../../Library/Library/unix/release -lcfitsio
  LIBS += -L../../Library/Library/unix/release -lGCL
  LIBS += -L../../Library/Library/unix/release -lMCL
  LIBS += -L../../Library/Library/unix/release -lNOVAS
  LIBS += -L../../Library/Library/unix/release -lPCL
  LIBS += -L../../Library/Library/unix/release -lqwt
  #LIBS += -L../../Library/Library/unix/release -lRaw
  LIBS += -L../../Library/Library/unix/release -lSCL
  LIBS += -L../QCL -lQCL
  LIBS += -L../../Library/Library/unix/release -lWCS
  LIBS += -L../../Library/Library/unix/release -lWCL
  LIBS += -L../../Library/Library/unix/release -lboost_filesystem
  LIBS += -L../../Library/Library/unix/release -lboost_system
  LIBS += -L../../Library/Library/unix/release -lboost_thread
  LIBS += -L../../Library/Library/unix/release -lboost_chrono
  LIBS += -L../../Library/Library/unix/release -lSOFA
  LIBS += -L../../Library/Library/unix/release -lQxt
  LIBS += -L../../Library/Library/unix/release -lGeographicLib
}

OTHER_FILES += \
    changelog.txt \
    license.txt \
    README

DISTFILES +=



