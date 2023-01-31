
#include "Orbuculum.h"

#include "OrbuculumProcessLaunch.h"

#include <iostream>
#include <thread>
#include <exception>
#include <mutex>
#include <queue>



// TODO: might have to do something like this to be more thorough: https://stackoverflow.com/questions/24012773/c-winapi-how-to-kill-child-processes-when-the-calling-parent-process-is-for
static std::vector<OrbuculumProcessManager*> globalInst;
static std::mutex m;

OrbuculumProcessManager::OrbuculumProcessManager() {
	{
		std::unique_lock<std::mutex> lock(m);

		globalInst.push_back(this);

		static bool cleanupRegistered = false;
		if (!cleanupRegistered) {
			cleanupRegistered = true;
			std::atexit([]() {
				while (!globalInst.empty()) {
					globalInst.back()->~OrbuculumProcessManager();
					globalInst.pop_back();
				}
			});
		}
	}

	try {
		pid = launchRocketLeague(hProcess);
		injectBM(hProcess);
	} catch (std::runtime_error& e) {
		std::cerr << "Orbuculum initialization error: " << e.what() << std::endl;
		OrbuculumProcessManager::~OrbuculumProcessManager();
		throw e;
	}
}

OrbuculumProcessManager::~OrbuculumProcessManager() {
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