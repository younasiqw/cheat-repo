#pragma once
#include "../../hooks/hooks.h"
#include "autowall.h"
#include "../../Controls/Controls.h"
#include <array>
class cresolver
{
public:
	void resolve();
	void anti_freestanding( C_CSPlayer* player );
	void bruteforce_resolve( C_CSPlayer* player );
	void update_local_animation( );

	struct resolver_info {
		float resolver_data[ 64 ];
		float choked_time;
		float fake_goal_feet_yaw;

		bool use_freestand_angle[ 65 ];
		float freestand_angle[ 65 ];
		float last_freestanding_angle[ 65 ];
		bool safepoint[ 64 ];
	} info;

private:
	struct animinfo {
		animation_layers m_overlays;

	};
};

extern cresolver resolver;