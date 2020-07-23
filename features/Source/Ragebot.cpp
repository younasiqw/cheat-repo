#include "../Headers/Ragebot.h"
#include "../Headers/Autowall.h"
#include "../Headers/Misc.h" //meep is a nigger
#include "../Headers/Resolver.h"
#include "../Headers/Events.h"
#include "../Headers/Lagcomp.h"
caimbot aimbot;

void caimbot::on_move( ) {

	if ( !Global::Local->IsAlive( ) || Global::Local == nullptr)
		return;

	C_BaseCombatWeapon* weapon = Global::Local->GetActiveWeapon( );
	if ( !weapon )
		return;

	if ( !Controls.aim_active ) {
		Global::valid_hitchance = false;
		Global::can_damage = false;
		Global::TargetID = 0;
		return;
	}

	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData ) {
		Global::valid_hitchance = false;
		Global::can_damage = false;
		Global::TargetID = 0;
		return;
	}

	Vector3D origin = Global::Local->GetEyePosition();

	/* knifebot not implemented, shit */
	if ( Global::Local->IsKnife( ) || Global::Local->WeaponNade( ) ) {
		Global::valid_hitchance = false;
		Global::can_damage = false;
		Global::TargetID = 0;
		return;
	}

	/* hitchance for specific weapon groups */
	int hitchance = 35;

	if ( Global::Local->IsPistol( ) ) hitchance = Controls.whitchance[ PISTOL ];
	else if ( Global::Local->IsHeavyPistol( ) ) hitchance = Controls.whitchance[ HEAVY_PISTOL ];
	else if ( Global::Local->IsSMG( ) ) hitchance = Controls.whitchance[ SMG ];
	else if ( Global::Local->IsRifle( ) ) hitchance = Controls.whitchance[ RIFLE ];
	else if ( Global::Local->IsSniper( ) ) hitchance = Controls.whitchance[ SNIPER ];
	else if ( Global::Local->IsAuto( ) ) hitchance = Controls.whitchance[ AUTO ];
	else if ( Global::Local->IsMisc( ) ) hitchance = Controls.whitchance[ MISC ];

	for ( auto player : target_selection( ) ) {

		if ( !player || player == Global::Local )
			continue;

		/* should aim for uninterpolated position, store for later*/
		Vector3D old_origin = player->get_absolute_origin( );
		player->set_abs_origin( player->GetVecOrigin( ) );

		Vector3D destination = hitscan( player );
		Vector3D angle_to_player = Math.CalcAngle( origin, destination );

		/* distance for playing legit */
		float dist_to_player = this->CalculateDist( Global::uCmd->viewangles, angle_to_player );
		int damage = autowall.CalculateDamage( Global::Local->GetEyePosition(), destination, Global::Local, player ).damage;

		if ( Global::WhiteListIndex[ player->Networkable( )->EntIndex( ) ] )
			continue;

		if ( dist_to_player > Controls.legit_fov && Controls.legit_aim )
			continue;

		Global::can_damage = false;

		if ( angle_to_player != Vector3D( 0, 0, 0 ) && damage > 0 ) {


			Global::can_damage = true;

			if ( !Global::Local->IsScoped( ) && Global::Local->IsScopedWeapon( ) && Global::Local->GetFlags( ) & FL_ONGROUND ) {
				Global::uCmd->buttons &= ~in_attack;
				Global::uCmd->buttons |= in_zoom;
			}

			Global::valid_hitchance = false;

			/* update accuracy and check if higher than hitchance */
			weapon->UpdateAccuracyPenalty( );

			/* this->valid_hitchance(player, angle_to_player, hitchance, weapon, WeaponData ) */
			if ( 1 / weapon->GetInaccuracy( ) < hitchance + 10 )
				continue;


			Global::valid_hitchance = true;

			angle_to_player.clamp( );

			Global::uCmd->viewangles = angle_to_player - ( Global::Local->aim_punch_angle( ) * Interfaces::Convar->FindVar( "weapon_recoil_scale" )->GetFloat( ) );

			Global::TargetID = player->Networkable( )->EntIndex( );

			if ( !( Global::uCmd->buttons & in_attack ) && Controls.autoshoot ) {
				Global::uCmd->buttons |= in_attack;
			}

			Global::uCmd->TickCount = time_to_ticks( player->get_simulation_time( ) + lerp_time( ) );
		}

		/* after aimbot, reset position */
		player->set_abs_origin( old_origin );
	}
}

