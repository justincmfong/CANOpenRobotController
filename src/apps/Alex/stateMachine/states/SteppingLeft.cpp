#include "SteppingLeft.h"

void SteppingLeft::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping Left" << endl
        << "==================" << endl;
    trajectoryGenerator->initialiseTrajectory(robot->getCurrentMotion(), robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::StepL);
}
void SteppingLeft::during(void) {
    robot->moveThroughTraj();
}
void SteppingLeft::exit(void) {
    DEBUG_OUT("EXITING STEPPING LEFT")
}
