CONFIG += c++11
CONFIG += staticlib
TEMPLATE = lib

TARGET = $$PWD/lib/QtSql

include(QtSql.pri)

SOURCES += \
    src/SqlService.cpp

HEADERS += \
    include/SqlService.h
