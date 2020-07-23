#include "../Headers/Events.h"
#include "../Headers/Visuals.h"
cevents events;
#include <Windows.h>
#pragma comment(lib, "Winmm.lib") 

void cevents::on_attach( ) {
	weaponfire = new DamageEventListener( weapon_fire );
	Interfaces::event_manager2->AddListener( weaponfire, "weapon_fire", false );

	playerhurt = new DamageEventListener( player_hurt );
	Interfaces::event_manager2->AddListener( playerhurt, "player_hurt", false );

	bulletimpact = new DamageEventListener( bullet_impact );
	Interfaces::event_manager2->AddListener( bulletimpact, "bullet_impact", false );

	bombplanted = new DamageEventListener( bomb_planted );
	Interfaces::event_manager2->AddListener( bombplanted, "bomb_planted", false );

	itempurchase = new DamageEventListener( item_purchase );
	Interfaces::event_manager2->AddListener( itempurchase, "item_purchase", false );

	playerfootstep = new DamageEventListener( player_footstep );
	Interfaces::event_manager2->AddListener( playerfootstep, "player_footstep", false );

	playerdeath = new DamageEventListener( player_death );
	Interfaces::event_manager2->AddListener( playerdeath, "player_death", false );

	smokegrenadedetonate = new DamageEventListener( smokegrenade_detonate );
	Interfaces::event_manager2->AddListener( smokegrenadedetonate, "smokegrenade_detonate", false );
}

void cevents::on_detach( ) {
	delete weaponfire;
	delete playerhurt;
	delete bulletimpact;
	delete bombplanted;
	delete bombdefused;
	delete itempurchase;
	delete playerfootstep;
	delete playerdeath;
	delete smokegrenadedetonate;
}
void weapon_fire( IGameEvent* event ) {
	auto shooter = event->GetInt( "userid" );
	C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( Interfaces::engine->get_player_for_user_id( shooter ) ) ) );

	if ( !player)
		return;

	if ( Interfaces::engine->get_player_for_user_id( shooter ) == Interfaces::engine->get_local_player( ) ) {
		Global::MissedShots[ Global::TargetID ]++;
	}
}

void bullet_impact( IGameEvent* event ) {
	int shooter = event->GetInt( "userid" );
	C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( shooter ) ) );
	C_CSPlayer* local_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );

	if ( !player || !local_player)
		return;

	if ( Interfaces::engine->get_player_for_user_id( shooter ) == Interfaces::engine->get_local_player( ) ) {
		if ( Controls.bullet_tracers ) {
			info.emplace_back( bulletinfo( player->GetBonePosition( 8 ),
				Vector3D( event->GetFloat( "x" ), event->GetFloat( "y" ), event->GetFloat( "z" ) ), Interfaces::GlobalVarsBase->CurrentTime) );
		}

		/*C_Ray ray;
		C_Trace trace;
		C_TraceFilter filter( local_player );

		Vector3D trace_start = local_player->GetEyePosition( );
		Vector3D trace_end = Vector3D( event->GetFloat( "x" ), event->GetFloat( "y" ), event->GetFloat( "z" ) );

		Interfaces::Trace->TraceRay( C_Ray( trace_start, trace_end ), 0x46004003, &filter, &trace );

		if ( Global::player_hit ) {
			event_vec.push_back( ceventlog( "Player Hurt.", Interfaces::GlobalVarsBase->CurrentTime ) );
		}
		if ( trace.m_pEnt && trace.m_pEnt->clientclass( )->class_id == CCSPlayer ) {
			if ( !Global::player_hit ) {
				event_vec.push_back( ceventlog( "Missed shot due to resolve.", Interfaces::GlobalVarsBase->CurrentTime ));
			}
		}
		else {
			event_vec.push_back( ceventlog( "Missed shot due to spread.", Interfaces::GlobalVarsBase->CurrentTime ));
		}*/
	}
}
void bomb_planted( IGameEvent* event ) {
	int planter = event->GetInt( "userid" );
	int site_index = event->GetInt( "site" );

	player_info_t planter_info;
	Interfaces::engine->get_player_info( Interfaces::engine->get_player_for_user_id( planter ), &planter_info );

	std::string planter_name = planter_info.name;

	std::string site = "";

	if ( site_index == 0 )
		site = "A";
	else if ( site_index == 1 )
		site = "B";

	std::string output = planter_name + " planted the bomb at " + std::to_string( site_index ) + " site";
	color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );

	if ( Controls.event_logs[ 2 ] ) {
		event_vec.emplace_back( ceventlog( output, Interfaces::GlobalVarsBase->CurrentTime ) );
		Interfaces::Convar->ConsoleColorPrintf( MenuColor, "[CS:GO] " );
		output += "\n";
		Interfaces::Convar->ConsoleColorPrintf( color( 255, 255, 255 ), output.c_str( ) );
	}
}

