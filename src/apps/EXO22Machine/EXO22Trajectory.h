/**
 * /file EXO22Trajectory.h
 * \author Justin Fong/Vincent Crocher
 * \version 0.1
 * \date 2023-04-19
 *
 * \copyright Copyright (c) 2023
 *
 */

#ifndef EXO22TRAJ_H_DEF
#define EXO22TRAJ_H_DEF

#include <Eigen/Dense>
#include <vector>

struct ViaPoint{
    Eigen::VectorXd point; // Joint Angles
    double normTime; // Normalised Time (between 0 and 1)
};


class EXO22Trajectory{
    private:
        // Viapoint Definitions
        std::vector<ViaPoint> vp; 
         
    public:
        // Constructor
        EXO22Trajectory(){
          // This is a construct which states that nothing is initialised yet
        };
        EXO22Trajectory(std::vector<ViaPoint> viaPoints, Eigen::VectorXd initPos);

        // Returns the required point at normalised time t
        Eigen::VectorXd getPoint(double t);
    
};

#endif //EXO22TRAJ_H_DEF