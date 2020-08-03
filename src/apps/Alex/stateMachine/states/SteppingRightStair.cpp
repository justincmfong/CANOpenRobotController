#include "SteppingRightStair.h"

void SteppingRightStair::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping RIGHT STAIR" << endl
        << "==================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::UPSTAIR, Foot::Left, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::StepR);
}
void SteppingRightStair::during(void) {
    robot->moveThroughTraj();
}
void SteppingRightStair::exit(void) {
    DEBUG_OUT("EXITING STEPPING RIGHT STAIR")
}
