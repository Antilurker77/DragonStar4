// ================================================================
//
// waitCommand.cpp
//
// ================================================================

#include "waitCommand.hpp"

#include "../entity/actor.hpp"

WaitCommand::WaitCommand(int64_t time) {
	timeToWait = time;
}

void WaitCommand::Execute(Actor* actor) {
	actor->Exhaust(timeToWait);
}