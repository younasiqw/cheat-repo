#pragma once
#include "vector3d.h"
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/mathlib/mathlib.h

// sounds good to me
//typedef float matrix3x4_t[3][4];

class matrix3x4_t
{
public:
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		flMatVal[0][0] = m00;	flMatVal[0][1] = m01; flMatVal[0][2] = m02; flMatVal[0][3] = m03;
		flMatVal[1][0] = m10;	flMatVal[1][1] = m11; flMatVal[1][2] = m12; flMatVal[1][3] = m13;
		flMatVal[2][0] = m20;	flMatVal[2][1] = m21; flMatVal[2][2] = m22; flMatVal[2][3] = m23;
	}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector3D& xAxis, const Vector3D& yAxis, const Vector3D& zAxis, const Vector3D& vecOrigin)
	{
		flMatVal[0][0] = xAxis.x; flMatVal[0][1] = yAxis.x; flMatVal[0][2] = zAxis.x; flMatVal[0][3] = vecOrigin.x;
		flMatVal[1][0] = xAxis.y; flMatVal[1][1] = yAxis.y; flMatVal[1][2] = zAxis.y; flMatVal[1][3] = vecOrigin.y;
		flMatVal[2][0] = xAxis.z; flMatVal[2][1] = yAxis.z; flMatVal[2][2] = zAxis.z; flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector3D& xAxis, const Vector3D& yAxis, const Vector3D& zAxis, const Vector3D& vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void SetOrigin(Vector3D const& p)
	{
		flMatVal[0][3] = p.x;
		flMatVal[1][3] = p.y;
		flMatVal[2][3] = p.z;
	}

	inline Vector3D GetOrigin( ) {
		return Vector3D( flMatVal[ 0 ][ 3 ],
			flMatVal[ 1 ][ 3 ],
			flMatVal[ 2 ][ 3 ] );
	}


	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				flMatVal[i][j] = std::numeric_limits<float>::infinity();;
			}
		}
	}

	float* operator[](int i) { return flMatVal[i]; }
	const float* operator[](int i) const { return flMatVal[i]; }
	float* Base() { return &flMatVal[0][0]; }
	const float* Base() const { return &flMatVal[0][0]; }

	float flMatVal[3][4];
};

struct animstate_pose_param_cache_t {
	std::uint8_t pad_0x0[ 0x4 ]; //0x0
	std::uint32_t m_idx; //0x4 
	char* m_name; //0x8 
};

class canimstate
{
public:
	std::uint8_t pad_0x0000[ 0x4 ]; //0x0000
	bool m_force_update; //0x0005 
	std::uint8_t pad_0x0006[ 0x5A ]; //0x0006
	void* m_entity; //0x0060 
	void* m_weapon; //0x0064 
	void* m_last_weapon; //0x0068 
	float m_last_clientside_anim_update; //0x006C 
	std::uint32_t m_last_clientside_anim_framecount; //0x0070 
	float m_last_clientside_anim_update_time_delta; //0x0074 
	float m_eye_yaw; //0x0078 
	float m_pitch; //0x007C 
	float m_goalfeet_yaw; //0x0080 
	float m_feet_yaw; //0x0084 
	float m_body_yaw; //0x0088 
	float m_body_yaw_clamped; //0x008C 
	float m_feet_vel_dir_delta; //0x0090 
	std::uint8_t pad_0x0094[ 0x4 ]; //0x0094
	float m_feet_cycle; //0x0098 
	float m_feet_yaw_rate; //0x009C 
	std::uint8_t pad_0x00A0[ 0x4 ]; //0x00A0
	float m_duck_amount; //0x00A4 
	float m_landing_duck_additive; //0x00A8 
	std::uint8_t pad_0x00AC[ 0x4 ]; //0x00AC
	Vector3D m_origin; //0x00B0 
	Vector3D m_old_origin; //0x00BC 
	vector2d_t m_vel2d; //0x00C8 
	std::uint8_t pad_0x00D0[ 0x10 ]; //0x00D0
	vector2d_t m_last_accelerating_vel; //0x00E0 
	std::uint8_t pad_0x00E8[ 0x4 ]; //0x00E8
	float m_speed2d; //0x00EC 
	float m_up_vel; //0x00F0 
	float m_speed_normalized; //0x00F4 
	float m_run_speed; //0x00F8 
	float m_unk_feet_speed_ratio; //0x00FC 
	float m_time_since_move; //0x0100 
	float m_time_since_stop; //0x0104 
	bool m_on_ground; //0x0108 
	bool m_hit_ground; //0x0109 
	std::uint8_t pad_0x010A[ 0x4 ]; //0x010A
	float m_time_in_air; //0x0110 
	std::uint8_t pad_0x0114[ 0x6 ]; //0x0114
	float m_ground_fraction; //0x011C 
	std::uint8_t pad_0x0120[ 0x2 ]; //0x0120
	float m_duck_rate; //0x0124 
	std::uint8_t pad_0x0128[ 0xC ]; //0x0128
	bool m_moving; //0x0134
	std::uint8_t pad_0x0135[ 0x7B ]; //0x0135
	animstate_pose_param_cache_t m_lean_yaw_pose; //0x1B0
	animstate_pose_param_cache_t m_speed_pose; //0x01BC
	animstate_pose_param_cache_t m_ladder_speed_pose; //0x01C8
	animstate_pose_param_cache_t m_ladder_yaw_pose; //0x01D4
	animstate_pose_param_cache_t m_move_yaw_pose; //0x01E0
	animstate_pose_param_cache_t m_unk_pose_pose; //0x01EC 
	animstate_pose_param_cache_t m_body_yaw_pose; //0x01F8
	animstate_pose_param_cache_t m_body_pitch_pose; //0x0204
	animstate_pose_param_cache_t m_dead_yaw_pose; //0x0210
	animstate_pose_param_cache_t m_stand_pose; //0x021C
	animstate_pose_param_cache_t m_jump_fall_pose; //0x0228
	animstate_pose_param_cache_t m_aim_blend_stand_idle_pose; //0x0234
	animstate_pose_param_cache_t m_aim_blend_crouch_idle_pose; //0x0240
	animstate_pose_param_cache_t m_strafe_yaw_pose; //0x024C
	animstate_pose_param_cache_t m_aim_blend_stand_walk_pose; //0x0258
	animstate_pose_param_cache_t m_aim_blend_stand_run_pose; //0x0264
	animstate_pose_param_cache_t m_aim_blend_crouch_walk_pose; //0x0270
	animstate_pose_param_cache_t m_move_blend_walk_pose; //0x027C
	animstate_pose_param_cache_t m_move_blend_run_pose; //0x0288
	animstate_pose_param_cache_t m_move_blend_crouch_pose; //0x0294
	std::uint8_t pad_0x02A0[ 0x4 ]; //0x02A0
	Vector3D velocity; //0x02A4 
	std::uint8_t pad_0x02A8[ 0x86 ]; //0x02A8
	float m_min_yaw; //0x0330 
	float m_max_yaw; //0x0334 
	float m_max_pitch; //0x0338 
	float m_min_pitch; //0x033C
};