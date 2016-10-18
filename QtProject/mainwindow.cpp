#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QDebug>
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMenu>

//-------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
/* Constructor, using the form Ui */
{
    ui->setupUi(this);

    this->setWindowTitle("RPG Inventory Manager");

    accentColor = "rgb(35,125,130)";
    QString grayGradient = "QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(75,75,80), stop: 1 rgb(100, 110, 110))";
    QString accentGradient = "QLinearGradient(x1: 0, y1:0, x2: 0, y2: 1, stop: 0"+accentColor+", stop: 1 rgb(100,110,100))";
    setStyleSheet("QWidget{background-color: "+grayGradient+"; color: white; font: 12pt 'Arial';}"
                  "QComboBox{background-color: "+accentColor+";}"
                  "QTableView{alternate-background-color: gray; background: "+accentColor+"; gridline-color: black;}"
                  "QTableView QHeaderView::section {background: "+accentColor+";}"
                  "QTableView QTableCornerButton::section {background: black;}"

                  "QTabBar{color: black; background-color: rgb(70,70,90);}");

                  //"QProgressBar{background-color: rgb(120,120,140);}");
                  //"QTabWidget{background-color: rgb(70,70,90);}");

    //ui->staminaProgBar->setFormat("%p%"); //Grr, text not appearing on Mac. Okay on Windows?

    pName = "Shady Old Man";
    ui->playerNameLabel->setText("Player Name: "+pName);
    connectSignalsSlots();
}


//-------------------------------------------------------------
MainWindow::~MainWindow()
/* Deconstructor */
{
    delete ui;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");          //Destroy connection so can launch process again in the same QtCreator session.
}                                                                       // (This was an issue on Mac.)


//-------------------------------------------------------------
void MainWindow::connectSignalsSlots()
{
    connect(ui->itemTypeComboBox_pInv, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePInvTViewByType(int)));
    //connect(ui->mainInvTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(showTableViewMenu(QModelIndex)));
}


//-------------------------------------------------------------
//QString MainWindow::queryPlayerName()
///* Retrieves current player name from DB.
//    Practice with QSql for now, since this is probably not logical to do. */
//{
//    pName = "Pooky the Bear";

//    QString name;

//        QSqlQuery getPlayerName;                                                    //Do queries by default use the currently opened DB? I think so.
//        getPlayerName.prepare("select name from PLAYER where name=:pName;");
//        getPlayerName.bindValue(":pName", "Pooky the Bear");

//        if (getPlayerName.exec())
//        {
//            qDebug() << "MainWin::queryPlayerName: Player name retrieved:" << name;
//            ui->playerNameLabel->setText("Player Name: "+name);
//            //pName = name;                                             //Update class variable.
//            updatePInvTViewByType(0);                            // Show all of player's items by default.
//        }
//        else
//        { qDebug() << "Error. Player name not retrieved."; }

//    return name;
//}


