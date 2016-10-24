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
        void startDisplays();
        //Q_SLOT void showTableViewMenu(QModelIndex rowNum);

    private:
        QString accentColor;
        Ui::MainWindow *ui;
        QSqlDatabase db;       //Addresses (pointers) are never null, so don't try to initialize them to NULL.
        QString pName;
        QString npcName;
        void setHealthStaminaBars();
        void connectSignalsSlots();
        void createPlayerTempTables();
        void createNpcTempTables();
        Q_SLOT void updatePInvTViewByKind(int itemKind=0);
        Q_SLOT void updateNpcInvTViewByKind(int itemKind=0);
};

#endif // MAINWINDOW_H
