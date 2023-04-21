#include "AIOSRobot.h"

AIOSRobot::AIOSRobot(std::string robot_name, std::string yaml_config_file) : Robot(robot_name, yaml_config_file) {
    spdlog::debug("Robot ({}) object created", robotName);

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

    //Check if YAML file exists and contain robot parameters: retrieve expected robot structure (IP, nb of joints, actuators IDs...)
    if (initialiseFromYAML(yaml_config_file)) {

        // Look for the motors on the network
        Fourier::Lookup lookup(&networkIP);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        lookup.setLookupFrequencyHz(0);
        group = lookup.getGroupFromFamily("Default");

        //Check presence and map
        if (group->size()>0) {
            //Define mapping between Fourier actuators group and expected robot structure and apply check on expected structure from YAML (nb joints, IPS...)
            auto entry_list = lookup.getEntryList(); //List of actuators actually connected
            //Init mapping
            if(!cv.initialise(entry_list, expected_aios_ids))
            {
                spdlog::error("Can't initialise. AIOS actuators actually present:");
                for (const auto &entry : *entry_list) {
                    spdlog::error(" \t -found {}", entry.serial_number_);
                }
            }
        }
        else {
            spdlog::error("Cannot find any AIOS Motors on the specified network.");
        }

        for (int i = 0; i < group->size(); i++) {
            drives.push_back(new AIOSDrive());
        }

        for (int i = 0; i < group->size(); i++){
            joints.push_back(new AIOSJoint(i, qLimits[2*i], qLimits[2*i+1], qSigns[i], -dqMax, dqMax, -tauMax, tauMax, drives[i], jointNames[i]));
        }

        inputs.push_back(keyboard = new Keyboard());
    }
}

AIOSRobot::~AIOSRobot() {
    spdlog::debug("AIOSRobot object deleted");
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

    int nb_joints=-1;
    if(params_r["nbJoints"]) {
        nb_joints = params_r["nbJoints"].as<int>();

    }
    if(nb_joints<0 || nb_joints>99) {
        spdlog::error("YAML incorrect nb of joints (nbJoints: {}) for {}.", nb_joints, robotName);
        return false;
    }

    //Required actual AIOS actuators IDs expected
    expected_aios_ids.clear();
    for(const auto &el: params_r["jointIDs"]) {
        expected_aios_ids.push_back(el.as<std::string>());
    }
    if(expected_aios_ids.size()!=nb_joints) {
        spdlog::error("YAML does not list proper number of joints (actuators) IDs for {}.", robotName);
        return false;
    }

    //Optional joint names
    if((params_r["jointNames"])){
        for(const auto &el: params_r["jointNames"]) {
            jointNames.push_back(el.as<std::string>());
        }
        if(jointNames.size()!=nb_joints) {
            spdlog::error("YAML does not list proper number of joint names for {}.", robotName);
            return false;
        }
    }
    else {
        for(unsigned int i=0; i<nb_joints; i++) {
            jointNames.push_back("Joint name");
        }
    }

    qSigns.resize(nb_joints);
    fill(qSigns.begin(), qSigns.end(), 1);
    if(params_r["qSigns"]){
        if(qSigns.size() == params_r["qSigns"].size()) {
            fillParamVectorFromYaml(params_r["qSigns"], qSigns);
        }
        else {
            spdlog::error("YAML does not list proper number of joint signs for {}.", robotName);
            return false;
        }
    }

    qCalibration = VX::Zero(nb_joints);
    if(params_r["qCalibration"]){
        if(qCalibration.size() == params_r["qCalibration"].size()) {
            for(unsigned int i=0; i<qCalibration.size(); i++)
                qCalibration[i]=params_r["qCalibration"][i].as<double>() * M_PI / 180.;
        }
        else {
            spdlog::error("YAML does not list proper number of calibration values for {}.", robotName);
            return false;
        }

    }

    qLimits.resize(2*nb_joints);
    fill(qLimits.begin(), qLimits.end(), 0);
    if(params_r["qLimits"]){
        if(qLimits.size() == params_r["qLimits"].size()) {
            for(unsigned int i=0; i<qLimits.size(); i++)
                qLimits[i]=params_r["qLimits"][i].as<double>() * M_PI / 180.;
        }
        else {
            spdlog::error("YAML does not list proper number of joint limits for {}.", robotName);
            return false;
        }
    }

    if(params_r["dqMax"]) {
        dqMax = fmin(fmax(0., params_r["dqMax"].as<double>()), 360.) * M_PI / 180.; //Hard constrained for safety
    }

    if(params_r["tauMax"]) {
        tauMax = fmin(fmax(0., params_r["tauMax"].as<double>()), 10.); //Hard constrained for safety
    }

    /*fillParamVectorFromYaml(params_r["iPeakDrives"], iPeakDrives);
    fillParamVectorFromYaml(params_r["motorCstt"], motorCstt);
    fillParamVectorFromYaml(params_r["linkLengths"], linkLengths);
    fillParamVectorFromYaml(params_r["massCoeff"], massCoeff);
    fillParamVectorFromYaml(params_r["qSpringK"], springK);
    fillParamVectorFromYaml(params_r["qSpringKo"], springKo);
    fillParamVectorFromYaml(params_r["frictionVis"], frictionVis);
    fillParamVectorFromYaml(params_r["frictionCoul"], frictionCoul);*/

    spdlog::info("Using YAML parameters of {}.", robotName);
    if(SPDLOG_ACTIVE_LEVEL<=SPDLOG_LEVEL_DEBUG)
    {
        std::cout << "Loaded robot configuration:\n";
        std::cout << "\tIP: " << networkIP << "\n";
        std::cout << "\tMax joints torque: " <<  tauMax << "\n";
        std::cout << "\tMax joints speed: " << dqMax*180./M_PI << " deg./s\n";
        std::cout << "\tJoints: ";
        for(unsigned int i=0; i< nb_joints; i++)
            std::cout << "\t" << jointNames[i];
        std::cout << "\n";
        std::cout << "\tIDs: \t";
        for(unsigned int i=0; i< nb_joints; i++)
            std::cout << "\t" << expected_aios_ids[i];
        std::cout << "\n";
        std::cout << "\tSigns: \t";
        for(unsigned int i=0; i< nb_joints; i++)
            std::cout << "\t" << qSigns[i];
        std::cout << "\n";
        std::cout << "\tStops: \t";
        for(unsigned int i=0; i< nb_joints; i++)
            std::cout << "\t" << qLimits[2*i]*180./M_PI << "/"<< qLimits[2*i+1]*180./M_PI;
        std::cout << "\n";
        std::cout << "\tCalibration:";
        for(unsigned int i=0; i< nb_joints; i++)
            std::cout << "\t" << qCalibration[i]*180./M_PI;
        std::cout << "\n";
    }
    return true;
}


