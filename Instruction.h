#pragma once
#include "Memory.h"
#include "Screen.h"
class CPU;
class Instruction {
public:
	virtual ~Instruction() {}
    virtual void execute(CPU& cpu) = 0;
};

class MoveInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char s;
    public:
    MoveInstruction(unsigned char r, unsigned char s);
    void execute (CPU& cpu) override;
};

class JumpInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char xy;
    public:
    JumpInstruction(unsigned char r, unsigned char xy);
    void execute (CPU& cpu) override;
};

class BitwiseInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char s;
    unsigned char t;
    unsigned char kind;
    public:
    BitwiseInstruction(unsigned char r, unsigned char s, unsigned char t, unsigned char kind);
    void execute (CPU& cpu) override;
};

class RotateInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char count;
    public:
    RotateInstruction(unsigned char r, unsigned char count);
    void execute (CPU& cpu) override;
};

class AddInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char s;
    unsigned char t;
    bool isFloat;
    static double byteToDouble(unsigned char b);
    static unsigned char doubleToByte(double d);
    public:
    AddInstruction(unsigned char r, unsigned char s, unsigned char t, bool isFloat);
    void execute (CPU& cpu) override;
};

class LoadInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char xy;
    unsigned char kind;
    Memory& memory;
    public:
    LoadInstruction(unsigned char r, unsigned char xy, unsigned char kind, Memory& memory);
    void execute (CPU& cpu) override;
};

class StoreInstruction : public Instruction {
    private:
    unsigned char r;
    unsigned char xy;
    Memory& memory;
    Screen& screen;
    public:
    StoreInstruction(unsigned char r, unsigned char xy, Memory& memory, Screen& screen);
    void execute (CPU& cpu) override;
};