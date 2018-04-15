//**********************************************************************************************************************************
//
// PROJECT:             AstroManager
// FILE:			          Qt
// SUBSYSTEM:           Qt headers
// TARGET OS:	          WINDOWS, LINUX, UNIX, MAC
// NAMESPACE:						AstroManager::photometry
// AUTHOR:              Gavin Blakeman
// LICENSE:             GPLv2
//
//                      Copyright 2015-2018 Gavin Blakeman.
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
// OVERVIEW:	          Header files for Qt
//
// CLASSES:		          None
//
// HISTORY:             2015-09-22 GGB - astroManager 2015.09 release
//
//**********************************************************************************************************************************

#ifndef QT_H_
#define QT_H_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4104)
#pragma warning(disable: 4127)
#pragma warning(disable: 4244)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4505)
#pragma warning(disable: 4512)
#pragma warning(disable: 4800)
#endif

#include <Qt>

#if QT_VERSION >= 0x050000
#include <QDateTime>
#include <QtPlugin>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtWidgets>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItemGroup>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsSimpleTextItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMdiSubWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include <QNetworkAccessManager>
#include <QtSql>
#include <QtUiTools/QUiLoader>
#else
#include <Qt>
#include <QtCore/Qt>
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include <QNetworkAccessManager>
#include <QtSql>
#include <QtUiTools/QtUiTools>
#endif

#include <GCL>


#ifdef _MSC_VER
#pragma warning(pop)
#endif

  /// @def ASSOCIATE_CONTROL(WIDGET, CONTROL, TYPE)
  /// This macro is used for associating controls in dialogs or similar with a variable.
  /// @param[in] WIDGET - The widget that contains the code.
  /// @param[in] CONTROL - The control and variable name. The control name is always assumed to be the same as the variable name.
  /// @param[in] TYPE - The Qt type of the control (QCheckBox etc)
  /// @version 2015-09-22/GGB - Macro created.

#define ASSOCIATE_CONTROL(POINTERNAME, WIDGET, CONTROLNAME, TYPE) \
  POINTERNAME = WIDGET->findChild<TYPE *>(CONTROLNAME); \
  RUNTIME_ASSERT(astroManager, POINTERNAME, "Control "#CONTROLNAME" not found." );

#define ASSOCIATE_LABEL(POINTERNAME, WIDGET, CONTROLNAME) ASSOCIATE_CONTROL(POINTERNAME, WIDGET, CONTROLNAME, QLabel)
#define ASSOCIATE_RADIOBUTTON(POINTERNAME, WIDGET, CONTROLNAME) ASSOCIATE_CONTROL(POINTERNAME, WIDGET, CONTROLNAME, QRadioButton)
#define ASSOCIATE_PUSHBUTTON(POINTERNAME, WIDGET, CONTROLNAME) ASSOCIATE_CONTROL(POINTERNAME, WIDGET, CONTROLNAME, QPushButton)
#define ASSOCIATE_SPINBOX(POINTERNAME, WIDGET, CONTROLNAME) ASSOCIATE_CONTROL(POINTERNAME, WIDGET, CONTROLNAME, QSpinBox)
#define ASSOCIATE_TABLEVIEW(POINTERNAME, WIDGET, CONTROLNAME) ASSOCIATE_CONTROL(POINTERNAME, WIDGET, CONTROLNAME, QTableView)

#endif // QT_H_H
