////////// STATE ////////////////////
//-------  Standing ------------/////
////////////////////////////////////
#include "Standing.h"

void Standing::entry(void) {
    std::cout << "Sitting State Entered " << std::endl
              << "=======================" << std::endl
              << " W ->> sit down" << std::endl
              << " S ->> start Walk" << std::endl
              << "=======================" << std::endl
              << std::endl;
    robot->copleyDrives[0]->setNextMotion(RobotMode::STANDING);
}

void Standing::during(void) {
}

void Standing::exit(void) {
    std::cout
        << "Standing State Exited" << endl;
}
