#include "../Headers/misc.h"
#include "../Headers/Autowall.h"
#include <sstream>
#include "../Headers/Anti-aim.h"

void cmisc::run_thirdperson(  ) {

	static bool init = false;

	if ( Controls.thirdperson && Global::Local ) {

		cconvar* sv_cheats = Interfaces::Convar->FindVar( "sv_cheats" );
		if ( GetKeyState( Controls.ThirdpersonKey ) && Global::Local->IsAlive( ) ) {
			if ( init ) {
				sv_cheats->set_value( 1 );
				Interfaces::engine->execute_client_cmd( _("thirdperson") );
			}
			init = false;
		}
		else {
			if ( !init ) {
				sv_cheats->set_value( 1 );
				Interfaces::engine->execute_client_cmd( _("firstperson") );
			}
			init = true;
		}

		cconvar* cam_dist = Interfaces::Convar->FindVar( _("cam_idealdist") );
		cam_dist->set_value( Controls.tpdistance );

		if ( GetKeyState( Controls.ThirdpersonKey ) && Global::Local->IsAlive( ) ) {
			Interfaces::prediction->set_local_view_angles( Global::real_angle );
			Global::InThirdperson = true;
		}
		else {
			Global::InThirdperson = false;
		}
	}
}
Vector3D m_oldangle;
float m_oldforward, m_oldsidemove;
void cmisc::StartMoveFix( ) {
	m_oldangle = Global::uCmd->viewangles;
	m_oldforward = Global::uCmd->forwardmove;
	m_oldsidemove = Global::uCmd->sidemove;
}

