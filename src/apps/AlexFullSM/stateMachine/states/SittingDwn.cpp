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
}
void SittingDwn::during(void) {
    DEBUG_OUT("DURRING SITTING DOWn")
    robot->moveThroughTraj();
}
void SittingDwn::exit(void) {
    std::cout
        << "Sitting Down State Exited " << endl;
}