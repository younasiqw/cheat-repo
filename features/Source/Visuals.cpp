#include "../Headers/Visuals.h"
#include "../../Controls/Controls.h"
#include "../../render/Textures.h"
#include "../Headers/Ragebot.h"
#include "../Headers/Lagcomp.h"
cvisuals visuals;

void cvisuals::run_visuals( ) {

	if ( Controls.compass )
		run_compass_numbers( );

	for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {
		C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

		if ( !player || !Global::Local)
			continue;

		/*std::vector<Vector3D> safe_points = aimbot.get_safe_points( player, Vector3D(0, 0, 0), Vector3D( 0, 0, 0 ) );

		for ( int i = 0; i < safe_points.size( ); i++ ) {
			Vector3D w2s;
			if ( !Interfaces::DebugOverlay->WorldToScreen( safe_points.at( i ), w2s ) )
				continue;

			Render::RoundedRect( w2s.x, w2s.y, 4, 4, 2, color( 255, 255, 255 ) );
		}*/
	

		if ( !player->IsAlive( ) || player == Global::Local || player->IsDormant( ) )
			continue;

		if ( Controls.fov_arrows[ 0 ] || Controls.fov_arrows[ 1 ] )
			run_fov_arrows( player );

		box bbox;
		if ( !get_playerbox( player, bbox ) )
			continue;

		player_info_t info;
		Interfaces::engine->get_player_info( player->Networkable( )->EntIndex( ), &info );

		if ( Controls.teamcheck && ( Global::Local->GetTeam( ) == player->GetTeam( ) ) )
			continue;

		if ( Controls.name )
			run_name( bbox, info );

		if ( Controls.box )
			run_box( bbox );

		if ( Controls.weapon )
			run_weapon( player, bbox );

		if ( Controls.health )
			run_health( player, bbox );

		if ( Controls.info )
			run_info( player, bbox );

		if ( Controls.compass )
			run_compass_players( player );

		if ( Controls.ammo )
			run_ammo( player, bbox );

		if ( Controls.skeleton )
			run_skeleton( player );
		
		if ( Controls.cock_mode && Global::Local->IsAlive()) {
			cTextures penis = cTextures( 105, 32, 54 );
			Global::Texture_id = Interfaces::Surface->create_new_Texture_id( true );
			menu.create_Texture( penis );

			Interfaces::Surface->draw_Textured_rect( bbox.x, bbox.y, bbox.w, bbox.h );
		}
	}
	for ( int i = 1; i <= Interfaces::EntityList->get_highest_entity_index( ); i++ ) {
		C_CSPlayer* entity = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

		if ( !entity )
			continue;

		run_smoke_esp( entity );
	}
}

#define blue 0,191,255
#define red 240, 128, 128
#define white 240, 240, 240

