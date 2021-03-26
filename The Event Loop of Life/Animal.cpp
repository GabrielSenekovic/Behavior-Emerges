#include "Animal.h"
#include "Grid.h"

Animal::Animal()
{
}

Animal::Animal(const Vector2& position_in, const Vector2& health_in, const std::vector<Decal*>& sprites_in):
	Entity(position_in, health_in, sprites_in), 
	walkingPosition(position_in), destination(Vector2(-1, -1)),
	moved(false), route_index(0)
{
}

bool Animal::Update()
{
	health.x -= 1 / (Time::timeSpeed / Time::deltaTime);
	if (health.x <= 0) { Die(); }
	return dead;
}

Vector2 Animal::GetRandomAdjacentPosition(Random& r, const Grid& grid, const int& range, const EntityType& type)
{
	int32_t index = position.x + grid.grid.x * position.y;
	int* constraints = grid.GetValidConstraints(index, range);
	std::vector<Vector2> possiblePositions = {};
	for (int y = constraints[3]; y < constraints[5]; y++)
	{
		for (int x = constraints[2]; x < constraints[4]; x++)
		{
			if (type == EntityType::SHEEP || type == EntityType::WOLF)
			{
				if (position != Vector2(x, y) && (grid.tileContent[x + grid.grid.x * y][0] == nullptr || grid.tileContent[x + grid.grid.x * y][1] == nullptr))
				{
					possiblePositions.push_back({ (float)x, (float)y });
				}
			}
			else
			{
				if (position != Vector2(x, y) && (grid.tileContent[x + grid.grid.x * y][0] == nullptr || grid.tileContent[x + grid.grid.x * y][1] == nullptr) && grid.tileContent[x + grid.grid.x * y][2] == nullptr)
				{
					possiblePositions.push_back({ (float)x, (float)y });
				}
			}
		}
	}
	if (possiblePositions.size() == 0) { return { -1,-1 }; }
	return possiblePositions[r.myRand() % possiblePositions.size()];
}

ReturnType Animal::Move()
{
	Vector2 nextStep = Vector2(-1, -1);
	if (route.size() > 0)
	{
		nextStep = route[route_index];
	}
	else
	{
		nextStep = destination;
	}
	if (nextStep == Vector2(-1, -1))
	{
		return ReturnType::R_ERROR;
	}
	Vector2 movementVector = (nextStep - position) / (Time::timeSpeed / Time::deltaTime) * 2;
	walkingPosition = //Check that the distance to the walkingPosition isn't bigger than the distance to the destination
		std::abs(((walkingPosition + movementVector) - position).x) >= std::abs((nextStep - position).x) &&
		std::abs(((walkingPosition + movementVector) - position).y) >= std::abs((nextStep - position).y) ?
		nextStep : walkingPosition + movementVector;
	if (walkingPosition == nextStep)
	{
		position = nextStep;
		nextStep = { -1,-1 };
		route_index++;
		moved = true;
		if (route.size() == 0)
		{
			destination = {-1,-1};
		}
	}
	if (route.size() > 0 && route_index >= route.size())
	{
		route.clear(); route_index = 0; destination = { -1,-1 };
		return ReturnType::R_SUCCESS;
	}
	return ReturnType::R_RUNNING;
}

bool Animal::TryPursue(Vector2& destination, const Vector2& direction, Grid& grid)
{
	for (int i = 2; i > 0; i--)								//i signifies amount of steps forward
	{
		Vector2 temp = direction * i;						//multiply the direction by the amount of steps
		temp += position;									//add it to the position
		grid.ClampPositionToGrid(temp);
		int target_i = temp.x + grid.grid.x * temp.y;
		if (grid.TryOccupyNewPosition(*this, target_i))
		{
			destination = temp;
			return true;
		}
	}
	return false;
}

ReturnType Animal::Wander(Random& r, Grid& grid)
{
	if (destination == Vector2{ -1, -1 })
	{
		destination = Entity::GetRandomAdjacentPosition(r, grid, entityType);
		if (destination == Vector2{ -1,-1 }) { return ReturnType::R_FAILURE; }
		else
		{
			grid.tileContent[position.x + grid.grid.x * position.y][spaceOccupying] = nullptr;
			if (grid.tileContent[destination.x + grid.grid.x * destination.y][0] == nullptr) { grid.tileContent[destination.x + grid.grid.x * destination.y][0] = this; spaceOccupying = 0; }
			else { grid.tileContent[destination.x + grid.grid.x * destination.y][1] = this; spaceOccupying = 1; }
		}
	}
	return Move();
}

Vector2 Animal::GetPosition()
{
	return walkingPosition;
}

ReturnType Animal::FindPath(const Grid& grid, const Vector2& destination)
{
	if (destination == Vector2(-1, -1)) { return ReturnType::R_ERROR; }; //If you have an invalid destination
	if (route.size() > 0) { return ReturnType::R_SUCCESS; }; //If you already have a route

	route_index = 0;
	std::vector<Node> openList;
	std::vector<Node> closedList;
	Node currentNode = Node(position, 0, (position - Vector2(destination.x, destination.y)).mag(), nullptr);
	openList.push_back(currentNode);
	while (openList.size() != 0)
	{
		int index = 0;
		currentNode = openList[0];
		for (int i = 0; i < openList.size(); i++)
		{
			if (currentNode.points() > openList[i].points())
			{
				index = i;
				currentNode = openList[i];
			}
		}
		if (currentNode.position == destination)
		{
			std::deque<Vector2> path;
			while (currentNode.previousPosition != nullptr)
			{
				path.push_front(currentNode.position);
				currentNode = *currentNode.previousPosition;
			}
			route = path;
			return ReturnType::R_SUCCESS;
		}
		else
		{
			bool push = false;
			openList.erase(openList.begin() + index);
			closedList.push_back(currentNode);
			int* constraints = grid.GetValidConstraints(currentNode.position.x + grid.grid.x * currentNode.position.y);
			for (int y = constraints[3]; y < constraints[5]; y++)
			{
				for (int x = constraints[2]; x < constraints[4]; x++)
				{
					if (VectorContains(openList, Vector2(x, y)) &&
						VectorContains(closedList, Vector2(x, y)) &&
						grid.tileTraversibility[x + grid.grid.x * y] != 2)
					{
						openList.push_back(Node(Vector2(x, y), currentNode.costFromStart + 1, (destination - Vector2(x, y)).mag(), &currentNode));
					}
				}
			}
			delete constraints;
		}
	}
	return ReturnType::R_FAILURE;
}
bool Animal::VectorContains(const std::vector<Node>& l, const Vector2& v)
{
	for (size_t i = 0; i < l.size(); i++)
	{
		if (l[i].position == v)
		{
			return false; //if it at any point is in the list then dont push
		}
	}
	return true;
}

void Animal::Die()
{
	if (destination != Vector2(-1, -1))
	{
		position = destination;
	}
	dead = true;
}