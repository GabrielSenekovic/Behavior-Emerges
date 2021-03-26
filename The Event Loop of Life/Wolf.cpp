#include "Wolf.h"
#include "BehaviorEmerges.h"

Wolf::Wolf(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageWolfPosition) : Animal(position_in, health_in, sprites_in),
renderState(EntityState::IDLE)
{
	entityType = EntityType::WOLF;
	behavior = BehaviorTree
	(new Selector
	({
		new Sequencer //Herder branch
		(
			{
				new Leaf([&]() {return grid.FindEntityOfType(position, 2, EntityType::HERDER, searchQuery); }),
				new Leaf([&]()
					{
						renderState = EntityState::EVADE;
						if (destination == Vector2(-1,-1))
						{
							int sheep_index = position.x + grid.grid.x * position.y;
							int* constraints = grid.GetValidConstraints(sheep_index);
							float lastDistance = 0;
							for (int y = constraints[3]; y < constraints[5]; y++)
							{
								for (int x = constraints[2]; x < constraints[4]; x++)
								{
									float distance = 0;
									for (int i = 0; i < searchQuery.v.size(); i++)
									{
										distance += (Vector2(x, y) - searchQuery.v[i]).mag();
									}
									if (distance > lastDistance)
									{
										destination = Vector2(x, y);
										lastDistance = distance;
									}
								}
							}
							delete constraints;
						}
						return ReturnType::R_SUCCESS;
					}
						),
				new Leaf([&]() {return FindPath(grid, destination); }),
				new Leaf([&]() {
					if (destination == Vector2{ -1,-1 }) { return ReturnType::R_FAILURE; }
					else
					{
						grid.tileContent[position.x + grid.grid.x * position.y][spaceOccupying] = nullptr;
						if (grid.tileContent[destination.x + grid.grid.x * destination.y][0] == nullptr) { grid.tileContent[destination.x + grid.grid.x * destination.y][0] = this; spaceOccupying = 0; }
						else { grid.tileContent[destination.x + grid.grid.x * destination.y][1] = this; spaceOccupying = 1; }
					}return ReturnType::R_SUCCESS;
					}),
				new Leaf([&]() {return Move(); })
			}
		),
		new Sequencer //Dog branch
		(
			{
				new Leaf([&]() {return grid.FindEntityOfType(position, 2, EntityType::DOG, searchQuery); }),
				new Leaf([&]()
					{
						renderState = EntityState::EVADE;
						if (destination == Vector2(-1,-1))
						{
							int sheep_index = position.x + grid.grid.x * position.y;
							int* constraints = grid.GetValidConstraints(sheep_index);
							float lastDistance = 0;
							for (int y = constraints[3]; y < constraints[5]; y++)
							{
								for (int x = constraints[2]; x < constraints[4]; x++)
								{
									float distance = 0;
									for (int i = 0; i < searchQuery.v.size(); i++)
									{
										distance += (Vector2(x, y) - searchQuery.v[i]).mag();
									}
									if (distance > lastDistance)
									{
										destination = Vector2(x, y);
										lastDistance = distance;
									}
								}
							}
							delete constraints;
						}
						return ReturnType::R_SUCCESS;
					}
						),
				new Leaf([&]() {return FindPath(grid, destination); }),
				new Leaf([&]() {
					if (destination == Vector2{ -1,-1 }) { return ReturnType::R_FAILURE; }
					else
					{
						grid.tileContent[position.x + grid.grid.x * position.y][spaceOccupying] = nullptr;
						if (grid.tileContent[destination.x + grid.grid.x * destination.y][0] == nullptr) { grid.tileContent[destination.x + grid.grid.x * destination.y][0] = this; spaceOccupying = 0; }
						else { grid.tileContent[destination.x + grid.grid.x * destination.y][1] = this; spaceOccupying = 1; }
					}return ReturnType::R_SUCCESS;
					}),
				new Leaf([&]() {return Move(); })
			}
		),
		new Sequencer //Hungry branch
		(
			{
				new Leaf([&]() //Am I hungry?
					{
						if (health.x < (health.y * 0.75f))
						{
							return ReturnType::R_SUCCESS;
						}
						return ReturnType::R_FAILURE;
					}),
				new Leaf([&]() {return grid.FindEntityOfType(position, 3, EntityType::SHEEP, searchQuery); }),
				new Leaf([&]() {
						renderState = EntityState::PURSUE;
						if (destination == Vector2(-1,-1))
						{
							destination = searchQuery.v[r.myRand() % searchQuery.v.size()];
						}
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
				new Leaf([&]() {return Move(); }),
				new Leaf([&]() //Eat sheep
					{
						int placementOfPrey = spaceOccupying == 0 ? 1 : 0;
						int index = position.x + grid.grid.x * position.y;
						if (grid.tileContent[index][placementOfPrey] != nullptr && grid.tileContent[index][placementOfPrey]->entityType == EntityType::SHEEP)
						{
							health.x += 5;
							grid.tileContent[index][placementOfPrey]->health.x = 0;
							dynamic_cast<Animal*>(grid.tileContent[index][placementOfPrey])->Die();
							//This is ok since only animals can occupy position 0 and 1
							return ReturnType::R_SUCCESS;
						}
						return ReturnType::R_FAILURE;
					})
			}
		),
		new Sequencer //Wolf Branch
		(
			{
				new Leaf([&]() {
								float distance = (averageWolfPosition - position).mag();
								if (distance > 5) { return ReturnType::R_SUCCESS; }
								else
								{
									return ReturnType::R_ERROR;
								}}
						),
				new Leaf([&]() {renderState = EntityState::WANDER; return grid.FindEntityOfType(position, 3, EntityType::WOLF, searchQuery); }),
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
		),
		new Leaf([&]() {	renderState = EntityState::WANDER;
							return Wander(r, grid); })
		})
	);
}

void Wolf::Render(BehaviorEmerges& game, Vector2 renderPosition)
{
	Vector2 emotePosition = { renderPosition.x - 0.8f, renderPosition.y - 0.8f };
	switch (renderState)
	{
		case EntityState::IDLE:
		case EntityState::WANDER:
		{
			game.DrawDecal(renderPosition, sprites[0], { 1,1 }, olc::WHITE);
			game.DrawDecal(emotePosition, sprites[1], { 1,1 }, olc::WHITE);
			break;
		}
		case EntityState::EVADE:
		{
			game.DrawDecal(renderPosition, sprites[0], { 1,1 }, olc::WHITE);
			game.DrawDecal(emotePosition, sprites[2], { 1,1 }, olc::YELLOW);
			break;
		}
		case EntityState::PURSUE:
		{
			game.DrawDecal(renderPosition, sprites[0], { 1,1 }, olc::WHITE);
			game.DrawDecal(emotePosition, sprites[2], { 1,1 }, olc::RED);
			break;
		}
		case EntityState::EAT:
		{
			game.DrawDecal(renderPosition, sprites[0], { 1,1 }, olc::WHITE);
			game.DrawDecal(emotePosition, sprites[4], { 1,1 }, olc::WHITE);
			break;
		}
		case EntityState::BREED:
		{
			game.DrawDecal(renderPosition, sprites[0], { 1,1 }, olc::WHITE);
			game.DrawDecal(emotePosition, sprites[3], { 1,1 }, olc::WHITE);
			break;
		}
		case EntityState::DEATH:
		{
			game.DrawDecal(renderPosition, sprites[0], { 1,1 }, olc::DARK_GREY);
			break;
		}
	}
}
