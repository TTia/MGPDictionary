TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread
INCLUDEPATH += "/usr/local/boost_1_55_0"

SOURCES += main.cpp \
    gtest/gtest-all.cc \
    gtest/close_addressing/GTest_ChainingHashTable.cpp \
    gtest/close_addressing/GTest_ChainingHashTable_Rehashing.cpp \
    gtest/close_addressing/GTest_Hashing.cpp \
    gtest/close_addressing/GTest_Iterator_Key.cpp \
    gtest/close_addressing/GTest_Iterator_Pair.cpp \
    gtest/close_addressing/GTest_Iterator_Value.cpp \
    gtest/open_addressing/GTest_Probing.cpp \
    gtest/open_addressing/GTest_ProbingHashTable.cpp

HEADERS += \
    Hashing.hpp \
    gtest/gtest.h \
    close_addressing/ChainingHashTable.hpp \
    close_addressing/ChainingHashTableIterator.hpp \
    open_addressing/ProbingHashTable.hpp \
    open_addressing/Probing.hpp \
    open_addressing/ProbingExceptions.hpp

OTHER_FILES += \
    README.md

