#pragma once

#include <stdexcept>

#include "OrbuculumProcess.h"
#include "RLBotBM.h"


namespace RLBotBM_NS = RLBotBM;
class Orbuculum : public OrbuculumProcess, public RLBotBM::RLBotBM {
public:
	RLBotBM_NS::GameState liveState;

	Orbuculum();
};