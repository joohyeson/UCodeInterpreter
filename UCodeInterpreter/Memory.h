#pragma once
#include <stack>
#include "MemoryInfo.h"

class Memory {
public:

    int GetMemoryValue(int block, int offset)
    {
        return MemoryStack[block][offset];
    }

    int GetMemoryValue(int addr)
    {
        int* ptr = reinterpret_cast<int*>(addr);
        return *ptr;
    }

    int GetMemoryAddress(int block, int offset)
    {
        auto it = MemoryStack[block].begin();
        return reinterpret_cast<int>(&(it+offset));
    }

    void SetMemoryValue(int value, int block, int offset)
    {
        //어디서 size 안 늘려주면 터질듯

        MemoryStack[block][offset]=value;
    }

    void SetMemoryValue(int value, int addr)
    {
        //어디서 size 안 늘려주면 터질듯
        int* ptr = reinterpret_cast<int*>(addr);
        *ptr= value;
    }

   
private:
    int BP;
    int SP;

    std::vector<int> MemoryStack[2];
};