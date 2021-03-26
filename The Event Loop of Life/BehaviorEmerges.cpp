#include "BehaviorEmerges.h"

BehaviorEmerges::BehaviorEmerges(IntVector2 dim_in): dim(dim_in),
tile(nullptr), rock(nullptr), averageSheepPosition({0,0}), averageWolfPosition({0,0}), sheepRadius(0)
{
	Time::SetTimeScale(2);
}

bool BehaviorEmerges::OnUserCreate()
{
	tile = new Decal(new Sprite("./Assets/Tile.png"));
	rock = new Decal(new Sprite("./Assets/Rock.png"));

	grid.Initialize(r);
	entityManager.Initialize(r, grid, averageSheepPosition, averageWolfPosition, sheepRadius);

	for (unsigned int i = 0; i < 10; i++)
	{
		uint8_t width = 8;
		Sprite* temp = new Sprite("./Assets/Letters.png", olc::vi2d(width), olc::vi2d((width * 29) + (i * width), 0));
		digits[i] = new Decal(temp);
	}

	frameRate = HUDElement(Word("FRAMERATE", 10, 10), Number(00000, 10 + 8 * 10, 10, 1, 5, digits));
	amountOfTurns = HUDElement(Word("TURNS", 10, 20), Number(00000, 10 + 8 * 10, 20, 1, 5, digits));
	amountOfGrass = HUDElement(Word("GRASS", 10, 100), Number(0000, 10 + 8 * 10, 100, 1, 4, digits));
	amountOfSheep = HUDElement(Word("SHEEP", 10, 110), Number(0000, 10 + 8 * 10, 110, 1, 4, digits));
	amountOfWolves = HUDElement(Word("WOLVES", 10, 120), Number(0000, 10 + 8 * 10, 120, 1, 4, digits));

	return true;
}

bool BehaviorEmerges::OnUserUpdate(float fElapsedTime)
{
	Time::deltaTime = fElapsedTime;
	float temp = timer;
	timer = (timer + fElapsedTime) * (timer <= Time::timeSpeed);

	if(timer == 0){ amountOfTurns.number.Add(1); }

	int n_amountOfGrass = 0;
	int n_amountOfSheep = 0;
	int n_amountOfWolves = 0;

	std::vector<Vector2> newAverageSheepPosition;
	std::vector<Vector2> newAverageWolfPosition;

	for (int i = 0; i < entityManager.Size(); i++)
	{
		if (entityManager.GetEntity(i)->entityType == Entity::EntityType::GRASS) { n_amountOfGrass++; }
		if (entityManager.GetEntity(i)->entityType == Entity::EntityType::SHEEP) { n_amountOfSheep++; }
		if (entityManager.GetEntity(i)->entityType == Entity::EntityType::WOLF) { n_amountOfWolves++; }

		if (entityManager.GetEntity(i)->entityType == Entity::EntityType::SHEEP)
		{
			Sheep* temp = dynamic_cast<Sheep*>(entityManager.GetEntity(i));
			temp->behavior.Execute();
			temp->Update();
			newAverageSheepPosition.push_back(temp->position);
		}
		if (entityManager.GetEntity(i)->entityType == Entity::EntityType::WOLF)
		{
			Wolf* temp = dynamic_cast<Wolf*>(entityManager.GetEntity(i));
			temp->behavior.Execute();
			temp->Update();
			newAverageWolfPosition.push_back(temp->position);
		}
		if (entityManager.GetEntity(i)->entityType == Entity::EntityType::GRASS || entityManager.GetEntity(i)->entityType == Entity::EntityType::FLOWER)
		{
			Grass* grass = dynamic_cast<Grass*>(entityManager.GetEntity(i));
			if (timer == 0)
			{
				grass->Sense(grid, entityManager);
				grass->Decide(r, grid);
			}
			grass->Act(r, grid, entityManager);
		}
		if (entityManager.GetEntity(i)->dead)
		{
			grid.tileContent[entityManager.GetEntity(i)->position.x + grid.grid.x * entityManager.GetEntity(i)->position.y][entityManager.GetEntity(i)->spaceOccupying] = nullptr;
			DEL(entityManager.entities[i]);
			entityManager.entities.erase(entityManager.entities.begin() + i);
			i--;
		}
	}
	entityManager.dog->behavior.Execute();
	entityManager.herder->behavior.Execute();
	averageSheepPosition = { 0,0 };
	for (int i = 0; i < newAverageSheepPosition.size(); i++)
	{
		averageSheepPosition += newAverageSheepPosition[i];
	}
	averageSheepPosition /= newAverageSheepPosition.size();
	for (int i = 0; i < newAverageSheepPosition.size(); i++)
	{
		if ((newAverageSheepPosition[i] - averageSheepPosition).mag() > sheepRadius)
		{
			sheepRadius = (newAverageSheepPosition[i] - averageSheepPosition).mag();
		}
	}
	averageWolfPosition = { 0,0 };
	for (int i = 0; i < newAverageWolfPosition.size(); i++)
	{
		averageWolfPosition += newAverageWolfPosition[i];
	}
	averageWolfPosition /= newAverageWolfPosition.size();

	for (int i = 0; i < entityManager.crows.size(); i++)
	{
		entityManager.UpdateBoid(i);
		entityManager.crows[i].Move(fElapsedTime, grid.grid);
	}
	
	frameRate.number.Reset(); 
	frameRate.number.Add(fElapsedTime * 1000000);
	amountOfGrass.number.Reset(); amountOfGrass.number.Add(n_amountOfGrass);
	amountOfSheep.number.Reset(); amountOfSheep.number.Add(n_amountOfSheep);
	amountOfWolves.number.Reset(); amountOfWolves.number.Add(n_amountOfWolves);

	if (timer == 0) { timer = fElapsedTime - (1 - temp); }

	OnUserDraw();
	return true;
}

