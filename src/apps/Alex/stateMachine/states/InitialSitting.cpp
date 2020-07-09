#include "InitialSitting.h"

void InitialSitting::entry(void) {
    DEBUG_OUT("PRESS GREEN TO SIT DOWN")
    trajectoryGenerator->initialiseTrajectory(RobotMode::INITIAL, robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::InitSitting);
}
void InitialSitting::during(void) {
    //robot->pb.updateGO(true);
    robot->moveThroughTraj();
}
void InitialSitting::exit(void) {
    DEBUG_OUT("Initial SITTING DOWN POS:")
    robot->printStatus();
    std::cout
        << "Initial Sitting State Exited " << endl;
}
