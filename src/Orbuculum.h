#pragma once

#include <stdexcept>

#include "RLBotBM.h"


class OrbuculumInternal {
protected:
	HANDLE hProcess = NULL;
	DWORD pid = 0;
};

class Orbuculum : public OrbuculumInternal, public RLBotBM::RLBotBM {
public:
	Orbuculum();
	~Orbuculum();
};