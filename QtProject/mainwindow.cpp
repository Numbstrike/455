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
    ui->equippedTView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //Hide all item action choices initially, when no item is selected!
    ui->itemActionComboBox->setItemData(1, QSize(0,0), Qt::SizeHintRole);
    ui->itemActionComboBox->setItemData(2, QSize(0,0), Qt::SizeHintRole);
    ui->itemActionComboBox->setItemData(3, QSize(0,0), Qt::SizeHintRole);

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
    connect(ui->mainInvTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(pInvItemSelected(QModelIndex)));
    connect(ui->pInvTableView_buySell, SIGNAL(clicked(QModelIndex)), this, SLOT(pInvItemSelected_buySell(QModelIndex)));
    connect(ui->npcInvTableView_buySell, SIGNAL(clicked(QModelIndex)), this, SLOT(npcInvItemSelected_buySell(QModelIndex)));
    connect(ui->itemActionButton, SIGNAL(clicked()), this, SLOT(itemActionClicked()));
}

//-------------------------------------------------------------
void MainWindow::updatePInvTViewByKind(int itemKind)
/* Finding player inventory by item type.  Each type might have a slightly different displayed table (attribute-wise). */
{
    qDebug() << "MainWin::updatePInvTViewByType:  Current pName is" << pName;
    QSqlQuery getItems;
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
void MainWindow::pInvItemSelected(const QModelIndex &i)
{
    QModelIndex nameIndex = mainInvQueryModel->index(i.row(), 0, QModelIndex());
    pSelectedItem = mainInvQueryModel->data(nameIndex).toString();
    qDebug() << pSelectedItem;
    ui->pItemSelected_main->setText("Item Selected: "+pSelectedItem);

    QModelIndex kindIndex = mainInvQueryModel->index(i.row(), 1, QModelIndex());
    QString kind = mainInvQueryModel->data(kindIndex).toString();
    qDebug() << "Selected item kind: " << kind;

    QSize size = ui->itemActionButton->sizeHint();

  //Item actions are "<Select Action>", drop, equip, or use, indexed in that order.
    ui->itemActionComboBox->setCurrentIndex(0);         //Default to first value ("<Select Action>" so cannot use last selected action on invalid item.

    //All items can be dropped.
    ui->itemActionComboBox->setItemData(1, size, Qt::SizeHintRole);

    //Disable or enable equpping an item.
    if (kind!="weapon" && kind!="armor")
    { ui->itemActionComboBox->setItemData(2, QSize(0,0), Qt::SizeHintRole); }
    else
    { ui->itemActionComboBox->setItemData(2, size, Qt::SizeHintRole); }

    //Disable or enable using an item.
    if (kind!="usable")
    { ui->itemActionComboBox->setItemData(3, QSize(0,0), Qt::SizeHintRole); }
    else
    { ui->itemActionComboBox->setItemData(3, size, Qt::SizeHintRole); }
}

//-------------------------------------------------------------
void MainWindow::pInvItemSelected_buySell(const QModelIndex &i)
{
    QModelIndex nameIndex = mainInvQueryModel->index(i.row(), 0, QModelIndex());
    pSelectedItem = mainInvQueryModel->data(nameIndex).toString();
    ui->pItemSelected_buySell->setText("Item Selected: "+pSelectedItem);

    QModelIndex priceIndex = mainInvQueryModel->index(i.row(), 5, QModelIndex());
    QString price = mainInvQueryModel->data(priceIndex).toString();
    ui->pItemPrice_buySell->setText("Sell item for "+price+" coins?");
}

//-------------------------------------------------------------
void MainWindow::npcInvItemSelected_buySell(const QModelIndex &i)
{
    QModelIndex nameIndex = npcInvQueryModel->index(i.row(), 0, QModelIndex());
    npcSelectedItem = npcInvQueryModel->data(nameIndex).toString();
    ui->npcItemSelected_buySell->setText("Item Selected: "+npcSelectedItem);

    QModelIndex priceIndex = npcInvQueryModel->index(i.row(), 5, QModelIndex());
    QString price = npcInvQueryModel->data(priceIndex).toString();
    ui->npcItemPrice_buySell->setText("Buy item for "+price+" coins?");
}

//-------------------------------------------------------------
void MainWindow::itemActionClicked()
{
    QString action = ui->itemActionComboBox->currentText();

    qDebug() << "MainWin::itemActionClicked()" << action;
    if (action=="Drop Item")
    {
        //dropItem();
    }

    else if (action=="Equip Item")
    {

    }

    else if (action=="Use Item")
    { useItem(); }
}

//-------------------------------------------------------------
//void MainWindow::dropItem()
//{
//    QSqlQuery deleteItem;
//    deleteItem.prepare("call deleteOrUpdateInOWNED(:pName, :itemName, :kind, 1);");
//    deleteItem.bindValue(":itemName", pSelectedItem);
//    deleteItem.bindValue(":pName", pName);
//    deleteItem.exec();

//    updatePInvTViewByKind(ui->itemKindComboBox_pInv->currentIndex());
//}

//-------------------------------------------------------------
void MainWindow::useItem()
{
    QString statType;
    QString modifier;

    QSqlQuery itemInfo;
    itemInfo.prepare("select affects, modifier from USABLE where name=:itemName;");
    itemInfo.bindValue(":itemName", pSelectedItem);
    itemInfo.exec();
    while (itemInfo.next())
    {
        statType = itemInfo.value(0).toString();
        modifier = itemInfo.value(1).toString();
    }

    QSqlQuery useQuery;
    useQuery.prepare("call updateHealthStamina(:pName, :statType, :modifier);");
    useQuery.bindValue(":pName", pName);
    useQuery.bindValue(":statType",statType);
    useQuery.bindValue(":modifier", modifier);
    useQuery.exec();

    QSqlQuery deleteItem;
    deleteItem.prepare("call deleteOrUpdateInOWNED(:pName, :itemName, 'usable', 1);");        //TODO: Pay attention to quantity in GUI. Might be fine to use ONE item at a time, but might want to have a qty option in other cases.
    deleteItem.bindValue(":itemName", pSelectedItem);
    deleteItem.bindValue(":pName", pName);
    deleteItem.exec();

    setHealthStaminaBars();
    updatePInvTViewByKind(ui->itemKindComboBox_pInv->currentIndex());
}
