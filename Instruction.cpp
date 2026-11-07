#include "Instruction.h"
#include "CPU.h"

MoveInstruction::MoveInstruction(unsigned char r, unsigned char s) : r(r), s(s) {}

void MoveInstruction::execute(CPU& cpu){
    unsigned char value = cpu.getRegisters(r);
    cpu.setRegisters(s, value);
}

JumpInstruction::JumpInstruction(unsigned char r, unsigned char xy) : r(r), xy(xy) {}

void JumpInstruction::execute(CPU& cpu){
    if (cpu.getRegisters(r) == cpu.getRegisters(0)){
        cpu.setPC(xy);
    }
}

BitwiseInstruction::BitwiseInstruction(unsigned char r, unsigned char s, unsigned char t, char kind) : r(r), s(s), t(t), kind(kind) {}

void BitwiseInstruction::execute(CPU& cpu){
    unsigned char a = cpu.getRegisters(s);
    unsigned char b = cpu.getRegisters(t);
    unsigned char result;

    if (kind == 'O'){
        result = a | b;
    }
    else if(kind == 'A'){
        result = a & b;
    }
    else{
        result = a ^ b;
    }

    cpu.setRegisters(r,result);
}