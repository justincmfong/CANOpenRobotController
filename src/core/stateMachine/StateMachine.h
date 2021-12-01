/**
 * \file StateMachine.h
 * \author William Campbell
 * \version 0.1
 * \date 2019-09-24
 //TODO: UPdate this: a generic state machine, managing a Robot and to be derived fr each app (see...)
 * For more detail on the architecture and mechanics of the state machine class see: https://embeded.readthedocs.io/en/latest/StaeMachines/.
 * \copyright Copyright (c) 2019
 *
 */
/**
 *  @defgroup stateMachine State Machine module
 * A group of abstract classes, used to build event-driven state machines.
 */
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <csignal> //For raise()
#include "logging.h"
#include "State.h"
#include "Robot.h"
#include "LogHelper.h"

class StateMachine;

typedef std::function<bool(StateMachine &)> TransitionCb_t; //TODO: make w/ template and pointer on specialised stateMachine?
typedef std::pair<TransitionCb_t, std::string> Transition_t;


/**
 * @ingroup stateMachine
 * \brief Abstract class representing a state machine. Includes a number of State and Transition objects
 *
 */
class StateMachine {
   public:
    /**
     * \brief Construct a new State Machine object
     *
     */
    StateMachine();
    /**
     * \brief Default destructor
     *
     */
    virtual ~StateMachine(){};
    /**
     * \brief Sets the current state. Note: No check made
     *
     * \param i Pointer to the desired current state.
     */
    void setInitState(std::string state_name);


    /**
     * \brief Calls the entry method of the current state
     *
     */
    void activate();

    /**
     * \brief Processes the state machine. For each possible transition, checks if that transition should be made
     *  If no, calls during() on the current state
     *  If yes, calls exit() on the current state, entry() and then during() on the new state.
     *
     */
    virtual void update();

    /**
     * \brief Custom initialisation of the state machine
     *
     */
    virtual void init() = 0;

    /**
     * \brief End the state machine execution state
     *
     */
    virtual void end() = 0;

    //TODO: doc.
    void setRobot(std::shared_ptr<Robot> r);

    //TODO: doc. If setInitState() is not used to define first execution state, the first added state is used instead.
    void addState(std::string state_name, std::shared_ptr<State> s_ptr);
    //TODO: doc. If setInitState() is not used to define first execution state, the first added state is used instead.
    void addTransition(std::string from, TransitionCb_t t_cb, std::string to);
    //TODO: doc.
    void addTransitionFromAny(TransitionCb_t t_cb, std::string to);

    //TODO: doc.
    std::shared_ptr<State> state(std::string state_name) {
        return _states[state_name];
    }
    //TODO: doc.
    template <typename S>
    std::shared_ptr<S> state(std::string state_name) {
        return std::static_pointer_cast<S>(_states[state_name]);
    }
    //TODO: doc.
    std::shared_ptr<State> state() {
        return _states[_currentState];
    }

    //TODO: doc.
    bool isRunning() { return _running; }

   private:

    /**
     * \brief Hardware update method called every loop (first thing) to update robot state...
     *
     */
    virtual void hwStateUpdate() = 0;

    /**
     * \brief Pointer to the current state
     *
     */
    std::string _currentState;
    std::map<std::string, std::shared_ptr<State>> _states; //Map of states
    std::map<std::string, std::vector<Transition_t>> _transitions; //Map holding for each state a vector of possible std::pair transistions.
    bool _running;

   protected:
    //TODO: Ok like that?
    std::shared_ptr<Robot> _robot = nullptr;

    /**
     * \brief Custom spdlogger allowing to conveniently log Eigen Vectors (among other things)
     * Required to be initialised in the derived state machine init()
     */
    LogHelper logHelper;
};

#endif  //STATEMACHINE_H
