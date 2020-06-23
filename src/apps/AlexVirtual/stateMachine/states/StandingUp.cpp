#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::STNDUP, robot->getJointStates());
    robot->startNewTraj();
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