TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread

SOURCES += main.cpp \
    gtest/gtest-all.cc \
    unit-test/GTest_ChainingHashTable.cpp \
    unit-test/GTest_OpenAddressingHashTable.cpp

HEADERS += \
    Dictionary.hpp \
    ChainingHashTable.hpp \
    OpenAddressingHashTable.hpp \
    gtest/gtest.h

