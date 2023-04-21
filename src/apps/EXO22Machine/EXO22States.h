/**
 * /file EXO22States.h
 * \author Justin Fong/Vincent Crocher
 * \version 0.1
 * \date 2023-04-19
 *
 * \copyright Copyright (c) 2023
 *
 */

#ifndef EXO22STATE_H_DEF
#define EXO22STATE_H_DEF

#include <algorithm>
#include <vector>

#include "AIOSRobot.h"
#include "State.h"

#include "EXO22Trajectory.h"

//using namespace std;

/**
 * \brief Conversion from a timespec structure to seconds (double)
 *
 */
double timeval_to_sec(struct timespec *ts);

/**
 * \brief Generic state type for used with AIOSTestMachine, providing running time and iterations number: been superseeded by default state.
 *
 */
class EXO22TimedState : public State {
   protected:
    /**
    *  \todo Might be good to make these Const
    *
    */
    AIOSRobot *robot;                               /*<!Pointer to state machines robot object*/

    EXO22TimedState(AIOSRobot *AIOS, const char *name = NULL) : State(name), robot(AIOS){};

   private:
    void entry(void) final {
        std::cout
        << "==================================" << std::endl
        << " STARTING  " << getName() << std::endl
        << "----------------------------------" << std::endl
        << std::endl;

        //Actual state entry
        entryCode();
    };
    void during(void) final {
        //Actual state during
        duringCode();
    };
    void exit(void) final {
        exitCode();
        std::cout
        << "----------------------------------" << std::endl
        << "EXIT "<< getName() << std::endl
        << "==================================" << std::endl
        << std::endl;
    };

   public:
    virtual void entryCode(){};
    virtual void duringCode(){};
    virtual void exitCode(){};
};

/**
 * \brief Position calibration: go to the psitive mechanical stop of each joint in torque control (damped) until zero velocity.
 *
 */
class EXO22CalibState : public EXO22TimedState {
   public:
    EXO22CalibState(AIOSRobot * AIOS, const char *name = "AIOS Calib State"):EXO22TimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);

    bool isCalibDone() {return calibDone;}

   private:
    double maxTorque=0, b=0;
    VX stop_reached_time;
    std::vector<bool> at_stop;
    bool calibDone=false;

};

/**
 * \brief A state in which the Robot is running in Position Control
 *
 */
class EXO22MovingState : public EXO22TimedState {
   private:
    // Trajectory Definition
    // List of viapoints for this object 
    std::vector<ViaPoint> viaPoints; 

    // Trajectory Object
    // A trajectory object which constructs 
    EXO22Trajectory * traj;
    // Flag to determine whether trajectory is complete or not
    
    bool trajComplete = false;

   public:
    EXO22MovingState(AIOSRobot *AIOS, std::vector<ViaPoint> vp, const char *name = "EXO22 Moving State") : EXO22TimedState(AIOS, name){
        viaPoints = vp;
    };

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};

/**
 * \brief A state in which the Robot is running in Position Control
 *
 */
class EXO22StationaryState : public EXO22TimedState {
   public:
    EXO22StationaryState(AIOSRobot *AIOS, const char *name = "EXO22 Stationary State") : EXO22TimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};


/**
 * @brief A state that send the check error command to the robot.
 *
 */
class EXO22CheckErrorState : public EXO22TimedState {
   public:
    EXO22CheckErrorState(AIOSRobot *AIOS, const char *name = "EXO22 Check Error State") : EXO22TimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};

#endif //EXO22STATE_H_DEF
