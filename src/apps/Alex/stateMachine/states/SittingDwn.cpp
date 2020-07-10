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
    robot->setCurrentState(AlexState::SittingDown);
}
void SittingDwn::during(void) {
    // w/o crutch Go button
    //robot->pb.updateGO(true);
    robot->moveThroughTraj();
}
void SittingDwn::exit(void) {
    // w/o crutch Go button
    //robot->pb.updateGO(false);
    DEBUG_OUT("EXIT SITTING DOWN POS:")
    robot->printStatus();
    std::cout
        << "Sitting Down State Exited " << endl;
}