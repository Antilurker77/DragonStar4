// ================================================================
//
// ability.cpp
//
// ================================================================

#include "ability.hpp"

#include "gameData.hpp"
#include "../core/tileMath.hpp"
#include "../entity/actor.hpp"

Ability::Ability() {

}

Ability::Ability(size_t id) {
	abilityData = gameData.GetAbility(id);

	if (abilityData != nullptr) {
		currentCharges = abilityData->MaxCharges;
	}
}

void Ability::Execute(Actor* user, std::vector<Actor*> targets) {
	CombatResult result;
	CombatOptions combatOptions = GetCombatOptions();

	if (abilityData != nullptr) {
		for (size_t i = 0; i < abilityData->Effects.size(); i++) {
			AbilityEffect effect = abilityData->Effects[i];
			int64_t base = 0;
			switch (effect.Effect) {
			case Effect::WeaponDamage:
				// todo: weapon damage
				base = user->GetWeaponDamage();
				for (size_t j = 0; j < targets.size(); j++) {
					result = Combat::Damage(user, targets[j], combatOptions, base, effect.Attribute);
				}
				break;
			default:
				break;
			}
		}

		// todo: resources, action cost
		user->Exhaust(200);
	}
}

size_t Ability::GetAbilityID() {
	if (abilityData != nullptr) {
		return abilityData->ID;
	}
	return 0;
}

int64_t Ability::GetMaxRange(Actor* user) {
	// todo: get bonus range stat from player
	if (abilityData != nullptr) {
		switch (abilityData->AreaType) {
		case AreaType::Self:
			return 0;
		case AreaType::Circle:
		{
			int64_t range = 0;
			int64_t area = abilityData->AreaArguments[1];
			// Weapon Attack
			if (abilityData->AreaArguments[0] < 0) {
				if (user != nullptr) {
					// todo: get weapon range
					range = 100;
				}
			}
			return range + area;
		}
		case AreaType::Line:
			return abilityData->AreaArguments[0];
		default:
			return 0;
		}
	}

	return 0;
}

std::vector<sf::Vector2i> Ability::GetAreaOfEffect(sf::Vector2i userTile, sf::Vector2i targetTile) {
	std::vector<sf::Vector2i> result{};

	if (abilityData != nullptr) {
		switch (abilityData->AreaType) {
		case AreaType::Circle:
			result = TileMath::CircleArea(targetTile, abilityData->AreaArguments[1]);
			break;
		case AreaType::Self:
			result.push_back(userTile);
			break;
		default:
			break;
		}
	}

	return result;
}

bool Ability::IsUsable(Actor* user) {
	if (abilityData == nullptr) {
		return false;
	}

	if (currentCharges <= 0) {
		return false;
	}

	return true;
}

CombatOptions Ability::GetCombatOptions() {
	CombatOptions co;

	if (abilityData != nullptr) {
		co.EventName = abilityData->Name;

		co.AbilityID = abilityData->ID;

		co.Categories = abilityData->Categories;
		co.Elements = abilityData->Elements;

		co.BaseHitChance = abilityData->HitChance;

		co.CanDodge = abilityData->CanDodge;
		co.CanBlock = abilityData->CanBlock;
		co.CanCrit = abilityData->CanCrit;
		co.CanDoubleStrike = abilityData->CanDoubleStrike;
	}

	return co;
}