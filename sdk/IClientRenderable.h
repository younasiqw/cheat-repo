#pragma once

#include "../utilities/utilities.h"
#include "../Math/matrix.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/iclientrenderable.h

class IClientUnknown;
struct model_t;
class IClientRenderable {
public:
	// gets at the containing class...
	virtual_fn( get_client_unknown( void ), 0, IClientUnknown* ( __thiscall* )( void* ) ); // 0

	virtual_fn( get_render_origin( void ), 1, Vector3D const& ( __thiscall* )( void* ) ); // 1

	virtual_fn( get_model( void ), 8, model_t* ( __thiscall* )( void* ) ); // 0

	virtual_fn( draw_model( int flags, const int& instance ), 9, int( __thiscall* )( void*, int, const int& ), flags, instance ); // 0

	// call this to get the current bone transforms for the model.
	// currentTime parameter will affect interpolation
	// nMaxBones specifies how many matrices pBoneToWorldOut can hold. (Should be greater than or
	// equal to studiohdr_t::numbones. Use MAXSTUDIOBONES to be safe.)
	virtual_fn( setup_bones( matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float CurrentTime ), 13, bool( __thiscall* )( void*, matrix3x4_t*, int, int, float ), bone_to_world_out, max_bones, bone_mask, CurrentTime ); // 13
};