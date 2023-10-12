#ifndef DATAENTRYDIALOG_H
#define DATAENTRYDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class DataEntryDialog : public QDialog
{
    Q_OBJECT

public:
    DataEntryDialog(QWidget* parent = nullptr);

    // Method to retrieve entered data
    QString getData() const;

    // Method to clear the data entry fields
    void clearData();

private:
    QFormLayout* layout;
    QLineEdit* dataLineEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;
};

#endif // DATAENTRYDIALOG_H
