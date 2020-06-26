#include "pocketBeagle.h"

pocketBeagle::pocketBeagle(){
    DEBUG_OUT("Virtual pocket Beagle created")}

RobotMode pocketBeagle::updateController(bool up, bool dwn, bool select) {
    RobotMode returnValue = RobotMode::INITIAL;
    if (up) {
        if (currentMode == (NUMMODES - 1)) {
            //endcurrentMode of list go to start
            currentMode = 0;
        } else {
            currentMode++;
        }
        printMenu();
    } else if (dwn) {
        if (currentMode == 0) {
            //endcurrentMode of list go to start
            currentMode = NUMMODES - 1;
        } else {
            currentMode--;
        }
        printMenu();
    } else if (select) {
        printRobotMode(nextMotion[currentMode]);
        returnValue = nextMotion[currentMode];
    }
    //printMenu();
    return returnValue;
}
/**
     * \brief print current menu of motions to the screen -> next Motion on crutch
     * 
     */
void pocketBeagle::printMenu() {
    int n = sizeof(nextMotion) / sizeof(nextMotion[0]);
    // print from ind-th index to (n+i)th index.
    for (int i = currentMode; i < n + currentMode; i++) {
        std::cout << printRobotMode(nextMotion[(i % n)]) << " , ";
    }
    std::cout << std::endl;
}
std::string pocketBeagle::printRobotMode(RobotMode mode) {
    switch (mode) {
        case RobotMode::NORMALWALK:
            return "Normal Walk";
            break;
        case RobotMode::SITDWN:
            return "Sitting Down";
            break;
        case RobotMode::STNDUP:
            return "Standing Up";
            break;
        default:
            return "No SELECTA";
            break;
    }
}
int pocketBeagle::intRobotMode(RobotMode mode) {
    switch (mode) {
        case RobotMode::NORMALWALK:
            return 1;
            break;
        case RobotMode::SITDWN:
            return 2;
            break;
        case RobotMode::STNDUP:
            return 3;
            break;
        default:
            return 0;
            break;
    }
}