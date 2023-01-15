#pragma once

#include <functional>

#include "RLBotBM.h"
#include "OrbuculumProcessManager.h"
#include "OrbuculumState.h"
#include "actionSequence.h"


namespace RLBotBM_NS = RLBotBM;
class Orbuculum : public OrbuculumProcessManager, public RLBotBM::RLBotBM {
public:
	RLBotBM_NS::GameState liveState;

	Orbuculum();

	struct StepResult {
		bool stop;
		RLBotBM_NS::ControllerInput input;
	};
	template<long numCars, long numBalls>
	ActionSequence createSequence(OrbuculumState<numCars, numBalls>& startState, std::function<StepResult(RLBotBM_NS::GameState&)> stepFn);
};