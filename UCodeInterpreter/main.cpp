#include "UCodeInterpreter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UCodeInterpreter w;
    w.ReadFile("bubble2.uco");
    w.show();
    return a.exec();
}
