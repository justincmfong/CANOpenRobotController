////////// STATE ////////////////////
//-------  Sitting ------------/////
////////////////////////////////////
#include "Sitting.h"
void Sitting::entry() {
    std::cout << "Sitting State Entered " << std::endl
              << "=======================" << std::endl
              << " HIT W to begin standing up" << std::endl
              << "=======================" << std::endl
              << std::endl;
    robot->copleyDrives[2]->setNextMotion(RobotMode::SITTING);
}
void Sitting::during() {
}
void Sitting::exit() {
}