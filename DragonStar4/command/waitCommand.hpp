// ================================================================
//
// waitCommand.hpp
//
// Command class that makes an actor move wait for a certain time.
//
// ================================================================

#pragma once

#include <SFML/System.hpp>
#include "command.hpp"

class WaitCommand : public Command {
public:
	WaitCommand(int64_t time);
	void Execute(Actor* actor);
private:
	int64_t timeToWait = 0;
};