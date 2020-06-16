
/**
 * 
 * \file RobotM3.h
 * \author Vincent Crocher
 * \version 0.1
 * \date 2020-06-16
 * \copyright Copyright (c) 2020
 * 
 * \breif  The<code> RobotM3</ code> class represents an M3 Robot.
 *
 */

#ifndef RobotM3_H_INCLUDED
#define RobotM3_H_INCLUDED

#include <map>

#include "DummyActJoint.h"
#include "Keyboard.h"
#include "Robot.h"
/**
     * \todo Load in paramaters and dictionary entries from JSON file.
     * 
     */
/**
 * \brief Example implementation of the Robot class, representing an X2 Exoskeleton, using DummyActuatedJoint and DummyTrajectoryGenerator.
 * 
 */
class RobotM3 : public Robot {
   private:
    /**
     * \brief motor drive position control profile paramaters, user defined.
     * 
     */
    motorProfile posControlMotorProfile{4000000, 240000, 240000};

   public:
    /**
      * \brief Default <code>RobotM3</code> constructor.
      * Initialize memory for the Exoskelton <code>Joint</code> + sensors. 
      * Load in exoskeleton paramaters to  <code>TrajectoryGenerator.</code>.
      */
    RobotM3();
    ~RobotM3();
    Keyboard keyboard;
//    std::vector<CopleyDrive *> copleyDrives;


    /**
       * \brief Initialises all joints to position control mode. 
       * 
       * \return true If all joints are successfully configured
       * \return false  If some or all joints fail the configuration
       */
    bool initPositionControl();

    /**
       * \brief Initialises all joints to torque control mode.
       *
       * \return true If all joints are successfully configured
       * \return false  If some or all joints fail the configuration
   */
    bool initTorqueControl();


    /**
    * @brief Set the target positions for each of the joints
    * 
    * @param positions a vector of target positions - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    setMovementReturnCode_t setPosition(std::vector<double> positions);

    /**
       * \brief Implementation of Pure Virtual function from <code>Robot</code> Base class.
       * Create designed <code>Joint</code> and <code>Driver</code> objects and load into 
       * Robot joint vector.
       */
    bool initialiseJoints();

    /**
       * \brief Implementation of Pure Virtual function from <code>Robot</code> Base class.
       * Initialize each <code>Drive</code> Objects underlying CANOpen Networking.

      */
    bool initialiseNetwork();
    /**
       * \brief Implementation of Pure Virtual function from <code>Robot</code> Base class.
       * Initialize each <code>Input</code> Object.

      */
    bool initialiseInputs();
    /**
       * \brief Free robot objects vector pointer memory.
       */
    void freeMemory();
    /**
       * \brief update current state of the robot, including input and output devices. 
       * Overloaded Method from the Robot Class. 
       * Example. for a keyboard input this would poll the keyboard for any button presses at this moment in time.
       */
    void updateRobot();
    /**
       * \brief Joint Limit Map between Joint value and min Degrees possible
       * \param int Joint value
       * \return double minDeg 
       */

  /**
    * \todo Move jointMinMap and jointMaxMap to RobotParams.h
    * 
    */
    /**
       * \brief Joint Limit Map between Joint value and max Degrees possible
       * \param int Joint value
       * \return int maxDeg 
       */
    /*std::map<int, double> jointMinMap = {{LEFT_HIP, 70},
                                         {RIGHT_HIP, 70},
                                         {LEFT_KNEE, 0},
                                         {RIGHT_KNEE, 0},
                                         {LEFT_ANKLE, 75},
                                         {RIGHT_ANKLE, 75}};
    *//**
       * \brief Joint Limit Map between Joint value and max Degrees possible
       * \param int Joint value
       * \return int maxDeg 
       */
    /*std::map<int, double> jointMaxMap = {{LEFT_HIP, 210},
                                         {RIGHT_HIP, 210},
                                         {LEFT_KNEE, 120},
                                         {RIGHT_KNEE, 120},
                                         {LEFT_ANKLE, 105},
                                         {RIGHT_ANKLE, 105}};*/
};
#endif /*RobotM3_H*/