void player_hurt( IGameEvent* event ) {
	int victim = event->GetInt( "userid" );
	int attacker = event->GetInt( "attacker" );

	/* get player information for the attacker, the victim, and Local*/
	C_CSPlayer* AttackerPlayer = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( attacker ) ) );
	C_CSPlayer* VictimPlayer = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( victim ) ) );
	C_CSPlayer* local_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );

	/* null checks*/
	if ( !AttackerPlayer || !VictimPlayer || !local_player )
		return;

	player_info_t KilledInfo;
	Interfaces::engine->get_player_info( Interfaces::engine->get_player_for_user_id( victim ), &KilledInfo );
	if ( Interfaces::engine->get_player_for_user_id( attacker ) == Interfaces::engine->get_local_player( ) ) {

		/* variables */
		Global::MissedShots[ VictimPlayer->Networkable()->EntIndex() ]--;
		std::string output = "";
		std::string hurt_name = KilledInfo.name;
		Global::player_hit = true;

		/* retrieving weapon*/
		C_BaseCombatWeapon* weapon = AttackerPlayer->GetActiveWeapon( );
		if ( !weapon ) return;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return;


		if ( Controls.capsule_overlay == 1 ) {
			color capsule_color = color( Controls.capsule_overlay_color.r, Controls.capsule_overlay_color.g, Controls.capsule_overlay_color.b, Controls.capsule_overlay_color.a );

			visuals.draw_capsule_overlay( VictimPlayer, capsule_color, 3.f );
		}

		/* retrieving hitchance*/
		float hitchance = 1 / weapon->GetInaccuracy( );

		/* setting shotlog info*/
		if ( Controls.player_info )
			shotlog.emplace_back( shot_log( hurt_name, events.hitrgoupstr( event->GetInt( "hitgroup" ) ), ( hitchance > 1000 || hitchance < 0 ) ? 0 : hitchance, event->GetString( "dmg_health" ) ) );


		/* initalizing variables for model on hit */	 
		if ( Controls.model_on_hit ) {
            matrix3x4_t matrix[ 128 ];
            matrices_s matrix_struct;
			//
            if ( VictimPlayer && VictimPlayer->renderable( )->setup_bones( matrix, 128, 0x7FF00, Interfaces::GlobalVarsBase->CurrentTime ) ) {

                memcpy( matrix_struct.matrix, matrix, sizeof( matrix ) );
                matrix_struct.flDisplayTime = Interfaces::GlobalVarsBase->CurrentTime;
                matrix_struct.entity = VictimPlayer;
                matrix_struct.origin = VictimPlayer->get_absolute_origin();
                matrix_struct.angles = VictimPlayer->get_abs_angles();
                Global::matrix[ VictimPlayer->Networkable( )->EntIndex( ) ].emplace_back( matrix_struct );
            }
        }

		/* retrieving position of hit hitgorup*/
		Vector3D hitgroup = aimbot.GetHitboxPosition( VictimPlayer, event->GetInt( "hitgroup" ) );

		storedhits.emplace_back( cstoredhits( hitgroup, VictimPlayer->get_abs_angles( ), 0, nullptr ) );

		/* setting hitmarker info */
		if ( Controls.hitmarkers == 1 )
			hitmarkers.emplace_back( chitmarkers( Vector3D( hitgroup.x, hitgroup.y, hitgroup.z ), Interfaces::GlobalVarsBase->CurrentTime ) );

		/* hitsound */
		if ( Controls.hitmarkersound > 0) {

			std::string file = Controls.custom_hitsounds.c_str();

			std::string file_location = "csgo\\sound\\" + file + ".wav";
	
			switch ( Controls.hitmarkersound ) {
			case 1: Interfaces::engine->execute_client_cmd( "play buttons\\arena_switch_press_02.wav" ); break;
			case 2: PlaySoundA( file_location.c_str(), NULL, SND_ASYNC ); break;
			}
		}

		color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );
		/* gathering info for eventlogs*/
		if ( std::stoi( event->GetString( "health" ) ) < 1 ) {

			output += "Killed " + hurt_name + " in the: " + events.hitrgoupstr( event->GetInt( "hitgroup" ) );
			Interfaces::Convar->ConsoleColorPrintf( MenuColor, "[CS:GO] " );
			output += "\n";
			Interfaces::Convar->ConsoleColorPrintf( color( 255, 255, 255 ), output.c_str( ) );

			if ( Controls.killsay.length( ) > 0 ) {
				const char* Text = Controls.killsay.c_str( );
				char buffer[ 250 ];
				sprintf_s( buffer, "say \"%s\"", Text );
				Interfaces::engine->execute_client_cmd( buffer );
			}
		}
		else {
			color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );

			output += "Hurt " + hurt_name + " for " + event->GetString( "dmg_health" ) + " in the " + events.hitrgoupstr( event->GetInt( "hitgroup" ) ) + " (" + event->GetString( "health" ) + " health remaining)";
			Interfaces::Convar->ConsoleColorPrintf( MenuColor, "[CS:GO] " );
			output += "\n";
			Interfaces::Convar->ConsoleColorPrintf( color( 255, 255, 255 ), output.c_str( ) );
		}

		/* setting event log info*/
		if ( Controls.event_logs[ 0 ] )
			event_vec.emplace_back( ceventlog( output, Interfaces::GlobalVarsBase->CurrentTime ) );

		/* initalizing nessecary dmg indicator variables*/
		damage_indicator_t dmgindicator;
		dmgindicator.damage = event->GetInt( "dmg_health" );
		dmgindicator.player = VictimPlayer;
		dmgindicator.curtime = local_player->GetTickBase( ) * Interfaces::GlobalVarsBase->IntervalPerTick + 3.f;
		dmgindicator.init = false;

		damage_indicator.emplace_back( dmgindicator );

		/* reset fade hitmarker timer*/
		hit_curtime = 255.f;
	}
}

