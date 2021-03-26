#ifndef NUMBERS_H
#define NUMBERS_H

#include "olcPixelGameEngine.h"
using namespace olc;

//Code taken from the Endless Runner I made in the first year and modified
class BehaviorEmerges;

struct Digit
{
	int m_value;

	olc::vf2d m_trans; //Transform
	Decal* m_sprite;

	Digit();
	Digit(const int value, const int x, const int y, std::array<Decal*, 10>& digits_in);
	void Update();
	void Render(BehaviorEmerges& game);
	void Increment();
	void Decrement();
	void Add(const int value);

	std::array<Decal*, 10>* digits;
};
struct Number
{
	std::vector<Digit> digits;
	int m_value;

	Number();
	Number(const int x, const int y, const float scale, const int integers, std::array<Decal*, 10>& digits);
	Number(const int value, const int x, const int y, const float scale, const int integers, std::array<Decal*, 10>& digits);
	~Number();

	void Increment(); void OnIncrement(int i);
	void Add(const int value); void OnAdd(const int value, const int modifier, const int totalValue, const int limit, const int i);

	void Decrement();
	void Subtract(int value);
	void Multiply(int value);
	void Divide(int value);
	void Exponentiate(int value);
	void Reset();
	void Render(BehaviorEmerges& game);
};

#endif