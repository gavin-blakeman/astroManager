//*********************************************************************************************************************************
//
// PROJECT:							astroManager (Astronomy Manager)
// FILE:								Settings
// SUBSYSTEM:						Registry Class
// LANGUAGE:						C++
// TARGET OS:						UNIX/LINUX/WINDOWS/MAC
// LIBRARY DEPENDANCE:	ACL, Qt
// NAMESPACE:						AstroManager::settings
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2009-2018 Gavin Blakeman.
//                      This file is part of the Astronomy Manager software(astroManager)
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
// OVERVIEW:            Implements the class and constants for storing persistant values for the program.
//
// HISTORY:		          2018-02-03 GGB - Rename Project to AstroManager
//                      2015-09-22 GGB - astroManager 2015.09 release
//                      2013-09-30 GGB - astroManager 2013.09 release.
//                      2013-03-22 GGB - astroManager 2013.03 release.
//                      2013-01-20 GGB - astroManager 0000.00 release.
//                      2011-12-18 GGB - Development of classes for astroManager
//
//*********************************************************************************************************************************

#ifndef astroManager_SETTINGS_H
#define astroManager_SETTINGS_H

#include "qtExtensions/qt.h"

namespace AstroManager
{
  namespace settings
  {
    int const RECENT_COUNT(10);

    QString const ORG_NAME                                          ("GPLv2");
    QString const APPL_NAME                                         ("AstroManager");

      // definitions for astroManager section

    QString const FRAME_GEOMETRY                                    ("Geometry");
    QString const FRAME_MAXIMISED                                   ("FrameMaximised");
    QString const FRAME_POSITION                                    ("FramePosition");
    QString const FRAME_SIZE                                        ("FrameSize");

    QString const SETTINGS_BASEDIR                                  ("BaseDir");
    QString const SETTINGS_LOGDIR                                   ("LogDir");
    QString const SETTINGS_LOGLEVEL                                 ("LogLevel");
    QString const SETTINGS_LT                                       ("Local Time");
    QString const FILE_LASTOPENEDDEPTH                              ("LastOpenedDepth");
    QString const FILE_LASTOPENED                                   ("LastOpened");
    QString const MAX_THREADS                                       ("MaximumThreads");
    QString const TOOLBAR_HEIGHT                                    ("ToolbarHeight");

    QString const SETTINGS_SITE_DEFAULTID                           ("Site/DefaultID");
    QString const SETTINGS_SITE_SAMEDISTANCE                        ("Site/SameDistance");

    QString const SETTINGS_OBSERVER                                 ("UserName");
#define SETTINGS_TELESCOPE			                    "Telescope"
#define SETTINGS_INSTRUMENT		  	                  "Instrument"

      // Various files

    QString const FILE_TAIUTC                                       ("TAIUTC");
    QString const FILE_UTCUT1                                       ("UTCUT1");

      // definitions for Astrometry section

    QString const ASTROMETRY_CSVDIRECTORY                           ("Astrometry/CSV/Directory");
    QString const ASTROMETRY_CSVDIRECTORY_UPDATE                    ("Astrometry/CSV/DirectoryUpdate");
    QString const ASTROMETRY_CSV_OUTPUTDEGREES                      ("Astrometry/CSV/OutputDegrees");
    QString const ASTROMETRY_CIRCLE_RADIUS                          ("Astrometry/CircleRadius");
    QString const ASTROMETRY_CENTROIDSEARCH_RADIUS                  ("Astrometry/CentroidSearch/Radius");
    QString const ASTROMETRY_CENTROIDSEARCH_SENSITIVITY             ("Astrometry/CentroidSearch/Sensitivity");
    QString const ASTROMETRY_DISPLAYINDICATORS                      ("Astrometry/DisplayIndicators");
    QString const ASTROMETRY_INDICATOR_SPACE                        ("Astrometry/Indicator/Space");
    QString const ASTROMETRY_INDICATOR_LENGTH                       ("Astrometry/Indicator/Length");
    QString const ASTROMETRY_INDICATOR_TYPE                         ("Astrometry/Indicator/Type");
    QString const ASTROMETRY_INDICATOR_COLOUR                       ("Astrometry/Indicator/Colour");
    QString const ASTROMETRY_INDICATOR_SELECTEDCOLOUR               ("Astrometry/Indicator/SelectedColour");
    QString const ASTROMETRY_TARGET_DIRECTORY                       ("Astrometry/Target/Directory");