void cvisuals::run_info( C_CSPlayer* player, box bbox ) {

	//pinfo.emplace_back(std::to_string(player->GetEyeAngles().x));

	std::vector<std::pair<std::string, color>> flags;

	//flags.emplace_back( player_info( Global::shot[ player->Networkable( )->EntIndex( ) ] ? "FIRED" : "STILL", color( red ) ) );

	//flags.emplace_back( player_info( std::to_string( last_shot_time[ player->Networkable( )->EntIndex( ) ] ), color( red ) ) );
	//flags.emplace_back( player_info( std::to_string( weapon->m_fLastShotTime( ) ), color( red ) ) );

	//flags.emplace_back( player_info( "missed shots: " + std::to_string( Global::missed_shots[ player->Networkable( )->EntIndex( ) ] ), color( red ) ) );

	flags.push_back( { Global::resolve_mode[ player->Networkable( )->EntIndex( ) ], color( white ) } );

	if ( Global::BodyAimIndex[ player->Networkable( )->EntIndex( ) ] ) {
		flags.push_back( { "baim", color( white ) } );
	}
	if ( Global::WhiteListIndex[ player->Networkable( )->EntIndex( ) ] ) {
		flags.push_back( { "whitelist", color( white ) } );
	}

	if ( player->has_helmet( ) && player->get_armor( ) < 0 ) {
		flags.push_back( { "h", color( blue ) } );
	}
	if ( player->get_armor( ) > 0 && player->has_helmet( ) ) {
		flags.push_back( { "hk", color( blue ) } );
	}
	if ( player->GetFlags( ) & in_duck ) {
		if ( player->duck_amount( ) > 1060000000 && player->duck_amount( ) < 1062000000 ) {
			flags.push_back( { "fd", color( red ) } );
		}
		else {
			flags.push_back( { "duck", color( white ) } );
		}
	}
	if ( player->IsScoped( ) ) {
		flags.push_back( { "scope", color( red ) } );
	}

	for ( int i = 0; i < flags.size( ); i++ ) {
		Render::Text( Render::Fonts::visuals, flags[ i ].first, bbox.x + bbox.w + 3, bbox.y + ( i * 8 ), false, flags[ i ].second.r, flags[ i ].second.g, flags[ i ].second.b );
	}
}
void cvisuals::run_skeleton( C_CSPlayer* player ) {
	color skeleton_color = color( Controls.skeleton_color.r, Controls.skeleton_color.g, Controls.skeleton_color.b, Controls.skeleton_color.a );

	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudioModel( player->renderable( )->get_model( ) );

	if ( !pStudioHdr )
		return;

	Vector3D vParent, vChild, sParent, sChild;

	for ( int j = 0; j < pStudioHdr->numbones; j++ ) {
		mstudiobone_t* pBone = pStudioHdr->GetBone( j );

		if ( pBone && ( pBone->flags & 0x100 ) && ( pBone->parent != -1 ) ) {
			vChild = player->GetBonePosition( j );
			vParent = player->GetBonePosition( pBone->parent );

			if ( Interfaces::DebugOverlay->WorldToScreen( vParent, sParent ) && Interfaces::DebugOverlay->WorldToScreen( vChild, sChild ) ) {
				Render::line( sParent[ 0 ], sParent[ 1 ], sChild[ 0 ], sChild[ 1 ], skeleton_color );
			}
		}
	}
}
void cvisuals::run_ammo( C_CSPlayer* player, box bbox ) {
	C_BaseCombatWeapon* weapon = player->GetActiveWeapon( );
	if ( !weapon ) return;
	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData ) return;

	if ( bbox.w > 400 ) //random value it shouldnt be larger than
		return;

	int
		max_in_clip = WeaponData->max_clip,
		ammo_in_clip = weapon->GetAmmo( ),
		scaled_ammo = ammo_in_clip * bbox.w / max_in_clip,
		value = 0;

	if ( scaled_ammo > 0 ) {
		int w = bbox.w;
		value = std::clamp( scaled_ammo, 0, w );
	}

	Render::FilledRect( bbox.x - 1, bbox.y + bbox.h + 1, bbox.w + 2, 3, color( 0, 0, 0, 100 ) );
	Render::FilledRect( bbox.x, bbox.y + bbox.h + 2, value, 1, color( 0, 191, 191 ) );

	if ( ammo_in_clip == max_in_clip )
		return;

	Render::Text( Render::Fonts::visuals, std::to_string( ammo_in_clip ), bbox.x + 1 + value - 3, bbox.y + bbox.h, true, 240, 240, 240, 255 );
}
void cvisuals::run_weapon( C_CSPlayer* player, box bbox ) {

	C_BaseCombatWeapon* weapon = player->GetActiveWeapon( );
	if ( !weapon ) return;
	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData ) return;

	std::string weapon_name = WeaponData->weapon_name;

	int size = weapon_name.size( );
	std::string substr = weapon_name.substr( 7, weapon_name.size( ) - 7 );

	auto WeaponIcon = player->get_WeaponIcon( weapon );

	color weapon_color = color( Controls.weapon_color.r, Controls.weapon_color.g, Controls.weapon_color.b, Controls.weapon_color.a );

	if ( Controls.weapon == 1 )
		Render::Text( Render::Fonts::visuals, substr, bbox.x + ( bbox.w / 2 ), bbox.y + bbox.h + ( Controls.ammo ? 5 : 0 ), true, weapon_color.r, weapon_color.g, weapon_color.b, weapon_color.a );
	else if ( Controls.weapon == 2 )
		Render::Text( Render::Fonts::WeaponIcon, WeaponIcon, bbox.x + ( bbox.w / 2 ), bbox.y + bbox.h + ( Controls.ammo ? 5 : 0 ), true, weapon_color.r, weapon_color.g, weapon_color.b, weapon_color.a );
	else if ( Controls.weapon == 3 ) {
		Render::Text( Render::Fonts::visuals, substr, bbox.x + ( bbox.w / 2 ), bbox.y + bbox.h + ( Controls.ammo ? 5 : 0 ), true, weapon_color.r, weapon_color.g, weapon_color.b, weapon_color.a );
		Render::Text( Render::Fonts::WeaponIcon, WeaponIcon, bbox.x + ( bbox.w / 2 ), bbox.y + bbox.h + 7 + ( Controls.ammo ? 5 : 0 ), true, weapon_color.r, weapon_color.g, weapon_color.b, weapon_color.a );

	}
}

