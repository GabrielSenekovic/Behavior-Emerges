#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include "olcPixelGameEngine.h"
using namespace olc;

#include "Random.h"
#include <math.h>

#define Vector2 vf2d
#define IntVector2 vi2d

class BehaviorEmerges;
class EntityManager;
class Grid;

class Entity
{
protected:
	enum class EntityState
	{
		EVADE,
		EAT,
		BREED,
		PURSUE,
		WANDER,
		DEATH,
		IDLE //used only for example, when sheep have bred once to prevent them from breeding eternally
	};
public:
	enum class EntityType
	{
		GRASS,
		SHEEP,
		WOLF,
		FLOWER,
		MANURE,
		HERDER,
		DOG
	};
	Entity();
	Entity(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in);
	Vector2 position;
	Vector2 health; //x = current health, y = max health
	EntityState state;
	std::vector<Decal*> sprites;
	uint8_t spaceOccupying; //0 = first, 1 = second
	bool dead; //flag for program to delete the entity
	EntityType entityType;

	Vector2 GetRandomAdjacentPosition(Random& r, const Grid& grid);
	Vector2 GetRandomAdjacentPosition(Random& r, const Grid& grid, const EntityType& type);

	virtual Vector2 GetPosition();

	virtual void Render(BehaviorEmerges& game, Vector2 position);
};
#endif