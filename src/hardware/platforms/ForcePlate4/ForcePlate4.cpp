#include "ForcePlate4.h"

ForcePlate4::ForcePlate4() {
    spdlog::info("New ForcePlate Robot");

    initialiseJoints();
    initialiseInputs();
}

bool ForcePlate4::initialiseInputs() {
    // Useful for testing, not really required but doesn't hurt to have it, even without a keyboard
    inputs.push_back(keyboard = new Keyboard());

    // Force Plate 1
    Eigen::Matrix<int, 4, 2> inputPins;

    inputPins(0,0) = 8; // Sensor 1
    inputPins(0,1) = 10;
    inputPins(1,0) = 8; // Sensor 2
    inputPins(1,1) = 12;
    inputPins(2, 0) = 8;  // Sensor 3
    inputPins(2, 1) = 14;
    inputPins(3, 0) = 8;  // Sensor 4
    inputPins(3, 1) = 16;
    Eigen::Vector2i clock = {8,8}; // Clock Pin


    // Force Plate 2
    Eigen::Matrix<int, 4, 2> inputPins2;
    inputPins2(0, 0) = 8;  // Sensor 1
    inputPins2(0, 1) = 9;
    inputPins2(1, 0) = 8;  // Sensor 2
    inputPins2(1, 1) = 11;
    inputPins2(2, 0) = 8;  // Sensor 3
    inputPins2(2, 1) = 15;
    inputPins2(3, 0) = 8;  // Sensor 4
    inputPins2(3, 1) = 17;

    Eigen::Vector2i clock2 = {8, 7};  // Clock Pin

    strainGauges.push_back(new HX711(inputPins, clock));
    strainGauges.push_back(new HX711(inputPins2, clock2));

    for (auto sg : strainGauges ){
        spdlog::info("Starting SGs");
        inputs.push_back(sg);
        sg->begin(128);
    }
    return true;
}

ForcePlate4::~ForcePlate4() {
    spdlog::debug("Delete ForcePlate4 object begins");

    // Delete any joints (there shouldn't be any)
    for (auto p : joints) {
        spdlog::info("Delete Joint ID: {}", p->getId());
        delete p;
    }
    joints.clear();

    // Delete the Inputs
    delete keyboard;

    for (auto sg: strainGauges){
        delete sg;
    }
    inputs.clear();

    spdlog::debug("ForcePlate4 deleted");
}


Eigen::VectorXd& ForcePlate4::getStrainReadings() {
    strainForces = Eigen::VectorXd::Zero(strainGauges.size()*4);
    int i = 0;
    for (auto sg : strainGauges) {
        strainForces.segment<4>(i * 4) = sg->getAllForces();
        Eigen::VectorXd forces = sg->getAllForces();
        //spdlog::info("{}, {}, {}, {}, {}", i, forces[0], forces[1], forces[2], forces[3]);
        i++;
    }
    updatePDOs();
    return strainForces;
}

Eigen::VectorXi ForcePlate4::getRawStrainReadings() {
    Eigen::VectorXi rawData = Eigen::VectorXi::Zero(strainGauges.size() * 4);
    int i = 0;
    for (auto sg : strainGauges) {
        rawData.segment<4>(i * 4) = sg->getAllRawData();
        i++;
    }
    return rawData;
}

void ForcePlate4::setStrainOffsets(Eigen::VectorXi offsets) {
    int j = 0;
    for (auto sg : strainGauges) {
        for (int i = 0; i < 4; i++) {
            sg->set_offset(i, offsets(j));
            j++;
        }
    }
}

bool ForcePlate4::configureMasterPDOs(){
    Robot::configureMasterPDOs();

    strainForcesTPDO = Eigen::VectorXi(4*strainGauges.size());
    UNSIGNED16 dataSize[2] = {4, 4};


    UNSIGNED16 TPDOStart = 0x3f5;

    // Create TPODs for the measurements
    for (int i = 0; i < strainGauges.size()*2; i++){
        void *dataPointer[] = {(void *)&strainForcesTPDO(2*i), (void *)&strainForcesTPDO(2*i+1)};
        tpdos.push_back(new TPDO(TPDOStart+i, 0xff, dataPointer, dataSize, 2));
    }

    UNSIGNED16 dataCmdSize[2] = {4};
    void *cmdPointer[] = {(void *)&currCommand};
    rpdoCmd = new RPDO(0x3f0, 0xff, cmdPointer, dataCmdSize, 1);

    return true;
}

void ForcePlate4::updateRobot(){
    Robot::updateRobot();

    // Uncomment the line below if you want updates on the CANbus all the time
    // getStrainReadings();
    // updatePDOs();
}

void ForcePlate4::updatePDOs() {
    // Uncomment the line below if you want updates on the CANbus all the time
    for (int i = 0; i < strainForces.size(); i++){
        strainForcesTPDO(i) = strainForces(i);
    }
}

ForcePlateCommand ForcePlate4::getCommand() {
    return currCommand;
}

void ForcePlate4::resetCommand(){
    currCommand = NONE;
}