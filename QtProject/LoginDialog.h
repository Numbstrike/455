#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class LoginDialog : public QDialog
{
    Q_OBJECT        //Q_OBJECT macro, to use Qt built-in stuff like setting up custom Q_SLOTs.

    public:
        explicit LoginDialog(QWidget *parent);
        ~LoginDialog();

    private:
        QWidget *mainWin;

        QHBoxLayout *serverLayout = new QHBoxLayout();
        QHBoxLayout *dbLayout = new QHBoxLayout();
        QHBoxLayout *userLayout = new QHBoxLayout();
        QHBoxLayout *pwdLayout = new QHBoxLayout();
        QLabel *serverLabel = new QLabel("Server Name:");
        QLabel *dbLabel = new QLabel("Database Name:");
        QLabel *userLabel = new QLabel("Username:");
        QLabel *pwdLabel = new QLabel("Password:");
        QLineEdit *serverLineEdit = new QLineEdit();
        QLineEdit *dbLineEdit = new QLineEdit();
        QLineEdit *userLineEdit = new QLineEdit();
        QLineEdit *pwdLineEdit = new QLineEdit();

        QString serverText;
        QString dbText;
        QString userText;
        QString pwdText;
        Q_SLOT void updateServerText(QString text);
        Q_SLOT void updateDbText(QString text);
        Q_SLOT void updateUserText(QString text);
        Q_SLOT void updatePwdText(QString text);
        Q_SLOT void setupDatabase();
        bool tryOpenDatabase();
};

#endif // LOGINDIALOG_H
