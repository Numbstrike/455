#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QDebug>
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>

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

    //ui->staminaProgBar->setFormat("%p%"); //Grr, text not appearing.

    connectSignalsSlots();

}


//-------------------------------------------------------------
MainWindow::~MainWindow()
/* Deconstructor */
{ delete ui; }


//-------------------------------------------------------------
//void MainWindow::setDatabase(QSqlDatabase database)
//{
//    qDebug() << "Database set in MainWindow.";
//    db = database;
//}


//-------------------------------------------------------------
void MainWindow::connectSignalsSlots()
{
    connect(ui->itemTypeComboBox_pInv, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePInvTableView(int)));
}


//-------------------------------------------------------------
QString MainWindow::queryPlayerName()
/* Retrieves current player name from DB.
    Practice with QSql for now, since this is probably not logical to do. */
{
    pName = "Aragorn";

    QString name;
 //   if (db.open())
 //   {
        QSqlQuery getPlayerName;                                                    //Do queries by default use the currently opened DB? I think so.
        getPlayerName.prepare("select name from PLAYER where name=:pName;");
        getPlayerName.bindValue(":pName", "Aragorn");

        if (getPlayerName.exec())
        {
            qDebug() << "MainWin::queryPlayerName: Player name retrieved:" << name;
            ui->playerNameLabel->setText("Player Name: "+name);
            //pName = name;                                             //Update class variable.
            updatePInvTableView(0);                            // Show all of player's items by default.
        }
        else
        { qDebug() << "Error. Player name not retrieved."; }
//   }
//   else
//    { qDebug() << "Database not open for MainWindow.queryPlayerName()."; }

    return name;
}


//-------------------------------------------------------------
void MainWindow::updatePInvTableView(int itemType)
{
    qDebug() << "MainWin::updatePInvTableView:  Current pName is" << pName;
    QSqlQuery getItems;
    QString str;

    if (itemType == 0)
    {   //MAKE SURE TO HAVE SPACES, SO QUERY EXECUTES NORMALLY.
        str = ";";
        qDebug() << "Displaying item type: ALL";
     }

    else if (itemType == 1)
    {
         str = "and itemType='weapon';";
        qDebug() << "Displaying item type: WEAPON";
    }

    else if (itemType == 2)
    {
         str = "and itemType='armor';";
        qDebug() << "Displaying item type: ARMOR";
    }

    else if (itemType == 3)
    {
        str = "and itemType='usable';";
        qDebug() << "Displaying item type: USABLE";
    }

    else if (itemType == 4)
    {
        str = "and itemType='misc';";
        qDebug() << "Displaying item type: MISC";
    }


    getItems.bindValue(":pName", pName);
    getItems.exec();
//        while (getItems.next())
//        {
//            qDebug() << getItems.value(0).toString();
//        }

    getItems.prepare("select itemName, weight, quantity, itemType "
                     "from OWNED join ITEM on itemName=name "
                     "where playerName=:pName "+str);

    qDebug() <<"select itemName, weight, quantity, itemType "
               "from OWNED join ITEM on itemName=name "
               "where playerName=:pName "+str;

    getItems.bindValue(":pName", "Aragorn");
    QSqlQueryModel *mainInvQueryModel = new QSqlQueryModel();      //QSqlQueryModel for DISPLAYING data, not for editing, unlike QSqlTableModel.
    mainInvQueryModel->setQuery(getItems);
    mainInvQueryModel->setHeaderData(0, Qt::Horizontal, "Item Name");
    mainInvQueryModel->setHeaderData(1, Qt::Horizontal, "Weight");
    mainInvQueryModel->setHeaderData(2, Qt::Horizontal, "Quantity");
    mainInvQueryModel->setHeaderData(3, Qt::Horizontal, "Item Type");

    ui->mainInvTableView->setModel(mainInvQueryModel);
    ui->mainInvTableView->show();

}
