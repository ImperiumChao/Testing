#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T21:06:07
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Testing
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    Widgets/table.cpp \
    main_tests.cpp \
    main_tracking.cpp \
    main_users.cpp \
    main_statistics.cpp \
    loginform.cpp \
    Widgets/fg.cpp \
    Tests/testsettings.cpp \
    adduser.cpp \
    Tests/test.cpp \
    Tests/edittest.cpp \
    Tests/training.cpp \
    Tests/answer.cpp \
    Tests/controltest.cpp \
    Widgets/querycontrol.cpp

HEADERS  += widget.h \
    Widgets/table.h \
    main_tests.h \
    main_tracking.h \
    main_users.h \
    main_statistics.h \
    loginform.h \
    Widgets/fg.h \
    Tests/testsettings.h \
    adduser.h \
    Tests/test.h \
    Tests/edittest.h \
    Tests/training.h \
    Tests/answer.h \
    Tests/controltest.h \
    Widgets/querycontrol.h

FORMS    += widget.ui \
    main_tests.ui \
    main_tracking.ui \
    main_users.ui \
    main_statistics.ui \
    loginform.ui \
    Widgets/fg.ui \
    Tests/testsettings.ui \
    adduser.ui \
    Tests/test.ui \
    Tests/edittest.ui \
    Tests/training.ui \
    Tests/controltest.ui \
    Widgets/querycontrol.ui
