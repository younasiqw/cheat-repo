#pragma once

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/client/iclientmode.h

class IClientMode {
public:

};

class IPanel {
public:
	//const char* get_panel_name(unsigned int panel_id) {
	//	using OriginalFn = const char* (__thiscall*)(IPanel*, unsigned int);
	//	return (*(OriginalFn**)this)[36](this, panel_id);
	//}

	virtual_fn(get_panel_name(unsigned int panel_id), 36, const char* (__thiscall*)(IPanel*, unsigned int), panel_id);


};