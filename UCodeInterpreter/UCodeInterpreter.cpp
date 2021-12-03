#include "UCodeInterpreter.h"
#include <fstream>
#include <QFileDialog>
#include <QDebug>
#include <iostream>
#include <cctype>
#include "ReadDialog.h"


UCodeInterpreter::UCodeInterpreter(QWidget* parent) : QMainWindow(parent)
{

    ui.setupUi(this);
    read = new ReadDialog();

    ui.tableWidget->setColumnWidth(0, 80);
    ui.tableWidget->setColumnWidth(1, 100);
    ui.tableWidget->setColumnWidth(2, 80);
    ui.tableWidget->setColumnWidth(3, 80);
    ui.tableWidget->setColumnWidth(4, 80);

    ui.tableWidget_2->setColumnWidth(0, 120);
    ui.tableWidget_2->setColumnWidth(1, 120);

    ui.CPUStackTable->setColumnWidth(0, 150);

    ui.MemoryTable->setColumnWidth(0, 75);
    ui.MemoryTable->setColumnWidth(1, 75);
    ui.MemoryTable->setColumnWidth(2, 80);

    connect(ui.pushButton_2, &QPushButton::clicked, this, &UCodeInterpreter::On_StepButton_Clicked);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &UCodeInterpreter::On_ReadUcoButton_Clicked);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &UCodeInterpreter::On_RunButton_Clicked);
    connect(ui.pushButton_5, &QPushButton::clicked, this, &UCodeInterpreter::On_CreateLstButton_Clicked);
    connect(ui.pushButton_6, &QPushButton::clicked, this, &UCodeInterpreter::On_ExitButton_Clicked);
    connect(ui.pushButton, &QPushButton::clicked, this, &UCodeInterpreter::On_JumpButton_Clicked);
}

void UCodeInterpreter::On_ReadUcoButton_Clicked()
{
    ReadFile(QFileDialog::getOpenFileName(this, "Search File", QDir::currentPath(), "Files(*.uco)").toStdString());
    Assemble();
}

void UCodeInterpreter::On_CreateLstButton_Clicked()
{
    CreateFile(QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath(), "Files(*.lst)").toStdString());
    Statistics();
}

void UCodeInterpreter::On_ExitButton_Clicked()
{
    this->close();
}

void UCodeInterpreter::On_JumpButton_Clicked()
{
    if (filecheck == 0)
    {
        msgbox.setText("First read .*uco file");
        msgbox.exec();
    }

    if (hasInstructions == true)
    {
        while (Instructions[PC].label=="") {
            Execute(PC);
            PC++;
           
        }

        ui.tableWidget->selectRow(PC);

        UCodeInterpreter::PrintCPUStack();
        UCodeInterpreter::PrintMemory();
    }
    else
    {
        ui.tableWidget->selectRow(PC - 1);
        msgbox.setText(QString::fromLocal8Bit("실행결과: \n"));

        for (int resultValue = 0; resultValue < result.size(); resultValue++)
        {
            msgbox.setText(msgbox.text() + QString::fromStdString(result[resultValue]));
        }

        msgbox.exec();
        return;
    }
}

void UCodeInterpreter::On_StepButton_Clicked()
{
    if (filecheck == 0)
    {
        msgbox.setText("First read .*uco file");
        msgbox.exec();
    }

    if (hasInstructions == true)
    {
        Execute(PC);
        ui.tableWidget->selectRow(PC);

        PC++;


        UCodeInterpreter::PrintCPUStack();
        UCodeInterpreter::PrintMemory();
    }
    else
    {
        ui.tableWidget->selectRow(PC - 1);
        msgbox.setText(QString::fromLocal8Bit("실행결과: \n"));

        for (int resultValue = 0; resultValue < result.size(); resultValue++)
        {
            msgbox.setText(msgbox.text() + QString::fromStdString(result[resultValue]));
        }

        msgbox.exec();
        return;
    }
}

