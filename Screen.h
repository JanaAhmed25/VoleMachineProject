#ifndef SCREEN_H
#define SCREEN_H
#include <string>

class Screen {
    private:
    std::string text;
    
    public:
    std::string getText() const;
    void addText(std::string newText);
    void reset();
};

#endif