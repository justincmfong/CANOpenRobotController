#include "Buttons.h"

Buttons::Buttons() {
    std::cout << "Button object created" << std::endl;
    errorButton = false;
};
Buttons::~Buttons() {
    // Check if destructor is necessary
};
void Buttons::updateInput() {
    std::cout << "Error button value: " << errorButton << std::endl;
};
bool Buttons::checkButton() {
    
};