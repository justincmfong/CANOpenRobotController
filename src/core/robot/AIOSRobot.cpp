#include "AIOSRobot.h"

AIOSRobot::AIOSRobot(std::string robot_name, std::string yaml_config_file) : Robot(robot_name, yaml_config_file) {
    spdlog::debug("Robot ({}) object created", robotName);

    //Check if YAML file exists and contain robot parameters: retrieve expected robot structure (IP, actuators IDs...)
    initialiseFromYAML(yaml_config_file);

    // Look for the motors on the network
    std::string str("192.168.86.255"); //TODO: this should come from the YAML
    Fourier::Lookup lookup(&str);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    lookup.setLookupFrequencyHz(0);
    group = lookup.getGroupFromFamily("Default");
    if (group->size()<1) {
        spdlog::error("Cannot find any AIOS Motors on the specified network.");
    }
    else
    {
        auto entry_list = lookup.getEntryList();
        for (const auto &entry : *entry_list) {
            std::cout << "SerialNb: " << entry.serial_number_ << std::endl;
        }
        //Define mapping between Fourier actuators group and expected robot structure
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

    //TODO: load YAMl including actuators ids and map[ping to joint numbers and total joint numbers

/*
    if(params_r["dqMax"]){
        dqMax = fmin(fmax(0., params_r["dqMax"].as<double>()), 360.) * M_PI / 180.; //Hard constrained for safety
    }

    if(params["tauMax"]){
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

    //Create and replace existing tool if one specified
    if(params_r["tool"]){
        if(params_r["tool"]["name"] && params_r["tool"]["length"] && params_r["tool"]["mass"]) {
            M3Tool *t = new M3Tool(params_r["tool"]["length"].as<double>(), params_r["tool"]["mass"].as<double>(), params_r["tool"]["name"].as<string>()); //Will be destroyed at end of app
            endEffTool = t;
        }
    }*/

    spdlog::info("Using YAML parameters of {}.", robotName);
    return true;
}


bool AIOSRobot::initialise() {
    if (initialiseNetwork()) {
        if (initialiseJoints()) {
            return true;
        }
    }
    return false;
}

bool AIOSRobot::initialiseJoints(){
    // Creates group feedback for joints
    feedback = std::make_shared<Fourier::GroupFeedback>((size_t) group->size());
    if(feedback->size()==group->size() && group->size()>0) {
        return true;
    }
    return false;
}

bool AIOSRobot::initialiseInputs(){
    return true;
}

bool AIOSRobot::initialiseNetwork(){
    return true;
}

bool AIOSRobot::disable() {
    spdlog::info("Disabling robot...");

    // Create a Group command to disable the robot
    std::vector<float> enable_status(group->size(),
                                     std::numeric_limits<float>::quiet_NaN());
    for (int i = 0; i < group->size(); ++i) {
        enable_status[i] = 0;
    }
    group_command->enable(enable_status);
    //group->sendCommand(std::const_pointer_cast<Fourier::GroupCommand>(group_command));  // Send the command

    for (auto p : joints) {
        p->disable(); // Reflect this in all the joints (will update the status)
    }
    return true;
}

void AIOSRobot::updateRobot() {
    spdlog::trace("AIOSRobot::updateRobot");

    //Retrieve latest values from hardware
    group->sendFeedbackRequest(FourierFeedbackCVP);
    group->getNextFeedback(*feedback, 2);

    std::cout << (*feedback)[0]->position << std::endl;

    // Take feedback and copy into AIOSDrive objects
    for (auto joint : joints)
        joint->updateValue();
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

void AIOSRobot::printStatus() {
    std::cout << "q=[ " << jointPositions_.transpose() * 180 / M_PI << " ]\t";
    //std::cout << "dq=[ " << jointVelocities_.transpose() * 180 / M_PI << " ]\t";
    //std::cout << "tau=[ " << jointTorques_.transpose() << " ]\t";
    std::cout << std::endl;
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