      // Definitions for photometry section

    QString const PHOTOMETRY_USEZMAG                                ("Photometry/ZMAG");
    QString const PHOTOMETRY_CENTROIDSEARCH_RADIUS                  ("Photometry/CentroidSearch/Radius");
    QString const PHOTOMETRY_CENTROIDSEARCH_SENSITIVITY             ("Photometry/CentroidSearch/Sensitivity");
    QString const PHOTOMETRY_DISPLAYINDICATORS                      ("Photometry/DisplayIndicators");
    QString const PHOTOMETRY_RADIUS1                                ("Photometry/Radius1");
    QString const PHOTOMETRY_RADIUS2                                ("Photometry/Radius2");
    QString const PHOTOMETRY_RADIUS3                                ("Photometry/Radius3");
    QString const PHOTOMETRY_CSVDIRECTORY                           ("Photometry/CSV/Directory");
    QString const PHOTOMETRY_CSVDIRECTORY_UPDATE                    ("Photometry/CSV/DirectoryUpdate");
    QString const PHOTOMETRY_CSV_OUTPUTDEGREES                      ("Photometry/CSV/OutputDegrees");
    QString const PHOTOMETRY_INDICATOR_COLOUR                       ("Photometry/Indicator/Colour");
    QString const PHOTOMETRY_INDICATOR_SELECTEDCOLOUR               ("Photometry/Indicator/SelectedColour");
    QString const PHOTOMETRY_TARGET_DIRECTORY                       ("Photometry/Target/Directory");

      // Definitions for Annotations

    QString const ANNOTATIONS_DISPLAY                               ("Annotations/Display");

      // Definitions for Imaging section

    QString const IMAGING_DIRECTORY                                 ("Imaging/Directory");
    QString const IMAGING_INSPECT_DIRECTORY                         ("Imaging/Inspect/Directory");
    QString const IMAGING_DATABSE_SAVE                              ("Imaging/Database/Save");
    QString const IMAGING_DATABASE_SAVEORIGINAL                     ("Imaging/Database/SaveOriginal");      ///< Save the original as version 0.
    QString const IMAGING_DATABASE_SAVEVERSIONS                     ("Imaging/Database/SaveVersions");
    QString const IMAGING_DATABASE_MAXVERSIONS                      ("Imaging/Database/MaximumVersions");
    QString const IMAGING_DATABASE_REGISTERONOPEN                   ("Imaging/Database/RegisterOnOpen");
    QString const IMAGING_DATABASE_UPLOAD_DIRECTORY                 ("Imaging/Database/Directory");
    QString const IMAGING_KEYWORDS_CLEAN                            ("Imaging/Keywords/Clean");

      // Definitions for image stacking

    QString const IMAGESTACK_DIRECTORY                              ("ImageStack/Directory");
    QString const IMAGESTACK_UPDATEONOPEN                           ("ImageStack/UpdateOnOpen");
    QString const IMAGESTACK_OPENMAXIMISED                          ("ImageStack/OpenMaximised");
    QString const IMAGESTACK_OPENFROMFOLDER                         ("ImageStack/OpenFromFolder");
    QString const IMAGESTACK_MISSINGALIGNMENTACTION                 ("ImageStack/MissingAlignmentAction");
    QString const IMAGESTACK_AUTO_DISTANCE                          ("ImageStack/Auto/Distance");
    QString const IMAGESTACK_AUTO_NOWCSACTION                       ("ImageStack/Auto/NoWCSAction");
    QString const IMAGESTACK_AUTO_SAVEOUTPUT                        ("ImageStack/Auto/AutoSaveOutput");

      // Definitions for Window Planning

    QString const WINDOWPLANNING_TIMESCALE                          ("WindowPlanning/TimeScale");
    QString const WINDOWPLANNING_REALTIME                           ("WindowPlanning/RealTime");

      // Definitions for the ATID Database section

    QString const ATID_DATABASE_DISABLE                             ("Database/ATID/Disable");
    QString const ATID_DATABASE_DBMS                                ("Database/ATID/DBMS");
    QString const ATID_DATABASE_USESIMBAD                           ("Database/ATID/UseSIMBAD");

