TEMPLATE = subdirs

#CONFIG += ordered
SUBDIRS += "cfitsio"
SUBDIRS += "ACL"
SUBDIRS += "GCL"
#SUBDIRS += "../Library/LibRaw"
SUBDIRS += "libWCS"
SUBDIRS += "MCL"
SUBDIRS += "novasc3.1"
SUBDIRS += "PCL"
SUBDIRS += "SCL"
SUBDIRS += "SOFA"
SUBDIRS += "qwt-6.1.3/qwt.pro"
SUBDIRS += "Qxt"
SUBDIRS += "SBIG"
SUBDIRS += "../../Library/WCL"
SUBDIRS += "GeographicLib-1.48/GeographicLib.pro"
SUBDIRS += "astroManager"

DEFINES += QWT_STATIC
