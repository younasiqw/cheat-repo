#pragma once
#include "../../entities/entity.h"
#include "../../sdk/IEngineTrace.h"
#include "../../Math/Math.h"
struct FireBulletData
{
	FireBulletData(const Vector3D& eyePos, C_CSPlayer* entity) : src(eyePos), filter(entity)
	{
	}

	Vector3D          src;
	C_Trace         enter_trace;
	Vector3D          direction;
	C_TraceFilter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

class Autowall
{
public:
	bool CanHitFloatingPoint(const Vector3D& point, const Vector3D& source);
	float Damage(const Vector3D& point);
	bool CanWallbang();
	float get_thickness(Vector3D& start, Vector3D& end, float distance);
	struct Autowall_Return_Info
	{
		int damage;
		int hitgroup;
		int penetration_count;
		bool did_penetrate_wall;
		float thickness;
		Vector3D end;
		C_BaseEntity* hit_entity;
	};
	Autowall_Return_Info CalculateDamage(Vector3D start, Vector3D end, C_CSPlayer* from_entity, C_CSPlayer* to_entity, int specific_hitgroup = -1);
	struct Autowall_Info
	{
		Vector3D start;
		Vector3D end;
		Vector3D current_position;
		Vector3D direction;

		ITraceFilter* filter;
		C_Trace enter_trace;

		float thickness;
		float current_damage;
		int penetration_count;
	};
	bool handle_bullet_penetration(CCSWeaponData* info, Autowall_Info& data);
	bool trace_to_exit(C_Trace* enter_trace, Vector3D start, Vector3D dir, C_Trace* exit_trace);

};

extern Autowall autowall;