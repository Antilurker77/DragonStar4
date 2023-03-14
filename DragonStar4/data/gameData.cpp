// ================================================================
//
// gameData.cpp
//
// ================================================================

#include "gameData.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "sqlite3.h"
#include "abilityEffect.hpp"
#include "id/aiCondition.hpp"
#include "id/aiTarget.hpp"
#include "id/attribute.hpp"
#include "id/category.hpp"
#include "id/element.hpp"
#include "id/equipType.hpp"
#include "id/itemRarity.hpp"
#include "id/itemType.hpp"
#include "../core/random.hpp"

GameData gameData;

std::vector<AbilityData> GameData::abilities{};
std::vector<ItemData> GameData::items{};
std::vector<MonsterData> GameData::monsters{};
std::vector<RaceData> GameData::races{};
std::vector<RoomData> GameData::rooms{};
std::vector<TileData> GameData::tiles{};

static std::vector<std::string> splitString(const char* s, char splitter) {
	std::string str;
	std::string temp;
	std::stringstream ss;
	std::vector<std::string> strv{};

	// Remove spaces.
	ss.str(s);
	while (std::getline(ss, temp, ' ')) {
		str += temp;
	}

	// Split the string.
	ss.clear();
	ss.str(str);
	while (std::getline(ss, temp, splitter)) {
		strv.push_back(temp);
	}

	return strv;
}

static AICondition stringToAICondition(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, AICondition> conditionMap = {
		{"none", AICondition::None}
	};

	auto result = conditionMap.find(s);

	if (result != conditionMap.end()) {
		return result->second;
	}

	std::cout << "AICondition Error: Could not parse " << s << ".\n";
	return AICondition::Undefined;
}

static AITarget stringToAITarget(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, AITarget> targetMap = {
		{"self", AITarget::Self},
		{"player", AITarget::Player},
		{"monster", AITarget::Monster}
	};

	auto result = targetMap.find(s);

	if (result != targetMap.end()) {
		return result->second;
	}

	std::cout << "AITarget Error: Could not parse " << s << ".\n";
	return AITarget::Undefined;
}

static AreaType stringToAreaType(const char* c) {
	std::string s(c);
	
	static const std::unordered_map<std::string, AreaType> areaTypeMap = {
		{"self", AreaType::Self},
		{"circle", AreaType::Circle},
		{"line", AreaType::Line},
		{"cone", AreaType::Cone},
		{"arbitrary", AreaType::Arbitrary}
	};

	auto result = areaTypeMap.find(s);

	if (result != areaTypeMap.end()) {
		return result->second;
	}

	std::cout << "AreaType Error: Could not parse " << s << ".\n";
	return AreaType::Undefined;
}

static Attribute stringToAttribute(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, Attribute> attributeMap = {
		{"hp", Attribute::HP},
		{"mp", Attribute::MP},
		{"sp", Attribute::SP}
	};

	auto result = attributeMap.find(s);

	if (result != attributeMap.end()) {
		return result->second;
	}

	std::cout << "Attribute Error: Could not parse " << s << ".\n";
	return Attribute::Undefined;
}

static Category stringToCategory(std::vector<std::string>& strv) {
	Category categories{};

	static const std::unordered_map<std::string, Category> categoryMap = {
		{"damaging", Category::Damaging},
		{"healing", Category::Healing},
		{"single_target", Category::SingleTarget},
		{"area of effect", Category::AreaOfEffect},
		{"direct", Category::Direct},
		{"over_time", Category::OverTime},
		{"attack", Category::Attack},
		{"skill", Category::Skill},
		{"tech", Category::Tech},
		{"spell", Category::Spell},
		{"passive", Category::Passive}
	};

	for (size_t i = 0; i < strv.size(); i++) {
		auto result = categoryMap.find(strv[i]);

		if (result != categoryMap.end()) {
			categories |= result->second;
		}
		else {
			std::cout << "Category Error: Could not parse " << strv[i] << ".\n";
		}
	}

	return categories;
}

