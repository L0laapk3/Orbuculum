#pragma once

#include <stdexcept>

#include "RLBotBM.h"


class OrbuculumInternal {
protected:
	HANDLE hProcess = NULL;
	DWORD pid = 0;
public:
	OrbuculumInternal();
	~OrbuculumInternal();
};

class Orbuculum : public OrbuculumInternal, public RLBotBM::RLBotBM {
public:
	Orbuculum();
};