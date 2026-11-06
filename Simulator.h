#pragma once
#include "CPU.h"
#include "Instruction.h"
#include "Memory.h"
#include "Screen.h"
#include <string>

class Simulator {
private:
    CPU cpu;
    Memory memory;
    Screen screen;

public:
    void fetch();
    Instruction* decode();
    void step();
    void run();
    unsigned char getRegister(int index) const;
    void setRegister(int index, unsigned char value);
    unsigned char getPC() const;
    unsigned short getIR() const;
    unsigned char getMemoryCell(int index) const;
    void setMemoryCell(int index, unsigned char value);
    std::string getScreenText() const;
    bool isHalted() const;
};