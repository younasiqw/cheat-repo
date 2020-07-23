#include "../../hooks/hooks.h"
#include "../../Controls/Controls.h"
#include "../../features/Headers/Misc.h"
#include "../../features/Headers/Resolver.h"
#include "../../features/Headers/Lagcomp.h"
#include "../../features/Headers/Skins.h"


namespace n_hooks {
	void __stdcall n_functions::FrameStageNotify( ClientFrameStage_t stage ) {
		static auto OriginalFn = n_hooks::ClientHook->get_original_function< decltype( &FrameStageNotify ) >( 37 );

		static int once = 0;
		if ( once != Controls.knife_model ) {
			if ( Interfaces::engine->IsConnected( ) && Interfaces::engine->IsInGame( ) ) {
				skin_changer.force_full_update( );
			}
			once = Controls.knife_model;
		}

		if ( stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
			skin_changer.run_skin_changer( );
		}
		
		for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {
			C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );
			if ( !player ) 
				continue;

			if ( player->Networkable( )->EntIndex( ) == Interfaces::engine->get_local_player( ) )
				continue;

			if ( stage == ClientFrameStage_t::FRAME_RENDER_START ) {
				*( int* ) ( ( uintptr_t ) player + 0xA30 ) = Interfaces::GlobalVarsBase->frame_count;
				*( int* ) ( ( uintptr_t ) player + 0xA28 ) = 0;
			}
			if ( stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
				player->ShouldInterp( false );
			}
		}

		if ( stage == ClientFrameStage_t::FRAME_RENDER_START ) {

			resolver.update_local_animation( );

			resolver.resolve( );

			misc.run_thirdperson( );
		}

		if ( OriginalFn ) {
			OriginalFn( stage );
		}
	}
}