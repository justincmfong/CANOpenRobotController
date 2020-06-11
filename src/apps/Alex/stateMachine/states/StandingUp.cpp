#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    // trajectoryGenerator->initialiseTrajectory(RobotMode::STNDUP, 2);
    trajectoryGenerator->initialiseTrajectory(Trajectory::STAND, 10);
    robot->startNewTraj();
}

void StandingUp::during(void) {
    robot->moveThroughTraj();
}
void StandingUp::exit(void) {
    std::cout
        << "Standing up motion State Exited"
        << endl;
}