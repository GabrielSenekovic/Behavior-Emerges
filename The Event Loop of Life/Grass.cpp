#include "Grass.h"
#include "Grid.h"
#include "BehaviorEmerges.h"

Grass::Grass() :Entity({ -1,-1 }, { 0,0 }, {})
{
	entityType = EntityType::GRASS;
	maturity = Maturity::GROWING;
	maturityTimer = 0;
	spaceOccupying = 2;
}
Grass::Grass(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, const bool flower) : Entity(position_in, health_in, sprites_in)
{
	entityType = flower? EntityType::FLOWER:EntityType::GRASS;
	if (health.x == health.y)
	{
		maturity = Maturity::MATURE;
	}
	else
	{
		maturity = Maturity::GROWING;
	}
	maturityTimer = 0;
	spaceOccupying = 2;
}

void Grass::Sense(const Grid& grid, const EntityManager& entityManager)
{
	//Am I being eaten or trampled?
	isBeingTrampled = (grid.tileContent[position.x + grid.grid.x * position.y][0] != nullptr || grid.tileContent[position.x + grid.grid.x * position.y][1] != nullptr);
	isBeingEaten = ((grid.tileContent[position.x + grid.grid.x * position.y][0] != nullptr && grid.tileContent[position.x + grid.grid.x * position.y][0]->entityType == EntityType::SHEEP && grid.tileContent[position.x + grid.grid.x * position.y][0]->state == EntityState::EAT) ||
		(grid.tileContent[position.x + grid.grid.x * position.y][1] != nullptr && grid.tileContent[position.x + grid.grid.x * position.y][1]->entityType == EntityType::SHEEP && grid.tileContent[position.x + grid.grid.x * position.y][1]->state == EntityState::EAT));
	if (maturity == Maturity::GROWING && health.x >= health.y)
	{
		maturity = Maturity::MATURE;
	}
	else if (maturity == Maturity::MATURE && maturityTimer >= 1)
	{
		maturity = Maturity::WITHERING;
	}
}

void Grass::Decide(Random& r, const Grid& grid)
{
	//Should I spread to an adjacent square, or should I grow?
	if (health.x <= 0)
	{
		state = EntityState::DEATH;
	}
	else if (isBeingTrampled || isBeingEaten) { state = EntityState::IDLE; }
	else if (maturity == Maturity::MATURE)
	{
		state = EntityState::BREED;
	}
}

void Grass::Act(Random& r, Grid& grid, EntityManager& entities)
{
	//Grow if not being trampled, wither if it has been mature for a while. Disappear if no HP
	if (state == EntityState::BREED)
	{
		Vector2 whereToGrow = { -1,-1 };
		if (whereToGrow == Vector2{ -1, -1 })
		{
			whereToGrow = GetRandomAdjacentPosition(r, grid, EntityType::GRASS);
			if (whereToGrow == Vector2{ -1,-1 }) { return; }
			else
			{
				Grass* temp = new Grass(whereToGrow, { (r.myRand() % 5) + 2.0f, 6.0f }, sprites, entityType == EntityType::GRASS? false: true);
				grid.tileContent[whereToGrow.x + grid.grid.x * whereToGrow.y][2] = temp;
				entities.Add(temp);
			}
		}
		state = EntityState::IDLE;
	}
	if (state == EntityState::DEATH)
	{
		dead = true;
	}
	if (maturity == Maturity::WITHERING)
	{
		health.x -= 1 / (Time::timeSpeed / Time::deltaTime);
	}
	else if (maturity == Maturity::GROWING)
	{
		health.x += 1 / (Time::timeSpeed / Time::deltaTime);
	}
	else if (maturity == Maturity::MATURE)
	{
		maturityTimer += 1 / (Time::timeSpeed / Time::deltaTime);
	}
}

void Grass::Render(BehaviorEmerges& game, Vector2 renderPosition, const Pixel& color)
{
	switch (maturity)
	{
	case Grass::Maturity::GROWING:	
		game.DrawDecal(renderPosition, sprites[1], { 1,1 }, color); break;
	case Grass::Maturity::MATURE: 
		game.DrawDecal(renderPosition, sprites[0], { 1,1 }, color); break;
	case Grass::Maturity::WITHERING: 
		game.DrawDecal(renderPosition, sprites[0], { 1,1 }, Pixel(158, 124, 66)); break;
	}
}
