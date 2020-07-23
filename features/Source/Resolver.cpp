#include "../Headers/Resolver.h"
#include <iostream>
#include "../../menu/config.h"
void cresolver::resolve( ) {
	if ( !Global::Local )
		return;

	for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {

		C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

		if ( !player || !player->IsAlive( ) || player == Global::Local || player->IsDormant( ) )
			continue;

		if ( player->GetTeam( ) == Global::Local->GetTeam( ) )
			continue;

		/* run onshot calculation */
		static float last_shot_time[ 64 ];

		/*C_BaseCombatWeapon* enemy_weapon = player->GetActiveWeapon( );
		if ( enemy_weapon ) {
			if ( last_shot_time[ player->Networkable( )->EntIndex( ) ] < enemy_weapon->m_fLastShotTime( ) ) {
				Global::fired_shot[ player->Networkable( )->EntIndex( ) ] = true;
				last_shot_time[ player->Networkable( )->EntIndex( ) ] = enemy_weapon->m_fLastShotTime( ) + 2;
				std::cout << last_shot_time[ player->Networkable( )->EntIndex( ) ] << enemy_weapon->m_fLastShotTime( ) << std::endl;
			}
			else {
				Global::fired_shot[ player->Networkable( )->EntIndex( ) ] = false;
			}
		}
		else {
			Global::fired_shot[ player->Networkable( )->EntIndex( ) ] = false;
			last_shot_time[ player->Networkable( )->EntIndex( ) ] = 0.f;
		}*/

		canimstate* state = player->AnimState( );
		if ( !state )
			continue;

		info.use_freestand_angle[ i ] = false;
		Global::resolve_mode[ player->Networkable( )->EntIndex( ) ] = "";

		if ( Controls.resolver_active ) {
			bruteforce_resolve( player );
		}
	}
}

