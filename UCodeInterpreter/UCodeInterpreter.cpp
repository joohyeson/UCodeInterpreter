#include "UCodeInterpreter.h"
#include <fstream>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cctype>

UCodeInterpreter::UCodeInterpreter(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.tableWidget->setColumnWidth(0, 80);
    ui.tableWidget->setColumnWidth(1, 100);
    ui.tableWidget->setColumnWidth(2, 80);
    ui.tableWidget->setColumnWidth(3, 80);
    ui.tableWidget->setColumnWidth(4, 80);

    ui.tableWidget_2->setColumnWidth(0, 230);
    ui.tableWidget_2->setColumnWidth(1, 230);

    ui.CPUStackTable->setColumnWidth(0, 150);

    ui.MemoryTable->setColumnWidth(0, 75);
    ui.MemoryTable->setColumnWidth(1, 75);
    ui.MemoryTable->setColumnWidth(2, 80);

    connect(ui.pushButton_2, &QPushButton::clicked, this, &UCodeInterpreter::On_StepButton_Clicked);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &UCodeInterpreter::On_ReadUcoButton_Clicked);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &UCodeInterpreter::On_RunButton_Clicked);
    connect(ui.pushButton_5, &QPushButton::clicked, this, &UCodeInterpreter::On_CreateLstButton_Clicked);
    connect(ui.pushButton_6, &QPushButton::clicked, this, &UCodeInterpreter::On_ExitButton_Clicked);
}

void UCodeInterpreter::On_ReadUcoButton_Clicked()
{
    ReadFile(QFileDialog::getOpenFileName(this, "Search File", QDir::currentPath(), "Files(*.uco)").toStdString());
    Assemble();
}

void UCodeInterpreter::On_CreateLstButton_Clicked()
{
    CreateFile(QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), "Files(*.lst)").toStdString());
}

void UCodeInterpreter::On_ExitButton_Clicked()
{
    this->close();
}

void UCodeInterpreter::On_StepButton_Clicked()
{
    if (hasInstructions == true)
    {
        Execute(PC);
        PC++;
    }
    else
    {
        return;
    }
}

void UCodeInterpreter::On_RunButton_Clicked()
{
    while (hasInstructions == true) {
        Execute(PC);
        PC++;
    }

    return;
}

typedef enum opcode {
    nop, bgn, sym, lod, lda, ldc, str, ldi, sti,
    not, neg, inc, dec, dup, add, sub, mult, divop, mod,
    gt, lt, ge, le, eq, ne, and, or , swp, ujp, tjp, fjp,
    call, ret, push, ldp, proc, endop, read, write, lf,
}opcode; // Execute 함수 case문에서 사용

std::string opcodeName[NO_OPCODE] =
{
    "nop", "bgn", "sym", "lod", "lda", "ldc", "str", "ldi", "sti",
    "not", "neg", "inc", "dec", "dup", "add", "sub", "mult", "div", "mod",
    "gt", "lt", "ge", "le", "eq", "ne", "and", "or", "swp", "ujp", "tjp", "fjp",
    "call", "ret", "push", "ldp", "proc", "end", "read", "write", "lf"
}; // 모든 명령어

