#include "../hooks.h"
#include "../../menu/config.h"
namespace n_hooks {
	void __fastcall n_functions::OverrideView(void* ecx, void* edx, CViewSetup* setup) 
	{
		static auto OriginalFn = n_hooks::ClientMode->get_original_function< decltype(&OverrideView) >(18);

		if (!Global::Local )
			return;

		if ( Controls.world_viewmodel && !Global::Local->IsScoped( ) )
			setup->fov = Controls.world_viewmodel + 90;

		if ( Controls.remove_zoom && Global::Local->IsScoped( ) )
			setup->fov = !Controls.world_viewmodel ? 90 : Controls.world_viewmodel + 90;

		if (GetAsyncKeyState(Controls.fakeduck_key)) {
			if (Controls.fakeduck && Global::Local->IsAlive())
				setup->origin.z = Global::Local->get_absolute_origin().z + 64.f;
		}

		//if ( GetAsyncKeyState( VK_MENU ) & 0x8000 )
		//	setup->fov -= 30;

		if ( Controls.remove_visual_recoil ) {
			Vector3D viewPunch = Global::Local->view_punch_angle( );
			Vector3D aimPunch = Global::Local->aim_punch_angle( );

			setup->angles[ 0 ] -= ( viewPunch[ 0 ] + ( aimPunch[ 0 ] * 2 * 0.4499999f ) );
			setup->angles[ 1 ] -= ( viewPunch[ 1 ] + ( aimPunch[ 1 ] * 2 * 0.4499999f ) );
			setup->angles[ 2 ] -= ( viewPunch[ 2 ] + ( aimPunch[ 2 ] * 2 * 0.4499999f ) );
		}
	

		OriginalFn(ecx, edx, setup);
	}
}