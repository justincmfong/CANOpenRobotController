/**
 * \file JointM1.h
 * \author Yue Wen, Tim Haswell borrowing heavily from Vincent Crocher
 * \brief An M1 actuated joint
 * \version 0.1
 * \date 2020-07-08
 *
 * \copyright Copyright (c) 2020
 *
 */
#ifndef AIOSJoint_H_INCLUDED
#define AIOSJoint_H_INCLUDED

#include <cmath>

#include "Joint.h"
#include "AIOSDrive.h"

/**
 * \brief M1 actuated joint, using Kinco drive.
 *
 */
class AIOSJoint : public Joint {
   public:
    short int sign;

    /**
     * \brief Conversion between drive unit (encoder count) and joint unit (radian).
     *
     * \return drive unit for low-level control purpose
     * \return joint unit for high-level control purpose
     */
    double driveUnitToJointPosition(int driveValue);
    int jointPositionToDriveUnit(double jointValue);
    double driveUnitToJointVelocity(int driveValue);
    int jointVelocityToDriveUnit(double jointValue);
    double driveUnitToJointTorque(int driveValue);
    int jointTorqueToDriveUnit(double jointValue);

   public:
    AIOSJoint(int jointID, double q_min, double q_max, AIOSDrive* drive = NULL, const std::string& name = "");
    ~AIOSJoint();

    bool updateValue();

    /**
     * \brief Check if current velocity and torque are within limits.
     *
     * \return OUTSIDE_LIMITS if outside the limits (!), SUCCESS otherwise
     */
    setMovementReturnCode_t setPosition(double qd);
    setMovementReturnCode_t setVelocity(double dqd);
    setMovementReturnCode_t setTorque(double taud);

    bool initNetwork();

    /**
     * \brief Get error message
     *
     * \return true if succesful
     * \return false if drive is currently not in the correct state to enable
     */
    void errorMessage(setMovementReturnCode_t errorCode);
};

#endif
