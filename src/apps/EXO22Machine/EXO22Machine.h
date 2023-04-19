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
#ifndef EXO22_SM_H
#define EXO22_SM_H

#include "AIOSRobot.h"
#include "StateMachine.h"

// State Classes
#include "EXO22States.h"

/**
 * @brief Example implementation of a StateMachine for the AIOSTest class. States should implemented M2DemoState
 *
 */
class EXO22Machine : public StateMachine {

   public:
    EXO22Machine();
    ~EXO22Machine();
    void init();

    AIOSRobot *robot() { return static_cast<AIOSRobot*>(_robot.get()); } //!< Robot getter with specialised type (lifetime is managed by Base StateMachine)
};

#endif /*EXO22_SM_H*/
