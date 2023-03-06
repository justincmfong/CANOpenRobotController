/**
 * @file AIOSJoint.cpp
 * @author Justin Fong
 * @brief
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>

#include "AIOSJoint.h"

AIOSJoint::AIOSJoint(int jointID, double q_min, double q_max, AIOSDrive* drive, const std::string& name) : Joint(jointID, q_min, q_max, drive, name) {
    spdlog::debug("Joint ID {} Created", this->id);
}

AIOSJoint::~AIOSJoint() {
}

bool AIOSJoint::initNetwork() {
    spdlog::debug("JointM1::initNetwork()");
    return drive->init();
}

/***************************************************************************************/
/****************** implementation of virtual function of ActuatedJoint class **********/
/***************************************************************************************/
// convert from driver unit to joint unit for reading command
double AIOSJoint::driveUnitToJointPosition(int driveValue) {
    return 1.0f * (double)driveValue;
}

double AIOSJoint::driveUnitToJointVelocity(int driveValue) {
    return 1.0f * driveValue;
}

double AIOSJoint::driveUnitToJointTorque(int driveValue) {
    return 1.0f * driveValue;
}

// covert from joint unit to driver unit for control command
int AIOSJoint::jointPositionToDriveUnit(double jointValue) {
    //    DEBUG_OUT("jointPositionToDriveUnit " << count);
    return int(round(jointValue));
}

int AIOSJoint::jointVelocityToDriveUnit(double jointValue) {
    return int(round(jointValue));
}

int AIOSJoint::jointTorqueToDriveUnit(double jointValue) {
    return int(round(jointValue));
}

bool AIOSJoint::updateValue() {
    position = driveUnitToJointPosition(drive->getPos());
    velocity = driveUnitToJointVelocity(drive->getVel());
    torque = driveUnitToJointTorque(drive->getTorque());
    return true;
}

/***************************************************************************************/
/****************** Check command safety range and send command to driver **************/
/***************************************************************************************/
// including position command, velocity command, torque command
setMovementReturnCode_t AIOSJoint::setPosition(double qd) {
    if (calibrated) {
        if (qd >= qMin && qd <= qMax) {
            return Joint::setPosition(qd);
        } else {
            spdlog::debug("Position out of bound: {}", qd);
            return OUTSIDE_LIMITS;
        }
    } else {
        return NOT_CALIBRATED;
    }
}

setMovementReturnCode_t AIOSJoint::setVelocity(double dqd) {
    // Position protection first only if calibrated
    if (calibrated) {
        if (position <= qMin && dqd < 0) {
            dqd = 0;
            return OUTSIDE_LIMITS;
        }
        if (position >= qMax && dqd > 0) {
            dqd = 0;
            return OUTSIDE_LIMITS;
        }
    }

    return Joint::setVelocity(dqd);
}

setMovementReturnCode_t AIOSJoint::setTorque(double taud) {
    // Position protection first only if calibrated
    if (calibrated) {
        if (position <= qMin && taud < 0) {
            taud = 0;
            return OUTSIDE_LIMITS;
        }
        if (position >= qMax && taud > 0) {
            taud = 0;
            return OUTSIDE_LIMITS;
        }
    }
    return Joint::setTorque(taud);
}