#pragma once
#include "../Controls/Controls.h"
#include "../hooks/hooks.h"
#include "../Controls/Controls.h"
#include "../render/render.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../menu/config.h"
#include "../render/Textures.h"
enum ctabs {
	TAB_AIM = 0,
	TAB_VISUALS,
	TAB_MISC,
	//TAB_SKINS,
	TAB_CONFIG
};
enum type {
	GB_HALF = 0,
	GB_FULL,
};
class ccontrols;
class cmenu
{
public:
	void DrawMenu( );
	void AddCheckbox( std::string name, bool* control, ctabs tab, bool keybind = false, int* ckey = 0, int subg = 0, int y = 0, std::string tip = "" );
	void AddCombobox( std::string name, std::vector<std::string> items, int* control, ctabs tab, int y = 0, int subg = 0, std::string tooltip = "" );
	void AddSlider( std::string name, int max, std::string add, int* control, ctabs tab, int subg = 0, int y = 0, int over_x = 0, std::string tooltip = "" );
	void add_button( std::string name, bool* control, ctabs tab );
	void AddColorPicker( std::string name, color2& item, ctabs tab, bool dalpha, int y = 0, int subg = 0 );
	void AddTextField( std::string name, std::string* item, ctabs tab, int subg = 0 );
	void AddMultiBox( std::string name, std::vector<std::string> items, bool* control, ctabs tab, int y = 0, int subg = 0 );

	void csgo_draw( );
	void draw_playerlist( );
	void draw_skins_group( );
	void draw_weapons_group( );
	void AddTab( std::string name );
	void add_listbox( int* control, ctabs tab, std::vector<std::string> items );
	void register_groupbox( std::string name, ctabs tab, type TYPE, int x, int y, std::string subname = "", int override_height = 0 );
	void register_subgroupbox( std::vector<std::string> name, ctabs tab, type TYPE, int x, int y, int* to_pass, int first = 0, int second = 1, int override_height = 0);

	void WeaponConfigTab( int alpha );

	void FadeAlpha( int* size, int FadeSpeed, float* value, bool pulse, int min, int PulseSpeed, bool RegionLimit );


	void run_esp_preview( int x, int y );
	int gbwidth = 190, gbheight = 0;
	bool is_sub_g = false;
	vector2d_t get_mouse_position( ) {
		POINT mousePosition;
		GetCursorPos( &mousePosition );
		ScreenToClient( FindWindow( 0, "Counter-Strike: Global Offensive" ), &mousePosition );
		return { static_cast< float >( mousePosition.x ), static_cast< float >( mousePosition.y ) };
	}

	bool mouse_in_region( int x, int y, int x2, int y2 ) {
		if ( get_mouse_position( ).x > x&& get_mouse_position( ).y > y&& get_mouse_position( ).x < x2 + x && get_mouse_position( ).y < y2 + y )
			return true;

		return false;
	}
	bool mouse_in_region( RECT area ) {
		if ( get_mouse_position( ).x > area.left&& get_mouse_position( ).y > area.top&& get_mouse_position( ).x < area.left + area.right && get_mouse_position( ).y < area.top + area.bottom )
			return true;

		return false;
	}

	int selected_subgroup1 = 0;
	int selected_subgroup2 = 2;
	int selected_subgroup3 = 4;
	int selected_subgroup4 = 6;
	int selected_subgroup5 = 8;
	int selected_subgroup6 = 10;

	void create_Texture( cTextures Texture ) {
		Interfaces::Surface->set_Texture_rgba( Global::Texture_id, Texture.load_image( ), Texture.width, Texture.height );

		Interfaces::Surface->DrawSetColor( 255, 255, 255, 255 );
		Interfaces::Surface->draw_set_Texture( Global::Texture_id );
	}

