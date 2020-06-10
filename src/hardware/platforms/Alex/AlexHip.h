/**
 * \file AlexJoint.h
 * \author Justin Fong
 * \brief A dummy class to test whether the actuated joint inheritence stuff works
 * \version 0.1
 * \date 2020-04-09
 * 
 * \copyright Copyright (c) 2020
 * 
 */
#ifndef AlexJoint_H_INCLUDED
#define AlexJoint_H_INCLUDED

#include "AlexJoint.h"

/**
 * \brief Hip jointimplementation of the Alex Joint class. 
 * 
 * 
 */
class AlexHip : public AlexJoint {
   private:
    //Hip motor reading and corresponding angle. Used for mapping between degree and motor values.
#define HIP_MOTOR_COUNT_POS1 (250880)
#define HIP_MOTOR_DEG1 (90)
#define HIP_MOTOR_POS2 (0)
#define HIP_MOTOR_DEG2 (180)
    /**
     * \brief  These functions are defined here for example
    *   In this implementation they do essentially nothing 
    * - it's a straight 1:1 relation between drive and motor units
    */
    double fromDriveUnits(int driveValue) { return driveValue / 10000; };
    int toDriveUnits(double jointValue) { return jointValue * 10000; };

   public:
    AlexHip(int jointID, double jointMin, double jointMax, Drive *drive);
};

#endif