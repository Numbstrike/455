#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QString>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void setDatabase(QSqlDatabase database);
        QString queryPlayerName();


    private:
        QString accentColor;
        Ui::MainWindow *ui;
        QSqlDatabase db;       //Addresses (pointers) are never null, so don't try to initialize them to NULL.
        QString pName;
        void connectSignalsSlots();

        Q_SLOT void updatePInvTableView(int itemType);
};

#endif // MAINWINDOW_H
