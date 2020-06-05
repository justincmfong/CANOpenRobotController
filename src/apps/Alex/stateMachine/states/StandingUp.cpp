#include "StandingUp.h"

// Negative bending control machine
void StandingUp::entry(void) {
    std::cout << "===================" << endl
              << " GREEN -> STAND UP" << endl
              << "===================" << endl;
    DEBUG_OUT("STANDING UP OBJ address")
    std::cout << &trajectoryGenerator << std::endl;
    trajectoryGenerator->initialiseTrajectory(RobotMode::STNDUP, 2);
    std::cout << "!!!!!!!!!!!" << trajectoryGenerator->getTestData() << std::endl;
    trajectoryGenerator->changeTestdata(1000);
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