#ifndef SHEEP_H
#define SHEEP_H

#include "Animal.h"

class Sheep :public Animal
{
	EntityState renderState;
public:
	BehaviorTree behavior;
	Grid::GridSearchQuery searchQuery;

	Sheep(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageSheepPosition);

	void Render(BehaviorEmerges& game, Vector2 position) override;
};

#endif