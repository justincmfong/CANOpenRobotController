#include "LeftForward.h"

void LeftForward::entry(void) {
    std::cout
        << "========================" << endl
        << " Left FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    robot->copleyDrives[0]->setCurrentState(AlexState::LeftForward);
    //\todo change to this: robot->copleyDrives[0]->setCurrentState(this.getName());
    robot->pb.printMenu();
}
void LeftForward::during(void) {
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected) << std::endl;
        robot->copleyDrives[1]->setNextMotion(modeSelected);
    }
    updateCrutch();
}
void LeftForward::exit(void) {
    DEBUG_OUT("Left Forward state exited")
}
