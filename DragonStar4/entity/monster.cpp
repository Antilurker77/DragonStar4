// ================================================================
//
// monster.cpp
//
// ================================================================

#include "monster.hpp"

#include "../command/abilityCommand.hpp"
#include "../command/moveCommand.hpp"
#include "../command/waitCommand.hpp"
#include "../core/random.hpp"
#include "../core/settings.hpp"
#include "../core/tileMath.hpp"
#include "../data/gameData.hpp"
#include "../data/id/aiTarget.hpp"
#include "../data/id/statModType.hpp"
#include "../scene/dungeonScene.hpp"
#include "../ui/messageLog.hpp"

Monster::Monster(size_t monsterID, sf::Vector2i spawnTile) {
	monsterData = gameData.GetMonster(monsterID);

	aiState = AIState::Sleeping;

	abilities.push_back(Ability(1)); // All monsters have basic attack.

	if (monsterData != nullptr) {
		// Stats
		currentHP = monsterData->BaseHP;
		currentMP = monsterData->BaseMP;
		currentSP = monsterData->BaseSP;

		// Abilities
		for (auto id : monsterData->Abilities) {
			abilities.push_back(Ability(id));
		}

		// Graphics
		sprites.resize(1);
		SetTexture("monster/" + monsterData->Graphic + ".png", 0);
	}

	MoveToTile(spawnTile);
}

CommandPtr Monster::CalcAI(DungeonScene* dungeonScene) {
	CommandPtr result = std::make_shared<WaitCommand>(50);
	bool seePlayer = false;

	// Check if the AI can see the player.
	sf::Vector2i playerTile = dungeonScene->GetPlayer()->GetTileLocation();
	int64_t sightRange = GetSightRadius();
	int64_t distanceToPlayer = TileMath::Distance(GetTileLocation(), playerTile);
	if (sightRange >= distanceToPlayer) {
		if (dungeonScene->InLineOfSight(GetTileLocation(), playerTile)) {
			seePlayer = true;
		}
	}

	// If sleeping or idle, check line of sight. If player is visable, awaken.
	if (aiState != AIState::Alert) {
		if (seePlayer) {
			if (aiState == AIState::Sleeping) {
				messageLog.AddMessage("#monster " + GetName() + " #default woke up.");
			}
			//dungeonScene->AlertGroup(encounterIndex);
			aiState = AIState::Alert;
		}
	}

	if (aiState == AIState::Alert) {
		// Player escaped AI, go into chase mode.
		if (!seePlayer) {
			// todo: monster chase turns
			if (3 > 0) {
				aiState = AIState::Chasing;
				chaseTurnsRemaining = 3;
			}
			else {
				aiState = AIState::Idle;
			}
		}
		// Pick an ability to use.
		else {
			AIResult aiResult = getAIResult(dungeonScene);

			if (aiResult.first != std::numeric_limits<size_t>::max()) {
				Ability* ability = &abilities[aiResult.first];

				// Self-Target Ability
				if (aiResult.second == GetTileLocation()) {
					std::vector<sf::Vector2i> area = ability->GetAreaOfEffect(aiResult.second, aiResult.second);
					std::vector<Actor*> targets = dungeonScene->GetActorsInArea(area);
					result = std::make_shared<AbilityCommand>(aiResult.first, targets);
				}
				else {
					std::vector<sf::Vector2i> tilesToCheck = TileMath::CircleArea(aiResult.second, ability->GetMaxRange(this));

					// Check to see if the monster is already standing in range.
					if (std::find(tilesToCheck.begin(), tilesToCheck.end(), GetTileLocation()) != tilesToCheck.end()) {
						std::vector<sf::Vector2i> area = ability->GetAreaOfEffect(GetTileLocation(), aiResult.second);
						std::vector<Actor*> targets = dungeonScene->GetActorsInArea(area);
						result = std::make_shared<AbilityCommand>(aiResult.first, targets);
					}
					else {
						// Remove tiles the monster can't stand on.
						std::vector<sf::Vector2i> traversable;
						for (auto& t : tilesToCheck) {
							if (dungeonScene->IsPassible(t) && !dungeonScene->IsTileOccupiedByActor(t)) {
								traversable.push_back(t);
							}
						}

						// Find the closest tile to get in range.
						bool foundTile = false;
						sf::Vector2i goalTile;
						int64_t lowestCost = std::numeric_limits<int64_t>::max();

						for (auto& t : traversable) {
							std::vector<sf::Vector2i> path = dungeonScene->Pathfind(GetTileLocation(), t);
							if (!path.empty()) {
								int64_t pathCost = dungeonScene->GetPathCost(path);
								if (pathCost <= lowestCost) {
									lowestCost = pathCost;
									foundTile = true;
									goalTile = path[1];
								}
							}
						}

						if (foundTile) {
							result = std::make_shared<MoveCommand>(goalTile, dungeonScene->GetTileData(goalTile)->MoveMod);
						}
					}
				}
			}
		}
	}

	return result;
}