void cvisuals::run_health( C_CSPlayer* player, box bbox ) {

	int bar_value = bbox.h * ( player->GetHealth( ) / 100.f );

	Render::FilledRect( bbox.x - 5, bbox.y - 1, 3, bbox.h + 2, color( 0, 0, 0, 170 ) );
	Render::FilledRect( bbox.x - 4, bbox.y + bbox.h - bar_value, 1, bar_value, color( 0, 255, 0 ) );

	if ( player->GetHealth( ) < 100 )
		Render::Text( Render::Fonts::preview, std::to_string( player->GetHealth( ) ), bbox.x - 6, bbox.y + bbox.h - bar_value - 4, true, 240, 240, 240 );
}

void cvisuals::run_box( box bbox ) {

	color box_color = color( Controls.box_color.r, Controls.box_color.g, Controls.box_color.b, Controls.box_color.a );

	Render::OutlinedRect( bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color( 0, 0, 0, 100 ) );
	Render::OutlinedRect( bbox.x, bbox.y, bbox.w, bbox.h, box_color );
	Render::OutlinedRect( bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color( 0, 0, 0, 100 ) );
}

void cvisuals::run_name( box bbox, player_info_t info ) {
	std::string name = info.name;

	color name_color = color( Controls.name_color.r, Controls.name_color.g, Controls.name_color.b, Controls.name_color.a );

	Render::Text( Render::Fonts::visuals, name, bbox.x + ( bbox.w / 2 ), bbox.y - 10, true, name_color.r, name_color.g, name_color.b, name_color.a );

}

void cvisuals::run_compass_numbers( ) {
	int screen_width, screen_height;
	Interfaces::Surface->get_screen_size( screen_width, screen_height );
	Render::FilledRect( 0, 0, screen_width, 2, color( 120, 120, 120 ) );

	for ( int i = 0; i < screen_width; i += 105 ) {
		Render::FilledRect( ( 1 * i ) + 15, 0, 1, 14, color( 140, 140, 140 ) );
	}

	for ( int j = 0; j <= 180; j += 5 ) {
		int y = 10;

		if ( j % 5 == 0 && j % 10 != 0 )
			y -= 5;
		else if ( j % 5 == 0 && j % 10 == 0 )
			y += 5;

		Render::Text( Render::Fonts::main, std::to_string( j ), ( j * 10.5 ) + 15, y, true, 180, 180, 180, 255 );
	}

	for ( int i = 52.5; i < screen_width; i += 105 ) {
		Render::FilledRect( ( 1 * i ) + 15, 0, 1, 6, color( 160, 160, 160 ) );
	}
}