    QString const ATID_ORACLE_DRIVERNAME                            ("Database/ATID/Oracle/DriverName");
    QString const ATID_ORACLE_HOSTNAME                              ("Database/ATID/Oracle/HostName");
    QString const ATID_ORACLE_DATABASENAME                          ("Database/ATID/Oracle/DatabaseName");
    QString const ATID_ORACLE_USERNAME                              ("Database/ATID/Oracle/UserName");
    QString const ATID_ORACLE_PASSWORD                              ("Database/ATID/Oracle/Password");
    QString const ATID_ORACLE_PORT                                  ("Database/ATID/Oracle/Port");

    QString const ATID_ODBC_DRIVERNAME                              ("Database/ATID/ODBC/DriverName");
    QString const ATID_ODBC_DATASOURCENAME                          ("Database/ATID/ODBC/DataSourceName");

    QString const ATID_MYSQL_DRIVERNAME                             ("Database/ATID/MySQL/DriverName");
    QString const ATID_MYSQL_HOSTADDRESS                            ("Database/ATID/MySQL/HostAddress");
    QString const ATID_MYSQL_DATABASENAME                           ("Database/ATID/MySQL/DatabaseName");
    QString const ATID_MYSQL_PORT                                   ("Database/ATID/MySQL/Port");
    QString const ATID_MYSQL_USERNAME                               ("Database/ATID/MySQL/UserName");
    QString const ATID_MYSQL_PASSWORD                               ("Database/ATID/MySQL/Password");

    QString const ATID_SQLITE_DRIVERNAME                            ("Database/ATID/SQLite/DriverName");
    QString const ATID_SQLITE_DATABASENAME                          ("Database/ATID/SQLite/DatabaseName");

      // Definitions for the ARID Database section

    QString const ARID_DATABASE_DISABLE                             ("Database/ARID/Disable");
    QString const ARID_DATABASE_DBMS                                ("Database/ARID/DBMS");

    QString const ARID_ORACLE_DRIVERNAME                            ("Database/ARID/Oracle/DriverName");
    QString const ARID_ORACLE_HOSTADDRESS                           ("Database/ARID/Oracle/HostAddress");
    QString const ARID_ORACLE_SCHEMANAME                            ("Database/ARID/Oracle/DatabaseName");
    QString const ARID_ORACLE_USERNAME                              ("Database/ARID/Oracle/UserName");
    QString const ARID_ORACLE_PASSWORD                              ("Database/ARID/Oracle/Password");
    QString const ARID_ORACLE_PORT                                  ("Database/ARID/Oracle/Port");

    QString const ARID_ODBC_DRIVERNAME                              ("Database/ARID/ODBC/DriverName");
    QString const ARID_ODBC_DATASOURCENAME                          ("Database/ARID/ODBC/DataSourceName");

    QString const ARID_MYSQL_DRIVERNAME                             ("Database/ARID/MySQL/DriverName");
    QString const ARID_MYSQL_HOSTADDRESS                            ("Database/ARID/MySQL/HostAddress");
    QString const ARID_MYSQL_DATABASENAME                           ("Database/ARID/MySQL/DatabaseName");
    QString const ARID_MYSQL_PORT                                   ("Database/ARID/MySQL/Port");
    QString const ARID_MYSQL_USERNAME                               ("Database/ARID/MySQL/UserName");
    QString const ARID_MYSQL_PASSWORD                               ("Database/ARID/MySQL/Password");

    QString const ARID_SQLITE_DRIVERNAME                            ("Database/ARID/SQLite/DriverName");
    QString const ARID_SQLITE_DATABASENAME                          ("Database/ARID/SQLite/DatabaseName");

    QString const ARID_POSTGRESQL_HOSTADDRESS                       ("Database/ARID/PostgreSQL/HostAddress");
    QString const ARID_POSTGRESQL_PORT                              ("Database/ARID/PostgreSQL/Port");
    QString const ARID_POSTGRESQL_DATABASENAME                      ("Database/ARID/PostgreSQL/DatabaseName");
    QString const ARID_POSTGRESQL_USERNAME                          ("Database/ARID/PostgreSQL/UserName");
    QString const ARID_POSTGRESQL_PASSWORD                          ("Database/ARID/PostgreSQL/Password");

      // Definitions for the weather database

    QString const WEATHER_DATABASE_DISABLE                          ("Database/Weather/Disable");
    QString const WEATHER_DATABASE_DBMS                             ("Database/Weather/DBMS");

