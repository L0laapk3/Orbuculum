
#include "Orbuculum.h"

#include "OrbuculumProcessLaunch.h"

#include <iostream>
#include <thread>
#include <exception>
#include <mutex>
#include <queue>



// TODO: might have to do something like this to be more thourough: https://stackoverflow.com/questions/24012773/c-winapi-how-to-kill-child-processes-when-the-calling-parent-process-is-for
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
		pid = launchRocketLeague(hProcess);
		injectBM(hProcess);
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