////////// STATE ////////////////////
//-------  Sitting ------------/////
////////////////////////////////////
#include "Sitting.h"
void Sitting::entry() {
    std::cout << "Sitting State Entered " << std::endl
              << "=======================" << std::endl
              << " HIT A to begin standing up" << std::endl
              << "=======================" << std::endl
              << std::endl;
    robot->copleyDrives[0]->setCurrentState(AlexState::Sitting);
    //\todo change to this: robot->copleyDrives[0]->setCurrentState(this.getName());
    robot->pb.printMenu();
}
void Sitting::during() {
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected) << std::endl;
        robot->copleyDrives[1]->setNextMotion(modeSelected);
    }
    updateCrutch();
}
void Sitting::exit() {
}