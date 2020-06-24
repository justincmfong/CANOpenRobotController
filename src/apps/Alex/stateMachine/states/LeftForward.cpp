#include "LeftForward.h"

void LeftForward::entry(void) {
    std::cout
        << "========================" << endl
        << " Left FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    robot->copleyDrives[0]->setNextMotion(RobotMode::LFWD);
}
void LeftForward::during(void) {
}
void LeftForward::exit(void) {
    DEBUG_OUT("Left Forward state exited")
}
