#include "../Headers/Autowall.h"
#include <algorithm>
#include <psapi.h>
#include <iostream>

#define HITGROUP_GENERIC  0
#define HITGROUP_HEAD     1
#define HITGROUP_CHEST    2
#define HITGROUP_STOMACH  3
#define HITGROUP_LEFTARM  4    
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG  6
#define HITGROUP_RIGHTLEG 7

#define HITGROUP_GEAR    10

Autowall autowall;

void TraceLine( Vector3D& vecAbsStart, Vector3D& vecAbsEnd, unsigned int mask, C_CSPlayer* ignore, C_Trace* ptr )
{
	C_TraceFilter filter( ignore );
	Interfaces::Trace->TraceRay( C_Ray( vecAbsStart, vecAbsEnd ), mask, &filter, ptr );
}

bool vectortovector_visible( Vector3D src, Vector3D point )
{
	C_CSPlayer* Local = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );
	if ( !Local ) return false;

	C_Trace TraceInit;
	TraceLine( src, point, mask_solid, Local, &TraceInit );
	C_Trace Trace;
	TraceLine( src, point, mask_solid, TraceInit.m_pEnt, &Trace );

	if ( Trace.flFraction == 1.0f || TraceInit.flFraction == 1.0f )
		return true;

	return false;
}

float GetHitgroupDamageMult( int iHitGroup )
{
	switch ( iHitGroup )
	{
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	}

	return 1.0f;
}

static DWORD find_pattern2( const char* module_name, const BYTE* mask, const char* mask_string )
{
	/// Get module address
	const unsigned int module_address = reinterpret_cast< unsigned int >( GetModuleHandle( module_name ) );

	/// Get module information to the size
	MODULEINFO module_info;
	GetModuleInformation( GetCurrentProcess( ), reinterpret_cast< HMODULE >( module_address ), &module_info, sizeof( MODULEINFO ) );

	auto IsCorrectMask = [ ] ( const unsigned char* data, const unsigned char* mask, const char* mask_string ) -> bool
	{
		for ( ; *mask_string; ++mask_string, ++mask, ++data )
			if ( *mask_string == 'x' && *mask != *data )
				return false;

		return ( *mask_string ) == 0;
	};

	/// Iterate until we find a matching mask
	for ( unsigned int c = 0; c < module_info.SizeOfImage; c += 1 )
	{
		/// does it match?
		if ( IsCorrectMask( reinterpret_cast< unsigned char* >( module_address + c ), mask, mask_string ) )
			return ( module_address + c );
	}

	return 0;
}

bool HandleBulletPenetration( CCSWeaponData* wpn_data, FireBulletData& data, bool extracheck );

void ScaleDamage( int hitgroup, C_CSPlayer* enemy, float weapon_armor_ratio, float& current_damage )
{
	current_damage *= GetHitgroupDamageMult( hitgroup );

	if ( enemy->get_armor( ) > 0.0f && hitgroup < HITGROUP_LEFTLEG )
	{
		if ( hitgroup == HITGROUP_HEAD && !enemy->has_helmet( ) )
			return;

		float armorscaled = ( weapon_armor_ratio * 0.5f ) * current_damage;
		if ( ( current_damage - armorscaled ) * 0.5f > enemy->get_armor( ) )
			armorscaled = current_damage - ( enemy->get_armor( ) * 2.0f );
		current_damage = armorscaled;
	}
}

