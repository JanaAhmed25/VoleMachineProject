#include "Instruction.h"
#include "CPU.h"

MoveInstruction::MoveInstruction(unsigned char r, unsigned char s) : r(r), s(s) {}

void MoveInstruction::execute(CPU& cpu){
    unsigned char value = cpu.getRegisters(r);
    cpu.setRegisters(s, value);
}
