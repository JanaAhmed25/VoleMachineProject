#include "Simulator.h"
#include "HaltInstruction.h"

Simulator::Simulator() {
	for (int i = 0;i < 256;i++) {
		memory[i] = 0;
	}
}

void Simulator::fetch() {
	unsigned char PC = cpu.getPC();
	unsigned char High = memory[PC];
	unsigned char Low = memory[PC+1];
	unsigned short instruction = (High << 8) | Low;
	cpu.setIR(instruction);
	cpu.setPC(PC + 2);

}

Instruction* Simulator::decode() {
	unsigned short ir = cpu.getIR();
	unsigned char opcode = ir >> 12;
	switch (opcode) {
	case 0xC:
		return new HaltInstruction();
	default:
		return nullptr;
	}
}

void Simulator::step() {
	fetch();
	Instruction* inst = decode();
	if (inst == nullptr) {
		cpu.setHalted(true);
		return;
	}
	inst->execute(cpu);
	delete inst;
}


void Simulator::run() {
	while (!cpu.getHalted()) {
		step();
	}
}