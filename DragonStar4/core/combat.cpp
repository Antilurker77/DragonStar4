// ================================================================
//
// combat.cpp
//
// ================================================================

#include "combat.hpp"

#include <sstream>
#include "../entity/actor.hpp"
#include "../ui/messageLog.hpp"

static void outputDamageMessage(Actor* user, Actor* target, CombatOptions& combatOptions, CombatResult& combatResult) {
	std::stringstream message;

	if (user->IsPlayer()) {
		message << "#player ";
	}
	else {
		message << "#monster ";
	}

	message << user->GetName() << "'s #default " << combatOptions.EventName;

	if (combatResult.DidHit) {
		message << " dealt #damage ";
		if (combatResult.DidCrit) {
			message << "\\*" << combatResult.Amount << "\\*";
		}
		else {
			message << combatResult.Amount;
		}

		message << " #default damage to ";
	}
	else {
		message << " #aaaaaa missed #default ";
	}

	if (target->IsPlayer()) {
		message << "#player ";
	}
	else {
		message << "#monster ";
	}

	message << target->GetName() << "#default .";

	if (combatResult.DidBlock) {
		message << " (Blocked)";
	}

	messageLog.AddMessage(message.str());
}

CombatResult Combat::Damage(Actor* user, Actor* target, CombatOptions& combatOptions, int64_t base, Attribute attribute) {
	CombatResult results;
	int64_t amount = base;

	results.DidHit = true;

	target->TakeDamage(amount, attribute);

	results.Amount = amount;

	outputDamageMessage(user, target, combatOptions, results);

	return results;
}