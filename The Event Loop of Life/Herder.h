#ifndef HERDER_H
#define HERDER_H

#include "Animal.h"
#include "Dog.h"

class Herder :public Animal
{
	EntityState renderState;
public:
	Herder();
	Herder(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in, Grid& grid, Random& r, Vector2& averageSheepPosition, Dog& dog_in);
	Dog* dog;
	BehaviorTree behavior;
	Grid::GridSearchQuery searchQuery;
	void Render(BehaviorEmerges& game, Vector2 position) override;
};

#endif
