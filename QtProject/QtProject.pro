#-------------------------------------------------
#
# Project created by QtCreator 2016-09-12T19:48:29
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtProject
TEMPLATE = app


SOURCES += main.cpp\
        LoginDialog.cpp \
        mainwindow.cpp

HEADERS  += LoginDialog.h \
    mainwindow.h

FORMS    += mainwindow.ui


macx: {
INCLUDEPATH += /usr/local/Cellar/mysql/5.7.10/include/mysql \
               /Users/foob/Qt/5.7/clang_64/plugins/sqldrivers
}

//macx: {
//LIBS += -L/usr/local/lib/mysql -lmysqld -lmysqlclient -v
//}
