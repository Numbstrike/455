#include "LoginDialog.h"
#include <QDialog>
#include <QSqlDatabase>
#include <QDebug>

//THIS DIALOG HANDLES QSqlDatabase CREATION AND CONNECTION.
//--------------------------------------
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    mainWin = parent;
    pwdLineEdit->setEchoMode(QLineEdit::Password);

    serverLayout->addWidget(serverLabel);
    serverLayout->addWidget(serverLineEdit);
    dbLayout->addWidget(dbLabel);
    dbLayout->addWidget(dbLineEdit);
    userLayout->addWidget(userLabel);
    userLayout->addWidget(userLineEdit);
    pwdLayout->addWidget(pwdLabel);
    pwdLayout->addWidget(pwdLineEdit);

    QPushButton *okayButton = new QPushButton("LEMME SEE THE DATA.");
    QVBoxLayout *mainVLayout = new QVBoxLayout();
    mainVLayout->addLayout(serverLayout);
    mainVLayout->addLayout(dbLayout);
    mainVLayout->addLayout(userLayout);
    mainVLayout->addLayout(pwdLayout);
    mainVLayout->addWidget(okayButton);

    connect(serverLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateServerText(QString)));
    connect(dbLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateDbText(QString)));
    connect(userLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateUserText(QString)));
    connect(pwdLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updatePwdText(QString)));
    connect(okayButton, SIGNAL(clicked(bool)), this, SLOT(setupDatabase()));

    this->setLayout(mainVLayout);
    this->show();
}

//--------------------------------------
LoginDialog::~LoginDialog()
{
    //Do I need to manually destroy anything?
}

//--------------------------------------
void LoginDialog::updateServerText(QString text)
{ serverText = text; }

//--------------------------------------
void LoginDialog::updateDbText(QString text)
{ dbText = text; }

//--------------------------------------
void LoginDialog::updateUserText(QString text)
{ userText = text; }

//--------------------------------------
void LoginDialog::updatePwdText(QString text)
{ pwdText = text; }


//--------------------------------------
void LoginDialog::setupDatabase()
{
    if (tryOpenDatabase())
    {
        this->close();
        mainWin->show();
    }

    else
    { qDebug() << "Try logging in again."; }
}


//--------------------------------------
bool LoginDialog::tryOpenDatabase()
{
     QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

     db.setHostName(serverText);
     db.setDatabaseName(dbText);
     db.setUserName(userText);
     db.setPassword(pwdText);

     db.open();

     if (!db.open())
     {
         qDebug() << "Database connection error occurred.";
         return false;
     }

     qDebug() << "Database opened okay.";
     return true;
}
