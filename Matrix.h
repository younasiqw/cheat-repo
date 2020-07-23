#pragma once
#include "entities/entity.h"
struct matrices_s
{
	matrix3x4_t matrix[ 128 ];
	C_CSPlayer* entity;
	Vector3D origin;
	Vector3D angles;
	bool valid;
	float flDisplayTime;
};