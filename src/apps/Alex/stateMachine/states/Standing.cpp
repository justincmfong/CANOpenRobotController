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
    robot->setCurrentState(AlexState::Standing);
    //robot->pb.printMenu();
    // entry flag must be set to true by a green button release
    robot->setResetFlag(false);
}

void Standing::during(void) {
    // RobotMode modeSelected = robot->pb.updateController(robot->keyboard.getE(), robot->keyboard.getW(), robot->keyboard.getX());
    // if (modeSelected != RobotMode::INITIAL) {
    //     std::cout << "Selected mode: " << robot->pb.printRobotMode(modeSelected) << std::endl;
    //     ;
    //     robot->setNextMotion(modeSelected);
    // }
    updateCrutch();
    updateFlag();
}

void Standing::exit(void) {
    std::cout
        << "Standing State Exited" << endl;
}
