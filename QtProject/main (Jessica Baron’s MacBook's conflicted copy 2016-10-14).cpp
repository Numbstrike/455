#include "mainwindow.h"

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

bool createConnection(string hostname, string dbName, string username, string pswd, MainWindow *win);

int main(int argc, char *argv[])
{
    ifstream loginFile("/Users/jessicabaron/Dropbox/455Proj_DBLogin.txt");

    string hostname;
    string dbName;
    string username;
    string pswd;

    while (loginFile >> hostname >> dbName >> username >> pswd)     //Input being line by line of file.
    {
        cout << hostname << " " << dbName << " " << username << " " << pswd;
    }

//    cout << "Enter server hostname: \n";
//    getline(cin, hostname);
//    cout << "Enter database name: \n";
//    getline(cin, dbName);
//    cout << "Enter username: \n";
//    getline(cin, username);
//    cout << "Enter password: \n";
//    getline(cin, pswd);

    QApplication app(argc, argv);
    MainWindow win;
    if (!createConnection(hostname, dbName, username, pswd, &win))
    {
        qDebug() << "Not connected to database";
        return 1;
    }

    else
    {
        qDebug() << "Connected to database.";
    }

    win.show();

    qDebug() << "User's Player Name:" << win.queryPlayerName();

    return app.exec();
}



bool createConnection(string hostname, string dbName, string username, string pswd, MainWindow *win)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    const QString &hn = QString::fromStdString(hostname);
//    const QString &dbn = QString::fromStdString(dbName);
//    const QString &un = QString::fromStdString(username);
//    const QString &pw = QString::fromStdString(pswd);

//    db.setHostName(hn);
//    db.setDatabaseName(dbn);
//    db.setUserName(un);
//    db.setPassword(pw);

    db.setHostName("webdev.cislabs.uncw.edu");
    db.setDatabaseName("jrb1056");
    db.setUserName("jrb1056");
    db.setPassword("kih4AbDGS");

    db.open();

    if (!db.open())
    {
        qDebug() << "Database error occurred";
        return false;
    }

    qDebug() << "Database opened okay.";
    win->setDatabase(db);
    return true;
}
