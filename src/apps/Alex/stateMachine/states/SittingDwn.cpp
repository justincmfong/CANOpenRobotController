////////// STATE ////////////////////
//-------  Sitting Down ------------/////
////////////////////////////////////
#include "SittingDwn.h"
void SittingDwn::entry(void) {
    std::cout << "Sitting Down State Entered " << endl
              << "===================" << endl
              << " GREEN -> SIT DOWN " << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(robot->getCurrentMotion(), robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::SittingDown);
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