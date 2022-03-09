/*Google test for trajectory generator class - change your specific type and add corner cases
* Run: g++ -lgtest trajTest.cpp TrajectoryGenerator.cpp DummyTrajectoryGenerator.cpp -o  ./test -std=c++17 && ./test
*/

#include <gtest/gtest.h>

#include "ALEXTrajectoryGenerator.h"
#include "DummyTrajectoryGenerator.h"
// test bool initialiseTrajectory(Trajectory traj, double time)
TEST(duminitTrajSuite, initialiseTrajectory) {
    // Create object
    DummyTrajectoryGenerator *tj = new DummyTrajectoryGenerator;
    // set up data for test
    double totalTime = 10;
    Trajectory traj = SIT;
    // make an assertion to test
    EXPECT_TRUE(tj->initialiseTrajectory(traj, totalTime)) << "failed to intialize trajectory variables";
}
// test std::vector<double> getSetPoint(double time)
TEST(dumgetSetPointSuite, getSetPoint) {
    // Create object
    DummyTrajectoryGenerator *tj = new DummyTrajectoryGenerator;
    // set up data for test
    double totalTime = 10;
    double time = 2;
    Trajectory traj = SIT;
    tj->initialiseTrajectory(traj, totalTime);
    std::vector<double> test = tj->getSetPoint(time);
    // make an assertion to test
    ASSERT_EQ(test.size(), 6)
        << "getSetPoint not returning correct number of points";
}
TEST(ATinitTrajSuite, initialiseTrajectory) {
    // Create object
    ALEXTrajectoryGenerator *tj = new ALEXTrajectoryGenerator;
    // set up data for initialise pose
    // Set pilot params
    PilotParameters exoParams = {
        .lowerleg_length = 0.44,
        .upperleg_length = 0.44,
        .ankle_height = 0.12,
        .foot_length = 0.30,
        .hip_width = 0.43,
        .torso_length = 0.4,
        .buttocks_height = 0.05};
    // Initial Pose -> Standing up
    jointspace_state initialPose;
    for (int i = 0; i < NUM_JOINTS; i++)
        initialPose.q[i] = 0;
    initialPose.time = 0;

    double totalTime = 10;
    Trajectory traj = SIT;
    // make an assertion to test
    EXPECT_TRUE(tj->initialiseTrajectory(traj, totalTime)) << "failed to intialize trajectory variables";
}
// test std::vector<double> getSetPoint(double time)
TEST(ATgetSetPointSuite, getSetPoint) {
    // Create object
    ALEXTrajectoryGenerator *tj = new ALEXTrajectoryGenerator;
    // set up data for test
    double totalTime = 10;
    double time = 2;
    Trajectory traj = SIT;
    tj->initialiseTrajectory(traj, totalTime);
    std::vector<double> test = tj->getSetPoint(time);
    // make an assertion to test
    ASSERT_EQ(test.size(), 6)
        << "getSetPoint not returning correct number of points";
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
