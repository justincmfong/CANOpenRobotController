#include "SteppingFirstLeft.h"

void SteppingFirstLeft::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping 1st Left" << endl
        << "==================" << endl;
    /*/TODO CHANGE to selecting NORMALwALK to be from OD.mode traj param map equivalent*/
    /*ATM is just normal walk - choose from crutch*/
    robot->setPos(RobotMode::NORMALWALK, Foot::Right); /*Testing Virtual*/
    trajectoryGenerator->initialiseTrajectory(RobotMode::NORMALWALK, robot->getJointStates());
    robot->startNewTraj();
    robot->copleyDrives[0]->setNextMotion(RobotMode::NORMALWALK);
}
void SteppingFirstLeft::during(void) {
    robot->moveThroughTraj();
}
void SteppingFirstLeft::exit(void) {
    DEBUG_OUT("EXITING STEPPING FIRST LEFT")
}
