#pragma once

#include <functional>

#include "RLBotBM.h"
#include "OrbuculumProcess.h"
#include "OrbuculumState.h"
#include "actionSequence.h"


namespace RLBotBM_NS = RLBotBM;
class Orbuculum : public OrbuculumProcess, public RLBotBM::RLBotBM {
public:
	RLBotBM_NS::GameState liveState;

	Orbuculum();

	template<long numCars, long numBalls>
	void SimulateSequence(OrbuculumState<numCars, numBalls>& startState, ActionSequence& sequence);

	struct StepResult {
		bool stop;
		RLBotBM_NS::ControllerInput input;
	};
	template<long numCars, long numBalls>
	ActionSequence createSequence(OrbuculumState<numCars, numBalls>& startState, std::function<StepResult(RLBotBM_NS::GameState&)> stepFn);
};