void UCodeInterpreter::On_RunButton_Clicked()
{
    if (filecheck == 0)
    {
        msgbox.setText("First read .*uco file");
        msgbox.exec();
    }

    while (hasInstructions == true) {
        Execute(PC);
        PC++;
    }
    ui.tableWidget->selectRow(PC-1);

    UCodeInterpreter::PrintCPUStack();
    UCodeInterpreter::PrintMemory();

    if (hasInstructions == false)
    {
        ui.tableWidget->selectRow(PC - 1);
        msgbox.setText(QString::fromLocal8Bit("실행결과: \n"));

        for (int resultValue = 0; resultValue < result.size(); resultValue++)
        {
            msgbox.setText(msgbox.text() + QString::fromStdString(result[resultValue]));
        }

        msgbox.exec();
    }


    return;
}


typedef enum opcode {
    nop, bgn, sym, lod, lda, ldc, str, ldi, sti,
    not, neg, inc, dec, dup, add, sub, mult, divop, mod,
    gt, lt, ge, le, eq, ne, and, or , swp, ujp, tjp, fjp,
    call, ret, push, ldp, proc, end, read, write, lf,
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
                        param[i] = *arr;
                    }
                }

                Instruction instruction = Instruction(label, inst, param[0], param[1], param[2]);

                if (inst == "bgn") {
                    PC = lineCount;
                    instCnt[1]++;
                }
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
                        param[i] = *arr;
                    }
                }

                if (inst == "bgn") {
                    PC = lineCount;
                    instCnt[1]++;
                }

                Instruction instruction = Instruction(label, inst, param[0], param[1], param[2]);

                LabelInfo tmpLabel;
                tmpLabel.addr = lineCount + 1;
                tmpLabel.label = label;

                Instructions.push_back(instruction);
                Labels.push_back(tmpLabel);
            }
            lineCount++;
        }
        filecheck++;
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

    for (int i = 0; i < 7; i++)//param1개수만큼
    {
        if (param1Inst[i] == ins)
        {
            return 1;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (param2Inst[i] == ins)
        {
            return 2;
        }
    }

    for (int i = 0; i < 1; i++)
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

    int pcTemp = 0;

    for (int j = 0; j < 40; j++)
    {
        if (opcodeName[j] == Instructions[now].inst)
        {
            inst = static_cast<opcode>(j);
        }//enum 변수값 찾아서 초기화해줌
    }

    if (Instructions[now].inst == "end") {
        hasInstructions = false;
        instCnt[36]++;
        return;
    }

    switch (inst)//switch문에서는 str못 넣어서 enum값 사용
    {

    case opcode::nop: {
        instCnt[0]++;
        break;
    }
    case opcode::sym: {
        instCnt[2]++;
        break;
    }
    case opcode::bgn:
    {
        mMemory.AddMemory(std::stoi(Instructions[now].param1), "bgn");
        break;
    }

    case opcode::proc:
    {
        int variableSize = std::stoi(Instructions[now].param1);

        mMemory.AddMemory(variableSize, std::string("proc"));
        instCnt[35]++;
        break;
    }

    // 함수 정의 및 호출  확실 x
    case opcode::ret:
    {
        int returnAddr = mMemory.GetMemoryValue(mMemory.GetBP() + 1);

        PC = returnAddr;

        mMemory.RemoveMemory();
        instCnt[32]++;
        break;
    }

    case opcode::ldp:
    {

        mMemory.AddMemory(2, "ldp");

        pcTemp = PC;

        while (1)
        {
            pcTemp++;
            if ((Instructions[pcTemp].inst == "call"))
            {
                break;
            }
        }//PC가 아닌 pcTemp를 증가시켜서 값을 찾아봄 (PC를 증가시키면 jump랑 같은 기능)

        mMemory.SetMemoryValue(pcTemp, mMemory.GetSP());
        instCnt[34];
        break;
    }

    case opcode::push:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mMemory.SetMemoryValue(origin);
        instCnt[33] ++;
        break;
    }

    case opcode::call:
    {
        instCnt[31] ++;
        for (int i = 0; i < Labels.size(); i++)
        {

            if (Instructions[now].param1 == "read") {

                read->setModal(true);
                int dialogCode = read->exec();

                if (dialogCode == QDialog::Accepted) {
                    int addr = mMemory.GetSP() + 1;
                    int value = mMemory.GetMemoryValue(addr);

                    mMemory.SetMemoryValue(read->GetReadValue(), value);

                    mMemory.SetSP(mMemory.GetSP() - 2);
                    mMemory.ResizeMemory(mMemory.GetSP() + 1);
                    //mCPU.push(read->GetReadValue());
                }
                instCnt[37] ++;
                break;
            }
            else if (Instructions[now].param1 == "write") {
                int addr = mMemory.GetSP() + 1;
                int value = mMemory.GetMemoryValue(addr);

                result.push_back(std::to_string(value));
                result.push_back("\t");

                mMemory.SetSP(mMemory.GetSP() - 2);
                mMemory.ResizeMemory(mMemory.GetSP() + 1);
                instCnt[38] ++;
                break;
            }
            else if (Instructions[now].param1 == "lf") {
                result.push_back("\n");

                mMemory.SetSP(mMemory.GetSP() - 2);
                mMemory.ResizeMemory(mMemory.GetSP() + 1);
                instCnt[39] ++;
                break;
            }
            else if (Instructions[now].param1 == Labels[i].label)
            {
                PC = Labels[i].addr - 2;
                break;
            }
        }
        break;
    }

    // 흐름 제어
    case opcode::ujp:
    {
        for (int labelcount = 0; labelcount < Labels.size(); labelcount++) {
            if (Labels[labelcount].label == Instructions[now].param1) {
                PC = Labels[labelcount].addr - 1;
                break;
            }
        }
        instCnt[28] ++;
        break;
    }

    case opcode::tjp:
    {
        if (mCPU.top() != 0)
        {
            for (int labelcount = 0; labelcount < Labels.size(); labelcount++) {
                if (Labels[labelcount].label == Instructions[now].param1) {
                    PC = Labels[labelcount].addr - 1;
                    mCPU.pop();
                    break;
                }
            }
        }
        else
        {
            mCPU.pop();
        }

        instCnt[29] ++;
        break;
    }

    case opcode::fjp:
    {
        if (mCPU.top() == 0)
        {
            for (int labelcount = 0; labelcount < Labels.size(); labelcount++) {
                if (Labels[labelcount].label == Instructions[now].param1) {
                    PC = Labels[labelcount].addr - 1;
                    mCPU.pop();
                    break;
                }
            }
        }
        else
        {
            mCPU.pop();
        }
        instCnt[30] ++;
        break;
    }

    // 단항 연산자
    case opcode::not:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(!origin);
        instCnt[9] ++;
        break;
    }

    case opcode::neg:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(-1 * origin);
        instCnt[10] ++;
        break;
    }

    case opcode::inc:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(++origin);
        instCnt[11] ++;
        break;
    }

    case opcode::dec:
    {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(--origin);
        instCnt[12] ++;
        break;
    }

    case opcode::dup:
    {
        int origin = mCPU.top();

        mCPU.push(origin);
        instCnt[13] ++;
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
        instCnt[14] ++;
        break;
    }

    case opcode::sub:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp - origin);
        
        instCnt[15] ++;
        break;
    }

    case opcode::mult:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp * origin);
        
        instCnt[16] ++;
        break;
    }

    case opcode::divop:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp / origin);
        
        instCnt[17] ++;
        break;
    }

    case opcode::mod:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp % origin);
        instCnt[18] ++;
        break;
    }

    case opcode::gt:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp > origin);
        instCnt[19] ++;
        break;
    }

    case opcode::lt:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp < origin);
        instCnt[20] ++;
        break;
    }

    case opcode::ge:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp >= origin);
        instCnt[21] ++;
        break;
    }

    case opcode::le:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp <= origin);
        instCnt[22] ++;
        break;
    }

    case opcode::eq:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp == origin);
        instCnt[23] ++;
        break;
    }


    case opcode::ne:
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp != origin);
        instCnt[24] ++;
        break;
    }

    case opcode::and :
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp && origin);
        instCnt[25] ++;
        break;
    }

    case opcode:: or :
    {
        int origin = mCPU.top();
        mCPU.pop();
        int cmp = mCPU.top();
        mCPU.pop();

        mCPU.push(cmp || origin);
        instCnt[26] ++;
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
        instCnt[27] ++;
        break;
    }

    case opcode::lod: {

        int value = mMemory.GetMemoryValue(std::stoi(Instructions[now].param1), std::stoi(Instructions[now].param2));
        mCPU.push(value);
        instCnt[3] ++;
        break;

    }

    case opcode::lda: {//확실 x
        int value = mMemory.GetMemoryAddress(std::stoi(Instructions[now].param1), std::stoi(Instructions[now].param2));
        mCPU.push(value);
        instCnt[4] ++;
        break;
    }
    case opcode::ldc: {
        mCPU.push(std::stoi(Instructions[now].param1));
        instCnt[5] ++;
        break;
    }

    case opcode::str: {
        int origin = mCPU.top();
        mCPU.pop();

        mMemory.SetMemoryValue(origin, std::stoi(Instructions[now].param1), std::stoi(Instructions[now].param2));
        instCnt[6]++;
        break;
    }
    case opcode::ldi: {
        int origin = mCPU.top();
        mCPU.pop();

        mCPU.push(mMemory.GetMemoryValue(origin));
        instCnt[7]++;
        break;
    }

    case opcode::sti: {
        int value = mCPU.top();
        mCPU.pop();

        int addr = mCPU.top();
        mCPU.pop();

        mMemory.SetMemoryValue(value, addr);
        instCnt[8]++;
        break;
    }

    default:
        break;
    }
}

