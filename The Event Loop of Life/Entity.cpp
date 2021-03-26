#include "Entity.h"
#include "BehaviorEmerges.h"
#include "Grid.h"
#include "EntityManager.h"

Entity::Entity()
{
}

Entity::Entity(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in) :
	position(position_in), health(health_in),
	dead(false), state(EntityState::IDLE), sprites(sprites_in)
{
}

Vector2 Entity::GetRandomAdjacentPosition(Random& r, const Grid& grid)
{
	int32_t index = position.x + grid.grid.x * position.y;
	int* constraints = grid.GetValidConstraints(index);
	std::vector<Vector2> possiblePositions = {};
	for (int y = constraints[3]; y < constraints[5]; y++)
	{
		for (int x = constraints[2]; x < constraints[4]; x++)
		{
			if (position != Vector2(x, y))
			{
				possiblePositions.push_back({ (float)x, (float)y });
			}
		}
	}
	return possiblePositions[r.myRand() % possiblePositions.size()];
}

Vector2 Entity::GetRandomAdjacentPosition(Random& r, const Grid& grid, const EntityType& type)
{
	int32_t index = position.x + grid.grid.x * position.y;
	int* constraints = grid.GetValidConstraints(index);
	std::vector<Vector2> possiblePositions = {};
	for (int y = constraints[3]; y < constraints[5]; y++)
	{
		for (int x = constraints[2]; x < constraints[4]; x++)
		{
			if (type == EntityType::SHEEP || type == EntityType::WOLF)
			{
				if (position != Vector2(x, y) && (grid.tileContent[x + grid.grid.x * y][0] == nullptr || grid.tileContent[x + grid.grid.x * y][1] == nullptr) && grid.tileTraversibility[x + grid.grid.x * y] < 2)
				{
					possiblePositions.push_back({ (float)x, (float)y });
				}
			}
			else
			{
				if (position != Vector2(x, y) && 
					(grid.tileContent[x + grid.grid.x * y][0] == nullptr || 
				     grid.tileContent[x + grid.grid.x * y][1] == nullptr) && 
					 grid.tileContent[x + grid.grid.x * y][2] == nullptr  &&
					 grid.tileTraversibility[x+grid.grid.x * y] < 2)
				{
					possiblePositions.push_back({ (float)x, (float)y });
				}
			}
		}
	}
	if (possiblePositions.size() == 0) { return { -1,-1 }; }
	return possiblePositions[r.myRand() % possiblePositions.size()];
}

Vector2 Entity::GetPosition()
{
	return position;
}

void Entity::Render(BehaviorEmerges& game, Vector2 renderPosition){}
