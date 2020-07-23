#pragma once

#include "../utilities/utilities.h"
#include "../Math/vector3d.h"
#include "../Math/qangle.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/cdll_int.h

class net_channel {
public:
	uint8_t pad_0x0000[0x17];
	bool m_bShouldDelete;
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;
};

class IVEngineClient {
public:
	// find the model's surfaces that intersect the given sphere.
	// returns the number of surfaces filled in.
	virtual_fn( get_intersecting_surfaces( const void* model, const Vector3D& center, const float radius, const bool only_visible_surfaces, void* infos, const int max_infos ), 0, int( __thiscall* )( void*, const void*, const Vector3D&, const float, const bool, void*, const int ), model, center, radius, only_visible_surfaces, infos, max_infos ); // 0

	// gets the dimensions of the game window
	virtual_fn( get_screen_size( int& width, int& height ), 5, void( __thiscall* )( void*, int&, int& ), width, height ); // 5

	// fill in the player info structure for the specified player index (name, model, etc.)
	virtual_fn( get_player_info( int ent_num, void* info ), 8, bool( __thiscall* )( void*, int, void* ), ent_num, info ); // 8

	// retrieve the player entity number for a specified userID
	virtual_fn( get_player_for_user_id( int user_id ), 9, int( __thiscall* )( void*, int ), user_id ); // 9

	// get the entity index of the Local player
	virtual_fn( get_local_player( void ), 12, int( __thiscall* )( void* ) ); // 12

	// copy current view orientation into va
	virtual_fn( get_view_angles( Vector3D& va ), 18, void( __thiscall* )( void*, Vector3D& ), va ); // 18

	// set current view orientation from va
	virtual_fn( set_view_angles( Vector3D& va ), 19, void( __thiscall* )( void*, Vector3D& ), va ); // 19

	// retrieve the current game's maxclients setting
	virtual_fn( get_max_clients( void ), 20, int( __thiscall* )( void* ) ); // 20

	// returns true if the player is fully connected and active in game (i.e, not still loading)
	virtual_fn( IsInGame( void ), 26, bool( __thiscall* )( void* ) ); // 26

	// returns true if the player is connected, but not necessarily active in game (could still be loading)
	virtual_fn( IsConnected( void ), 27, bool( __thiscall* )( void* ) ); // 27

	// gets a way to perform spatial queries on the BSP tree
	virtual_fn( get_bsp_tree_query( void ), 43, void* ( __thiscall* )( void* ) ); // 43

	// get the name of the current map
	virtual_fn( get_level_name( void ), 52, char const* ( __thiscall* )( void* ) ); // 52

	// accesses netchannel
	virtual_fn(get_net_channel(void), 78, net_channel*(__thiscall*)(void*)); // 78

	// is the game paused?
	virtual_fn( is_paused( void ), 90, bool( __thiscall* )( void* ) ); // 90

	// is this a HLTV broadcast ?
	virtual_fn( is_hltv( void ), 93, bool( __thiscall* )( void* ) ); // 93

	// current screen aspect ratio (eg. 4.0f/3.0f, 16.0f/9.0f)
	virtual_fn( get_screen_aspect_ratio( void ), 100, float( __thiscall* )( void* ) ); // 100

	// for normal console commands
	virtual_fn( execute_client_cmd( const char* uCmd_string ), 108, void( __thiscall* )( void*, const char* ), uCmd_string); // 108

	// this version does NOT check against FCVAR_CLIENTuCmd_CAN_EXECUTE.
	virtual_fn( client_uCmd_urestricted( const char* uCmd_string ), 114, void( __thiscall* )( void*, const char* ), uCmd_string ); // 114

	// check if pressing mic button
	virtual_fn(is_voice_recording( void), 224, bool(__thiscall*)(void*)); // 225
};