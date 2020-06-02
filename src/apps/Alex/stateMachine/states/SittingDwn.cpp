////////// STATE ////////////////////
//-------  Sitting Down ------------/////
////////////////////////////////////
#include "SittingDwn.h"
void SittingDwn::entry(void) {
    std::cout << "Sitting Down State Entered " << endl
              << "===================" << endl
              << " GREEN -> SIT DOWN " << endl
              << "===================" << endl;
    /*/todo */
    trajectoryGenerator->initialiseTrajectory();
    robot->startNewTraj();
}
void SittingDwn::during(void) {
    robot->moveThroughTraj();
}
void SittingDwn::exit(void) {
    std::cout
        << "Sitting Down State Exited " << endl;
}