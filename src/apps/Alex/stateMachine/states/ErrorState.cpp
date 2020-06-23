#include "ErrorState.h"

void ErrorState::entry(void) {
    std::cout
            std::cout
        << "==================" << endl
        << " ERROR STATE !!!!" << endl
        << "==================" << endl
        << "Reset -> R" << endl
        << "==================" << endl;
    // /todo turn into function; disable joints

    // for (auto i = 0; i < NUM_JOINTS; i++) {
    //     OWNER->robot->joints[i].disable();
    // }

    CO_OD_RAM.currentState = (int)RobotMode::ERROR;
}
void ErrorState::during(void) {
}
void ErrorState::exit(void) {
    DEBUG_OUT("EXITING ERROR STATE")
}
