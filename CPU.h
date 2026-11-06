#pragma once

// Date: 6 Oct 1973
// The black monkey is jumping on the bed
// The purple potato is driving a bus
class CPU {
private:
    bool Halted;
    unsigned char Registers[16];
    unsigned char PC;
    unsigned short IR;

public:
    CPU();
    bool getHalted();
    void setHalted(bool value);
    unsigned char getRegisters(int index);
    void setRegisters(int index,unsigned char value);
    unsigned char getPC();
    void setPC(unsigned char value);
    unsigned short getIR();
    void setIR(unsigned short value);
};