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

#include "ActuatedJoint.h"
typedef struct JointKnownPos {
    int motorCountA;
    int motorCountB;
    int motorDegPosA;
    int motorDegPosB;
} JointKnownPos;
/**
 * \brief Example implementation of the ActuatedJoints class. 
 * 
 * Important to note the simple implementation between the driveValue and jointValue
 * 
 */
class AlexJoint : public ActuatedJoint {
   private:
    JointKnownPos jointParamaters;
    double lastQCommand = 0;
    long A = 0; /* For use in drive and motor unit conversion, differs for each joints implementation*/
    long B = 0; /* For use in drive and motor unit conversion, differs for each joints implementation*/

    /**
     * \brief  These functions are defined here for example
    *   In this implementation they do essentially nothing 
    * - it's a straight 1:1 relation between drive and motor units
    */
    double fromDriveUnits(int driveValue);
    int toDriveUnits(double jointValue);
    /**
     * \brief precalulate A and B values for faster linear interpolating calculation of y and x in y = Ax+B
     *  Note: these values are used in determining motor degree position from motor count readings and vice versa.
     * 
     * The function uses the specified joints joint Paramaters, set at construction.
     * 
     */
    void linearInterpolatePreCalc();

   public:
    AlexJoint(int jointID, double jointMin, double jointMax, Drive *drive, JointKnownPos jointParams);
    bool updateValue();
    setMovementReturnCode_t setPosition(double desQ);
    bool initNetwork();
    double getQ();
};

#endif