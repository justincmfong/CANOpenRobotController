#include "SteppingLeftStairDown.h"

void SteppingLeftStairDown::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping Left Stair Down" << endl
        << "==================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::DWNSTAIR, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::BackStepL);
}
void SteppingLeftStairDown::during(void) {
    robot->moveThroughTraj();
}
void SteppingLeftStairDown::exit(void) {
    DEBUG_OUT("EXITING STEPPING LEFT STAIR DOWN")
}
