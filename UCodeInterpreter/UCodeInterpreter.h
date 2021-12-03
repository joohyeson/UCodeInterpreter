#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UCodeInterpreter.h"
#include "ReadDialog.h"
#include "Memory.h"
#include <stack>
#include "LabelInfo.h"
#include "Instruction.h"
#include "Result.h"
#include <fstream>
#include <Qmessagebox>

#define NO_OPCODE 40

class UCodeInterpreter : public QMainWindow
{
    Q_OBJECT

public:
    UCodeInterpreter(QWidget *parent = Q_NULLPTR);
    
    std::ifstream is;

    void ReadFile(std::string path);
    int GetParamCount(std::string ins);
    void Assemble();
    void Execute(int now);
    void Statistics();
    void CreateFile(std::string path);
    void PrintCPUStack();
    void PrintMemory();

private slots:
    void On_ReadUcoButton_Clicked();
    void On_CreateLstButton_Clicked();
    void On_ExitButton_Clicked();
    void On_StepButton_Clicked();
    void On_RunButton_Clicked();
    void On_JumpButton_Clicked();

private:

    Ui::UCodeInterpreterClass ui;
    ReadDialog* read;
    Memory mMemory;
    std::stack<int> mCPU;
    std::stack<int> topstack;
    std::vector<Result> Results;
    std::vector<std::string> result;
    std::vector<LabelInfo> Labels;
    std::vector<Instruction> Instructions;
    bool hasInstructions = true;

    QMessageBox msgbox;
    int filecheck = 0;
    int instCnt[40];
    int PC = 0;
};