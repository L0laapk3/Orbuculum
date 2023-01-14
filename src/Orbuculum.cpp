
#include "Orbuculum.h"

#include "Launch.h"

#include <iostream>


Orbuculum::Orbuculum() : OrbuculumProcess(), RLBotBM(true, std::to_string(pid = launchRocketLeague(hProcess))) {
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