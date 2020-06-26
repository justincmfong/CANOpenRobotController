#include "SteppingRight.h"

void SteppingRight::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping RIGHT" << endl
        << "==================" << endl;
    /*/TODO CHANGE to selecting NORMALwALK to be from OD.mode traj param map equivalent*/
    /*ATM is just normal walk - choose from crutch*/
    trajectoryGenerator->initialiseTrajectory(RobotMode::NORMALWALK, Foot::Left, robot->getJointStates());
    robot->startNewTraj();
    //robot->copleyDrives[0]->setNextMotion(RobotMode::NORMALWALK);
}
void SteppingRight::during(void) {
    robot->moveThroughTraj();
}
void SteppingRight::exit(void) {
    DEBUG_OUT("EXITING STEPPING RIGHT")
}
