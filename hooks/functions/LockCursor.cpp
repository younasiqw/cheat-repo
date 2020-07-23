#include "../../hooks/hooks.h"
#include "../../Controls/Controls.h"
namespace n_hooks {
	void __fastcall n_functions::LockCursor(std::uintptr_t ecx, std::uintptr_t edx) {
		static auto OriginalFn = n_hooks::SurfaceVGUI->get_original_function< decltype(&LockCursor) >(67);

		if (n_menu::menu_opened) {
			Interfaces::Surface->unlock_cursor();
			Interfaces::input_system->enable_input(false);
			return;
		}
		else {
			Interfaces::input_system->enable_input(true);
		}

		OriginalFn(ecx, edx);
	}
}