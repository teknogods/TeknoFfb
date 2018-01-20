#pragma once

enum class FfbDirection {
	Left,
	Right
};

class TeknoFfbPluginBase
{
public:
	virtual void Initialize(const char* guid) = 0;

	virtual void SendConstant(int strength, FfbDirection direction) = 0;

	virtual void SetFriction(int unk1, int strength) = 0;

	virtual void SendVibration(int unk1, int strength) = 0;
};