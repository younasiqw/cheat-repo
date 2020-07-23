#pragma once
#include "../../hooks/hooks.h"
#include "../../menu/config.h"
#include "../../sdk/ClientClass.h"
#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

class c_skinchanger {
public:
	bool apply_knife_model( AttributableItem* weapon, const char* model ) noexcept;
	bool apply_knife_skin( AttributableItem* weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear ) noexcept;
	void run( ) noexcept;

	struct hud_weapons_t {
		std::int32_t* get_weapon_count( ) {
			return reinterpret_cast< std::int32_t* >( std::uintptr_t( this ) + 0x80 );
		}
	};

	template<class T>
	static T* find_hud_element( const char* name ) {
		static auto fn = *reinterpret_cast< DWORD** >( Utilities::FindPattern( "client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08" ) + 1 );

		static auto find_hud_element = reinterpret_cast< DWORD( __thiscall* )( void*, const char* ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
		return ( T* ) find_hud_element( fn, name );
	}

	typedef void( *CL_FullUpdate_t ) ( void );
	CL_FullUpdate_t CL_FullUpdate = nullptr;

	int get_knife_model_indicies( ) {
		if ( !Global::Local )
			return 59;

		switch ( Controls.knife_model ) {
			case 0: return Global::Local->GetTeam( ) == 1 ? 42 : 26; break;
			case 1: return 508; break;
			case 2: return 500; break;
			case 3: return 505; break;
			case 4: return 506; break;
			case 5: return 507; break;
			case 6: return 509;  break;
			case 7: return 512; break;
			case 8: return 514; break;
			case 9: return 515; break;
			case 10: return 516; break;
			case 11: return 520; break;
			case 12: return 522; break;
			case 13: return 519; break;
			case 14: return 523; break;
			case 15: return 503; break;
			case 16: return 517; break;
			case 17: return 518; break;
			case 18: return 521; break;
			default: return 525; break;
		}
	}
	const char* get_knife_model( bool viewmodel = true ) {
		if ( !Global::Local )
			return "models/weapons/v_knife_default_t.mdl";

		switch ( Controls.knife_model ) {
		case 0: return viewmodel ? Global::Local->GetTeam( ) == 2 ? "models/weapons/v_knife_default_t.mdl" : "models/weapons/v_knife_default_ct.mdl" : Global::Local->GetTeam( ) == 2 ? "models/weapons/w_knife_default_t.mdl" : "models/weapons/w_knife_default_ct.mdl"; break;
		case 1: return viewmodel ? "models/weapons/v_knife_m9_bay.mdl" : "models/weapons/w_knife_m9_bay.mdl"; break;
		case 2: return viewmodel ? "models/weapons/v_knife_bayonet.mdl" : "models/weapons/w_knife_bayonet.mdl"; break;
		case 3: return viewmodel ? "models/weapons/v_knife_flip.mdl" : "models/weapons/w_knife_flip.mdl"; break;
		case 4: return viewmodel ? "models/weapons/v_knife_gut.mdl" : "models/weapons/w_knife_gut.mdl"; break;
		case 5: return viewmodel ? "models/weapons/v_knife_karam.mdl" : "models/weapons/w_knife_karam.mdl"; break;
		case 6: return viewmodel ? "models/weapons/v_knife_tactical.mdl" : "models/weapons/w_knife_tactical.mdl"; break;
		case 7: return viewmodel ? "models/weapons/v_knife_falchion_advanced.mdl" : "models/weapons/w_knife_falchion_advanced.mdl"; break;
		case 8: return viewmodel ? "models/weapons/v_knife_survival_bowie.mdl" : "models/weapons/w_knife_survival_bowie.mdl"; break;
		case 9: return viewmodel ? "models/weapons/v_knife_butterfly.mdl" : "models/weapons/w_knife_butterfly.mdl"; break;
		case 10: return viewmodel ? "models/weapons/v_knife_push.mdl" : "models/weapons/w_knife_push.mdl"; break;
		case 11: return viewmodel ? "models/weapons/v_knife_gypsy_jackknife.mdl" : "models/weapons/w_knife_gypsy_jackknife.mdl"; break;
		case 12: return viewmodel ? "models/weapons/v_knife_stiletto.mdl" : "models/weapons/w_knife_stiletto.mdl"; break;
		case 13: return viewmodel ? "models/weapons/v_knife_ursus.mdl" : "models/weapons/w_knife_ursus.mdl"; break;
		case 14: return viewmodel ? "models/weapons/v_knife_widowmaker.mdl" : "models/weapons/2_knife_widowmaker.mdl"; break;
		case 15: return viewmodel ? "models/weapons/v_knife_css.mdl" : "models/weapons/w_knife_css.mdl"; break;
		case 16: return viewmodel ? "models/weapons/v_knife_cord.mdl" : "models/weapons/w_knife_cord.mdl"; break;
		case 17: return viewmodel ? "models/weapons/v_knife_canis.mdl" : "models/weapons/v_knife_canis.mdl"; break;
		case 18: return viewmodel ? "models/weapons/v_knife_outdoor.mdl" : "models/weapons/w_knife_outdoor.mdl"; break;
		case 19: return viewmodel ? "models/weapons/v_knife_skeleton.mdl" : "models/weapons/w_knife_skeleton.mdl"; break;
		default: return ""; break;
		}
	}

	void run_skin_changer( );

	void force_full_update( ) {
		static auto fn = reinterpret_cast< std::int32_t( __thiscall* )( void*, std::int32_t ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C" ) );
		auto element = find_hud_element<std::uintptr_t*>( ( "CCSGO_HudWeaponSelection" ) );

		auto hud_weapons = reinterpret_cast< hud_weapons_t* >( std::uintptr_t( element ) - 0xA0 );
		if ( hud_weapons == nullptr )
			return;

		if ( !*hud_weapons->get_weapon_count( ) )
			return;

		for ( std::int32_t i = 0; i < *hud_weapons->get_weapon_count( ); i++ )
			i = fn( hud_weapons, i );

		static auto CL_FullUpdate = reinterpret_cast< CL_FullUpdate_t >( Utilities::pattern_scan( "engine.dll", ( PBYTE ) ( "\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85" ), "x????x????xxxxxxx" ) );
		CL_FullUpdate( );
	}

	void apply_sequence_update( );
};
extern c_skinchanger skin_changer;