#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <string.h>
#include <map>
#include <sstream>
#include <vector>

#include "logging.h"

/**
 * An enum type.
 * Constants representing the control mode of the drive
 */
enum ControlMode {
    CM_UNCONFIGURED = 0,
    CM_POSITION_CONTROL = 1,
    CM_VELOCITY_CONTROL = 2,
    CM_TORQUE_CONTROL = 3,
    CM_ERROR = -1,
    CM_UNACTUATED_JOINT = -2
};
/**
 * An enum type.
 * Constants representing the Drives State
 */
enum DriveState {
    DISABLED = 0,
    READY_TO_SWITCH_ON = 1,
    ENABLED = 2,
};


/**
 * \brief struct to hold desired velocity, acceleration and deceleration values for a
 *     drives motor controller profile.
 */
struct motorProfile {
    int profileVelocity;
    int profileAcceleration;
    int profileDeceleration;
};


class Drive {
    protected:
        /**
            * \brief The mode in which the drive is currently configured
            *
            */
        ControlMode controlMode = CM_UNCONFIGURED;

    public:

    /**
       * \brief Initialises the drive 
       *
       * \return True if successful, False if not
       */
    virtual bool init() = 0;

    /**
       * \brief 
       *
       * \return 0 if unsuccesfull
       */
    virtual int preop() = 0; //TODO: CAN THIS JUST BE NOTHING?

    /**
       * \brief 
       *
       * \return 0 if unsuccesfull
       */
    virtual int start() = 0;

    /**
       * \brief 
       *
       * \return 0 if unsuccesfull
       */
    virtual int stop() = 0;

        virtual bool configureMasterPDOs();
 /**
           * \brief Initialises velocity and acceleration profiles (used by position and velocity controls) through SDOs write
           *
           * \return true if sucessfull
           * \return false otherwise
           */
    virtual bool setMotorProfile(motorProfile profile);

    /**
           * Sets the drive to Position control with set parameters (through SDO messages)
           *
           * Note: Should be overloaded to allow parameters to be set
           *
           * \param motorProfile The position control motor profile to be used
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool initPosControl(motorProfile posControlMotorProfile) { return false; };
    virtual bool initPosControl() { return false; };

    /**
           * Sets the drive to Velocity control with default parameters (through SDO messages)
           *
           * Note: Should be overloaded to allow parameters to be set
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool initVelControl(motorProfile velControlMotorProfile) { return false; };
    virtual bool initVelControl() { return false; };

    /**
           * Sets the drive to Torque control with default parameters (through SDO messages)
           *
           * Note: Should be overloaded to allow parameters to be set
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool initTorqueControl() { return false; };

    /**
           * Updates the internal representation of the state of the drive
           *
           * \return The current value of the status word (0x6041)
           */
    virtual int getStatus();

    /**
           * Writes the desired position to the Target Position of the motor drive (0x607A)
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool setPos(int position);

    /**
           * Writes the desired velocity to the Target Velocity of the motor drive (0x60FF)
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool setVel(int velocity);

    /**
           * Writes the desired torque to the Target Torque of the motor drive (0x6071)
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool setTorque(int torque);

    /**
           * Returns the current position from the motor drive (0x6064)
           *
           * \return Position from the motor drive
           */
    virtual int getPos();

    /**
           * Returns the current velocity from the motor drive (0x606C)
           * Returns 0 if NODEID is 5 or 6: ankles. They have no OD entry.
           * \return Velocity from the motor drive
           */
    virtual int getVel();

    /**
           * Returns the current torque from the motor drive (0x6077)
           * Returns 0 if NODEID is 5 or 6: ankles. They have no OD entry.
           * \return Torque from the motor drive
           */
    virtual int getTorque();

    /**
           * Returns the value of digital IN
           * \return Digital in state from the motor drive
           */
    virtual int getDigitalIn();

    // CiA402Drive State Modifiers
    /**
           * \brief Clears errors (and changes the state of the drive to "disabled".
           *
           * This is equivalent to setting bits 7 Control Word (0x6064) to 1.
           * See also the CANopen Programmer's Manual (from Copley Controls)
           *
           * \return true if operation successful
           * \return false if operation unsuccessful
           */
    virtual DriveState resetErrors();

    /**
           * \brief Changes the state of the drive to "ready to switch on".
           *
           * This is equivalent to setting bits 2 and 3 of Control Word (0x6064) to 1.
           * See also the CANopen Programmer's Manual (from Copley Controls)
           *
           * \return true if operation successful
           * \return false if operation unsuccessful
           */
    virtual DriveState readyToSwitchOn();

    /**
           * \brief Sets the state of the drive to "enabled"
           *
           * This is equivalent to setting bits 0, 1, 2, 3 of the control word (0x06064) to 1
           * See also the CANopen Programmer's Manual (from Copley Controls)
           *
           * \return true if operation successful
           * \return false if operation unsuccessful
           */
    virtual DriveState enable();

    /**
           * \brief sets the state of the drive to "disabled"
           *
           * This is equivalent to setting the control word (0x06064) to 0
           * See also the CANopen Programmer's Manual (from Copley Controls)
           *
           * \return true if operation successful
           * \return false if operation unsuccessful
           */
    virtual DriveState disable();

    /**
        * \brief Flips Bit 4 of Control Word (0x6041) - A new set point is only confirmed if the transition is from 0 to 1
        *
        * \return true The control word was previously 0 (i.e. successful set point confirm)
        * \return false The control word was previously 1 (i.e. unsuccessful set point confirm)
        */
    virtual bool posControlConfirmSP();

    /**
        * \brief Sets the continous/not continous profile bit
        *
        * \param continuous if this is true, continous movement is enabled, otherwise it is disabled
        * \return true If change is successful
        * \return false If drive was not in position control mode
        */
    virtual bool posControlSetContinuousProfile(bool continuous);

    /**
        * \brief Get the current state of the drive
        *
        * \return DriveState
        */
    virtual DriveState getState();

    /**
        * \brief Get the current control mode of the drive
        *
        * \return controlMode
        */
    virtual ControlMode getControlMode() { return controlMode; };

};

#endif // DRIVE_H_INCLUDED
