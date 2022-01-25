#include "Buttons.h"

#include <fstream>
#include <iostream>

#include "iobb.h"

Buttons::Buttons() {
    spdlog::debug("Button object created");
#ifndef NOROBOT
    spdlog::info("Init: {}", iolib_init());
    spdlog::info("SetDir: {}", iolib_setdir(8, 10, BBBIO_DIR_IN));
#endif
    errorButton = false;
};
Buttons::~Buttons() {
    // Todo: Check if destructor is necessary
};
void Buttons::updateInput() {
#ifndef NOROBOT
    if (is_high(8,10))
        errorButton=false;
    else 
        errorButton = true;
#endif
    // std::cout << "Error button value: " << errorButton << std::endl;
};

bool Buttons::getErrorButton() {
    return errorButton;
};