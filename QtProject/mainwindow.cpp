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
    npcName = "Shady Old Man";
    pName = "Aragorn";

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

    ui->pInvTableView_buySell->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->npcInvTableView_buySell->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->mainInvTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connectSignalsSlots();
}

//-------------------------------------------------------------
MainWindow::~MainWindow()
/* Deconstructor */
{
    delete ui;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");          //Destroy connection so can launch process again in the same QtCreator session.
                                                                        // (This was an issue on Mac.)
    QSqlDatabase::removeDatabase("QMYSQL");
}

//-------------------------------------------------------------
void MainWindow::startDisplays()
/* Called from LoginDialog when database successfully connected. */
{
    ui->playerNameLabel->setText("Player Name: "+pName);
    ui->pNameLabel_buySell->setText("Player Name: "+pName);
    ui->npcNameLabel_buySell->setText("NPC Name: "+npcName);

    setHealthStaminaBars();
    setCoinsAndWeights();
    updateNpcInvTViewByKind(0);
    updatePInvTViewByKind(0);
}

//-------------------------------------------------------------
void MainWindow::setHealthStaminaBars()
{
    int h;
    int s;
    QSqlQuery query;
    query.prepare("select health, stamina from healthStaminaView where name=:pName;");
    query.bindValue(":pName", pName);
    query.exec();

    while (query.next())
    {
         h = query.value(0).toInt();            // .value() returns attrib. name at the given index.
         s = query.value(1).toInt();
    }

    ui->healthProgBar->setValue(h);
    ui->staminaProgBar->setValue(s);
}

//-------------------------------------------------------------
void MainWindow::setCoinsAndWeights()
{
    QString pCoins;
    QString pWeight;
    QSqlQuery coinQuery;
    coinQuery.prepare("select coins, totalWeight from PLAYER where name=:pName;");
    coinQuery.bindValue(":pName", pName);
    coinQuery.exec();

    while (coinQuery.next())
    {
        pCoins = coinQuery.value(0).toString();
        pWeight = coinQuery.value(1).toString();
    }

    ui->coinsLabel->setText("Coins: "+pCoins);
    ui->pCoinsLabel_buySell->setText("Coins: "+pCoins);
    ui->weightLabel->setText("Weight Carried: "+pWeight);
    ui->pWeightLabel_buySell->setText("Weight Carried: "+pWeight);


    QSqlQuery npcCoinQuery;
    npcCoinQuery.prepare("select coins, totalWeight from PLAYER where name=:npcName;");
    npcCoinQuery.bindValue(":npcName", npcName);
    npcCoinQuery.exec();

    while (npcCoinQuery.next())
    {
        ui->npcCoinsLabel_buySell->setText("Coins: "+npcCoinQuery.value(0).toString());
        ui->npcWeightLabel_buySell->setText("Weight Carried: "+npcCoinQuery.value(1).toString());
    }
}

//-------------------------------------------------------------
void MainWindow::connectSignalsSlots()
{
    connect(ui->itemKindComboBox_pInv, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePInvTViewByKind(int)));
    connect(ui->mainTabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(updatePInvTViewByKind()));
    //connect(ui->mainInvTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(showTableViewMenu(QModelIndex)));
}

