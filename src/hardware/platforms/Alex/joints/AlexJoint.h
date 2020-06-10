/**
 * \file AlexJoint.h
 * \author William Campbell 
 * \version 0.1
 * \date 2020-06-10
 * \brief A dummy class to test whether the actuated joint inheritence stuff works
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
 * \brief implementation of the ActuatedJoints class for the Alex Exoskeleton. 
 * 
 * 
 * 
 */
class AlexJoint : public ActuatedJoint {
   private:
    JointKnownPos jointParamaters;
    double lastQCommand = 0;
    long A = 0; /* For use in drive and motor unit conversion, differs for each joints implementation*/
    long B = 0; /* For use in drive and motor unit conversion, differs for each joints implementation*/

    /**
     * \brief converter drive motor count value to joint values (angles)
     * 
     * \params driveValue read from a joints drive object
     * \return double joint angle
    */
    double fromDriveUnits(int driveValue);
    /**
     * \brief converts joint angles to driver motor count values
     * 
     * @param jointValue angle from a robot object
     * \return int driver Value for use by this joints Drive object
     */
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