void UCodeInterpreter::Statistics()
{
    ui.textBrowser->append(QString::fromLocal8Bit("==================== 실행 결과 ===================="));
    ui.textBrowser->append(QString::fromLocal8Bit("<<<<<< 결과값 >>>>>>"));
    for (int i = 0; i < result.size(); i++)
    {
        ui.textBrowser->append(QString::fromStdString(result[i]));
    }
    ui.textBrowser->append(QString::fromLocal8Bit("<<<<<< 명령어 실행 횟수 >>>>>>"));
    for (int i = 0; i < Instructions.size(); i++)
    {
        ui.textBrowser->append(QString::fromStdString(Instructions[i].inst) + ": " + QString::number(instCnt[i]));
    }

    std::vector<int> tmpMemory = mMemory.GetMemoryStack();

    ui.textBrowser->append(QString::fromLocal8Bit("<<<<<< MemoryStack >>>>>>"));
    for (int i = 0; i < tmpMemory.size(); i++)
    {
        ui.textBrowser->append(QString::number(i+1) + QString::fromLocal8Bit("번지 : ") + QString::number(tmpMemory[i]));
    }
}

//GUI MemoryStack 출력
void UCodeInterpreter::PrintMemory() {

    std::vector<int> tmpMemory = mMemory.GetMemoryStack();

    ui.MemoryTable->setRowCount(0);

    for (int i = 0; i < tmpMemory.size(); i++) {
        ui.MemoryTable->insertRow(ui.MemoryTable->rowCount());
        //block, offset, value
        if (i < mMemory.GetDataSize())
        {
            ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 1, new QTableWidgetItem("1"));
        }
        else
        {
            ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 1, new QTableWidgetItem("2"));
        }

        ui.MemoryTable->selectRow(mMemory.GetBP() + 2);
        ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(i)));
        ui.MemoryTable->setItem(ui.MemoryTable->rowCount() - 1, 2, new QTableWidgetItem(QString::number(tmpMemory[i])));
    }
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