void UCodeInterpreter::ReadFile(std::string path)
{
    std::ifstream is(path, std::ifstream::in, std::ifstream::binary);
    std::string nowLine;
    std::string lines;

    int lineCount = 0;

    if (is.is_open()) {
        while (std::getline(is, nowLine))
        {
            lines += nowLine;

            if (nowLine[0] == ' ')//label이 없는 경우
            {
                std::string label;

                std::string param[3] = { "", "", "" };

                char* arr[1024];
                *arr = strtok(&nowLine[0], " ");

                std::string inst(*arr);

                for (int i = 0; i < UCodeInterpreter::GetParamCount(inst); i++)
                {
                    *arr = strtok(NULL, " ");
                    if (*arr == NULL)
                    {
                        param[i] = -1;
                        break;
                    }
                    else
                    {
                        //param[i] = atoi(*arr); //ascii to Integer -> String
                        
                        param[i] = *arr;
                       
                    }
                }

                Instruction instruction = Instruction(label, inst, param[0], param[1], param[2]);

                Instructions.push_back(instruction);

            }
            else //label 있는 경우
            {
                std::string param[3] = { "", "", "" };

                char* arr[1024];

                *arr = strtok(&nowLine[0], " ");
                std::string label(*arr);

                *arr = strtok(NULL, " ");
                std::string inst(*arr);

                for (int i = 0; i < UCodeInterpreter::GetParamCount(inst); i++)
                {
                    *arr = strtok(NULL, " ");
                    if (*arr == NULL)
                    {
                        param[i] = -1;
                        break;
                    }
                    else
                    {
                        param[i] = atoi(*arr);
                    }
                }

                if (inst == "bgn") PC = lineCount;

                Instruction instruction = Instruction(label, inst, param[0], param[1], param[2]);

                LabelInfo tmpLabel;
                tmpLabel.addr = lineCount + 1;
                tmpLabel.label = label;

                Instructions.push_back(instruction);
                Labels.push_back(tmpLabel);
            }
            lineCount++;
        }
    }

    //ui.textEdit->setText(QString::number(Labels[1].addr));

}

int UCodeInterpreter::GetParamCount(std::string ins)
{
    std::string param0Inst[] = { "nop", "end", "ret", "ldp", "push", "ldi", "sti", "not", "neg", "inc", "dec", "dup",
                                 "add", "sub", "mult", "div", "mod", "gt", "lt", "ge", "le", "eq", "ne", "and", "or", "swap" };
    std::string param1Inst[] = { "bgn", "proc", "call", "ujp", "tjp", "fjp", "ldc" };
    std::string param2Inst[] = { "lod", "lda", "str" };
    std::string param3Inst[] = { "sym" };

    for (int i = 0; i < param1Inst->size(); i++)
    {
        if (param1Inst[i] == ins)
        {
            return 1;
        }
    }

    for (int i = 0; i < param2Inst->size(); i++)
    {
        if (param2Inst[i] == ins)
        {
            return 2;
        }
    }

    for (int i = 0; i < param3Inst->size(); i++)
    {
        if (param3Inst[i] == ins)
        {
            return 3;
        }
    }

    return 0;
}

void UCodeInterpreter::Assemble()
{
    for (int i = 0; i < Instructions.size(); i++)
    {
        ui.tableWidget->insertRow(ui.tableWidget->rowCount());

        ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(Instructions[i].label)));
        ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(Instructions[i].inst)));
        ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 2, new QTableWidgetItem(QString::fromStdString(Instructions[i].param1)));
        ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 3, new QTableWidgetItem(QString::fromStdString(Instructions[i].param2)));
        ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 4, new QTableWidgetItem(QString::fromStdString(Instructions[i].param3)));
    }

    for (int i = 0; i < Labels.size(); i++)
    {
        ui.tableWidget_2->insertRow(ui.tableWidget_2->rowCount());

        ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(Labels[i].label)));
        ui.tableWidget_2->setItem(ui.tableWidget_2->rowCount() - 1, 1, new QTableWidgetItem(QString::number(Labels[i].addr)));
    }
}

