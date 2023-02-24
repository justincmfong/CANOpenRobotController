/**
 * \file AOISDrive.h
 * \author  Justin Fong
 * \brief A class implementing an AIOS drive. This is used to interface CORC with any 
 * AIOS drives from Fourier Dynamics. Any implementation which uses these drives (and 
 * thus this class) must use the 
 * 
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef AIOSDRIVE_H
#define AIOSDRIVE_H

#include <string.h>

#include <vector>

#include "Drive.h"
#include "CO_OD.h"


#include "aios.h"

// NOTE: Should implement a generic drive, not a DRIVE here which is CANOpen

class AIOSDrive : public Drive {
   private:
    /**
     * \brief Current status word of the drive
     *
     */
    UNSIGNED16 statusWord = 0;
    UNSIGNED16 errorWord = 0;
    INTEGER32 actualPos = 0;
    INTEGER32 actualVel = 0;
    INTEGER16 actualTor = 0;
    INTEGER32 targetPos = 0;
    INTEGER32 targetVel = 0;
    INTEGER16 targetTor = 0;

    DriveState driveState = DISABLED;
    ControlMode controlMode = CM_UNCONFIGURED;
       // Things should exist here to enable stuff from happening
        virtual bool init();

    /**
       * \brief 
       *
       * \return 0 if unsuccesfull
       */
    virtual int preop(); //TODO: CAN THIS JUST BE NOTHING?

    /**
       * \brief 
       *
       * \return 0 if unsuccesfull
       */
    virtual int start();

    /**
       * \brief 
       *
       * \return 0 if unsuccesfull
       */
       virtual int stop();

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
    virtual bool initPosControl(motorProfile posControlMotorProfile);
    virtual bool initPosControl();

    /**
           * Sets the drive to Velocity control with default parameters (through SDO messages)
           *
           * Note: Should be overloaded to allow parameters to be set
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool initVelControl(motorProfile velControlMotorProfile);
    virtual bool initVelControl();

    /**
           * Sets the drive to Torque control with default parameters (through SDO messages)
           *
           * Note: Should be overloaded to allow parameters to be set
           *
           * \return true if successful
           * \return false if not
           */
    virtual bool initTorqueControl();

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
    virtual ControlMode getControlMode();
};
#endif  // AIOSDRIVE_H