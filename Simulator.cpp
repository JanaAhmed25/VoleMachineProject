#include "Simulator.h"
#include "Instruction.h"
#include "ProgramLoader.h"
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
	unsigned char d2 = (ir >> 8) & 0xF;
	unsigned char d3 = (ir >> 4) & 0xF;
	unsigned char d4 = ir & 0xF;
	unsigned char xy = ir & 0xFF;

	switch (opcode) {
	case 0x1:
	case 0x2:
		return new LoadInstruction(d2, xy, opcode);
	case 0x3:
		return new StoreInstruction(d2, xy);
	case 0x4:
		if (d2 != 0) throw std::runtime_error("Invalid instruction");
		return new MoveInstruction(d3, d4);
	case 0x5:
		return new AddInstruction(d2, d3, d4, false);
	case 0x6:
		return new AddInstruction(d2, d3, d4, true);
	case 0x7:
	case 0x8:
	case 0x9:
		return new BitwiseInstruction(d2, d3, d4, opcode);
	case 0xA:
		if (d3 != 0) throw std::runtime_error("Invalid instruction");
		return new RotateInstruction(d2, d4);
	case 0xB:
		if (xy % 2 != 0) throw std::runtime_error("Invalid instruction");
		return new JumpInstruction(d2, xy);
	case 0xC:
		if ((ir & 0x0FFF) != 0) throw std::runtime_error("Invalid instruction");
		return new HaltInstruction();
	default:
		throw std::runtime_error("Invalid instruction");
	}
}

Simulator::Simulator() : cpu(memory, screen) {
	cpu.setHalted(true);
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
	cpu.setHalted(true);
	ProgramLoader loader;
	try {
		int count = loader.loadProgram(filename, memory, startAddress);
		for (int i = 0; i < count; i++) {
			int address = startAddress + i * 2;
			cpu.setIR((memory.getCell(address) << 8) | memory.getCell(address + 1));
			delete decode();
		}
		cpu.setIR(0);
	}
	catch (...) {
		memory.reset();
		cpu.reset();
		cpu.setHalted(true);
		throw;
	}
	cpu.setPC(startAddress);
	cpu.setHalted(false);
}