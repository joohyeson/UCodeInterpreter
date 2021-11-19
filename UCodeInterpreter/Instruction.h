#include <string>

class Instruction {
public:
    std::string label;
    std::string inst;

    int param1;
    int param2;
    int param3;

    Instruction(std::string a, std::string b, int p1, int p2, int p3)
    {
        label = a;
        inst = b;

        param1 = p1;
        param2 = p2;
        param3 = p3;
    }
};