void ScaleDamage( C_CSPlayer* entity, CCSWeaponData* weapon_info, int hitgroup, float& current_damage )
{
	//Cred. to N0xius for reversing this.
	//TODO: _xAE^; look into reversing this yourself sometime

	bool hasHeavyArmor = false;
	int armorValue = entity->get_armor( );

	//Fuck making a new function, lambda beste. ~ Does the person have armor on for the hitbox checked?
	auto IsArmored = [ &entity, &hitgroup ] ( )-> bool
	{
		C_CSPlayer* targetEntity = entity;
		switch ( hitgroup )
		{
		case HITGROUP_HEAD:
			return targetEntity->has_helmet( );
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch ( hitgroup )
	{
	case HITGROUP_HEAD:
		current_damage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if ( armorValue > 0 && IsArmored( ) )
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weapon_info->armor_ratio / 2.f;

		//Damage gets modified for heavy armor users
		if ( hasHeavyArmor )
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = current_damage * armorRatio;

		if ( hasHeavyArmor )
			NewDamage *= 0.85f;

		if ( ( ( current_damage - ( current_damage * armorRatio ) ) * ( bonusValue * armorBonusRatio ) ) > armorValue )
			NewDamage = current_damage - ( armorValue / armorBonusRatio );

		current_damage = NewDamage;
	}
}

#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
//#define CHAR_TEX_UNUSED		'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
//#define CHAR_TEX_UNUSED		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.
#define   SURF_LIGHT                    0x0001
#define   SURF_HITBOX                   0x8000   
#define   SURF_NODRAW                   0x0080 
void trace_line( Vector3D& start, Vector3D& end, unsigned int mask, C_CSPlayer* ignore, C_Trace* trace ) {

	//ray.Init(start, end);

	C_TraceFilter filter( ignore );
	filter.pSkip1 = ignore;

	Interfaces::Trace->TraceRay( C_Ray( start, end ), mask, &filter, trace );
}
bool is_breakable_entity( C_CSPlayer* e ) {
	using Fn = bool( __fastcall* )( C_BaseEntity* );
	static auto fn = reinterpret_cast< Fn >( Utilities::FindPattern( "client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE" ) );

	if ( !fn )
		return 0;

	if ( !e || !e->Networkable()->EntIndex( ) )
		return false;

	auto take_damage { ( char* ) ( ( uintptr_t ) e + *( size_t* ) ( ( uintptr_t ) fn + 38 ) ) };
	auto take_damage_backup { *take_damage };

	auto* cclass = Interfaces::client->get_all_classes( );

	if ( ( cclass->network_name[ 1 ] ) != 'F'
		|| ( cclass->network_name[ 4 ] ) != 'c'
		|| ( cclass->network_name[ 5 ] ) != 'B'
		|| ( cclass->network_name[ 9 ] ) != 'h' )
		*take_damage = 2;

	bool breakable = fn( e );
	*take_damage = take_damage_backup;

	return breakable;
}
bool Autowall::trace_to_exit( C_Trace* enter_trace, Vector3D start, Vector3D dir, C_Trace* exit_trace ) {
	Vector3D end;
	float distance = 0.f;
	signed int distance_check = 23;
	int first_contents = 0;

	do {
		distance += 4.f;
		end = start + dir * distance;

		if ( !first_contents )
			first_contents = Interfaces::Trace->GetPointContents( end, mask_shot | contents_grate, NULL );

		int point_contents = Interfaces::Trace->GetPointContents( end, mask_shot | contents_grate, NULL );

		if ( !( point_contents & ( mask_shot_hull | contents_hitbox ) ) || point_contents & contents_hitbox && point_contents != first_contents ) {
			Vector3D new_end = end - ( dir * 4.f );


			//ray.Init(end, new_end);

			Interfaces::Trace->TraceRay( C_Ray( end, new_end ), mask_shot | contents_grate, nullptr, exit_trace );

			if ( exit_trace->startSolid && exit_trace->Surface.flags & SURF_HITBOX ) {
				trace_line( start, end, mask_shot_hull | contents_hitbox, exit_trace->m_pEnt, exit_trace ); //flip start and end

				if ( exit_trace->DidHit( ) && !exit_trace->startSolid )
					return true;

				continue;
			}

			if ( exit_trace->DidHit( ) && !exit_trace->startSolid ) {
				if ( enter_trace->Surface.flags & SURF_NODRAW || !( exit_trace->Surface.flags & SURF_NODRAW ) ) {
					if ( exit_trace->plane.normal.dot( dir ) <= 1.f )
						return true;

					continue;
				}

				if ( is_breakable_entity( enter_trace->m_pEnt ) && is_breakable_entity( exit_trace->m_pEnt ) )
					return true;

				continue;
			}

			if ( exit_trace->Surface.flags & SURF_NODRAW ) {
				if ( is_breakable_entity( enter_trace->m_pEnt ) && is_breakable_entity( exit_trace->m_pEnt ) )
					return true;
				else if ( !( enter_trace->Surface.flags & SURF_NODRAW ) )
					continue;
			}

			if ( ( !enter_trace->m_pEnt || enter_trace->m_pEnt->Networkable()->EntIndex( ) == 0 ) && ( is_breakable_entity( enter_trace->m_pEnt ) ) ) {
				exit_trace = enter_trace;
				exit_trace->end = start + dir;
				return true;
			}

			continue;
		}

		distance_check--;
	} while ( distance_check );

	return false;
}

bool Autowall::handle_bullet_penetration( CCSWeaponData* info, Autowall_Info& data ) {
	C_Trace trace_exit {};
	surfacedata_t* enter_surface_data = Interfaces::SurfaceData->GetSurfaceData( data.enter_trace.Surface.surfaceProps );
	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float compined_penetration_modifier = 0.f;
	bool solid_surf = ( ( data.enter_trace.contents >> 3 )& contents_solid );
	bool light_surf = ( ( data.enter_trace.Surface.flags >> 7 )& SURF_LIGHT );

	if ( data.penetration_count <= 0
		|| ( !data.penetration_count && !light_surf && !solid_surf && enter_material != CHAR_TEX_GLASS && enter_material != CHAR_TEX_GRATE )
		|| info->penetration <= 0.f
		|| !trace_to_exit( &data.enter_trace, data.enter_trace.end, data.direction, &trace_exit )
		&& !( Interfaces::Trace->GetPointContents( data.enter_trace.end, mask_shot_hull | contents_hitbox, NULL ) & ( mask_shot_hull | contents_hitbox ) ) )
		return false;

	surfacedata_t* exit_surface_data = Interfaces::SurfaceData->GetSurfaceData( trace_exit.Surface.surfaceProps );
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;

	if ( enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE ) {
		compined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	}

	else if ( light_surf || solid_surf ) {
		compined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	}
	else {
		compined_penetration_modifier = ( enter_surf_penetration_modifier + exit_surf_penetration_modifier ) * 0.5f;
		final_damage_modifier = 0.16f;
	}

	if ( enter_material == exit_material ) {
		if ( exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD )
			compined_penetration_modifier = 3.f;
		else if ( exit_material == CHAR_TEX_PLASTIC )
			compined_penetration_modifier = 2.0f;
	}

	float thickness = ( trace_exit.end - data.enter_trace.end ).length_sqr( );
	float modifier = fmaxf( 0.f, 1.f / compined_penetration_modifier );

	float lost_damage = fmaxf(
		( ( modifier * thickness ) / 24.f ) //* 0.041666668
		+ ( ( data.current_damage * final_damage_modifier )
			+ ( fmaxf( 3.75 / info->penetration, 0.f ) * 3.f * modifier ) ), 0.f );

	if ( lost_damage > data.current_damage )
		return false;

	if ( lost_damage > 0.f )
		data.current_damage -= lost_damage;

	if ( data.current_damage < 1.f )
		return false;

	data.current_position = trace_exit.end;
	data.penetration_count--;

	return true;
}
void UTIL_ClipTraceToPlayers( const Vector3D& vecAbsStart, const Vector3D& vecAbsEnd, unsigned int mask, ITraceFilter* filter, C_Trace* tr )
{
	static DWORD dwAddress = find_pattern2( "client_panorama.dll", ( BYTE* ) "\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx" );

	if ( !dwAddress )
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool SimulateFireBullet( C_CSPlayer* Local, C_BaseCombatWeapon* weapon, FireBulletData& data )
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto* wpn_data = weapon->GetWeaponData( );
	data.current_damage = ( float ) wpn_data->damage;
	while ( ( data.penetrate_count > 0 ) && ( data.current_damage >= 1.0f ) )
	{
		data.trace_length_remaining = wpn_data->range - data.trace_length;
		Vector3D End_Point = data.src + data.direction * data.trace_length_remaining;
		TraceLine( data.src, End_Point, 0x4600400B, Local, &data.enter_trace );
		UTIL_ClipTraceToPlayers( data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace );
		if ( data.enter_trace.flFraction == 1.0f )
			break;
		if ( ( data.enter_trace.hitGroup <= 7 ) && ( data.enter_trace.hitGroup > 0 ) && ( Local->GetTeam( ) != data.enter_trace.m_pEnt->GetTeam( ) ) )
		{
			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= pow( wpn_data->range_modifier, data.trace_length * 0.002 );
			ScaleDamage( data.enter_trace.hitGroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage );
			return true;
		}
		if ( !HandleBulletPenetration( wpn_data, data, false ) )
			break;
	}
	return false;
}

bool TraceToExitalt( Vector3D& end, C_Trace& tr, Vector3D start, Vector3D vEnd, C_Trace* trace )
{
	typedef bool( __cdecl* TraceToExitFn )( Vector3D&, C_Trace&, float, float, float, float, float, float, C_Trace* );
	static TraceToExitFn oTraceToExit = ( TraceToExitFn ) Utilities::FindPattern( "client_panorama.dll", "55 8B EC 83 EC 30 F3 0F 10 75" );

	if ( !oTraceToExit )
		return false;

	__asm
	{
		push trace
		push vEnd.z
		push vEnd.y
		push vEnd.x
		push start.z
		push start.y
		push start.x
		mov edx, tr
		mov ecx, end
		call oTraceToExit
		add esp, 0x1C
	}
}
#define FastSqrt(x)	(sqrt)(x)
inline float vector_length( const Vector3D& v ) {
	CHECK_VALID( v );
	return ( float ) FastSqrt( v.x * v.x + v.y * v.y + v.z * v.z );
}
static Vector3D umomaim;
bool HandleBulletPenetration( CCSWeaponData* wpn_data, FireBulletData& data, bool extracheck )
{
	surfacedata_t* enter_surface_data = Interfaces::SurfaceData->GetSurfaceData( data.enter_trace.Surface.surfaceProps );
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= pow( wpn_data->range_modifier, ( data.trace_length * 0.002 ) );
	if ( ( data.trace_length > 3000.f ) || ( enter_surf_penetration_mod < 0.1f ) )
		data.penetrate_count = 0;
	if ( data.penetrate_count <= 0 )
		return false;
	Vector3D dummy;
	C_Trace trace_exit;
	if ( !TraceToExitalt( dummy, data.enter_trace, data.enter_trace.end, data.direction, &trace_exit ) )
		return false;
	surfacedata_t* exit_surface_data = Interfaces::SurfaceData->GetSurfaceData( trace_exit.Surface.surfaceProps );
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;
	if ( ( ( data.enter_trace.contents & contents_grate ) != 0 ) || ( enter_material == 89 ) || ( enter_material == 71 ) )
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = ( enter_surf_penetration_mod + exit_surf_penetration_mod ) * 0.5f;
	if ( enter_material == exit_material )
	{
		if ( exit_material == 87 || exit_material == 85 )combined_penetration_modifier = 3.0f;
		else if ( exit_material == 76 )combined_penetration_modifier = 2.0f;
	}
	float v34 = fmaxf( 0.f, 1.0f / combined_penetration_modifier );
	float v35 = ( data.current_damage * final_damage_modifier ) + v34 * 3.0f * fmaxf( 0.0f, ( 3.0f / wpn_data->penetration ) * 1.25f );
	float thickness = vector_length( trace_exit.end - data.enter_trace.end );
	if ( extracheck ) {
		if ( !vectortovector_visible( trace_exit.end, umomaim ) )
			return false;
	}
	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;
	float lost_damage = fmaxf( 0.0f, v35 + thickness );
	if ( lost_damage > data.current_damage )
		return false;
	if ( lost_damage >= 0.0f )
		data.current_damage -= lost_damage;
	if ( data.current_damage < 1.0f )
		return false;
	data.src = trace_exit.end;
	data.penetrate_count--;

	return true;
}
inline float normalize( Vector3D& v )
{
	assert( v.is_valid( ) );
	float l = v.Length( );
	if ( l != 0.0f ) {
		v /= l;
	}
	else {
		v.x = v.y = 0.0f; v.z = 1.0f;
	}
	return l;
}
float Autowall::Damage( const Vector3D& point )
{
	C_CSPlayer* Local = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );
	if ( !Local ) return 0.f;

	auto data = FireBulletData( Local->GetEyePosition(), Local );

	Vector3D angles;
	angles = Math.CalcAngle( data.src, point );
	Math.AngleVectors( angles, &data.direction );
	Math.vectornormalize( data.direction );
	C_BaseCombatWeapon* pWeapon = Local->GetActiveWeapon( );

	if ( !pWeapon )
		return false;

	CCSWeaponData* weaponData = pWeapon->GetWeaponData( );

	if ( !weaponData )
		return false;
	data.penetrate_count = 1;
	data.trace_length = 0.0f;
	data.current_damage = ( float ) weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector3D end = data.src + data.direction * data.trace_length_remaining;

	TraceLine( data.src, end, mask_shot | contents_hitbox, Local, &data.enter_trace );

	if ( vectortovector_visible( data.src, point ) )
	{
		return true;
	}
	static bool extra_check = true;
	if ( HandleBulletPenetration( weaponData, data, extra_check ) )
	{
		return true;

	}

	if ( SimulateFireBullet( Local, Local->GetActiveWeapon( ), data ) )
		return data.current_damage;

	return 0.f;
}
Autowall::Autowall_Return_Info Autowall::CalculateDamage( Vector3D start, Vector3D end, C_CSPlayer* from_entity, C_CSPlayer* to_entity, int specific_hitgroup )
{

	// default values for return info, in case we need to return abruptly
	Autowall_Return_Info return_info;
	return_info.damage = -1;
	return_info.hitgroup = -1;
	return_info.hit_entity = nullptr;
	return_info.penetration_count = 4;
	return_info.thickness = 0.f;
	return_info.did_penetrate_wall = false;

	Autowall_Info autowall_info;
	autowall_info.penetration_count = 4;
	autowall_info.start = start;
	autowall_info.enter_trace.end = end;
	autowall_info.current_position = start;
	autowall_info.thickness = 0.f;

	C_CSPlayer* Local = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );
	if (!Local) return return_info;

	// direction 
	Math.AngleVectors( Math.CalcAngle( start, end ), &autowall_info.direction );

	// attacking entity
	if ( !from_entity )
		from_entity = Local;
	if ( !from_entity )
		return return_info;

	auto filter_player = CTraceFilterOneEntity( );
	filter_player.pEntity = to_entity;

	auto filter_local = CTraceFilter( );
	filter_local.pSkip1 = from_entity;

	// setup filters
	if ( to_entity )
		autowall_info.filter = &filter_player;
	else
		autowall_info.filter = &filter_player;

	// weapon
	C_BaseCombatWeapon* weapon = from_entity->GetActiveWeapon( );
	if ( !weapon )
		return return_info;

	// weapon data
	CCSWeaponData* weapon_info = weapon->GetWeaponData( );
	if ( !weapon_info )
		return return_info;


	// weapon range
	float range = min( weapon_info->range, ( start - end ).Length( ) );
	end = start + ( autowall_info.direction * range );
	autowall_info.current_damage = weapon_info->damage;

	while ( autowall_info.current_damage > 0 && autowall_info.penetration_count > 0 )
	{
		return_info.penetration_count = autowall_info.penetration_count;
		//g_pIVDebugOverlay->AddLineOverlay(g::pLocalEntity->GetEyePosition(), end, 0, 255, 0, true, 0.1);
		TraceLine( autowall_info.current_position, end, mask_shot | contents_grate, from_entity, &autowall_info.enter_trace );
		UTIL_ClipTraceToPlayers( autowall_info.current_position, autowall_info.current_position + autowall_info.direction * 40.f, mask_shot | contents_grate, autowall_info.filter, &autowall_info.enter_trace );

		const float distance_traced = ( autowall_info.enter_trace.end - start ).Length( );
		autowall_info.current_damage *= pow( weapon_info->range_modifier, ( distance_traced / 500.f ) );

		/// if reached the end
		if ( autowall_info.enter_trace.flFraction == 1.f )
		{
			if ( to_entity && specific_hitgroup != 0 )
			{
				ScaleDamage( to_entity, weapon_info, specific_hitgroup, autowall_info.current_damage );

				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = specific_hitgroup;
				return_info.end = autowall_info.enter_trace.end;
				return_info.hit_entity = to_entity;
			}
			else
			{
				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = -1;
				return_info.end = autowall_info.enter_trace.end;
				return_info.hit_entity = nullptr;
			}

			break;
		}
		// if hit an entity
		if ( autowall_info.enter_trace.hitGroup > 0 && autowall_info.enter_trace.hitGroup <= 7 && autowall_info.enter_trace.m_pEnt )
		{
			// checkles gg
			if ( ( to_entity && autowall_info.enter_trace.m_pEnt != to_entity ) ||
				( autowall_info.enter_trace.m_pEnt->GetTeam( ) == from_entity->GetTeam( ) ) )
			{
				return_info.damage = -1;
				return return_info;
			}

			if ( specific_hitgroup != -1 )
				ScaleDamage( autowall_info.enter_trace.m_pEnt, weapon_info, specific_hitgroup, autowall_info.current_damage );
			else
				ScaleDamage( autowall_info.enter_trace.m_pEnt, weapon_info, autowall_info.enter_trace.hitGroup, autowall_info.current_damage );

			// fill the return info
			return_info.damage = autowall_info.current_damage;
			return_info.hitgroup = autowall_info.enter_trace.hitGroup;
			return_info.end = autowall_info.enter_trace.end;
			return_info.hit_entity = autowall_info.enter_trace.m_pEnt;

			break;
		}

		// break out of the loop retard

		if ( !handle_bullet_penetration( weapon_info, autowall_info ) )
			break;

		return_info.did_penetrate_wall = true;
	}

	return_info.penetration_count = autowall_info.penetration_count;

	return return_info;
}

