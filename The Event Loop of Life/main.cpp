#include <Windows.h>
#include "BehaviorEmerges.h"

int main()
{
	HWND hd = GetDesktopWindow();
	RECT rect;
	GetClientRect(hd, &rect);
	int client_width = (rect.right - rect.left);
	int client_height = (rect.bottom - rect.top);

	BehaviorEmerges program = (IntVector2(client_width, client_height));
	if (program.Construct((int32_t)program.dim.x, (int32_t)program.dim.y, 2, 2, true))
	{
		program.Start();
	}
	program.Quit();
	return 0;
}