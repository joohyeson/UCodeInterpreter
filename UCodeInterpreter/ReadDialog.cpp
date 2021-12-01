#include "ReadDialog.h"

ReadDialog::ReadDialog(QWidget* parent) :QDialog(parent) {
    ui.setupUi(this);

    connect(ui.pushButton, &QPushButton::clicked, this, &ReadDialog::sendButtonClicked);
}

void ReadDialog::sendButtonClicked() {

    if (ui.lineEdit->text().toStdString() != "")
    {
        strValue = ui.lineEdit->text().toStdString();

        accept();
        close();
    }
}
