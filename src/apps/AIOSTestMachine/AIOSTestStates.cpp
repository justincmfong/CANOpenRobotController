#include "AIOSTestMachine.h"

double timeval_to_sec(struct timespec *ts)
{
    return (double)(ts->tv_sec + ts->tv_nsec / 1000000000.0);
}

void AIOSCalibState::entryCode(void) {
    calibDone=false;
    VX q0=robot->getPosition();
    stop_reached_time = 0.*q0;
    for(unsigned int i=0; i<q0.size(); i++) {
        at_stop.push_back(false);
    }
    robot->decalibrate();
    robot->initTorqueControl();
    maxTorque = 3;
    b = 2.;//damping coef
    std::cout << "Calibrating (keep clear)..." << std::flush;
}
//Move slowly on each joint until max force detected
void AIOSCalibState::duringCode(void) {
    VX tau=robot->getTorque()*0.0;

    //Apply constant torque (with damping) unless stop has been detected for more than 0.5s
    VX vel=robot->getVelocity();
    for(unsigned int i=0; i<vel.size(); i++) {
        tau(i) = std::min(std::max(maxTorque - b * vel(i), .0), maxTorque);
        if(stop_reached_time(i)>0.5) {
            at_stop[i]=true;
        }
        if(vel(i)<0.01) {
            stop_reached_time(i) += dt();
        }
    }

    //Switch torque control when done
    if(robot->isCalibrated()) {
        robot->setTorque(robot->getTorque()*0.0);
        calibDone=true; //Trigger event
    }
    else {
        //If all joints are calibrated
        if(std::all_of(at_stop.begin(), at_stop.end(), [](bool v) { return v; })) {
            robot->applyCalibration();
            std::cout << "OK." << std::endl;
        }
        else {
            robot->setTorque(tau);
            if(iterations()%100==1) {
                std::cout << "." << std::flush;
            }
        }
    }
}
void AIOSCalibState::exitCode(void) {
    robot->setTorque(robot->getTorque()*0.0);
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
}



void AIOSCheckErrorState::entryCode(void) {
}
void AIOSCheckErrorState::duringCode(void) {
    robot->getErrors();
}
void AIOSCheckErrorState::exitCode(void) {
}
