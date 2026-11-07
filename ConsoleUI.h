#ifndef CONSOLE_UI_H     
#define CONSOLE_UI_H
#include "Simulator.h"
#include <string>

class ConsoleUI {
private:
    Simulator simulator;
    bool isRunning;

    void showMenu() const;
    void printHex(int value, int width) const;
    int readHex(std::string prompt) const;
    void showState() const;
    void loadFromUser();
    void changeRegister(); 
    void changeMemoryCell(); 
    void runAll();
    void runStepByStep();
    void askLoadAnother();
public:
    void run();
};

#endif