    QString const WEATHER_ORACLE_DRIVERNAME                         ("Database/Weather/Oracle/DriverName");
    QString const WEATHER_ORACLE_HOSTADDRESS                        ("Database/Weather/Oracle/HostName");
    QString const WEATHER_ORACLE_SCHEMANAME                         ("Database/Weather/Oracle/SchemaName");
    QString const WEATHER_ORACLE_USERNAME                           ("Database/Weather/Oracle/UserName");
    QString const WEATHER_ORACLE_PASSWORD                           ("Database/Weather/Oracle/Password");
    QString const WEATHER_ORACLE_PORT                               ("Database/Weather/Oracle/Port");

    QString const WEATHER_ODBC_DRIVERNAME                           ("Database/Weather/ODBC/DriverName");
    QString const WEATHER_ODBC_DATASOURCENAME                       ("Database/Weather/ODBC/DataSourceName");

    QString const WEATHER_MYSQL_DRIVERNAME                          ("Database/Weather/MySQL/DriverName");
    QString const WEATHER_MYSQL_HOSTADDRESS                         ("Database/Weather/MySQL/HostAddress");
    QString const WEATHER_MYSQL_DATABASENAME                        ("Database/Weather/MySQL/DatabaseName");
    QString const WEATHER_MYSQL_PORT                                ("Database/Weather/MySQL/Port");
    QString const WEATHER_MYSQL_USERNAME                            ("Database/Weather/MySQL/UserName");
    QString const WEATHER_MYSQL_PASSWORD                            ("Database/Weather/MySQL/Password");

    QString const WEATHER_SQLITE_DRIVERNAME                         ("Database/Weather/SQLite/DriverName");
    QString const WEATHER_SQLITE_DATABASENAME                       ("Database/Weather/SQLite/DatabaseName");

    QString const WEATHER_POSTGRESQL_DRIVERNAME                     ("Database/Weather/PostgreSQL/DriverName");
    QString const WEATHER_POSTGRESQL_HOSTADDRESS                    ("Database/Weather/PostgreSQL/HostAddress");
    QString const WEATHER_POSTGRESQL_DATABASENAME                   ("Database/Weather/PostgreSQL/DatabaseName");
    QString const WEATHER_POSTGRESQL_PORT                           ("Database/Weather/PostgreSQL/Port");
    QString const WEATHER_POSTGRESQL_USERNAME                       ("Database/Weather/PostgreSQL/UserName");
    QString const WEATHER_POSTGRESQL_PASSWORD                       ("Database/Weather/PostgreSQL/Password");

      // Definitions for imageCalibration

    QString const IMAGE_CALIBRATION_MASTERDARK_DIRECTORY            ("Image/Calibration/MasterDark/Directory");
    QString const IMAGE_CALIBRATION_MASTERBIAS_DIRECTORY            ("Image/Calibration/MasterBias/Directory");
    QString const IMAGE_CALIBRATION_MASTERFLAT_DIRECTORY            ("Image/Calibration/MasterFlat/Directory");

    QString const IMAGE_CALIBRATION_DARKFRAME_DIRECTORY             ("Image/Calibration/DarkFrames/Directory");
    QString const DARKFRAME_DIRECTORY_AUTOUPDATE                    ("Image/Calibration/DarkFrames/AutoUpdateDirectory");
    QString const IMAGE_CALIBRATION_FLATFRAME_DIRECTORY             ("Image/Calibration/FlatFrames/Directory");
    QString const FLATFRAME_DIRECTORY_AUTOUPDATE                    ("Image/Calibration/FlatFrames/AutoUpdateDirectory");
    QString const IMAGE_CALIBRATION_BIASFRAME_DIRECTORY             ("Image/Calibration/BiasFrames/Directory");
    QString const BIASFRAME_DIRECTORY_AUTOUPDATE                    ("Image/Calibration/BiasFrames/AutoUpdateDirectory");

    QString const IMAGE_CALIBRATION_CREATEDARK_USEBIAS              ("Image/Calibration/CreateDark/UseBias");
    QString const IMAGE_CALIBRATION_CREATEDARK_MASTERBIAS           ("Image/Calibration/CreateDark/MasterBias");
    QString const IMAGE_CALIBRATION_CREATEDARK_BIASSELECTION        ("Image/Calibration/CreateDark/BiasSelection");
    QString const IMAGE_CALIBRATION_CREATEDARK_BIASSAVEAS           ("Image/Calibration/CreateDark/BiasSaveAs");

