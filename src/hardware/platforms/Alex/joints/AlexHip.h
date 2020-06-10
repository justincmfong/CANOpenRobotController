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
#ifndef AlexHip_H_INCLUDED
#define AlexHip_H_INCLUDED

#include "AlexJoint.h"

/**
 * \brief Hip jointimplementation of the Alex Joint class. 
 * 
 * 
 */
class AlexHip : public AlexJoint {
   private:
   public:
    AlexHip(int jointID, double jointMin, double jointMax, Drive *drive, JointKnownPos jointParams);
};

#endif