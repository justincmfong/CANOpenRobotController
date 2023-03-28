#include "AIOSTestMachine.h"

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
    double inc = 0.2*M_PI/180.;
    if (robot->keyboard->getKeyUC() == 'S') {
        for (int i = 0; i < targetPos.size(); i++) {
            targetPos[i] = targetPos[i] + inc;
        }
        robot->setPosition(targetPos);
    }
    else if(robot->keyboard->getKeyUC() == 'D') {
        for (int i = 0; i < targetPos.size(); i++) {
            targetPos[i] = targetPos[i] - inc;
        }
        robot->setPosition(targetPos);
    }

    if(iterations()%50) {
        std::cout<< "CORC: " << targetPos.transpose()*180/M_PI  << "=>" << robot->getPosition().transpose()*180/M_PI << "(deg)   " << robot->getVelocity().transpose()*180/M_PI << "(deg/s)\n";
    }
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
    robot->setVelocity(targetVel);
}
void AIOSVelControlState::duringCode(void) {
    double vel_inc = 10/180.*M_PI;
    if (robot->keyboard->getKeyUC() == 'S') {
        for (int i = 0; i < targetVel.size(); i++) {
            targetVel[i] += +vel_inc;
        }
    } else if (robot->keyboard->getKeyUC() == 'D') {
        for (int i = 0; i < targetVel.size(); i++) {
            targetVel[i] += -vel_inc;
        }
    }
    robot->setVelocity(targetVel);

    if(iterations()%50) {
        std::cout<< "CORC: " << robot->getPosition().transpose()*180/M_PI << "(deg)   " << targetVel.transpose()*180/M_PI << " => " <<robot->getVelocity().transpose()*180/M_PI << "(deg/s)\n";
    }
}
void AIOSVelControlState::exitCode(void) {
    robot->setVelocity(robot->getVelocity()*0.0);
    robot->disable();
}



void AIOSTorqControlState::entryCode(void) {
    robot->initTorqueControl();
    targetTorq = robot->getTorque();
    for (int i = 0; i < targetTorq.size(); i++) {
        targetTorq[i] = 0;
    }
    robot->setTorque(targetTorq);
}
void AIOSTorqControlState::duringCode(void) {
    if (robot->keyboard->getKeyUC() == 'S') {
        for (int i = 0; i < targetTorq.size(); i++) {
            targetTorq[i] = targetTorq[i] + 0.5;
        }
    } else if (robot->keyboard->getKeyUC() == 'D') {
        for (int i = 0; i < targetTorq.size(); i++) {
            targetTorq[i] = targetTorq[i] - 0.5;
        }
    }
    robot->setTorque(targetTorq);

    if(iterations()%50) {
        std::cout<< "CORC: " <<  robot->getPosition().transpose()*180/M_PI << "(deg)   " << targetTorq.transpose() << "??  => " << robot->getTorque().transpose() << "?? \n";
    }
}
void AIOSTorqControlState::exitCode(void) {
    robot->setTorque(robot->getTorque()*0.0);
    robot->disable();
}

void AIOSCheckErrorState::entryCode(void) {
}
void AIOSCheckErrorState::duringCode(void) {
    robot->getErrors();
}
void AIOSCheckErrorState::exitCode(void) {
}
