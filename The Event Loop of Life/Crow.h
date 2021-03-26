#ifndef CROW_H
#define CROW_H

#include "olcPixelGameEngine.h"
using namespace olc;

#define Vector2 vf2d

class BehaviorEmerges;

class Crow
{
public:
	Vector2 position;
	Vector2 velocity;
	float cohesionRadius = 8;
	float alignmentRadius = cohesionRadius / 1.5f;
	float separationRadius = alignmentRadius / 2;
	float angle = 0;

	bool cohesion = true;
	bool alignment = true;
	bool separation = true;

	Crow(const Vector2& dim);

	void Move(float fElapsedTime, const Vector2& dim);

	void Render(BehaviorEmerges& game, Decal* sprite, Vector2& renderPosition);
};

#endif
