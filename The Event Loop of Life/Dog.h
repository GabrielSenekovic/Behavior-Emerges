#ifndef DOG_H
#define DOG_H

#include "Animal.h"

class Dog :public Animal
{
	EntityState renderState;
public:
	Dog();
	Dog(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageSheepPosition, float& sheepRadius);
	BehaviorTree behavior;
	Vector2 target;
	float angle;
	Grid::GridSearchQuery searchQuery;
	void Render(BehaviorEmerges& game, Vector2 position) override;
	ReturnType Command(Vector2& wolfPosition);
};

#endif