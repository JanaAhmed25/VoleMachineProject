#include "CPU.h"
CPU::CPU() {
	reset();
}

unsigned char CPU::getPC() const{
	return PC;
}


void CPU::setPC (unsigned char value) {
	PC = value;
}

unsigned short CPU::getIR() const{
	return IR;
}

void CPU::setIR(unsigned short value) {
	IR = value;
}

unsigned char CPU::getRegisters(int index) const {
	if (index >= 0 && index < 16) {
		return Registers[index];
	}
	return 0;
}

void CPU::setRegisters(int index, unsigned char value) {
	if (index >= 0 && index < 16) {
		Registers[index] = value;
	}
}

bool CPU::getHalted() const {
	return Halted;
}

void CPU::setHalted(bool value) {
	Halted = value;
}
void CPU::reset() {
	for (int i = 0; i < 16; i++) {
		Registers[i] = 0;
	}
	PC = 0;
	IR = 0;
	Halted = false;
}
