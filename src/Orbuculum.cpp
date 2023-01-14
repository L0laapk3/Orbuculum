
#include "Orbuculum.h"

#include "Launch.h"

#include <iostream>
#include <thread>


Orbuculum::Orbuculum() : ipComm(false) {
	// if (ipComm.mem->version != Shared::VERSION) {
	// 	std::cerr << "Orbuculum: Shared memory version mismatch. Has " << Shared::VERSION << ", but server requires " << ipComm.mem->gameState.version << std::endl;
	// 	throw OrbuculumVersionMisMatchException(Shared::VERSION, ipComm.mem->gameState.version);
	// }

	try {
		pid = launchRocketLeague(hProcess);
		injectDLLs(hProcess);

		do {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
			tryHideRocketLeagueWindow(pid);
		} while (false);
		std::cout << "connected" << std::endl;
	} catch (...) {
		Orbuculum::~Orbuculum();
		throw;
	}
}

Orbuculum::~Orbuculum() {
	if (hProcess != NULL) {
		TerminateProcess(hProcess, 1);
		CloseHandle(hProcess);
	}
}

void Orbuculum::setBotInput(const ControllerInput& controls, const int carIndex) {
	ipComm.mem->gameState.cars[carIndex].input = controls;
}

// GameState& Orbuculum::getGameState() {
// 	return ipComm.mem->gameState;
// }

// StateSetObj& Orbuculum::getStateSetObj() {
// 	return ipComm.mem->stateSetObj;
// }


// bool Orbuculum::simulateTicks(GameStateObj& state, int numTicks) {
// 	bool hadToWait = false;
// 	// lock the wait cv to prevent tick from being updated later
// 	ipComm.cvWaitTick.lock();	// todo: somehow all bots should be able to lock and unlock this simultaneously but not the framework
// 	if (lastTick != ipComm.mem->gameState.tick) { // next tick is already ready, immediately progress
// 		ipComm.cvWaitTick.unlock();
// 	} else {
// 		// signal the framework that we're ready with current tick
// 		ipComm.cvWaitControls.notifyOne();
		
// 		// wait for next tick
// 		ipComm.cvWaitTick.waitOne<true>();
// 		while (lastTick == ipComm.mem->gameState.tick) // eat any possible extra abandoned notifications if we're not on the next tick yet
// 			ipComm.cvWaitTick.CondVar_SingleRecipient::waitOne(0);

// 		hadToWait = true;
// 	}
	
// 	getCurrentState(state);
// 	return hadToWait;
// }