void cvisuals::run_compass_players( C_CSPlayer* player ) {

	Vector3D pos, pos_3d = ( player->get_absolute_origin( ) - Vector3D( 0, 0, 10 ) );

	if ( !Interfaces::DebugOverlay->WorldToScreen( pos_3d, pos ) )
		return;

	Render::FilledRect( pos.x, 32, 3, 3, color( 255, 255, 255 ) );

	Render::Text( Render::Fonts::main, std::to_string( player->GetHealth( ) ), pos.x, 38, true, 255, 255, 255, 255 );
}

void cvisuals::run_smoke_esp( C_CSPlayer* player) {

	int time = 0;
	bool running_clock = false;
	if ( player->clientclass( )->class_id == CSmokeGrenade && Controls.smoke_radius) {

		for ( int i = 0; i < grenadeinfo.size( ); i++ ) {

			Render::draw_circle_3d( grenadeinfo.at( i ).position );

			if ( strstr( player->clientclass( )->network_name, "CSmoke" ) ) {
				Vector3D w2s;
				if ( !Interfaces::DebugOverlay->WorldToScreen( grenadeinfo.at( i ).position, w2s ) )
					return;

				time = grenadeinfo.at( i ).curtime + 18 - Interfaces::GlobalVarsBase->CurrentTime;
				int count_down = std::clamp( time, 0, 18 ); running_clock = true;
				Render::Text( Render::Fonts::visuals, "Smoke: " + std::to_string( count_down ), w2s.x, w2s.y, false, 240, 240, 240 );
			}

			if ( grenadeinfo.at( i ).curtime + 18 < Interfaces::GlobalVarsBase->CurrentTime )
				grenadeinfo.erase( grenadeinfo.begin( ) + i );
		}
	}		
	if ( strstr( player->clientclass( )->network_name, "CWeapon" ) && Controls.weapon_esp[ 2 ] )
	{
		if ( player->get_owner_handle( ) == -1 ) {
			Vector3D w2s;
			if ( !Interfaces::DebugOverlay->WorldToScreen( player->get_absolute_origin( ), w2s ) )
				return;

			std::string weapon_name = player->clientclass( )->network_name;
			std::string substr = weapon_name.substr( 7, weapon_name.length( ) - 7 );

			Render::Text( Render::Fonts::visuals, substr, w2s.x, w2s.y, false, 240, 240, 240 );
		}
	}
	if ( Controls.grenade_esp[ 2 ] ) {
		if ( strstr( player->clientclass( )->network_name, "CSmoke" ) || strstr( player->clientclass( )->network_name, "CHE" )
			|| strstr( player->clientclass( )->network_name, "CMolo" ) || strstr( player->clientclass( )->network_name, "CFlash" ) || strstr( player->clientclass( )->network_name, "CDecoy" ) ) {

			if ( player->get_owner_handle( ) == -1 ) {
				Vector3D w2s;
				if ( !Interfaces::DebugOverlay->WorldToScreen( player->get_absolute_origin( ), w2s ) )
					return;

				std::string weapon_name = player->clientclass( )->network_name;
				std::string substr = weapon_name.substr( 1, weapon_name.length( ) - 1 );

				if ( running_clock && player->clientclass( )->class_id == CSmokeGrenade )
					return;

				Render::Text( Render::Fonts::visuals, substr, w2s.x, w2s.y, false, 240, 240, 240 );
			}
		}
	}
}