void caimbot::run_autostop( int speed_percentage ) {

	if ( Global::uCmd->buttons & in_jump )
		return;

	float speed = ( float ) ( FastSqrt( square( Global::uCmd->forwardmove ) + square( Global::uCmd->sidemove ) + square( Global::uCmd->upmove ) ) );
	if ( speed <= 0.f )
		return;

	if ( Global::uCmd->buttons & in_duck )
		speed_percentage *= 2.94117647f;

	if ( speed <= speed_percentage )
		return;

	float ratio = speed_percentage / speed;
	Global::uCmd->forwardmove *= ratio;
	Global::uCmd->sidemove *= ratio;
	Global::uCmd->upmove *= ratio;
}

std::vector<Vector3D> caimbot::get_safe_points( C_CSPlayer* player ) { //LMFAO i totally missed the point of safepoint

	std::vector<Vector3D> RETURNVECTOR;

	C_Ray desync_ray, real_ray;
	C_Trace desync_trace, real_trace;
	CTraceFilter desync_filter, real_filter;

	desync_ray.Init( Global::Local->GetEyePosition( ), Global::resolved_head_position[ player->Networkable( )->EntIndex( ) ] );

	Interfaces::Trace->TraceRay( desync_ray, mask_shot, &desync_filter, &desync_trace );

	if ( desync_trace.hitGroup < 0 )
		return RETURNVECTOR;

	/* ray hit desynced head */

	real_ray.Init( Global::Local->GetEyePosition( ), player->GetEyePosition( ) );

	Interfaces::Trace->TraceRay( real_ray, mask_shot, &real_filter, &real_trace );

	if ( real_trace.hitGroup < 0 )
		return RETURNVECTOR;

	/* ray hit real head */

	RETURNVECTOR.emplace_back( real_trace.end.average( desync_trace.end ) );

	return RETURNVECTOR;
}

bool sort_health( player_data one, player_data two ) {
	return one.health < two.health;
}

bool sort_distance( player_data one, player_data two ) {
	return one.distance < two.distance;
}

std::vector<C_CSPlayer*> caimbot::target_selection( ) {
	std::vector<player_data> players;
	std::vector<C_CSPlayer*> out;

	for ( int index = 1; index <= Interfaces::GlobalVarsBase->MaxClients; index++ ) {

		C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( index ) );

		if ( !player || !player->IsAlive( ) || player->IsDormant( ) || player->IsImmune( ) || Global::Local->GetTeam( ) == player->GetTeam( ) ) {
			Global::valid_hitchance = true;
			Global::can_damage = false;
			continue;
		}

		players.emplace_back( player_data( index, player->GetHealth( ), ( Global::Local->GetVecOrigin( ) - player->GetVecOrigin( ) ).Length( ) ) );
	}

	std::sort( players.begin( ), players.end( ), sort_health ); //sort by health
	//std::sort( players.begin( ), players.end( ), sort_distance ); //sort by distance

	for ( auto player : players )
		out.emplace_back( reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( player.id ) ) );

	return out;
}

