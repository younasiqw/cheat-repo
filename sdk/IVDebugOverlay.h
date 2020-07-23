#pragma once

#include "../utilities/utilities.h"
#include "../Math/vector3d.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivdebugoverlay.h

class IVDebugOverlay {
public:
	virtual_fn( screen_position( Vector3D& point, Vector3D& screen ), 13, int( __thiscall* )( void*, Vector3D&, Vector3D& ), point, screen );

	bool WorldToScreen(const Vector3D& in, Vector3D& out) {
		using OriginalFn = int(__thiscall*)(IVDebugOverlay*, const Vector3D&, Vector3D&);
		int return_value = (*(OriginalFn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}

	virtual_fn( add_capsule_overlay( Vector3D& mins, Vector3D& maxs, float pillradius, int r, int g, int b, int a, float duration ), 24, void( __thiscall* )( void*, Vector3D&, Vector3D&, float&, int, int, int, int, float ), mins, maxs, pillradius, r, g, b, a, duration);

};