#pragma once

#include <stdexcept>

#include "shared/SharedObjects.h"
#include "shared/IPComm.h"


typedef RLBotBM::Shared::GameStateObj GameState;
typedef RLBotBM::Shared::ControllerInput ControllerInput;
typedef RLBotBM::Shared::Vec3 Vec3;
typedef RLBotBM::Shared::Quat Quat;
typedef RLBotBM::Shared::StateSetObj StateSetObj;

class Orbuculum {
	RLBotBM::Shared::IPComm ipComm;
	int lastTick = 0;
	
	HANDLE hProcess = NULL;
	DWORD pid = 0;
	
public:
	Orbuculum();
	void getCurrentState(GameState& state);

	// set controls for bot at carIndex
	void setBotInput(const ControllerInput& controls, const int carIndex);

	// test if the next tick is available (state.tick) and if so, update state
	bool pollNextTick(GameState& state);
	// wait until next tick is available (state.tick) and update state, return true if the thread had to wait
	bool waitNextTick(GameState& state);

	StateSetObj& getStateSetObj();
};

class OrbuculumVersionMisMatchException : public std::runtime_error {
public:
	OrbuculumVersionMisMatchException(unsigned int currentVersion, unsigned int expectedVersion) : std::runtime_error("Orbuculum: Shared memory version mismatch. Has " + std::to_string(currentVersion) + ", but server requires " + std::to_string(expectedVersion)) {}
};