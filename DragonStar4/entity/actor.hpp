// ================================================================
//
// actor.hpp
//
// Base class for the player and monster classes. Mostly handles
// stats and actions.
//
// ================================================================

#pragma once

class Command;
class DungeonScene;

enum class Attribute;
enum class StatModType;

#include <memory>
#include <string>
#include "entity.hpp"
#include "../data/ability.hpp"
#include "../ui/actorHUD.hpp"

typedef std::shared_ptr<Command> CommandPtr;

class Actor : public Entity {
public:
	void UpdateExtend(float secondsPerUpdate);
	void DrawExtend(sf::RenderTarget& window, float timeRatio);
	
	// Return the actor's index (position in vector).
	size_t GetIndex();

	// Set's the actor's index (position in vector).
	void SetIndex(size_t i);

	// Returns the command the actor's AI calculates.
	virtual CommandPtr CalcAI(DungeonScene* dungeonScene);

	// Returns true if the actor is alive (HP greater than 0).
	bool IsAlive();

	// Returns the actor's tile location.
	sf::Vector2i GetTileLocation();

	// Returns the actor's current level of exhaustion.
	int64_t GetExhaustion();

	// Adds the specified amount of exhaustion to the actor.
	void Exhaust(int64_t amount);

	// Decrements the actor's exhaustion and causes auras to tick.
	void DecrementExhaustion();

	// Returns true if the actor's exhaustion is 0 and not stunned.
	bool IsReadyToAct();

	// Returns the ability based on the ability ID.
	Ability* GetAbilityByID(size_t abilityID);

	// Returns true if the ability is ready to be used.
	bool IsAbilityReady(size_t abilityID);

	// Moves the actor to the specified tile and adds exhaustion based on movement speed.
	// Should only be called on adjacent tiles.
	void Walk(sf::Vector2i targetTile, int64_t moveMod);

	// Makes the actor use the ability on the specified targets.
	void UseAbility(size_t index, std::vector<Actor*> targets);

	// Damages the actor. Should be called after all damage calculations are complete.
	void TakeDamage(int64_t amount, Attribute attribute);

	// Returns the current HP of the actor.
	int64_t GetCurrentHP();

	// Returns the actor's max HP. It can't be less than 1.
	int64_t GetMaxHP();

	// Returns the actor's sight radius.
	int64_t GetSightRadius();

	// Moves the actor to the specified tile. Note that players are slighly offset
	// updwards by 4 pixels.
	virtual void MoveToTile(sf::Vector2i tile, float unitsPerSecond = 0.f) = 0;

	// Returns true if the actor is the player.
	virtual bool IsPlayer();

	// Returns the actor's name and title.
	virtual std::string GetName() = 0;

	// Returns the weapon damage of the actor.
	virtual int64_t GetWeaponDamage() = 0;

protected:
	// Returns the base stat for the designated stat type. Pulls from race
	// for players, monster data for monsters.
	virtual int64_t getBaseStat(StatModType statModType) = 0;

	// ================================
	size_t index = 0;
	
	sf::Vector2i tileLocation = { 0, 0 };

	int64_t exhaustion = 0;

	int64_t currentHP = 0;
	int64_t currentMP = 0;
	int64_t currentSP = 0;

	std::vector<Ability> abilities{};

	ActorHUD actorHUD;
};

typedef std::shared_ptr<Actor> ActorPtr;