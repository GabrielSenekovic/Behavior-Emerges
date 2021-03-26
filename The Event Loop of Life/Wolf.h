#ifndef WOLF_H
#define WOLF_H

#include "Animal.h"

class Wolf :public Animal
{
	EntityState renderState;
public:
	Wolf(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageWolfPosition);
	BehaviorTree behavior;
	Grid::GridSearchQuery searchQuery;
	void Render(BehaviorEmerges& game, Vector2 position) override;
};

#endif