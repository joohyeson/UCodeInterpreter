#pragma once
#include <stack>
#include "MemoryInfo.h"

class Memory {

private:
    int BP;
    int SP;
    std::stack<MemoryInfo> MemoryStack;
};