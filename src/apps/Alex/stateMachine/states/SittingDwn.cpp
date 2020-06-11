////////// STATE ////////////////////
//-------  Sitting Down ------------/////
////////////////////////////////////
#include "SittingDwn.h"
void SittingDwn::entry(void) {
    std::cout << "Sitting Down State Entered " << endl
              << "===================" << endl
              << " GREEN -> SIT DOWN " << endl
              << "===================" << endl;
    /*Better version takes in current robot pos, time and desired motion? instead of robot doing that*/
    // trajectoryGenerator->initialiseTrajectory(RobotMode::SITDWN, 2);
    trajectoryGenerator->initialiseTrajectory(Trajectory::SIT, 2);
    robot->startNewTraj();
}
void SittingDwn::during(void) {
    robot->moveThroughTraj();
}
void SittingDwn::exit(void) {
    std::cout
        << "Sitting Down State Exited " << endl;
}