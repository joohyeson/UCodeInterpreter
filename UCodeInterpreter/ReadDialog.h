#pragma once
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Read.h"

class ReadDialog : public QDialog
{
    Q_OBJECT

public:
    ReadDialog(QWidget* parent = Q_NULLPTR);

    int GetReadValue() {
        return std::stoi(strValue);
    }

    void reject()
    {
        return;
    }

signals:
    void sendData(QString text);


public slots:
    void sendButtonClicked();

private:

    Ui::Read ui;
    std::string strValue = "";
};