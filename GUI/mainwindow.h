#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <string.h>
#include <QListWidget>
#include <QMainWindow>
#include <QtWidgets>
#include <QDebug>
#include <QPushButton>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void listBrand();
    void listBrandDetails(std::string brandChosen);
    void listModel(std::string brand);
    void listModelDetails(std::string model);
    void brandLogo(std::string brandChosen);
    void addTemplate(std::string from);

public slots:
    void radioButton();
    //void addBrand();
    //void addModel();
    //void changeBrandName();
    //void changeHQ();
    //void changeModelName();
    //void changePower();
    //void removeBrand();
    //void removeModel();

private slots:
    void onBrandClicked(QListWidgetItem* item);
    void onModelClicked(QListWidgetItem* item);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QListWidget* listWidget;
    float KWFloat;
    float PKCalculated;
    bool radioState;
};

#endif // MAINWINDOW_H
