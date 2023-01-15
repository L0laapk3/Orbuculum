
#include "Orbuculum.h"
#include "OrbuculumProcessLaunch.h"

#include <iostream>
#include <thread>


Orbuculum::Orbuculum() : OrbuculumProcessManager(), RLBotBM(true, std::to_string(pid)) {
	int ms = 0;
	do {
		if (ms > 30000)
			throw std::runtime_error("Orbuculum: Rocket League window not found");
		ms += 10;
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(10ms);
		tryHideRocketLeagueWindow(pid);
	} while (!pollNextTick(liveState));
	
	// set car 0 to be controlled by
	ipComm.mem->gameState.cars[0].RLBotBMControlled = true;
}




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