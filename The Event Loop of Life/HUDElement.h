#ifndef HUDELEMENT_H
#define HUDELEMENT_H

#include "Numbers.h"
#include "Text.h"

struct HUDElement
{
	Number number;
	Word text;

	HUDElement();
	HUDElement(Word text_in, Number number_in);

	void Render(BehaviorEmerges& game);
};

#endif