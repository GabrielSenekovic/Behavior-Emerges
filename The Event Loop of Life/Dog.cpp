#include "Dog.h"
#include "BehaviorEmerges.h"

Dog::Dog()
{
}

Dog::Dog(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageSheepPosition, float& sheepRadius) : Animal(position_in, health_in, sprites_in), target(Vector2(-1,-1))
{
	angle = 0;
	entityType = EntityType::DOG;
	behavior = BehaviorTree
	(new Selector
	({
		new Sequencer //Chase away wolves from sheep if being commanded
		(
			{
				new Leaf([&]() {
				if (target == Vector2(-1,-1))
				{ 
					return ReturnType::R_FAILURE; 
				} 
				destination = target;
				return ReturnType::R_SUCCESS; 
				}), //Check if is commanded
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
				new Leaf([&]() 
					{
						ReturnType temp = Move();
						if (temp == ReturnType::R_SUCCESS)
						{
							target = Vector2(-1, -1);
							return temp;
						}
						return temp;
					})
			}
		),
		new Sequencer //Circle the sheep somehow
		(
			{
				new Leaf([&]() {
				destination = Vector2(sheepRadius * sin(angle) + averageSheepPosition.x, sheepRadius * cos(angle) + averageSheepPosition.y);
				destination = Vector2((int)destination.x, (int)destination.y);
				grid.ClampPositionToGrid(destination);
				angle+=0.000005f;
				return ReturnType::R_SUCCESS;
					}),
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
			}
		)
	}
	));
}

void Dog::Render(BehaviorEmerges& game, Vector2 position)
{
	game.DrawDecal(position, sprites[0], { 1,1 }, olc::WHITE);
}

ReturnType Dog::Command(Vector2& wolfPosition)
{
	if (target == Vector2(-1, -1))
	{
		target = wolfPosition;
		return ReturnType::R_SUCCESS;
	}
	return ReturnType::R_FAILURE;
}
