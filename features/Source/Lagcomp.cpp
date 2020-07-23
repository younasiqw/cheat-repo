#include "../Headers/Lagcomp.h"
#include "../Headers/Ragebot.h"
#include "../Headers/Autowall.h"
void clagcomp::draw( ) {
	//for ( auto& tick : ticks ) {
	//	for ( auto& record : tick.records ) {
	//		Vector3D screenPos;
			//if ( !Interfaces::DebugOverlay->WorldToScreen( record.head, screenPos ) )
			//	return;

			//Render::FilledRect( screenPos.x, screenPos.y, 4, 4, color( 255, 0, 0 ) );
	//	}
	//}
}

void clagcomp::end( ) {
	if ( entity )
		entity->get_absolute_origin( ) = previous_origin;

	entity = nullptr;
}

void clagcomp::begin( ) {
	entity = nullptr;

	float serverTime = Global::Local->GetTickBase( ) * Interfaces::GlobalVarsBase->IntervalPerTick;
	auto weapon = Global::Local->GetActiveWeapon( );

	if ( !weapon )
		return;

	if ( Global::uCmd->buttons & in_attack && weapon->m_flNextPrimaryAttack( ) < serverTime + 0.001 ) {
		float fov = 0.5f;
		int tickcount = 0;
		bool hasTarget = false;
		Vector3D orig;

		for ( auto& tick : ticks ) {
			for ( auto& record : tick.records ) {

				Vector3D angle = Math.CalcAngle( Global::Local->GetBonePosition( 8 ), record.hitbox );
				float tmpFOV = CalculateDist( Global::uCmd->viewangles, angle );

				if ( tmpFOV < fov ) {
					fov = tmpFOV;
					tickcount = tick.tickcount;
					hasTarget = true;
					entity = record.entity;
					orig = record.origin;
				}
			}
		}

		if ( entity && hasTarget ) {
			Global::uCmd->TickCount = tickcount;
			previous_origin = entity->get_absolute_origin( );
			entity->get_absolute_origin( ) = orig;
		}
	}
}
void clagcomp::register_tick( ) {
	ticks.insert( ticks.begin( ), backtrack_tick { Global::uCmd->TickCount } );
	auto& cur = ticks[ 0 ];

	while ( ticks.size( ) > 14 )
		ticks.pop_back( );

	for ( int i = 1; i < Interfaces::engine->get_max_clients( ); i++ ) {
		C_CSPlayer* entity = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

		if ( !entity || entity->IsDormant( ) || entity->GetHealth( ) <= 0 || entity->GetTeam( ) == Global::Local->GetTeam( ) || entity->IsImmune( ) )
			continue;

		/*int 
			damage = 0,
			best_damage = 0,
			best_hitbox_id = 0;

		for ( int i = 0; i < 19; i++ ) {
			damage = autowall.CalculateDamage( Global::Local->GetEyePosition( ), aimbot.GetHitboxPosition( entity, i ), Global::Local, entity ).damage;

			if ( damage > best_damage ) {
				best_hitbox_id = i;
				best_damage = damage;
			}
		}*/

		lag_comp_record record = lag_comp_record( entity, aimbot.hitscan( entity ) );
		PlayerRecords[ i ].push_front( lag_comp_record( record ) );

		while ( PlayerRecords[ i ].size( ) >= 14 )
			PlayerRecords[ i ].pop_back( );

		for ( int i = 0; i < 19; i++ ) {
			cur.records.emplace_back( backtrack_record { entity, aimbot.GetHitboxPosition( entity, i ), entity->get_absolute_origin( ) } );
		}
	}
}

std::array<std::deque<lag_comp_record>, 64> PlayerRecords;
clagcomp lagcomp;
//backtrack_tick backtrack[ 64 ][ 14 ];