void Monster::MoveToTile(sf::Vector2i tile, float unitsPerSecond) {
	tileLocation = tile;
	sf::Vector2f goal = TileMath::TileToPixel(tile);
	Move(goal, unitsPerSecond);
}

std::string Monster::GetName() {
	std::string name = "Error";

	if (monsterData != nullptr) {
		name = monsterData->Name;
		if (!monsterData->Title.empty()) {
			name += ", " + monsterData->Title;
		}
	}

	return name;
}

int64_t Monster::GetWeaponDamage() {
	if (monsterData != nullptr) {
		return monsterData->WeaponDamage;
	}
	return 0;
}

int64_t Monster::getBaseStat(StatModType statModType) {
	int64_t result = 0;

	if (monsterData != nullptr) {
		switch (statModType) {
		case StatModType::HP:
			result = monsterData->BaseHP;
			break;
		case StatModType::MP:
			result = monsterData->BaseMP;
			break;
		case StatModType::SP:
			result = monsterData->BaseSP;
			break;
		case StatModType::AttackPower:
			result = monsterData->BaseAttackPower;
			break;
		case StatModType::SpellPower:
			result = monsterData->BaseSpellPower;
			break;
		case StatModType::HealingPower:
			result = monsterData->BaseHealingPower;
			break;
		case StatModType::Accuracy:
			result = monsterData->BaseAccuracy;
			break;
		case StatModType::Armor:
			result = monsterData->BaseArmor;
			break;
		case StatModType::SpellDefense:
			result = monsterData->BaseSpellDefense;
			break;
		case StatModType::Evasion:
			result = monsterData->BaseEvasion;
			break;
		default:
			break;
		}
	}
	return result;
}

AIResult Monster::getAIResult(DungeonScene* dungeonScene) {
	AIResult result = { std::numeric_limits<size_t>::max(), {0, 0} };
	MonsterTactic choosenTactic;

	// First, get valid tactics.
	std::vector<MonsterTactic> validTactics{};
	for (auto& t : monsterData->Tactics) {
		if (IsAbilityReady(t.AbilityID)) {
			// Check condition. TODO
			bool conditionCheck = true;
			switch (t.Condition) {
			default:
				break;
			}

			if (conditionCheck) {
				validTactics.push_back(t);
			}
		}
	}

	// Get valid tactic(s) with lowest rating.
	if (!validTactics.empty()) {
		std::vector<MonsterTactic> tacticsAtRating{};
		int lowestRating = std::numeric_limits<int>::max();
		for (auto& t : validTactics) {
			if (t.Rating < lowestRating) {
				tacticsAtRating.clear();
				lowestRating = t.Rating;
				tacticsAtRating.push_back(t);
			}
			else if (t.Rating == lowestRating) {
				tacticsAtRating.push_back(t);
			}
		}

		// If multiple tactics at this rating, select one at random.
		choosenTactic = tacticsAtRating[0];
		if (tacticsAtRating.size() > 1) {
			int64_t sum = 0;
			for (auto& t : tacticsAtRating) {
				sum += t.Weight;
			}
			int64_t roll = Random::RandomInt64(1, sum);
			for (auto& t : tacticsAtRating) {
				roll -= t.Weight;
				if (roll <= 0) {
					choosenTactic = t;
					break;
				}
			}
		}

		for (size_t i = 0; i < abilities.size(); i++) {
			if (abilities[i].GetAbilityID() == choosenTactic.AbilityID) {
				result.first = i;
				break;
			}
		}

		switch (choosenTactic.Target) {
		case AITarget::Self:
			result.second = GetTileLocation();
			break;
		case AITarget::Player:
			result.second = dungeonScene->GetPlayer()->GetTileLocation();
			break;
		case AITarget::Monster:
			// todo: more complicated logic for targeting a monster ally
			break;
		default:
			result.second = GetTileLocation();
			break;
		}
	}
	return result;
}