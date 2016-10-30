//RPG Inventory Manager
//Jessica Baron     UNCW CSC 455     Fall 2016

#include "mainwindow.h"
#include "LoginDialog.h"

#include <iostream>
#include <fstream>
#include <string>

#include <QApplication>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

using namespace std;    //For strings

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow *win = new MainWindow();
    LoginDialog *dia = new LoginDialog(win);            //Needs to be a pointer, else compiler yells at me.

    return app.exec();
}


