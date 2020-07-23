#include "../Headers/Engine Prediction.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/server/player_command.cpp

namespace n_engine_prediction {
	float old_current_time;
	float old_frame_time;
	void* move_data;
	int* prediction_random_seed;
	int* prediction_player;

	void initialize( ) {

		if ( !Global::Local )
			return;

		if ( !Global::Local->IsAlive( ) || !Global::uCmd )
			return;

		if ( !move_data )
			move_data = std::malloc( 182 );

		old_current_time = Interfaces::GlobalVarsBase->CurrentTime;
		old_frame_time = Interfaces::GlobalVarsBase->frame_time;

		Interfaces::GlobalVarsBase->CurrentTime = Global::Local->GetTickBase( ) * Interfaces::GlobalVarsBase->IntervalPerTick;
		Interfaces::GlobalVarsBase->frame_time = *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Interfaces::prediction ) + 0x0A ) ? 0 : Interfaces::GlobalVarsBase->IntervalPerTick;

		if ( !prediction_random_seed || !prediction_player ) {
			prediction_random_seed = *reinterpret_cast< int** >( Utilities::FindPattern( "client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86" ) + 0x1 );
			prediction_player = *reinterpret_cast< int** >( Utilities::FindPattern( "client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48" ) + 0x2 );
		}

		// CPlayerMove::StartCommand
		{
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x3334 ) = reinterpret_cast < std::uint32_t >( Global::uCmd );

			*prediction_random_seed = Global::uCmd ? Global::uCmd->random_seed : -1;
			*prediction_player = reinterpret_cast< int >( Global::Local );
		}

		Interfaces::move_helper->set_host( Global::Local );

		Interfaces::game_movement->start_track_prediction_errors( Global::Local ); // @BigMoneyBoss - (*(void (__thiscall **)(int *, _DWORD *))(game_movement + 12))(&game_movement, Global::Local);


		//C_BaseCombatWeapon* weapon = reinterpret_cast< C_BaseCombatWeapon* >( Interfaces::EntityList->GetClientEntity( Global::uCmd->weapon_select ) );
		//if ( weapon) {
		//	CCSWeaponData* WeaponData = weapon->GetWeaponData( );

		//	if ( WeaponData )
		//		Global::Local->select_item( WeaponData->weapon_name, Global::uCmd->weapon_subtype );
		//}

		//C_CSPlayer* vehicle = nullptr;

		//if ( Global::Local->get_h_vehicle( ).is_valid( ) )
		//	vehicle = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->get_client_entity_from_handle( Global::Local->get_h_vehicle( ) ) );

		//if ( Global::uCmd->impulse && ( !vehicle || Global::Local->using_standard_weapons_in_vechile( ) ) )
			//*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x31FC ) = Global::uCmd->impulse;

		// CBasePlayer::UpdateButtonState
		{
			Global::uCmd->buttons |= *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x3330 );

			int m_nButtons = Global::uCmd->buttons;
			int* m_afButtonLast = reinterpret_cast< int* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x31F8 );
			int buttonsChanged = m_nButtons ^ *m_afButtonLast;
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x31EC ) = *m_afButtonLast;
			*m_afButtonLast = m_nButtons;
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x31F0 ) = m_nButtons & buttonsChanged;
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x31F4 ) = buttonsChanged & ~m_nButtons;
		}

		Interfaces::prediction->check_moving_ground( Global::Local, Interfaces::GlobalVarsBase->frame_time );

		Global::Local->set_local_view_angles( Global::uCmd->viewangles );

		// CPlayerMove::RunPreThink
		{
			if ( Global::Local->physics_run_think( 0 ) )
				Global::Local->pre_think( );
		}

		// CPlayerMove::RunThink
		{
			int* thinktick = reinterpret_cast< int* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x0FC );
			if ( *thinktick != -1 && *thinktick > 0 && *thinktick <= Global::Local->GetTickBase( ) && thinktick) {
				*thinktick = -1;

				static auto unknown_fn = reinterpret_cast< void( __thiscall* )( int ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) );
				if (unknown_fn)
					unknown_fn( 0 );

				Global::Local->think( );
			}
		}

		Interfaces::prediction->setup_move( Global::Local, Global::uCmd, Interfaces::move_helper, reinterpret_cast< CMoveData* >( move_data ) );

		//if ( vehicle )
		//	Utilities::get_virtual_function< void( __thiscall* )( int, C_CSPlayer*, void* ) >( vehicle, 5 )( reinterpret_cast< std::uint32_t >( vehicle ), Global::Local, move_data );
		//else
			Interfaces::game_movement->process_movement( Global::Local, reinterpret_cast< CMoveData* >( move_data ) );

		Interfaces::prediction->finish_move( Global::Local, Global::uCmd, reinterpret_cast< CMoveData* >( move_data ) );

		Interfaces::move_helper->process_impacts( );

		// CPlayerMove::RunPostThink
		{
			auto post_think = [ ]( ) -> int {
				Utilities::get_virtual_function< void( __thiscall* )( void* ) >( Interfaces::model_cache, 33 )( Interfaces::model_cache );

				if ( Global::Local->IsAlive( ) /* index 155 for vfunc */ || *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x3A81 ) ) {
					Utilities::get_virtual_function< void( __thiscall* )( void* ) >( Global::Local, 339 )( Global::Local );

					if ( Global::Local->GetFlags( ) & 1 ) // FL_ONGROUND
						*reinterpret_cast< std::uintptr_t* >( std::uintptr_t( Global::Local ) + 0x3014 ) = 0;

					if ( *reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x28BC ) == -1 )
						Utilities::get_virtual_function< void( __thiscall* )( void*, int ) >( Global::Local, 218 )( Global::Local, 0 );

					Utilities::get_virtual_function< void( __thiscall* )( void* ) >( Global::Local, 219 )( Global::Local );

					static auto post_think_v_physics = reinterpret_cast< bool( __thiscall* )( C_CSPlayer* ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) );
					post_think_v_physics( Global::Local );
				}

				static auto simulate_player_simulated_entities = reinterpret_cast< bool( __thiscall* )( C_CSPlayer* ) >( Utilities::FindPattern( "client_panorama.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72" ) );
				simulate_player_simulated_entities( Global::Local );

				return Utilities::get_virtual_function< int( __thiscall* )( void* ) >( Interfaces::model_cache, 34 )( Interfaces::model_cache );
			};

			post_think( );
		}
	}

	void unload( ) {
		if ( !Global::Local->IsAlive( ) )
			return;

		Interfaces::game_movement->finish_track_prediction_errors( Global::Local );

		Interfaces::move_helper->set_host( 0 );

		Interfaces::game_movement->reset( );

		// CPlayerMove::FinishCommand
		{
			*reinterpret_cast< std::uint32_t* >( reinterpret_cast< std::uint32_t >( Global::Local ) + 0x3334 ) = 0;

			*prediction_random_seed = -1;
			*prediction_player = 0;
		}

		if ( Interfaces::GlobalVarsBase->frame_time > 0.f )
			Global::Local->GetTickBase( )++;

		Interfaces::GlobalVarsBase->CurrentTime = old_current_time;
		Interfaces::GlobalVarsBase->frame_time = old_frame_time;
	}
}