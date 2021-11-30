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
        if (MemoryStack[block-1].size() <= offset) {
            MemoryStack[block-1].resize(offset + 1);
        }

        MemoryStack[block-1][offset]=value;
    }

    void SetMemoryValue(int value, int addr)
    {
        //어디서 size 안 늘려주면 터질듯
        int* ptr = reinterpret_cast<int*>(addr);
        *ptr= value;
    }

    void SetMemoryValue(int value) // 확실x
    {
        MemoryStack->push_back(value);
    }

   
private:
    int BP;
    int SP;

    std::vector<int> MemoryStack[2];
};