#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(QSqlDatabase::addDatabase("QMYSQL"))
{
    ui->setupUi(this);
    qDebug()<< QSqlDatabase::drivers();

    db.setHostName("213.10.91.15");
    db.setPort(3306);
    db.setDatabaseName("eindopdracht");
    db.setUserName("testacc") ;
    db.setPassword("help123!");

    if(db.open())
    {
        QMessageBox::information(this, "Message", "Connection established");
        qDebug() << db.lastError().text();
    }
    else{
        QMessageBox::information(this, "Error", "No connection established");
        qDebug() << db.lastError().text();
    }
    db.close();
    MainWindow::listBrand();
    MainWindow::listBrandDetails("Philips");
    MainWindow::listModel("Philips");
    MainWindow::brandLogo("Philips");
}

void MainWindow::listBrand()
{
    db.open();
    QSqlQuery query;
    if(!query.exec("SELECT strMerk FROM tblMerk"))
    {
        // Handle the case where the query execution failed
        qDebug() << "Query failed:" << query.lastError().text();
        QListWidgetItem *failed = new QListWidgetItem("Query failed");
        ui->merk->addItem(failed);
        return;
    }
    while(query.next())
    {
        QString brand = query.value(0).toString(); // Assuming the result is in column 0

        // Add the item to the QListWidget
        QListWidgetItem *item = new QListWidgetItem(brand);
        ui->merk->addItem(item);
    }
    db.close();
}

void MainWindow::listBrandDetails(std::string brandChosen)
{
    db.open();
    QSqlQuery query;
    query.prepare("SELECT tblstad.strStad, tblland.strLand "
                  "FROM tblmerk "
                  "JOIN tbllocatie ON tblmerk.LocatieID = tbllocatie.ID "
                  "JOIN tblstad ON tblLocatie.stadID = tblstad.ID "
                  "JOIN tblland ON tblLocatie.landID = tblland.ID "
                  "WHERE tblmerk.strMerk = :brandChosen ");    //basic MySQL query for finding out the city and country of the brand
    query.bindValue(":brandChosen", QString::fromStdString(brandChosen)); //bind brand to the were statement in the query
    if(!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError().text();
        QListWidgetItem *failed = new QListWidgetItem("Query failed");
        ui->merkInfo->addItem(failed);
        return;
    }
    while(query.next())
    {
        QString stad = query.value(0).toString(); //get stad from the query
        QString land = query.value(1).toString(); //get land from the query

        QString brandDetails = stad + ", " + land; //combine stad and land

        QListWidgetItem *item = new QListWidgetItem(brandDetails);
        ui->merkInfo->addItem(item);
    }
    db.close();
}

void MainWindow::listModel(std::string brand)
{
    db.open();
    QSqlQuery query;
    query.prepare("SELECT tblModel.strType, tblUitvoering.strUitvoering FROM tblAuto "
                  "JOIN tblModel ON tblAuto.TypeID = tblModel.ID "
                  "JOIN tblUitvoering ON tblModel.UitvoeringID = tblUitvoering.ID "
                  "JOIN tblMerk ON tblAuto.MerkID = tblMerk.ID "
                  "WHERE tblMerk.Merk = :brand ");
    query.bindValue(":brand", QString::fromStdString(brand));
    if(!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError().text();
        QListWidgetItem *failed = new QListWidgetItem("Query failed");
        ui->model->addItem(failed);
        return;
    }
    while(query.next())
    {
        QString type = query.value(0).toString(); //get type from the query
        QString uitvoering = query.value(1).toString(); //get uitvoering from the query

        QString model = type + " " + uitvoering; //combine type and uitvoering for the model

        QListWidgetItem *item = new QListWidgetItem(model);
        ui->model->addItem(item);
    }
    db.close();
}
/*
void MainWindow::listModelDetails(std::string model)
{
    db.open();
    QSqlQuery query;
    db.close();
}
*/

void MainWindow::brandLogo(std::string brandChosen)
{
    db.open();
    QSqlQuery query;
    //query.prepare("SELECT Plaatje FROM tblModel WHERE ID = 1"); //code used to test
    query.prepare("SELECT blob FROM tblMerk "
                  "WHERE merk = :chosen");
    query.bindValue(":chosen", QString::fromStdString(brandChosen));
    if(query.exec() && query.next())
    {
        QByteArray blobData = query.value(0).toByteArray();

        QImageReader imageReader;
        imageReader.setDevice(new QBuffer(&blobData));
        QImage image = imageReader.read();

        if(image.isNull())
        {
            qDebug() << "Failed to load image from blob";
            return;
        }
        ui->logo->setPixmap(QPixmap::fromImage(image));
        ui->logo->setAlignment(Qt::AlignCenter);
    }
    else
    {
        qDebug() << "Query failed";
    }

    db.close();
}
MainWindow::~MainWindow()
{
    delete ui;
}


