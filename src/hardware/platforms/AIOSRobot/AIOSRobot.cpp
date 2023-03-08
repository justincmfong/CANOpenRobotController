#include "AIOSRobot.h"

AIOSRobot::AIOSRobot(std::string robot_name, std::string yaml_config_file) : Robot(robot_name, yaml_config_file) {
    spdlog::debug("Robot ({}) object created", robotName);

    //Check if YAML file exists and contain robot parameters: retrieve expected robot structure (IP, nb of joints, actuators IDs...)
    initialiseFromYAML(yaml_config_file);

    //Map AIOS library log level to general one
    switch(SPDLOG_ACTIVE_LEVEL)
    {
        case SPDLOG_LEVEL_TRACE:
            fourierSetLogLevel("DEBUG");
            break;
        case SPDLOG_LEVEL_DEBUG:
            fourierSetLogLevel("DEBUG");
            break;
         case SPDLOG_LEVEL_INFO:
            fourierSetLogLevel("INFO");
            break;
        case SPDLOG_LEVEL_WARN:
            fourierSetLogLevel("WARN");
            break;
        case SPDLOG_LEVEL_ERROR:
            fourierSetLogLevel("ERROR");
            break;
        case SPDLOG_LEVEL_CRITICAL:
            fourierSetLogLevel("ERROR");
            break;
        default:
            fourierSetLogLevel("ERROR");
    }

    // Look for the motors on the network
    Fourier::Lookup lookup(&networkIP);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lookup.setLookupFrequencyHz(0);
    group = lookup.getGroupFromFamily("Default");

    //Check presence and map
    if (group->size()>0) {
        //Define mapping between Fourier actuators group and expected robot structure and apply check on expected structure from YAML (nb joints, IPS...)
        auto entry_list = lookup.getEntryList(); //List of actuators actually connected
        cv.initialise(entry_list, expected_aios_ids); //Init mapping
    }
    else {
        spdlog::error("Cannot find any AIOS Motors on the specified network.");
    }

    for (int i = 0; i < group->size(); i++) {
        drives.push_back(new AIOSDrive());
    }

    //TODO: define limits properly and load from YAML if they exist or default otherwise (see loadParam from YAML)
    double jointMin = 0;
    double jointMax= 100;

    for (int i = 0; i < group->size(); i++){
        joints.push_back(new AIOSJoint(i, jointMin, jointMax, drives[i], "Joint Name"));
    }

    inputs.push_back(keyboard = new Keyboard());
}

AIOSRobot::~AIOSRobot() {
    spdlog::debug("Robot object deleted");
}

void AIOSRobot::fillParamVectorFromYaml(YAML::Node node, std::vector<double> &vec) {
    if(node){
        for(unsigned int i=0; i<vec.size(); i++)
            vec[i]=node[i].as<double>();
    }
}

bool AIOSRobot::loadParametersFromYAML(YAML::Node params) {
    YAML::Node params_r=params[robotName]; //Specific node corresponding to the robot

    if(params_r["networkIP"]) {
        networkIP = params_r["networkIP"].as<std::string>();
    }
    else {
        spdlog::error("YAML does not specify a network ip address for {}.", robotName);
        return false;
    }

    expected_aios_ids.clear();
    for(const auto &el: params_r["jointIDs"]) {
        expected_aios_ids.push_back(el.as<std::string>());
    }
    if(expected_aios_ids.size()==0) {
        spdlog::error("YAML does not list any joint (actuator).");
        return false;
    }

    //TODO: optional joints names??

    /*
    if(params_r["dqMax"]) {
        dqMax = fmin(fmax(0., params_r["dqMax"].as<double>()), 360.) * M_PI / 180.; //Hard constrained for safety
    }

    if(params["tauMax"]) {
        tauMax = fmin(fmax(0., params_r["tauMax"].as<double>()), 80.); //Hard constrained for safety
    }

    fillParamVectorFromYaml(params_r["iPeakDrives"], iPeakDrives);
    fillParamVectorFromYaml(params_r["motorCstt"], motorCstt);
    fillParamVectorFromYaml(params_r["linkLengths"], linkLengths);
    fillParamVectorFromYaml(params_r["massCoeff"], massCoeff);
    fillParamVectorFromYaml(params_r["qSpringK"], springK);
    fillParamVectorFromYaml(params_r["qSpringKo"], springKo);
    fillParamVectorFromYaml(params_r["frictionVis"], frictionVis);
    fillParamVectorFromYaml(params_r["frictionCoul"], frictionCoul);

    if(params_r["qLimits"]){
        for(unsigned int i=0; i<qLimits.size(); i++)
            qLimits[i]=params_r["qLimits"][i].as<double>() * M_PI / 180.;
    }

    fillParamVectorFromYaml(params_r["qSigns"], qSigns);

    if(params_r["qCalibration"]){
        for(unsigned int i=0; i<qCalibration.size(); i++)
            qCalibration[i]=params_r["qCalibration"][i].as<double>() * M_PI / 180.;
    }
    */

    spdlog::info("Using YAML parameters of {}.", robotName);
    return true;
}


