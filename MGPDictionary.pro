TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread
#LIBS += -L[/usr/local/boost_1_55_0] -l[boost]
INCLUDEPATH += "/usr/local/boost_1_55_0"
#INCLUDEPATH += "/usr/local/boost_1_55_0/boost"

SOURCES += main.cpp \
    unit-test/GTest_Hashing.cpp \
    unit-test/GTest_ChainingHashTable.cpp \
    gtest/gtest-all.cc \
    unit-test/GTest_Iterator_Pair.cpp \
    unit-test/GTest_Iterator_Key.cpp \
    unit-test/GTest_Iterator_Value.cpp

HEADERS += \
    Dictionary.hpp \
    ChainingHashTable.hpp \
    OpenAddressingHashTable.hpp \
    Hashing.hpp \
    ChainingHashTableIterator.hpp \
    gtest/gtest.h

OTHER_FILES += \
    README.md

