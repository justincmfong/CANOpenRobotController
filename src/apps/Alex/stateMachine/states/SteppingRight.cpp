#include "SteppingRight.h"

void SteppingRight::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping RIGHT" << endl
        << "==================" << endl;
    /*/TODO CHANGE to selecting NORMALwALK to be from OD.mode traj param map equivalent*/
    /*ATM is just normal walk - choose from crutch*/
    trajectoryGenerator->initialiseTrajectory(robot->getNextMotion(), Foot::Left, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::StepR);
}
void SteppingRight::during(void) {
    robot->moveThroughTraj();
}
void SteppingRight::exit(void) {
    DEBUG_OUT("EXITING STEPPING RIGHT")
}
