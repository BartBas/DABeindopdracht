#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(QSqlDatabase:: addDatabase("QMYSQL"))
{
    ui->setupUi(this);
    qDebug()<< QSqlDatabase::drivers();

    db.setHostName("213.10.91.15");
    db.setPort(3306);
    db.setDatabaseName("fun4all");
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
    MainWindow::showListBrand();
}

void MainWindow::showListBrand()
{
    db.open();
    QSqlQuery query;
    if(query.exec("SELECT studentnr FROM student"))
    {
        while(query.next())
        {
            QString studentNr = query.value(0).toString(); // Assuming the result is in column 0

            // Add the item to the QListWidget
            QListWidgetItem *item = new QListWidgetItem(studentNr);
            ui->merk->addItem(item);
        }
    }
    else
    {
        // Handle the case where the query execution failed
        qDebug() << "Query failed:" << query.lastError().text();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