void cvisuals::draw_capsule_overlay( C_CSPlayer* player, color col, float duration ) {

	studiohdr_t* pStudioModel = Interfaces::ModelInfo->GetStudioModel( ( model_t* ) player->renderable()->get_model( ) );
	if ( !pStudioModel )
		return;

	static matrix3x4_t pBoneToWorldOut[ 128 ];
	if ( !player->renderable()->setup_bones( pBoneToWorldOut, 128, 0x00000100, player->get_simulation_time( ) ))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet( 0 );
	if ( !pHitboxSet )
		return;

	auto VectorTransform = [ ] ( const Vector3D in1, matrix3x4_t in2, Vector3D& out ) {
		out[ 0 ] = Math.DotProduct( in1, Vector3D( in2[ 0 ][ 0 ], in2[ 0 ][ 1 ], in2[ 0 ][ 2 ] ) ) + in2[ 0 ][ 3 ];
		out[ 1 ] = Math.DotProduct( in1, Vector3D( in2[ 1 ][ 0 ], in2[ 1 ][ 1 ], in2[ 1 ][ 2 ] ) ) + in2[ 1 ][ 3 ];
		out[ 2 ] = Math.DotProduct( in1, Vector3D( in2[ 2 ][ 0 ], in2[ 2 ][ 1 ], in2[ 2 ][ 2 ] ) ) + in2[ 2 ][ 3 ];
	};

	for ( int i = 0; i < pHitboxSet->numhitboxes; i++ ) {
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox( i );
		if ( !pHitbox )
			continue;

		Vector3D vMin, vMax;
		VectorTransform( pHitbox->min, pBoneToWorldOut[ pHitbox->bone ], vMin );
		VectorTransform( pHitbox->max, pBoneToWorldOut[ pHitbox->bone ], vMax );

		if ( pHitbox->radius > -1 ) {
			Interfaces::DebugOverlay->add_capsule_overlay( vMin, vMax, pHitbox->radius, col.r, col.g, col.b, col.a, duration );
		}
	}
}

void cvisuals::run_fov_arrows( C_CSPlayer* player ) {

	if ( !Global::Local )
		return;

	if (  Global::Local->GetTeam( ) == player->GetTeam( )  )
		return;

	Vector3D viewangles;
	int w, h;
	Interfaces::engine->get_screen_size( w, h );

	const auto screen_center = vector2d_t( w * .5f, h * .5f );
	Interfaces::engine->get_view_angles( viewangles );

	const auto angle_yaw_rad = deg2rad( viewangles.y - Math.CalcAngle( Global::Local->GetVecOrigin( ), player->GetVecOrigin( ) ).y - 90 );

	int radius = max( 10, Controls.diameter ); //distance
	int size = max( 5, Controls.size ); //size

	const auto new_point_x = screen_center.x + ( ( ( ( w - ( size * 3 ) ) * .5f ) * ( radius / 100.0f ) ) * cos( angle_yaw_rad ) ) + ( int ) ( 6.0f * ( ( ( float ) size - 4.f ) / 16.0f ) );
	const auto new_point_y = screen_center.y + ( ( ( ( h - ( size * 3 ) ) * .5f ) * ( radius / 100.0f ) ) * sin( angle_yaw_rad ) );

	auto player_on_screen = [ ] ( Vector3D origin, Vector3D& screen ) -> bool {
		if ( !Interfaces::DebugOverlay->WorldToScreen( origin, screen ) )
			return false;

		int width, height;
		Interfaces::engine->get_screen_size( width, height );

		return ( width > screen.x&& screen.x > 0 ) && ( height > screen.y&& screen.y > 0 );
	};

	Vector3D screen_pos;

	if ( Controls.fov_arrows[ 0 ] && !Controls.fov_arrows[ 1 ] ) {
		if ( !player_on_screen( player->get_absolute_origin( ), screen_pos ) )
			return;
	}
	else if ( !Controls.fov_arrows[ 0 ] && Controls.fov_arrows[ 1 ] ) {
		if ( player_on_screen( player->GetVecOrigin( ), screen_pos ) )
			return;
	}

	auto rotate_arrow = [ ] ( std::array< vector2d_t, 3 >& points, float rotation ) -> void {
		const auto points_center = ( points.at( 0 ) + points.at( 1 ) + points.at( 2 ) ) / 3;
		for ( auto& point : points ) {
			point -= points_center;

			const auto temp_x = point.x;
			const auto temp_y = point.y;

			const auto theta = deg2rad( rotation );
			const auto c = cos( theta );
			const auto s = sin( theta );

			point.x = temp_x * c - temp_y * s;
			point.y = temp_x * s + temp_y * c;

			point += points_center;
		}
	};

	auto draw_filled_triangle = [ ] ( std::array< vector2d_t, 3 > points, color col ) -> void {
		std::array< vertex_t, 3 > vertices
		{
			vertex_t( points.at( 0 ) ),
			vertex_t( points.at( 1 ) ),
			vertex_t( points.at( 2 ) )
		};
		Interfaces::Surface->DrawSetColor( col.r, col.g, col.b, col.a );
		Interfaces::Surface->draw_Textured_polygon( 3, vertices.data( ), false );
	};

	std::array< vector2d_t, 3 > points {
		vector2d_t( new_point_x - size, new_point_y - size ),
		vector2d_t( new_point_x + size, new_point_y ),
		vector2d_t( new_point_x - size, new_point_y + size )
	};

	rotate_arrow( points, viewangles.y - Math.CalcAngle( Global::Local->GetVecOrigin( ), player->GetVecOrigin( ) ).y - 90 );
	draw_filled_triangle( points, color( Controls.arrow_color.r, Controls.arrow_color.g, Controls.arrow_color.b, Controls.arrow_color.a ) );
}

