#pragma once

#include "../netvar_manager/netvar_manager.h"
#include "../utilities/utilities.h"
#include "../interfaces/interfaces.h"
#include "../Math/vector3d.h"
#include "../Math/qangle.h"
#include "../Math/matrix.h"
#include "../sdk/IEngineTrace.h"
#include "../Controls/Controls.h"
#include <iostream>
#include <iomanip>
#define OFFSETPTR( type, offset )	( ( type ) ( ( std::uintptr_t ) this + ( offset ) ) )

template<typename T>
T getvfunc( const void* instance, const std::uint32_t index ) {
	if ( instance == nullptr )
		return ( T )nullptr;

	return ( T ) ( ( *( void*** ) instance )[ index ] );
}

enum MoveType_t {
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};
class collideable_t {
public:
	Vector3D& mins( ) {
		using OriginalFn = Vector3D & ( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 1 ]( this );
	}
	Vector3D& maxs( ) {
		using OriginalFn = Vector3D & ( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 2 ]( this );
	}
};

class C_BaseEntity {
public:
	IClientRenderable* renderable( ) {
		if ( this )
			return reinterpret_cast< IClientRenderable* >( reinterpret_cast< std::uintptr_t >( this ) + 0x4 );
	}

	IClientNetworkable* Networkable( ) {
		if ( this != nullptr ) {

			return reinterpret_cast< IClientNetworkable* >( reinterpret_cast< std::uintptr_t >( this ) + 0x8 );
		}
	}
	virtual_fn( is_player( void ), 157, bool( __thiscall* )( void* ) ); // 158

	int index( ) {
		using OriginalFn = int( __thiscall* )( void* );
		if ( Networkable( ) != nullptr )
			return ( *( OriginalFn** ) Networkable( ) )[ 10 ]( Networkable( ) );
	}

	ClientClass* clientclass( ) {
		using OriginalFn = ClientClass * ( __thiscall* )( void* );
		return ( *( OriginalFn** ) Networkable( ) )[ 2 ]( Networkable( ) );
	}
};

class C_BaseCombatCharacter : public C_BaseEntity {
public:
	netvar_fn( get_h_active_weapon( ), CBaseHandle, "DT_BaseCombatCharacter", "m_hActiveWeapon" );

};

class C_BasePlayer : public C_BaseCombatCharacter {
public:
	netvar_fn( get_h_vehicle( ), CBaseHandle, "DT_BasePlayer", "m_hVehicle" );
	netvar_fn( GetHealth( ), int, "DT_BasePlayer", "m_iHealth" );
	netvar_fn( get_life_state( ), int, "DT_BasePlayer", "m_lifeState" );
	netvar_fn( GetFlags( ), int, "DT_BasePlayer", "m_fFlags" );

	netvar_fn( GetTickBase( ), int, "DT_BasePlayer", "localdata", "m_nTickBase" );

	offset_fn( IsDormant( ), bool, 0xED );

	virtual_fn( think( void ), 138, void( __thiscall* )( void* ) ); // 8B C1 8B 50 40
	virtual_fn( pre_think( void ), 317, void( __thiscall* )( void* ) ); // 55 8B EC 83 E4 F8 51 56 8B F1 8B 06
	virtual_fn( post_think( void ), 318, void( __thiscall* )( void* ) ); // 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07
	virtual_fn( set_local_view_angles( Vector3D& angle ), 372, void( __thiscall* )( void*, Vector3D& ), angle ); // unk

	bool using_standard_weapons_in_vechile( void ) {
		static auto using_standard_weapons_in_vechile_fn = reinterpret_cast< bool( __thiscall* )( void* ) >( Utilities::FindPattern( "client_panorama.dll", "56 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 41" ) );
		return using_standard_weapons_in_vechile_fn( this );
	}

	bool physics_run_think( int index ) {
		static auto physics_run_think_fn = reinterpret_cast< bool( __thiscall* )( void*, int ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );
		return physics_run_think_fn( this, index );
	}

