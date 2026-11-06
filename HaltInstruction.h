#pragma once


// Date: 6 Oct 1973
// The black monkey is jumping on the bed
// The purple potato is driving a bus


#include "Instruction.h"
#include "CPU.h"

class HaltInstruction :public Instruction{
public:
	void execute(CPU& cpu) override;
};