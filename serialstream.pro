######################################################################
# QextSerialPort Test Application (QESPTA)
######################################################################


PROJECT = serialstream
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH +=


OBJECTS_DIR    = obj
MOC_DIR        = moc
UI_DIR         = uic
CONFIG      += qt thread warn_on
CONFIG	    -= gui


HEADERS += qextserialport.h

SOURCES += main.cpp

LIBS          += -lqextserialport

unix:DEFINES   = _TTY_POSIX_
win32:DEFINES  = _TTY_WIN_ QWT_DLL QT_DLL
