/**
 * \file AIOSTestMachine.h
 * \author Vincent Crocher
 * /brief The AIOSTestMachine class represents an example implementation of an M2 state machine.
 * \version 0.2
 * \date 2021-02-05
 *
 * \copyright Copyright (c) 2020 - 2021
 *
 */
#ifndef AIOS_SM_H
#define AIOS_SM_H

#include "AIOSRobot.h"
#include "StateMachine.h"

// State Classes
#include "AIOSTestStates.h"

/**
 * @brief Example implementation of a StateMachine for the AIOSTest class. States should implemented M2DemoState
 *
 */
class AIOSTestMachine : public StateMachine {

   public:
    AIOSTestMachine();
    ~AIOSTestMachine();
    void init();
    void end();

    void hwStateUpdate();

    AIOSRobot *robot() { return static_cast<AIOSRobot*>(_robot.get()); } //!< Robot getter with specialised type (lifetime is managed by Base StateMachine)
};

#endif /*AIOS_SM_H*/
