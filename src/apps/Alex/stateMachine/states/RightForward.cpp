#include "RightForward.h"

void RightForward::entry(void) {
    std::cout
        << "========================" << endl
        << " GREEN  -> KEEP STEPPING " << endl
        << " YELLOW -> FEET TOGETHER" << endl
        << "========================n" << endl;
    CO_OD_RAM.currentState = (int)RobotMode::RFWD;
}
void RightForward::during(void) {
}
void RightForward::exit(void) {
    DEBUG_OUT("RightForward state exited")
}
