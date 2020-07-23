#pragma once
#include "../Math/vector3d.h"
#include "../sdk/CUserCmd.h"
#include "../Math/matrix.h"
#include <vector>
#include <sstream>
#include <windows.h>
#include "../Matrix.h"
namespace n_menu {
	extern bool menu_opened;
}
namespace Global {
	extern Vector3D real_angle;
	extern Vector3D fake_angle;
	extern c_usercmd* uCmd;
	extern bool send_packet;
	extern bool save_cfg;
	extern bool load_cfg;
	extern int config;
	extern matrix3x4_t local_last_bone_matrix[ 128 ];
	extern int choked_commands;
	extern bool scene_end;
	extern int MissedShots[ 64 ];
	extern std::string resolve_mode[ 64 ];
	extern HMODULE dllmodule;
	extern int choke;
	extern bool clear_draw;
	extern bool undo_draw;

	extern int selected_player;
	extern bool player_hit;
	extern bool in_desync;
	extern bool can_damage;
	extern bool valid_hitchance;
	extern bool round_changed;
	extern bool post_screen;
	extern std::vector<matrices_s> matrix[64];
	extern Vector3D resolved_head_position[64];
	extern std::vector<Vector3D> safe_points;
	extern bool fired_shot[ 64 ];
	extern int TargetID;
	extern Vector3D aim_at_hitbox;
	extern matrix3x4_t fake_matrix[ 128 ];
	extern int Texture_id;
	extern int play_time;
	extern bool InThirdperson;
	extern bool is_before_local_chams;
	extern Vector3D local_bone_origin_delta[128];
	extern bool fakeducking;
	extern bool bullet_impact;
	extern C_CSPlayer* Local;
	extern int tick_base_shift;
	extern int stored_fake;
	extern std::vector<Vector3D> hit_origin;
	extern int LocalKills;
	extern int LocalDeaths;
	extern int LocalHeadshots;
	extern int TickCount;
	extern bool WhiteListIndex[ 64 ];
	extern bool BodyAimIndex[ 64 ];
}