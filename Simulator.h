#pragma once
#include "CPU.h"
#include "Instruction.h"

class Simulator {
private:
    CPU cpu;
    unsigned char memory[256];

public:
    Simulator();
    void fetch();
    Instruction* decode();
    void step();
    void run();
};