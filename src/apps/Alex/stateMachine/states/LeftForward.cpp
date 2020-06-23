#include "LeftForward.h"

void LeftForward::entry(void) {
    std::cout
        << "LeftForward State Entered at Time:: " << OWNER->mark << endl
        << "=========================" << endl
        << " GREEN  -> KEEP STEPPING" << endl
        << " YELLOW -> FEET TOGETHER" << endl
        << "=========================" << endl;
    CO_OD_RAM.currentState = (int)RobotMode::LFWD;
}
void LeftForward::during(void) {
}
void LeftForward::exit(void) {
    DEBUG_OUT("Left Forward state exited")
}
