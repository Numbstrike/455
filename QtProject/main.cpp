#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <QApplication>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

using namespace std;

bool createConnection();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!createConnection())
    {
        qDebug() << "Not connected to database";
        return 1;
    }

    else
    {
        qDebug() << "Connected to database.";

    }

    MainWindow win;
    win.show();

    return app.exec();
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("webdev.cislabs.uncw.edu");
    db.setDatabaseName("jrb1056");
    db.setUserName("jrb1056");
    db.setPassword("kih4AbDGS");

    if (!db.open())
    {
        qDebug() << "Database error occurred";
        return false;
    }
    return true;
}



//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
//}
