// ================================================================
//
// statModType.hpp
// 
// List of all stat modifiers.
//
// ================================================================

#pragma once

enum class StatModType {
	Undefined = 0,
	HP,
	HPMulti,
	HPPerLevel,
	HPRegen,
	HPLeech,
	HPOnKill,
	MP,
	MPMulti,
	MPPerLevel,
	MPRegen,
	MPLeech,
	MPOnKill,
	SP,
	SPMulti,
	SPPerLevel,
	SPRegen,
	SPLeech,
	SPOnKill,
	DamageFlat,
	DamageMulti,
	HealingFlat,
	HealingMulti,
	AttackPower,
	AttackPowerMulti,
	SpellPower,
	SpellPowerMulti,
	HealingPower,
	HealingPowerMulti,
	Accuracy,
	CritChance,
	CritPower,
	CritProtection,
	CritPowerProtection,
	DoubleStrikeChance,
	DoubleStrikePower,
	Haste,
	Armor,
	ArmorMulti,
	ArmorPen,
	SpellDefense,
	SpellDefenseMulti,
	SpellDefensePen,
	Evasion,
	BlockChance,
	BlockPower,
	DamageTakenFlat,
	DamageTakenMulti,
	Resistance,
	HealingTakenFlat,
	HealingTakenMulti,
	CooldownReduction,
	CooldownRecoveryRate,
	MPCostMulti,
	SPCostMulti,
	AuraDurationFlat,
	AuraDurationMulti,
	GoldFind,
	EXPRate
};
