#include <gtest/gtest.h>

#include "DummyTrajectoryGenerator.h"

// test bool initialiseTrajectory(Trajectory traj, double time)
TEST(initTrajSuite, initialiseTrajectory) {
    // Create object
    DummyTrajectoryGenerator *tj = new DummyTrajectoryGenerator;
    // set up data for test
    double totalTime = 10;
    Trajectory traj = SIT;
    // make an assertion to test
    EXPECT_TRUE(tj->initialiseTrajectory(traj, totalTime)) << "failed to intialize trajectory variables";
}
// test std::vector<double> getSetPoint(double time)
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
