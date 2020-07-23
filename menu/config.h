#pragma once
#include "../Controls/Controls.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../entities/entity.h"
#include "../features/Headers/misc.h"
#include "menu.h"
#include <map>
enum {
	AUTO = 0,
	RIFLE,
	SNIPER,
	PISTOL,
	SMG,
	HEAVY_PISTOL,
	MISC,
};

class config_t {
public:
	bool visuals_active = false;
	bool health = false;
	bool name = false;
	int weapon = 0;
	bool box = false;
	int chams = 0;
	bool teamcheck = false;;
	bool fakelag_chams = false;
	bool compass = false;
	int viewmodel = 0;
	bool drawmenu = false;
	bool autowallcross = false;
	bool thirdperson = false;
	int tpdistance = 120;
	bool bunnyhop = false;
	int nightmode = 100;
	int fakelag = 0;
	bool playerlist = false;
	int fov = 0;
	bool autoshoot = false;
	int mindamage = 0;
	int hitchance = 0;
	int aim_type = 0;
	bool antiaim = false;
	int pitch = 0;
	int yaw = 0;
	bool autostop = false;
	bool bullet_tracers = false;
	int alphaprops = 100;
	bool info = false;
	bool aim_active = false;
	bool glow = false;
	color2 glow_color = { 255, 255, 255, 255 };
	color2 chams_color = { 255, 255, 255, 255 };
	color2 MenuColor = { 220, 20, 60, 255 };
	color2 fakechams_color = { 255, 255, 255, 254 };
	color2 bullettrace_color = { 255, 255, 255, 255 };
	color2 nightmode_color = { 255, 255, 255, 255 };
	color2 props_color = { 255, 255, 255, 255 };
	int aspect_ratio = 0;
	int fakeduck_key = 0;
	bool fakeduck = false;
	int ThirdpersonKey = 0;
	bool event_logs[ 3 ] = { false, false, false, }; //damage purchases bomb
	bool chams_xqz = false;
	color2 chams_xqz_color = { 255, 255, 255, 255 };
	int weapon_cfg = 0;
	int baimafterx = 10;
	int mindamage_pistol = 0;
	int mindamage_heavy_pistol = 0;
	int mindamage_smg = 0;
	int mindamage_rifle = 0;
	int mindamage_sniper = 0;
	int mindamage_auto = 0;
	int mindamage_misc = 0;
	int hitchance_pistol = 0;
	int hitchance_heavy_pistol = 0;
	int hitchance_smg = 0;
	int hitchance_rifle = 0;
	int hitchance_sniper = 0;
	int hitchance_auto = 0;
	int hitchance_misc = 0;
	std::string killsay = "";
	std::string clantag = "";
	bool doorspam = false;
	int doorspam_key = 0;
	bool ammo = false;
	bool DrawMenu = false;
	int draw_key = false;
	color2 draw_color = { 255, 255, 255, 255 };
	bool draw_clear = false;
	bool draw_undo = false;
	bool autowall_crosshair = false;
	bool remove_scope = false;
	int remove_smoke = 0;
	bool grenade_prediciton = false;
	bool player_info = false;
	bool hitboxes[ 6 ] = { false, false, false, false, false, false };
	int chams_entity = 0;
	color2 chams_colorf[ 3 ] = { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };
	color2 chams_xqz_colorf[ 3 ] = { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };
	int chams_type[ 3 ];
	bool remove_zoom = false;
	bool chams_xqzf[ 3 ];
	bool esp_preview = false;
	bool BodyAim[ 64 ];
	bool WhiteList[ 64 ];
	bool priority_aim[ 64 ];
	int skybox = 0;
	std::string skybox_custom = "";
	bool resolver_active = false;
	bool render_teammates = true;
	bool cock_mode = false;
	int fake_yaw_limit = 0;
	int hitmarkers = 0;
	int hitmarkersound = 0;
	bool attargets = false;
	bool gloss_chams[ 3 ];
	int chams_type_xqz[ 3 ];
	bool remove_flash = false;
	bool bomb_timer = false;
	bool damage_indicator = false;
	color2 damage_indicator_color = { 255, 215, 0, 255 };
	bool model_on_hit = false;
	int MinimumDamage[ 7 ] = { 0, 0, 0, 0, 0, 0, 0 };
	int whitchance[ 7 ] = { 0, 0, 0, 0, 0, 0, 0 };
	color2 footstep_esp_color = { 255, 255, 255, 255 };
	bool footstep_esp = false;
	color2 glow_colorf[ 3 ] = { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };
	bool glowf[ 3 ] = { false, false, false };
	std::string custom_hitsounds;

