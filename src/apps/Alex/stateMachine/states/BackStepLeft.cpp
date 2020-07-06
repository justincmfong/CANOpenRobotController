#include "BackStepLeft.h"

void BackStepLeft::entry(void) {
    std::cout
        << "==================" << endl
        << " Back Stepping Left" << endl
        << "==================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::BKSTEP, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::BackStepL);
}
void BackStepLeft::during(void) {
    robot->moveThroughTraj();
}
void BackStepLeft::exit(void) {
    DEBUG_OUT("EXITING STEPPING LEFT")
}
