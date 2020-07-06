#include "LeftForward.h"

void LeftForward::entry(void) {
    std::cout
        << "========================" << endl
        << " Left FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    robot->setCurrentState(AlexState::LeftForward);
    robot->pb.printMenu();
    // entry flag must be set to true by a green button release
    robot->setResetFlag(false);
}
void LeftForward::during(void) {
    // Virtual crutch menu + OD.nm
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected) << std::endl;
        robot->setNextMotion(modeSelected);
    }
    updateCrutch();
    updateFlag();
}
void LeftForward::exit(void) {
    DEBUG_OUT("Left Forward state exited")
}
