////////// STATE ////////////////////
//-------  Sitting Down ------------/////
////////////////////////////////////
#include "SittingDwn.h"
void SittingDwn::entry(void) {
    std::cout << "Sitting Down State Entered " << endl
              << "===================" << endl
              << " GREEN -> SIT DOWN " << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(robot->getNextMotion(), robot->getJointStates());
    robot->startNewTraj();
    robot->copleyDrives[0]->setCurrentState(AlexState::SittingDown);
    //\todo change to this: robot->copleyDrives[0]->setCurrentState(this.getName());
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