static std::vector<std::vector<char>> stringToCharMap(const char* c) {
	std::vector<std::vector<char>> charMap;

	std::string_view str(c);
	std::vector<char> row{};

	for (char ch : str) {
		if (ch == '\n') {
			charMap.push_back(row);
			row.clear();
		}
		else {
			row.push_back(ch);
		}
	}

	// add the final row
	charMap.push_back(row);

	// format charMap
	std::vector<std::vector<char>> formattedMap(charMap[0].size(), std::vector<char>(charMap.size()));

	for (size_t i = 0; i < charMap.size(); i++) {
		for (size_t j = 0; j < charMap[i].size(); j++) {
			formattedMap[j][i] = charMap[i][j];
		}
	}

	return formattedMap;
}

static Effect stringToEffect(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, Effect> effectMap = {
		{"wpn", Effect::WeaponDamage},
		{"atk", Effect::AttackDamage},
		{"spl", Effect::SpellDamage},
		{"heal", Effect::Healing}
	};

	auto result = effectMap.find(s);

	if (result != effectMap.end()) {
		return result->second;
	}

	std::cout << "Effect Error: Could not parse " << s << ".\n";
	return Effect::Undefined;
}

static Element stringToElement(std::vector<std::string>& strv) {
	Element elements{};

	static const std::unordered_map<std::string, Element> elementMap = {
		{"physical", Element::Physical},
		{"arcane", Element::Arcane},
		{"fire", Element::Fire},
		{"ice", Element::Ice},
		{"lightning", Element::Lightning},
		{"poison", Element::Poison},
		{"light", Element::Light},
		{"dark", Element::Dark}
	};

	for (size_t i = 0; i < strv.size(); i++) {
		auto result = elementMap.find(strv[i]);

		if (result != elementMap.end()) {
			elements |= result->second;
		}
		else {
			std::cout << "Element Error: Could not parse " << strv[i] << ".\n";
		}
	}

	return elements;
}

static EquipType stringToEquipType(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, EquipType> equipTypeMap = {
		{"unarmed", EquipType::Unarmed},
		{"sword", EquipType::Sword},
		{"axe", EquipType::Axe},
		{"mace", EquipType::Mace},
		{"dagger", EquipType::Dagger},
		{"spear", EquipType::Spear},
		{"bow", EquipType::Bow},
		{"staff", EquipType::Staff},
		{"wand", EquipType::Wand}
	};

	auto result = equipTypeMap.find(s);

	if (result != equipTypeMap.end()) {
		return result->second;
	}

	std::cout << "Equip Type Error: Could not parse " << s << ".\n";
	return EquipType::Undefined;
}

static std::vector<int64_t> stringToInt64(const char* c) {
	std::vector<int64_t> result{};
	std::vector<std::string> strv = splitString(c, ',');
	result.reserve(strv.size());

	for (size_t i = 0; i < strv.size(); i++) {
		result.push_back(std::stoll(strv[i]));
	}

	return result;
}

static ItemRarity stringToItemRarity(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, ItemRarity> rarityMap = {
		{"common", ItemRarity::Common},
		{"magical", ItemRarity::Magical},
		{"rare", ItemRarity::Rare},
		{"epic", ItemRarity::Epic},
		{"fixed_artifact", ItemRarity::FixedArtifact},
		{"set_artifact", ItemRarity::SetArtifact},
		{"random_artifact", ItemRarity::RandomArtifact}
	};

	auto result = rarityMap.find(s);

	if (result != rarityMap.end()) {
		return result->second;
	}

	std::cout << "Item Rarity Error: Could not parse " << s << ".\n";
	return ItemRarity::Undefined;
}

static ItemType stringToItemType(const char* c) {
	std::string s(c);

	static const std::unordered_map<std::string, ItemType> typeMap = {
		{"weapon", ItemType::Weapon},
		{"armor", ItemType::Armor},
		{"ability_gem", ItemType::AbilityGem},
		{"consumable", ItemType::Consumable},
		{"potion", ItemType::Potion},
		{"scroll", ItemType::Scroll}
	};

	auto result = typeMap.find(s);

	if (result != typeMap.end()) {
		return result->second;
	}

	std::cout << "Item Type Error: Could not parse " << s << ".\n";
	return ItemType::Undefined;
}

