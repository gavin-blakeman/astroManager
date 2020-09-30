TEMPLATE = subdirs

#CONFIG += ordered
SUBDIRS += "cfitsio"
SUBDIRS += "ACL"
SUBDIRS += "astrometry"
SUBDIRS += "GCL"
#SUBDIRS += "LibRaw"
SUBDIRS += "libWCS"
SUBDIRS += "MCL"
#SUBDIRS += "NOVAS"
SUBDIRS += "PCL"
SUBDIRS += "QCL"
SUBDIRS += "SCL"
SUBDIRS += "SOFA"
SUBDIRS += "qwt-6.1.3/qwt.pro"
SUBDIRS += "Qxt"
SUBDIRS += "SBIG"
SUBDIRS += "WCL"
SUBDIRS += "GeographicLib-1.48/GeographicLib.pro"
SUBDIRS += "astroManager"

DEFINES += QWT_STATIC
