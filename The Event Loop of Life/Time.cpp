#include "Time.h"

float Time::deltaTime = 0;
float Time::timeSpeed = 0;

void Time::SetTimeScale(const float& timeSpeed_in)
{
	deltaTime = 0;
	timeSpeed = timeSpeed_in;
}