bool caimbot::valid_hitchance( C_CSPlayer* player, Vector3D angle, int hitchance, C_BaseCombatWeapon* weapon, CCSWeaponData* data ) {
	Vector3D forward, right, up;
	Math.AngleVectors( angle, &forward, &right, &up );

	Vector3D eye_pos = Global::Local->GetEyePosition( );
	int hits = 0;

	for ( int i = 1; i <= 256.f; i++ ) {
		float innacuracy = Math.RandomFloat( 0.f, 1.f ) * weapon->GetInaccuracy( );
		float spread = Math.RandomFloat( 0.f, 1.f ) * weapon->GetSpread( );

		Vector3D spread_view( ( cos( Math.RandomFloat( 0.f, 2.f * M_PI ) ) * innacuracy ) + ( cos( Math.RandomFloat( 0.f, 2.f * M_PI ) ) * spread ),
			( sin( Math.RandomFloat( 0.f, 2.f * M_PI ) ) * innacuracy ) + ( sin( Math.RandomFloat( 0.f, 2.f * M_PI ) ) * spread ), 0 ),
			direction;

		direction =
			Vector3D( forward.x + ( spread_view.x * right.x ) + ( spread_view.y * up.x ),
				forward.y + ( spread_view.x * right.y ) + ( spread_view.y * up.y ),
				forward.z + ( spread_view.x * right.z ) + ( spread_view.y * up.z ) );

		direction.normalize( );

		Vector3D vspread, view_forward;

		Math.VectorAngles( direction, up, vspread );
		Math.NormalizeAngles( vspread );

		Math.AngleVectors( vspread, &view_forward );
		view_forward.NormalizeInPlace( );

		view_forward = eye_pos + ( view_forward * data->range );

		C_Trace trace;

		Interfaces::Trace->ClipRayToEntity( C_Ray( eye_pos, view_forward ), mask_shot | contents_grate, player, &trace );

		if ( trace.m_pEnt == player )
			hits++;

		if ( hits >= ( float( hitchance + 10 ) * ( 2.56f ) ) )
			return true;
	}
	return false;
}
Vector3D caimbot::hitscan( C_CSPlayer* player ) {

	std::vector<int> HitboxVector;

	C_BaseCombatWeapon* weapon = Global::Local->GetActiveWeapon( );
	if ( !weapon ) return Vector3D( 0, 0, 0 );

	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData ) return Vector3D( 0, 0, 0 );

	if ( Global::Local->IsKnife( ) || Global::Local->WeaponNade( ) )
		return Vector3D( 0, 0, 0 );

	bool body_aim =
		resolver.info.safepoint[ player->Networkable( )->EntIndex( ) ] ||
		WeaponData->weapon_name == "weapon_taser" ||
		Global::BodyAimIndex[ player->Networkable( )->EntIndex( ) ];

	int MinimumDamage = 0;

	std::vector<bool*> options = {
		Controls.AutoHitboxes,
		Controls.RifleHitboxes,
		Controls.SniperHitboxes,
		Controls.PistolHitboxes,
		Controls.SMGHitboxes,
		Controls.HeavyPistolHitboxes,
		Controls.MiscHitboxes
	};

	if ( body_aim ) {
		HitboxVector.emplace_back( hitbox::HITBOX_LOWER_CHEST );
		HitboxVector.emplace_back( hitbox::HITBOX_THORAX );
		HitboxVector.emplace_back( hitbox::HITBOX_PELVIS );
		HitboxVector.emplace_back( hitbox::HITBOX_BELLY );
	}
	else {
		int index = 0;
		//for ( int i = 0; i < options.size( ); i++ ) {
		if ( Global::Local->IsAuto( ) ) { index = 0; }
		if ( Global::Local->IsRifle( ) ) { index = 1; }
		if ( Global::Local->IsSniper( ) ) { index = 2; }
		if ( Global::Local->IsPistol( ) ) { index = 3; }
		if ( Global::Local->IsSMG( ) ) { index = 4; }
		if ( Global::Local->IsHeavyPistol( ) ) { index = 5; }
		if ( Global::Local->IsMisc( ) ) { index = 6; }

		if ( options.at( index )[ 0 ] ) {
			HitboxVector.emplace_back( hitbox::HITBOX_HEAD );
		}
		if ( options.at( index )[ 1 ] ) {
			HitboxVector.emplace_back( hitbox::HITBOX_NECK );
		}
		if ( options.at( index )[ 2 ] ) {
			HitboxVector.emplace_back( hitbox::HITBOX_LOWER_CHEST );
			HitboxVector.emplace_back( hitbox::HITBOX_UPPER_CHEST );
		}
		if ( options.at( index )[ 3 ] ) {
			HitboxVector.emplace_back( hitbox::HITBOX_THORAX );
			HitboxVector.emplace_back( hitbox::HITBOX_PELVIS );
			HitboxVector.emplace_back( hitbox::HITBOX_BELLY );
		}
		if ( options.at( index )[ 4 ] ) {
			HitboxVector.emplace_back( hitbox::HITBOX_LEFT_FOREARM );
			HitboxVector.emplace_back( hitbox::HITBOX_RIGHT_FOREARM );
			HitboxVector.emplace_back( hitbox::HITBOX_LEFT_HAND );
			HitboxVector.emplace_back( hitbox::HITBOX_RIGHT_HAND );
			HitboxVector.emplace_back( hitbox::HITBOX_LEFT_UPPER_ARM );
			HitboxVector.emplace_back( hitbox::HITBOX_RIGHT_UPPER_ARM );
		}
		if ( options.at( index )[ 5 ] ) {
			HitboxVector.emplace_back( hitbox::HITBOX_LEFT_CALF );
			HitboxVector.emplace_back( hitbox::HITBOX_RIGHT_CALF );
			HitboxVector.emplace_back( hitbox::HITBOX_LEFT_THIGH );
			HitboxVector.emplace_back( hitbox::HITBOX_RIGHT_THIGH );
			HitboxVector.emplace_back( hitbox::HITBOX_LEFT_FOOT );
			HitboxVector.emplace_back( hitbox::HITBOX_RIGHT_FOOT );
		}

		MinimumDamage = Controls.MinimumDamage[ index ];
	}

	int best_hitbox = 0;
	float damage = 0.f;
	float highest_dmg = 0.f;
	float radius = 0.f;

	//std::cout << "HV: " << HitboxVector.size( ) << std::endl;

	for ( size_t i = 0; i < HitboxVector.size( ); i++ ) {
		damage = autowall.CalculateDamage( Global::Local->GetEyePosition( ), this->GetHitboxPosition( player, HitboxVector.at( i ) ), Global::Local, player ).damage;

		if ( damage > highest_dmg ) {
			highest_dmg = damage;
			best_hitbox = HitboxVector.at( i );
		}
	}

	int best_min_dmg = weapon->GetAmmo( ) <= 3 ? player->GetHealth( ) : MinimumDamage;

	if ( best_hitbox == hitbox::HITBOX_HEAD )
		radius *= Controls.HeadPointScale;
	else
		radius *= Controls.BodyPointScale;

	if ( Global::fired_shot[ player->Networkable( )->EntIndex( ) ] ) {
		return this->GetHitboxPosition( player, hitbox::HITBOX_HEAD );
	}
	else if ( highest_dmg >= best_min_dmg ) {
		return this->GetHitboxPosition( player, best_hitbox );
	}
	return Vector3D( 0, 0, 0 );
}

