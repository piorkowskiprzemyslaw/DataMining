#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T15:30:39
#
#-------------------------------------------------

TARGET = DataMining
CONFIG -= app_bundle
CONFIG += c++11
# clang++ version 3.5 or 3.6 is preferred.
QMAKE_CXX = clang++-3.6
# As long as one have installed gcc >= 4.8 (I'm not sure about lower
# versions - didn't tested them) there is no need to change
# stdlib to libc++
QMAKE_CXXFLAGS += -Werror -Wall -Wextra -pedantic

SOURCES += main.cpp \
    Data/Data.cpp \
    Data/DataLoader.cpp \
    KNNClassifier/KNNClassifier.cpp \
    DFT/DFTReduction.cpp \
    MI/MIReduction.cpp \
    Logger.cpp \
    CHI/CHIReduction.cpp \
    Data/DataAdapter.cpp

HEADERS += \
    Data/Data.h \
    Data/DataLoader.h \
    KNNClassifier/KNNClassifier.h \
    DFT/DFTReduction.h \
    MI/MIReduction.h \
    Logger.h \
    CHI/CHIReduction.h \
    ParallelExecutor.h \
    AtomicHelper.h \
    Data/DataAdapter.h
