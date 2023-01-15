#pragma once

#include "SharedObjects.h"


typedef BallState OrbuculumBallState;

struct OrbuculumCarStateFrame : public CarState {
	ControllerInput input;
}

struct OrbuculumCarState {
	OrbuculumCarStateFrame current, last;
};

template<long numCars, long numBalls>
struct OrbuculumState {
	OrbuculumCarState cars[numCars];
	OrbuculumBallState balls[numBalls];
};