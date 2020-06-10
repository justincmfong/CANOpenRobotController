/**
 * @file AlexHip.cpp
 * @author William Campbell
 * @brief 
 * @version 0.1
 * @date 2020-06-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "AlexHip.h"

#include <iostream>

#include "DebugMacro.h"

AlexHip::AlexHip(int jointID, double jointMin, double jointMax, Drive *drive) : ActuatedJoint(jointID, jointMin, jointMax, drive) {
    DEBUG_OUT("MY JOINT ID: " << this->id)
    // Do nothing else
}