bool AIOSRobot::initialise() {
    spdlog::info("Start Robot Initialise");
    if (initialiseNetwork()) {
        spdlog::info("Network Done");
        if (initialiseJoints()) {
            spdlog::info("Joints Done");
            if(initialiseInputs()) {
                spdlog::info("Inputs Done");
                return true;
            }
        }
    }
    return false;
}

bool AIOSRobot::initialiseJoints() {
    spdlog::debug("initialiseJoints");
    // Creates group feedback for joints
    gfeedback = std::make_shared<Fourier::GroupFeedback>((size_t) group->size());
    gcommand = std::make_shared<Fourier::GroupCommand>((size_t)group->size());

    if(gfeedback->size()==group->size() && group->size()>0 && gcommand->size() == group->size()) {
        return true;
    }
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

void AIOSRobot::applyCalibration() {
    spdlog::debug("AIOSRobot::ApplyCalib: {} {} => {}", getPosition()[0]*180/M_PI, qCalibration[0]*180/M_PI);
    for (unsigned int i = 0; i < joints.size(); i++) {
        ((AIOSJoint *)joints[i])->setPositionOffset(qCalibration[i]);
    }
    calibrated = true;
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
    if (group->sendCommand(*gcommand)) {
        for (auto j : joints) {
            j->disable();
        }
    } else {
        // Include some additional error handling here
        return false;
    }
    return true;
}

void AIOSRobot::updateRobot() {
    spdlog::trace("AIOSRobot::updateRobot");

    //Retrieve latest values from hardware
    group->sendFeedbackRequest(FourierFeedbackAll);
    group->getNextFeedback(*gfeedback, 2);

    //apply these values to virtual 'drives'
    for (int i = 0; i < group->size(); i++) {
        drives[cv.tocorc(i)]->updateValues((*gfeedback)[i]);
    }

    //Then equivalent process to generic robot
    Robot::updateRobot();

    if (safetyCheck() != SUCCESS) {
        disable();
    }

    /*
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
    }*/
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


setMovementReturnCode_t AIOSRobot::safetyCheck() {
    //Joint max speed and stops
    for (unsigned int i = 0; i < group->size(); i++) {
        if (((AIOSJoint *)joints[i])->safetyCheck() != SUCCESS) {
            spdlog::error("AIOS Robot: Joint {} safety triggered!", i);
            return OUTSIDE_LIMITS;
        }
    }
    return SUCCESS;
}

bool AIOSRobot::initPositionControl() {
    // Need to send a couple of commands to the drives
    // First enable the drives (copied from Fourier's example code)
    spdlog::trace("AIOSRobot::initPositionControl");
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
        spdlog::error("AIOSRobot::initPositionControl: failed init command.");
        return false;
    }
    return true;
};

bool AIOSRobot::initVelocityControl() {
    // Need to send a couple of commands to the drives
    // First enable the drives (copied from Fourier's example code)
    spdlog::trace("AIOSRobot::initVelocityControl");
    std::vector<float> enable_status(group->size(),
                                     std::numeric_limits<float>::quiet_NaN());
    for (int i = 0; i < group->size(); ++i) {
        enable_status[i] = 1;
    }
    gcommand->enable(enable_status);
    // If the enabling is successful, update the drives values also
    if (group->sendCommand(*gcommand)) {
        for (auto j : joints) {
            j->setMode(CM_VELOCITY_CONTROL);
            j->enable();  // Should cascade down to the drive level
        }
    } else {
        // Include some additional error handling here
        spdlog::error("AIOSRobot::initVelocityControl: failed init command.");
        return false;
    }
    return true;
};

bool AIOSRobot::initTorqueControl() {
    // Need to send a couple of commands to the drives
    // First enable the drives (copied from Fourier's example code)
    spdlog::trace("AIOSRobot::initTorqueControl");
    std::vector<float> enable_status(group->size(),
                                     std::numeric_limits<float>::quiet_NaN());
    for (int i = 0; i < group->size(); ++i) {
        enable_status[i] = 1;
    }
    gcommand->enable(enable_status);
    // If the enabling is successful, update the drives values also
    if (group->sendCommand(*gcommand)) {
        for (auto j : joints) {
            j->setMode(CM_TORQUE_CONTROL);
            j->enable();  // Should cascade down to the drive level
        }
    } else {
        // Include some additional error handling here
        spdlog::error("AIOSRobot::initTorqueControl: failed init command.");
        return false;
    }
    return true;
};

setMovementReturnCode_t AIOSRobot::setPosition(Eigen::VectorXd positions) {
    // Only add to the vector if the joints are in the right mode
    std::vector<PosPtInfo> pos_pt_infos(joints.size(), {0});
    if (!calibrated) {
        spdlog::debug("AIOSRObot: not calibrated, can't apply position control.");
        return NOT_CALIBRATED;
    }
    else {
        for (int i = 0; i < joints.size(); i++) {
            if (joints[i]->setPosition(positions[i]) == SUCCESS) {
                // We can add this to the group command
                PosPtInfo info = {0};
                info.pos = ((AIOSJoint*)joints[i])->jointPositionToDriveUnitDoubleWithOffset(positions[i]);  // Need a conversion which is accessible
                pos_pt_infos[cv.toaios(i)] = info;
            }
        }
        gcommand->setInputPositionPt(pos_pt_infos);
        if (group->sendCommand(*gcommand)) {
            return SUCCESS;
        }
    }
    return UNKNOWN_ERROR;
};

setMovementReturnCode_t AIOSRobot::setVelocity(Eigen::VectorXd velocities) {
    // Only add to the vector if the joints are in the right mode
    std::vector<float> driveVels(joints.size(), 0);
    for (int i = 0; i < joints.size(); i++) {
        if (joints[i]->setVelocity(velocities[i]) == SUCCESS) {
            // We can add this to the group command
            driveVels[cv.toaios(i)] = ((AIOSJoint*)joints[i])->jointVelocityToDriveUnitDouble(velocities[i]);
        }
    }
    gcommand->setInputVelocityPt(driveVels);
    if (group->sendCommand(*gcommand)) {
        return SUCCESS;
    }
    return UNKNOWN_ERROR;
};

setMovementReturnCode_t AIOSRobot::setTorque(Eigen::VectorXd torques) {
    // Only add to the vector if the joints are in the right mode
    std::vector<float> driveTorques(joints.size(), 0);
    for (int i = 0; i < joints.size(); i++) {
        if (joints[i]->setTorque(torques[i]) == SUCCESS) {
            // We can add this to the group command
            driveTorques[cv.toaios(i)] = ((AIOSJoint*)joints[i])->jointTorqueToDriveUnitDouble(torques[i]);
        }
    }
    gcommand->setInputTorquePt(driveTorques);
    if (group->sendCommand(*gcommand)){
        return SUCCESS;
    }
    return UNKNOWN_ERROR;
};

bool AIOSRobot::getErrors(){
    std::vector<bool> errors(joints.size(), true);
    gcommand->getError(errors);
    group->sendCommand(*gcommand);

    for (int i = 0; i < joints.size(); i++){
        drives[i]->setErrorPointer(group->getError(cv.tocorc(i)));
    }
    return true;
}

