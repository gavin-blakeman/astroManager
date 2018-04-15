# Project file for AstroManager application

TARGET = AstroManager
TEMPLATE = app
CONFIG += qt thread debug qxt
QXT += core gui
DEFINES += BOOST_THREAD_USE_LIB QT_GUI_LIB QT_CORE_LIB

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
  "../../Library/WeatherLink" \
  "../boost 1.62" \
  "../../Library" \
  "../cfitsio" \
  "../ACL" \
  "../GCL" \
  #"../../Library/LibRaw" \
  "../MCL" \
  "../PCL" \
  "../qwt-6.1.3/src" \
  "../Qxt/include" \
  "../SCL" \
  "../SOFA/src" \
  "../GeographicLib-1.48/include" \
  "../astrometry++"

SOURCES += \
    Source/FrameWindow.cpp \
    Source/Error.cpp \
    Source/VSOPView.cpp \
    Source/Utilities.cpp \
    Source/TextEditor.cpp \
    Source/Settings.cpp \
    Source/Photometry.cpp \
    Source/ImageComparison.cpp \
    Source/FileBatchConvert.cpp \
    Source/Configure.cpp \
    Source/AstroGraphicsView.cpp \
    Source/TextEditorFITS.cpp \
    Source/StellarObjects.cpp \
    Source/dockWidgets/dockWidgetWeatherScale.cpp \
    Source/dockWidgets/dockWidget.cpp \
    Source/dockWidgets/dockWidgetImage.cpp \
    Source/dockWidgets/dockWidgetImageInformation.cpp \
    Source/dockWidgets/dockWidgetAstrometry.cpp \
    Source/dockWidgets/dockWidgetHistogram.cpp \
    Source/dockWidgets/dockWidgetMagnify.cpp \
    Source/dockWidgets/dockWidgetMessage.cpp \
    Source/dockWidgets/dockWidgetNavigator.cpp \
    Source/dockWidgets/dockWidgetPhotometry.cpp \
    Source/imaging/imageControl.cpp \
    Source/astrometry/astrometryObservation.cpp \
    Source/photometry/photometryObservation.cpp \
    Source/dockWidgets/dockWidgetWeather.cpp \
    Source/windowWeather/windowWeatherHistory.cpp \
    Source/windowWeather/windowWeather.cpp \
    Source/windowImage/windowImageDisplay.cpp \
    Source/windowImage/windowImage.cpp \
    Source/windowCalibration/windowCalibration.cpp \
    Source/windowCalibration/ImageCalibration.cpp \
    Source/database/databaseATID.cpp \
    Source/database/databaseWeather.cpp \
    Source/database/databaseARID.cpp \
    Source/database/database.cpp \
    Source/dialogs/dialogBinPixels.cpp \
    Source/dialogs/dialogOptions.cpp \
    Source/dialogs/dialogExportAsJPEG.cpp \
    Source/dialogs/dialogExportAsPNG.cpp \
    Source/dialogs/dialogFindStars.cpp \
    Source/dialogs/dialogImageCalibration.cpp \
    Source/dialogs/dialogImageCrop.cpp \
    Source/dialogs/dialogImageFloat.cpp \
    Source/dialogs/dialogImageResample.cpp \
    Source/dialogs/dialogSaveAligned.cpp \
    Source/dialogs/dialogSelectInstrument.cpp \
    Source/dialogs/dialogSelectObject.cpp \
    Source/dialogs/dialogSelectObserver.cpp \
    Source/dialogs/dialogSelectTelescope.cpp \
    Source/dialogs/dialogSimpleXY.cpp \
    Source/dialogs/dialogs.cpp \
    Source/dialogs/dialogWeatherEdit.cpp \
    Source/dockWidgets/dockWidgetWeatherParameters.cpp \
    Source/windowCalculation/windowCalculation.cpp \
    Source/windowCalculation/gregorian2JD.cpp \
    Source/windowCalculation/JD2Gregorian.cpp \
    Source/ACL/astroFile.cpp \
    Source/windowSelectImage.cpp \
    Source/qtExtensions/application.cpp \
    Source/qtExtensions/TextEditSink.cpp \
    Source/qtExtensions/MdiSubWindow.cpp \
    Source/qtExtensions/sqlQueryModel.cpp \
    Source/qtExtensions/selectImageQueryModel.cpp \
    Source/dialogs/dialogConfigureSite.cpp \
    Source/dialogs/dialogConfigureTelescope.cpp \
    Source/dialogs/dialogSelectImageVersion.cpp \
    Source/qtExtensions/selectImageVersionQueryModel.cpp \
    Source/ACL/FITSMemoryFileArray.cpp \
    Source/dialogs/dialogImageDetails.cpp \
    Source/dialogs/dialogSelectImages.cpp \
    Source/windowImage/windowImageStacking.cpp \
    Source/ACL/observatoryInformation.cpp \
    Source/CInstrumentDockwidget.cpp \
    Source/SIMBAD.cpp \
    Source/widgets/widget.cpp \
    Source/dialogs/dialogEditResources.cpp \
    Source/widgets/widgetSunInformation.cpp \
    Source/windowPlanning/windowPlanning.cpp \
    Source/qtExtensions/queryModelPlanning.cpp \
    Source/astroManager.cpp \
    Source/astroManagerHelp.cpp