    QString const IMAGE_CALIBRATION_CREATEFLAT_USEDARK              ("Image/Calibration/CreateFlat/UseDark");
    QString const IMAGE_CALIBRATION_CREATEFLAT_MASTERDARK           ("Image/Calibration/CreateFlat/MasterDark");
    QString const IMAGE_CALIBRATION_CREATEFLAT_DARKSELECTION        ("Image/Calibration/CreateFlat/DarkSelection");
    QString const IMAGE_CALIBRATION_CREATEFLAT_DARKSAVEAS           ("Image/Calibration/CreateFlat/DarkSaveAs");

    QString const IMAGE_CALIBRATION_RAWIMAGESDIRECTORY              ("Image/Calibration/Raw Images/Directory");
    QString const IMAGE_CALIBRATION_PROCESSEDDIRECTORY              ("Image/Calibration/Processed Images/Directory");

      // Definitions for image Comparison

    QString const IMAGE_COMPARISON_DIRECTORY                        ("Image/Comparison/Directory");
    QString const IMAGE_COMPARISON_UPDATEONOPEN                     ("Image/Comparison/UpdateOnOpen");
    QString const IMAGE_COMPARISON_METHOD                           ("Image/Comparison/Method");
    QString const IMAGE_COMPARISON_BLINKINTERVAL                    ("Image/Comparison/BlinkInterval");

      // Definitions for Source Extraction

    QString const SOURCE_EXTRACTION_ALGORITHM                       ("SourceExtraction/Algorithm");
    QString const SOURCE_EXTRACTION_ADD_ASTROMETRY                  ("SourceExtraction/AddAstrometry");
    QString const SOURCE_EXTRACTION_ADD_PHOTOMETRY                  ("SourceExtraction/AddPhotometry");

      // Definitions for Source Extraction - Find Stars

    QString const SOURCE_EXTRACTION_FINDSTARS_BORDER                ("SourceExtraction/FindStars/Border");
    QString const SOURCE_EXTRACTION_FINDSTARS_RNOISE                ("SourceExtraction/FindStars/RNoise");
    QString const SOURCE_EXTRACTION_FINDSTARS_MINPEAK               ("SourceExtraction/FindStars/MinPeak");
    QString const SOURCE_EXTRACTION_FINDSTARS_STARSIGMA             ("SourceExtraction/FindStars/StarSigma");
    QString const SOURCE_EXTRACTION_FINDSTARS_ISTATPIX              ("SourceExtraction/FindStars/ISTATPIX");
    QString const SOURCE_EXTRACTION_FINDSTARS_NSTATPIX              ("SourceExtraction/FindStars/NSTATPIX");
    QString const SOURCE_EXTRACTION_FINDSTARS_MAXWALK               ("SourceExtraction/FindStars/MaxWalk");
    QString const SOURCE_EXTRACTION_FINDSTARS_BURNEDOUT             ("SourceExtraction/FindStars/BurnedOut");
    QString const SOURCE_EXTRACTION_FINDSTARS_MINSEP                ("SourceExtraction/FindStars/MinSep");
    QString const SOURCE_EXTRACTION_FINDSTARS_MINRAD                ("SourceExtraction/FindStars/MinRad");
    QString const SOURCE_EXTRACTION_FINDSTARS_MAXRAD                ("SourceExtraction/FindStars/MaxRad");

      // Definitions for Source Extraction - SimpleXY

    QString const SOURCE_EXTRACTION_SIMPLEXY_DPSF                   ("SourceExtraction/SimpleXY/dpsf");
    QString const SOURCE_EXTRACTION_SIMPLEXY_PLIM                   ("SourceExtraction/SimpleXY/plim");
    QString const SOURCE_EXTRACTION_SIMPLEXY_DLIM                   ("SourceExtraction/SimpleXY/dlim");
    QString const SOURCE_EXTRACTION_SIMPLEXY_SADDLE                 ("SourceExtraction/SimpleXY/Saddle");
    QString const SOURCE_EXTRACTION_SIMPLEXY_MAXPER                 ("SourceExtraction/SimpleXY/maxper");
    QString const SOURCE_EXTRACTION_SIMPLEXY_MAXNPEAKS              ("SourceExtraction/SimpleXY/maxnpeaks");
    QString const SOURCE_EXTRACTION_SIMPLEXY_MAXSIZE                ("SourceExtraction/SimpleXY/maxsize");
    QString const SOURCE_EXTRACTION_SIMPLEXY_HALFBOX                ("SourceExtraction/SimpleXY/halfbox");
    QString const SOURCE_EXTRACTION_SIMPLEXY_SIGMA                  ("SourceExtraction/SimpleXY/sigma");

