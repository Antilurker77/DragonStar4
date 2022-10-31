// ================================================================
//
// abilityCommand.cpp
//
// ================================================================

#include "abilityCommand.hpp"

#include "../entity/actor.hpp"

AbilityCommand::AbilityCommand(size_t index, std::vector<Actor*> targets) {
	this->index = index;
	this->targets = targets;
}

void AbilityCommand::Execute(Actor* actor) {
	actor->UseAbility(index, targets);
}