void UCodeInterpreter::Execute(int now)
{
    enum opcode inst;    // 열거형 변수 선언

    for (int j = 0; j < 40; j++)
    {
        if (opcodeName[j] == Instructions[now].inst)
        {
            inst = static_cast<opcode>(j);
        }//enum 변수값 찾아서 초기화해줌
    }

    if (Instructions[now].inst == "end") {
        hasInstructions == false;
        return;
    }

    switch (inst)//switch문에서는 str못 넣어서 enum값 사용
    {
        // 함수 정의 및 호출  확실 x
    case opcode::ret:
    {
        int origin = topstack.top();
        topstack.pop();

        mCPU.top() = origin;
        PC = mCPU.top();
        break;
    }

    case opcode::ldp:
    {
        break;
    }

    case opcode::push:
    {
        int origin = mCPU.top();

        mMemory.SetMemoryValue(origin);

        break;
    }

    case opcode::call:
    {
        //if (!strcmp(Instructions[now].param1, ))
        {

        }
        break;
    }

    // 흐름 제어
    case opcode::ujp:
    {
        int location = std::stoi(Instructions[now].param1);
        PC = Labels[location].addr;
        break;
    }

    case opcode::tjp:
    {
        if (mCPU.top() != 0)
        {
            int location = std::stoi(Instructions[now].param1);
            PC = Labels[location].addr;
        }
        break;
    }

    case opcode::fjp:
    {
        if (mCPU.top() == 0)
        {
            int location = std::stoi(Instructions[now].param1);
            PC = Labels[location].addr;
        }
        break;
    }

    // 단항 연산자
    case opcode::not:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(!origin);
        break;
    }

    case opcode::neg:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(-1 * origin);
        break;
    }

    case opcode::inc:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(++origin);
        break;
    }

    case opcode::dec:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(--origin);
        break;
    }

    case opcode::dup:
    {
        int origin = mCPU.top();

        mCPU.push(origin);
        break;
    }

    // 이항 연산자
    case opcode::add:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp + origin);
        break;
    }

    case opcode::sub:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp - origin);
        break;
    }

    case opcode::mult:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp * origin);
        break;
    }

    case opcode::divop:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp / origin);
        break;
    }

    case opcode::mod:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp % origin);
        break;
    }

    case opcode::gt:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp > origin);
        break;
    }

    case opcode::lt:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp < origin);
        break;
    }

    case opcode::ge:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp >= origin);
        break;
    }

    case opcode::le:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp <= origin);
        break;
    }

    case eq:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp == origin);
        break;
    }


    case opcode::ne:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp != origin);
        break;
    }

    case opcode::and :
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp && origin);
        break;
    }

    case opcode:: or :
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp || origin);
        break;
    }

    case opcode::swp:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(origin);
        mCPU.push(cmp);
        break;
    }

    case opcode::lod: {

        int value = mMemory.GetMemoryValue(std::stoi(Instructions[now].param1), std::stoi(Instructions[now].param2));
        mCPU.push(value);
        break;

    }

    case opcode::lda: {//확실 x
        int value = mMemory.GetMemoryAddress(std::stoi(Instructions[now].param1), std::stoi(Instructions[now].param2));
        mCPU.push(value);
        break;
    }
    case opcode::ldc: {
        mCPU.push(std::stoi(Instructions[now].param1));

        break;
    }

    case opcode::str: {
        int origin = mCPU.top();
        mCPU.pop();

        mMemory.SetMemoryValue(origin, std::stoi(Instructions[now].param1), std::stoi(Instructions[now].param2));
        break;
    }
    case opcode::ldi: {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(mMemory.GetMemoryValue(origin));
        break;
    }

    case opcode::sti: {
        int value = mCPU.top();
        mCPU.pop();

        int addr = mCPU.top();
        mCPU.pop();

        mMemory.SetMemoryValue(value, addr);
        break;
    }

    default:
        break;
    }

    mMemory.SetMemoryValue(1, 0, 0);

    ui.MemoryTable->setRowCount(1);

    ui.MemoryTable->setItem(0, 0, new QTableWidgetItem(QString::number(mMemory.GetMemoryValue(0, 0))));

    PrintCPUStack();

}

//GUI CPUStack 출력
void UCodeInterpreter::PrintCPUStack() {

    std::stack<int> tmp = mCPU;
    std::vector<int> vec;

    for (int i = 0; i < mCPU.size(); i++) {
        vec.push_back(tmp.top());
        tmp.pop();
    }

    ui.CPUStackTable->setRowCount(mCPU.size());

    for (int i = 0; i < vec.size(); i++) {
        ui.CPUStackTable->setItem(i, 0, new QTableWidgetItem(QString::number(vec[i])));
    }
}

void UCodeInterpreter::CreateFile(std::string path)
{
    std::ofstream os(path, std::ofstream::out, std::ofstream::binary);

}