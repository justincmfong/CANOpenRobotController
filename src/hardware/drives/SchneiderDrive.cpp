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
#ifndef VIRTUAL
    sendSDOMessages(generatePosControlConfigSDO(posControlMotorProfile));
#endif

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

bool SchneiderDrive::initPDOs() {
    DEBUG_OUT("Drive::initPDOs")
    DEBUG_OUT("Set up STATUS_WORD TPDO")
    sendSDOMessages(generateTPDOConfigSDO({STATUS_WORD}, 1, 0xFF));

    DEBUG_OUT("Set up ACTUAL_POS and ACTUAL_VEL TPDO")
    sendSDOMessages(generateTPDOConfigSDO({ACTUAL_POS, ACTUAL_VEL}, 2, 1));

    DEBUG_OUT("Set up TARGET_POS RPDO")
    sendSDOMessages(generateRPDOConfigSDO({TARGET_POS}, 3, 0xff));

    DEBUG_OUT("Set up TARGET_VEL RPDO")
    sendSDOMessages(generateRPDOConfigSDO({TARGET_VEL}, 4, 0xff));
    return true;
}
std::vector<std::string> SchneiderDrive::generateRPDOConfigSDO(std::vector<OD_Entry_t> items, int PDO_Num, int UpdateTiming) {
    /**
     *  \todo Do a check to make sure that the OD_Entry_t items can be Received
     *
     */

    // Calculate COB_ID. If TPDO:
    int COB_ID = 0x100 * PDO_Num + NodeID;

    // Define Vector to be returned as part of this method
    std::vector<std::string> CANCommands;

    // Define stringstream for ease of constructing hex strings

    std::stringstream sstream;
    // Disable PDO
    sstream
        << "[1] " << NodeID << " write 0x" << std::hex
        << 0x1400 + PDO_Num - 2 << " 1 u32 0x" << std::hex << 0x80000000 + COB_ID;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    // Set so that there no PDO items, enable mapping change
    sstream
        << "[1] " << NodeID << " write 0x" << std::hex
        << 0x1600 + PDO_Num - 2 << " 0 u8 0";
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    // Set the PDO so that it triggers every SYNC Message
    sstream
        << "[1] " << NodeID << " write 0x" << std::hex
        << 0x1400 + PDO_Num - 2 << " 2 u8 0x" << std::hex << UpdateTiming;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    for (int i = 1; i <= items.size(); i++) {
        // Set transmit parameters
        sstream
            << "[1] " << NodeID << " write 0x" << std::hex
            << 0x1600 + PDO_Num - 2 << " " << i << " u32 0x"
            << std::hex << OD_Addresses[items[i - 1]] * 0x10000 + OD_Data_Size[items[i - 1]];
        CANCommands.push_back(sstream.str());
        sstream.str(std::string());
    }

    // Sets Number of PDO items to reenable
    sstream
        << "[1] " << NodeID << " write 0x" << std::hex
        << 0x1600 + PDO_Num - 2 << " 0 u8 " << std::dec << items.size();
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    // Enable  PDO
    sstream
        << "[1] " << NodeID << " write 0x" << std::hex
        << 0x1400 + PDO_Num - 2 << " 1 u32 0x" << std::hex << COB_ID;
    CANCommands.push_back(sstream.str());
    sstream.str(std::string());

    return CANCommands;
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
