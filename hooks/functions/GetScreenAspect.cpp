#include "../hooks.h"
#include "../../menu/config.h"//
namespace n_hooks {
	float __fastcall n_functions::Hook_GetScreenAspectRatio(void* pecx, void* pedx, int32_t width, int32_t height)
	{
		static int sw, sh;
		Interfaces::engine->get_screen_size(sw, sh);

		float ratio = (Controls.aspect_ratio * 0.1) / 2;
		if (Controls.aspect_ratio > 1)
			return ratio;
		else
			return (float)sw / (float)sh;
	}
}