/**
 * /file AIOSDemoState.h
 * \author Vincent Crocher
 * \version 0.1
 * \date 2020-12-09
 *
 * \copyright Copyright (c) 2020
 *
 */

#ifndef AIOSTestSTATE_H_DEF
#define AIOSTestSTATE_H_DEF

#include "AIOSRobot.h"
#include "State.h"

using namespace std;

/**
 * \brief Conversion from a timespec structure to seconds (double)
 *
 */
double timeval_to_sec(struct timespec *ts);

/**
 * \brief Generic state type for used with AIOSTestMachine, providing running time and iterations number: been superseeded by default state.
 *
 */
class AIOSTimedState : public State {
   protected:
    /**
    *  \todo Might be good to make these Const
    *
    */
    AIOSRobot *robot;                               /*<!Pointer to state machines robot object*/

    AIOSTimedState(AIOSRobot *AIOS, const char *name = NULL) : State(name), robot(AIOS){};

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

class AIOSStationaryState : public AIOSTimedState {
   public:
    AIOSStationaryState(AIOSRobot *AIOS, const char *name = "AIOS Stationary State"):AIOSTimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};

/**
 * \brief A state in which the Robot is running in Position Control
 *
 */
class AIOSPosControlState : public AIOSTimedState {
   private:
    Eigen::VectorXd targetPos;

   public:
    AIOSPosControlState(AIOSRobot *AIOS, const char *name = "AIOS Position Control State") : AIOSTimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};

/**
 * \brief A state in which the Robot is running in Velocity Control
 *
 */
class AIOSVelControlState : public AIOSTimedState {
    private:
     Eigen::VectorXd targetVel;
    public : AIOSVelControlState(AIOSRobot *AIOS, const char *name = "AIOS Velocity Control State") : AIOSTimedState(AIOS, name){};

     void entryCode(void);
     void duringCode(void);
     void exitCode(void);
};

/**
 * \brief A state in which the Robot is running in Torque Control
 *
 */
class AIOSTorqControlState : public AIOSTimedState {
    private:
    Eigen::VectorXd targetTorq;
   public:
    AIOSTorqControlState(AIOSRobot *AIOS, const char *name = "AIOS Torque Control State") : AIOSTimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};

/**
 * @brief A state that send the check error command to the robot. 
 * 
 */
class AIOSCheckErrorState : public AIOSTimedState {
   public:
    AIOSCheckErrorState(AIOSRobot *AIOS, const char *name = "AIOS Check Error State") : AIOSTimedState(AIOS, name){};

    void entryCode(void);
    void duringCode(void);
    void exitCode(void);
};

#endif
