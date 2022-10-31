// ================================================================
//
// actor.cpp
//
// ================================================================

#include "actor.hpp"

#include "../command/waitCommand.hpp"
#include "../data/id/attribute.hpp"
#include "../data/id/statModType.hpp"
#include "../ui/messageLog.hpp"

void Actor::UpdateExtend(float secondsPerUpdate) {
	actorHUD.Update(this);
}


void Actor::DrawExtend(sf::RenderTarget& window, float timeRatio) {
	if (IsAlive() && IsOnScreen(window)) {
		actorHUD.Draw(window);
	}
}

size_t Actor::GetIndex() {
	return index;
}

void Actor::SetIndex(size_t i) {
	index = i;
}

CommandPtr Actor::CalcAI(DungeonScene* dungeonScene) {
	return std::make_shared<WaitCommand>(100);
}

bool Actor::IsAlive() {
	return currentHP > 0;
}

sf::Vector2i Actor::GetTileLocation() {
	return tileLocation;
}

int64_t Actor::GetExhaustion() {
	return exhaustion;
}

void Actor::Exhaust(int64_t amount) {
	exhaustion += amount;
}

void Actor::DecrementExhaustion() {
	if (exhaustion > 0) {
		exhaustion--;
	}
}

bool Actor::IsReadyToAct() {
	return exhaustion <= 0 && IsAlive();
}

Ability* Actor::GetAbilityByID(size_t abilityID) {
	for (size_t i = 0; i < abilities.size(); i++) {
		if (abilities[i].GetAbilityID() == abilityID) {
			return &abilities[i];
		}
	}

	return nullptr;
}

bool Actor::IsAbilityReady(size_t abilityID) {
	for (auto& ab : abilities) {
		if (ab.GetAbilityID() == abilityID) {
			return ab.IsUsable(this);
		}
	}
	return false;
}

void Actor::Walk(sf::Vector2i targetTile, int64_t moveMod) {
	int64_t cost = 100 * moveMod / 10000;
	// Diagonals cost more.
	if (targetTile.x != tileLocation.x && targetTile.y != tileLocation.y) {
		cost = cost * 14142 / 10000;
	}

	// todo: get movementspeed


	// Exhaust and move actor.
	Exhaust(cost);
	MoveToTile(targetTile, 256.f);
}

void Actor::UseAbility(size_t index, std::vector<Actor*> targets) {
	if (index < abilities.size()) {
		abilities[index].Execute(this, targets);
	}
}

void Actor::TakeDamage(int64_t amount, Attribute attribute) {
	if (amount > 0) {
		switch (attribute) {
		case Attribute::HP:
			currentHP = std::max(0ll, currentHP - amount);
			actorHUD.UpdateLifeBar(this);
			break;
		case Attribute::MP:
			currentMP = std::max(0ll, currentMP - amount);
			break;
		case Attribute::SP:
			currentSP = std::max(0ll, currentSP - amount);
			break;
		default:
			break;
		}
	}
}

int64_t Actor::GetCurrentHP() {
	return currentHP;
}

int64_t Actor::GetMaxHP() {
	int64_t result = 0;

	result = getBaseStat(StatModType::HP);

	return std::max(1ll, result);
}

int64_t Actor::GetSightRadius() {
	int64_t result = 25000;

	return result;
}

bool Actor::IsPlayer() {
	return false;
}