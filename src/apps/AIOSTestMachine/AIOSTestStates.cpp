#include "AIOSTestMachine.h"

#define OWNER ((AIOSTestMachine *)owner)

double timeval_to_sec(struct timespec *ts)
{
    return (double)(ts->tv_sec + ts->tv_nsec / 1000000000.0);
}

void AIOSStationaryState::entryCode(void) {
}
void AIOSStationaryState::duringCode(void) {

    if(iterations()%100==1) {
        //std::cout << "Doing nothing for "<< elapsedTime << "s..." << std::endl;
        std::cout << running() << " ";
        robot->printStatus();
    }
}
void AIOSStationaryState::exitCode(void) {
}

void AIOSPosControlState::entryCode(void) {
    robot->initPositionControl();
    targetPos = robot->getPosition();
}
void AIOSPosControlState::duringCode(void) {
    // Read Values
    if (robot->keyboard->getKeyUC() == 'S') {
        for (int i = 0; i < targetPos.size(); i++) {
            targetPos[i] = targetPos[i] + 0.1;
        }
        robot->setPosition(targetPos);
    }
    else if(robot->keyboard->getKeyUC() == 'D') {
        for (int i = 0; i < targetPos.size(); i++) {
            targetPos[i] = targetPos[i] -0.1;
        }
        robot->setPosition(targetPos);
    }
    robot->printStatus();
}
void AIOSPosControlState::exitCode(void) {
    Eigen::VectorXd pos = robot->getPosition();
    robot->setPosition(pos);
    robot->disable();
}

void AIOSVelControlState::entryCode(void) {
    robot->initVelocityControl();
    targetVel = robot->getVelocity();
    for (int i = 0; i < targetVel.size(); i++) {
        targetVel[i] = 0;
    }
}
void AIOSVelControlState::duringCode(void) {
    if (robot->keyboard->getKeyUC() == 'S') {
        for (int i = 0; i < targetVel.size(); i++) {
            targetVel[i] = targetVel[i] + 1;
        }
        robot->setVelocity(targetVel);
    } else if (robot->keyboard->getKeyUC() == 'D') {
        for (int i = 0; i < targetVel.size(); i++) {
            targetVel[i] = targetVel[i] - 1;
        }
        robot->setVelocity(targetVel);
    }
}
void AIOSVelControlState::exitCode(void) {
    Eigen::VectorXd vel = robot->getVelocity();
    for (int i = 0; i < vel.size(); i++) {
        vel[i] = 0;
    }
    robot->setVelocity(vel);
    robot->disable();
}

void AIOSTorqControlState::entryCode(void) {
    robot->initTorqueControl();
    targetTorq = robot->getTorque();
    for (int i = 0; i < targetTorq.size(); i++) {
        targetTorq[i] = 0;
    }
}
void AIOSTorqControlState::duringCode(void) {
    if (robot->keyboard->getKeyUC() == 'S') {
        for (int i = 0; i < targetTorq.size(); i++) {
            targetTorq[i] = targetTorq[i] + 1;
        }
        robot->setTorque(targetTorq);
    } else if (robot->keyboard->getKeyUC() == 'D') {
        for (int i = 0; i < targetTorq.size(); i++) {
            targetTorq[i] = targetTorq[i] - 1;
        }
        robot->setTorque(targetTorq);
    }
}
void AIOSTorqControlState::exitCode(void) {
    Eigen::VectorXd torq = robot->getTorque();
    for (int i = 0; i < torq.size(); i++) {
        torq[i] = 0;
    }
    robot->setTorque(torq);
    robot->disable();
}

void AIOSCheckErrorState::entryCode(void) {
}
void AIOSCheckErrorState::duringCode(void) {
    robot->getErrors();
}
void AIOSCheckErrorState::exitCode(void) {
}