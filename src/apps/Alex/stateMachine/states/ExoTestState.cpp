#include "ExoTestState.h"

ExoTestState::ExoTestState(StateMachine *m, ExoRobot *exo, AlexTrajectoryGenerator *tg, const char *name) : State(m, name), robot(exo), trajectoryGenerator(tg) { std::cout << "address of traj" << &trajectoryGenerator << std::endl; };