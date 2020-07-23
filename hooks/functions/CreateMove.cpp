#include "../hooks.h"
#include "../../Controls/Controls.h"
#include "../../features/Headers/Engine Prediction.h"
#include <string>
#include "../../features/Headers/Ragebot.h"
#include "../../features/Headers/Misc.h"
#include "../../features/Headers/Anti-aim.h"
#include <iostream>
#include <mmsystem.h>
#include "../../features/Headers/Lagcomp.h"
#include "../../features/Headers/Legitbot.h"
#include "../../features/Headers/Resolver.h"
#define fcvar_cheat				(1<<14)

namespace n_hooks {
	void clan_tag( std::string tag ) {
		static auto set_clan_tag = reinterpret_cast< void( __fastcall* )( const char*, const char* ) >( ( ( DWORD ) ( Utilities::pattern_scan( "engine.dll", ( PBYTE ) "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx" ) ) ) );
		const char* Text = tag.c_str( );
		if ( Controls.clantag.size( ) > 0 )
			set_clan_tag( Text, Text );
		else
			set_clan_tag( "", "" );
	}

	bool __fastcall n_functions::CreateMove( std::uintptr_t ecx, std::uintptr_t edx, float input_sample_time, c_usercmd* cmd ) {
		if ( !cmd || !cmd->command_number )
			return false;

		C_CSPlayer* local_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );

		if ( !local_player )
			return false;

		uintptr_t* framePtr;
		__asm mov framePtr, ebp;

		Global::uCmd = cmd;
		Global::send_packet = true;
		Global::Local = local_player;

