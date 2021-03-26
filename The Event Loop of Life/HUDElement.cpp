#include "HUDElement.h"

HUDElement::HUDElement()
{
}

HUDElement::HUDElement(Word text_in, Number number_in): text(text_in), number(number_in)
{
}

void HUDElement::Render(BehaviorEmerges& game)
{
	text.Render(game);
	number.Render(game);
}
