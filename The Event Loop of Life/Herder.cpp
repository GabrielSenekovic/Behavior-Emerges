#include "Herder.h"
#include "BehaviorEmerges.h"

Herder::Herder()
{
}

Herder::Herder(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageSheepPosition, Dog& dog_in) : Animal(position_in, health_in, sprites_in), dog(&dog_in)
{
	entityType = EntityType::HERDER;
	behavior = BehaviorTree
	(new Selector
	({
		new Sequencer //Chase away wolves
		(
			{
				new Leaf([&]() {return grid.FindEntityOfType(dog->position, 3, EntityType::WOLF, searchQuery); }),
				new Leaf([&]()
					{
						if (dog->target == Vector2(-1,-1))
						{
							return dog->Command(searchQuery.v[r.myRand() % searchQuery.v.size()]);
						}
						return ReturnType::R_FAILURE;
					})
			}
		),
		new Sequencer //Keep close to sheep
		(
			{
				new Leaf([&]() {
								float distance = (averageSheepPosition - position).mag();
								if (distance > 3) { return ReturnType::R_SUCCESS; }
								else
								{
									return ReturnType::R_ERROR;
								}}
						),
				new Leaf([&]() {return grid.FindEntityOfType(position, 8, EntityType::SHEEP, searchQuery); }),
				new Leaf([&]() {if (searchQuery.v.size() > 0) { return ReturnType::R_SUCCESS; }
								else { return ReturnType::R_FAILURE; }}),
				new Leaf([&]() {destination = averageSheepPosition; 
								destination = Vector2((int)destination.x, (int)destination.y);
								return ReturnType::R_SUCCESS; }),
				new Leaf([&]() {return FindPath(grid, destination); }),
				new Leaf([&]() {
				if (destination == Vector2{ -1,-1 }) { return ReturnType::R_FAILURE; }
				else
				{
					grid.tileContent[position.x + grid.grid.x * position.y][spaceOccupying] = nullptr;
					if (grid.tileContent[destination.x + grid.grid.x * destination.y][0] == nullptr) { grid.tileContent[destination.x + grid.grid.x * destination.y][0] = this; spaceOccupying = 0; }
					else { grid.tileContent[destination.x + grid.grid.x * destination.y][1] = this; spaceOccupying = 1; }
				} return ReturnType::R_SUCCESS;
				}),
				new Leaf([&]() {return Move(); })
			}
		) }
	));
}

void Herder::Render(BehaviorEmerges& game, Vector2 position)
{
	game.DrawDecal(position, sprites[0], { 1,1 }, olc::WHITE);
}