TEMPLATE = subdirs

#CONFIG += ordered
SUBDIRS += "../../Library/cfitsio"
SUBDIRS += "../../Library/ACL/ACL.pro"
SUBDIRS += "../../Library/GCL/GCL.pro"
#SUBDIRS += "../Library/LibRaw"
SUBDIRS += "../../Library/libWCS"
SUBDIRS += "../../Library/MCL/MCL.pro"
SUBDIRS += "../../Library/NOVAS/novasc3.1"
SUBDIRS += "../../Library/PCL/PCL.pro"
SUBDIRS += "../../Library/SCL/SCL.pro"
SUBDIRS += "../../Library/SOFA"
SUBDIRS += "../../Library/qwt/qwt-6.1.3/qwt.pro"
SUBDIRS += "../../Library/Qxt"
SUBDIRS += "../../Library/SBIG"
SUBDIRS += "../../Library/WCL"
SUBDIRS += "../../Library/GeographicLib/GeographicLib-1.48/GeographicLib.pro"
SUBDIRS += "AstroManager"

DEFINES += QWT_STATIC
