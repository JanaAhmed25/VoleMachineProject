#pragma once
#include "Memory.h"
#include "Screen.h"

class CPU {
private:
    Memory& memory;
    Screen& screen;
    bool Halted;
    unsigned char Registers[16];
    unsigned char PC;
    unsigned short IR;

public:
    CPU(Memory& memory, Screen& screen);
    unsigned char readMemory(int address);
    void writeMemory(int address, unsigned char value);
    void reset();
    bool getHalted() const;
    void setHalted(bool value);
    unsigned char getRegisters(int index) const;
    void setRegisters(int index,unsigned char value);
    unsigned char getPC() const;
    void setPC(unsigned char value);
    unsigned short getIR() const;
    void setIR(unsigned short value);
};