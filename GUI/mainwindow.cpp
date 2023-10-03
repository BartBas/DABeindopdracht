#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

