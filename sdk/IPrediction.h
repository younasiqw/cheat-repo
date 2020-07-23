#pragma once

#include "../utilities/utilities.h"
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iprediction.h

class c_usercmd;
class C_CSPlayer;
class C_BasePlayer;
class IMoveHelper;
class CMoveData;

class IPrediction {
public:
	// return Local viewangles
	virtual_fn(set_local_view_angles(Vector3D& angle), 13, void(__thiscall*)(void*, Vector3D&), angle); // 13

	// yes
	virtual_fn( in_prediction( void ), 14, bool( __thiscall* )( void* ) ); // 14

	// checks if the player is standing on a moving entity and adjusts velocity and basevelocity appropriately
	virtual_fn( check_moving_ground( C_BasePlayer* player, float frame_time ), 18, void( __thiscall* )( void*, C_BasePlayer*, double ), player, frame_time ); // 18

	// call the default SetupMove code.
	virtual_fn( setup_move( C_BasePlayer* player, c_usercmd* uCmd, IMoveHelper* helper, CMoveData* move ), 20, void( __thiscall* )( void*, C_BasePlayer*, c_usercmd*, IMoveHelper*, CMoveData* ), player, uCmd, helper, move ); // 20

	// call the default FinishMove code.
	virtual_fn( finish_move( C_BasePlayer* player, c_usercmd* uCmd, CMoveData* move ), 21, void( __thiscall* )( void*, C_BasePlayer*, c_usercmd*, CMoveData* ), player, uCmd, move ); // 21
};