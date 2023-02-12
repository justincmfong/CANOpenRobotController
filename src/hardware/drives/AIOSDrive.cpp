/**
 * \file AOISDrive.cpp
 * \author  Justin Fong
 * \brief A class implementing an AIOS drive. This is used to interface CORC with any 
 * AIOS drives from Fourier Dynamics. Any implementation which uses these drives (and 
 * thus this class) must derive from the AIOSRobot.h class, to manage the communications
 * with these drives. Note that they are significantly different from the CiA402 CANopen 
 * drives, due to the mechanisms of communication.
 * 
 * @version 0.1
 * @date 2023-02-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "AIOSDrive.h"

bool AIOSDrive::init() { return true; }  // No equivalent here here I think
int AIOSDrive::preop() {
    spdlog::error("No such method preop() in AIOSDrive");
    return 1; 
}     // No equivalent
int AIOSDrive::start() {
    spdlog::error("No such method preop() in AIOSDrive");
    return 1;
}  // No equivalent
int AIOSDrive::stop()  {
    spdlog::error("No such method preop() in AIOSDrive");
    return 1; 
}      // No equivalent

bool AIOSDrive::configureMasterPDOs() {
    spdlog::info("No such method configureMasterPDOs() in AIOSDrive");
    return true;
}  // No equivalent

bool AIOSDrive::setMotorProfile(motorProfile profile){
    // Should take this and store a copy locally
    // The set command should send through the the next motor command
}                                                                            // Set TraPTraj (includes acc_lim, dec_lim, vel_lim)
bool AIOSDrive::initPosControl(motorProfile posControlMotorProfile) { 
    // Should set the TrapTraj to have posControlMotorProfile
    // Should set controlMode to Position_Control
    // Should return things
    // Should ensure drive is in correct state
    return false; };  // Set TraPTraj (includes acc_lim, dec_lim, vel_lim)
bool AIOSDrive::initPosControl() { return false; };                                     // No equivalent - maybe set to enable?
bool AIOSDrive::initVelControl(motorProfile velControlMotorProfile) { return false; };  // // Set TraPTraj (includes acc_lim, dec_lim, vel_lim)
bool AIOSDrive::initVelControl() { return false; };                                     //  No equivalent - maybe set to enable?
bool AIOSDrive::initTorqueControl() { return false; };                                  //  No equivalent - maybe set to enable?

int AIOSDrive::getStatus(){
    return 0; // TODO: Need to figure out what to return here..
}  // Get Error?

bool AIOSDrive::setPos(int position){
    if (controlMode == CM_POSITION_CONTROL){
        targetPos = position;
        return true;
    } else{
        return false;
    }
};  // SetInputPosPt
bool AIOSDrive::setVel(int velocity){
    if (controlMode == CM_VELOCITY_CONTROL) {
        targetVel = velocity;
        return true;
    } else {
        return false;
    }
} // SetInputVelocityPt
bool AIOSDrive::setTorque(int torque){
    if (controlMode == CM_TORQUE_CONTROL){
        targetTor = torque;
        return true;
    }else{
        return false;
    }
};  // SetInputTorquePt

int AIOSDrive::getPos() {
    return actualPos;
}  // Feedback command // TODO
int AIOSDrive::getVel() {
    return actualVel;
}  // Feedback command // TODO
int AIOSDrive::getTorque() {
    return actualTor;
}  // Feedback command // TODO

int AIOSDrive::getDigitalIn(){
    spdlog::error("No Digital Inputs in AIOS Drive");
    return 0; 
};  // Feedback command // TODO

DriveState AIOSDrive::resetErrors();                   // Reset Errors
DriveState AIOSDrive::readyToSwitchOn();               // No equivalent
DriveState AIOSDrive::enable();                        // Enable  (true)
DriveState AIOSDrive::disable();                       // Enable (false)
bool AIOSDrive::posControlConfirmSP(){
    spdlog::error("No such method posControlConfirmSP() in AIOSDrive");
    return false;
}                 // No equivalent

bool AIOSDrive::posControlSetContinuousProfile(bool continuous){
    spdlog::error("No such method posControlSetContinuousProfile() in AIOSDrive");
    return false;
}  // No equivalent (should check behaviour)
DriveState AIOSDrive::getState(){
        // Should only return DISABLED or ENABLED?
        // DISABLED = 0,
        // READY_TO_SWITCH_ON = 1,
        // ENABLED = 2,
    return driveState;
}
ControlMode AIOSDrive::getControlMode() { return controlMode; };  // No equivalent