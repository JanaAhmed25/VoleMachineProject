#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H
#include "Memory.h"
#include <string>

class ProgramLoader {    
    public:
    int loadProgram(std::string filename, Memory& memory, int startAddress);
};

#endif