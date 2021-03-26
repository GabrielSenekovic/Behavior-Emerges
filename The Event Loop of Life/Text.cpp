#include "Text.h"
#include "BehaviorEmerges.h"

Letter::Letter() : m_value('A')
{
}
Letter::Letter(const char c, const int x, const int y)
{
	for (unsigned int i = 0; i <= 25; i++)
	{
		uint8_t width = 8;
		Sprite* temp = new Sprite("./Assets/Letters.png", olc::vi2d(width), olc::vi2d(i*width,0));
		sprites[i] = new Decal(temp);
	}
	m_value = c;
	m_trans = { (float)x, (float)y };
	//m_trans.SetPosition((float)x, (float)y);
	Update();
}
void Letter::Update()
{
	switch (m_value)
	{
	case 'A': m_sprite = sprites[0]; break;
	case 'B': m_sprite = sprites[1]; break;
	case 'C': m_sprite = sprites[2]; break;
	case 'D': m_sprite = sprites[3]; break;
	case 'E': m_sprite = sprites[4]; break;
	case 'F': m_sprite = sprites[5]; break;
	case 'G': m_sprite = sprites[6]; break;
	case 'H': m_sprite = sprites[7]; break;
	case 'I': m_sprite = sprites[8]; break;
	case 'J': m_sprite = sprites[9]; break;
	case 'K': m_sprite = sprites[10]; break;
	case 'L': m_sprite = sprites[11]; break;
	case 'M': m_sprite = sprites[12]; break;
	case 'N': m_sprite = sprites[13]; break;
	case 'O': m_sprite = sprites[14]; break;
	case 'P': m_sprite = sprites[15]; break;
	case 'Q': m_sprite = sprites[16]; break;
	case 'R': m_sprite = sprites[17]; break;
	case 'S': m_sprite = sprites[18]; break;
	case 'T': m_sprite = sprites[19]; break;
	case 'U': m_sprite = sprites[20]; break;
	case 'V': m_sprite = sprites[21]; break;
	case 'W': m_sprite = sprites[22]; break;
	case 'X': m_sprite = sprites[23]; break;
	case 'Y': m_sprite = sprites[24]; break;
	case 'Z': m_sprite = sprites[25]; break;
	default: m_sprite = sprites[0]; break;
	}
}
void Letter::Render(BehaviorEmerges& game)
{
	game.DrawDecal(m_trans, m_sprite, { 1,1 }, olc::WHITE);
}

Word::Word()
{

}
Word::Word(const std::string word)
{
	for (int i = 0; i < word.size(); i++)
	{
		letters.push_back(Letter(word[i], 0, 0));
	}
}
Word::Word(const std::string word, const int x, const int y)
{
	for (int i = 0; i < word.size(); i++)
	{
		letters.push_back(Letter(word[i], x + (i * 8), y));
	}
}
void Word::Move(vf2d newPosition)
{
	for (int i = 0; i < letters.size(); i++)
	{
		letters[i].m_trans = { newPosition.x + (i * 8), newPosition.y };
		//letters[i].m_trans.SetPosition(newPosition.x + (i * 8), newPosition.y);
	}
}
void Word::Render(BehaviorEmerges& game)
{
	for (int i = 0; i < letters.size(); i++)
	{
		letters[i].Render(game);
	}
}
void Word::Reset()
{
	for (int i = letters.size() - 1; i >= 0; i--)
	{
		letters.pop_back();
	}
}