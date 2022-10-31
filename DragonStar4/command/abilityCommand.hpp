// ================================================================
//
// abilityCommand.hpp
//
// Command class that makes an actor execute an ability.
//
// ================================================================

#pragma once

#include <vector>
#include <SFML/System.hpp>
#include "command.hpp"

class AbilityCommand : public Command {
public:
	AbilityCommand(size_t index, std::vector<Actor*> targets);
	void Execute(Actor* actor);
private:
	size_t index = 0;
	std::vector<Actor*> targets{};
};
