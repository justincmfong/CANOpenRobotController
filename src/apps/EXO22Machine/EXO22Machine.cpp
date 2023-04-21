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

EXO22Machine::EXO22Machine() {
    setRobot(std::make_unique<AIOSRobot>("AIOSDEMO", "AIOSRobot_params.yaml"));

    std::vector<ViaPoint> sittingViaPoints;
    sittingViaPoints.push_back({Eigen::VectorXd::Zero(6), 1}); 

    std::vector<ViaPoint> standingViaPoints {
        {Eigen::VectorXd::Zero(6), 1}
    }; 

    //Create state instances and add to the State Machine
    addState("InitState", std::make_shared<EXO22StationaryState>(robot()));
    addState("CalibrationState", std::make_shared<EXO22CalibState>(robot()));
    addState("StandingState", std::make_shared<EXO22StationaryState>(robot(), "Standing (Stationary) State"));
    addState("M_SittingState", std::make_shared<EXO22MovingState>(robot(), sittingViaPoints, "Sitting Down (Moving) State"));
    addState("SittingState", std::make_shared<EXO22StationaryState>(robot(), "Sitting (Stationary) State"));
    addState("M_StandingState", std::make_shared<EXO22MovingState>(robot(), standingViaPoints, "Standing Up (Moving) State"));


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