/**
 * \file JointM1.h
 * \author Justin Fong
 * \brief An AIOS actuated joint
 * \version 0.1
 * \date 2023-03-10
 *
 * \copyright Copyright (c) 2023
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
    const short int sign;
    const double driveToJointPos = 3.0/180.*M_PI; //!< Conversion multiplier used for both position and velocity conversions
    const double driveToJointTorque = 1.0;
    const double dqMin, dqMax, tauMin, tauMax;

    /**
     * \brief Conversion between drive unit (encoder count) and joint unit (radian).
     *
     * \return drive unit for low-level control purpose
     * \return joint unit for high-level control purpose
     */
    double driveUnitToJointPosition(int driveValue);
    int jointPositionToDriveUnit(double jointValue);
    double jointPositionToDriveUnitDouble(double jointValue);
    double driveUnitToJointVelocity(int driveValue);
    int jointVelocityToDriveUnit(double jointValue);
    double jointVelocityToDriveUnitDouble(double jointValue);
    double driveUnitToJointTorque(int driveValue);
    int jointTorqueToDriveUnit(double jointValue);
    double jointTorqueToDriveUnitDouble(double jointValue);
    double jointPositionToDriveUnitDoubleWithOffset(double jointValue);

   public:
    AIOSJoint(int jointID, double q_min, double q_max, short int _sign, double dq_min = 0, double dq_max = 0, double tau_min = 0, double tau_max = 0, AIOSDrive* drive = NULL, const std::string& name = "");
    ~AIOSJoint();

    /**
     * \brief Check if current velocity and torque are within limits.
     *
     * \return OUTSIDE_LIMITS if outside the limits (!), SUCCESS otherwise
     */
    setMovementReturnCode_t safetyCheck();

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
