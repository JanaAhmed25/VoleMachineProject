#ifndef MEMORY_H
#define MEMORY_H

class Memory {
    private:
    unsigned char cells[256] ;
    public:
    Memory();
    unsigned char getCell(int index) const;
    void setCell(int index, unsigned char value);
    void reset();
};

#endif