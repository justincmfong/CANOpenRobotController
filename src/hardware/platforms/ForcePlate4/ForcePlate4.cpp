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
    inputPins(0,0) = 2; // Sensor 1
    inputPins(0,1) = 4;
    inputPins(1,0) = 2; // Sensor 2
    inputPins(1,1) = 6;
    inputPins(2, 0) = 2;  // Sensor 3
    inputPins(2, 1) = 8;
    inputPins(3, 0) = 2;  // Sensor 4
    inputPins(3, 1) = 10;
    Eigen::Vector2i clock = {2,2}; // Clock Pin

    strainGauges.push_back(new HX711(inputPins, clock));

    // Force Plate 2
    Eigen::Matrix<int, 4, 2> inputPins2;
    inputPins2(0, 0) = 2;  // Sensor 1
    inputPins2(0, 1) = 18;
    inputPins2(1, 0) = 2;  // Sensor 2
    inputPins2(1, 1) = 20;
    inputPins2(2, 0) = 2;  // Sensor 3
    inputPins2(2, 1) = 22;
    inputPins2(3, 0) = 2;  // Sensor 4
    inputPins2(3, 1) = 24;

    Eigen::Vector2i clock2 = {2, 17};  // Clock Pin

    strainGauges.push_back(new HX711(inputPins2, clock2));

    // Force Plate 3
    Eigen::Matrix<int, 4, 2> inputPins3;
    inputPins3(0, 0) = 2;  // Sensor 1
    inputPins3(0, 1) = 28;
    inputPins3(1, 0) = 2;  // Sensor 2
    inputPins3(1, 1) = 30;
    inputPins3(2, 0) = 2;  // Sensor 3
    inputPins3(2, 1) = 32;
    inputPins3(3, 0) = 2;  // Sensor 4
    inputPins3(3, 1) = 34;

    Eigen::Vector2i clock3 = {2, 27};  // Clock Pin

    strainGauges.push_back(new HX711(inputPins3, clock3));

    // Force Plate 4
    Eigen::Matrix<int, 4, 2> inputPins4;
    inputPins4(0, 0) = 1;  // Sensor 1
    inputPins4(0, 1) = 29;
    inputPins4(1, 0) = 1;  // Sensor 2
    inputPins4(1, 1) = 31;
    inputPins4(2, 0) = 1;  // Sensor 3
    inputPins4(2, 1) = 33;
    inputPins4(3, 0) = 1;  // Sensor 4
    inputPins4(3, 1) = 35;

    Eigen::Vector2i clock4 = {1, 36};  // Clock Pin

    strainGauges.push_back(new HX711(inputPins4, clock4));

    for (auto sg : strainGauges ){
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

    for (int i = 0; i < 8; i++){
        void *dataPointer[] = {(void *)&strainForcesTPDO(2*i), (void *)&strainForcesTPDO(2*i+1)};
        tpdos.push_back(new TPDO(0x3f1+i, 0xff, dataPointer, dataSize, 2));
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