#ifndef TIME_H
#define TIME_H

static class Time
{
public:
	static void SetTimeScale(const float& timeSpeed_in);
	static float timeSpeed;
	static float deltaTime;
};

#endif