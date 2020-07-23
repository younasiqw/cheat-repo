#include "../Headers/Anti-aim.h"

cantiaim antiaim;
float freal = 0.f;

float corrected_tickbase()
{
	c_usercmd* last_ucmd = nullptr;
	int corrected_tickbase = 0;

	corrected_tickbase = ( !last_ucmd ) ? ( float ) Global::Local->GetTickBase( ) : corrected_tickbase++;
	last_ucmd = Global::uCmd;
	float corrected_curtime = corrected_tickbase * Interfaces::GlobalVarsBase->IntervalPerTick;
	return corrected_curtime;

}

bool cantiaim::predict_lby_update(bool& send_packet )
{
	static float next_lby_update_time = 0;

	if ( !( Global::Local->GetFlags( ) & FL_ONGROUND ) )
		return false;

	if ( Global::Local->get_velocity( ).Length( ) > 0.1f ) {
		next_lby_update_time = corrected_tickbase( ) + 0.22f;
	}
	else if ( next_lby_update_time - corrected_tickbase(  ) <= 0.0f ) {
		next_lby_update_time = corrected_tickbase( ) + 1.1f;
		send_packet = false;
	}
	else if ( next_lby_update_time - corrected_tickbase( ) <= 1 * Interfaces::GlobalVarsBase->IntervalPerTick ) {
		send_packet = true;
	}

	return false;
}
void cantiaim::on_move( ) {
	if ( !Global::Local->IsAlive( ) )
		return;

	if ( ( Global::uCmd->buttons & in_attack ) )
		return;

	if ( Global::Local->GetMoveType( ) == MoveType_t::MOVETYPE_LADDER )
		return;

	if ( Global::uCmd->buttons & in_use )
		return;

	if ( Global::Local->WeaponNade( ) )
		return;

	Vector3D eye;
	Interfaces::engine->get_view_angles( eye );

	if ( Controls.antiaim ) {
		run_pitch( );
		run_yaw( );
	}
	else {
		Global::real_angle = eye;
	}
}

void cantiaim::run_pitch( ) {
	switch ( Controls.pitch ) {
	case 1: Global::uCmd->viewangles.x = 89;  break;
	case 2: Global::uCmd->viewangles.x = -89; break;
	}
}

void cantiaim::run_yaw( ) {
	switch ( Controls.yaw ) {
	case 1: {

		/*if ( ( Global::Local->GetFlags( ) & FL_ONGROUND ) && fabsf( Global::uCmd->sidemove ) < 5.0f ) {

			float SwitchAmount = 0.f;

			if ( Global::Local->GetFlags( ) & in_duck )
				SwitchAmount = 3.25f;
			else
				SwitchAmount = 1.1f;

			Global::uCmd->sidemove = Global::TickCount & 1 ? SwitchAmount : -SwitchAmount;
		}

		Global::send_packet = !Global::send_packet; */

		int invert = GetKeyState( VK_XBUTTON2 ) ? -29.f : 29.f;

		if ( Global::send_packet ) {
			Global::uCmd->viewangles.y += 180.f - invert;
			Global::fake_angle = Global::uCmd->viewangles;
		}
		else {
			Global::uCmd->viewangles.y += 180.f + invert;
			Global::real_angle = Global::uCmd->viewangles;
		}

		break; //desync 
	}
	case 2: run_freestanding( ); break; //auto directon
	}
}

void cantiaim::should_fake_lag_on_peek( ) {
	auto weapon = Global::Local->GetActiveWeapon( );
	if ( weapon && weapon->ItemDefinitionIndex( ) == (int)ItemDefinitionIndex::WEAPON_REVOLVER )
		return;

	if ( Global::Local->get_velocity( ).Length( ) < 3.f )
		return;

	auto net_channel = Interfaces::engine->get_net_channel( );

	static bool Hitinit = false;
	bool SkipTick = false;
	static bool canHit = false;
	bool HitCheck = false;
	static bool LagPeek = false;

	static float LagTime = ticks_to_time( 14.f );

	if ( Controls.box ) {
		Vector3D Head = Vector3D( Global::Local->GetVecOrigin( ).x, Global::Local->GetVecOrigin( ).y, 0 );
		Vector3D HeadExtr = ( Head + ( Global::Local->get_velocity( ) * LagTime ) );
		Vector3D OriginExtr = ( ( Global::Local->GetVecOrigin( ) + ( Global::Local->get_velocity( ) * LagTime ) ) + Vector3D( 0, 0, 8.f ) );

		for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {
			C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

			if ( !player || !player->is_player( ) || !player->IsAlive( ) || player->IsDormant( ) || Global::Local->GetTeam( ) == player->GetTeam( ) )
				continue;

			Vector3D EnemyHead = Vector3D( player->GetVecOrigin( ).x, player->GetVecOrigin( ).y, 0 );

			if ( autowall.CanHitFloatingPoint( HeadExtr, EnemyHead ) || autowall.CanHitFloatingPoint( OriginExtr, EnemyHead ) ) {
				if ( !Hitinit ) {
					canHit = true;
					Hitinit = true;
				}
				HitCheck = true;
			}
		}

		if ( !HitCheck )
			Hitinit = false;

		if ( canHit ) {
			Global::send_packet = true;
			SkipTick = true;
			LagPeek = true;
			canHit = false;
		}

		if ( LagPeek && !SkipTick ) {
			if ( net_channel->m_nChokedPackets < 14 )
				Global::send_packet = false;
			else
				LagPeek = false;
		}

		if ( !LagPeek && !SkipTick )
			Global::send_packet = ( net_channel->m_nChokedPackets >= Controls.fakelag );
		
	}
}

void cantiaim::double_tap(  ) {
	auto net_channel = Interfaces::engine->get_net_channel( );
	static bool counter = false;
	static int counters = 0;

	if ( GetAsyncKeyState( 'I' ) ) {
		if ( counters == 2 ) {
			counters = 0;
			counter = !counter;
		}

		counters++;
		if ( counter ) {
			Global::uCmd->buttons &= ~in_attack;
			Global::tick_base_shift = 100;
		}
		else {
			if ( !net_channel->m_nChokedPackets ) {
				Global::uCmd->buttons |= in_attack;
				Global::tick_base_shift = 0;
			}
		}

	}
}