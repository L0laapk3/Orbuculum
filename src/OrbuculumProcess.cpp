
#include "Orbuculum.h"

#include "Launch.h"

#include <iostream>
#include <thread>
#include <cstdlib>
#include <mutex>
#include <queue>



static std::vector<OrbuculumProcess*> globalInst;
static std::mutex m;

OrbuculumProcess::OrbuculumProcess() {
	{
		std::unique_lock<std::mutex> lock(m);

		globalInst.push_back(this);

		static bool cleanupRegistered = false;
		if (!cleanupRegistered) {
			cleanupRegistered = true;
			std::atexit([]() {
				while (!globalInst.empty()) {
					globalInst.back()->~OrbuculumProcess();
					globalInst.pop_back();
				}
			});
		}
	}
	
	try {
		injectBM(hProcess);

		do {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
			tryHideRocketLeagueWindow(pid);
		} while (false);
		std::cout << "connected" << std::endl;
	} catch (std::runtime_error& e) {
		std::cerr << "Orbuculum initialization error: " << e.what() << std::endl;
		OrbuculumProcess::~OrbuculumProcess();
		throw e;
	}
}

OrbuculumProcess::~OrbuculumProcess() {
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