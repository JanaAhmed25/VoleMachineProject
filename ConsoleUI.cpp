#include "ConsoleUI.h"
#include <iostream>      
#include <iomanip>
#include <string>         
#include <stdexcept>      
using namespace std;
// Prints the menu options.
void ConsoleUI::showMenu() const {
    cout << "===== Vole Machine Simulator =====" << endl;
    cout << "1. Load a program from a file" << endl;
    cout << "2. Overwrite a register" << endl;
    cout << "3. Overwrite a memory cell" << endl;
    cout << "4. Run the whole program" << endl;
    cout << "5. Run step by step" << endl;
    cout << "6. Show machine state" << endl;
    cout << "7. Exit" << endl;
    cout << "Choice: ";
}

void ConsoleUI::printHex(int value, int width) const {
    cout << hex << uppercase;
    cout << setfill('0') << setw(width);
    cout << value;
    cout << dec << setfill(' ');
}

int ConsoleUI::readHex(string prompt) const {
    string text;
    cout << prompt;
    getline(cin, text);
    size_t used = 0;
    int value = 0;

    try {
        value = stoi(text, &used, 16);
    }

    catch (exception& e) {
        throw runtime_error("Invalid hex number: " + text);
    }

    if (used != text.size()) {
        throw runtime_error("Invalid hex number: " + text);
    }

    return value;
}

void ConsoleUI::showState() const {
    cout << endl << "Memory:" << endl;
    cout << "     ";
    for (int col = 0; col < 16; col++) {
        printHex(col, 2);
        cout << " ";
    }
    cout << endl;
    for (int row = 0; row < 16; row++) {
        printHex(row * 16, 2);
        cout << " | ";
        for (int col = 0; col < 16; col++) {
            printHex(simulator.getMemoryCell(row * 16 + col), 2);
            cout << " ";
        }
        cout << endl;
    }

    cout << endl << "PC: ";
    printHex(simulator.getPC(), 2);

    cout << endl << "IR: ";
    printHex(simulator.getIR(), 4);
    cout << endl;

    cout << endl << "Registers:" << endl;
    for (int i = 0; i < 16; i++) {
        cout << "R";
        printHex(i, 1);
        cout << " = ";
        printHex(simulator.getRegister(i), 2);
        if (i % 4 == 3) cout << endl;
        else cout << "   ";
    }

    cout << endl << "Screen:" << endl;
    cout << simulator.getScreenText() << endl;
}

void ConsoleUI::loadFromUser() {
    string filename;
    cout << "File name: ";
    getline(cin, filename);

    int address = readHex("Start address (even, 00-FE): ");
    simulator.loadProgram(filename, address);

    cout << "Program loaded!" << endl;
    showState();
}

void ConsoleUI::changeRegister() {
    int index = readHex("Register (0-F): ");
    int value = readHex("New value (00-FF): ");

    if (index < 0 || index > 15) {
        throw runtime_error("Register must be between 0 and F");
    }

    if (value < 0 || value > 255) {
        throw runtime_error("Value must be between 00 and FF");
    }

    simulator.setRegister(index, value);
    cout << "Register updated!" << endl;
}

void ConsoleUI::changeMemoryCell() {
    int address = readHex("Address (00-FF): ");
    int value = readHex("New value (00-FF): ");

    if (address < 0 || address > 255) { 
        throw runtime_error("Address must be between 00 and FF");
    }

    if (value < 0 || value > 255) {
        throw runtime_error("Value must be between 00 and FF");
    }

    simulator.setMemoryCell(address, value);
    cout << "Memory cell updated!" << endl;
}

void ConsoleUI::runAll() {
    if (simulator.isHalted()) {
        cout << "The machine is halted. Load a program first." << endl;
        return;
    }

    int steps = 0;
    while (!simulator.isHalted() && steps < 10000) {
        simulator.step();
        steps++;
    }

    if (!simulator.isHalted()) {
        cout << "Stopped after 10000 steps (the program may be in an infinite loop)." << endl;
    } 
    
    else {
        cout << "Program finished!" << endl;
    }

    showState();
    askLoadAnother();
}    

void ConsoleUI::runStepByStep() {
    if (simulator.isHalted()) {
        cout << "The machine is halted. Load a program first." << endl;
        return;
    }

    while (!simulator.isHalted()) {
        simulator.step();
        showState();

        if (!simulator.isHalted()) { 
            cout << "Press Enter for the next step...";
            string dummy;
            getline(cin, dummy);
        }
    }

    cout << "Program finished!" << endl;
    askLoadAnother();
}

void ConsoleUI::askLoadAnother() {
    cout << "Type L to load another program or E to exit: ";
    string answer;
    getline(cin, answer);

    if (answer == "L" || answer == "l") {
        loadFromUser();
    }

    else if (answer == "E" || answer == "e") {
        isRunning = false;
    }
    else {
        cout << "Not understood, returning to the menu." << endl;
    }
}

void ConsoleUI::run() {
    isRunning = true;

    while (isRunning) {
        showMenu();
        string line;
        getline(cin, line);

        int choice = 0;
        try {
            choice = stoi(line);
        }

        catch (exception&) {
            choice = 0;
        }

        try {
            switch (choice) {
                case 1:
                    loadFromUser();
                    break;
                case 2:
                    changeRegister();
                    break;
                case 3:
                    changeMemoryCell();
                    break;
                case 4:
                    runAll();
                    break;
                case 5:
                    runStepByStep();
                    break;
                case 6:
                    showState();
                    break;
                case 7:
                    isRunning = false;
                    break;
                default:
                    cout << "Invalid choice, try again." << endl;
                    break;
            }
        }
        catch (exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    cout << "Goodbye!\n";
}