	color2 entity_esp_color[2] = { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };

	color2 entity_glow_color[ 2 ] = { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };

	color2 entity_chams_color[ 2 ] = { { 255, 255, 255, 255 }, { 255, 255, 255, 255 } };

	bool weapon_esp[3] = { false, false, false };
	bool grenade_esp[3] = { false, false, false };

	int other_entity = 0;
	bool smoke_radius = false;
	bool fakelag_triggers[ 3 ] = { false, false, false };
	bool disable_post_processing = false;
	bool lag_compensation = false;
	bool triggerbot = false;
	int thickness = 0;
	bool lagcomp_chams = false;
	bool lock_layout = false;
	bool open_menu_draw = false;

	int paint_kit_vector_index_knife = 0;
	int paint_kit_index_knife = 0;
	int paint_kit_vector_index_glove = 0;
	int paint_kit_index_glove = 0;
	int paint_kit_vector_index_usp = 0;
	int paint_kit_index_usp = 0;
	int paint_kit_vector_index_p2000 = 0;
	int paint_kit_index_p2000 = 0;
	int paint_kit_vector_index_glock = 0;
	int paint_kit_index_glock = 0;
	int paint_kit_vector_index_p250 = 0;
	int paint_kit_index_p250 = 0;
	int paint_kit_vector_index_fiveseven = 0;
	int paint_kit_index_fiveseven = 0;
	int paint_kit_vector_index_tec = 0;
	int paint_kit_index_tec = 0;
	int paint_kit_vector_index_cz = 0;
	int paint_kit_index_cz = 0;
	int paint_kit_vector_index_duals = 0;
	int paint_kit_index_duals = 0;
	int paint_kit_vector_index_deagle = 0;
	int paint_kit_index_deagle = 0;
	int paint_kit_vector_index_revolver = 0;
	int paint_kit_index_revolver = 0;
	int paint_kit_vector_index_famas = 0;
	int paint_kit_index_famas = 0;
	int paint_kit_vector_index_galil = 0;
	int paint_kit_index_galil = 0;
	int paint_kit_vector_index_m4a4 = 0;
	int paint_kit_index_m4a4 = 0;
	int paint_kit_vector_index_m4a1 = 0;
	int paint_kit_index_m4a1 = 0;
	int paint_kit_vector_index_ak47 = 0;
	int paint_kit_index_ak47 = 0;
	int paint_kit_vector_index_sg553 = 0;
	int paint_kit_index_sg553 = 0;
	int paint_kit_vector_index_aug = 0;
	int paint_kit_index_aug = 0;
	int paint_kit_vector_index_ssg08 = 0;
	int paint_kit_index_ssg08 = 0;
	int paint_kit_vector_index_awp = 0;
	int paint_kit_index_awp = 0;
	int paint_kit_vector_index_scar = 0;
	int paint_kit_index_scar = 0;
	int paint_kit_vector_index_g3sg1 = 0;
	int paint_kit_index_g3sg1 = 0;
	int paint_kit_vector_index_sawoff = 0;
	int paint_kit_index_sawoff = 0;
	int paint_kit_vector_index_m249 = 0;
	int paint_kit_index_m249 = 0;
	int paint_kit_vector_index_negev = 0;
	int paint_kit_index_negev = 0;
	int paint_kit_vector_index_mag7 = 0;
	int paint_kit_index_mag7 = 0;
	int paint_kit_vector_index_xm1014 = 0;
	int paint_kit_index_xm1014 = 0;
	int paint_kit_vector_index_nova = 0;
	int paint_kit_index_nova = 0;
	int paint_kit_vector_index_bizon = 0;
	int paint_kit_index_bizon = 0;
	int paint_kit_vector_index_mp5sd = 0;
	int paint_kit_index_mp5sd = 0;
	int paint_kit_vector_index_mp7 = 0;
	int paint_kit_index_mp7 = 0;
	int paint_kit_vector_index_mp9 = 0;
	int paint_kit_index_mp9 = 0;
	int paint_kit_vector_index_mac10 = 0;
	int paint_kit_index_mac10 = 0;
	int paint_kit_vector_index_p90 = 0;
	int paint_kit_index_p90 = 0;
	int paint_kit_vector_index_ump45 = 0;
	int paint_kit_index_ump45 = 0;
	int knife_model = 0;
	int knife_skin = 0;

