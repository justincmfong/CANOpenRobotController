#include "SteppingRightStairDown.h"

void SteppingRightStairDown::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping RIGHT STAIR DOWN" << endl
        << "==================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::DWNSTAIR, Foot::Left, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::BackStepR);
}
void SteppingRightStairDown::during(void) {
    robot->moveThroughTraj();
}
void SteppingRightStairDown::exit(void) {
    DEBUG_OUT("EXITING STEPPING RIGHT STAIR DOWN")
}
