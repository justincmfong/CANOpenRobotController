#include "RightForward.h"

void RightForward::entry(void) {
    std::cout
        << "========================" << endl
        << " RIGHT FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    //robot->copleyDrives[0]->setNextMotion(RobotMode::RFWD);
    robot->pb.printMenu();
}
void RightForward::during(void) {
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected);
        robot->copleyDrives[1]->setNextMotion(modeSelected);
    }
}
void RightForward::exit(void) {
    DEBUG_OUT("RightForward state exited")
}
