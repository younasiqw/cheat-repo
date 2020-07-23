#pragma once
#include "../../hooks/hooks.h"
#include "../../render/render.h"
#include "../../menu/config.h"
class cvisuals {

public:
	void run_visuals( );
	void draw_capsule_overlay( C_CSPlayer* player, color col, float duration );

	struct box {
		int x, y, w, h;
		box( ) = default;
		box( int x, int y, int w, int h ) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	void RunHitmarkers( );
	void RunDamageIndicators( );
	void RunBulletBeams( );
	void RunIndicators( );
	void RunBombTimer( );
	void RunRemoveSmoke( );
private:
	void run_health( C_CSPlayer* player, box bbox );
	void run_weapon( C_CSPlayer* player, box bbox );
	void run_ammo( C_CSPlayer* player, box bbox );
	void run_name( box bbox, player_info_t info );
	void run_box( box bbox );
	void run_compass_players( C_CSPlayer* player );
	void run_compass_numbers( );
	void run_info( C_CSPlayer* player, box bbox );
	void run_smoke_esp( C_CSPlayer* player );
	void run_skeleton( C_CSPlayer* player );
	void run_fov_arrows( C_CSPlayer* player );

	bool get_playerbox( C_CSPlayer* ent, box& in ) {
		Vector3D origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
		int left, top, right, bottom;

		origin = ent->get_absolute_origin( );
		min = ent->collideable( )->mins( ) + origin;
		max = ent->collideable( )->maxs( ) + origin;

		Vector3D points[ ] = {
			Vector3D( min.x, min.y, min.z ),
			Vector3D( min.x, max.y, min.z ),
			Vector3D( max.x, max.y, min.z ),
			Vector3D( max.x, min.y, min.z ),
			Vector3D( max.x, max.y, max.z ),
			Vector3D( min.x, max.y, max.z ),
			Vector3D( min.x, min.y, max.z ),
			Vector3D( max.x, min.y, max.z )
		};
		if ( !Interfaces::DebugOverlay->WorldToScreen( points[ 3 ], flb ) || !Interfaces::DebugOverlay->WorldToScreen( points[ 5 ], brt )
			|| !Interfaces::DebugOverlay->WorldToScreen( points[ 0 ], blb ) || !Interfaces::DebugOverlay->WorldToScreen( points[ 4 ], frt )
			|| !Interfaces::DebugOverlay->WorldToScreen( points[ 2 ], frb ) || !Interfaces::DebugOverlay->WorldToScreen( points[ 1 ], brb )
			|| !Interfaces::DebugOverlay->WorldToScreen( points[ 6 ], blt ) || !Interfaces::DebugOverlay->WorldToScreen( points[ 7 ], flt ) )
			return false;

		Vector3D arr[ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

		left = flb.x;
		top = flb.y;
		right = flb.x;
		bottom = flb.y;

		for ( int i = 1; i < 8; i++ ) {
			if ( left > arr[ i ].x )
				left = arr[ i ].x;
			if ( bottom < arr[ i ].y )
				bottom = arr[ i ].y;
			if ( right < arr[ i ].x )
				right = arr[ i ].x;
			if ( top > arr[ i ].y )
				top = arr[ i ].y;
		}
		in.x = ( int ) left;
		in.y = ( int ) top;
		in.w = int( right - left );
		in.h = int( bottom - top );


		return true;
	}

};
class player_info {
public:
	std::string info;
	color fcolor;

	player_info( std::string info, color fcolor ) {
		this->info = info;
		this->fcolor = fcolor;
	}
};
extern cvisuals visuals;