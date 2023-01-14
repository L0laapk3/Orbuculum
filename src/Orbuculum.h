#pragma once

#include <stdexcept>

#include "RLBotBM.h"


typedef RLBotBM::GameStateObj GameState;
typedef RLBotBM::ControllerInput ControllerInput;
typedef RLBotBM::Vec3 Vec3;
typedef RLBotBM::Quat Quat;
typedef RLBotBM::StateSetObj StateSetObj;

class Orbuculum {
	RLBotBM::IPComm ipComm;
	int lastTick = 0;
	
	HANDLE hProcess = NULL;
	DWORD pid = 0;
	
public:
	Orbuculum();
	~Orbuculum();
	
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