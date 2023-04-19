#include "EXO22Machine.h"

bool isCalibratedTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    return (sm.state<EXO22CalibState>("CalibState"))->isCalibDone();
}

bool calibTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='C')
        return true;

    //Otherwise false
    return false;
}

bool trajTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='T')
        return true;

    //Otherwise false
    return false;
}

bool posTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getNb()==1)
        return true;

    //Otherwise false
    return false;
}

bool velTransition(StateMachine &SM) {
    EXO22Machine &sm = static_cast<EXO22Machine &>(SM);  // Cast to specific StateMachine type

    // keyboard press
    if (sm.robot()->keyboard->getNb() == 2)
        return true;

    // Otherwise false
    return false;
}
bool torqTransition(StateMachine &SM) {
    EXO22Machine &sm = static_cast<EXO22Machine &>(SM);  // Cast to specific StateMachine type

    // keyboard press
    if (sm.robot()->keyboard->getNb() == 3)
        return true;

    // Otherwise false
    return false;
}

bool errorTransition(StateMachine &SM) {
    EXO22Machine &sm = static_cast<EXO22Machine &>(SM);  // Cast to specific StateMachine type

    // keyboard press
    if (sm.robot()->keyboard->getNb() == 0)
        return true;

    // Otherwise false
    return false;
}

EXO22Machine::EXO22Machine() {
    setRobot(std::make_unique<AIOSRobot>("AIOSDEMO", "AIOSRobot_params.yaml"));

    //Create state instances and add to the State Machine
    addState("StationaryState", std::make_shared<EXO22StationaryState>(robot()));


    //Define transitions between states
    addTransition("StationaryState", &calibTransition, "StationaryState");

    //Initialize the state machine with first state of the designed state machine
    setInitState("StationaryState");
}
EXO22Machine::~EXO22Machine() {
}

/**
 * \brief start function for running any designed statemachine specific functions
 *
 */
void EXO22Machine::init() {
    spdlog::debug("EXO22Machine::init()");
    if(robot()->initialise()) {
        logHelper.initLogger("EXO22MachineLog", "logs/EXO22Machine.csv", LogFormat::CSV, true);
        logHelper.add(runningTime(), "Time (s)");
    }
    else {
        spdlog::critical("Failed robot initialisation. Exiting...");
        std::raise(SIGTERM); //Clean exit
    }
}