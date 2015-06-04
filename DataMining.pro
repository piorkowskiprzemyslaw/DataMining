#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T15:30:39
#
#-------------------------------------------------

TARGET = DataMining
CONFIG -= app_bundle
CONFIG += c++11

SOURCES += main.cpp \
    Data/Data.cpp \
    Data/DataLoader.cpp \
    KNNClassifier/KNNClassifier.cpp \
    DFT/DFTReduction.cpp \
    MI/MIReduction.cpp \
    Logger.cpp

HEADERS += \
    Data/Data.h \
    Data/DataLoader.h \
    KNNClassifier/KNNClassifier.h \
    DFT/DFTReduction.h \
    MI/MIReduction.h \
    Logger.h
