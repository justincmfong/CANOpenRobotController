#include "InitState.h"

void InitState::entry(void) {
    std::cout
        << "==================================" << endl
        << " WELCOME TO THE TEST STATE MACHINE" << endl
        << "==================================" << endl
        << endl
        << "========================" << endl
        << " PRESS S to start program" << endl
        << "========================" << endl;
}
void InitState::during(void) {
}
void InitState::exit(void) {
    robot->initPositionControl();
    PilotParameters Brad_parameters = {
        .lowerleg_length = 0.44,
        .upperleg_length = 0.44,
        .ankle_height = 0.12,
        .foot_length = 0.30,
        .hip_width = 0.43,
        .torso_length = 0.4,
        .buttocks_height = 0.05};
    trajectoryGenerator->setPilotParameters(Brad_parameters);
    std::cout
        << "Initialise State Exited" << endl;
}