//-------------------------------------------------------------
void MainWindow::updatePInvTViewByKind(int itemKind)
/* Finding player inventory by item type.  Each type might have a slightly different displayed table (attribute-wise). */
{
    qDebug() << "MainWin::updatePInvTViewByType:  Current pName is" << pName;
    QSqlQuery getItems;
    QSqlQueryModel *mainInvQueryModel = new QSqlQueryModel();      //QSqlQueryModel for DISPLAYING data, not for editing, unlike QSqlTableModel.
    createPlayerTempTables();

    //MAKE SURE TO HAVE SPACES, SO QUERY EXECUTES NORMALLY.
    if (itemKind == 0)
    {
        qDebug() << "Displaying item type: ALL";
        getItems.prepare("select * from P_OWNEDALLITEMS;");
     }

    else if (itemKind == 1)
    {
        qDebug() << "Displaying item type: WEAPON";
        getItems.prepare("select * from P_OWNEDWEAPONS;");
    }

    else if (itemKind == 2)
    {
         qDebug() << "Displaying item type: ARMOR";
         getItems.prepare("select * from P_OWNEDARMOR;");
    }

    else if (itemKind == 3)
    {
        qDebug() << "Displaying item type: USABLE";
        getItems.prepare("select * from P_OWNEDUSABLES;");
    }

    else if (itemKind == 4)
    {
        qDebug() << "Displaying item type: MISC";
        getItems.prepare("select * from P_OWNEDMISC;");
    }

    getItems.exec();
    mainInvQueryModel->setQuery(getItems);
    mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
    mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
    mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
    mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
    mainInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
    mainInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");

    ui->mainInvTableView->setModel(mainInvQueryModel);
    ui->pInvTableView_buySell->setModel(mainInvQueryModel);
    ui->pInvTableView_buySell->show();
    ui->mainInvTableView->show();               //Allows inv tab to be the default one viewed.


    QSqlQueryModel *equippedModel = new QSqlQueryModel();
    QSqlQuery equippedQuery;
    equippedQuery.prepare("select itemName, itemKind, bodyRegion from EQUIPPED "
                          "where playerName=:pName; ");
    equippedQuery.bindValue(":pName", pName);
    equippedQuery.exec();
    equippedModel->setQuery(equippedQuery);
    equippedModel->setHeaderData(0, Qt::Horizontal, "Name");
    equippedModel->setHeaderData(1, Qt::Horizontal, "Kind");
    equippedModel->setHeaderData(2, Qt::Horizontal, "Body Region");

    ui->equippedTView->setModel(equippedModel);
    ui->equippedTView->show();
}

//-------------------------------------------------------------
void MainWindow::updateNpcInvTViewByKind(int itemKind)
{
    qDebug() << "MainWin::updatePInvTViewByType:  Current npcName is" << npcName;
    QSqlQuery getItems;
    QSqlQueryModel *npcInvQueryModel = new QSqlQueryModel();      //QSqlQueryModel for DISPLAYING data, not for editing, unlike QSqlTableModel.
    createNpcTempTables();

    if (itemKind == 0)
    {
        qDebug() << "Displaying item type: ALL";
        getItems.prepare("select * from NPC_OWNEDALLITEMS;");
     }

    else if (itemKind == 1)
    {
        qDebug() << "Displaying item type: WEAPON";
        getItems.prepare("select * from NPC_OWNEDWEAPONS;");
    }

    else if (itemKind == 2)
    {
         qDebug() << "Displaying item type: ARMOR";
         getItems.prepare("select * from NPC_OWNEDARMOR;");
    }

    else if (itemKind == 3)
    {
        qDebug() << "Displaying item type: USABLE";
        getItems.prepare("select * from NPC_OWNEDUSABLES;");
    }

    else if (itemKind == 4)
    {
        qDebug() << "Displaying item type: MISC";
        getItems.prepare("select * from NPC_OWNEDMISC;");
    }

    getItems.exec();

    npcInvQueryModel->setQuery(getItems);
    npcInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
    npcInvQueryModel->setHeaderData(1, Qt::Horizontal, "Item Type");
    npcInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
    npcInvQueryModel->setHeaderData(3, Qt::Horizontal, "Weight");
    npcInvQueryModel->setHeaderData(4, Qt::Horizontal, "Total Weight");
    npcInvQueryModel->setHeaderData(5, Qt::Horizontal, "Value");

    ui->npcInvTableView_buySell->setModel(npcInvQueryModel);
}

//-------------------------------------------------------------
void MainWindow::createPlayerTempTables()
{
    QSqlQuery callTablesProc;
    callTablesProc.prepare("call updatePlayerTempTables(:pName);");
    callTablesProc.bindValue(":pName", pName);
    callTablesProc.exec();
}

//-------------------------------------------------------------
void MainWindow::createNpcTempTables()
{
    QSqlQuery callTablesProc;
    callTablesProc.prepare("call updateNpcTempTables(:npcName);");
    callTablesProc.bindValue(":npcName", npcName);
    callTablesProc.exec();
}

//-------------------------------------------------------------
//void MainWindow::showTableViewMenu(QModelIndex rowNum)
//{
//    //QMenu *menu = new QMenu(ui->mainInvTableView);
//    //QMenu->show();
//}