void bomb_defused( IGameEvent* event ) {
	int defuser = event->GetInt( "userid" );

	player_info_t planter_info;
	Interfaces::engine->get_player_info( Interfaces::engine->get_player_for_user_id( defuser ), &planter_info );

	std::string planter_name = planter_info.name;

	color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );
	if ( Controls.event_logs[ 2 ] ) {
		std::string output = planter_name + " defused the bomb";
		event_vec.emplace_back( ceventlog( planter_name + " defused the bomb", Interfaces::GlobalVarsBase->CurrentTime ) );

		Interfaces::Convar->ConsoleColorPrintf( MenuColor, "[CS:GO] " );
		output += "\n";
		Interfaces::Convar->ConsoleColorPrintf( color( 255, 255, 255 ), output.c_str( ) );
	}
}

void item_purchase( IGameEvent* event ) {
	int purchaser = event->GetInt( "userid" );
	std::string weapon_bought = event->GetString( "weapon" );

	player_info_t purchaser_info;
	Interfaces::engine->get_player_info( Interfaces::engine->get_player_for_user_id( purchaser ), &purchaser_info );

	std::string purchaser_name = purchaser_info.name;
	color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );

	if ( Controls.event_logs[ 1 ] ) {
		std::string output = purchaser_name + " bought: " + weapon_bought;
		event_vec.emplace_back( ceventlog( output, Interfaces::GlobalVarsBase->CurrentTime ) );
		Interfaces::Convar->ConsoleColorPrintf( MenuColor, "[CS:GO] " );
		output += "\n";
		Interfaces::Convar->ConsoleColorPrintf( color( 255, 255, 255 ), output.c_str( ) );
	}
}