GameData::GameData() {

}

void GameData::LoadData() {
	sqlite3* db;
	int error = 0;

	error = sqlite3_open_v2("ds4_data.db", &db, SQLITE_OPEN_READONLY, NULL);
	if (error == SQLITE_OK) {
		std::cout << "Database successfully opened.\n";
		char* errorMessage = 0;
		std::string query;

		// Abilities
		query = "SELECT COUNT(*) FROM Ability;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeAbilities, 0, &errorMessage);

		query = "SELECT * FROM Ability;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadAbilities, 0, &errorMessage);

		query = "SELECT * FROM AbilityEffect;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadAbilityEffects, 0, &errorMessage);

		std::cout << "\n";

		// Items
		query = "SELECT COUNT(*) FROM Item;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeItems, 0, &errorMessage);

		query = "SELECT * FROM Item;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadItems, 0, &errorMessage);

		std::cout << "\n";

		// Monsters
		query = "SELECT COUNT(*) FROM Monster;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeMonsters, 0, &errorMessage);

		query = "SELECT * FROM Monster;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsters, 0, &errorMessage);

		query = "SELECT * FROM MonsterAbility;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsterAbilities, 0, &errorMessage);

		query = "SELECT * FROM MonsterAI;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadMonsterAI, 0, &errorMessage);

		std::cout << "\n";

		// Races
		query = "SELECT COUNT(*) FROM Race;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeRaces, 0, &errorMessage);

		query = "SELECT * FROM Race;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadRaces, 0, &errorMessage);

		std::cout << "\n";

		// Rooms
		query = "SELECT COUNT(*) FROM Room;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeRooms, 0, &errorMessage);

		query = "SELECT * FROM Room;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadRooms, 0, &errorMessage);

		std::cout << "\n";

		// Tiles
		query = "SELECT COUNT(*) FROM Tile;";
		error = sqlite3_exec(db, query.c_str(), GameData::resizeTiles, 0, &errorMessage);

		query = "SELECT * FROM Tile;";
		error = sqlite3_exec(db, query.c_str(), GameData::loadTiles, 0, &errorMessage);

		std::cout << "\n";
	}
	else {
		std::cout << "An error has occured opening the database. ERROR " << error << ": " << sqlite3_errmsg(db) << "\n";
	}

	std::cout << "Closing connection to the database...\n";
	sqlite3_close(db);
	std::cout << "Connection closed.\n\n";
}

AbilityData* GameData::GetAbility(size_t id) {
	if (id < abilities.size()) {
		return &abilities[id];
	}
	return nullptr;
}

ItemData* GameData::GetItem(size_t id) {
	if (id < items.size()) {
		return &items[id];
	}
	return nullptr;
}

MonsterData* GameData::GetMonster(size_t id) {
	if (id < monsters.size()) {
		return &monsters[id];
	}
	return nullptr;
}

RaceData* GameData::GetRace(size_t id) {
	if (id < races.size()) {
		return &races[id];
	}
	return nullptr;
}

RoomData* GameData::GetRandomRoom(pcg64& pcg, size_t currentFloor) {
	std::vector<std::pair<size_t, int64_t>> possibleRooms{};
	int64_t weightSum = 0;

	for (auto& rd : rooms) {
		if (rd.MinDepth <= currentFloor && rd.MaxDepth >= currentFloor) {
			possibleRooms.push_back({ rd.ID, rd.Weight });
			weightSum += rd.Weight;
		}
	}

	int64_t roll = Random::RandomInt64(1, weightSum, pcg);

	for (auto& pair : possibleRooms) {
		roll -= pair.second;
		if (roll <= 0) {
			return &rooms[pair.first];
		}
	}

	return nullptr;
}

TileData* GameData::GetTile(size_t id) {
	if (id < tiles.size()) {
		return &tiles[id];
	}
	return nullptr;
}

