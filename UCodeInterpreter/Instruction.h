#include <string>

class Instruction {
public:
    std::string label;
    std::string inst;

    std::string param1;
    std::string param2;
    std::string param3;

    Instruction(std::string a, std::string b, std::string p1, std::string p2, std::string p3)
    {
        label = a;
        inst = b;

        param1 = p1;
        param2 = p2;
        param3 = p3;
    }
};