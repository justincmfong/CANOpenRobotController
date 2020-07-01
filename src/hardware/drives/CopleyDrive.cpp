/**
 * @brief An implementation of the Drive Object, specifically for the Copley Drive
 * 
 */
#include "CopleyDrive.h"

#include <iostream>

#include "DebugMacro.h"

CopleyDrive::CopleyDrive(int NodeID) : Drive::Drive(NodeID) {
    this->NodeID = NodeID;
}
CopleyDrive::~CopleyDrive() {
    DEBUG_OUT(" CopleyDrive Deleted ")
}

bool CopleyDrive::Init() {
    return false;
}

bool CopleyDrive::initPosControl(motorProfile posControlMotorProfile) {
    DEBUG_OUT("NodeID " << NodeID << " Initialising Position Control")
    /*Catch SDO msg set up for real robot here*/
    //if (sendSDOMessages(generatePosControlConfigSDO(posControlMotorProfile))) {
    //    return true;
    //} else {
    //    return false;
    // }
    // /**
    //  * \todo Move jointMinMap and jointMaxMap to set additional parameters (bit 5 in 0x6041 makes updates happen immediately)
    //  *
    //  */
    sendSDOMessages(generatePosControlConfigSDO(posControlMotorProfile));
    /**
     * \todo Move jointMinMap and jointMaxMap to set additional parameters (bit 5 in 0x6041 makes updates happen immediately)
     * 
     */
    return true;
}

bool CopleyDrive::initVelControl(motorProfile velControlMotorProfile) {
    DEBUG_OUT("NodeID " << NodeID << " Initialising Velocity Control")
    /**
     * \todo create velControlMOTORPROFILE and test on exo
     * \todo Tune velocity loop gain index 0x2381 to optimize V control
     * 
    */
    sendSDOMessages(generateVelControlConfigSDO(velControlMotorProfile));
    return true;
}

bool CopleyDrive::initTorqueControl() {
    DEBUG_OUT("NodeID " << NodeID << " Initialising Torque Control")
    sendSDOMessages(generateTorqueControlConfigSDO());

    return false;
}
std::vector<std::string> CopleyDrive::generatePosControlConfigSDO(motorProfile positionProfile) {
    return Drive::generatePosControlConfigSDO(positionProfile); /*<!execute base class function*/
};

std::vector<std::string> CopleyDrive::generateVelControlConfigSDO(motorProfile velocityProfile) {
    return Drive::generateVelControlConfigSDO(velocityProfile); /*<!execute base class function*/
};

std::vector<std::string> CopleyDrive::generateTorqueControlConfigSDO() {
    return Drive::generateTorqueControlConfigSDO(); /*<!execute base class function*/
}
void CopleyDrive::setCurrentState(AlexState state) {
    *(&CO_OD_RAM.currentState) = static_cast<int>(state);
    DEBUG_OUT("current state SET TO:" << *(&CO_OD_RAM.currentState));
}
void CopleyDrive::setNextMotion(RobotMode mode) {
    *(&CO_OD_RAM.nextMovement) = static_cast<int>(mode);
    //DEBUG_OUT("NEXTMOVEMENT SET TO:" << *(&CO_OD_RAM.nextMovement));
}
int CopleyDrive::getNextMotion() {
    return *(&CO_OD_RAM.nextMovement);
}
void CopleyDrive::setCurrentMotion(RobotMode mode) {
    *(&CO_OD_RAM.currentMovement) = static_cast<int>(mode);
}

int CopleyDrive::getCurrentMotion() {
    return *(&CO_OD_RAM.currentMovement);
}
int CopleyDrive::getGoButton() {
    return *(&CO_OD_RAM.goButton);
}
int CopleyDrive::intRobotMode(RobotMode mode) {
    switch (mode) {
        case RobotMode::NORMALWALK:
            return 0;
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