void cvisuals::RunDamageIndicators( ) {
	if ( Controls.damage_indicator ) {
		float curtime = Global::Local->GetTickBase( ) * Interfaces::GlobalVarsBase->IntervalPerTick;
		for ( int i = 0; i < damage_indicator.size( ); i++ ) {

			if ( damage_indicator.at( i ).curtime < curtime ) {
				damage_indicator.erase( damage_indicator.begin( ) + i );
				continue;
			}

			if ( !damage_indicator.at( i ).init && damage_indicator.at( i ).player->IsAlive( ) ) {
				damage_indicator.at( i ).position = damage_indicator.at( i ).player->GetEyePosition( );
				damage_indicator.at( i ).init = true;
			}

			if ( curtime - damage_indicator.at( i ).update > 0.0001f ) {
				damage_indicator.at( i ).position.z -= ( 0.1f * ( curtime - damage_indicator.at( i ).curtime ) );
				damage_indicator.at( i ).update = curtime;
			}

			Vector3D w2s;
			if ( Interfaces::DebugOverlay->WorldToScreen( damage_indicator.at( i ).position, w2s ) ) {
				Render::Text( Render::Fonts::visuals, " - " + std::to_string( damage_indicator.at( i ).damage ), w2s.x, w2s.y, false, Controls.damage_indicator_color.r, Controls.damage_indicator_color.g, Controls.damage_indicator_color.b );
			}
		}
	}
}

