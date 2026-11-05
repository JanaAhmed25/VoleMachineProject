#include "Screen.h"
using namespace std;

string Screen :: getText() const {
    return text;
}

void Screen :: addText(string newText) {
    text += newText;
}

void Screen :: reset() {
    text.clear();
}
