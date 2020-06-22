/**
 * @file AlexJoint.cpp
 * @author William Campbell
 * @brief 
 * @version 0.1
 * @date 2020-06-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "AlexJoint.h"

#include <iostream>

#include "DebugMacro.h"

AlexJoint::AlexJoint(int jointID, double jointMin, double jointMax, Drive *drive, JointKnownPos jointParams) : ActuatedJoint(jointID, jointMin, jointMax, drive) {
    jointParamaters = jointParams;
    DEBUG_OUT("MY JOINT ID: " << this->id)
    // Do nothing else
}

bool AlexJoint::updateValue() {
    q = fromDriveUnits(drive->getPos());
    // FOR TESTING w/o real robot -> set current pos to last setPosition
    //q = lastQCommand;

    return true;
}

setMovementReturnCode_t AlexJoint::setPosition(double desQ) {
    // for testing w/o Robot
    lastQCommand = desQ;
    // if (desQ > qMax) {
    //     DEBUG_OUT("Joint" << this->id << " COMMAND:" << desQ << " OUTSIDE OF MAXQ")
    //     desQ = qMax;
    // } else if (desQ < qMin) {
    //     DEBUG_OUT("Joint" << this->id << " COMMAND:" << desQ << " OUTSIDE OF MINQ")
    //     desQ = qMin;
    // }
    return ActuatedJoint::setPosition(desQ);
}

bool AlexJoint::initNetwork() {
    DEBUG_OUT("Joint::initNetwork()")
    if (drive->initPDOs()) {
        return true;
    } else {
        return false;
    }
    // For testing
    // return true;
}
double AlexJoint::getQ() {
    return q;
}
double AlexJoint::fromDriveUnits(int driveValue) {
    if (A == 0) {
        //is first run -> calculate + set A and B
        linearInterpolatePreCalc();
    }
    return (double)(driveValue - B) / A;
}
int AlexJoint::toDriveUnits(double jointValue) {
    if (A == 0) {
        //is first run -> calculate + store A and B
        linearInterpolatePreCalc();
    }
    int output = (int)(A * jointValue + B);
    return output;
}
void AlexJoint::linearInterpolatePreCalc() {
    long y1 = jointParamaters.motorCountA;
    long y2 = jointParamaters.motorCountB;
    long x1 = jointParamaters.motorDegPosA;
    long x2 = jointParamaters.motorDegPosB;
    A = 1.0 * (y2 - y1) / (x2 - x1);
    B = 1.0 * (y1 * x2 - y2 * x1) / (x2 - x1);
}

void AlexJoint::bitFlip() {
    drive->posControlConfirmSP();
}