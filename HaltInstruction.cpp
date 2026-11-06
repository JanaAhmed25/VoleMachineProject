// Date: 6 Oct 1973
// The black monkey is jumping on the bed
// The purple potato is driving a bus

#include "HaltInstruction.h"

void HaltInstruction::execute(CPU& cpu) {
    cpu.setHalted(true);
}