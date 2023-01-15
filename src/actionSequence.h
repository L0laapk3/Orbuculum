#pragma once

#include "RLBotBM.h"

#include <span>



struct Action {
	int duration;
	RLBotBM::ControllerInput input;
};

typedef std::span<Action> ActionSequence;

int getLength(const ActionSequence& seq);

class ActionSequenceExecutor {
	
public:
	ActionSequence::iterator currentStep;
	int actionTicks;

	void restart(ActionSequence::iterator begin);
	bool step(ActionSequence::iterator end, int ticks);
	bool finished(ActionSequence::iterator end);
	const RLBotBM::ControllerInput& getInput();
};