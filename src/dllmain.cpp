#include <Windows.h>
#include <string>

#include <TeknoFfb.h>

#pragma comment(lib, "SDL2.lib")
#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>

SDL_Haptic* haptic;
int effect_left_id;
int effect_right_id;
int effect_friction_id;
int effect_vibration_id;

void info(const char* format, ...)
{
	va_list args;
	char buffer[1024];

	va_start(args, format);
	int len = _vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	buffer[len] = '\n';
	buffer[len + 1] = '\0';

	OutputDebugStringA(buffer);
}

void TriggerConstantEffect(int effectId, int direction, int strength)
{
	SDL_HapticStopEffect(haptic, effect_left_id);
	SDL_HapticStopEffect(haptic, effect_right_id);

	SDL_HapticEffect tempEffect;

	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_CONSTANT;
	tempEffect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
	tempEffect.constant.direction.dir[0] = direction;
	tempEffect.constant.length = 120;
	tempEffect.constant.delay = 0;
	tempEffect.constant.level = strength * 1320;

	SDL_HapticUpdateEffect(haptic, effectId, &tempEffect);

	SDL_HapticRunEffect(haptic, effectId, 1);

	std::string dir = "";

	if (direction == 1)
	{
		dir = "rightwards";
	}
	else
	{
		dir = "leftwards";
	}

	info("moving wheel %s with strength %i", dir.c_str(), strength);
}

void TriggerFrictionEffect(int unk1, int strength)
{
	SDL_HapticEffect tempEffect;

	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_FRICTION;
	tempEffect.condition.delay = 0;
	tempEffect.condition.length = 0;
	tempEffect.condition.left_sat[0] = 0xFFFF;
	tempEffect.condition.right_sat[0] = 0xFFFF;
	tempEffect.condition.left_coeff[0] = (int16_t)((strength / (float)256) * INT16_MAX);
	tempEffect.condition.right_coeff[0] = (int16_t)((strength / (float)256) * INT16_MAX);

	SDL_HapticUpdateEffect(haptic, effect_friction_id, &tempEffect);

	SDL_HapticRunEffect(haptic, effect_friction_id, 1);
}

void TriggerVibrationEffect(int unk1, int strength)
{
	SDL_HapticEffect tempEffect;

	SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
	tempEffect.type = SDL_HAPTIC_SINE;
	tempEffect.periodic.delay = 0;
	tempEffect.periodic.length = 0;
	tempEffect.periodic.period = 50;
	tempEffect.periodic.magnitude = (int16_t)((strength / (float)256) * INT16_MAX);

	SDL_HapticUpdateEffect(haptic, effect_vibration_id, &tempEffect);

	SDL_HapticRunEffect(haptic, effect_vibration_id, 1);
}

class FfbImpl : public TeknoFfbPluginBase
{
	virtual void Initialize(const char* guid)
	{
		SDL_Init(SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK);

		int numJoysticks = SDL_NumJoysticks();
		bool hasHaptic = false;

		info("Detected %i joysticks", numJoysticks);

		for (int i = 0; i < numJoysticks; i++)
		{
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
				
			if (SDL_JoystickIsHaptic(joystick))
			{
				hasHaptic = true;
				// check guid from somewhere?
			}
		}

		if (hasHaptic)
		{
			haptic = SDL_HapticOpen(0);

			if (haptic == NULL)
			{
				info("Haptic init failed");
				info("SDL ERROR: %s", SDL_GetError());
			}

			SDL_HapticSetAutocenter(haptic, 0);

			SDL_HapticEffect tempEffect;

			SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
			tempEffect.type = SDL_HAPTIC_CONSTANT;
			tempEffect.constant.direction.type = SDL_HAPTIC_CARTESIAN;
			tempEffect.constant.direction.dir[0] = -1;
			tempEffect.constant.length = 30;
			tempEffect.constant.delay = 0;
			tempEffect.constant.level = 9999;

			// Upload the effect
			effect_left_id = SDL_HapticNewEffect(haptic, &tempEffect);

			tempEffect.constant.direction.dir[0] = 1;

			effect_right_id = SDL_HapticNewEffect(haptic, &tempEffect);

			SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
			tempEffect.type = SDL_HAPTIC_FRICTION;
			tempEffect.condition.delay = 0;
			tempEffect.condition.length = 99999999;
			
			effect_friction_id = SDL_HapticNewEffect(haptic, &tempEffect);

			SDL_memset(&tempEffect, 0, sizeof(SDL_HapticEffect));
			tempEffect.type = SDL_HAPTIC_SINE;
			tempEffect.periodic.delay = 0;
			tempEffect.periodic.length = 0;
			tempEffect.periodic.period = 50;

			effect_vibration_id = SDL_HapticNewEffect(haptic, &tempEffect);
		}
	}
	
	virtual void SendConstant(int strength, FfbDirection direction)
	{
		if (direction == FfbDirection::Left)
		{
			TriggerConstantEffect(effect_left_id, -1, strength);
		}
		else if (direction == FfbDirection::Right)
		{
			TriggerConstantEffect(effect_right_id, 1, strength);
		}
	}

	virtual void SetFriction(int unk1, int strength)
	{
		TriggerFrictionEffect(unk1, strength);
	}

	virtual void SendVibration(int unk1, int strength)
	{
		TriggerVibrationEffect(unk1, strength);
	}
};

static FfbImpl g_ffbImpl;

extern "C" __declspec(dllexport) TeknoFfbPluginBase* CreateFfbInterface()
{
	return &g_ffbImpl;
}