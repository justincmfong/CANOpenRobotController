#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::STNDUP, 2);
    robot->startNewTraj();
}

void StandingUp::during(void) {
    // if the green button is pressed move. Or do nothing
    DEBUG_OUT("ENTERDURRING")
    robot->moveThroughTraj();
}
void StandingUp::exit(void) {
    std::cout
        << "Standing up motion State Exited"
        << endl;
}