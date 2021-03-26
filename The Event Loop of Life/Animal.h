#ifndef ANIMAL_H
#define ANIMAL_H

#include <deque>
#include "Entity.h"
#include "BehaviorTree.h"
#include "Time.h"

using BehaviorNode = BehaviorTree::BehaviorNode;
using Selector = BehaviorTree::Selector;
using Leaf = BehaviorTree::Leaf;
using Sequencer = BehaviorTree::Sequencer;
using Decorator = BehaviorTree::Decorator;

class Animal : public Entity
{
	struct Node
	{
		Vector2 position;
		float costFromStart;
		float heuristic;
		Node* previousPosition;
		float points() { return heuristic + costFromStart; }
		Node() :position(Vector2(-1, -1)), costFromStart(0), heuristic(0), previousPosition(nullptr) {}
		Node(Vector2 position_in, float cost_in, float heuristic_in, Node* previousPosition_in)
		{
			position = position_in;
			if (previousPosition_in != nullptr)
			{
				previousPosition = new Node(previousPosition_in->position, previousPosition_in->costFromStart, previousPosition_in->heuristic, previousPosition_in->previousPosition);
			}
			else
			{
				previousPosition = nullptr;
			}
			costFromStart = cost_in;
			heuristic = heuristic_in;
		}
	};
	bool moved;
public:
	std::deque<Vector2> route;
	Vector2 walkingPosition; //The renderedPosition
	Vector2 destination;
	int route_index;

	Animal();
	Animal(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in);

	bool Update();

	void Die();

	Vector2 GetRandomAdjacentPosition(Random& r, const Grid& grid, const int& range, const EntityType& type);

	ReturnType Move();
	bool TryPursue(Vector2& destination, const Vector2& direction, Grid& grid);
	ReturnType Wander(Random& r, Grid& grid);

	ReturnType FindPath(const Grid& grid, const Vector2& destination);
	bool VectorContains(const std::vector<Node>&, const Vector2& v);

	Vector2 GetPosition() override;
};

#endif