#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T18:19:52
#
#-------------------------------------------------

QT += concurrent core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mdichild.cpp \
    CBufor.cpp \
    CAlgorithmPanel.cpp \
    COpenCV.cpp

HEADERS  += mainwindow.h \
    mdichild.h \
    CBufor.h \
    CAlgorithmPanel.h \
    COpenCV.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab

