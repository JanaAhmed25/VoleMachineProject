#pragma once
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