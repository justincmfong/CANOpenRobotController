/**
 * /file RecordState.cpp
 * /author Justin Fong
 * /brief Just records data
 * /version 0.1
 * /date 2021-1-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "RecordState.h"

RecordState::RecordState(StateMachine *m, LoggingRobot *robot, const char *name) : State(m, name), robot(robot) {
    spdlog::info("RecordState Created");
};
void RecordState::entry(void) {
    spdlog::info("RecordState entry");
    spdlog::info("S to Stop");
    lastCrutchForce = robot->getCrutchReadings();
    lastForcePlateForce = robot->getForcePlateReadings();
    robot->startSensors();
};

void RecordState::during(void){
    Eigen::VectorXd crutchForce = robot->getCrutchReadings();
    Eigen::VectorXi forcePlateForce = robot->getForcePlateReadings();

    // Check if some sensors are not responding properly every one second
    if(ticker % 100 == 99){
        bool ok =true;
        if (lastCrutchForce.isApprox(crutchForce)){
            spdlog::error("Crutches Not Updating");
            ok = false;
        }
        if (lastForcePlateForce.segment(0,8).isApprox(forcePlateForce.segment(0,8))){
            spdlog::error("First ForcePlate Set not updating");
            ok = false;
        }
        if (lastForcePlateForce.segment(8,8).isApprox(forcePlateForce.segment(8,8))){
            spdlog::error("Second ForcePlate Set not updating");
            ok = false;
        }
        if (ok) {
            spdlog::info("Recording...");
        }
        lastCrutchForce = crutchForce;
        lastForcePlateForce = forcePlateForce;
    }
    ticker++;

};

void RecordState::exit(void) {
    robot->stopSensors();
    spdlog::info("RecordState Exit");
};