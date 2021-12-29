QT       += core gui
QT += serialport
QT += printsupport
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PenColor.cpp \
    dataplot.cpp \
    datatimer.cpp \
    main.cpp \
    mainwindow.cpp \
    mythread.cpp \
    plotingpanel.cpp \
    qcustomplot.cpp \
    serialhandler.cpp \
    sockethandler.cpp \
    uithread.cpp \
    var_processor.cpp \
    variables.cpp \
    variables_container.cpp \
    vartable.cpp

HEADERS += \
    PenColor.h \
    dataplot.h \
    datatimer.h \
    mainwindow.h \
    mythread.h \
    plotingpanel.h \
    qcustomplot.h \
    serialhandler.h \
    sockethandler.h \
    uithread.h \
    var_processor.h \
    variables.h \
    variables_container.h \
    vartable.h

FORMS += \
    mainwindow.ui \
    mainwindow_copy.ui \
    relationPlotdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    .clang-format \
    Plan.md
