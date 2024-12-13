QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    departmentview.cpp \
    docterview.cpp \
    idatabase.cpp \
    loginview.cpp \
    main.cpp \
    masterview.cpp \
    paticenteditview.cpp \
    patientview.cpp \
    welcomeview.cpp

HEADERS += \
    departmentview.h \
    docterview.h \
    idatabase.h \
    loginview.h \
    masterview.h \
    paticenteditview.h \
    patientview.h \
    welcomeview.h

FORMS += \
    departmentview.ui \
    docterview.ui \
    loginview.ui \
    masterview.ui \
    paticenteditview.ui \
    patientview.ui \
    welcomeview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
