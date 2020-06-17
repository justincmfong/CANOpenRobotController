#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    robot->setPos(RobotMode::STNDUP);
    trajectoryGenerator->initialiseTrajectory(RobotMode::STNDUP, robot->getJointStates());
    robot->startNewTraj();
    DEBUG_OUT("INITIAL Standing UP POS:")
    robot->printStatus();
}

void StandingUp::during(void) {
    robot->moveThroughTraj();
}
void StandingUp::exit(void) {
    DEBUG_OUT("FINAL Standing UP POS:")
    robot->printStatus();
    std::cout
        << "Standing up motion State Exited"
        << endl;
}