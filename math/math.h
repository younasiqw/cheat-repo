#pragma once

#include "vector3d.h"
#include <assert.h>
#include "matrix.h"
#include <windows.h>
#define M_PI 3.14159265358979323846
#define M_PI_F		((float)(M_PI))
#define deg2rad( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define rad2deg(x)  ((float)(x) * (float)(180.f / M_PI_F))
#define FastSqrt(x)	(sqrt)(x)
#define square( x ) ( x * x )

class cmath {
public:
	void AngleVectors(const Vector3D& angles, Vector3D* forward, Vector3D* right, Vector3D* up)
	{
		float sp, sy, sr, cp, cy, cr;

		sin_cos(GRD_TO_BOG(angles.x), &sp, &cp);
		sin_cos(GRD_TO_BOG(angles.y), &sy, &cy);
		sin_cos(GRD_TO_BOG(angles.z), &sr, &cr);

		if (forward != nullptr)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right != nullptr)
		{
			right->x = -1 * sr * sp * cy + -1 * cr * -sy;
			right->y = -1 * sr * sp * sy + -1 * cr * cy;
			right->z = -1 * sr * cp;
		}

		if (up != nullptr)
		{
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}
	float angle_mod( float flAngle )
	{
		return( ( 360.0f / 65536.0f ) * ( ( int32_t ) ( flAngle * ( 65536.0f / 360.0f ) ) & 65535 ) );
	}

	inline float fast_sin( float x ) {
		x *= float( 0.159155 );
		x -= floor( x );
		const auto xx = x * x;
		auto y = -6.87897;
		y = y * xx + 33.7755;
		y = y * xx - 72.5257;
		y = y * xx + 80.5874;
		y = y * xx - 41.2408;
		y = y * xx + 6.28077;
		return float( x * y );
	}

	inline float fast_cos( const float x ) {
		return fast_sin( x + 1.5708f );
	}

	float approach_angle( float flTarget, float flValue, float flSpeed ) {
		float flAdjustedSpeed = flSpeed;
		if ( flAdjustedSpeed < 0.0f )
			flAdjustedSpeed *= -1.0f;

		float flAngleModTarget = angle_mod( flTarget );
		float flAngleModValue = angle_mod( flValue );

		float flDelta = ( flAngleModTarget - flAngleModValue );
		if ( flDelta >= -180.0f ) {
			if ( flDelta >= 180.0f )
				flDelta -= 360.0f;
		}
		else {
			if ( flDelta <= -180.0f )
				flDelta += 360.0f;
		}

		float flReturn;

		if ( flDelta <= flAdjustedSpeed )
		{
			if ( ( flAdjustedSpeed * -1.0f ) <= flDelta )
				flReturn = flAngleModTarget;
			else				
				flReturn = ( flAngleModValue - flAdjustedSpeed );
		}
		else
			flReturn = ( flAngleModValue + flAdjustedSpeed );

		return flReturn;
	}

	static vec_t normalize_yaw(vec_t ang)
	{
		while (ang < -180.0f)
			ang += 360.0f;
		while (ang > 180.0f)
			ang -= 360.0f;
		return ang;
	}
	Vector3D CalcAngle(const Vector3D& vecSource, const Vector3D& vecDestination) {
		Vector3D qAngles;
		Vector3D delta = Vector3D((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
		float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
		qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / M_PI));
		qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / M_PI));
		qAngles[2] = 0.f;
		if (delta[0] >= 0.f)
			qAngles[1] += 180.f;

		return qAngles;
	}
	static float RandomFloat(float flMin, float flMax) {
		static auto fn_random_float = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));
		return fn_random_float(flMin, flMax);
	}
	float DotProduct(const Vector3D& a, const Vector3D& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	static void VectorAngles(const Vector3D& vForward, Vector3D& vUp, Vector3D& vAngles)
	{
		Vector3D vLeft = Vector3D(vForward.y * vUp.z - vForward.z * vUp.y, vForward.z * vUp.x - vForward.x * vUp.z, vForward.x * vUp.y - vForward.y * vUp.x);
		vLeft.normalize();

		float flForwardDistance = vForward.Length();
		if (flForwardDistance > 0.001f)
		{
			vAngles.x = rad2deg(atan2f(-vForward.z, flForwardDistance));
			vAngles.y = rad2deg(atan2f(vForward.y, vForward.x));

			float flUpZ = (vLeft.y * vForward.x) - (vLeft.x * vForward.y);
			vAngles.z = rad2deg(atan2f(vLeft.z, flUpZ));
		}
		else
		{
			vAngles.x = rad2deg(atan2f(-vForward.z, flForwardDistance));
			vAngles.y = rad2deg(atan2f(-vLeft.x, vLeft.y));
			vAngles.z = 0;
		}
	}
	void VectorTransform(const Vector3D in1, const matrix3x4_t in2, Vector3D& out) {
		out[0] = DotProduct(in1, Vector3D(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = DotProduct(in1, Vector3D(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = DotProduct(in1, Vector3D(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	}

	void AngleVectors(const Vector3D& angles, Vector3D* forward) {
		assert(forward);

		float sp, sy, cp, cy;

		sy = sin(deg2rad(angles[1]));
		cy = cos(deg2rad(angles[1]));

		sp = sin(deg2rad(angles[0]));
		cp = cos(deg2rad(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	void NormalizeAngles(Vector3D& angles)
	{
		for (auto i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}
	void VectorAngles(const Vector3D& vForward, Vector3D& vAngles)
	{
		if (vForward.x == 0.f && vForward.y == 0.f)
		{
			vAngles.x = vForward.z > 0.f ? -90.f : 90.f;
			vAngles.y = 0.f;
		}
		else
		{
			vAngles.x = rad2deg(atan2(-vForward.z, vForward.Length()));
			vAngles.y = rad2deg(atan2(vForward.y, vForward.x));
		}

		vAngles.z = 0.f;
	}
	void clamp_angles(Vector3D& angles) {
		if (angles.y > 180.0f)
			angles.y = 180.0f;
		else if (angles.y < -180.0f)
			angles.y = -180.0f;

		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		angles.z = 0;
	}
	void sin_cos(float radian, float* sin, float* cos)
	{
		*sin = std::sin(radian);
		*cos = std::cos(radian);
	}
	float GRD_TO_BOG(float GRD) {
		return (M_PI / 180) * GRD;
	}
	vec_t vectornormalize(Vector3D& v)
	{
		vec_t l = v.Length();

		if (l != 0.0f)
		{
			v /= l;
		}
		else
		{
			v.x = v.y = 0.0f; v.z = 1.0f;
		}

		return l;
	}

	bool clamp(Vector3D& angles)
	{
		Vector3D a = angles;
		NormalizeAngles(a);
		clamp_angles(a);

		if (isnan(a.x) || isinf(a.x) ||
			isnan(a.y) || isinf(a.y) ||
			isnan(a.z) || isinf(a.z)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}
};

extern cmath Math;