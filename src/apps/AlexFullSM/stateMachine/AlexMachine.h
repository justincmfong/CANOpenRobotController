/**
 * \file AlexMachine.h
 * \author William Campbell 
 * \version 0.1
 * \date 2020-06-01
 * \copyright Copyright (c) 2020
 *
 * /brief The <code>AlexMachine</code> class represents the Alex teamsimplementation of an exoskeleton state machine 
 * with five states. 
 * Initialisation, sitting, standing, standing up and sitting down./todo: add walking states. 
 * The machine uses the AlexTrajectory generator to perform position control opperations using an X2 hardware platform.
 * 
 *  
 * For more detail on the architecture, mechanics and design of the project please go to embedded.readthedocs.io
 * 
 *  State transition Diagram.
 * 
 *         startExo             startStand
 *  initState +-----> sitting +---------> standingUp
 *                      ^                  +
 *           EndTraj    |                  | EndTraj
 *                      |                  |
 *                      +                  |
 *                 sittingDwn <---------+ standing
 *                              startSit
 * 
 * Version 0.1
 * Date: 07/04/2020
 *
 */
#ifndef ALEX_SM_H
#define ALEX_SM_H

#include <sys/time.h>

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "AlexRobot.h"
#include "ExoTestState.h"
#include "StateMachine.h"

// State Classes
#include "DebugMacro.h"
#include "InitState.h"
#include "Sitting.h"
#include "SittingDwn.h"
#include "Standing.h"
#include "StandingUp.h

/**
 * @brief implementation of a StateMachine for the ExoRobot class. States implement ExoTestState
 * 
 */
class AlexMachine : public StateMachine {
   public:
    bool running = false;
    /**
     *  \todo Pilot Parameters would be set in constructor here
     * 
     */
    AlexMachine();
    void init();
    void activate();
    void deactivate();

    void hwStateUpdate();
    State *gettCurState();
    void initRobot(AlexRobot *rb);
    bool trajComplete;
    AlexTrajectoryGenerator *trajectoryGenerator;

    /**
     * Pointers to the relevant states - initialised in init 
     * 
     */
    InitState *initState;
    SittingDwn *sittingDwn;
    StandingUp *standingUp;
    Sitting *sitting;
    Standing *standing;

   protected:
    AlexRobot *robot; /*<!Pointer to the Robot*/

   private:
    /**
     * 
     * \brief Event Objects defined using Macro defined in StateMachine.h
     * Defines the Class itself, as well as initialises an object of that class
     * An events check function are defined in the .cpp file.
    */
    EventObject(EndTraj) * endTraj;
    EventObject(IsAPressed) * isAPressed;
    EventObject(StartButtonsPressed) * startButtonsPressed;
    EventObject(StartExo) * startExo;
    EventObject(StartSit) * startSit;
    EventObject(StartStand) * startStand;
};

#endif /*EXO_SM_H*/
