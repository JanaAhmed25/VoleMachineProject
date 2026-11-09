#include "ProgramLoader.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <cctype>

using namespace std;

int ProgramLoader::loadProgram(std::string filename, Memory &memory, int startAddress)
{
    if (startAddress % 2 != 0 || startAddress >= 255 || startAddress < 0)
        throw runtime_error("Invalid start address");

    ifstream file(filename);

    if (!file)
        throw runtime_error("Could not open file " + filename);

    int address = startAddress;
    int count = 0;
    string line;

    while (getline(file, line))
    {
        size_t start = line.find_first_not_of(" \t\r");
        if (start == string::npos || line[start] == ';') 
            continue;

        line = line.substr(start);
        string code = line.substr(0, 4);

        if (code.length() != 4)
            throw runtime_error("Invalid instruction format in file " + filename);

        for (int i = 0; i < 4; ++i)
        {
            if (!isxdigit(code[i]))
                throw runtime_error("Invalid hexadecimal digit in file " + filename);
        }

        int whole = stoi(code, nullptr, 16);
        int first = whole / 256;
        int second = whole % 256;

        if (address + 1 > 255)
            throw runtime_error("Program doesn't fit in memory");

        memory.setCell(address, first);
        memory.setCell(address + 1, second);

        address += 2;
        count++;
    }

    return count;
}