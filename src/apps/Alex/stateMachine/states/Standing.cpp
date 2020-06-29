////////// STATE ////////////////////
//-------  Standing ------------/////
////////////////////////////////////
#include "Standing.h"

void Standing::entry(void) {
    std::cout << "Standing State Entered " << std::endl
              << "=======================" << std::endl
              << " A ->> sit down" << std::endl
              << " S ->> start Walk" << std::endl
              << "=======================" << std::endl
              << std::endl;
    robot->copleyDrives[0]->setCurrentState(AlexState::Standing);
    //\todo change to this: robot->copleyDrives[0]->setCurrentState(this.getName());
    robot->pb.printMenu();
}

void Standing::during(void) {
    RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    if (modeSelected != RobotMode::INITIAL) {
        std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected);
        robot->copleyDrives[1]->setNextMotion(modeSelected);
    }
}

void Standing::exit(void) {
    std::cout
        << "Standing State Exited" << endl;
}
