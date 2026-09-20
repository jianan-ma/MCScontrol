QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    logquery.cpp \
    main.cpp \
    mainform.cpp \
    statistics.cpp

HEADERS += \
    logquery.h \
    mainform.h \
    statistics.h

FORMS +=
INCLUDEPATH += $$PWD/UI/config \
            $$PWD/UI/custom \
            $$PWD/communication
include($$PWD/UI/config/config.pri)
include($$PWD/UI/custom/custom.pri)
include($$PWD/communication/communication.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