bool Autowall::CanWallbang( )
{
	if ( !Global::Local || !Global::Local->IsAlive())
		return false;

	FireBulletData data = FireBulletData( Global::Local->GetEyePosition(), Global::Local );
	data.filter = C_TraceFilter( Global::Local );

	Vector3D EyeAng;
	Interfaces::engine->get_view_angles( EyeAng );

	Vector3D dst, forward;

	Math.AngleVectors( EyeAng, &forward );
	dst = data.src + ( forward * 8196.f );

	Vector3D angles = Math.CalcAngle( data.src, dst );
	Math.AngleVectors( angles, &data.direction );
	Math.vectornormalize( data.direction );

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	//std::cout << "before";

	C_BaseCombatWeapon* pWeapon = Global::Local->GetActiveWeapon( );
	if ( !pWeapon ) return false;

	//std::cout << "before data";

	CCSWeaponData* weaponData = pWeapon->GetWeaponData( );
	if ( !weaponData ) return false;

	//std::cout << "after data";

	data.current_damage = ( float ) weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector3D end = data.src + data.direction * data.trace_length_remaining;

	TraceLine( data.src, end, mask_shot | contents_grate, Global::Local, &data.enter_trace );

	if ( data.enter_trace.flFraction == 1.0f )
		return false;

	if ( HandleBulletPenetration( weaponData, data, false ) )
	{
		return true;
	}

	return false;
}
bool Autowall::CanHitFloatingPoint( const Vector3D& point, const Vector3D& source ) // ez
{
	C_CSPlayer* Local = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );
	if ( !Local ) return false;

	umomaim = point;

	if ( !Local )
		return false;

	FireBulletData data = FireBulletData( source, Local );

	Vector3D angles = Math.CalcAngle( data.src, point );
	Math.AngleVectors( angles, &data.direction );
	Math.vectornormalize( data.direction );

	C_BaseCombatWeapon* pWeapon = Local->GetActiveWeapon( );

	if ( !pWeapon )
		return false;

	CCSWeaponData* weaponData = pWeapon->GetWeaponData( );

	if ( !weaponData )
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	data.current_damage = ( float ) weaponData->damage;
	data.trace_length_remaining = weaponData->range - data.trace_length;
	Vector3D end = data.src + ( data.direction * data.trace_length_remaining );
	TraceLine( data.src, end, mask_shot | contents_hitbox, Local, &data.enter_trace );

	if ( vectortovector_visible( data.src, point ) || HandleBulletPenetration( weaponData, data, true ) )
		return true;

	return false;
}
bool trace_to_exit_short( Vector3D& point, Vector3D& dir, const float step_size, float max_distance )
{
	float flDistance = 0;

	while ( flDistance <= max_distance )
	{
		flDistance += step_size;

		point += dir * flDistance;
		int point_contents = Interfaces::Trace->GetPointContents( point, mask_shot_hull );
		if ( !( point_contents & mask_shot_hull ) )
		{
			// found first free point
			return true;
		}
	}

	return false;
}
float Autowall::get_thickness( Vector3D& start, Vector3D& end, float distance ) {
	Vector3D dir = end - start;
	Vector3D step = start;
	if ( dir.Length( ) > distance&& distance != -1 )
		return -1;

	dir.NormalizeInPlace( );
	CTraceFilterWorldOnly filter;
	C_Trace trace;
	C_Ray ray( start, end );
	float thickness = 0;
	while ( true ) {
		Interfaces::Trace->TraceRay( ray, mask_shot_hull, &filter, &trace );

		if ( !trace.DidHit( ) )
			return thickness;

		const Vector3D lastStep = trace.end;
		step = trace.end;

		if ( ( end - start ).Length( ) <= ( step - start ).Length( ) )
			break;

		if ( !trace_to_exit_short( step, dir, 5, 90 ) )
			return FLT_MAX;


		thickness += ( step - lastStep ).Length( );
	}
	return thickness;
}