bool AIOSRobot::initialise() {
    if (initialiseNetwork()) {
        if (initialiseJoints()) {
            if(initialiseInputs()) {
                return true;
            }
        }
    }
    return false;
}

bool AIOSRobot::initialiseJoints() {
    // Creates group feedback for joints
    gfeedback = std::make_shared<Fourier::GroupFeedback>((size_t) group->size());
    if(gfeedback->size()==group->size() && group->size()>0) {
        return true;
    }
    gcommand = std::make_shared<Fourier::GroupCommand>((size_t)group->size());
    return false;
}

bool AIOSRobot::initialiseInputs() {
    return true;
}

bool AIOSRobot::initialiseNetwork() {
    //First ensure that we actually have the proper list of actuators on the network
    if(cv.isInitialised()) {
        return true;
    }
    return false;
}

bool AIOSRobot::disable() {
    spdlog::info("Disabling robot...");

    // Create a Group command to disable the robot
    std::vector<float> enable_status(group->size(),
                                     std::numeric_limits<float>::quiet_NaN());
    for (int i = 0; i < group->size(); ++i) {
        enable_status[i] = 0;
    }
    gcommand->enable(enable_status);
    //group->sendCommand(std::const_pointer_cast<Fourier::GroupCommand>(group_command));  // Send the command

    for (auto p : joints) {
        p->disable(); // Reflect this in all the joints (will update the status)
    }
    return true;
}

void AIOSRobot::updateRobot() {
    spdlog::trace("AIOSRobot::updateRobot");

    //Retrieve latest values from hardware
    group->sendFeedbackRequest(FourierFeedbackAll);
    group->getNextFeedback(*gfeedback, 2);

    //Test example of assignment:
    for (int i = 0; i < group->size(); i++){
        drives[cv.aios(i)]->updateValues((*gfeedback)[i]);
    }

    for (int i = 0; i < group->size(); i++){
        drives[i]->updateValues((*gfeedback)[i]);
    }
    // Take feedback and copy into AIOSDrive objects
    for (auto joint : joints)
        joint->updateValue(); // This should take from the AIOS drives and put into Joint objects for later use
    for (auto input : inputs ){
        input->updateInput();
    }

    //Update local copies of joint values
    if((unsigned int)jointPositions_.size()!=joints.size()) {
        jointPositions_ = Eigen::VectorXd::Zero(joints.size());
    }
    if((unsigned int)jointVelocities_.size()!=joints.size()) {
        jointVelocities_ = Eigen::VectorXd::Zero(joints.size());
    }
    if((unsigned int)jointTorques_.size()!=joints.size()) {
        jointTorques_ = Eigen::VectorXd::Zero(joints.size());
    }
    unsigned int i = 0;
    for (auto joint : joints) {
        jointPositions_[i] = joint->getPosition();
        jointVelocities_[i] = joint->getVelocity();
        jointTorques_[i] = joint->getTorque();
        i++;
    }
}

// No change here with AIOS --- should just pull values from the Joint objects
Eigen::VectorXd& AIOSRobot::getPosition() {
    //Initialise vector if not already done
    return Robot::getPosition();
}

Eigen::VectorXd& AIOSRobot::getVelocity() {
    //Initialise vector if not already done

    return Robot::getVelocity();
}

Eigen::VectorXd& AIOSRobot::getTorque() {
    return Robot::getTorque();
}

void AIOSRobot::printJointStatus(int J_i) {
    joints[J_i]->printStatus();
}

bool AIOSRobot::configureMasterPDOs() {
    for (auto j : joints) {
        j->configureMasterPDOs();
    }
    for (auto i : inputs) {
        i->configureMasterPDOs();
    }
    return true;
}


bool AIOSRobot::initPositionControl() {
    // Need to send a couple of commands to the drives
    // First enable the drives (copied from Fourier's example code)
    std::vector<float> enable_status(group->size(),
                                     std::numeric_limits<float>::quiet_NaN());
    for (int i = 0; i < group->size(); ++i) {
        enable_status[i] = 1;
    }
    gcommand->enable(enable_status);

    // If the enabling is successful, update the drives values also
    if (group->sendCommand(*gcommand)) {
        for (auto j : joints) {
            j->setMode(CM_POSITION_CONTROL);
            j->enable(); // Should cascade down to the drive level
        }
    } else {
        // Include some additional error handling here
        return false;
    }
    return true;
};

setMovementReturnCode_t AIOSRobot::setPosition(std::vector<double> positions) {
    // Only add to the vector if the joints are in the right mode
    std::vector<PosPtInfo> pos_pt_infos(joints.size(), {0});
    for (int i = 0; i < joints.size(); i++) {
        if (joints[i]->setPosition(positions[i]) == SUCCESS) {
            // We can add this to the group command
            PosPtInfo info = {0};
            info.pos = ((AIOSJoint*)joints[i])->jointPositionToDriveUnit(positions[i]);  // Need a conversion which is accessible
            pos_pt_infos[cv.aios(i)] = info;
        }
    }
    gcommand->setInputPositionPt(pos_pt_infos);
    group->sendCommand(*gcommand);
    return INCORRECT_MODE;
};
