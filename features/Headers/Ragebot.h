#pragma once
#include "../../entities/entity.h"
#include "../../Controls/Controls.h"
#include "../../Math/Math.h"
#include "../../menu/config.h"
#include <iostream>

enum hitbox {
	HITBOX_HEAD = 0,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BELLY,
	HITBOX_THORAX,
	HITBOX_LOWER_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX 
};

struct player_data {
	int id, health, distance;

	player_data( int id, int health, int distance ) {
		this->id = id;
		this->health = health;
		this->distance = distance;
	}
};

extern bool sort_health( player_data one, player_data two ), sort_distance( player_data one, player_data two );

#define tick_interval			( Interfaces::GlobalVarsBase->IntervalPerTick )
#define time_to_ticks( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::GlobalVarsBase->IntervalPerTick ) )
#define ticks_to_time( t )		( tick_interval *( t ) )

/*class chitbox {
public:
	int hitbox_id;
	bool is_obb;
	Vector3D mins;
	Vector3D maxs;
	float radius;
	int bone;
};*/

class caimbot
{
public:
	void on_move( );
	Vector3D hitscan(C_CSPlayer* player );
	void run_autostop( int speed_percentage );
	std::vector<Vector3D> get_safe_points( C_CSPlayer* player );
	std::vector<C_CSPlayer*> target_selection( );
	//void get_hitbox_data( chitbox* rtn, C_CSPlayer* ent, int ihitbox, matrix3x4_t* matrix );
	bool valid_hitchance( C_CSPlayer* player, Vector3D angle, int hitchance, C_BaseCombatWeapon* weapon, CCSWeaponData* data );
	Vector3D GetHitboxPosition( C_CSPlayer* entity, int hitbox_id, float* radius = nullptr);

	float CalculateDist( const Vector3D& viewAngle, const Vector3D& aimAngle ) {
		Vector3D ang, aim;

		Math.AngleVectors( viewAngle, &aim );
		Math.AngleVectors( aimAngle, &ang );

		return rad2deg( acos( aim.dot( ang ) / aim.length_sqr( ) ) );
	}

	float lerp_time( ) {
		static cconvar* updaterate = Interfaces::Convar->FindVar( "cl_updaterate" );
		static cconvar* minupdate = Interfaces::Convar->FindVar( "sv_minupdaterate" );
		static cconvar* maxupdate = Interfaces::Convar->FindVar( "sv_maxupdaterate" );
		static cconvar* lerp = Interfaces::Convar->FindVar( "cl_interp" );
		static cconvar* cmin = Interfaces::Convar->FindVar( "sv_client_min_interp_ratio" );
		static cconvar* cmax = Interfaces::Convar->FindVar( "sv_client_max_interp_ratio" );
		static cconvar* ratio = Interfaces::Convar->FindVar( "cl_interp_ratio" );

		float lerpurmom = lerp->GetFloat( ), maxupdateurmom = maxupdate->GetFloat( ),
			ratiourmom = ratio->GetFloat( ), cminurmom = cmin->GetFloat( ), cmaxurmom = cmax->GetFloat( );
		int updaterateurmom = updaterate->get_int( ),
			sv_maxupdaterate = maxupdate->get_int( ), sv_minupdaterate = minupdate->get_int( );

		if ( sv_maxupdaterate && sv_minupdaterate )
			updaterateurmom = maxupdateurmom;

		if ( ratiourmom == 0 )
			ratiourmom = 1.0f;

		if ( cmin && cmax && cmin->GetFloat( ) != 1 )
			ratiourmom = std::clamp( ratiourmom, cminurmom, cmaxurmom );

		return max( lerpurmom, ratiourmom / updaterateurmom );
	}
};

extern caimbot aimbot;