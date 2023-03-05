#include "AIOSTestMachine.h"

#define OWNER ((AIOSTestMachine *)owner)

double timeval_to_sec(struct timespec *ts)
{
    return (double)(ts->tv_sec + ts->tv_nsec / 1000000000.0);
}

void AIOSDemoState::entryCode(void) {
    robot->initTorqueControl();
    std::cout << " Hi";
}
void AIOSDemoState::duringCode(void) {

    if(iterations()%100==1) {
        //std::cout << "Doing nothing for "<< elapsedTime << "s..." << std::endl;
        std::cout << running() << " ";
        robot->printStatus();
    }
}
void AIOSDemoState::exitCode(void) {
}

void AIOSTransparent::entryCode(void) {
    robot->initTorqueControl();
}
void AIOSTransparent::duringCode(void) {

    //Apply corresponding force

    if(iterations()%100==1) {
        robot->printStatus();
    }
}
void AIOSTransparent::exitCode(void) {
}