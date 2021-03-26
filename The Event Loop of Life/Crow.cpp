#include "Crow.h"
#include "BehaviorEmerges.h"

Crow::Crow(const Vector2& dim) :position({ (float)(rand() % (int)dim.x + 1),(float)(rand() % (int)dim.y + 1) }), velocity({ 0,0 })
{
}

void Crow::Move(float fElapsedTime, const Vector2& dim)
{
	position += velocity * fElapsedTime * 0.01f;
	if (position.y > dim.y + 1) //Sprite Height is 1
	{
		position.y = 0 - 1;
	}
	else if (position.y < 0 - 1)
	{
		position.y = dim.y + 1;
	}
	if (position.x > dim.x + 1)
	{
		position.x = 0 - 1;
	}
	else if (position.x < 0 - 1)
	{
		position.x = dim.x + 1;
	}
}

void Crow::Render(BehaviorEmerges& game, Decal* sprite, Vector2& renderPosition)
{
	game.DrawRotatedDecal(renderPosition, sprite, angle, { (float)sprite->sprite->width / 2,(float)sprite->sprite->height / 2 }, { 1,1 }, olc::WHITE);
}