void cvisuals::RunIndicators( ) {
	static int sw, sh;
	Interfaces::engine->get_screen_size( sw, sh );

	float eye_delta = fabsf( Global::real_angle.y - Global::fake_angle.y );
	int unclampedfakebar = eye_delta - fabsf( Global::Local->get_velocity( ).Length( ) / 300 ) * 25;
	float fake = std::clamp( unclampedfakebar, 0, 58 );
	float packets = Global::choked_commands;

	auto draw_value_bar = [ ] ( std::string name, float value, int multiplier, int width, int height, int x, int y ) -> void {
		Render::Text( Render::Fonts::menu, name, x, y, false, 255, 255, 255 );
		Render::FilledRect( x, y + 15, width, height, color( 0, 0, 0, 180 ) );

		int bar = ( value / 58 ) * multiplier;

		int clamped = std::clamp( bar, 0, width );

		Render::FilledRect( x, y + 15, clamped, 13, color( 255 - ( value * 2.55f ), value * 2.55f, 0, 255 ) );
	};

	if ( Controls.indicators ) {
		Render::RoundedRect( sw - 130, 295, 120, 120, 4, color( 0, 0, 0, 200 ) );
		Render::Text( Render::Fonts::menu, "Indicators", sw - 97, 299, false, 255, 255, 255 );
		Render::RoundedRect( sw - 120, 318, 100, 2, 1, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

		draw_value_bar( "Desync", Controls.yaw == 1 ? fake : 1, 100, 100, 13, sw - 120, 325 );
		draw_value_bar( "Choked Packets", packets, 400, 100, 13, sw - 120, 365 );

		int fps = ( int ) 1.f / Interfaces::GlobalVarsBase->frame_time;
		Render::Text( Render::Fonts::menu, "FPS: " + std::to_string( fps ), sw - 120, 398, false, 255, 255, 255 );
	}
}

void cvisuals::RunRemoveSmoke( ) {
	if ( Interfaces::engine->IsConnected( ) && Interfaces::engine->IsInGame( ) ) {
		/* remove smoke / wireframe smoke*/
		static auto linegoesthrusmoke = Utilities::pattern_scan( "client_panorama.dll", ( PBYTE ) "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx" );
		static auto smokecout = *( DWORD* ) ( linegoesthrusmoke + 0x8 );
		static std::vector< const char* > smoke_materials = {
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
			"particle/vistasmokev1/vistasmokev1_fire",
		};
		if ( Interfaces::MaterialSystem != nullptr ) {
			if ( Controls.remove_smoke >= 1 ) {
				for ( auto matName : smoke_materials ) {
					IMaterial* mat = Interfaces::MaterialSystem->get_material( matName, "Other Textures" );
					if ( mat != nullptr ) {
						mat->increment_reference_count( );
						switch ( Controls.remove_smoke ) {
						case 1: mat->set_material_var_flag( MATERIAL_VAR_NO_DRAW, true ); mat->set_material_var_flag( MATERIAL_VAR_WIREFRAME, false );  break;
						case 2: mat->set_material_var_flag( MATERIAL_VAR_WIREFRAME, true ); mat->set_material_var_flag( MATERIAL_VAR_NO_DRAW, false ); break;
						}
					}
					*( int* ) ( smokecout ) = 0;
				}
			}
			else if ( Controls.remove_smoke == 0 ) {
				for ( auto matName : smoke_materials ) {
					IMaterial* mat = Interfaces::MaterialSystem->get_material( matName, "Other Textures" );
					if ( mat != nullptr ) {
						mat->increment_reference_count( );
						mat->set_material_var_flag( MATERIAL_VAR_NO_DRAW, false );
						mat->set_material_var_flag( MATERIAL_VAR_WIREFRAME, false );
					}
				}
			}
		}
	}
}
void cvisuals::RunBombTimer( ) {
	if ( Controls.bomb_timer ) {
		for ( int i = 0; i <= 2048; i++ ) { //fuck
			C_CSPlayer* entity = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );
			if ( !entity ) continue;

			if ( entity->clientclass( )->class_id == 128 ) {
				C_CSBomb* bomb = reinterpret_cast< C_CSBomb* >( entity );

				if ( bomb ) {

					if ( bomb->m_bBombDefused( ) )
						continue;

					std::string invalid = "Bomb Time: 0.0";

					float c4_clock = bomb->m_flC4Blow( ) - Interfaces::GlobalVarsBase->CurrentTime;
					char buf[ 24 ];
					sprintf_s( buf, sizeof( buf ) - 1, "Bomb Time: %.1f", c4_clock );

					Render::Text( Render::Fonts::tab, c4_clock >= 0 ? buf : invalid, 634, 90, false, 240, 240, 240 );
					Render::FilledRect( 379, 109, 600, 12, color( 0, 0, 0, 180 ) );
					Render::FilledRect( 380, 110, c4_clock * 15, 10, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );
				}
			}
		}
	}
}
void cvisuals::RunBulletBeams() {

	if ( Controls.bullet_tracers ) {
		if ( !info.empty( ) || !Global::Local->IsAlive( ) ) {

			color bcolor = color( Controls.bullettrace_color.r, Controls.bullettrace_color.g, Controls.bullettrace_color.b, Controls.bullettrace_color.a );

			BeamInfo_t beamInfo;
			beamInfo.m_nType = TE_BEAMPOINTS;
			beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
			beamInfo.m_nModelIndex = -1;
			beamInfo.m_flHaloScale = 0.0f;
			beamInfo.m_flLife = 3.f;
			beamInfo.m_flWidth = 4.0f;
			beamInfo.m_flEndWidth = 4.0f;
			beamInfo.m_flFadeLength = 0.0f;
			beamInfo.m_flAmplitude = 2.0f;
			beamInfo.m_flBrightness = bcolor.a;
			beamInfo.m_flSpeed = 0.2f;
			beamInfo.m_nStartFrame = 0;
			beamInfo.m_flFrameRate = 0.f;
			beamInfo.m_flRed = bcolor.r;
			beamInfo.m_flGreen = bcolor.g;
			beamInfo.m_flBlue = bcolor.b;
			beamInfo.m_nSegments = 2;
			beamInfo.m_bRenderable = true;
			beamInfo.m_nFlags = 0;

			for ( size_t i = 0; i < info.size( ); i++ ) {

				beamInfo.m_vecStart = info.at( i ).start;
				beamInfo.m_vecEnd = info.at( i ).end;

				auto beam = Interfaces::RenderBeams->CreateBeamPoints( beamInfo );

				if ( beam )
					Interfaces::RenderBeams->DrawBeam( beam );

				info.erase( info.begin( ) + i );
			}
		}
	}
}

