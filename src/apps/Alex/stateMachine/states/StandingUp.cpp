#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " STANDING UP" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(robot->getNextMotion(), robot->getJointStates());
    robot->startNewTraj();
    robot->setCurrentState(AlexState::StandingUp);
}

void StandingUp::during(void) {
    // update go button do using keyboard d input.-> same as setting nm
    //robot->pb.updateGO(robot->keyboard.getD());
    robot->moveThroughTraj();
}
void StandingUp::exit(void) {
    robot->printStatus();
    std::cout
        << "Standing up motion State Exited"
        << endl;
}