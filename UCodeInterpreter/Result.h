#pragma once

#include <string>

class Result {
public:
    std::string cpuValue; // �����
    std::string instValue;  // ��ɾ� ���� Ƚ��
    std::string memoryValue;  // memorystack

    /*Result(std::string c)
    {
        cpuValue = c;
    }*/

    /*Result(std::string c, std::string i, std::string m)
    {
        cpuValue = c;
        instValue = i;
        memoryValue = m;
    }*/
};