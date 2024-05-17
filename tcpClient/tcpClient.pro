QT       += core gui network
CONFIG += utf8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    clientwin.ui \
    fileinfopage.ui \
    online.ui \
    privatechat.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    IconImg.qrc


include($$PWD/File/File.pri)
include($$PWD/Friend/Friend.pri)
include($$PWD/MainWindow/MainWindow.pri)
include($$PWD/ClientSocket/ClientSocket.pri)
include($$PWD/PDU/PDU.pri)
include($$PWD/View/View.pri)
include($$PWD/LoginAndRegist/LoginAndRegist.pri)
include($$PWD/ResponseHandler/ResponseHandler.pri)
include($$PWD/About/About.pri)

SOURCES += \
    commondata.cpp \
    commonfunc.cpp \
    main.cpp \
    mymain.cpp

HEADERS += \
    commondata.h \
    commonfunc.h \
    mymain.h \
    style.h
