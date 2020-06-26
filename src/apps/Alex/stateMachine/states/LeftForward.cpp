#include "LeftForward.h"

void LeftForward::entry(void) {
    std::cout
        << "========================" << endl
        << " Left FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    //robot->copleyDrives[0]->setNextMotion(RobotMode::LFWD);
    robot->pb.printMenu();
}
void LeftForward::during(void) {
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected);
        robot->copleyDrives[1]->setNextMotion(modeSelected);
    }
}
void LeftForward::exit(void) {
    DEBUG_OUT("Left Forward state exited")
}
