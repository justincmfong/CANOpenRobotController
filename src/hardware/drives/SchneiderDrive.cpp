/**
 * @brief An implementation of the Drive Object, specifically for a Schneider electric motor drive.
 * 
 */
#include "SchneiderDrive.h"

#include <iostream>

#include "DebugMacro.h"

SchneiderDrive::SchneiderDrive(int NodeID) : Drive::Drive(NodeID) {
    this->NodeID = NodeID;
}
SchneiderDrive::~SchneiderDrive() {
    DEBUG_OUT(" SchneiderDrive Deleted ")
}

bool SchneiderDrive::Init() {
    return false;
}

bool SchneiderDrive::initPosControl(motorProfile posControlMotorProfile) {
    DEBUG_OUT("NodeID " << NodeID << " Initialising Position Control")

    // Send same as other motro config using motor profile
    sendSDOMessages(generatePosControlConfigSDO(posControlMotorProfile));

    return true;
}

bool SchneiderDrive::initVelControl(motorProfile velControlMotorProfile) {
    DEBUG_OUT("NodeID " << NodeID << " Initialising Velocity Control")
    /**
     * \todo create velControlMOTORPROFILE and test on exo
     * \todo Tune velocity loop gain index 0x2381 to optimize V control
     * 
    */
    sendSDOMessages(generateVelControlConfigSDO(velControlMotorProfile));
    return true;
}

bool SchneiderDrive::initTorqueControl() {
    DEBUG_OUT("NodeID " << NodeID << " Initialising Torque Control")
    sendSDOMessages(generateTorqueControlConfigSDO());

    return false;
}
std::vector<std::string> SchneiderDrive::generatePosControlConfigSDO(motorProfile positionProfile) {
    DEBUG_OUT("generating Pos Control config SDO")
    std::vector<std::string> CANCommands;
    // Define stringstream for ease of constructing hex strings
    std::stringstream sstream;
    // start drive
    sstream << "[1] " << NodeID << " start";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    //enable profile position mode
    sstream << "[1] " << NodeID << " write 0x6060 0 i8 1";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    //Set velocity profile
    sstream << "[1] " << NodeID << " write 0x6081 0 i32 " << std::dec << positionProfile.profileVelocity;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    //Set acceleration profile
    sstream
        << "[1] " << NodeID << " write 0x6083 0 i32 " << std::dec << positionProfile.profileAcceleration;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    //Set deceleration profile
    sstream << "[1] " << NodeID << " write 0x6084 0 i32 " << std::dec << positionProfile.profileDeceleration;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    // BIT FLIP SDOs
    //Set control word to low
    sstream << "[1] " << NodeID << " write 0x6040 0 i16 6";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());
    // Set control word to high
    sstream << "[1] " << NodeID << " write 0x6040 0 i16 15";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    return CANCommands;
    ; /*<!execute base class function*/
};

std::vector<std::string> SchneiderDrive::generateVelControlConfigSDO(motorProfile velocityProfile) {
    return Drive::generateVelControlConfigSDO(velocityProfile); /*<!execute base class function*/
};

std::vector<std::string> SchneiderDrive::generateTorqueControlConfigSDO() {
    return Drive::generateTorqueControlConfigSDO(); /*<!execute base class function*/
}
