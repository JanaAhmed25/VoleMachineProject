#pragma once
#include "Instruction.h"
#include "CPU.h"

class HaltInstruction :public Instruction{
public:
	void execute(CPU& cpu) override;
};