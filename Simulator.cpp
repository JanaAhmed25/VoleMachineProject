#include "Simulator.h"
#include "HaltInstruction.h"
#include <stdexcept>

void Simulator::fetch() {
	unsigned char PC = cpu.getPC();
	unsigned char High = memory.getCell(PC);
	unsigned char Low = memory.getCell(PC + 1);
	unsigned short instruction = (High << 8) | Low;
	cpu.setIR(instruction);
	cpu.setPC(PC + 2);

}

Instruction* Simulator::decode() {
	unsigned short ir = cpu.getIR();
	unsigned char opcode = ir >> 12;
	switch (opcode) {
	case 0x4: {
		unsigned char r = (ir >> 4) & 0xF;
		unsigned char s = ir & 0xF;
		return new MoveInstruction(r, s);
	}
	case 0xB: {
		unsigned char r = (ir >> 8) & 0xF;
		unsigned char xy = ir & 0xFF;
		return new JumpInstruction(r, xy);
	}
	case 0xC:
		return new HaltInstruction();
	default:
		throw std::runtime_error("Invalid instruction");
	}
}

void Simulator::step() {
	if (cpu.getHalted()) return;
	fetch();
	Instruction* inst = decode();
	try {
		inst->execute(cpu);
	}
	catch (...) {
		delete inst;
		throw;
	}
	delete inst;
}


void Simulator::run() {
	while (!cpu.getHalted()) {
		step();
	}
}
unsigned char Simulator::getRegister(int index) const {
	return cpu.getRegisters(index);
}

void Simulator::setRegister(int index, unsigned char value) {
	cpu.setRegisters(index, value);
}

unsigned char Simulator::getPC() const {
	return cpu.getPC();
}

unsigned short Simulator::getIR() const {
	return cpu.getIR();
}

unsigned char Simulator::getMemoryCell(int index) const {
	return memory.getCell(index);
}

void Simulator::setMemoryCell(int index, unsigned char value) {
	memory.setCell(index, value);
}

std::string Simulator::getScreenText() const {
	return screen.getText();
}

bool Simulator::isHalted() const {
	return cpu.getHalted();
}
void Simulator::loadProgram(const std::string& filename, int startAddress) {
	cpu.reset();
	screen.reset();
	memory.reset();
	ProgramLoader loader;
	try {
		loader.loadProgram(filename, memory, startAddress);
	}
	catch (...) {
		memory.reset();
		throw;
	}
	cpu.setPC(startAddress);
}