		if ( Interfaces::engine->IsConnected( ) && Interfaces::engine->IsInGame( ) )
		{

			Global::uCmd->buttons |= ( 1 << 22 ); // IN_BULLRUSH

			static int value = 0, value2 = 0;
			static color2 valuecol;
			if ( value != Controls.nightmode || value2 != Controls.alphaprops || valuecol != Controls.props_color ) {
				static auto r_DrawSpecificStaticProp = Interfaces::Convar->FindVar( "r_DrawSpecificStaticProp" );
				r_DrawSpecificStaticProp->nFlags &= ~fcvar_cheat;
				r_DrawSpecificStaticProp->set_value( 1 );

				for ( MaterialHandle_t i = Interfaces::MaterialSystem->first_material( ); i != Interfaces::MaterialSystem->invalid_material( ); i = Interfaces::MaterialSystem->next_material( i ) ) {
					IMaterial* material = Interfaces::MaterialSystem->get_material( i );

					if ( !material || material->is_error_material( ) )
						continue;

					if ( strstr( material->GetTextureGroupName( ), "World" ) ) {
						material->color_modulate( Controls.nightmode * 0.01f, Controls.nightmode * 0.01f, Controls.nightmode * 0.01f );
					}
					if ( strstr( material->GetTextureGroupName( ), "Model" ) ) {
						material->color_modulate( Controls.nightmode * 0.01f + .3f, Controls.nightmode * 0.01f + .3f, Controls.nightmode * 0.01f + .3f );
					}
					if ( strstr( material->GetTextureGroupName( ), "StaticProp" ) ) {
						material->color_modulate( Controls.props_color.r / 255.f, Controls.props_color.g / 255.f, Controls.props_color.b / 255.f );
						material->alpha_modulate( Controls.alphaprops * 0.01f );
					}
				}
				value = Controls.nightmode;
				value2 = Controls.alphaprops;
				valuecol = Controls.props_color;
			}

			/* data when appropriate */
			if (!Interfaces::engine->IsInGame() || !Interfaces::engine->IsConnected( ) ) {
				if ( !Global::Local->IsAlive( ) ) {
					shotlog.clear( );
					info.clear( );
					hitmarkers.clear( );
				}
				Global::LocalDeaths = 0;
				Global::LocalKills = 0;
				Global::LocalDeaths = 0;
			}

			/* run bunnyhop */
			misc.RunBunnyhop( );

			// clantag changer
			static std::string old_tag = "";
			if ( Controls.clantag.size( ) > 0 ) {
				if ( Global::send_packet && Controls.clantag != old_tag && Interfaces::GlobalVarsBase->TickCount % 3 == 0 ) {
					clan_tag( Controls.clantag );
					old_tag = Controls.clantag;
				}
			}

			/* run skybox changer */
			cconvar* sv_skyname = Interfaces::Convar->FindVar( "sv_skyname" );

			//static int skyval = 0;
			//if ( skyval != Controls.skybox ) {
				switch ( Controls.skybox ) {
				case 0: sv_skyname->set_value( sv_skyname->get_string( ) ); break;
				case 1: sv_skyname->set_value( "sky_csgo_night02" ); break; //night 1
				case 2: sv_skyname->set_value( "jungle" ); break; //jungle
				case 3: sv_skyname->set_value( "office" ); break; //office
				case 4: sv_skyname->set_value( "sky_dust" ); break; //Dust
				}
			//	skyval = Controls.skybox;
			//}
     
			static int lastDoubleTapInTickcount = 0;
			int doubletapTickcountDelta = Interfaces::GlobalVarsBase->TickCount - lastDoubleTapInTickcount;
			if ( cmd->buttons & in_attack && doubletapTickcountDelta >= time_to_ticks( 0.2f ) /*&& SETTINGS::settings.doubletap*/ )
			{
				lastDoubleTapInTickcount = Interfaces::GlobalVarsBase->TickCount;
				Global::tick_base_shift = time_to_ticks( 0.2f );
				//Global::disable_fakelag = true;
			}

			/*static int since_use = 0;
			static int delay_shot = 0;
			static int shoot_state = 2;
			static int state_delay = 0;
			static bool shoot = false;

			if ( delay_shot != 0 && shoot_state == 0 ) {
				delay_shot--;

				Global::uCmd->buttons &= ~in_attack;
				since_use = 0;

				if ( delay_shot == 0 )
					shoot_state = 2;
			}

			if ( Global::uCmd->buttons & in_attack ) {
				if ( since_use++ < 3 ) {
					Global::tick_base_shift = 14;
					delay_shot = 23;
					state_delay = 6;
					shoot_state = 1;
				}
			}

			if ( shoot_state == 1 ) {
				state_delay--;
				if ( state_delay == 0 )
				{
					Global::uCmd->buttons |= in_attack;
					shoot_state = 0;
				}
			}

			if ( state_delay <= 0 )
				state_delay = 0;
			if ( delay_shot <= 0 )
				delay_shot = 0;*/

			static bool unlocked_fakelag = false;
			if ( !unlocked_fakelag ) {
				auto cl_move_clamp = Utilities::FindPattern( _("engine.dll"), _( "B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC" ) ) + 1;
				unsigned long protect = 0;

				VirtualProtect( ( void* ) cl_move_clamp, 4, PAGE_EXECUTE_READWRITE, &protect );
				*( std::uint32_t* ) cl_move_clamp = 62;
				VirtualProtect( ( void* ) cl_move_clamp, 4, protect, &protect );
				unlocked_fakelag = true;
			}

			if ( Controls.remove_flash && Global::Local ) {
				Global::Local->flash_duration( ) = 0;
				Global::Local->max_flash_alpha( ) = 0;
			}

			/* run fakeduck */
			misc.RunFakeduck( );

			Global::TickCount++;

			/* run fakelag*/
			misc.RunFakelag( );

			if ( Global::Local ) {
				if ( !antiaim.pressing_move( ) && Global::Local->GetFlags( ) & FL_ONGROUND ) {
					float SwitchAmount = 0.f;

					auto animstate = Global::Local->AnimState( );
					if ( animstate ) {

						animstate->m_duck_amount ? SwitchAmount = 3.25f : SwitchAmount = 1.01f;

						Global::uCmd->sidemove = Global::uCmd->TickCount % 2 ? SwitchAmount : -SwitchAmount;
					}
				}
			}
			/* run movement fix*/
			misc.StartMoveFix( );

			/* run anti-aim */
			antiaim.on_move( );

			/* run triggerbot*/
			legitbot.on_move( );

			/* run buybot */
			misc.RunBuyBot( );

			/* disable post processing */
			cconvar* mat_postprocess_enable = Interfaces::Convar->FindVar( "mat_postprocess_enable" );
			mat_postprocess_enable->set_value( !Controls.disable_post_processing );

			/* fullbright */
			cconvar* mat_fullbright = Interfaces::Convar->FindVar( "mat_fullbright" );
			mat_fullbright->set_value( Controls.fullbright );

			/* run lag compensation*/
			if ( Controls.lag_compensation || Controls.legit_lagcomp ) {
				lagcomp.register_tick( );
				lagcomp.begin( );
			}

			n_engine_prediction::initialize( ); {
				/* run aimbot */
				aimbot.on_move( );
			}
			n_engine_prediction::unload( );

			if ( Controls.lag_compensation ) {
				lagcomp.end( );
			}

			/* end movefix, main bulk of code, reset vars*/
			misc.EndMoveFix( );

			auto slow_walk = [ ] ( ) -> void {
				if ( !GetAsyncKeyState( VK_SHIFT ))
					return;

				if ( !Controls.slow_walk )
					return;

				aimbot.run_autostop( 30 );
			};

			/* run slow walk, ghetto LAMBDA FTW*/
			slow_walk( );

			/* run autostop, only if movefix is over and it needs to be called*/
			if ( Global::can_damage && !Global::valid_hitchance ) {
				if ( local_player->GetFlags( ) & FL_ONGROUND ) {
					switch ( Controls.auto_stop_mode ) {
					case 1: 
						Global::uCmd->forwardmove = 0;
						Global::uCmd->sidemove = 0;
						break;
					case 2:
						aimbot.run_autostop( 30 );
						break;
					}
				}
			}

			/* run grenade prediciton */
			static bool once = false;
			if ( once != Controls.grenade_prediciton ) {
				if ( Controls.grenade_prediciton )
					Interfaces::engine->execute_client_cmd( "cl_grenadepreview 1" );
				else
					Interfaces::engine->execute_client_cmd( "cl_grenadepreview 0" );

				once = Controls.grenade_prediciton;
			}

			/* run packet updates*/
			if ( Global::send_packet ) {
				local_player->renderable( )->setup_bones( Global::local_last_bone_matrix, 128, 0x0100, Interfaces::GlobalVarsBase->CurrentTime );
			}
			else {
				Global::real_angle = cmd->viewangles;
			}

			/*static int lastDoubleTapInTickcount = 0;
			if ( Global::uCmd->buttons & in_attack )
			{

				int doubletapTickcountDelta = Interfaces::GlobalVarsBase->TickCount - lastDoubleTapInTickcount;

				if ( doubletapTickcountDelta >= time_to_ticks( 2.0f ) ) {

					lastDoubleTapInTickcount = Interfaces::GlobalVarsBase->TickCount;

					Global::tick_base_shift = time_to_ticks( 0.2f );

					std::string nibba = "Shifting Tickbase by 0.2 seconds (" + std::to_string( time_to_ticks( 0.2f ) ) + std::string( " ticks)\n" );

					Interfaces::Convar->ConsoleColorPrintf( color( 255, 0, 0 ), nibba.c_str( ) );
				}
			}*/

			Math.clamp( cmd->viewangles );
			*( bool* ) ( *framePtr - 0x1C ) = Global::send_packet;
		}

		return false;
	}
}