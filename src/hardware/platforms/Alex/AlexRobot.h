
/**
 * 
 * \file AlexRobot.h
 * \author William Campbell 
 * \version 0.1
 * \date 2020-06-10
 * \copyright Copyright (c) 2019
 * 
 * \breif  The<code> AlexRobot</ code> class represents an ExoSkeleton Robot in terms of its
 * representation of the Alex exoskeleton hardware whose memory is managed in this class.
 *
 *
 * Version 0.1
 * Date: 07/04/2020
 */

#ifndef AlexRobot_H_INCLUDED
#define AlexRobot_H_INCLUDED

#include <time.h>

#include <map>

#include "AlexJoint.h"
#include "CopleyDrive.h"
#include "DummyTrajectoryGenerator.h"
#include "Keyboard.h"
#include "Robot.h"
#include "RobotParams.h"

/**
     * \todo Load in paramaters and dictionary entries from JSON file.
     * 
     */
/**
 * \brief Example implementation of the Robot class, representing an X2 Exoskeleton, using DummyActuatedJoint and DummyTrajectoryGenerator.
 * 
 */
class AlexRobot : public Robot {
   private:
    /** Parameters associated with Trajectory Progression */
    double currTrajProgress = 0;
    double currTrajTime; /*currently loaded trajectories total time of execution, must be set before begining a trajectory*/
    timespec prevTime;
    /**
     * \brief motor drive position control profile paramaters, user defined.
     * 
     */
    motorProfile posControlMotorProfile{4000000, 240000, 240000};
    //motorProfile posControlMotorProfile{2000000, 140000, 140000};

   public:
    AlexRobot();
    /**
      * \brief Default <code>AlexRobot</code> constructor.
      * Initialize memory for the Exoskelton <code>Joint</code> + sensors. 
      * Load in exoskeleton paramaters to  <code>TrajectoryGenerator.</code>.
      */
    AlexRobot(DummyTrajectoryGenerator *tj);
    ~AlexRobot();
    DummyTrajectoryGenerator *trajectoryGenerator;
    Keyboard keyboard;
    vector<CopleyDrive *> copleyDrives;

    // /**
    //  * \brief Timer Variables for moving through trajectories
    //  *
    //  */
    struct timeval tv, tv_diff, moving_tv, tv_changed, stationary_tv, start_traj, last_tv;

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
      * /brief For each joint, move through(send appropriate commands to joints) the currently 
      * generated trajectory of the TrajectoryGenerator object - this assumes the trajectory and robot is in position control. 
      *
      * /return true if successful
      * /return false if not successful (e.g. any joint not in position control.)
      */
    bool moveThroughTraj();

    /** 
      *  \brief Begin a new trajectory with the currently loaded trajectory paramaters. 
      * Using the <code>AlexRobot</code> current configuration (read in from joint objects) 
      * and the trajecotry generator object, generate and save a spline to move from current 
      * to desired position.
      * 
      */
    void startNewTraj();

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
    void
    freeMemory();
    /**
       * \brief update current state of the robot, including input and output devices. 
       * Overloaded Method from the Robot Class. 
       * Example. for a keyboard input this would poll the keyboard for any button presses at this moment in time.
       */
    void updateRobot();
    /**
       * \brief getter method for currentTrajectory progress variable.
       *
       * \return double currentTrajProgress
       */
    double getCurrTrajProgress();
    /**
    * \todo Move jointMinMap and jointMaxMap to RobotParams.h
    * 
    */
    /**
       * \brief Joint Limit Map between Joint value and max Degrees possible
       * \param int Joint value
       * \return int maxDeg 
       */
    std::map<int, double> jointMinMap = {{LEFT_HIP, 70},
                                         {LEFT_KNEE, 0},
                                         {RIGHT_HIP, 70},
                                         {RIGHT_KNEE, 0},
                                         {LEFT_ANKLE, 75},
                                         {RIGHT_ANKLE, 75}};
    /**
       * \brief Joint Limit Map between Joint value and max Degrees possible
       * \param int Joint value
       * \return int maxDeg 
       */
    std::map<int, double> jointMaxMap = {{LEFT_HIP, 210},
                                         {LEFT_KNEE, 120},
                                         {RIGHT_HIP, 210},
                                         {RIGHT_KNEE, 120},
                                         {LEFT_ANKLE, 105},
                                         {RIGHT_ANKLE, 105}};
};
#endif /*AlexRobot_H*/