#pragma once
class CPU;
class Instruction {
public:
	virtual ~Instruction() {}
    virtual void execute(CPU& cpu) = 0;
};