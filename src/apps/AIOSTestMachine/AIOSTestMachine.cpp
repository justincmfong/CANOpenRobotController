#include "AIOSTestMachine.h"


bool goToNextState(StateMachine & SM) {
    AIOSTestMachine & sm = static_cast<AIOSTestMachine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getNb()==1)
        return true;

    //Otherwise false
    return false;
}


AIOSTestMachine::AIOSTestMachine() {
    //Create an M2 Robot and set it to generic state machine
    setRobot(std::make_unique<AIOSRobot>("AIOSDEMO", "AIOSRobot_params.yaml"));

    //Create state instances and add to the State Machine
    addState("TestState", std::make_shared<AIOSDemoState>(robot()));
    addState("StandbyState", std::make_shared<AIOSTransparent>(robot()));

    //Define transitions between states
    addTransition("TestState", &goToNextState, "StandbyState");
    addTransition("StandbyState", &goToNextState, "TestState");

    //Initialize the state machine with first state of the designed state machine
    setInitState("TestState");
}
AIOSTestMachine::~AIOSTestMachine() {
}

/**
 * \brief start function for running any designed statemachine specific functions
 *
 */
void AIOSTestMachine::init() {
    spdlog::debug("AIOSTestMachine::init()");
    if(robot()->initialise()) {
        logHelper.initLogger("AIOSTestMachineLog", "logs/AIOSTestMachine.csv", LogFormat::CSV, true);
        logHelper.add(runningTime(), "Time (s)");
        logHelper.startLogger();
    }
    else {
        spdlog::critical("Failed robot initialisation. Exiting...");
        std::raise(SIGTERM); //Clean exit
    }
}

void AIOSTestMachine::end() {
    StateMachine::end();
}


////////////////////////////////////////////////////////////////
// Events ------------------------------------------------------
///////////////////////////////////////////////////////////////

/**
 * \brief Statemachine to hardware interface method.
 *
 */
void AIOSTestMachine::hwStateUpdate(void) {
    StateMachine::hwStateUpdate();
}
