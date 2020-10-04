Settings
========

All settings are stored in a setting file managed by the Qt code.

All settings are referred to by QString constants stored in the settings.h file. (namespace astroManager::settings)

Organisation Name - GPL2
Application Name = astroManager

ATID Database
	Oracle
		DriverName
		HostName
		DatabaseName
		UserName
		Password
		Port
	ODBC
		DriverName
		DatabaseName
	MySQL
		DriverName
		HostAddress
		DatabaseName
		Port
		UserName
		Password

Weather Database/Database
Weather Database/Disable
Weather Database/Oracle/DriverName
Weather Database/Oracle/HostName
Weather Database/Oracle/DatabaseName
Weather Database/Oracle/UserName
Weather Database/Oracle/Password
Weather Database/Oracle/Port
Weather Database/ODBC/DriverName
Weather Database/ODBC/DataSourceName
Weather Database/MySQL/DriverName
Weather Database/MySQL/HostAddress
Weather Database/MySQL/DatabaseName
Weather Database/MySQL/Port
Weather Database/MySQL/UserName
Weather Database/MySQL/Password
Weather Database/SQLite/DriverName
Weather Database/SQLite/DatabaseName

MPCORB File
-----------

| Constant               | Useage                                      |
|------------------------|---------------------------------------------|
| FILE_MPCORB_LOCATION   | File name and location of MPC orbitals file |
| FILE_MPCORB_UPDATE     | Update frequency in days.                   |
| FILE_MPCORB_LASTUPDATE | Last time the file was downloaded (updated) |

CometEls File
-------------

| Constant                 | Useage                                            |
|--------------------------|---------------------------------------------------|
| FILE_COMETELS_LOCATION   | File name and location of MPC Comet elements file |
| FILE_COMETELS_UPDATE     | Update frequency in days.                         |
| FILE_COMETELS_LASTUPDATE | Last time the file was downloaded (updated)       |
|                          |                                                   |
|                          |                                                   |
