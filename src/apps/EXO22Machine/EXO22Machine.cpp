#include "EXO22Machine.h"


// Dummy Test just for testing
bool dummyTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    // For testing only. 

    //Otherwise false
    return false;
}

// Transition to the Calibration State - Requires Keypress "C"
bool calibTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='C')
        return true;

    //Otherwise false
    return false;
}

// Calibration Finish - Polls CalibState to see whether it is finished or not
bool isCalibratedTransition(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    return (sm.state<EXO22CalibState>("CalibState"))->isCalibDone();
}

// Method for exiting out of moving state (i.e. when it is finished)
bool trajectoryFinished(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    if (static_cast<EXO22MovingState*>(sm.state().get())->isTrajComplete()){
        return true;
    }

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='F')
        return true;

    //Otherwise false
    return false;
}

// Trigger to start a standing movement ("U")
bool startStanding(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='U')
        return true;

    //Otherwise false
    return false;
}

// Trigger to start a sitting movement ("D")
bool startSitting(StateMachine & SM) {
    EXO22Machine & sm = static_cast<EXO22Machine &>(SM); //Cast to specific StateMachine type

    //keyboard press
    if (sm.robot()->keyboard->getKeyUC()=='D')
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

EXO22Machine::EXO22Machine() {
    setRobot(std::make_unique<AIOSRobot>("AIOSDUMMY", "AIOSRobot_params.yaml"));

    // Define everything in degrees
    std::vector<ViaPoint> sittingViaPoints;
    Eigen::VectorXd sit1(6);
    sit1 << 0, 0, 90, 90, 90, 90;
    sittingViaPoints.push_back({sit1*180/M_PI, 1}); 

    std::vector<ViaPoint> standingViaPoints;
    Eigen::VectorXd stand1(6);
    stand1 << 0, 0, 0, 0, 0, 0;
    standingViaPoints.push_back({stand1*180/M_PI, 1});

    //Create state instances and add to the State Machine
    addState("InitState", std::make_shared<EXO22StationaryState>(robot(), "Initial State"));
    addState("CalibrationState", std::make_shared<EXO22CalibState>(robot()));
    addState("StandingState", std::make_shared<EXO22StationaryState>(robot(), "Standing (Stationary) State"));
    addState("M_SittingState", std::make_shared<EXO22MovingState>(robot(), sittingViaPoints, "Sitting Down (Moving) State"));
    addState("SittingState", std::make_shared<EXO22StationaryState>(robot(), "Sitting (Stationary) State"));
    addState("M_StandingState", std::make_shared<EXO22MovingState>(robot(), standingViaPoints, "Standing Up (Moving) State"));

    //Define transitions between states
    addTransition("InitState", &dummyTransition, "InitState");
    addTransition("InitState", &calibTransition, "CalibrationState");
    addTransition("CalibrationState", &isCalibratedTransition, "InitState");
    addTransition("InitState", &startStanding, "M_StandingState");
    addTransition("M_StandingState", &trajectoryFinished, "StandingState");
    addTransition("StandingState", &startSitting, "M_SittingState");
    addTransition("M_SittingState", &trajectoryFinished, "SittingState");
    addTransition("SittingState", &startStanding, "M_StandingState");

    //Initialize the state machine with first state of the designed state machine
    setInitState("InitState");
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