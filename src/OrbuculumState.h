#pragma once

#include "SharedObjects.h"


typedef RLBotBM::BallState OrbuculumBallState;

struct OrbuculumCarStateFrame : public RLBotBM::CarState {
	RLBotBM::ControllerInput input;
};

struct OrbuculumCarState {
	OrbuculumCarStateFrame current, last;
};

template<long numCars, long numBalls>
struct OrbuculumState {
	OrbuculumCarState cars[numCars];
	OrbuculumBallState balls[numBalls];
};