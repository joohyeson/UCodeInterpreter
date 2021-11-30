#pragma once
#include <stack>
#include "MemoryInfo.h"

class Memory {
public:

    int GetMemoryValue(int block, int offset)
    {
        
    }


    

private:
    int BP;
    int SP;
    std::vector<MemoryInfo> MemoryStack;
};