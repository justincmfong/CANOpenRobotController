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

    bool init(); // No equivalent here here I think

    int preop(); // No equivalent 

    int start(); // No equivalent

    int stop(); // No equivalent 

    bool configureMasterPDOs(); // No equivalent


    bool setMotorProfile(motorProfile profile); // Set TraPTraj (includes acc_lim, dec_lim, vel_lim)
    bool initPosControl(motorProfile posControlMotorProfile) { return false; }; // Set TraPTraj (includes acc_lim, dec_lim, vel_lim)
    bool initPosControl() { return false; }; // No equivalent - maybe set to enable?
    bool initVelControl(motorProfile velControlMotorProfile) { return false; }; // // Set TraPTraj (includes acc_lim, dec_lim, vel_lim)
    bool initVelControl() { return false; }; //  No equivalent - maybe set to enable?
    bool initTorqueControl() { return false; }; //  No equivalent - maybe set to enable?

    int getStatus(); // Get Error?

    bool setPos(int position); // SetInputPosPt
    bool setVel(int velocity);  // SetInputVelocityPt
    bool setTorque(int torque); // SetInputTorquePt

    int getPos(); // Feedback command // TODO
    int getVel(); // Feedback command // TODO
    int getTorque(); // Feedback command // TODO
    int getDigitalIn(); // Feedback command // TODO

    DriveState resetErrors(); // Reset Errors 
    DriveState readyToSwitchOn(); // No equivalent
    DriveState enable(); // Enable  (true)
    DriveState disable(); // Enable (false)
    bool posControlConfirmSP(); // No equivalent
    bool posControlSetContinuousProfile(bool continuous); // No equivalent (should check behaviour)
    DriveState getState(); // Should return something
    ControlMode getControlMode() { return controlMode; }; // No equivalent
