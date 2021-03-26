#include "Numbers.h"
#include "BehaviorEmerges.h"

Digit::Digit(): digits(nullptr)
{
}

Digit::Digit(const int value, const int x, const int y, std::array<Decal*, 10>& digits_in): digits(&digits_in)
{
	m_value = value;
	m_trans = vf2d((float)x, (float)y);
	m_sprite = (*digits)[m_value];
}
void Digit::Update()
{
	m_sprite = (*digits)[m_value];
}
void Digit::Render(BehaviorEmerges& game)
{
	game.DrawDecal(m_trans, m_sprite, { 1,1 }, olc::WHITE);
}
void Digit::Increment()
{
	m_value++;
	m_value %= 10;
	Update();
}
void Digit::Decrement()
{
	m_value--;
	if (m_value < 0)
	{
		m_value = 9;
	}
	Update();
}
void Digit::Add(const int value)
{
	m_value += value;
	m_value %= 10;
	Update();
}

Number::Number(): m_value(0){}

Number::Number(const int x, const int y, const float scale, const int integers, std::array<Decal*, 10>& digits_in) :
	m_value(0)
{
	for (int i = 0; i < integers; i++)
	{
		digits.push_back(Digit(0, x + (8 * i) * (int)scale + 48, y, digits_in));
		//digits[i].m_trans.SetScale(scale, scale);
	}
}
Number::Number(const int value, const int x, const int y, const float scale, const int integers, std::array<Decal*, 10>& digits_in) :
	m_value(0)
{
	for (int i = 0; i < integers; i++)
	{
		digits.push_back(Digit(0, x + (8 * i) * (int)scale, y, digits_in)); //Where does 48 come from? scale + 48
		//digits[i].m_trans.SetScale(scale, scale);
	}
	Add(value);
}
Number::~Number()
{

}
void Number::Increment()
{
	OnIncrement(digits.size() - 1);
	m_value++;
}
void Number::OnIncrement(const int i)
{
	if (digits[i].m_value == 9 && i != 0)
	{
		OnIncrement(i - 1);
	}
	digits[i].Increment();
}
void Number::Add(const int value)
{
	if (digits[digits.size() - 1].m_value + value > 9 && digits.size() - 1 != 0)
	{
		OnAdd(value, 10, value + digits[digits.size() - 1].m_value, 99, digits.size() - 2);
		digits[digits.size() - 1].Add(value % 10);
	}
	else
	{
		digits[digits.size() - 1].Add(value);
	}
	m_value += value;
}
void Number::OnAdd(const int value, const int modifier, const int totalValue, const int limit, const int i)
{
	if (digits[i].m_value * modifier + totalValue > limit
		&&
		i != 0)
	{
		OnAdd(value, modifier * 10, totalValue + digits[i].m_value * modifier, limit * 10 + 9, i - 1);
	}
	digits[i].Add((totalValue / modifier) % 10);
}
void Number::Decrement()
{

}
void Number::Subtract(int value)
{

}
void Number::Multiply(int value)
{

}
void Number::Divide(int value)
{

}
void Number::Exponentiate(int value)
{

}
void Number::Reset()
{
	for (unsigned int i = 0; i < digits.size(); i++)
	{
		digits[i].m_value = 0; digits[i].Update();
	}
	m_value = 0;
}
void Number::Render(BehaviorEmerges& game)
{
	for (unsigned int i = 0; i < digits.size(); i++)
	{
		digits[i].Render(game);
	}
}