#include "HaltInstruction.h"

void HaltInstruction::execute(CPU& cpu) {
    cpu.setHalted(true);
}