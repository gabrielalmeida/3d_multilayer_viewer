#-------------------------------------------------
#
# Project created by QtCreator 2013-09-23T23:15:28
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3d_multilayer_viewer
TEMPLATE = app

linux {
    LIBS += -lglut  -lGL -lGLU
}

SOURCES += main.cpp\
    mainwindow.cpp \
    glwidget.cpp \
    vector3d.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    vector3d.h \
    layer.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

