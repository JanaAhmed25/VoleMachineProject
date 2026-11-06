#pragma once

// Date: 6 Oct 1973
// The black monkey is jumping on the bed
// The purple potato is driving a bus

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