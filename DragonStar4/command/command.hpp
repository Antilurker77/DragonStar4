// ================================================================
//
// command.hpp
//
// Base class for making actors do an action.
//
// ================================================================

#pragma once

class Actor;

#include <memory>

class Command {
public:
	// Executes the command on the specified actor.
	virtual void Execute(Actor* actor) = 0;
protected:

};

typedef std::shared_ptr<Command> CommandPtr;
