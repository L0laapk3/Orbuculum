
#include "Orbuculum.h"

#include "Launch.h"

#include <iostream>
#include <thread>
#include <cstdlib>
#include <mutex>
#include <queue>



static std::vector<OrbuculumInternal*> globalInst;
static std::mutex m;

OrbuculumInternal::OrbuculumInternal() {
	std::unique_lock<std::mutex> lock(m);

	globalInst.push_back(this);

	static bool cleanupRegistered = false;
	if (!cleanupRegistered) {
		cleanupRegistered = true;
		std::atexit([]() {
			while (!globalInst.empty()) {
				globalInst.back()->~OrbuculumInternal();
				globalInst.pop_back();
			}
		});
	}
}
OrbuculumInternal::~OrbuculumInternal() {
	bool kill;
	{
		std::unique_lock<std::mutex> lock(m);
		kill = hProcess != NULL;
		hProcess = NULL;
		globalInst.erase(std::find(globalInst.begin(), globalInst.end(), this));
	}
	if (kill) {
		TerminateProcess(hProcess, 1);
		CloseHandle(hProcess);
	}
}


Orbuculum::Orbuculum() : OrbuculumInternal(), RLBotBM(true, std::to_string(pid = launchRocketLeague(hProcess))) {
	try {
		injectBM(hProcess);

		do {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
			tryHideRocketLeagueWindow(pid);
		} while (false);
		std::cout << "connected" << std::endl;
	} catch (std::runtime_error& e) {
		std::cerr << "Orbuculum error: " << e.what() << std::endl;
		Orbuculum::~Orbuculum();
	}
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