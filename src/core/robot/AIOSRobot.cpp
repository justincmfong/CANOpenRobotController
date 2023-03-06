#include "AIOSRobot.h"

AIOSRobot::AIOSRobot(std::string robot_name, std::string yaml_config_file) : robotName(robot_name) {
    spdlog::debug("Robot ({}) object created", robotName);

    // Look for the motors on the network
    std::string str("192.168.86.255");
    Fourier::Lookup lookup(&str);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    lookup.setLookupFrequencyHz(0);
    group = lookup.getGroupFromFamily("Default");
    if (!group) {
        std::cout << "No group found!" << std::endl;
        spdlog::error("Cannot find any AIOS Motors on the specified network.");
    }
    inputs.push_back(keyboard = new Keyboard());
}

AIOSRobot::~AIOSRobot() {
    spdlog::debug("Robot object deleted");
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
    // Creates some joints
    // In Robot this is a pure virtual function
    // Normally populates the joints vector... might not need to do this now
    feedback = std::make_shared<Fourier::GroupFeedback>((size_t) group->size());
    return true;
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
