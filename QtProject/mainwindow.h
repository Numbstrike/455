#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QString>
#include <QSqlQueryModel>

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

    private:
        QString accentColor;
        Ui::MainWindow *ui;
        QSqlDatabase db;       //Addresses (pointers) are never null, so don't try to initialize them to NULL.

        QString pName;
        QString npcName;
        QSqlQueryModel *mainInvQueryModel = new QSqlQueryModel();      //QSqlQueryModel for DISPLAYING data, not for editing, unlike QSqlTableModel.
        QSqlQueryModel *npcInvQueryModel = new QSqlQueryModel();
        QString pSelectedItem;
        QString npcSelectedItem;
        QString pSelectedItemKind;
        QString npcSelectedItemKind;
        QString pSelectedItemValue;
        QString npcSelectedItemValue;

        void setHealthStaminaBars();
        void setCoinsAndWeights();
        void connectSignalsSlots();
        void createPlayerTempTables();
        void createNpcTempTables();
        void useItem();
        Q_SLOT void updatePInvTViewByKind(int itemKind=0);
        Q_SLOT void updateNpcInvTViewByKind(int itemKind=0);
        Q_SLOT void pInvItemSelected(const QModelIndex &i);
        Q_SLOT void pInvItemSelected_buySell(const QModelIndex &i);
        Q_SLOT void npcInvItemSelected_buySell(const QModelIndex &i);
        Q_SLOT void itemActionClicked();
        Q_SLOT void playerSelling();
        Q_SLOT void playerBuying();
};

#endif // MAINWINDOW_H
