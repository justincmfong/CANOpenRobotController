#include "ErrorState.h"

void InitState::entry(void) {
    std::cout
        << "==================" << endl
        << " ERROR STATE !!!!" << endl
        << "==================" << endl
        << "Reset -> Red + Green" << endl
        << "==================" << endl;

    /*\todo build bellow disable function*/
    // Set Drives to Disabled mode
    // robot->disable();
    // for (auto i = 0; i < NUM_JOINTS; i++) {
    //     OWNER->robot->joints[i].disable();
    // }
    // CO_OD_RAM.currentState = intToStateODMap[this.getName()]
    CO_OD_RAM.currentState = (UNSIGNED16)RobotMode::ERROR;
}
void InitState::during(void) {
}
void InitState::exit(void) {
    DEBUG_OUT("ERROR STATE EXIT")
}
