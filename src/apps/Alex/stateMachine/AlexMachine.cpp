
#include "AlexMachine.h"

#define OWNER ((AlexMachine *)owner)

AlexMachine::AlexMachine() {
    trajectoryGenerator = new AlexTrajectoryGenerator(6);
    robot = new AlexRobot(trajectoryGenerator);
    // Create PRE-DESIGNED State Machine events and state objects.
    isAPressed = new IsAPressed(this);
    endTraj = new EndTraj(this);
    startButtonsPressed = new StartButtonsPressed(this);
    startExo = new StartExo(this);
    startSit = new StartSit(this);
    startStand = new StartStand(this);
    initState = new InitState(this, robot, trajectoryGenerator);
    standing = new Standing(this, robot, trajectoryGenerator);
    sitting = new Sitting(this, robot, trajectoryGenerator);
    standingUp = new StandingUp(this, robot, trajectoryGenerator);
    sittingDwn = new SittingDwn(this, robot, trajectoryGenerator);
    steppingFirstLeft = new SteppingFirstLeft(this);
    leftForward = new LeftForward(this);
    steppingRight = new SteppingRight(this);
    rightForward = new RightForward(this);
    steppingLeft = new SteppingLeft(this);
    steppingLastRight = new SteppingLastRight(this);
    steppingLastLeft = new SteppingLastLeft(this);
    errorState = new ErrorState(this);

    /**
     * \brief Moving Trajectory Transitions
     *
     */
    NewTransition(standingUp, endTraj, standing);
    NewTransition(sittingDwn, endTraj, sitting);
    // Stepping
    NewTransition(steppingRight, endTraj, rightForward);
    NewTransition(steppingFirstLeft, endTraj, leftForward);
    NewTransition(steppingLeft, endTraj, leftForward);
    NewTransition(steppingLastRight, endTraj, standing);
    NewTransition(steppingLastLeft, endTraj, standing);
    /**
     * \brief Stationary to moving state transitions (OD.NM controls type of walk)
     *
     */
    NewTransition(initState, startExo, sitting);
    NewTransition(sitting, startStand, standingUp);
    NewTransition(standing, startSit, sittingDwn);
    NewTransition(standing, startWalk, steppingFirstLeft);
    NewTransition(leftForward, startWalk, steppingRight);
    NewTransition(rightForward, startWalk, steppingLeft);
    NewTransition(leftForward, feetTogether, steppingLastRight);
    NewTransition(rightForward, feetTogether, steppingLastLeft);

    /**
     * \brief  Error State Transitions
     *
     */
    NewTransition(errorState, resetButtonsPressed, initState);
    NewTransition(sitting, isRPressed, errorState);
    NewTransition(standing, isRPressed, errorState);
    NewTransition(standingUp, isRPressed, errorState);
    NewTransition(sittingDwn, isRPressed, errorState);
    NewTransition(steppingFirstLeft, isRPressed, errorState);
    NewTransition(leftForward, isRPressed, errorState);
    NewTransition(steppingRight, isRPressed, errorState);
    NewTransition(rightForward, isRPressed, errorState);
    NewTransition(steppingLeft, isRPressed, errorState);
    NewTransition(steppingLastRight, isRPressed, errorState);
    NewTransition(steppingLastLeft, isRPressed, errorState);
    //Initialize the state machine with first state of the designed state machine, using baseclass function.
    StateMachine::initialize(initState);
}
/**
 * \brief start function for running any designed statemachine specific functions
 * for example initialising robot objects.
 * 
 */
void AlexMachine::init() {
    robot->initialise();
    running = true;
}

////////////////////////////////////////////////////////////////
// Events ------------------------------------------------------------
///////////////////////////////////////////////////////////////
/**
     * \brief poll the trajectory Generators flag to see if the currently loaded motion is complete
     *
     */
bool AlexMachine::EndTraj::check() {
    /*For alex Traj Generator*/
    return OWNER->trajectoryGenerator->isTrajectoryFinished(OWNER->robot->getCurrTrajProgress());
}
bool AlexMachine::IsAPressed::check(void) {
    if (OWNER->robot->keyboard.getA() == true) {
        return true;
    }
    return false;
}
bool AlexMachine::StartButtonsPressed::check(void) {
    if (OWNER->robot->keyboard.getW() == true) {
        return true;
    }
    return false;
}
bool AlexMachine::StartExo::check(void) {
    if (OWNER->robot->keyboard.getS() == true) {
        std::cout << "LEAVING INIT and entering Sitting" << endl;
        return true;
    }
    return false;
}
bool AlexMachine::StartStand::check(void) {
    if (OWNER->robot->keyboard.getW() == true) {
        return true;
    }
    return false;
}

bool AlexMachine::StartSit::check(void) {
    if (OWNER->robot->keyboard.getW()) {
        return true;
    }
    return false;
}
bool AlexMachine::startWalk::check(void) {
    if (OWNER->robot->keyboard.getW()) {
        return true;
    }
    return false;
}
bool AlexMachine::feetTogether::check(void) {
    if (OWNER->robot->keyboard.getS()) {
        return true;
    }
    return false;
}
/**
 * \brief Statemachine to hardware interface method. Run any hardware update methods
 * that need to run every program loop update cycle.
 * 
 */
void AlexMachine::hwStateUpdate(void) {
    robot->updateRobot();
}