int GameData::resizeAbilities(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoull(data[0]);
	abilities.reserve(count);

	std::cout << "Loading " << count << " abilities...\n";

	return 0;
}

int GameData::loadAbilities(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	std::vector<std::string> strv{};

	AbilityData ability;

	ability.ID = std::stoull(data[0]);
	ability.Name = data[1];
	ability.Icon = data[2];

	if (data[3] != nullptr) {
		ability.Description = data[3];
	}

	ability.IsPassive = (std::stoi(data[4]) == 1);

	strv = splitString(data[5], ',');
	ability.Categories = stringToCategory(strv);

	strv = splitString(data[6], ',');
	ability.Elements = stringToElement(strv);

	if (data[7] != nullptr) {
		strv = splitString(data[7], ',');
		for (auto& s : strv) {
			ability.RequiredEquipTypes.push_back(stringToEquipType(s.c_str()));
		}
	}

	if (data[8] != nullptr) {
		ability.AreaType = stringToAreaType(data[8]);
	}

	if (data[9] != nullptr) {
		ability.AreaArguments = stringToInt64(data[9]);
	}

	ability.IsProjectile = (std::stoi(data[10]) == 1);
	ability.UseTime = std::stoll(data[11]);
	ability.Cooldown = std::stoll(data[12]);
	ability.MaxCharges = std::stoll(data[13]);
	ability.MPCost = std::stoll(data[14]);
	ability.SPCost = std::stoll(data[15]);
	ability.HitChance = std::stoll(data[16]);
	ability.CanDodge = (std::stoi(data[17]) == 1);
	ability.CanBlock = (std::stoi(data[18]) == 1);
	ability.CanCrit = (std::stoi(data[19]) == 1);
	ability.CanDoubleStrike = (std::stoi(data[20]) == 1);

	abilities.push_back(ability);

	std::cout << "Loaded " << data[1] << " into the ability vector.\n";

	return 0;
}

int GameData::loadAbilityEffects(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	AbilityEffect effect;
	size_t abilityID = std::stoull(data[1]);

	effect.Effect = stringToEffect(data[2]);
	effect.Attribute = stringToAttribute(data[3]);
	effect.Arguments = stringToInt64(data[4]);
	
	abilities[abilityID].Effects.push_back(effect);

	return 0;
}

int GameData::resizeItems(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoull(data[0]);
	items.reserve(count);

	std::cout << "Loading " << count << " items...\n";

	return 0;
}

int GameData::loadItems(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	ItemData item;

	item.ID = std::stoull(data[0]);
	item.Name = data[1];

	if (data[2] != nullptr) {
		item.EquipGraphic = data[2];
	}

	item.ItemRarity = stringToItemRarity(data[3]);
	item.BaseValue = std::stoll(data[4]);
	item.ItemType = stringToItemType(data[5]);
	
	if (data[6] != nullptr) {
		item.EquipType = stringToEquipType(data[6]);
	}

	item.WeaponDamage = std::stoll(data[7]);
	item.WeaponMultiplier = std::stoll(data[8]);
	item.WeaponSpeed = std::stoll(data[9]);
	item.WeaponHitChance = std::stoll(data[10]);
	item.Armor = std::stoll(data[11]);
	item.SpellDefense = std::stoll(data[12]);
	item.InvokedAbilityID = std::stoull(data[13]);

	items.push_back(item);

	std::cout << "Loaded " << data[1] << " into the item vector.\n";

	return 0;
}

int GameData::resizeMonsters(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoull(data[0]);
	monsters.reserve(count);

	std::cout << "Loading " << count << " monsters...\n";

	return 0;
}