	bool FUCKFUCKFUCK[ 3 ] = { false, false, false };
	bool FUCKFUCKFUCK1[ 3 ] = { false, false, false };
	bool FUCKFUCKFUCK2[ 5 ] = { false, false, false, false, false };

	int capsule_overlay = 0;
	color2 capsule_overlay_color = { 255, 255, 255, 255 };

	color2 name_color = { 255, 255, 255, 255 };
	color2 weapon_color = { 255, 255, 255, 255 };
	color2 flags_color = { 255, 255, 255, 255 };
	color2 box_color = { 255, 255, 255, 255 };
	bool desync_chams = false;
	bool remove_visual_recoil = false;
	bool skeleton = false;
	color2 skeleton_color = { 255, 255, 255, 255 };
	bool fullbright = false;
	int world_viewmodel = 0;

	int primary_weapon = 0;
	int secondary_weapon = 0;
	bool grenade_choice[ 5 ] = { false, false, false, false, false };
	int legit_fov = false;
	bool legit_lagcomp = false;
	bool legit_aim = false;
	bool fov_arrows[ 2 ] = { false, false };
	color2 arrow_color = { 255, 255, 255, 255 };
	int diameter = 0;
	int size = 0;
	bool armor_choice[ 2 ] = { false, false };
	int auto_stop_mode = 0;
	bool slow_walk = false;
	bool auto_pistol = false;
	int trigger_hitchance = 0;
	bool indicators = false;
	bool double_tap = false;
	bool desync_display[ 2 ] = { false, false };
	bool angle_lines = false;
	color2 desync_chams_color = { 255, 255, 255, 255 };
	int arm_mat = 0;
	color2 arm_color = { 255, 255, 255, 255 };

	bool AutoHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool SniperHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool RifleHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool PistolHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool HeavyPistolHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool SMGHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool MiscHitboxes[ 6 ] = { false, false, false, false, false, false };
	bool player_logs[ 2 ] = { false, false };
	int weapon_mat = 0;
	color2 weaponc_color = { 255, 255, 255, 255 };
	int HeadPointScale = 0;
	int BodyPointScale = 0;
	bool PointScale = false;
	bool AngleLines = false;
};

extern color2 stored_color;
extern config_t Controls;

class cconfig {
public:
	static void load( ) {
		std::string config_name;
		switch ( Global::config ) {
			case 0: config_name = "legit"; break;
			case 1: config_name = "hvh"; break;
			case 2: config_name = "cfg1"; break;
			case 3: config_name = "cfg2"; break;
		}

		CreateDirectory( "cockfigs", NULL );
		std::string ConfigName;
		ConfigName = "cockfigs/";
		ConfigName += config_name;
		ConfigName += ".cock";

		std::ifstream f( ConfigName.c_str( ) );
		if ( !f.good( ) ) return;

		FILE* fin = fopen( ConfigName.c_str( ), "r" );
		fread( &Controls, sizeof( config_t ), 1, fin );
		fclose( fin );
	}

	static void save( ) {
		std::string config_name;
		switch ( Global::config ) {
			case 0: config_name = "legit"; break;
			case 1: config_name = "hvh"; break;
			case 2: config_name = "cfg1"; break;
			case 3: config_name = "cfg2"; break;
		}

		CreateDirectory( "cockfigs", NULL );
		std::string ConfigName;
		ConfigName = "cockfigs/";
		ConfigName += config_name;
		ConfigName += ".cock";
		FILE* fout = fopen( ConfigName.c_str( ), "w" );
		fwrite( &Controls, sizeof( config_t ), 1, fout );
		fclose( fout );
	}
};

extern cconfig config;