HEADERS  += \
    Include/FrameWindow.h \
    Include/FileBatchConvert.h \
    Include/ImageComparison.h \
    Include/AstroGraphicsView.h \
    Include/VSOPView.h \
    Include/Utilities.h \
    Include/TextEditor.h \
    Include/Settings.h \
    Include/Photometry.h \
    Include/Error.h \
    Include/Configure.h \
    Include/TextEditorFITS.h \
    Include/StellarObjects.h \
    Include/dockWidgets/dockWidgetWeatherScale.h \
    Include/dockWidgets/dockWidget.h \
    Include/dockWidgets/dockWidgetImage.h \
    Include/dockWidgets/dockWidgetImageInformation.h \
    Include/dockWidgets/dockWidgetAstrometry.h \
    Include/dockWidgets/dockWidgetHistogram.h \
    Include/dockWidgets/dockWidgetMagnify.h \
    Include/dockWidgets/dockWidgetMessage.h \
    Include/dockWidgets/dockWidgetNavigator.h \
    Include/dockWidgets/dockWidgetPhotometry.h \
    Include/imaging/imageControl.h \
    Include/astrometry/astrometryObservation.h \
    Include/photometry/photometryObservation.h \
    Include/dockWidgets/dockWidgetWeather.h \
    Include/windowWeather/windowWeatherHistory.h \
    Include/windowWeather/windowWeather.h \
    Include/windowImage/windowImage.h \
    Include/windowImage/windowImageDisplay.h \
    Include/windowCalibration/windowCalibration.h \
    Include/windowCalibration/ImageCalibration.h \
    Include/database/database.h \
    Include/database/databaseARID.h \
    Include/database/databaseATID.h \
    Include/database/databaseWeather.h \
    Include/dialogs/dialogExportAsJPEG.h \
    Include/dialogs/dialogExportAsPNG.h \
    Include/dialogs/dialogFindStars.h \
    Include/dialogs/dialogImageCalibration.h \
    Include/dialogs/dialogImageCrop.h \
    Include/dialogs/dialogImageFloat.h \
    Include/dialogs/dialogImageResample.h \
    Include/dialogs/dialogs.h \
    Include/dialogs/dialogSaveAligned.h \
    Include/dialogs/dialogSelectInstrument.h \
    Include/dialogs/dialogSelectObject.h \
    Include/dialogs/dialogSelectObserver.h \
    Include/dialogs/dialogSelectTelescope.h \
    Include/dialogs/dialogSimpleXY.h \
    Include/dialogs/dialogWeatherEdit.h \
    Include/dialogs/dialogBinPixels.h \
    Include/dialogs/dialogOptions.h \
    Include/dockWidgets/dockWidgetWeatherParameters.h \
    Include/windowCalculation/windowCalculation.h \
    Include/windowCalculation/gregorian2JD.h \
    Include/windowCalculation/JD2Gregorian.h \
    Include/ACL/astroFile.h \
    Include/windowSelectImage.h \
    Include/qtExtensions/application.h \
    Include/qtExtensions/qt.h \
    Include/qtExtensions/TextEditSink.h \
    Include/qtExtensions/MdiSubWindow.h \
    Include/qtExtensions/sqlQueryModel.h \
    Include/qtExtensions/selectImageQueryModel.h \
    Include/dialogs/dialogConfigureSite.h \
    Include/dialogs/dialogConfigureTelescope.h \
    Include/ACL/telescope.h \
    Include/dialogs/dialogSelectImageVersion.h \
    Include/qtExtensions/selectImageVersionQueryModel.h \
    Include/ACL/FITSMemoryFileArray.h \
    Include/dialogs/dialogImageDetails.h \
    Include/dialogs/dialogSelectImages.h \
    Include/windowImage/windowImageStacking.h \
    Include/ACL/observatoryInformation.h \
    Include/CInstrumentDockwidget.h \
    Include/QDebugStream.h \
    Include/SIMBAD.h \
    Include/widgets/widget.h \
    Include/dialogs/dialogEditResources.h \
    Include/widgets/widgetDateTimePanel.h \
    Include/widgets/widgetSunInformation.h \
    Include/widgets/widgetMoonInformation.h \
    Include/widgets/widgetShortTermVisibility.h \
    Include/qtExtensions/queryModelPlanning.h \
    Include/windowPlanning/windowPlanning.h \
    Include/astroManager.h \
    Include/astroManagerHelp.h


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



