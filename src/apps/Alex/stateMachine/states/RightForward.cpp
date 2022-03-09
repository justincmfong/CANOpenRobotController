#include "RightForward.h"

void RightForward::entry(void) {
    std::cout
        << "========================" << endl
        << " RIGHT FORWARD STATE " << endl
        << " S ->> WALK " << endl
        << " A ->> FEET TOGETHER " << endl
        << "========================n" << endl;
    robot->setCurrentState(AlexState::RightForward);
    //robot->pb.printMenu();
    // entry flag must be set to true by a green button release
    robot->setResetFlag(false);
}
void RightForward::during(void) {
    // RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    // if (modeSelected != RobotMode::INITIAL) {
    //     std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected) << std::endl;
    //     ;
    //     robot->setNextMotion(modeSelected);
    // }
    updateCrutch();
    updateFlag();
}
void RightForward::exit(void) {
    DEBUG_OUT("RightForward state exited")
}