void draw_step_circle( color colors, Vector3D position ) {
	BeamInfo_t beam_info;
	beam_info.m_nType = TE_BEAMRINGPOINT;
	beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
	beam_info.m_nModelIndex = Interfaces::ModelInfo->GetModelIndex( "sprites/purplelaser1.vmt" );
	beam_info.m_nHaloIndex = -1;
	beam_info.m_flHaloScale = 5;
	beam_info.m_flLife = .75f;
	beam_info.m_flWidth = 10.f;
	beam_info.m_flFadeLength = 1.0f;
	beam_info.m_flAmplitude = 0.f;
	beam_info.m_flRed = colors.r;
	beam_info.m_flGreen = colors.g;
	beam_info.m_flBlue = colors.b;
	beam_info.m_flBrightness = colors.a;
	beam_info.m_flSpeed = 10.f;
	beam_info.m_nStartFrame = 0.f;
	beam_info.m_flFrameRate = 60.f;
	beam_info.m_nSegments = -1;
	beam_info.m_nFlags = FBEAM_SHADEIN; //FBEAM_FADEOUT
	beam_info.m_vecCenter = position + Vector3D( 0, 0, 2 );
	beam_info.m_flStartRadius = 20.f;
	beam_info.m_flEndRadius = 400.f;

	auto beam = Interfaces::RenderBeams->CreateBeamRingPoint( beam_info );
	if ( beam )
		Interfaces::RenderBeams->DrawBeam( beam );
}

void player_footstep( IGameEvent* event ) {
	int stepper = event->GetInt( "userid" );

	C_CSPlayer* stepper_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( stepper ) ) );
	C_CSPlayer* local_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );

	/* null checks*/
	if ( !stepper_player || !local_player )
		return;

	if ( stepper_player == local_player || stepper_player->GetTeam() == local_player->GetTeam())
		return;

	if ( stepper_player->IsDormant( ) || !stepper_player->IsAlive( ) )
		return;

	if ( !Controls.footstep_esp )
		return;

	//static int timer;
	//timer += 1;
	//if ( timer > 1 ) timer = 0;

	//if ( timer < 1 ) {
		footsteps.emplace_back( cfootsteps( stepper_player->get_absolute_origin( ), Interfaces::GlobalVarsBase->CurrentTime, event->GetInt( "userid" ) ) );
	//}
}
void player_death( IGameEvent* event ) {
	int victim = event->GetInt( "userid" );
	int killer = event->GetInt( "attacker" );

	/* get player information for the attacker, the victim */
	C_CSPlayer* AttackerPlayer = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( killer ) ) );
	C_CSPlayer* VictimPlayer = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_player_for_user_id( victim ) ) );

	/* null checks*/
	if ( !AttackerPlayer || !VictimPlayer ) {
		return;
	}

	player_info_t KilledInfo;
	Interfaces::engine->get_player_info( Interfaces::engine->get_player_for_user_id( victim ), &KilledInfo );

	if ( Interfaces::engine->get_player_for_user_id( victim ) == Interfaces::engine->get_local_player( ) ) {
		/* if we die, reset ALL players missed shots*/
		for ( int i = 0; i < 64; i++ ) {
			Global::MissedShots[ i ] = 0;
		}
		Global::LocalDeaths++;
	}
	else {
		if ( Interfaces::engine->get_player_for_user_id( killer ) == Interfaces::engine->get_local_player( ) ) {
			
			Global::LocalKills++;

			if ( std::stoi( event->GetString( "headshot" ) ) ) {
				Global::LocalHeadshots++;
			}

			if ( Controls.capsule_overlay == 2 ) {

				color capsule_color = color( Controls.capsule_overlay_color.r, Controls.capsule_overlay_color.g, Controls.capsule_overlay_color.b, Controls.capsule_overlay_color.a );
				visuals.draw_capsule_overlay( VictimPlayer, capsule_color, 3.f );
			}
		}

		/* if they died and it's not us, reset missed shots for them */
		Global::MissedShots[ VictimPlayer->Networkable( )->EntIndex( ) ] = 0;
	}
}

void smokegrenade_detonate( IGameEvent* event ) {
	grenadeinfo.emplace_back( grenade_info( Vector3D(event->GetFloat( "x" ), event->GetFloat( "y" ), event->GetFloat( "z" ) ), Interfaces::GlobalVarsBase->CurrentTime, "smoke" ) );
}