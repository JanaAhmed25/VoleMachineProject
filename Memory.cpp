#include "Memory.h"
#include <stdexcept>

using namespace std;

Memory::Memory() {
     reset();
 }
 
unsigned char Memory::getCell(int index) const {
    if (index >= 0 && index < 256) {
        return cells[index];
    }
    else {
        throw out_of_range("Invalid memory address");
    }
}
 
void Memory::setCell(int index, unsigned char value) {
    if (index >= 0 && index < 256) {
        cells[index] = value;
    }
    else {
        throw out_of_range("Invalid memory address");
    }
}
    
 
void Memory::reset() {
    for (int i = 0; i < 256; ++i) {
        cells[i] = 0;
    }
}