void cmisc::EndMoveFix(  ) {

	if ( !Global::Local )
		return;

	if ( Global::Local->GetMoveType( ) == MoveType_t::MOVETYPE_LADDER )
		return;

	float f1, f2, yaw_delta = Global::uCmd->viewangles.y - m_oldangle.y;
	if ( m_oldangle.y < 0.f )
		f1 = 360.0f + m_oldangle.y;
	else
		f1 = m_oldangle.y;

	if ( Global::uCmd->viewangles.y < 0.0f )
		f2 = 360.0f + Global::uCmd->viewangles.y;
	else
		f2 = Global::uCmd->viewangles.y;

	if ( f2 < f1 )
		yaw_delta = abs( f2 - f1 );
	else
		yaw_delta = 360.0f - abs( f1 - f2 );

	yaw_delta = 360.0f - yaw_delta;
	Global::uCmd->forwardmove = cos( deg2rad( yaw_delta ) ) * m_oldforward + cos( deg2rad( yaw_delta + 90.f ) ) * m_oldsidemove;
	Global::uCmd->sidemove = sin( deg2rad( yaw_delta ) ) * m_oldforward + sin( deg2rad( yaw_delta + 90.f ) ) * m_oldsidemove;
}
template<class T, class U>
inline T clamp_value( T in, U low, U high )
{
	if ( in <= low )
		return low;
	else if ( in >= high )
		return high;
	else
		return in;
}
void cmisc::RunBunnyhop() {

	if ( !Global::Local )
		return;

	if ( Controls.bunnyhop ) {

		if ( Global::Local->GetMoveType( ) == MoveType_t::MOVETYPE_LADDER )
			return;

		static bool bLastJumped = false;
		static bool bShouldFake = false;
		if ( !bLastJumped && bShouldFake ) {
			bShouldFake = false;
			Global::uCmd->buttons |= in_jump;
		}
		else if ( Global::uCmd->buttons & in_jump ) {
			if ( Global::Local->GetFlags( ) & FL_ONGROUND )
				bShouldFake = bLastJumped = true;
			else {
				Global::uCmd->buttons &= ~in_jump;
				bLastJumped = false;
			}
		}
		else
			bShouldFake = bLastJumped = false;

		if ( !( Global::Local->GetFlags( ) & FL_ONGROUND ) ) {
			float cl_sidespeed = Interfaces::Convar->FindVar( "cl_sidespeed" )->GetFloat( );
			if ( GetAsyncKeyState( 'S' ) ) {
				Global::uCmd->viewangles.y -= 180;
			}
			else if ( GetAsyncKeyState( 'D' ) ) {
				Global::uCmd->viewangles.y -= 90;
			}
			else if ( GetAsyncKeyState( 'A' ) ) {
				Global::uCmd->viewangles.y += 90;
			}
			if ( Global::Local->get_velocity( ).Length2D( ) < 0.5f || Global::Local->get_velocity( ).Length2D( ) == NAN || Global::Local->get_velocity( ).Length2D( ) == INFINITE )
			{
				Global::uCmd->forwardmove = 400;
				return;
			}

			Global::uCmd->forwardmove = clamp_value( 5850.f / Global::Local->get_velocity( ).Length2D( ), -400, 400 );
			if ( ( Global::uCmd->forwardmove < -400 || Global::uCmd->forwardmove > 400 ) )
				Global::uCmd->forwardmove = 0;

			const auto vel = Global::Local->get_velocity( );
			const float y_vel = rad2deg( atan2( vel.y, vel.x ) );
			const float diff_ang = Math.normalize_yaw( Global::uCmd->viewangles.y - y_vel );

			Global::uCmd->sidemove = ( diff_ang > 0.0 ) ? -cl_sidespeed : cl_sidespeed;
			Global::uCmd->viewangles.y = Math.normalize_yaw( Global::uCmd->viewangles.y - diff_ang );
		}
	}
}
void cmisc::RunFakelag( ) {

	if ( !Global::Local )
		return;

	if ( !Global::Local->IsAlive( ) || Interfaces::engine->is_voice_recording( ) )
		return;

	if ( Global::uCmd->buttons & in_attack && !(GetAsyncKeyState( Controls.fakeduck_key ) || Controls.fakeduck ) )
		return;

	static int choked = 0;

	if ( !Controls.fakelag_triggers[ 0 ] && ( Global::Local->GetFlags( ) & FL_ONGROUND ) )
		return;

	if ( !Controls.fakelag_triggers[ 1 ] && ( Global::uCmd->buttons & in_attack ) )
		return;

	if ( !Controls.fakelag_triggers[ 2 ] && ( !( Global::Local->GetFlags( ) & FL_ONGROUND ) ) )
		return;

	if ( Global::uCmd->buttons & in_attack ) {
		if ( !GetAsyncKeyState( Controls.fakeduck_key ) ) {
			if ( Global::Local->get_velocity( ).Length( ) < 30 )
				return;
		}
	}

	if ( choked > Controls.fakelag ) {
		Global::send_packet = true;
		choked = 0;
	}
	else {
		Global::send_packet = false;
		choked++;
	}
	Global::choked_commands = choked;

	/*if ( Controls.box ) {
		static auto unchoke = false;
		static auto onpeek_called = 0.f;
		auto net_channel = Interfaces::engine->get_net_channel( );

		if ( unchoke ) {
			Global::send_packet = true;
			unchoke = false;
			return;
		}

		if ( Global::send_packet )
			onpeek_called = 0.f;

		if ( antiaim.should_fake_lag_on_peek( ) ) {
			if ( Global::send_packet )
				return;

			if ( onpeek_called < Interfaces::GlobalVarsBase->CurrentTime )
				onpeek_called = Interfaces::GlobalVarsBase->CurrentTime + ticks_to_time( 14 );
			else if ( net_channel->m_nChokedPackets > 1 && onpeek_called < Interfaces::GlobalVarsBase->CurrentTime )
				unchoke = true;
		}
	}*/
}
void cmisc::RunFakeduck( ) {
	if ( Controls.fakeduck ) {
		auto net_channel = Interfaces::engine->get_net_channel( );

		if ( GetAsyncKeyState( Controls.fakeduck_key ) ) {

			Global::send_packet = net_channel->m_nChokedPackets >= Controls.fakelag;
			Global::uCmd->buttons |= in_bullrush;
			net_channel->m_nChokedPackets <= Controls.fakelag / 2 ? Global::uCmd->buttons &= ~in_duck : Global::uCmd->buttons |= in_duck;

		}
	}
}
void cmisc::PaintEvents( ) {
	std::string new_line = "\n";

	if ( !event_vec.empty( ) ) {
		for ( int i = event_vec.size( ) - 1; i >= 0; i-- ) {

			if ( event_vec[ i ].time + 4 < Interfaces::GlobalVarsBase->CurrentTime )
				event_vec[ i ].alpha -= 2;

			color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );

			Render::Text( Render::Fonts::main, _("[CS:GO]"), 5, 5 + ( i * 15 ), false, MenuColor.r, MenuColor.g, MenuColor.b, event_vec[ i ].alpha );

			Render::Text( Render::Fonts::main, event_vec.at( i ).name, 50, 5 + ( i * 15 ), false, 255, 255, 255, event_vec[ i ].alpha );

			if ( event_vec.at( i ).alpha < 0 ) event_vec.erase( event_vec.begin( ) + i );
			if ( event_vec.size( ) > 10 ) event_vec.erase( event_vec.begin( ) );
		}
	}
}
void cmisc::PaintShotLog( ) {
	Render::RoundedRect( 20, 295, 274, 33 + ( shotlog.size( ) * 17.5 ), 4, color( 0, 0, 0, 170 ) );

	Render::Text( Render::Fonts::menu, _("Name                Hitgroup        Hitchance          Damage"), 30, 300, false, 220, 220, 220 );

	Render::RoundedRect( 26, 320, 260, 2, 1, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

	if ( !shotlog.empty( ) ) {
		for ( int i = shotlog.size( ) - 1; i >= 0; i-- ) {

			int dmg = std::stoi( shotlog.at( i ).damage );
			Render::RoundedRect( 27, 327 + ( i * 16.5 ), 3, 10, 2, color( 255 - ( dmg * 2.55f ), dmg * 2.55f, 0, 255 ) );

			std::string name = shotlog.at( i ).name.substr( 0, 10 );

			Render::Text( Render::Fonts::menu, name, 35, 325 + ( i * 16.5 ), false, 220, 220, 220 );
			Render::Text( Render::Fonts::menu, shotlog.at( i ).hitbox, 108, 325 + ( i * 16.5 ), false, 220, 220, 220 );
			Render::Text( Render::Fonts::menu, std::to_string( shotlog.at( i ).hitchance ), 180, 325 + ( i * 16.5 ), false, 220, 220, 220 );
			Render::Text( Render::Fonts::menu, shotlog.at( i ).damage, 255, 325 + ( i * 16.5 ), false, 220, 220, 220 );

			if ( shotlog.size( ) > 8 ) shotlog.erase( shotlog.begin( ) );
		}
	}
}
void voice_record_start( const char* uncompressed_file, const char* decompressed_file, const char* mic_input_file ) {
	using voice_record_start_t = void( __fastcall* )( const char*, const char*, const char* );
	//offsets->voice_record_start = util::FindPattern( "engine.dll", "55 8B EC 83 EC 0C 83 3D ? ? ? ? ? 56 57" );
	static auto voice_record_start_fn = reinterpret_cast< voice_record_start_t >( Utilities::FindPattern( "engine.dll", "55 8B EC 83 EC 0C 83 3D ? ? ? ? ? 56 57" ) );

	__asm {
		push mic_input_file
		mov edx, decompressed_file
		mov ecx, uncompressed_file
		call voice_record_start_fn
		add esp, 0x4
	}
}

void cmisc::OnPlayerDeath( std::string file ) {
	typedef bool( __cdecl* Voice_RecordStartFn )( const char*, const char*, const char* );
	static Voice_RecordStartFn Voice_RecordStart = Voice_RecordStartFn( Utilities::FindPattern( "engine.dll", "55 8B EC 83 EC 0C 83 3D ?? ?? ?? ?? ?? 56 57" ) );

	const char* WavSound = file.c_str();

	Global::play_time = Interfaces::GlobalVarsBase->CurrentTime;

	Interfaces::engine->execute_client_cmd( _("voice_loopback 1") );
	Voice_RecordStart( WavSound, nullptr, nullptr );
}

void cmisc::RunBuyBot(  ) {
	if ( run_buybot ) {

		if ( !Global::Local )
			return;

		auto weapon = Global::Local->GetActiveWeapon( );

		if ( !weapon )
			return;

		/* these will buy whatever you have equipped at the time, very adaptive */
		if ( Controls.primary_weapon == 1 ) {
			Interfaces::engine->execute_client_cmd( _( "buy scar20;" ) );
		}
		else if ( Controls.primary_weapon == 2 ) {
			Interfaces::engine->execute_client_cmd( _( "buy m4a1;" ));
		}
		else if ( Controls.primary_weapon == 3) {
			Interfaces::engine->execute_client_cmd( _( "buy aug;" ) );
		}


		if ( Controls.secondary_weapon == 1 ) {
			Interfaces::engine->execute_client_cmd( _( "buy elite;" ) );
		}
		else if ( Controls.secondary_weapon == 2 ) {
			Interfaces::engine->execute_client_cmd( _( "buy deagle;" ) );
		}
		else if ( Controls.secondary_weapon == 3 ) {
			Interfaces::engine->execute_client_cmd( _( "buy tec9;" ) );
		}
		else if ( Controls.secondary_weapon == 3 ) {
			Interfaces::engine->execute_client_cmd( _( "buy p250") );
		}


		if ( Controls.grenade_choice[0] ) {
			Interfaces::engine->execute_client_cmd( _( "buy smokegrenade;" ) );
		}
		if ( Controls.grenade_choice[ 1 ] ) {
			Interfaces::engine->execute_client_cmd( _( "buy incgrenade;" ) );
		}
		if ( Controls.grenade_choice[ 2 ] ) {
			Interfaces::engine->execute_client_cmd( _( "buy decoy;" ) );
		}
		if ( Controls.grenade_choice[ 3 ] ) {
			Interfaces::engine->execute_client_cmd( _( "buy flashbang" ) );
		}
		if ( Controls.grenade_choice[ 4 ] ) {
			Interfaces::engine->execute_client_cmd( _( "buy hegrenade" ) );
		}

		if ( Controls.armor_choice[ 0 ] ) {
			Interfaces::engine->execute_client_cmd( _( "buy vest" ) );
		}
		if ( Controls.armor_choice[ 1 ] ) {
			Interfaces::engine->execute_client_cmd( _( "buy vesthelm" ) );
		}

		run_buybot = false;
	}
	else {
		if (!Global::Local->IsAlive() )
			run_buybot = true;
	}
}

void cmisc::RunStatsBox( ) {

	if ( !Global::Local )
		return;

	int x = 828;
	int y = 15;

	Render::RoundedRect( x, y, 274, 40, 4, color( 0, 0, 0, 170 ) );

	Render::RoundedRect( x + 6, y + 18, 260, 2, 1, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

	Render::Text( Render::Fonts::menu, "Kills          Deaths          Baim            HS            K/D", x + 10, y + 2, false, 255, 255, 255 );

	int BodyAims = Global::LocalKills - Global::LocalHeadshots;

	float KillDeath = 0.f;
	if ( Global::LocalDeaths != 0 )
		KillDeath = ( float ) Global::LocalKills / Global::LocalDeaths;
	else
		KillDeath = Global::LocalKills;

	std::string KillDeathStr = std::to_string( ( int ) KillDeath ) + "." + std::to_string( ( int ) ( ( KillDeath - ( int ) KillDeath ) * 100 ) );

	y += 24;
	x += 9;

	Render::Text( Render::Fonts::menu, std::to_string( Global::LocalKills ),                       x + 5, y, false, 255, 255, 255 ); x += 60;
	Render::Text( Render::Fonts::menu, std::to_string( Global::LocalDeaths ) ,                     x, y, false, 255, 255, 255 ); x += 60;
	Render::Text( Render::Fonts::menu, std::to_string( BodyAims ),                                 x, y, false, 255, 255, 255 ); x += 55;
	Render::Text( Render::Fonts::menu, std::to_string( Global::LocalHeadshots ),                   x, y, false, 255, 255, 255 ); x += 50;
	Render::Text( Render::Fonts::menu, KillDeathStr,                                               x, y, false, 255, 255, 255 ); x += 85;
}
cmisc misc;
std::vector<ceventlog> event_vec;