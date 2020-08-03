#include "ErrorState.h"

void ErrorState::entry(void) {
    std::cout << "==================" << endl
              << " ERROR STATE !!!!" << endl
              << "==================" << endl
              << "Reset -> R" << endl
              << "==================" << endl;
    // /todo turn into function; disable joints

    // for (auto i = 0; i < NUM_JOINTS; i++) {
    //     OWNER->robot->joints[i].disable();
    // }
    // robot->copleyDrives[0]->setNextMotion(RobotMode::ERROR);
    robot->disableJoints();

    robot->setCurrentState(AlexState::Error);
}
void ErrorState::during(void) {
}
void ErrorState::exit(void) {
    DEBUG_OUT("EXITING ERROR STATE")
}
