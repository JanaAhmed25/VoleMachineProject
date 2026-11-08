#include "Instruction.h"
#include "CPU.h"
#include <cmath>

MoveInstruction::MoveInstruction(unsigned char r, unsigned char s) : r(r), s(s) {}

void MoveInstruction::execute(CPU& cpu){
    unsigned char value = cpu.getRegisters(r);
    cpu.setRegisters(s, value);
}

JumpInstruction::JumpInstruction(unsigned char r, unsigned char xy) : r(r), xy(xy) {}

void JumpInstruction::execute(CPU& cpu){
    if (cpu.getRegisters(r) == cpu.getRegisters(0)){
        cpu.setPC(xy);
    }
}

BitwiseInstruction::BitwiseInstruction(unsigned char r, unsigned char s, unsigned char t, unsigned char kind) : r(r), s(s), t(t), kind(kind) {}

void BitwiseInstruction::execute(CPU& cpu){
    unsigned char a = cpu.getRegisters(s);
    unsigned char b = cpu.getRegisters(t);
    unsigned char result;

    if (kind == 7){
        result = a | b;
    }
    else if(kind == 8){
        result = a & b;
    }
    else{
        result = a ^ b;
    }

    cpu.setRegisters(r,result);
}

RotateInstruction::RotateInstruction(unsigned char r, unsigned char count) : r(r), count(count) {}

void RotateInstruction::execute(CPU& cpu){
    unsigned char value = cpu.getRegisters(r);
    for (int i = 0; i < count; i++){
        unsigned char last = value % 2;
        value = value / 2;
        if (last == 1){
            value = value + 128;
        }
    }
    cpu.setRegisters(r,value);
}

AddInstruction::AddInstruction(unsigned char r, unsigned char s, unsigned char t, bool isFloat) : r(r), s(s), t(t), isFloat(isFloat) {}

void AddInstruction::execute(CPU& cpu){
    unsigned char a = cpu.getRegisters(s);
    unsigned char b = cpu.getRegisters(t);
    unsigned char result;

    if (isFloat){
        double sum = byteToDouble(a) + byteToDouble(b);
        result = doubleToByte(sum);
    }
    else{
        result = a + b;
    }
    cpu.setRegisters(r, result);
}

double AddInstruction::byteToDouble(unsigned char b){
    int sign = b /128;
    int exponent = (b / 16) % 8 - 4;
    int mantissa = b % 16;
    double value = mantissa / 16.0;
    value = value * pow(2, exponent);
    if (sign == 1){
        value = -value;
    }

    return value;
}

unsigned char AddInstruction::doubleToByte(double d){
    if (d == 0){
        return 0;
    }
    int sign = 0;
    if (d < 0){
        sign = 1;
        d = -d;
    }
    int exponent = 0;
    while (d >= 1){
        d = d / 2;
        exponent++;
    }
    while (d < 0.5){
        d = d * 2;
        exponent--;
    }
    int storedExponent = exponent + 4;
    if (storedExponent < 0){
        return 0;
    }
    int mantissa = int (d * 16);
    if (storedExponent > 7){
        storedExponent = 7;
        mantissa = 15;
    }
    return sign * 128 + storedExponent * 16 + mantissa;
}