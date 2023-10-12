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
    //connect lists and buttons to functions
    connect(ui->merk, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(onBrandClicked(QListWidgetItem*)));
    connect(ui->model, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(onModelClicked(QListWidgetItem*)));
    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(radioButton()));
    connect(ui->actionBrand, &QAction::triggered, this, &MainWindow::addBrand);
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
    ui->merkInfo->clear();
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
    query.prepare("SELECT tbltype.strType, tblUitvoering.strUitvoering FROM tblAuto "
                  "JOIN tblModel ON tblAuto.TypeID = tblModel.ID "
                  "JOIN tblType ON tblModel.TypeID = tblType.ID "
                  "LEFT JOIN tblUitvoering ON tblModel.UitvoeringID = tblUitvoering.ID "
                  "JOIN tblMerk ON tblAuto.MerkID = tblMerk.ID "
                  "WHERE tblMerk.strMerk = :brand ");
    query.bindValue(":brand", QString::fromStdString(brand));
    if(!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError().text();
        QListWidgetItem *failed = new QListWidgetItem("Query failed");
        ui->model->addItem(failed);
        return;
    }
    ui->model->clear();
    while(query.next())
    {
        QString type = query.value(0).toString(); //get type from the query
        QString uitvoering = query.value(1).toString(); //get uitvoering from the query


        QString model = type + " ";

        if(uitvoering != "")
        {
            model = model + uitvoering; //combine type and uitvoering for the model
        }

        QListWidgetItem *item = new QListWidgetItem(model);

        ui->model->addItem(item);
    }
    db.close();
}

void MainWindow::listModelDetails(std::string model)
{
    db.open();
    QSqlQuery query;
    query.prepare("SELECT tblModel.Vermogen FROM tblModel "
                  "JOIN tblType ON tblModel.TypeID = tblType.ID "
                  "LEFT JOIN tblUitvoering ON tblModel.UitvoeringID = tblUitvoering.ID "
                  "WHERE CONCAT(tblType.strType, IF(tblUitvoering.strUitvoering IS NOT NULL, CONCAT(\" \", tblUitvoering.strUitvoering), \"\")) =  :modelChosen ");
    query.bindValue(":modelChosen", QString::fromStdString(model));
    if(!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError().text();
        QListWidgetItem *failed = new QListWidgetItem("Query failed");
        ui->modelDetails->addItem(failed);
        return;
    }
    ui->modelDetails->clear();
    QString KW = query.value(0).toString(); //get PK from query
    KWFloat = KW.toFloat();

    PKCalculated = round(KWFloat*(1.3625));

    QString vermogen = "KW: " + KW; //combine PK and KW with context for reader

    QListWidgetItem *item = new QListWidgetItem(vermogen);
    ui->modelDetails->addItem(item);

    db.close();
}

void MainWindow::radioButton()
{
    ui->modelDetails->clear();
    qDebug() << "radioButton Called";
    if(radioState)
    {
        QString vermogen = "PK: " + QString::number(PKCalculated);

        QListWidgetItem *item = new QListWidgetItem(vermogen);
        ui->modelDetails->addItem(item);
        radioState = false;
    }
    else
    {
        QString vermogen = "PK: " + QString::number(KWFloat);

        QListWidgetItem *item = new QListWidgetItem(vermogen);
        ui->modelDetails->addItem(item);
        radioState = true;
    }
}

void MainWindow::brandLogo(std::string brandChosen)
{
    db.open();
    QSqlQuery query;
    //query.prepare("SELECT Plaatje FROM tblModel WHERE ID = 1"); //code used to test
    query.prepare("SELECT Plaatje FROM tblMerk "
                  "WHERE strMerk = :chosen");
    query.bindValue(":chosen", QString::fromStdString(brandChosen));
    if(query.exec() && query.next())
    {
        QByteArray blobData = query.value(0).toByteArray();

        QImage image;
        image.loadFromData(QByteArray::fromBase64(blobData));
        QImage image2 = image.scaled(200, 200, Qt::KeepAspectRatio);
        ui->logo->setPixmap(QPixmap::fromImage(image2));
        ui->logo->setAlignment(Qt::AlignCenter);
    }
    else
    {
        qDebug() << "Query failed";
    }

    db.close();
}

// Slot function to handle item click events of brand list
void MainWindow::onBrandClicked(QListWidgetItem* brand)
{
    // Retrieve the text of the clicked item
    QString brandName = brand->text();
    qDebug() << "Clicked item: " << brandName; //for debugging
    MainWindow::listBrandDetails(brandName.toStdString()); //show brand details
    MainWindow::listModel(brandName.toStdString()); //show list of models
    MainWindow::brandLogo(brandName.toStdString());
    ui->modelDetails->clear();
}

void MainWindow::onModelClicked(QListWidgetItem* model)
{
    QString modelName = model->text();
    qDebug() << "Clicked item: " << modelName; //for debugging
    MainWindow::listModelDetails(modelName.toStdString()); //show model details
}

void MainWindow::addTemplate(std::string from)
{
    db.open();
    QSqlQuery query;
    QLabel *chooseBrand = new QLabel(this);
    bool ok;
    QString windowName = "Add" + QString::fromStdString(from);
    QString fromQString = QString::fromStdString(from).trimmed();
    QString enter;

    if(fromQString != "Power")
    {
        enter = QString::fromStdString(from) + "Name:";
        qDebug()<< "entered if";
    }
    else
    {
        enter = QString::fromStdString(from);
    }
    QString text = QInputDialog::getText(this, tr(qPrintable(windowName)),
                                         tr(qPrintable(enter)), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
        chooseBrand->setText(text);

    QString intotbl = "tbl" + QString::fromStdString(from);
    QString queryStr = "SELECT max(ID) FROM " + intotbl;

    query.prepare(queryStr);
    if(!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError().text();
        QMessageBox::information(this, "Error", "Query failed");
        return;
    }

    queryStr = "INSERT INTO " + intotbl + " VALUES (:value1, :value2)";
    query.prepare(queryStr);
    query.bindValue(":value1", query.value(0));  // Replace "column1" with your actual column name
    query.bindValue(":value2", text);           // Replace "column2" with your actual column name

    if (query.exec()) {
        // Handle the successful insertion here
    } else {
        qDebug() << "Query failed:" << query.lastError().text();
    }

        db.close();
}

void MainWindow::addBrand()
{
        addTemplate("Merk");
}

MainWindow::~MainWindow()
{
    delete ui;
}


