#include "EXO22States.h"

double timeval_to_sec(struct timespec *ts)
{
    return (double)(ts->tv_sec + ts->tv_nsec / 1000000000.0);
}

void EXO22CalibState::entryCode(void) {
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
void EXO22CalibState::duringCode(void) {
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
void EXO22CalibState::exitCode(void) {
    robot->setTorque(robot->getTorque()*0.0);
}


void EXO22MovingState::entryCode(void) {
    robot->initPositionControl();
    targetPos = robot->getPosition();
}
void EXO22MovingState::duringCode(void) {
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
void EXO22MovingState::exitCode(void) {
    Eigen::VectorXd pos = robot->getPosition();
    robot->setPosition(pos);
}


void EXO22StationaryState::entryCode(void) {

}
void EXO22StationaryState::duringCode(void) {

}

void EXO22StationaryState::exitCode(void) {
}


void EXO22CheckErrorState::entryCode(void) {
}
void EXO22CheckErrorState::duringCode(void) {
    robot->getErrors();
}
void EXO22CheckErrorState::exitCode(void) {
}
