#include "AIOSTestMachine.h"

bool isCalibratedTransition(StateMachine & SM) {
    AIOSTestMachine & sm = static_cast<AIOSTestMachine &>(SM); //Cast to specific StateMachine type

    return (sm.state<AIOSCalibState>("CalibState"))->isCalibDone();
}

bool calibTransition(StateMachine & SM) {
    AIOSTestMachine & sm = static_cast<AIOSTestMachine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='C')
        return true;

    //Otherwise false
    return false;
}

bool trajTransition(StateMachine & SM) {
    AIOSTestMachine & sm = static_cast<AIOSTestMachine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='T')
        return true;

    //Otherwise false
    return false;
}

bool posTransition(StateMachine & SM) {
    AIOSTestMachine & sm = static_cast<AIOSTestMachine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getNb()==1)
        return true;

    //Otherwise false
    return false;
}

bool velTransition(StateMachine &SM) {
    AIOSTestMachine &sm = static_cast<AIOSTestMachine &>(SM);  // Cast to specific StateMachine type

    // keyboard press
    if (sm.robot()->keyboard->getNb() == 2)
        return true;

    // Otherwise false
    return false;
}
bool torqTransition(StateMachine &SM) {
    AIOSTestMachine &sm = static_cast<AIOSTestMachine &>(SM);  // Cast to specific StateMachine type

    // keyboard press
    if (sm.robot()->keyboard->getNb() == 3)
        return true;

    // Otherwise false
    return false;
}

bool errorTransition(StateMachine &SM) {
    AIOSTestMachine &sm = static_cast<AIOSTestMachine &>(SM);  // Cast to specific StateMachine type

    // keyboard press
    if (sm.robot()->keyboard->getNb() == 0)
        return true;

    // Otherwise false
    return false;
}

AIOSTestMachine::AIOSTestMachine() {
    //Create an M2 Robot and set it to generic state machine
    setRobot(std::make_unique<AIOSRobot>("AIOSDEMO", "AIOSRobot_params.yaml"));

    //Create state instances and add to the State Machine
    addState("StationaryState", std::make_shared<AIOSStationaryState>(robot()));
    addState("CalibState", std::make_shared<AIOSCalibState>(robot()));
    addState("PositionControl", std::make_shared<AIOSPosControlState>(robot()));
    addState("VelocityControl", std::make_shared<AIOSVelControlState>(robot()));
    addState("TorqueControl", std::make_shared<AIOSTorqControlState>(robot()));
    addState("Trajectory", std::make_shared<AIOSPosTrajState>(robot()));
    addState("ErrorCheck", std::make_shared<AIOSCheckErrorState>(robot()));

    //Define transitions between states
    addTransition("StationaryState", &calibTransition, "CalibState");
    addTransition("CalibState", &isCalibratedTransition, "StationaryState");
    addTransition("StationaryState", &posTransition, "PositionControl");
    addTransition("PositionControl", &posTransition, "StationaryState");
    addTransition("StationaryState", &velTransition, "VelocityControl");
    addTransition("VelocityControl", &velTransition, "StationaryState");
    addTransition("StationaryState", &torqTransition, "TorqueControl");
    addTransition("TorqueControl", &torqTransition, "StationaryState");
    addTransition("StationaryState", &trajTransition, "Trajectory");
    addTransition("Trajectory", &trajTransition, "StationaryState");
    addTransitionFromAny(&errorTransition, "ErrorCheck");
    addTransition("ErrorCheck", &errorTransition, "StationaryState");

    //Initialize the state machine with first state of the designed state machine
    setInitState("StationaryState");
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
    }
    else {
        spdlog::critical("Failed robot initialisation. Exiting...");
        std::raise(SIGTERM); //Clean exit
    }
}
