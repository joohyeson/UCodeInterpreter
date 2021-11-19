#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UCodeInterpreter.h"
#include "Memory.h"
#include <stack>
#include "LabelInfo.h"
#include "Instruction.h"

class UCodeInterpreter : public QMainWindow
{
    Q_OBJECT

public:
    UCodeInterpreter(QWidget *parent = Q_NULLPTR);

    void ReadFile(std::string path);
    int GetParamCount(std::string ins);
    void Assemble() {};
    void Execute() {};
    void Statistics() {};

private slots:
    void On_ExitButton_Clicked();

private:
    Ui::UCodeInterpreterClass ui;
    Memory mMemory;
    std::stack<int> mCPU;
    std::vector<LabelInfo> Labels;
    std::vector<Instruction> Instructions;
};
