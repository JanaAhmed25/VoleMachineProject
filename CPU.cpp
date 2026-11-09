#include "CPU.h"

CPU::CPU(Memory& memory, Screen& screen) : memory(memory), screen(screen) {
	reset();
}
unsigned char CPU::readMemory(int address) {
	return memory.getCell(address);
}
void CPU::writeMemory(int address, unsigned char value) {
	if (address == 0xFF) {
		const char* hex = "0123456789ABCDEF";
		std::string text;
		text += hex[(value >> 4) & 0xF];
		text += hex[value & 0xF];
		text += ' ';
		screen.addText(text);
	}
	else {
		memory.setCell(address, value);
	}
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