Vector3D caimbot::GetHitboxPosition( C_CSPlayer* entity, int HitboxID, float* radius ) {
	auto getHitbox = [ ] ( C_CSPlayer* entity, int hitboxIndex ) -> mstudiobbox_t* {
		if ( entity->IsDormant( ) || entity->GetHealth( ) <= 0 ) return NULL;

		const auto pModel = entity->renderable( )->get_model( );
		if ( !pModel ) return NULL;

		auto pStudioHdr = Interfaces::ModelInfo->GetStudioModel( pModel );
		if ( !pStudioHdr ) return NULL;

		auto pSet = pStudioHdr->GetHitboxSet( 0 );
		if ( !pSet ) return NULL;

		if ( hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0 ) return NULL;

		return pSet->GetHitbox( hitboxIndex );
	};

	auto hitbox = getHitbox( entity, HitboxID );
	if ( !hitbox ) return Vector3D( 0, 0, 0 );

	auto bone_matrix = entity->GetBoneMatrix( hitbox->bone );

	Vector3D bbmin, bbmax;
	Math.VectorTransform( hitbox->min, bone_matrix, bbmin );
	Math.VectorTransform( hitbox->max, bone_matrix, bbmax );

	if ( radius && Controls.PointScale )
		*radius = hitbox->radius;

	return ( bbmin + bbmax ) * 0.5f;
}