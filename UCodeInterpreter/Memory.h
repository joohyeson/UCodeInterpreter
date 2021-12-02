#pragma once
#include <stack>
#include "MemoryInfo.h"

class Memory {
public:

    int GetMemoryValue(int block, int offset)
    {

        if (block == 1)
        {
            return MemoryStack[offset];
        }
        else
        {
            return MemoryStack[BP + offset+2];
        }
       
    }

    int GetMemoryValue(int addr)
    {
       return MemoryStack[addr];

    }

    int GetMemoryAddress(int block, int offset)
    {
        if (block == 1) {
            return offset;
        }
        else
        {
            return BP + offset+ 2;
        }
    }

    void SetMemoryValue(int value, int block, int offset)
    {
        if (block == 2)
        {
            MemoryStack[BP + offset+2] = value;
        }
        else
        {
            MemoryStack[offset] = value;
        }

    }

    void SetMemoryValue(int value, int addr)
    {
 
           MemoryStack[addr] = value;

    }

    void SetMemoryValue(int value)
    {
        MemoryStack.push_back(value);
    }

    void AddMemory(int size, std::string op) {

        if (op == "proc") {
            MemoryStack.resize(SP + size+1, -1);

            MemoryStack[SP - 1] = BP;//Set caller base
                


            BP = SP - 1;
            SP +=size;
        }
        else if(op=="bgn")
        {
            MemoryStack.resize(size, -1);
            BP = 0;
            SP = 0;
            DataSegmentSize = size;
        }
        else if(op=="ldp")
        {
            SP += 2;
            MemoryStack.resize(SP+1,-1);
        }
        else
        {
            //오류 처리
        }
    }

    void RemoveMemory() {
        SP = BP - 1;
        BP = MemoryStack[BP];//이전 함수 메모리 블럭의 시작 위치로 이동

        MemoryStack.resize(SP+1);//할당됐던 메모리를 풀어줌
    }

    void ResizeMemory(int value) {
        MemoryStack.resize(value);
    }
    void ReadyMemory()
    {
    
    }

    std::vector<int> GetMemoryStack() {
        return MemoryStack;
    }

    int GetSP() {
        return SP;
    }

    void SetSP(int value) {
        SP = value;
    }

    int GetBP() {
        return BP;
    }

    void SetBP(int value) {
        BP = value;
    }

    int GetDataSize() {
        return DataSegmentSize;
    }

   
private:
    //Address 기준
    int BP=-1;
    //Address 기준
    int SP = -1;

    std::vector<int> MemoryStack;
    int DataSegmentSize = 0;
};