//-------------------------------------------------------------
void MainWindow::updatePInvTViewByType(int itemType)
/* Finding player inventory by item type.  Each type might have a slightly different displayed table (attribute-wise). */
{
    qDebug() << "MainWin::updatePInvTViewByType:  Current pName is" << pName;
    QSqlQuery getItems;
    QSqlQueryModel *mainInvQueryModel = new QSqlQueryModel();      //QSqlQueryModel for DISPLAYING data, not for editing, unlike QSqlTableModel.

    QSqlQuery getWeapons;
    QSqlQuery getArmor;
    QSqlQuery getUsables;
    QSqlQuery getMisc;

    getWeapons.prepare(
                "create temporary table OWNEDWEAPONS as "
                "select  itemName, itemType, weight, quantity, weight*quantity as totalWeight, degradation*baseValue as value "
                "from OWNED join ITEM join WEAPON on itemName=ITEM.name and ITEM.name=WEAPON.name "
                "where playerName=:pName and itemType='weapon';");
    getWeapons.bindValue(":pName", pName);
    getWeapons.exec();

    getArmor.prepare(
                "create temporary table OWNEDARMOR as "
                "select  itemName, itemType, weight, quantity, weight*quantity as totalWeight, degradation*baseValue as value "
                "from OWNED join ITEM join ARMOR on itemName=ITEM.name and ITEM.name=ARMOR.name "
                "where playerName=:pName and itemType='armor';");
    getArmor.bindValue(":pName", pName);
    getArmor.exec();

    getUsables.prepare(
                     "create temporary table OWNEDUSABLES as "
                     "select itemName, itemType, weight, quantity, weight*quantity as totalWeight, baseValue as value "
                     "from OWNED join ITEM on itemName=name "
                     "where playerName=:pName and itemType='usable';");
    getUsables.bindValue(":pName", pName);
    getUsables.exec();

    getMisc.prepare(
                     "create temporary table OWNEDMISC as "
                     "select itemName, itemType, weight, quantity, weight*quantity as totalWeight, baseValue as value "
                     "from OWNED join ITEM on itemName=name "
                     "where playerName=:pName and itemType='misc';");

    getMisc.bindValue(":pName", pName);
    getMisc.exec();

    //MAKE SURE TO HAVE SPACES, SO QUERY EXECUTES NORMALLY.
    if (itemType == 0)
    {
        qDebug() << "Displaying item type: ALL";

//        getItems.prepare("select itemName, itemType, weight, quantity, weight*quantity as totalWeight, baseValue "
//                         "from OWNED join ITEM on itemName=name "
//                         "where playerName=:pName;");
        getItems.prepare(
                    "select * from OWNEDWEAPONS "
                    "union "
                    "select * from OWNEDARMOR "
                    "union "
                    "select * from OWNEDUSABLES "
                    "union "
                    "select * from OWNEDMISC;");
     }


    //WEAPON Items: has degradation (affects value. Get from WEAPON table.)
    else if (itemType == 1)
    {
        qDebug() << "Displaying item type: WEAPON";
        getItems.prepare("select * from OWNEDWEAPONS;");

//         getItems.prepare(
//                     "select  itemName, itemType, weight, quantity, weight*quantity as totalWeight, degradation*baseValue as value "
//                     "from OWNED join ITEM join WEAPON on itemName=ITEM.name and ITEM.name=WEAPON.name "
//                     "where playerName=:pName and itemType='weapon';");

//         getItems.bindValue(":pName", pName);
//         getItems.exec();

//         mainInvQueryModel->setQuery(getItems);
//         mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
//         mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
//         mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
//         mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
//         mainInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
//         mainInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");
    }


    //ARMOR Items: has degradation
    else if (itemType == 2)
    {
         qDebug() << "Displaying item type: ARMOR";
         getItems.prepare("select * from OWNEDARMOR;");

//         getItems.prepare(
//                     "select  itemName, itemType, weight, quantity, weight*quantity as totalWeight, degradation*baseValue as value "
//                     "from OWNED join ITEM join ARMOR on itemName=ITEM.name and ITEM.name=ARMOR.name "
//                     "where playerName=:pName and itemType='armor';");

//         getItems.bindValue(":pName", pName);
//         getItems.exec();

//         mainInvQueryModel->setQuery(getItems);
//         mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
//         mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
//         mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
//         mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
//         mainInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
//         mainInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");
    }


    //USABLE Items: No degradation
    else if (itemType == 3)
    {
        qDebug() << "Displaying item type: USABLE";
        getItems.prepare("select * from OWNEDUSABLES;");

//        getItems.prepare("select itemName, itemType, weight, quantity, weight*quantity as totalWeight, baseValue "
//                         "from OWNED join ITEM on itemName=name "
//                         "where playerName=:pName and itemType='usable';");

//        getItems.bindValue(":pName", pName);
//        getItems.exec();

//        mainInvQueryModel->setQuery(getItems);
//        mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
//        mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
//        mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
//        mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
//        mainInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
//        mainInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");
    }


    //MISC Items: No degradation
    else if (itemType == 4)
    {
        qDebug() << "Displaying item type: MISC";
        getItems.prepare("select * from OWNEDMISC;");

//        getItems.prepare("select itemName, itemType, weight, quantity, weight*quantity as totalWeight, baseValue "
//                         "from OWNED join ITEM on itemName=name "
//                         "where playerName=:pName and itemType='misc';");

//        getItems.bindValue(":pName", pName);
//        getItems.exec();

//        mainInvQueryModel->setQuery(getItems);
//        mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
//        mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
//        mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
//        mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
//        mainInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
//        mainInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");
    }


    getItems.bindValue(":pName", pName);
    getItems.exec();

    mainInvQueryModel->setQuery(getItems);
    mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
    mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
    mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
    mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
    mainInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
    mainInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");

    ui->mainInvTableView->setModel(mainInvQueryModel);
    ui->mainInvTableView->show();
}


//-------------------------------------------------------------
//void MainWindow::showTableViewMenu(QModelIndex rowNum)
//{
//    //QMenu *menu = new QMenu(ui->mainInvTableView);
//    //QMenu->show();
//}
