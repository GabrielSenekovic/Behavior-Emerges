#ifndef THEEVENTLOOPOFLIFE_H
#define THEEVENTLOOPOFLIFE_H

#include "olcPixelGameEngine.h"
#include "Random.h"
#include "HUDElement.h"
#include "Grid.h"
#include "Sheep.h"
#include "Wolf.h"
#include "Grass.h"
#include "EntityManager.h"
#include "Time.h"

using namespace olc;

//These are just my standard Macros i copy pasted
#define TEA(val, comparison, ternary) (val = val == comparison ternary);
//Ternary Equivalence Assignment
//x = x==3? 1:2; turns into
//TEA(x, 3, ?1:2);
//Its only really useful for readability if x is reeeeaaaaally long
//It also means that you only have to write x once
#define DEL(val) {delete val; val = nullptr;};
//Simple and nice way to both delete and set to nullptr
//Only use when you want to do both
#define forall(i,a,b) for(size_t i=a;i<b;i++)
//Goes through all elements of a list
#define uint unsigned int
#define Vector2 vf2d
#define IntVector2 vi2d

class BehaviorEmerges : public olc::PixelGameEngine
{
	float timer = 0;

	Grid grid;

	olc::Decal* tile;
	olc::Decal* rock; //Rock is here for the A* task. I'm prepared

	HUDElement frameRate;
	HUDElement amountOfTurns;

	HUDElement amountOfGrass;
	HUDElement amountOfSheep;
	HUDElement amountOfWolves;

	EntityManager entityManager;

	Vector2 averageSheepPosition;
	float sheepRadius;
	Vector2 averageWolfPosition;

	std::array<Decal*, 10> digits;

	Random r;

public:
	BehaviorEmerges(IntVector2 dim_in);

	bool OnUserCreate()override;
	bool OnUserUpdate(float fElepsedTime)override;
	void OnUserDraw();

	void Quit();

	IntVector2 dim; //Dimensions of the screen
};

#endif