#pragma once
#include <stack>
#include "MemoryInfo.h"

class Memory {
public:

    int GetMemoryValue(int block, int offset)
    {

        if (block == 1)
        {
            return MemoryStack[block - 1][offset];
        }
        else
        {
            return MemoryStack[1][BP + offset];
        }
       
    }

    int GetMemoryValue(int addr)
    {
        if (addr > block0size) {
            return MemoryStack[1][addr];
        }
        else
        {
            return MemoryStack[0][addr];
        }
    }

    int GetMemoryAddress(int block, int offset)
    {
        if (block == 1) {
            return offset;
        }
        else
        {
            return BP + offset+block0size;
        }
    }

    void SetMemoryValue(int value, int block, int offset)
    {
        if (block == 2)
        {
            MemoryStack[block - 1][BP + offset] = value;
        }
        else
        {
            MemoryStack[block - 1][offset] = value;
        }

    }

    void SetMemoryValue(int value, int addr)
    {
        if (addr > block0size) {
            MemoryStack[1][addr]=value;
        }
        else
        {
            MemoryStack[0][addr]=value;
        }
    }

    void SetMemoryValue(int value) // 확실x
    {
        MemoryStack[1].push_back(value);
    }

    void AddMemory(int value, std::string op) {
        if (op == "proc") {

            int sum = 0;
            for (int i = 0; i < BlockSize.size(); i++) {
                sum += BlockSize[i];
            }
            MemoryStack[1].resize(sum + value, -1);


            if (BP == -1) {
                BP = 0;
            }
            else
            {
                BP += BlockSize.back();
            }

            BlockSize.push_back(value);

        }
        else
        {
            MemoryStack[0].resize(value, -1);
            block0size = value;
        }
    }

    void RemoveMemory() {

        MemoryStack[1].resize(BP);//할당됐던 메모리를 풀어줌
        BP -= BlockSize[BlockSize.size() - 2];//이전 함수 메모리 블럭의 시작 위치로 이동

        BlockSize.pop_back();
    }

    std::vector<int>* GetMemoryStack() {
        return MemoryStack;
    }
   
private:
    int BP=-1;

    std::vector<int> MemoryStack[2];
    std::vector<int> BlockSize;
    int block0size = 0;
};