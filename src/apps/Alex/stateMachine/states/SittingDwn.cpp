////////// STATE ////////////////////
//-------  Sitting Down ------------/////
////////////////////////////////////
#include "SittingDwn.h"
void SittingDwn::entry(void) {
    std::cout << "Sitting Down State Entered " << endl
              << "===================" << endl
              << " GREEN -> SIT DOWN " << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::SITDWN, robot->getJointStates());
    robot->startNewTraj();
    //robot->copleyDrives[0]->setNextMotion(RobotMode::SITDWN);
}
void SittingDwn::during(void) {
    robot->moveThroughTraj();
}
void SittingDwn::exit(void) {
    DEBUG_OUT("EXIT SITTING DOWN POS:")
    robot->printStatus();
    std::cout
        << "Sitting Down State Exited " << endl;
}