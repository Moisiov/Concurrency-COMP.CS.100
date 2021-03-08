# COMP.CS.310 Concurrency Project 3 - spring 2021
#
TEMPLATE = app

CONFIG += cmdline c++17

# we use qt as build system, no qt libs:
CONFIG -= qt
QT -= core gui concurrent

# POSIX thread library (C++ threads need this in most platforms):
LIBS += -pthread

# Use course library (DO NOT CHANGE)
include( concur2021lib/concur2021.pri )

# Implement your own code here:
SOURCES += \
    main.cpp
