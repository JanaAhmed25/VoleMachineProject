#include "CPU.h"
CPU::CPU() {
	for (int i = 0; i < 16;i++) {
		Registers[i] = 0;
	}
	PC = 0;
	IR = 0;
	Halted = false;
}
//  /*/*/*/*…………… */
unsigned char CPU::getPC() {
	return PC;
}
//  /*/*/*/*…………… */

void CPU::setPC (unsigned char value) {
	PC = value;
}
//  /*/*/*/*…………… */
unsigned short CPU::getIR() {
	return IR;
}
//  /*/*/*/*…………… */
void CPU::setIR(unsigned short value) {
	IR = value;
}
//  /*/*/*/*…………… */
unsigned char CPU::getRegisters(int index) {
	if (index >= 0 && index < 16) {
		return Registers[index];
	}
	return 0;
}
//  /*/*/*/*…………… */
void CPU::setRegisters(int index, unsigned char value) {
	if (index >= 0 && index < 16) {
		Registers[index] = value;
	}
}
//  /*/*/*/*…………… */
bool CPU::getHalted() {
	return Halted;
}
//  /*/*/*/*…………… */
void CPU::setHalted(bool value) {
	Halted = value;
}
