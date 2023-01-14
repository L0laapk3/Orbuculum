#pragma once

#include <stdexcept>

#include "OrbuculumProcess.h"
#include "RLBotBM.h"



class Orbuculum : public OrbuculumProcess, public RLBotBM::RLBotBM {
public:
	Orbuculum();
};