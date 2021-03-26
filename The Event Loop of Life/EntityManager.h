#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "Entity.h"
#include "Grass.h"
#include "Sheep.h"
#include "Wolf.h"
#include "Grid.h"
#include "Crow.h"
#include "Dog.h"
#include "Herder.h"

//The EntityManager was made because when an entity was deleted because it was dead, the target pointer in the wolf class would contain garbage valuyes
//Therefore, I had to make a class that contains a function that can return an entity based on a handle value
#define DEL(val) {delete val; val = nullptr;};

class EntityManager
{
	olc::Decal* sheep;
	olc::Decal* sheep_eating;
	olc::Decal* wolf;
	olc::Decal* grass;
	olc::Decal* grass_growing;
	olc::Decal* flower;
	olc::Decal* flower_growing;
	olc::Decal* wander; //A question mark
	olc::Decal* pursue; //An exclamation point
	olc::Decal* breed; //A heart
	olc::Decal* eat; //A jaw
	olc::Decal* herderSprite;
	olc::Decal* dogSprite;
public:
	olc::Decal* crow;

	EntityManager();
	void Initialize(Random& r, Grid& grid, Vector2& averageSheepPosition_in, Vector2& averageWolfPosition_in, float& sheepRadius);
	void Quit();

	std::vector<Entity*> entities;
	std::vector<Crow> crows;
	Dog* dog;
	Herder* herder;

	void Add(Entity* entity);

	Entity* GetEntity(const uint32_t& i);

	void UpdateBoid(const int& i);

	size_t Size();
};

#endif ENTITYMANAGER_H