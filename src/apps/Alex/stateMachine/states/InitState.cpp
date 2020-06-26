#include "InitState.h"

void InitState::entry(void) {
    std::cout
        << "==================================" << endl
        << " WELCOME USER" << endl
        << "==================================" << endl
        << endl
        << "========================" << endl
        << " PRESS S to start program" << endl
        << "========================" << endl;
    robot->pb.printMenu();
}
void InitState::during(void) {
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "output:" << robot->pb.printRobotMode(modeSelected);
    }
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
}
