#pragma once
#include "../utilities/utilities.h"
#include "../Math/vector3d.h"
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/ienginevgui.h

enum VGuiPanel_t {
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
};

// In-game panels are cropped to the current engine viewport size
enum PaintMode_t {
	PAINT_UIPANELS = ( 1 << 0 ),
	PAINT_INGAMEPANELS = ( 1 << 1 ),
	PAINT_CURSOR = ( 1 << 2 ), // software cursor, if appropriate
};

struct player_info_t {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name[128];
	int userid;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

class IVEngineVGui {
public:
	virtual_fn(get_player_info(int index, player_info_t* info), 8, void(__thiscall*)(void*, int, player_info_t*), index, info);

};