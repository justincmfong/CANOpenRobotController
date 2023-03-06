/**
 * \file Robot.h
 * \author Justin Fong
 * \brief Extension of the Robot Class to include methods to deal with AIOS Actuators
 *
 * @version 0.1
 * @date 2023/2/12
 *
 * @copyright Copyright (c) 2023
 *
 */
/**
 *  @defgroup Robot Robot Module
 *  A group of abstract classes, acting as the software representation of a robot.
 */
#ifndef AIOSROBOT_H_INCLUDED
#define AIOSROBOT_H_INCLUDED
#include <vector>
#define EIGEN_RUNTIME_NO_MALLOC //! Flag preventing Eigen to do dynaic allocation (can be bad in RT). See https://github.com/stulp/tutorials/blob/master/test.md for details.
#include <Eigen/Dense>
// yaml-parser
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "Keyboard.h"

// These are used to access the MACRO: BASE_DIRECTORY
#define XSTR(x) STR(x)
#define STR(x) #x

#include "InputDevice.h"
#include "Joint.h"
#include "AIOSDrive.h"
#include "Robot.h"


#include "lookup.hpp"
#include "group.hpp"
#include "groupCommand.hpp"
#include "groupFeedback.hpp"

short int sign(double val);

/**
 * @ingroup Robot
 * \brief Abstract Class representing a robot. Includes vectors of Joint and InputDevice.
 *
 */
class AIOSRobot : public Robot {
   protected:
    /**
    * \brief Vector of pointers to Abstract <class>Joint<class> Objects, number and type must be specified by
    * Software design in <class>Robot<class> Implementation.
    * Note: Use pointers to the joint objects here, so that the derived objects are not cast to Joint, truncating
    * any of their explicit implementations.
    *
    */
    std::string robotName;

    std::vector<Joint *> joints;
    std::vector<InputDevice *> inputs;

    Eigen::VectorXd jointPositions_;
    Eigen::VectorXd jointVelocities_;
    Eigen::VectorXd jointTorques_;

    // References to Fourier motors
    std::shared_ptr<Fourier::Group> group;
    std::shared_ptr<Fourier::GroupFeedback> feedback; // Fourier::GroupFeedback((size_t) 0);  // Not sure if need this here or to instantiate in each method
    std::shared_ptr<Fourier::GroupCommand> group_command; // = Fourier::GroupCommand((size_t) 0);  // Not sure if need this here or to instantiate in each method


   public:
    Keyboard* keyboard;

    /** @name Constructors and Destructors */
    //@{
    /**
     * \brief Default Robot constructor.
     * \param robot_name a name of the robot. If a yaml_config_file is also provided, the name will be used to seek parameters in this file (and so should match robot name in the YAML file).
     * \param yaml_config_file the name of a valide YAML file describing kinematic and dynamic parameters of the M3. If absent or incomplete default parameters are used instead.
     */
    AIOSRobot(std::string robot_name = "", std::string yaml_config_file = "");
    virtual ~AIOSRobot();
    //@}

    /** @name Initialisation Methods */
    //@{
    /**
     * \brief Initialize memory for the designed <code>Robot</code> classes specific
     * <code>Joint</code> objects + sensors (if available) using the pure virtual initialiseJoints()
     * implemented by the robot designer. Based on the given Joints, initNetwork() will configure
     * these joints for CAN PDO messaging and Load the specififed Controller, by default set to Positio.
     *
     * \return true if successful
     * \return false if unsuccessful
     */
    bool initialise();

   protected:

    /**
    * \brief Utility method filling vec with the values loaded from the YAML node. Expect same vector lengths.
    *
    */
    void fillParamVectorFromYaml(YAML::Node node, std::vector<double> &vec);

    /**
    * \brief Load parameters from YAML file if valid one specified in constructor.
    * \param params a valid YAML robot parameters node loaded by initialiseFromYAML() method.
    * \return true
    */
    virtual bool loadParametersFromYAML(YAML::Node params);

   public:

    /**
     * \brief Stop the robot: disable all actuated joints.
     *
     * \return true if successful
     * \return false if unsuccessful
     */
    virtual bool disable();

    /**
     * \brief Function used to set up the Master Object Dictionary to respond to any PDOs expected from any device. Is called before
     * the initialisation of the state machine.
     *
     */
    virtual bool configureMasterPDOs();
    /**
     * \brief Pure Virtual function, implemeted by robot designer with specified number of each concrete joint classes
     * for the robot hardware desired.
     *
     */
    virtual bool initialiseJoints();
    /**
     * \brief Pure Virtual function, implemeted by robot designer with specified number of each concrete input classes
     * for the robot hardware desired.
     *
     * \return true if successful
     * \return flase if unsuccesful
     *
     */
    virtual bool initialiseInputs();
    /**
     * \brief For each <class>Joint</class> in the robots joints Vector.
     * Individually set up the underlying CANopen PDO messaging to and from
     * the hardware attached.
     *
     * \return true if successful
     * \return false if unsuccessful
     */
    virtual bool initialiseNetwork();
    //@}

    /** @name Core Update and State Methods */
    //@{
    /**
    * \brief Update all of this <code>Robot<code> software joint positions
    * from object dictionary entries.
    *
    */
    virtual void updateRobot();

    /**
    * \brief Get the latest joints position
    *
    * \return Eigen::VectorXd a reference to the vector of actual joint positions
    */
    Eigen::VectorXd& getPosition();

    /**
    * \brief Get the latest joints velocity
    *
    * \return Eigen::VectorXd a reference to the vector of actual joint positions
    */
    Eigen::VectorXd& getVelocity();

    /**
    * \brief Get the latest joints torque
    *
    * \return Eigen::VectorXd a reference to the vector of actual joint positions
    */
    Eigen::VectorXd& getTorque();

    /**
    * \brief print out status of robot and all of its joints
    *
    */
    void printStatus();
    /**
    * \brief print out status of <code>Joint<code> J_i
    *
    */
    void printJointStatus(int J_i);
    //@}


    /** @name Core Control Methods */
    //@{
    /**
    * @brief Initialises position control on this robot. Default function is to report failure
    *
    * @return true If successful
    * @return false If unsuccesful
    */
    virtual bool initPositionControl() { return false; };

    /**
    * @brief Initialises position control on this robot. Default function is to report failure
    *
    * @return true If successful
    * @return false If unsuccesful
    */
    virtual bool initVelocityControl() { return false; };

    /**
    * @brief Initialises position control on this robot. Default function is to report failure
    *
    * @return true If successful
    * @return false If unsuccesful
    */
    virtual bool initTorqueControl() {
        std::cout << " initTorqueControl";

        return false; };

    /**
    * @brief Set the target positions for each of the joints
    *
    * @param positions a vector of target positions - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    virtual setMovementReturnCode_t setPosition(std::vector<double> positions) { return INCORRECT_MODE; };

    /**
    * @brief Set the target velocities for each of the joints
    *
    * @param positions a vector of target velocities - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    virtual setMovementReturnCode_t setVelocity(std::vector<double> velocities) { return INCORRECT_MODE; };

    /**
    * @brief Set the target torques for each of the joints
    *
    * @param positions a vector of target torques - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    virtual setMovementReturnCode_t setTorque(std::vector<double> torques) { return INCORRECT_MODE; };
    //@}
};

#endif  //ROBOT_H