void cresolver::anti_freestanding( C_CSPlayer* player ) {
	bool
		ran_autowall = false,
		can_hit_right = false,
		can_hit_left = false;

	if ( !Global::Local )
		return;

	float ang_to_local = Math.CalcAngle( Global::Local->GetVecOrigin( ), player->GetVecOrigin( ) ).y;
	Vector3D view_point = Global::Local->GetVecOrigin( ) + Vector3D( 0, 0, 90 );
	Vector3D origin = player->GetVecOrigin( );

	vector2d_t origin_left_right[ ] = {
		vector2d_t( ( 45 * sin( Math.GRD_TO_BOG( ang_to_local ) ) ),( 45 * cos( Math.GRD_TO_BOG( ang_to_local ) ) ) ),
		vector2d_t( ( 45 * sin( Math.GRD_TO_BOG( ang_to_local + 180 ) ) ) ,( 45 * cos( Math.GRD_TO_BOG( ang_to_local + 180 ) ) ) )
	};

	vector2d_t origin_left_right_local[ ] = {
		vector2d_t( ( 50 * sin( Math.GRD_TO_BOG( ang_to_local ) ) ),( 50 * cos( Math.GRD_TO_BOG( ang_to_local ) ) ) ),
		vector2d_t( ( 50 * sin( Math.GRD_TO_BOG( ang_to_local + 180 ) ) ) ,( 50 * cos( Math.GRD_TO_BOG( ang_to_local + 180 ) ) ) )
	};

	for ( int side = 0; side < 2; side++ ) {
		Vector3D origin_autowall = {
			origin.x + origin_left_right[ side ].x,
			origin.y - origin_left_right[ side ].y ,
			origin.z + 80
		};
		Vector3D origin_autowall_local = {
			view_point.x + origin_left_right_local[ side ].x,
			view_point.y - origin_left_right_local[ side ].y ,

			view_point.z
		};

		if ( autowall.CanHitFloatingPoint( origin_autowall, view_point ) ) {
			if ( side == 0 ) {
				can_hit_right = true;
				info.freestand_angle[ player->Networkable( )->EntIndex( ) ] = 58;
			}
			else if ( side == 1 ) {
				can_hit_left = true;
				info.freestand_angle[ player->Networkable( )->EntIndex( ) ] = -58;
			}
			ran_autowall = true;
		}
		else {
			for ( int bside = 0; bside < 2; bside++ ) {
				Vector3D origin_autowall_sides = {
					origin.x + origin_left_right[ bside ].x,
					origin.y - origin_left_right[ bside ].y ,
					origin.z + 80
				};

				if ( autowall.CanHitFloatingPoint( origin_autowall_sides, origin_autowall_local ) ) {
					if ( bside == 0 ) {
						if ( bside == 0 ) {
							can_hit_right = true;
							info.freestand_angle[ player->Networkable( )->EntIndex( ) ] = 58;
						}
						else if ( bside == 1 ) {
							can_hit_left = true;
							info.freestand_angle[ player->Networkable( )->EntIndex( ) ] = -58;
						}
						ran_autowall = true;
					}
				}
			}
		}
		if ( ran_autowall ) {
			if ( can_hit_right && can_hit_left ) {
				info.use_freestand_angle[ player->Networkable( )->EntIndex( ) ] = false;
			}
			else {
				info.use_freestand_angle[ player->Networkable( )->EntIndex( ) ] = true;
				info.last_freestanding_angle[ player->Networkable( )->EntIndex( ) ] = info.freestand_angle[ player->Networkable( )->EntIndex( ) ];
			}
		}
	}
	if ( info.use_freestand_angle[ player->Networkable( )->EntIndex( ) ] )
		info.last_freestanding_angle[ player->Networkable( )->EntIndex( ) ] += Math.normalize_yaw( ang_to_local );
}
void cresolver::bruteforce_resolve( C_CSPlayer* player ) {
	auto state = player->AnimState( );
	if ( !state )
		return;

	auto& resolver_backup_info = info.resolver_data[ player->Networkable( )->EntIndex( ) ];

	/* normalize velocity, will go further later */
	Vector3D velocity = player->get_velocity( );
	float spd = velocity.length_sqr( );
	if ( spd > std::powf( 1.2f * 260.0f, 2.f ) ) {
		Vector3D velocity_normalized = velocity.r_normalize( );
		velocity = velocity_normalized * ( 1.2f * 260.0f );
	}

	float duck_amnt_anim = std::clamp( player->duck_amount( ) + state->m_landing_duck_additive, 0.0f, 1.0f );
	float max_eye = resolver_backup_info * 6.0f;
	float real_duck;

	// clamp angles
	if ( ( duck_amnt_anim - state->m_duck_amount ) <= max_eye ) {
		if ( -max_eye <= ( duck_amnt_anim - state->m_duck_amount ) )
			real_duck = duck_amnt_anim;
		else
			real_duck = state->m_duck_amount - max_eye;
	}
	else {
		real_duck = state->m_duck_amount + max_eye;
	}

	/* return velocity smoothed to a usable number */
	static auto get_smoothed_velocity = [ ] ( float min_delta, Vector3D a, Vector3D b ) {
		Vector3D delta = a - b;
		float delta_length = delta.Length( );

		if ( delta_length <= min_delta ) {
			Vector3D result;
			if ( -min_delta <= delta_length ) {
				return a;
			}
			else {
				float iradius = 1.0f / ( delta_length + FLT_EPSILON );
				return b - ( ( delta * iradius ) * min_delta );
			}
		}
		else {
			float iradius = 1.0f / ( delta_length + FLT_EPSILON );
			return b + ( ( delta * iradius ) * min_delta );
		}
	};

	float duck_amnt = std::clamp( real_duck, 0.0f, 1.0f );

	/* minimum delta between pre-smoothed and new, smoothed velocity, as to limit errors*/
	Vector3D animation_velocity = get_smoothed_velocity( info.choked_time * 2000.0f, velocity, state->velocity );

	/* max run speed 250, bhop around 315, average*/
	float speed = std::fminf( animation_velocity.Length( ), 260.0f );

	/* GET WEAPONS NIGGA */
	C_BaseCombatWeapon* weapon = Global::Local->GetActiveWeapon( );
	if ( !weapon ) return;

	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData ) return;

	float max_movement_speed = 260.0f;
	if ( weapon ) {
		/* a kind of clamp, prevents outliers*/
		max_movement_speed = std::fmaxf( WeaponData->max_speed, 0.001f );
	}

	/* CLASSIC MAGIC NUMBERS*/
	float running_speed = speed / ( max_movement_speed * 0.520f );
	float ducking_speed = speed / ( max_movement_speed * 0.340f );

	running_speed = std::clamp( running_speed, 0.0f, 1.0f );

	/* Unknowncheats magic code */
	float yaw_modifier = ( ( ( state->m_ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;
	if ( duck_amnt > 0.0f ) {
		float ducking_speed = std::clamp( ducking_speed, 0.0f, 1.0f );
		yaw_modifier += ( duck_amnt * ducking_speed ) * ( 0.5f - yaw_modifier );
	}

	/* max right and max left yaw*/
	float min_body_yaw = std::fabsf( state->m_min_yaw * yaw_modifier );
	float max_body_yaw = std::fabsf( state->m_max_yaw * yaw_modifier );

	/* gets delta of real yaw and our supposed "resolved" yaw */
	float eye_yaw = player->GetEyeAngles( ).y;
	float eye_yaw_diff = std::remainderf( eye_yaw - info.fake_goal_feet_yaw, 360.f );

	/* Unknowncheats magic code, also in SetupVelocity */
	if ( eye_yaw_diff <= max_body_yaw ) {
		if ( min_body_yaw > eye_yaw_diff )
			info.fake_goal_feet_yaw = fabs( min_body_yaw ) + eye_yaw;
	}
	else
		info.fake_goal_feet_yaw = eye_yaw - fabs( max_body_yaw );

	info.fake_goal_feet_yaw = std::remainderf( info.fake_goal_feet_yaw, 360.f );

	if ( speed > 0.1f || fabs( velocity.z ) > 100.0f )
		info.fake_goal_feet_yaw = Math.approach_angle( eye_yaw, info.fake_goal_feet_yaw, ( ( state->m_ground_fraction * 20.0f ) + 30.0f ) * info.choked_time );
	else
		info.fake_goal_feet_yaw = Math.approach_angle( player->lower_body_yaw( ), info.fake_goal_feet_yaw, info.fake_goal_feet_yaw * 100.0f );

	/* desync either left or right */
	float left = state->m_eye_yaw + min_body_yaw;
	float right = state->m_eye_yaw + max_body_yaw;

	float resolve_yaw = 0.f;

	switch ( Global::MissedShots[ player->Networkable( )->EntIndex( ) ] % 3 ) {
	case 0: /* brute calc'd real */
		resolve_yaw = info.fake_goal_feet_yaw;
		Global::resolve_mode[ player->Networkable( )->EntIndex( ) ] = _( "r: smart 1" );
		break;
	case 1: /* brute left */
		resolve_yaw = left;
		Global::resolve_mode[ player->Networkable( )->EntIndex( ) ] = _( "r: smart 2" );
		break;
	case 2: /* brute right side */
		resolve_yaw = right;
		Global::resolve_mode[ player->Networkable( )->EntIndex( ) ] = _( "r: smart 3" );
		break;
	}

	state->m_goalfeet_yaw = resolve_yaw;
}


void cresolver::update_local_animation( ) {

	if ( !Global::Local || !Global::Local->IsAlive( ) )
		return;

	const auto state = Global::Local->AnimState( );
	if ( !state )
		return;

	if ( Global::InThirdperson ) {
		static float proper_abs = state->m_goalfeet_yaw;
		static std::array<float, 24> sent_pose_params = Global::Local->fl_mPoseParameter( );
		static std::array< animation_layers, 13 > o_layers;

		auto is_fresh_tick = [ ] ( ) -> bool {
			static int old_TickCount;

			if ( old_TickCount != Interfaces::GlobalVarsBase->TickCount ) {
				old_TickCount = Interfaces::GlobalVarsBase->TickCount;
				return true;
			}

			return false;
		};

		auto update_state = [ ] ( canimstate* state, Vector3D ang ) -> void {
			using fn = void( __vectorcall* )( void*, void*, float, float, float, void* );
			static auto fn_update_state = reinterpret_cast< fn >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) );

			if ( !fn_update_state )
				return;

			fn_update_state( state, nullptr, 0, ang.y, ang.x, 0 );
		};

		auto anim_overlays = Global::Local->animations_overlays( );
		if ( !anim_overlays )
			return;

		if ( is_fresh_tick( ) && Global::uCmd != nullptr && &o_layers != nullptr ) {
			std::memcpy( &o_layers, anim_overlays, sizeof( animation_layers ) * 13 );
			Global::Local->ClientSideAnimations( true );
			update_state( state, Global::uCmd->viewangles );

			if ( Global::send_packet ) {
				proper_abs = state->m_goalfeet_yaw;
				sent_pose_params = Global::Local->fl_mPoseParameter( );
			}
		}

		Global::Local->ClientSideAnimations( false );
		Global::Local->SetAbsAngles( Vector3D( 0, proper_abs, 0 ) );
		std::memcpy( anim_overlays, &o_layers, ( sizeof( animation_layers ) * 13 ) );

		Global::Local->fl_mPoseParameter( ) = sent_pose_params;
		Global::Local->fl_mPoseParameter( ).at( 6 ) = 0;
	}
	else {
		Global::Local->ClientSideAnimations( true );
	}
}

cresolver resolver;