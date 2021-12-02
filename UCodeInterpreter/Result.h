#pragma once

#include <string>

class Result {
public:
    std::string cpuValue; // 결과값
    std::string instValue;  // 명령어 실행 횟수
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