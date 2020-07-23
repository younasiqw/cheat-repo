#include "Controls.h"
namespace n_menu {
	bool menu_opened = false;
}
namespace Global {
	Vector3D real_angle;
	Vector3D fake_angle;
	c_usercmd* uCmd;
	bool send_packet = false;
	bool save_cfg = false;
	bool load_cfg = false;
	int config = 0;
	matrix3x4_t local_last_bone_matrix[ 128 ];
	int choked_commands = 0;
	bool scene_end = false;
	bool hit_enemy;
	int time_hit;
	int MissedShots[ 64 ];
	bool aiming[ 64 ];
	std::string resolve_mode[ 64 ];
	HMODULE dllmodule;
	int called_ticks;
	int choke;
	bool clear_draw;
	bool undo_draw;

	int selected_player = 0;
	bool player_hit = false;
	bool in_desync = false;
	bool can_damage = false;
	bool valid_hitchance = false;
	bool round_changed = false;
	bool post_screen;
	bool fired_shot[ 64 ];
	std::vector<matrices_s> matrix[ 64 ];
	Vector3D resolved_head_position[ 64 ];
	std::vector<Vector3D> safe_points;
	int TargetID;
	matrix3x4_t fake_matrix[ 128 ];
	int Texture_id;
	int play_time;
	bool InThirdperson;
	bool is_before_local_chams;
	Vector3D local_bone_origin_delta[ 128 ];
	bool fakeducking;
	bool bullet_impact; 
	C_CSPlayer* Local;
	int tick_base_shift;
	int stored_fake;
	std::vector<Vector3D> hit_origin;
	int LocalKills;
	int LocalDeaths;
	int LocalHeadshots;
	int TickCount;
	bool WhiteListIndex[ 64 ];
	bool BodyAimIndex[ 64 ];
}