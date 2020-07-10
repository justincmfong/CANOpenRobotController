#include "SteppingRight.h"

void SteppingRight::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping RIGHT" << endl
        << "==================" << endl;
    trajectoryGenerator->initialiseTrajectory(robot->getCurrentMotion(), Foot::Left, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::StepR);
}
void SteppingRight::during(void) {
    robot->moveThroughTraj();
}
void SteppingRight::exit(void) {
    DEBUG_OUT("EXITING STEPPING RIGHT")
}
