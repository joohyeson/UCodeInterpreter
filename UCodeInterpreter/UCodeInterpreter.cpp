#include "UCodeInterpreter.h"
#include <fstream>

UCodeInterpreter::UCodeInterpreter(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_6, &QPushButton::clicked, this, &UCodeInterpreter::On_ExitButton_Clicked);
}

void UCodeInterpreter::On_ExitButton_Clicked()
{
    this->close();
}

void UCodeInterpreter::ReadFile(std::string path)
{
    std::ifstream is(path, std::ifstream::binary);
    std::string nowLine;
    std::string lines;
    int count = 0;

    if (is.is_open()) {
        while (std::getline(is, nowLine))
        {
            lines += nowLine;

            if (nowLine[0] == ' ')//label이 없는 경우
            {
                std::string label;
                

                int param[3] = { -1, -1, -1 };

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
                        param[i] = atoi(*arr);
                    }
                }

                Instruction instruction = Instruction(label, inst, param[0], param[1], param[2]);

                Instructions.push_back(instruction);

                ui.textEdit_2->setText(QString::fromStdString(instruction.inst)+" "+QString::number(instruction.param1) + " " + QString::number(instruction.param2) + " " + QString::number(instruction.param3));

            }
            else//label 있는 경우
            {
                //std::string label;
                //std::string inst;

                //int param1;
                //int param2;
                //int param3;

                //char* arr[1024];

                //*arr = strtok(&nowLine[0], " ");
                //strncpy(&label[0], *arr, nowLine.size());
                //*arr = strtok(&nowLine[0], " ");
                //strncpy(&inst[0], *arr, nowLine.size());
                //*arr = strtok(&nowLine[0], " ");
                //param1 = atoi(*arr);
                //*arr = strtok(&nowLine[0], " ");
                //param2 = atoi(*arr);
                //*arr = strtok(&nowLine[0], " ");
                //param3 = atoi(*arr);

                //Instruction instruction = Instruction(label, inst, param1, param2, param3);

                //ui.textEdit_2->setText(QString::fromStdString(instruction.label));
                //ui.textEdit_2->setText(QString::fromStdString(instruction.inst));
                ////ui.textEdit_2->setText(QString::number(instruction.param1));
                ////ui.textEdit_2->setText(QString::number(instruction.param2));
                ////ui.textEdit_2->setText(QString::number(instruction.param3));
            }

            count++;
        }


    }

    ui.textEdit->setText(QString::fromStdString(lines));

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
        if (param1Inst[i]==ins)
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


    //입출력 처리 함수(read, wrtie, lf 는 뭔지 교수님한테 물어보기)

}
