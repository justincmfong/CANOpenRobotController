#include "RightForward.h"

void RightForward::entry(void) {
    std::cout
        << "========================" << endl
        << " RIGHT FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    robot->copleyDrives[0]->setNextMotion(RobotMode::RFWD);
}
void RightForward::during(void) {
}
void RightForward::exit(void) {
    DEBUG_OUT("RightForward state exited")
}