      // Definitions for Load Objects

    QString const LOAD_OBJECTS_ADDASTOMETRY                         ("Load Objects/AddAstrometry");
    QString const LOAD_OBJECTS_ADDPHOTOMETRY                        ("Load Objects/AddPhotometry");

      // Definitions for dialogs

    QString const DIALOG_BINPIXELS_BINSIZE                          ("Dialogs/BinPixels/BinSize");
    QString const DIALOG_IMAGERESAMPLE_ASPECT                       ("Dialog/ResampleImage/MaintainAspectRatio");
    QString const DIALOG_ROTATEIMAGE_LASTVALUE                      ("Dialog/RotateImage/LastValue");
    QString const DIALOG_EXPORTASJPEG_QUALITY                       ("Dialog/ExportAsJPEG/Quality");
    QString const DIALOG_EXPORTASPNG_QUALITY                        ("Dialog/ExportAsJPEG/Quality");
    QString const DIALOG_SELECTOBJECT_RECENT                        ("Dialog/SelectObjects/Recent/%1");

      // Calibrate current image dialog

    QString const DIALOG_SINGLEIMAGECALIBRATION_USEFLAT             ("Dialog/CalibrateCurrentImage/UseFlatFrame");
    QString const DIALOG_SINGLEIMAGECALIBRATION_USEBIAS             ("Dialog/CalibrateCurrentImage/UseBiasFrame");
    QString const DIALOG_SINGLEIMAGECALIBRATION_MASTERDARK          ("Dialog/CalibrateCurrentImage/MasterDarkFrame");
    QString const DIALOG_SINGLEIMAGECALIBRATION_MASTERFLAT          ("Dialog/CalibrateCurrentImage/MasterFlatFrame");
    QString const DIALOG_SINGLEIMAGECALIBRATION_MASTERBIAS          ("Dialog/CalibrateCurrentImage/MasterBiasFrame");
    QString const DIALOG_SINGLEIMAGECALIBRATION_IGNORETEMPERATURE   ("Dialog/CalibrateCurrentImage/IgnoreTemperature");
    QString const DIALOG_SINGLEIMAGECALIBRATION_IGNOREEXPOSURE      ("Dialog/CalibrateCurrentImage/IgnoreExposure");
    QString const DIALOG_SINGLEIMAGECALIBRATION_SAVEORIGINAL        ("Dialog/CalibrateCurrentImage/SaveOriginal");
    QString const DIALOG_SINGLEIMAGECALIBRATION_APPENDFRAMES        ("Dialog/CalibrateCurrentImage/AppendFrames");

    QString const DIALOG_IMAGEFLOAT_BACKGROUND                      ("Dialog/FloatImage/Background");
    QString const DIALOG_IMAGEFLOAT_MANUAL                          ("Dialog/FloatImage/ManualValue");

      // Dialog "File | Batch Convert"

    QString const DIALOG_BATCHCONVERT_DIRECTORY                     ("Dialog/BatchConvert/Directory");
    QString const DIALOG_BATCHCONVERT_OUTPUTDIRECTORY               ("Dialog/BatchConvert/OutputDirectory");
    QString const DIALOG_BATCHCONVERT_SITE                          ("Dialog/BatchConvert/Site");
    QString const DIALOG_BATCHCONVERT_SITENAME                      ("Dialog/BatchConvert/SiteName");
    QString const DIALOG_BATCHCONVERT_WEATHER                       ("Dialog/BatchConvert/Weather");
    QString const DIALOG_BATCHCONVERT_OBSERVER                      ("Dialog/BatchConvert/Observer");
    QString const DIALOG_BATCHCONVERT_OBSERVERNAME                  ("Dialog/BatchConvert/ObserverName");

      // Definitions for weather

    QString const WEATHER_WEATHERDIRECTORY                          ("Weather/WeatherDirectory");

