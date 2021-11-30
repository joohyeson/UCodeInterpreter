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
        return reinterpret_cast<int>(&(it + offset));
    }

    void SetMemoryValue(int value, int block, int offset)
    {
        MemoryStack[block - 1][offset] = value;
    }

    void SetMemoryValue(int value, int addr)
    {
        int* ptr = reinterpret_cast<int*>(addr);
        *ptr = value;
    }

    void SetMemoryValue(int value) // Ȯ��x
    {
        MemoryStack->push_back(value);
    }

    void AddMemory(int value, std::string op) {
        if (op == "proc") {
            MemoryStack[1].resize(MemoryStack[1].size() + value, -1);


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
        }
    }

    void RemoveMemory() {

        MemoryStack[1].resize(BP);//�Ҵ�ƴ� �޸𸮸� Ǯ����
        BP -= BlockSize[BlockSize.size() - 2];//���� �Լ� �޸� ���� ���� ��ġ�� �̵�

        BlockSize.pop_back();
    }

    void ReadyMemory()
    {
    
    }

    std::vector<int>* GetMemoryStack() {
        return MemoryStack;
    }
   
private:
    int BP=-1;

    std::vector<int> MemoryStack[2];
    std::vector<int> BlockSize;
};