#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/igamemovement.h

class C_CSPlayer;
class CMoveData;

class IGameMovement {
public:
	// process the current movement command
	virtual_fn( process_movement( C_CSPlayer* player, CMoveData* move ), 1, void( __thiscall* )( void*, C_CSPlayer*, CMoveData* ), player, move ); // 1
	virtual_fn( reset( void ), 2, void( __thiscall* )( void* ) ); // 2
	virtual_fn( start_track_prediction_errors( C_CSPlayer* player ), 3, void( __thiscall* )( void*, C_CSPlayer* ), player ); // 3
	virtual_fn( finish_track_prediction_errors( C_CSPlayer* player ), 4, void( __thiscall* )( void*, C_CSPlayer* ), player ); // 4
	virtual_fn( get_player_mins ( bool ducked ), 6, Vector3D const& ( __thiscall* )( void*, bool ), ducked ); // 6
	virtual_fn( get_player_view_offset( bool ducked ), 8, Vector3D const&( __thiscall* )( void*, bool ), ducked ); // 8
};