      // Definitions for dockwidgets

    QString const DW_STATE                                          ("DockWidgets/State");

    QString const DW_IMAGE_ASTROMETRY_VISIBLE                       ("DockWidgets/Image/Astrometry/Visible");

    QString const DW_IMAGE_MAGNIFY_VISIBLE                          ("DockWidgets/Image/Magnify/Visible");
    QString const DW_MAGNIFY_MAGNIFICATION                          ("DockWidgets/Image/Magnify/Magnification");
    QString const DW_MAGNIFY_FLOATING                               ("DockWidgets/Image/Magnify/Floating");
    QString const DW_MAGNIFY_POSITION                               ("DockWidgets/Image/Magnify/Position");

    QString const DW_IMAGE_NAVIGATOR_VISIBLE                        ("DockWidgets/Image/Navigator/Visible");
    QString const DW_NAVIGATOR_FLOATING                             ("DockWidgets/Image/Navigator/Floating");
    QString const DW_NAVIGATOR_POSITION                             ("DockWidgets/Image/Navigator/Position");
    QString const DW_NAVIGATOR_SIZE                                 ("DockWidgets/Image/Navigator/Size");

    QString const DW_IMAGE_HISTOGRAM_VISIBLE                        ("DockWidgets/Image/Histogram/Visible");
    QString const DW_HISTOGRAM_FLOATING                             ("DockWidgets/Image/Histogram/Floating");
    QString const DW_HISTOGRAM_POSITION                             ("DockWidgets/Image/Histogram/Position");
    QString const DW_HISTOGRAM_SIZE                                 ("DockWidgets/Image/Histogram/Size");
    QString const DW_HISTOGRAM_BINS                                 ("DockWidgets/Image/Histogram/Bins");
    QString const DW_HOTOGRAM_GAMMA                                 ("DockWidgets/Image/Histogram/Gamma");
    QString const DW_HISTOGRAM_TRANSFERFUNCTION                     ("DockWidgets/Image/Histogram/TransferFunction");

    QString const DW_IMAGE_INFORMATION_VISIBLE                      ("DockWidgets/Image/Information/Visible");

    QString const DW_IMAGE_PHOTOMETRY_VISIBLE                       ("DockWidgets/Image/Photometry/Visible");

      // dockWidgetWeatherScale

    QString const DW_WEATHER_SCALE_VISIBLE                          ("DockWidgets/Weather/Scale/Visible");

      // dockWidgetWeatherParameters

    QString const DW_WEATHER_PARAMETERS_VISIBLE                     ("DockWidgets/Weather/Parameters/Visible");

    QString const DW_MESSAGE_VISIBLE                                ("DockWidgets/Message/Visible");

      // JD->Gregorian Window

    QString const WINDOW_CALCULATION_JD2GREG_JDSELECTED             ("Windows/Calculation/JD2GREG/JDSelected");

      // Confirmation messages

    QString const CM_DISCLAIMER                                     ("ConfirmationMessages/Disclaimer");
    QString const CM_IMAGECOMPARISON_REMOVE                         ("ConfirmationMessages/ImageComparison/Remove");
    QString const CM_IMAGECOMPARISON_REMOVEALL                      ("ConfirmationMessages/ImageComparison/RemoveAll");
    QString const CM_IMAGESTACKING_REMOVE                           ("ConfirmationMessages/ImageStacking/Remove");
    QString const CM_IMAGESTACKING_REMOVEALL                        ("ConfirmationMessages/ImageStacking/RemoveAll");
    QString const CM_DATABASE_OPENFILE_LASTVERSION                  ("ConfirmationMessages/Database/OpenFile/LatestVersion");

      // Other definitions

    QString const SQLITE                                            ("SQLite");

    int const LL_CRITICAL = 1;
    int const LL_ERROR  =   2;
    int const LL_WARNING =  4;
    int const LL_NOTICE =   8;
    int const LL_INFO =     16;
    int const LL_DEBUG =    32;
    int const LL_TRACE =    64;

    extern QSettings *astroManagerSettings;

    void InsertRecentObject(const QString &);

    void InitialiseStartupSettings();

    void createDefaultARIDSettings();

    class CTemporaryDefaults
    {
    private:
    protected:
    public:
    };

  }  // namespace settings

}	// namespace AstroManager

#endif // SETTINGS_H
