#pragma once
#include "../../Controls/Controls.h"
#include "../../hooks/hooks.h"
#include "../../menu/config.h"
#include "Ragebot.h"
#include "autowall.h"

class cantiaim
{
public:
	void on_move( );
	void run_pitch( );
	void run_yaw( );
	bool predict_lby_update( bool& send_packet );
	void should_fake_lag_on_peek( );
	void double_tap( );
	bool pressing_move( ) {
		return ( Global::uCmd->buttons & in_left || Global::uCmd->buttons & in_forward || Global::uCmd->buttons & in_back ||
			Global::uCmd->buttons & in_right || Global::uCmd->buttons & in_moveleft || Global::uCmd->buttons & in_moveright );
	}

private:

	void run_freestanding( ) {
		C_CSPlayer* Local = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );

		if ( !Local || !Local->IsAlive( ) )
			return;

		static float final_angle;
		bool bside1 = false;
		bool bside2 = false;
		bool autowalld = false;
		for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; ++i ) {
			C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );
			if ( !player
				|| !player->IsAlive( )
				|| player->IsDormant( )
				|| player == Local
				|| player->GetTeam( ) == Local->GetTeam( ) )
				continue;

			float angle_to_local = Math.CalcAngle( Local->GetVecOrigin( ), player->GetVecOrigin( ) ).y;
			Vector3D ViewPoint = player->GetVecOrigin( ) + Vector3D( 0, 0, 90 );
			vector2d_t Side1 = { ( 45 * sin( Math.GRD_TO_BOG( angle_to_local ) ) ),( 45 * cos( Math.GRD_TO_BOG( angle_to_local ) ) ) };
			vector2d_t Side2 = { ( 45 * sin( Math.GRD_TO_BOG( angle_to_local + 180 ) ) ) ,( 45 * cos( Math.GRD_TO_BOG( angle_to_local + 180 ) ) ) };
			vector2d_t Side3 = { ( 50 * sin( Math.GRD_TO_BOG( angle_to_local ) ) ),( 50 * cos( Math.GRD_TO_BOG( angle_to_local ) ) ) };
			vector2d_t Side4 = { ( 50 * sin( Math.GRD_TO_BOG( angle_to_local + 180 ) ) ) ,( 50 * cos( Math.GRD_TO_BOG( angle_to_local + 180 ) ) ) };
			Vector3D Origin = Local->GetVecOrigin( );
			vector2d_t OriginLeftRight[ ] = { vector2d_t( Side1.x, Side1.y ), vector2d_t( Side2.x, Side2.y ) };
			vector2d_t OriginLeftRightLocal[ ] = { vector2d_t( Side3.x, Side3.y ), vector2d_t( Side4.x, Side4.y ) };
			for ( int side = 0; side < 2; side++ ) {
				Vector3D OriginAutowall = { Origin.x + OriginLeftRight[ side ].x,  Origin.y - OriginLeftRight[ side ].y , Origin.z + 80 };
				Vector3D OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[ side ].x,  ViewPoint.y - OriginLeftRightLocal[ side ].y , ViewPoint.z };
				if ( autowall.CanHitFloatingPoint( OriginAutowall, ViewPoint ) ) {
					if ( side == 0 ) {
						bside1 = true;
						final_angle = angle_to_local + 90;
					}
					else if ( side == 1 ) {
						bside2 = true;
						final_angle = angle_to_local - 90;
					}
					autowalld = true;
				}
				else {
					for ( int side222 = 0; side222 < 2; side222++ ) {
						Vector3D OriginAutowall222 = { Origin.x + OriginLeftRight[ side222 ].x,  Origin.y - OriginLeftRight[ side222 ].y , Origin.z + 80 };
						if ( autowall.CanHitFloatingPoint( OriginAutowall222, OriginAutowall2 ) ) {
							if ( side222 == 0 ) {
								bside1 = true;
								final_angle = angle_to_local + 90;
							}
							else if ( side222 == 1 ) {
								bside2 = true;
								final_angle = angle_to_local - 90;
							}
							autowalld = true;
						}
					}
				}
			}
		}
		if ( !autowalld || ( bside1 && bside2 ) ) {
			Global::uCmd->viewangles.y += 180;
		}
		else {
			Global::uCmd->viewangles.y = final_angle;
		}
	}
};

extern cantiaim antiaim;