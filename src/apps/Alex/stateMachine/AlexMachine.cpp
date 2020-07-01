
#include "AlexMachine.h"

#define OWNER ((AlexMachine *)owner)

AlexMachine::AlexMachine() {
    trajectoryGenerator = new AlexTrajectoryGenerator(6);
    robot = new AlexRobot(trajectoryGenerator);
    // Events
    isAPressed = new IsAPressed(this);
    endTraj = new EndTraj(this);
    startButtonsPressed = new StartButtonsPressed(this);
    startExo = new StartExo(this);
    startSit = new StartSit(this);
    startStand = new StartStand(this);
    startWalk = new StartWalk(this);
    feetTogether = new FeetTogether(this);
    resetButtonsPressed = new ResetButtons(this);
    standSelect = new StandSelect(this);
    sitSelect = new SitSelect(this);
    walkSelect = new WalkSelect(this);
    //States
    initState = new InitState(this, robot, trajectoryGenerator);
    standing = new Standing(this, robot, trajectoryGenerator);
    sitting = new Sitting(this, robot, trajectoryGenerator);
    standingUp = new StandingUp(this, robot, trajectoryGenerator);
    sittingDwn = new SittingDwn(this, robot, trajectoryGenerator);
    steppingFirstLeft = new SteppingFirstLeft(this, robot, trajectoryGenerator);
    leftForward = new LeftForward(this, robot, trajectoryGenerator);
    steppingRight = new SteppingRight(this, robot, trajectoryGenerator);
    rightForward = new RightForward(this, robot, trajectoryGenerator);
    steppingLeft = new SteppingLeft(this, robot, trajectoryGenerator);
    steppingLastRight = new SteppingLastRight(this, robot, trajectoryGenerator);
    steppingLastLeft = new SteppingLastLeft(this, robot, trajectoryGenerator);
    errorState = new ErrorState(this, robot, trajectoryGenerator);

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
    NewTransition(sitting, standSelect, standingUp);
    NewTransition(standing, sitSelect, sittingDwn);
    NewTransition(standing, walkSelect, steppingFirstLeft);
    NewTransition(leftForward, walkSelect, steppingRight);
    NewTransition(rightForward, walkSelect, steppingLeft);
    NewTransition(leftForward, feetTogether, steppingLastRight);
    NewTransition(rightForward, feetTogether, steppingLastLeft);

    /**
     * \brief  Error State Transitions
     *
     */
    // NewTransition(errorState, resetButtonsPressed, initState);
    // NewTransition(sitting, isRPressed, errorState);
    // NewTransition(standing, isRPressed, errorState);
    // NewTransition(standingUp, isRPressed, errorState);
    // NewTransition(sittingDwn, isRPressed, errorState);
    // NewTransition(steppingFirstLeft, isRPressed, errorState);
    // NewTransition(leftForward, isRPressed, errorState);
    // NewTransition(steppingRight, isRPressed, errorState);
    // NewTransition(rightForward, isRPressed, errorState);
    // NewTransition(steppingLeft, isRPressed, errorState);
    // NewTransition(steppingLastRight, isRPressed, errorState);
    // NewTransition(steppingLastLeft, isRPressed, errorState);
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
    if (OWNER->robot->keyboard.getA() == true) {
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
    if (OWNER->robot->keyboard.getA() == true) {
        DEBUG_OUT("START STAND PRESSED")
        return true;
    }
    return false;
}

bool AlexMachine::StartSit::check(void) {
    if (OWNER->robot->keyboard.getA() == true) {
        DEBUG_OUT("START SIT PRESSED")
        return true;
    }
    return false;
}
bool AlexMachine::StartWalk::check(void) {
    if (OWNER->robot->keyboard.getS() == true) {
        DEBUG_OUT("START WALK PRESSED")
        return true;
    }
    return false;
}
bool AlexMachine::FeetTogether::check(void) {
    if (OWNER->robot->getCurrentMotion() == RobotMode::NORMALWALK && OWNER->robot->keyboard.getA()) {
        return true;
    }
    return false;
}

bool AlexMachine::IsRPressed::check(void) {
    if (OWNER->robot->keyboard.getX() == true) {
        return true;
    }
    return false;
}
bool AlexMachine::ResetButtons::check(void) {
    if (OWNER->robot->keyboard.getS() == true) {
        return true;
    }
    return false;
}
bool AlexMachine::StandSelect::check(void) {
    if (OWNER->robot->getCurrentMotion() == RobotMode::STNDUP && OWNER->robot->keyboard.getA()) {
        DEBUG_OUT("Stand selected! Begin standing up")
        return true;
    }
    return false;
}
bool AlexMachine::SitSelect::check(void) {
    if (OWNER->robot->getCurrentMotion() == RobotMode::SITDWN && OWNER->robot->keyboard.getA()) {
        DEBUG_OUT("Sit selected! Begin standing up")
        return true;
    }
    return false;
}
bool AlexMachine::WalkSelect::check(void) {
    // \todo be any range of walking motions
    if (OWNER->robot->getCurrentMotion() == RobotMode::NORMALWALK && OWNER->robot->keyboard.getS()) {
        DEBUG_OUT("Normal walk selected begin left step")
        return true;
    } else if (OWNER->robot->getNextMotion() == RobotMode::UNEVEN && OWNER->robot->keyboard.getS()) {
        DEBUG_OUT("Uneven step selected begin left step")
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