	bool manually_set_key = false;
	const char* KeyStringsStick[ 254 ] =
	{
		"[None]", "[M1]", "[M2]", "[BRK]", "[M3]", "[M4]", "[M5]",
		"[None]", "[BSPC]", "[TAB]", "[None]", "[None]", "[None]", "[ENTER]", "[None]", "[None]", "[SHI]",
		"[CTRL]", "[ALT]", "[PAU]", "[CAPS]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[ESC]", "[None]", "[None]", "[None]", "[None]", "[SPACE]", "PGUP]", "[PGDOWN]", "[END]", "[HOME]", "[LEFT]",
		"[UP]", "[RIGHT]", "[DOWN]", "[None]", "[PRNT]", "[None]", "[PRTSCR]", "[INS]", "[DEL]", "[None]", "[0]", "[1]",
		"[2]", "[3]", "[4]", "[5]", "[6", "[7]", "[8]", "[9]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]", "[J]", "[K]", "[L]", "[M]", "[N]", "[O]", "[P]", "[Q]", "[R]", "[S]", "[T]", "[U]",
		"[V]", "[W]", "[X]", "[Y]", "[Z]", "[LFTWIN]", "[RGHTWIN]", "[None]", "[None]", "[None]", "[NUM0]", "[NUM1]",
		"[NUM2]", "[NUM3]", "[NUM4]", "[NUM5]", "[NUM6]", "[NUM7]", "[NUM8]", "[NUM9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[F1]", "[F2]", "[F3]",
		"[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]", "[F13]", "[F14]", "[F15]", "[F16]", "[F17]", "[F18]", "[F19]", "[F20]",
		"[F21]",
		"[F22]", "[F23]", "[F24]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[NUM LOCK]", "[SCROLL LOCK]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[LSHFT]", "[RSHFT]", "[LCTRL]",
		"[RCTRL]", "[LMENU]", "RMENU", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[NTRK]", "[PTRK]", "[STOP]", "PLAY", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[;]", "[+]", "[,]", "[-]", "[.]", "[/?]", "[~]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[{]", "[\\|]", "}", "['\"]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]", "[None]",
		"[None]", "[None]"
	};
	const char* StrDigitsUpper[ 254 ] =
	{
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "NIGGER", nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, " ", nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, ")", "!",
		"@", "#", "$", "%", "^", "&", "*", "(", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
		"V", "W", "X", "Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, ":", "=", "<", "_", ">", "?", "~", nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "\\|", "}", "'\"", nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr
	};

	const char* StrDigitsLower[ 254 ] =
	{
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "NIGGER", nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, " ", nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1",
		"2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
		"v", "w", "x", "t", "z", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "*", "+", "_", "-", ".", "/", nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "\\|", "}", "'\"", nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr
	};

	bool keys[ 256 ];
	bool oldKeys[ 256 ];
	bool GetKeyPress( unsigned int key ) {
		if ( keys[ key ] == true && oldKeys[ key ] == false )
			return true;
		return false;
	}

	int subgroups = 0;  // this below is a clusterfuck of shit and i am planning on cleaning it, if you see this, it wasn't cleaned.

	int temp_height = 0;

	int selected_subgroup = 0;
	ctabs current_tab = ctabs::TAB_AIM;
	int current_control_index = 0;
	vector2d_t current_control_position = { 0, 0 };
	int SelectedTab = 0;
	int previous_control = -1;

	int MenuX = 215, MenuY = 200;

	bool drawing = false;

	int GROUP_X1 = 0;
	int GROUP_X2 = 0;

	int previous_group_type = 0;
	int previous_height = 0;

	int GROUP_HALF_Y1 = 0;
	int GROUP_HALF_Y2 = 0;

	int GROUP_FULL_Y = 0;

	int mwidth = 475;
	int mheight = 410;

	std::string ToolTip;

private:
	int position_y = 0, position_y2 = 0;

	int tab_items = 0;

	bool open_colorpicker = false;

	std::vector<ccontrols> search_controls;
	int groupbox_x = 0, groupbox_y = 0;
	int TabIndex = 0;
	int loop_index = 0;
	int item_x = MenuX + 190;
	int itemname_x = MenuX + 108;
	int selected_cfg = 0;
	int selected_item = 0;
	std::vector<std::string> tabs;
	bool indented = false, rest = false;
	int add = 0;

	bool started_new = false;

	std::vector<std::string> skins = {
	"groundwater",
	"candy apple",
	"forest ddpat",
	"arctic camo",
	"hot rod",
	"metallic ddpat",
	"ossified",
	"blaze",
	"fade",
	"bulldozer",
	"night",
	"orange ddpat",
	"pink ddpat",
	"boom",
	"fire serpent",
	"golden koi",
	"nitro",
	"asiimov",
	"guardian",
	"atomic alloy",
	"vulcan",
	"dragon lore",
	"man-o'-war",
	"tiger tooth",
	"damascus steel 1",
	"marble fade 1",
	"doppler 1",
	"hyper beast",
	"man-o'-war",
	"hyper beast",
	"chantico's fire",
	"asiimov",
	"wasteland rebel",
	"desolate space",
	"doppler 2",
	"doppler 3",
	"marble fade 2",
	"damascus steel 2",
	"crimson weave",
	"boom!",
	"vice",
	"pow!",
	"emerald",
	"case hardened",
	"crimson web"
	};
	std::vector<std::string> weapons = {
	"weapon_m4a1",
	"weapon_m4a1_silencer",
	"weapon_ak47",
	"weapon_aug",
	"weapon_awp",
	"weapon_famas",
	"weapon_gs3sg1",
	"weapon_galilar",
	"weapon_scar20",
	"weapon_sg556",
	"weapon_ssg08",
	"weapon_bizon",
	"weapon_mac10",
	"weapon_mp7",
	"weapon_mp9",
	"weapon_p90",
	"weapon_ump45",
	"weapon_mp5sd",
	"weapon_m249",
	"weapon_mag7",
	"weapon_negev",
	"weapon_nova",
	"weapon_sawedoff",
	"weapon_xm1014",
	"weapon_usp_silencer",
	"weapon_deagle",
	"weapon_elite",
	"weapon_fiveseven",
	"weapon_glock",
	"weapon_hkp2000",
	"weapon_p250",
	"weapon_tec9",
	"weapon_cz75a",
	"weapon_revolver",
	};
};
extern cmenu menu;

class ccontrols {
public:
	ctabs tab;
	std::string name;
	int index;

	ccontrols( ) {}

	ccontrols( std::string name, ctabs tab, int index ) {
		this->name = name;
		this->tab = tab;
		this->index = index;
	}
};

/*class draw_csgo {
public:
	int x, y;
	draw_csgo(int x, int y) {
		this->x = x;
		this->y = y;
	}
};*/

class draw_csgo {
public:
	vector2d_t start, end;
	color col = color( 0, 0, 0 );
	int thickness = 0;
	draw_csgo( vector2d_t start, vector2d_t end, color col, int thickness ) {
		this->start = start;
		this->end = end;
		this->col = col;
		this->thickness = thickness;
	}
};

