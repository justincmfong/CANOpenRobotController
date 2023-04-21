#include "EXO22Trajectory.h"

EXO22Trajectory::EXO22Trajectory(std::vector<ViaPoint> viaPoints, Eigen::VectorXd initPos){
    // Need to set up the trajectory here
    vp.push_back({initPos, 0});
    
    for (auto via : viaPoints) {
        vp.push_back(via);
    }
}

Eigen::VectorXd EXO22Trajectory::getPoint(double t){
    int i = 0;

    if(t >= 1){
        return vp[vp.size()-1].point;
    }else {
        while(t < vp[i].normTime){
            i++;
        }
        double s = (t-vp[i-1].normTime)/(vp[i].normTime - vp[i-1].normTime);
        return vp[i-1].point + s*(vp[i].point  - vp[i-1].point );
    }
}