void BehaviorEmerges::OnUserDraw()
{
	vi2d centering = { grid.grid.x * tile->sprite->width / 2, grid.grid.y * tile->sprite->height / 2 };
	for (size_t i = 0; i < grid.grid.x * grid.grid.y; i++)
	{
		int x = i % grid.grid.x; int y = i / grid.grid.y;
		vi2d position = { tile->Width() * x + dim.x / 2 - centering.x, tile->Height() * y + dim.y / 2 - centering.y };
		bool hasGrass = grid.tileContent[i][2] != nullptr && grid.tileContent[i][2]->entityType == Entity::EntityType::GRASS;
		bool hasFlower = grid.tileContent[i][2] != nullptr && grid.tileContent[i][2]->entityType == Entity::EntityType::FLOWER;
		
		Pixel color = hasGrass || hasFlower || grid.tileTraversibility[i] == 2 ? grid.tileColors[i] : Pixel( 40, 30, 20 );

	/*	color = grid.tileContent[i][0] != nullptr && grid.tileContent[i][1] != nullptr ? Pixel(255, 0, 255) :
				grid.tileContent[i][0] != nullptr ? Pixel(255, 0, 0) :
				grid.tileContent[i][1] != nullptr ? Pixel(0, 0, 255) :
				color; *///Debugging tool to see what tiles are currently occupied

		DrawDecal(position, tile, { 1,1 }, color);
		if (grid.tileTraversibility[i] == 2)
		{
			DrawDecal(position, rock, { 1,1 }, color);
		}
		if (hasGrass){dynamic_cast<Grass*>(grid.tileContent[i][2])->Render(*this, position, color);}
		if (hasFlower) 
		{ 
			dynamic_cast<Grass*>(grid.tileContent[i][2])->Render(*this, position, grid.flowerColors[i]); 
		}
	}
	for (size_t i = 0; i < entityManager.Size(); i++)
	{
		Vector2 position = { tile->Width() * entityManager.GetEntity(i)->GetPosition().x + dim.x / 2 - centering.x, tile->Height() * entityManager.GetEntity(i)->GetPosition().y + dim.y / 2 - centering.y };
		if (entityManager.GetEntity(i)->entityType != Entity::EntityType::GRASS)
		{
			entityManager.GetEntity(i)->Render(*this, position);
		}
	}
	Vector2 position = { tile->Width() * entityManager.dog->GetPosition().x + dim.x / 2 - centering.x, tile->Height() * entityManager.dog->GetPosition().y + dim.y / 2 - centering.y };
	entityManager.dog->Render(*this, position);
	position = { tile->Width() * entityManager.herder->GetPosition().x + dim.x / 2 - centering.x, tile->Height() * entityManager.herder->GetPosition().y + dim.y / 2 - centering.y };
	entityManager.herder->Render(*this, position);
	for (size_t i = 0; i < entityManager.crows.size(); i++)
	{
		Vector2 position = { tile->Width() * entityManager.crows[i].position.x + dim.x / 2 - centering.x, tile->Height() * entityManager.crows[i].position.y + dim.y / 2 - centering.y };
		entityManager.crows[i].Render(*this, entityManager.crow, position);
	}
	frameRate.Render(*this);
	amountOfTurns.Render(*this);
	amountOfGrass.Render(*this);
	amountOfSheep.Render(*this);
	amountOfWolves.Render(*this);
}

void BehaviorEmerges::Quit()
{
	DEL(tile);
	DEL(rock);
	
	for (int i = 0; i < grid.tileContent.size(); i++)
	{
		DEL(grid.tileContent[i][0]);
		DEL(grid.tileContent[i][1]);
		DEL(grid.tileContent[i][2]);
	}
}
