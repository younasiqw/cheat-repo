#include "../hooks.h"
#include "../../Controls/Controls.h"
#include "../../menu/config.h"
namespace n_hooks {
	float __fastcall n_functions::GetViewModelFOV( std::uintptr_t ecx, std::uintptr_t edx ) {

		return 60.f + Controls.viewmodel;
	}
}