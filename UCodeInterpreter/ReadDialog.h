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


public slots:
    void sendButtonClicked();

private:

    Ui::Read ui;
    std::string strValue = "";
};