#-------------------------------------------------
#
# Project created by QtCreator 2019-05-12T22:03:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = px4_gcs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qnode_test.cpp

HEADERS  += mainwindow.h \
    qnode_test.h

FORMS    += mainwindow.ui



INCLUDEPATH += \
                /opt/ros/kinetic/include\
                /usr/include\
                /home/jbs/catkin_ws/src/traj_gen/include