int GameData::loadMonsters(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	MonsterData monster;

	monster.ID = std::stoull(data[0]);
	monster.Name = data[1];

	if (data[2] != nullptr) {
		monster.Title = data[2];
	}

	monster.Graphic = data[3];
	monster.IsUnique = (std::stoi(data[4]) == 1);
	monster.Level = std::stoll(data[5]);
	monster.BaseHP = std::stoll(data[6]);
	monster.BaseMP = std::stoll(data[7]);
	monster.BaseSP = std::stoll(data[8]);
	monster.BaseAttackPower = std::stoll(data[9]);
	monster.BaseSpellPower = std::stoll(data[10]);
	monster.BaseHealingPower = std::stoll(data[11]);
	monster.BaseArmor = std::stoll(data[12]);
	monster.BaseSpellDefense = std::stoll(data[13]);
	monster.BaseAccuracy = std::stoll(data[14]);
	monster.BaseEvasion = std::stoll(data[15]);
	monster.HitChance = std::stoll(data[16]);
	monster.AttackSpeed = std::stoll(data[17]);
	monster.WeaponDamage = std::stoll(data[18]);
	monster.WeaponMultiplier = std::stoll(data[19]);
	monster.WeaponType = stringToEquipType(data[20]);
	monster.EXP = std::stoll(data[21]);
	monster.Gold = std::stoll(data[22]);
	monster.LootPoints = std::stoll(data[23]);

	monsters.push_back(monster);

	std::cout << "Loaded " << data[1] << " into the monster vector.\n";

	return 0;
}

int GameData::loadMonsterAbilities(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	size_t monsterID = std::stoull(data[1]);
	monsters[monsterID].Abilities.push_back(std::stoull(data[2]));

	return 0;
}

int GameData::loadMonsterAI(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	size_t monsterID = std::stoull(data[1]);

	MonsterTactic tactic;

	tactic.AbilityID = std::stoull(data[2]);
	tactic.Rating = std::stoi(data[3]);
	tactic.Weight = std::stoll(data[4]);

	tactic.Target = stringToAITarget(data[5]);

	if (data[6] != nullptr) {
		tactic.TargetArguments = stringToInt64(data[6]);
	}

	tactic.Condition = stringToAICondition(data[7]);

	if (data[8] != nullptr) {
		tactic.ConditionArguments = stringToInt64(data[8]);
	}

	monsters[monsterID].Tactics.push_back(tactic);

	return 0;
}

int GameData::resizeRaces(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoull(data[0]);
	races.reserve(count);

	std::cout << "Loading " << count << " races...\n";

	return 0;
}

int GameData::loadRaces(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	RaceData race;

	race.ID = std::stoull(data[0]);
	race.Name = data[1];
	race.BaseHP = std::stoll(data[2]);
	race.BaseMP = std::stoll(data[3]);
	race.BaseSP = std::stoll(data[4]);
	race.BaseEXPPerLevel = std::stoll(data[5]);
	race.FolderName = data[6];

	races.push_back(race);

	std::cout << "Loaded " << data[1] << " into the race vector.\n";

	return 0;
}

int GameData::resizeRooms(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoull(data[0]);
	rooms.reserve(count);

	std::cout << "Loading " << count << " rooms...\n";

	return 0;
}

int GameData::loadRooms(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	RoomData room;

	room.ID = std::stoull(data[0]);
	room.Name = data[1];
	room.MinDepth = std::stoull(data[2]);
	room.MaxDepth = std::stoull(data[3]);
	room.Weight = std::stoll(data[4]);
	room.Tiles = stringToCharMap(data[5]);

	rooms.push_back(room);

	std::cout << "Loaded " << data[1] << " into the room vector.\n";

	return 0;
}

int GameData::resizeTiles(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;
	size_t count = 0;

	count = std::stoull(data[0]);
	tiles.reserve(count);

	std::cout << "Loading " << count << " tiles...\n";

	return 0;
}

int GameData::loadTiles(void* notUsed, int argc, char** data, char** colname) {
	notUsed = 0;

	TileData tile;

	tile.ID = std::stoull(data[0]);
	tile.Name = data[1];
	tile.SpriteNumber = std::stoi(data[2]);
	tile.MoveMod = std::stoll(data[3]);
	tile.IsPassible = (std::stoi(data[4]) == 1);
	tile.IsOpeque = (std::stoi(data[5]) == 1);

	tiles.push_back(tile);

	std::cout << "Loaded " << data[1] << " into the tile vector.\n";

	return 0;
}