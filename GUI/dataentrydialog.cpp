#include "DataEntryDialog.h"

DataEntryDialog::DataEntryDialog(QWidget* parent) : QDialog(parent)
{
    layout = new QFormLayout(this);

    dataLineEdit = new QLineEdit(this);
    layout->addRow("Data:", dataLineEdit);

    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    layout->addRow(okButton, cancelButton);

    connect(okButton, &QPushButton::clicked, this, &DataEntryDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &DataEntryDialog::reject);
}

QString DataEntryDialog::getData() const
{
    return dataLineEdit->text();
}

void DataEntryDialog::clearData()
{
    dataLineEdit->clear();
}
