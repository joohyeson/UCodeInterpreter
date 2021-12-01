#pragma once
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Read.h"

class ReadDialog : public QDialog
{
    Q_OBJECT

public:
    ReadDialog(QWidget* parent = Q_NULLPTR);

private:

    Ui::Read ui;
};