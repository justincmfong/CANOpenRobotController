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
#ifndef AIOSROBOT_H_INCLUDED
#define AIOSROBOT_H_INCLUDED

#include "Robot.h"
#include "AIOSJoint.h"
#include "Keyboard.h"

#include "lookup.hpp"
#include "group.hpp"
#include "groupCommand.hpp"
#include "groupFeedback.hpp"

typedef Eigen::VectorXd VX; //!< Generic (dynamic) size version

short int sign(double val);

/**
 * @ingroup Robot
 * \brief Abstract Class representing a robot. Includes vectors of Joint and InputDevice.
 *
 */
class AIOSRobot : public Robot {
   protected:
    //Expected AIOS IDs in CORC order, loaded from YAML config
    std::vector<std::string> expected_aios_ids;

    std::vector<AIOSDrive*> drives;

    // References to Fourier motors
    std::shared_ptr<Fourier::Group> group;
    std::shared_ptr<Fourier::GroupFeedback> gfeedback;
    std::shared_ptr<Fourier::GroupCommand> gcommand;

    std::string networkIP;

    std::vector<std::string> jointNames;
    VX qCalibration;             //!< Calibration configuration: posture in which the robot is when using the calibration procedure
    std::vector<double> qSigns;

    bool calibrated;

    /** @name Utils to convert expected AIOS network (actuator) vectors order to CORC robot structure one (and vice-versa) */
    //@{
    /**

    **/
    class Convert {
        public:

            Convert() {};

            bool initialise(std::shared_ptr<Fourier::Lookup::EntryList> current_aios_order, std::vector<std::string> expected_aios_ids) {
                initialised = false;
                if(current_aios_order->size()==expected_aios_ids.size()) {
                    N = expected_aios_ids.size();
                    aios_ids.resize(N);
                    for(unsigned int i=0; i<expected_aios_ids.size(); i++) {
                        int aios_nb=0;
                        bool found=false;
                        for (const auto &entry : *current_aios_order) {
                            if(entry.serial_number_==expected_aios_ids[i]) {
                                if(push(aios_nb, i, entry.serial_number_)) {
                                    found = true;
                                    break;
                                }
                                else {
                                    return false;
                                }
                            }
                            aios_nb++;
                        }
                        if(!found) {
                            spdlog::error("AIOSRobot Convert: Expected actuator ID {} not found.", expected_aios_ids[i]);
                            return false;
                        }
                    }
                    initialised = true;
                    spdlog::info("AIOSRobot actuators\tcorc\t->\taios\t(ID)");
                    for(unsigned int i=0; i<N; i++) {
                        spdlog::info("AIOSRobot:\t\t{}\t->\t{}\t({})", i, toaios(i),  aios_ids[toaios(i)] );
                    }
                    return true;
                }
                else {
                    spdlog::error("AIOSRobot Convert: can't initialise. expected_aios_ids vector of wrong size.");
                }
                return false;
            }


            unsigned int tocorc(unsigned int n) {if(initialised) {return aios_corc[n];} else {spdlog::error("AIOSRobot Convert: Not initialised."); return 0;}} //!< AIOS index to CORC index convertion
            unsigned int toaios(unsigned int n) {if(initialised) {return corc_aios[n];}  else {spdlog::error("AIOSRobot Convert: Not initialised."); return 0;}} //!< CORC index to AIOS index convertion

            bool isInitialised() {return initialised;}

        protected:
            bool push(unsigned int aios_nb, unsigned int corc_nb, std::string aios_id) {
                if(corc_aios.count(corc_nb)==0 && aios_corc.count(aios_nb)==0 && aios_nb<N) {
                    corc_aios.insert(std::pair<unsigned int, unsigned int> (corc_nb, aios_nb));
                    aios_corc.insert(std::pair<unsigned int, unsigned int> (aios_nb, corc_nb));
                    aios_ids[aios_nb] = aios_id;
                    return true;
                }

                spdlog::error("AIOSRobot Convert: can't add new entry for AIOS ID {} ({} <-> {}). Already exists.", aios_id, aios_nb, corc_nb);
                return false;
            }

            /*TODO
            std::vector<double> to_corc(std::vector<double> &v_aios) {}
            std::vector<double> to_aios(std::vector<double> &v_corc) {}*/

        private:
            bool initialised = false;

            //Simplified bimap storing actuators order on both sides
            std::map<unsigned int, unsigned int> corc_aios;
            std::map<unsigned int, unsigned int> aios_corc;

            //Storing list of aios IDs for reference (in aios order)
            std::vector<std::string> aios_ids;

            unsigned int N=0; //!<< Nb of joints
    };

    Convert cv;

    //@}

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
     * \brief Apply current configuration as calibration configuration using qcalibration such that:
     *  q=qcalibration in current configuration.
     */
    void applyCalibration();

    bool isCalibrated() {return calibrated;}
    void decalibrate() {calibrated = false;}

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
    virtual bool initPositionControl();

    /**
    * @brief Initialises position control on this robot. Default function is to report failure
    *
    * @return true If successful
    * @return false If unsuccesful
    */
    virtual bool initVelocityControl();

    /**
    * @brief Initialises position control on this robot. Default function is to report failure
    *
    * @return true If successful
    * @return false If unsuccesful
    */
    virtual bool initTorqueControl();

    /**
    * @brief Set the target positions for each of the joints
    *
    * @param positions a vector of target positions - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    virtual setMovementReturnCode_t setPosition(Eigen::VectorXd positions);

    /**
    * @brief Set the target velocities for each of the joints
    *
    * @param positions a vector of target velocities - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    virtual setMovementReturnCode_t setVelocity(Eigen::VectorXd velocities);

    /**
    * @brief Set the target torques for each of the joints
    *
    * @param positions a vector of target torques - applicable for each of the actauted joints
    * @return MovementCode representing success or failure of the application
    */
    virtual setMovementReturnCode_t setTorque(Eigen::VectorXd torques);
    //@}

    bool getErrors();
};

#endif  //ROBOT_H
