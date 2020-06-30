#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " STANDING UP" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(robot->getNextMotion(), robot->getJointStates());
    robot->startNewTraj();
    robot->copleyDrives[0]->setCurrentState(AlexState::StandingUp);
    //\todo change to this: robot->copleyDrives[0]->setCurrentState(this.getName());
}

void StandingUp::during(void) {
    robot->moveThroughTraj();
}
void StandingUp::exit(void) {
    robot->printStatus();
    std::cout
        << "Standing up motion State Exited"
        << endl;
}