#include "EXO22Trajectory.h"

EXO22Trajectory::EXO22Trajectory(std::vector<ViaPoint> viaPoints, Eigen::VectorXd initPos){
    // Need to set up the trajectory here
    vp = viaPoints;
}

Eigen::VectorXd EXO22Trajectory::getPoint(double t){
    return vp[0].point;
}