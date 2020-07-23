#pragma once
#include "../../hooks/hooks.h"
#include "../../Math/Math.h"
#include <deque>
#include <array>
#define M_PI 3.14159265358979323846

struct backtrack_record {
	C_CSPlayer* entity;
	Vector3D hitbox;
	Vector3D origin;
};

struct backtrack_tick {
	int tickcount;
	std::vector<backtrack_record> records;
};

struct lag_comp_record : public backtrack_record {
	lag_comp_record( C_CSPlayer* player, Vector3D hitbox ) {
		this->hitbox = hitbox;
		origin = player->get_absolute_origin( );
	}
};

class clagcomp {
public:
	void register_tick( );
	void begin( );
	void end( );
	void draw( );

	std::vector<backtrack_tick> ticks;

private:

	C_CSPlayer* entity;
	Vector3D previous_origin;
	matrix3x4_t* previous_matrix;

	float CalculateDist( const Vector3D& viewAngle, const Vector3D& aimAngle ) {
		Vector3D ang, aim;

		Math.AngleVectors( viewAngle, &aim );
		Math.AngleVectors( aimAngle, &ang );

		return rad2deg( acos( aim.dot( ang ) / aim.length_sqr( ) ) );
	}
};
extern clagcomp lagcomp;
extern std::array<std::deque<lag_comp_record>, 64> PlayerRecords;