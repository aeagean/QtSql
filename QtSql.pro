CONFIG += c++11
CONFIG += staticlib
TEMPLATE = lib

unix:TARGET = $$PWD/lib/QtSql
win32: {
DESTDIR = $$PWD/lib/
TARGET = QtSql
}

include(QtSql.pri)

SOURCES += \
    src/SqlService.cpp

HEADERS += \
    include/SqlService.h
