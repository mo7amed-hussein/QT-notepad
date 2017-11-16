
QT       += core gui widgets printsupport
#greaterThan(QT_MAJOR_VERSION,4): QT += widgets

TARGET = notepad
TEMPLATE = app

DESTDIR = ./build

INCLUDEPATH += ./include \

#DEPENDPATH=./include

OBJECTS_DIR= ./obj
MOC_DIR = ./moc 



SOURCES += \ 
    src/main.cpp \ 
    src/mainwindow.cpp





HEADERS += \ 
    include/mainwindow.h

