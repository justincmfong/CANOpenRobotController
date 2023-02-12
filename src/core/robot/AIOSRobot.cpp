#include "AIOSRobot.h"

AIOSRobot::AIOSRobot(std::string robot_name, std::string yaml_config_file) : robotName(robot_name) {
    spdlog::debug("Robot ({}) object created", robotName);
}

AIOSRobot::~AIOSRobot() {
    spdlog::debug("Robot object deleted");
}

bool AIOSRobot::initialiseFromYAML(std::string yaml_config_file) {
    if(yaml_config_file.size()>0) {
        // need to use address of base directory because when run with ROS, working directory is ~/.ros
        std::string baseDirectory = XSTR(BASE_DIRECTORY);
        std::string relativeFilePath = "/config/";
        try {
            YAML::Node params = YAML::LoadFile(baseDirectory + relativeFilePath + yaml_config_file);

            if(!params[robotName]){
                spdlog::error("Parameters of {} couldn't be found in {} !", robotName, baseDirectory + relativeFilePath + yaml_config_file);
                spdlog::error("Default parameters used !");

                return false;
            }
            else {
                spdlog::info("Loading robot parameters from {}.", baseDirectory + relativeFilePath + yaml_config_file);
                //Attempt to load parameters from YAML file (delegated to each custom robot implementation)
                return loadParametersFromYAML(params);
            }

        } catch (...) {
            spdlog::error("Failed loading parameters from {}. Using default parameters instead.", baseDirectory + relativeFilePath + yaml_config_file);
            return false;
        }
    }
    else {
        spdlog::info("Using default robot parameters (no YAML file specified).");
        return false;
    }
}

bool AIOSRobot::initialise() {
    if (initialiseNetwork()) {
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
    group_command.enable(enable_status);
    group->sendCommand(group_command);  // Send the command

    for (auto p : joints) {
        p->disable(); // Reflect this in all the joints (will update the status)
    }
    return true;
}

void AIOSRobot::updateRobot() {
    //Retrieve latest values from hardware
    Fourier::GroupFeedback feedback(group->size());
    group->getNextFeedback(feedback, 2);

    // Need to create a feedback object here and update all the joints
    group->getNextFeedback(feedback, 2);

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
