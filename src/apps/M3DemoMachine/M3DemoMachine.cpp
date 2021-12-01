#include "M3DemoMachine.h"

bool endCalib(StateMachine & sm) {
    M3DemoMachine & SM = static_cast<M3DemoMachine &>(sm); //annoying....

    return (sm.state<M3CalibState>("CalibState"))->isCalibDone(); //annoying as well...
}

bool goToNextState(StateMachine & SM) {
    M3DemoMachine & sm = static_cast<M3DemoMachine &>(SM); //annoying....

    //keyboard or joystick press
    if ( (sm.robot()->joystick->isButtonTransition(3)>0 || sm.robot()->keyboard->getNb()==1) )
        return true;

    //Check incoming command requesting state change
    if ( sm.UIserver->isCmd() ) {
        string cmd;
        vector<double> v;
        sm.UIserver->getCmd(cmd, v);
        if (cmd == "GTNS") { //Go To Next State command received
            sm.UIserver->clearCmd();
            //Acknowledge
            sm.UIserver->sendCmd(string("OK"));

            return true;
        }
    }

    //Otherwise false
    return false;
}

bool standby(StateMachine & SM) {
    M3DemoMachine & sm = (M3DemoMachine &)SM; //annoying....

    if (sm.robot()->keyboard->getQ()==1) {
        return true;
    }
    return false;
}


M3DemoMachine::M3DemoMachine() {
    //robot = std::make_shared<RobotM3>("EMU_MELB", "M3_params.yaml"); //???
    setRobot(std::make_shared<RobotM3>("EMU_MELB", "M3_params.yaml"));
    //robot = new RobotM3("EMU_MELB", "M3_params.yaml");
    //TODO: local template for one occurence only?

    //Create state instances and add to the State Machine
    addState("TestState", std::make_shared<M3DemoState>(this, robot()));
    addState("CalibState", std::make_shared<M3CalibState>(this, robot()));
    addState("StandbyState", std::make_shared<M3MassCompensation>(this, robot()));
    addState("MinJerkState", std::make_shared<M3DemoMinJerkPosition>(this, robot()));
    addState("EndEffState", std::make_shared<M3EndEffDemo>(this, robot()));
    addState("TimingState", std::make_shared<M3SamplingEstimationState>(this, robot()));
    addState("PathState", std::make_shared<M3DemoPathState>(this, robot()));
    addState("ImpedanceState", std::make_shared<M3DemoImpedanceState>(this, robot()));

    //Define transitions between states
    addTransition("CalibState", &endCalib, "StandbyState");
    addTransition("StandbyState", &goToNextState, "MinJerkState");
    addTransition("MinJerkState", &goToNextState, "ImpedanceState");
    addTransition("ImpedanceState", &goToNextState, "PathState");
    addTransition("PathState", &goToNextState, "EndEffState");
    addTransition("EndEffState", &goToNextState, "TimingState");
    addTransition("TimingState", &goToNextState, "StandbyState");
    addTransitionFromAny(&standby, "StandbyState");

    //Initialize the state machine with first state of the designed state machine, using baseclass function.
    setInitState("CalibState");
}
M3DemoMachine::~M3DemoMachine() {
    if(UIserver) {
        delete UIserver;
    }
    //delete robot;//TODO: remove if unique or shared
}

/**
 * \brief start function for running any designed statemachine specific functions
 * for example initialising robot objects.
 *
 */
void M3DemoMachine::init() {
    spdlog::debug("M3DemoMachine::init()");
    if(robot()->initialise()) {
        logHelper.initLogger("M3DemoMachineLog", "logs/M3DemoMachine.csv", LogFormat::CSV, true);
        logHelper.add(time_running, "Time (s)");
        logHelper.add(robot()->getEndEffPosition(), "X");
        logHelper.add(robot()->getEndEffVelocity(), "dX");
        logHelper.add(robot()->getInteractionForce(), "F");
        logHelper.add(robot()->getEndEffAcceleration(), "ddX");
        logHelper.add(robot()->getEndEffVelocityFiltered(), "dXFilt");
        logHelper.startLogger();
//        UIserver = new FLNLHelper(robot, "192.168.6.2");//TODO
        UIserver = new FLNLHelper("192.168.6.2");
    }
    else {
        spdlog::critical("Failed robot initialisation. Exiting...");
        std::raise(SIGTERM); //Clean exit
    }
    time_init = std::chrono::steady_clock::now();
    time_running = 0;
}

void M3DemoMachine::end() {
    if(isRunning()) {
        if(logHelper.isStarted())
            logHelper.endLog();
        UIserver->closeConnection();
        state()->exit();
        robot()->disable();
    }
}


/**
 * \brief Statemachine to hardware interface method. Run any hardware update methods
 * that need to run every program loop update cycle.
 *
 */
void M3DemoMachine::hwStateUpdate(void) {
    time_running = (std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - time_init).count()) / 1e6;
    robot()->updateRobot();
    UIserver->sendState();
}


bool M3DemoMachine::configureMasterPDOs() {
    spdlog::debug("M3DemoMachine::configureMasterPDOs()");
    return robot()->configureMasterPDOs();
}