	void select_item( const char* string, int sub_type = 0 ) {
		if ( this != nullptr && string != nullptr) {
			static auto select_item_fn = reinterpret_cast< void( __thiscall* )( void*, const char*, int ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 56 8B F1 ? ? ? 85 C9 74 71 8B 06" ) );
			select_item_fn( this, string, sub_type );
		}
	}
};

class C_BaseAnimating : public C_BasePlayer {
public:
	offset_fn( get_model_ptr( ), void*, 0x294C );

	int get_bone_id( const char* name ) {
		static auto lookup_bone_fn = reinterpret_cast< int( __thiscall* )( void*, const char* ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75" ) );
		return lookup_bone_fn( this, name );
	}

	netvar_fn( m_flFrozen( ), float, "DT_BaseAnimating", "m_flFrozen" );

	Vector3D GetBonePosition( int bone_id ) { // found this old post when looking for this func - https://www.unknowncheats.me/forum/counterstrike-Global-offensive/169553-bones-dyanimically.html
		Vector3D position { }, rotation { };

		static auto get_bone_position_fn = reinterpret_cast< void( __thiscall* )( void*, int, Vector3D*, Vector3D* ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 30 8D" ) );
		get_bone_position_fn( this, bone_id, &position, &rotation );

		return position;
	}
};

class C_BaseViewModel
{
public:
	inline DWORD GetOwner( ) {
		return *( PDWORD ) ( ( DWORD ) this + 0x29CC );
	}
	inline int GetModelIndex( ) {
		return *( int* ) ( ( DWORD ) this + 0x258 );
	}
};

class C_BaseWeaponWorldModel : public C_BaseEntity {
public:
	inline int* GetModelIndex( ) {
		return ( int* ) ( ( DWORD ) this + 0x258 );
	}
};

class C_BaseCombatWeapon : public C_BaseEntity {
public:
	netvar_fn( ItemDefinitionIndex( ), int, "DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex" );

	netvar_fn( m_flNextPrimaryAttack( ), float, "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack" );

	netvar_fn( GetAmmo( ), int, "DT_BaseCombatWeapon", "m_iClip1" );

	//netvar_fn( fallback_paint_kit( ), int, "DT_BaseCombatWeapon", "m_nFallbackPaintKit" );

	//netvar_fn( item_id_high( ), int, "DT_BaseCombatWeapon", "m_iItemIDHigh" );

	offset_fn( fallback_paint_kit( ), int, 0x31B8 );

	offset_fn( item_id_high( ), int, 0x2FC0 );

	netvar_fn( get_fire_ready_time( ), float, "DT_WeaponCSBase", "m_flNextPrimaryAttack" );

	netvar_fn( m_fLastShotTime( ), float, "DT_WeaponCSBase", "m_fLastShotTime" );

	void original_owner_xuid_low( int value ) {
		static int m_OriginalOwnerXuidLow = n_netvar_tree::netvar_tree->get_offset( "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow" );
		*reinterpret_cast< int* >( uintptr_t( this ) + m_OriginalOwnerXuidLow ) = value;
	}

	void original_owner_xuid_high( int value ) {
		static int m_OriginalOwnerXuidHigh = n_netvar_tree::netvar_tree->get_offset( "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh" );
		*reinterpret_cast< int* >( uintptr_t( this ) + m_OriginalOwnerXuidHigh ) = value;
	}

	CCSWeaponData* GetWeaponData( ) {
		using OriginalFn = CCSWeaponData * ( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 460 ]( this );
	}

	float GetInaccuracy( ) {
		using OriginalFn = float( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 482 ]( this );
	}

	CBaseHandle weapon_world_model_handle ( ) {
		return *( CBaseHandle* ) ( ( uintptr_t ) this + 0x3234 );
	}

	void set_model_index( int index ) {
		using OriginalFn = void( __thiscall* )( void*, int );
		return ( *( OriginalFn** ) this )[ 75 ]( this, index );
	}

	float GetSpread( ) {
		using OriginalFn = float( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 452 ]( this );
	}

	void UpdateAccuracyPenalty( ) {
		using OriginalFn = void( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 483 ]( this );
	}
};

enum class ItemDefinitionIndex : short
{
	WEAPON_NONE = 0, WEAPON_DEAGLE,
	WEAPON_ELITE, WEAPON_FIVESEVEN,
	WEAPON_GLOCK, WEAPON_AK47 = 7,
	WEAPON_AUG, WEAPON_AWP,
	WEAPON_FAMAS, WEAPON_G3SG1,
	WEAPON_GALILAR = 13, WEAPON_M249,
	WEAPON_M4A1 = 16, WEAPON_MAC10,
	WEAPON_P90 = 19, WEAPON_ZONE_REPULSOR,
	WEAPON_MP5SD = 23, WEAPON_UMP45,
	WEAPON_XM1014, WEAPON_BIZON,
	WEAPON_MAG7, WEAPON_NEGEV,
	WEAPON_SAWEDOFF, WEAPON_TEC9,
	WEAPON_TASER, WEAPON_HKP2000,
	WEAPON_MP7, WEAPON_MP9,
	WEAPON_NOVA, WEAPON_P250,
	WEAPON_SHIELD, WEAPON_SCAR20,
	WEAPON_SG556, WEAPON_SSG08,
	WEAPON_KNIFEGG, WEAPON_KNIFE,
	WEAPON_FLASHBANG, WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE, WEAPON_MOLOTOV,
	WEAPON_DECOY, WEAPON_INCGRENADE,
	WEAPON_C4, WEAPON_HEALTHSHOT = 57, WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER, WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63, WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68, WEAPON_FISTS,
	WEAPON_BREACHCHARGE, WEAPON_TABLET = 72,
	WEAPON_MELEE = 74, WEAPON_AXE,
	WEAPON_HAMMER, WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80, WEAPON_FIREBOMB,
	WEAPON_DIVERSION, WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL, WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT, WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512, WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_CORD, WEAPON_KNIFE_CANIS,
	WEAPON_KNIFE_URSUS, WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_OUTDOOR, WEAPON_KNIFE_STILETTO,
	WEAPON_KNIFE_WIDOWMAKER, WEAPON_KNIFE_SKELETON = 525,
};
enum class c_weapon_types : int
{
	type_knife = 1,
	type_pistol,
	type_smg,
	type_rifle,
	type_shotgun,
	type_sniper,
	type_machinegun
};

class animation_layers {
public:
	char  pad_0000[ 20 ];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void* m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[ 4 ]; //0x0034
};

//interpolation.. make that run false
struct varmapentry_t {
	std::uint16_t m_type;
	std::uint16_t m_needstointerpolate;
	void* data;
	void* watcher;
};

struct varmapping_t {
	varmapentry_t* m_entries;
	int m_interpolatedentries;
	float m_lastinterptime;
};

class C_CSBomb {
public:
	netvar_fn( m_flDefuseCountDown( ), float, "DT_PlantedC4", "m_flDefuseCountDown" );
	netvar_fn( m_flDefuseLength( ), float, "DT_PlantedC4", "m_flDefuseLength" );
	netvar_fn( m_flC4Blow( ), float, "DT_PlantedC4", "m_flC4Blow" );
	netvar_fn( m_bBombDefused( ), bool, "DT_PlantedC4", "m_bBombDefused" );
};

class econ_view_item_t {
public:
	netvar_fn( is_initialized(), bool, "DT_ScriptCreatedItem", "m_bInitialized");
	netvar_fn( entity_level( ), int, "DT_ScriptCreatedItem", "m_iEntityLevel" );
	netvar_fn( account_id( ), int , "DT_ScriptCreatedItem", "m_iAccountID"  );
	netvar_fn( item_id_low( ), int, "DT_ScriptCreatedItem", "m_iItemIDLow" );

};

class base_view_model : public C_BaseEntity {
public:
	netvar_fn( model_index( ), int , "DT_BaseViewModel", "m_nModelIndex" );
	netvar_fn( view_model_index( ), int , "DT_BaseViewModel", "m_nViewModelIndex");
	netvar_fn( m_hweapon( ), int, "DT_BaseViewModel", "m_hWeapon" );
	netvar_fn( m_howner( ), int, "DT_BaseViewModel", "m_hOwner" );
};

class AttributableItem : public C_BaseEntity {
public:
	netvar_fn( model_index(), int,  "DT_BaseViewModel", "m_nModelIndex" );
	netvar_fn( item_definition_index(), short , "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	netvar_fn( original_owner_xuid_low( ), int, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow" );
	netvar_fn( original_owner_xuid_high( ), int, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh" );
	netvar_fn( fallback_stattrak( ), int, "DT_BaseAttributableItem", "m_nFallbackStatTrak" );
	netvar_fn( fallback_paint_kit( ), int, "DT_BaseAttributableItem", "m_nFallbackPaintKit" );
	netvar_fn( fallback_seed( ), int, "DT_BaseAttributableItem", "m_nFallbackSeed" );
	netvar_fn( entity_quality( ), int, "DT_BaseAttributableItem", "m_iEntityQuality" );
	netvar_fn( fallback_wear( ), float, "DT_BaseAttributableItem", "m_flFallbackWear" );
	netvar_fn( world_model_handle( ), unsigned long, "DT_BaseCombatWeapon", "m_hWeaponWorldModel" );
	netvar_fn( item_id_high( ), int,  "DT_ScriptCreatedItem", "m_iItemIDHigh" ); 
	netvar_fn( acc_id( ), int,  "DT_ScriptCreatedItem", "m_iAccountID" );
	netvar_fn( ragdoll_pos( ), Vector3D, "DT_Ragdoll", "m_ragPos" );

	void net_pre_data_update( int update_type ) {
		using OriginalFn = void( __thiscall* )( void*, int );
		return ( *( OriginalFn** ) Networkable( ) )[ 6 ]( Networkable( ), update_type );
	}

	void net_release( ) {
		using OriginalFn = void( __thiscall* )( void* );
		return ( *( OriginalFn** ) Networkable( ) )[ 1 ]( Networkable( ) );
	}

	int net_set_destroyed_on_recreate_entities( ) {
		using OriginalFn = int( __thiscall* )( void* );
		return ( *( OriginalFn** ) Networkable( ) )[ 13 ]( Networkable( ) );
	}

	void set_model_index( int index ) {
		using OriginalFn = void( __thiscall* )( void*, int );
		return ( *( OriginalFn** ) this )[ 75 ]( this, index );
	}

	base_view_model* get_view_model( ) {
		return ( base_view_model* ) ( DWORD ) this;
	}

	econ_view_item_t& item( ) {
		return *( econ_view_item_t* ) this;
	}
};

class C_CSPlayer : public C_BaseAnimating, C_CSBomb {
public:
	netvar_fn( has_helmet( ), bool, "DT_CSPlayer", "m_bHasHelmet" );
	netvar_fn( get_armor( ), int, "DT_CSPlayer", "m_ArmorValue" );
	netvar_fn( IsScoped( ), bool, "DT_CSPlayer", "m_bIsScoped" );
	netvar_fn( lower_body_yaw( ), float, "DT_CSPlayer", "m_flLowerBodyYawTarget" );
	netvar_fn( m_flNextAttack( ), float, "DT_BaseCombatCharacter", "bcc_localdata", "m_flNextAttack" );
	netvar_fn( get_velocity( ), Vector3D, "DT_BasePlayer", "localdata", "m_vecVelocity[0]" );
	netvar_fn( get_zoomlevel( ), int, "DT_WeaponCSBaseGun", "m_zoomLevel" );
	netvar_fn( duck_amount( ), int, "DT_BasePlayer", "m_flDuckAmount" );
	netvar_fn( IsImmune( ), bool, "DT_CSPlayer", "m_bGunGameImmunity" );
	netvar_fn( max_flash_alpha( ), bool, "DT_CSPlayer", "m_flFlashMaxAlpha" );
	netvar_fn( flash_duration( ), bool, "DT_CSPlayer", "m_flFlashDuration" );
	netvar_fn( view_model(), int, "DT_BasePlayer", "m_hViewModel[0]");
	netvar_fn( ragdoll_pos( ), Vector3D, "DT_Ragdoll", "m_ragPos" );
	offset_fn( GetMoveType( ), int, 0x25C );

	netvar_fn( ping( ), int, "DT_CSPlayerResource", "m_iPing" );

	collideable_t* collideable( ) {
		using OriginalFn = collideable_t * ( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 3 ]( this );
	}

	Vector3D& get_absolute_origin( ) {
		using OriginalFn = Vector3D & ( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 10 ]( this );;
	}

	Vector3D& get_abs_angles( ) {
		using OriginalFn = Vector3D & ( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 11 ]( this );
	}

	ULONG get_owner_handle( void ) {
		return *( ULONG* ) ( ( DWORD ) this + ( DWORD ) 0x00000148 );
	}

	CBaseHandle* GetWeapons( ) {
		return ( CBaseHandle* ) ( ( DWORD ) this + 0x2DE8 );
	}

	canimstate* AnimState( )  {
		return *( canimstate** ) ( ( DWORD ) this + 0x3900);
	}

	int& get_bone_count( ) {
		return *( int* ) ( ( uintptr_t ) this + 0x291C );
	}

	varmapping_t* varmap( void ) {
		return OFFSETPTR( varmapping_t*, 0x24 );
	}

	void ShouldInterp( bool interp ) {
		auto map = this->varmap( );

		if ( !map )
			return;

		for ( auto i = 0; i < map->m_interpolatedentries; i++ )
			map->m_entries[ i ].m_needstointerpolate = interp;
	}

	int get_animation_layers_count( ) {
		if ( this == nullptr )
			return 13;

		return *( int* ) ( ( DWORD ) this + 0x298C );
	}

	CUtlVector< matrix3x4_t >& cached_bone_data( ) {
		return *( CUtlVector< matrix3x4_t >* )( ( uintptr_t ) this + 0x2910 );
	}

	void SetAbsAngles( Vector3D angles ) {
		using fn = void( __thiscall* )( C_CSPlayer*, const Vector3D & angles );
		static fn abs_angles = ( fn ) ( Utilities::pattern_scan( "client_panorama.dll", ( BYTE* ) "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx" ) );

		abs_angles( this, angles );
	}

	void create_animation_state( canimstate* state ) {
		using create_anim_state_t = void( __thiscall* )( canimstate*, C_CSPlayer* );
		static auto create_anim_state = ( create_anim_state_t ) Utilities::FindPattern( "client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" );
		if ( !create_anim_state )
			return;

		create_anim_state( state, this );
	}

	Vector3D GetEyePosition( ) {
		if ( !this )
			return Vector3D( 0, 0, 0 );

		Vector3D ret;
		typedef void( __thiscall* OrigFn )( void*, Vector3D& );
		getvfunc<OrigFn>( this, 284 )( this, ret );

		return ret;
	}

	std::array<float, 24>& fl_mPoseParameter( ) {
		static int m_flPoseParameter = n_netvar_tree::netvar_tree->get_offset( "DT_BaseAnimating", "m_flPoseParameter" );
		return *( std::array<float, 24>* )( ( uintptr_t ) this + m_flPoseParameter );
	}

	void ClientSideAnimations( bool value ) {
		static int m_bClientSideAnimation = n_netvar_tree::netvar_tree->get_offset( "DT_BaseAnimating", "m_bClientSideAnimation" );
		*reinterpret_cast< bool* >( uintptr_t( this ) + m_bClientSideAnimation ) = value;
	}

	void update_client_animations( ) {
		using OriginalFn = void( __thiscall* )( void* );
		return ( *( OriginalFn** ) this )[ 223 ]( this );
	}

	void set_abs_origin( Vector3D origin ) {
		using set_abs_origin_fn = void( __thiscall* )( void*, const Vector3D & origin );
		static set_abs_origin_fn set_abs_origin;
		if ( !set_abs_origin )
			set_abs_origin = ( set_abs_origin_fn ) ( ( DWORD ) Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1" ) );
		set_abs_origin( this, origin );
	}

	float_t spawn_time( ) {
		return *( float_t* ) ( ( uintptr_t ) this + 0xA360 );
	}

	animation_layers* animations_overlays( ) {
		return *reinterpret_cast< animation_layers** >( uintptr_t( this ) + 0x2980 );
	}

	animation_layers& animation_overlay( int layer ) {
		return ( *( animation_layers** ) ( ( DWORD ) this + 0x2980 ) )[ layer ];
	}

	int get_sequence_activity( int sequence ) {
		const auto model = this->renderable( )->get_model( );
		if ( !model ) return NULL;

		auto hdr = Interfaces::ModelInfo->GetStudioModel( model );

		if ( hdr == nullptr )
			return -1;

		if ( !hdr )
			return -1;

		static auto get_sequence_activity = reinterpret_cast< int( __fastcall* )( void*, studiohdr_t*, int ) >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83" ) );

		return get_sequence_activity( this, hdr, sequence );
	}

	float get_max_desync_delta( ) {
		auto animstate = uintptr_t( this->AnimState( ) );

		float duckammount = *( float* ) ( animstate + 0xA4 );
		float speedfraction = max( 0, min( *reinterpret_cast< float* >( animstate + 0xF8 ), 1 ) );

		float speedfactor = max( 0, min( 1, *reinterpret_cast< float* > ( animstate + 0xFC ) ) );

		float unk1 = ( ( *reinterpret_cast< float* > ( animstate + 0x11C ) * -0.30000001 ) - 0.19999999 )* speedfraction;
		float unk2 = unk1 + 1.f;
		float unk3;

		if ( duckammount > 0 )
			unk2 += ( ( duckammount * speedfactor ) * ( 0.5f - unk2 ) );

		unk3 = *( float* ) ( animstate + 0x334 ) * unk2;

		return unk3;
	}


	bool WeaponNade( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;

		return weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_INCGRENADE
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_DECOY
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_FLASHBANG
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_HEGRENADE
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_SMOKEGRENADE
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_MOLOTOV;
	}

	bool weapon_knife( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		if ( WeaponData->weapon_name == "weapon_knife_ct" )
			return true;

		if ( WeaponData->weapon_name == "weapon_knife_t" )
			return true;

		return false;
	}

	bool IsScopedWeapon( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;

		return weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_AWP
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_SSG08
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_SCAR20
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_G3SG1
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_AUG
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_SG556;
	}

	bool IsKnife( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return ( WeaponData->type == ( int ) c_weapon_types::type_knife );
	}

	bool IsPistol( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return ( WeaponData->type == ( int ) c_weapon_types::type_pistol &&
			!( ( WeaponData->weapon_name == "weapon_deagle" ) || WeaponData->weapon_name == "weapon_revolver" ) );
	}

	bool IsHeavyPistol( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return WeaponData->weapon_name == "weapon_deagle" || WeaponData->weapon_name == "weapon_revolver";
	}

	bool IsSMG( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return WeaponData->type == ( int ) c_weapon_types::type_smg;
	}

	bool IsRifle( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return WeaponData->type == ( int ) c_weapon_types::type_rifle &&
			weapon->ItemDefinitionIndex( ) != ( int ) ItemDefinitionIndex::WEAPON_AWP
			&& weapon->ItemDefinitionIndex( ) != ( int ) ItemDefinitionIndex::WEAPON_SSG08
			&& weapon->ItemDefinitionIndex( ) != ( int ) ItemDefinitionIndex::WEAPON_SCAR20
			&& weapon->ItemDefinitionIndex( ) != ( int ) ItemDefinitionIndex::WEAPON_G3SG1;
	}

	bool IsSniper( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;

		return ( weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_AWP
			|| weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_SSG08 )
			&& weapon->ItemDefinitionIndex( ) != ( int ) ItemDefinitionIndex::WEAPON_SCAR20
			&& weapon->ItemDefinitionIndex( ) != ( int ) ItemDefinitionIndex::WEAPON_G3SG1;
	}

	bool IsAuto( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_SCAR20 || weapon->ItemDefinitionIndex( ) == ( int ) ItemDefinitionIndex::WEAPON_G3SG1;
	}

	bool IsMisc( ) {
		C_BaseCombatWeapon* weapon = this->GetActiveWeapon( );
		if ( !weapon ) return false;
		CCSWeaponData* WeaponData = weapon->GetWeaponData( );
		if ( !WeaponData ) return false;

		return WeaponData->type == ( int ) c_weapon_types::type_shotgun ||
			WeaponData->type == ( int ) c_weapon_types::type_machinegun;
	}

	Vector3D aim_punch_angle( ) {
		return *reinterpret_cast< Vector3D* >( uintptr_t( this ) + uintptr_t( 0x302C ) );
	}

	Vector3D view_punch_angle( ) {
		return *reinterpret_cast< Vector3D* >( uintptr_t( this ) + uintptr_t( 0x3020 ) );
	}

	netvar_fn( GetEyeAngles( ), Vector3D, "DT_CSPlayer", "m_angEyeAngles" );

	matrix3x4_t GetBoneMatrix( int bone_id ) {
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast< uintptr_t* >( uintptr_t( this ) + 0x26A8 );
		if ( offset )
			matrix = *reinterpret_cast< matrix3x4_t* >( offset + 0x30 * bone_id );

		return matrix;
	}
	C_BaseCombatWeapon* GetActiveWeapon( void ) {
		return reinterpret_cast< C_BaseCombatWeapon* >( Interfaces::EntityList->get_client_entity_from_handle( this->get_h_active_weapon( ) ) );
	}

	netvar_fn( get_simulation_time( ), int, "DT_BaseEntity", "m_flSimulationTime" );
	netvar_fn( GetVecOrigin( ), Vector3D, "DT_BaseEntity", "m_vecOrigin" );
	netvar_fn( GetTeam( ), int, "DT_BaseEntity", "m_iTeamNum" );

	bool IsAlive( ) {
		if ( !this )
			return false;

		return this->GetHealth( ) > 0;
	}
	std::string get_WeaponIcon( C_BaseCombatWeapon* weapon ) {
		int id = weapon->ItemDefinitionIndex( );
		switch ( id ) {
		case ( int ) ItemDefinitionIndex::WEAPON_DEAGLE:
		case ( int ) ItemDefinitionIndex::WEAPON_REVOLVER:
			return "F";
		case ( int ) ItemDefinitionIndex::WEAPON_ELITE:
			return "S";
		case ( int ) ItemDefinitionIndex::WEAPON_FIVESEVEN:
			return "U";
		case ( int ) ItemDefinitionIndex::WEAPON_GLOCK:
		case ( int ) ItemDefinitionIndex::WEAPON_TEC9:
			return "C";
		case ( int ) ItemDefinitionIndex::WEAPON_AK47:
			return "B";
		case ( int ) ItemDefinitionIndex::WEAPON_AUG:
			return "E";
		case ( int ) ItemDefinitionIndex::WEAPON_AWP:
			return "R";
		case ( int ) ItemDefinitionIndex::WEAPON_FAMAS:
			return "T";
		case ( int ) ItemDefinitionIndex::WEAPON_G3SG1:
		case ( int ) ItemDefinitionIndex::WEAPON_SCAR20:
			return "I";
		case ( int ) ItemDefinitionIndex::WEAPON_GALILAR:
			return "V";
		case ( int ) ItemDefinitionIndex::WEAPON_M249:
			return "Z";
		case ( int ) ItemDefinitionIndex::WEAPON_M4A1:
		case ( int ) ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
			return "W";
		case ( int ) ItemDefinitionIndex::WEAPON_MAC10:
			return "L";
		case ( int ) ItemDefinitionIndex::WEAPON_P90:
			return "M";
		case ( int ) ItemDefinitionIndex::WEAPON_UMP45:
			return "Q";
		case ( int ) ItemDefinitionIndex::WEAPON_XM1014:
			return "]";
		case ( int ) ItemDefinitionIndex::WEAPON_BIZON:
		case ( int ) ItemDefinitionIndex::WEAPON_MP9:
			return "D";
		case ( int ) ItemDefinitionIndex::WEAPON_MAG7:
		case ( int ) ItemDefinitionIndex::WEAPON_SAWEDOFF:
		case ( int ) ItemDefinitionIndex::WEAPON_NOVA:
			return "K";
		case ( int ) ItemDefinitionIndex::WEAPON_NEGEV:
			return "Z";
		case ( int ) ItemDefinitionIndex::WEAPON_USP_SILENCER:
		case ( int ) ItemDefinitionIndex::WEAPON_TASER:
		case ( int ) ItemDefinitionIndex::WEAPON_HKP2000:
		case ( int ) ItemDefinitionIndex::WEAPON_P250:
		case ( int ) ItemDefinitionIndex::WEAPON_CZ75A:
			return "Y";
		case ( int ) ItemDefinitionIndex::WEAPON_MP7:
			return "X";
		case ( int ) ItemDefinitionIndex::WEAPON_SG556:
			return "[";
		case ( int ) ItemDefinitionIndex::WEAPON_SSG08:
			return "N";
		case ( int ) ItemDefinitionIndex::WEAPON_FLASHBANG:
		case ( int ) ItemDefinitionIndex::WEAPON_DECOY:
			return "G";
		case ( int ) ItemDefinitionIndex::WEAPON_HEGRENADE:
		case ( int ) ItemDefinitionIndex::WEAPON_MOLOTOV:
		case ( int ) ItemDefinitionIndex::WEAPON_INCGRENADE:
			return "H";
		case ( int ) ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
			return "P";
		case ( int ) ItemDefinitionIndex::WEAPON_C4:
			return "\\";
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_T:
		case ( int ) ItemDefinitionIndex::WEAPON_BAYONET:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_GUT:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
		case ( int ) ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
			return "J";
		default:
			return " ";
		}
	}
	int* weapons( ) {
		return reinterpret_cast< int* > ( uintptr_t( this ) + 0x2DF8 );
	}
};


struct C_Trace
{

	bool DidHitWorld( ) const
	{
		return m_pEnt && m_pEnt->Networkable( )->EntIndex( ) == 0;
	}

	bool DidHitNonWorldEntity( ) const
	{
		return m_pEnt != 0 && !DidHitWorld( );
	}

	bool DidHit( ) const
	{
		return flFraction < 1.f;
	}

	Vector3D start;
	Vector3D end;
	cplane_t plane;
	float flFraction;
	int contents;
	WORD dispFlags;
	bool allsolid;
	bool startSolid;
	float fractionLeftSolid;
	csurface_t Surface;
	int hitGroup;
	short physicsBone;
	C_CSPlayer* m_pEnt;
	int hitbox;
};

class C_TraceFilter : public ITraceFilter
{
public:

	explicit C_TraceFilter( C_CSPlayer* entity, TraceType_t tracetype = TraceType_t::TRACE_EVERYTHING )
	{
		pSkip1 = entity;
	}

	bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
	{
		return ( pEntityHandle != pSkip1 );
	}

	TraceType_t GetTraceType( ) const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}

	void* pSkip1;
};
enum EntityFlags : int
{
	FL_ONGROUND = ( 1 << 0 ),
	FL_DUCKING = ( 1 << 1 ),
	FL_WATERJUMP = ( 1 << 2 ),
	FL_ONTRAIN = ( 1 << 3 ),
	FL_INRAIN = ( 1 << 4 ),
	FL_FROZEN = ( 1 << 5 ),
	FL_ATCONTROLS = ( 1 << 6 ),
	FL_CLIENT = ( 1 << 7 ),
	FL_FAKECLIENT = ( 1 << 8 )
};
enum
{
	CAI_BaseNPC = 0,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBRC4Target,
	CBreachCharge,
	CBreachChargeProjectile,
	CBreakableProp,
	CBreakableSurface,
	CBumpMine,
	CBumpMineProjectile,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDangerZone,
	CDangerZoneController,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDrone,
	CDronegun,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvGasCanister,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFists,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGrassBurn,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoMapRegion,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemCash,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMaterialModifyControl,
	CMelee,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParadropChopper,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPhysPropAmmoBox,
	CPhysPropLootCrate,
	CPhysPropRadarJammer,
	CPhysPropWeaponUpgrade,
	CPlantedC4,
	CPlasma,
	CPlayerPing,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropCounter,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSnowball,
	CSnowballPile,
	CSnowballProjectile,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CSurvivalSpawnChopper,
	CTablet,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponShield,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWorld,
	CWorldVguiText,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail,
	MAX_CLASSID
};