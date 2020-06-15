#include "ExoTestState.h"

ExoTestState::ExoTestState(StateMachine *m, AlexRobot *exo, AlexTrajectoryGenerator *tg, const char *name) : State(m, name), robot(exo), trajectoryGenerator(tg){};