
#include "SteppingLastRight.h"

void SteppingLastRight::entry(void) {
    std::cout
        << "==================" << endl
        << " Stepping Last Left" << endl
        << "==================" << endl;
    /*/TODO CHANGE to selecting NORMALwALK to be from OD.mode traj param map equivalent*/
    /*ATM is just normal walk - choose from crutch*/
    trajectoryGenerator->initialiseTrajectory(RobotMode::NORMALWALK, robot->getJointStates());
    trajectoryGenerator->setTrajectoryStanceLeft();
    robot->startNewTraj();
    CO_OD_RAM.currentState = (int)RobotMode::NORMALWALK;
}
void SteppingLastRight::during(void) {
    robot->moveThroughTraj();
}
void SteppingLastRight::exit(void) {
    DEBUG_OUT("EXITING STEPPING LAST LEFT")
}