void cvisuals::RunHitmarkers( ) {

	static int sw, sh;
	Interfaces::engine->get_screen_size( sw, sh );

	if ( Controls.hitmarkers == 1 ) {
		for ( auto marker : hitmarkers ) {

			Vector3D w2s;

			if ( marker.time + 3 <= Interfaces::GlobalVarsBase->CurrentTime )
				continue;

			if ( !Interfaces::DebugOverlay->WorldToScreen( marker.postition, w2s ) )
				continue;

			Render::line( w2s.x - 8, w2s.y - 6, w2s.x - 3, w2s.y - 2, color( 255, 255, 255, hit_curtime ) );
			Render::line( w2s.x - 8, w2s.y + 7, w2s.x - 3, w2s.y + 3, color( 255, 255, 255, hit_curtime ) );
			Render::line( w2s.x + 8, w2s.y - 6, w2s.x + 3, w2s.y - 2, color( 255, 255, 255, hit_curtime ) );
			Render::line( w2s.x + 8, w2s.y + 7, w2s.x + 3, w2s.y + 3, color( 255, 255, 255, hit_curtime ) );
		}
	}
	else if ( Controls.hitmarkers == 2 ) {
		int x = sw / 2;
		int y = sh / 2;

		if ( hit_curtime > 0 ) {
			Render::line( x - 8, y - 6, x - 3, y - 2, color( 255, 255, 255, hit_curtime ) );
			Render::line( x - 8, y - 7, x - 3, y - 3, color( 255, 255, 255, hit_curtime ) );
			Render::line( x - 8, y + 7, x - 3, y + 3, color( 255, 255, 255, hit_curtime ) );
			Render::line( x - 8, y + 6, x - 3, y + 2, color( 255, 255, 255, hit_curtime ) );
			Render::line( x + 8, y - 6, x + 3, y - 2, color( 255, 255, 255, hit_curtime ) );
			Render::line( x + 8, y - 7, x + 3, y - 3, color( 255, 255, 255, hit_curtime ) );
			Render::line( x + 8, y + 7, x + 3, y + 3, color( 255, 255, 255, hit_curtime ) );
			Render::line( x + 8, y + 6, x + 3, y + 2, color( 255, 255, 255, hit_curtime ) );

			hit_curtime -= 1;
		}
	}
}