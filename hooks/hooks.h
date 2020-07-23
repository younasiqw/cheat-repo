#pragma once

#include "../vmt/vmt.h"
#include "../entities/entity.h"
#include "../render/render.h"

namespace n_hooks {
	extern std::unique_ptr< c_vmt > ClientMode;
	extern std::unique_ptr< c_vmt > Panel;
	extern std::unique_ptr< c_vmt > EngineClient;
	extern std::unique_ptr< c_vmt > ClientHook;
	extern std::unique_ptr< c_vmt > BSPQuery;
	extern std::unique_ptr< c_vmt > ModelRender;
	extern std::unique_ptr< c_vmt > SurfaceVGUI;
	extern std::unique_ptr< c_vmt > RenderView;
	extern std::unique_ptr<VMTHook> pClientHook;

	bool initialize( void );


	namespace n_functions {
		bool __fastcall CreateMove( std::uintptr_t ecx, std::uintptr_t edx, float input_sample_time, c_usercmd* uCmd );
		float __fastcall GetViewModelFOV( std::uintptr_t ecx, std::uintptr_t edx );
		void __fastcall PaintTraverse( PVOID panels, int edx, unsigned int vgui_panel, bool force_repaint, bool allow );
		void __fastcall LockCursor( std::uintptr_t ecx, std::uintptr_t edx );
		bool __fastcall SetupBones( std::uintptr_t ecx, std::uintptr_t edx, void* bone_to_world_out, int max_bones, int bone_mask, float CurrentTime );
		int __fastcall ListLeavesInBox( std::uintptr_t ecx, std::uintptr_t edx, Vector3D& mins, Vector3D& maxs, unsigned short* list, int list_max );
		void __stdcall FrameStageNotify( ClientFrameStage_t curStage );
		void __fastcall SceneEnd( std::uintptr_t ecx, std::uintptr_t edx );
		int  __fastcall hkDoPostScreenEffects( void* _this, int edx, int a1 );
		void __fastcall DrawModelExecute( std::uintptr_t ecx, std::uintptr_t edx, void* render_conText, void* state, ModelRenderInfo_t& info, void* custom_bone_to_world );
		float __fastcall Hook_GetScreenAspectRatio( void* pecx, void* pedx, int32_t width, int32_t height );
		void __fastcall OverrideView( void* ecx, void* edx, CViewSetup* setup );
		bool __fastcall WriteUseruCmdDeltaToBuffer_hook( void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool isnewcommand );

		typedef bool( __thiscall* WriteUseruCmdDeltaToBufferFn )( void* ecx, int slot, bf_write* buf, int from, int to, bool isnewcommand );
	}

	bool unload( void );

}
class cmatricies {
public:
	matrix3x4_t enemy_matrix[ 65 ][ 128 ];
	matrix3x4_t backtrack_matrix[ 65 ][ 128 ];
	matrix3x4_t local_matrix[ 128 ];
};
extern cmatricies matrix;

/* Below is lots of classes with basic constructors that there has to be a better way of doing, probably using unnessecary amounts of memory and losing speed, fix */

/* i could probably use templates */



class bulletinfo {
public:
	Vector3D start;
	Vector3D end;
	float curtime;

	bulletinfo( Vector3D start, Vector3D end, float curtime ) {
		this->start = start;
		this->end = end;
		this->curtime = curtime;
	}
};

extern std::vector<bulletinfo> info;

class grenade_info {
public:
	Vector3D position;
	float curtime;
	std::string grenade;

	grenade_info( Vector3D position, float curtime, std::string grenade ) {
		this->position = position;
		this->curtime = curtime;
		this->grenade = grenade;
	}
};

extern std::vector<grenade_info> grenadeinfo;

class cfootsteps {
public:
	Vector3D start;
	float curtime;
	int id;

	cfootsteps( Vector3D start, float curtime, int id ) {
		this->start = start;
		this->id = id;
		this->curtime = curtime;
	}
};

extern std::vector<cfootsteps> footsteps;

struct damage_indicator_t
{
	int damage;
	bool init;
	float curtime;
	float update;
	C_CSPlayer* player;
	Vector3D position;
};
extern std::vector<damage_indicator_t> damage_indicator;


class cstoredhits
{
public:
	Vector3D origin;
	Vector3D angles;
	float curtime;
	C_CSPlayer* player;

	cstoredhits( Vector3D origin, Vector3D angles, float curtime, C_CSPlayer* player )
	{
		this->origin = origin;
		this->angles = angles;
		this->curtime = curtime;
		this->player = player;
	}
};

extern std::vector<cstoredhits> storedhits;

class shot_log {
public:
	std::string name;
	std::string hitbox;
	int hitchance;
	std::string damage;

	shot_log( std::string name, std::string hitbox, int hitchance, std::string damage ) {
		this->name = name;
		this->hitbox = hitbox;
		this->hitchance = hitchance;
		this->damage = damage;
	}
};
extern std::vector<shot_log> shotlog;

class chitmarkers {
public:
	Vector3D postition = { 0, 0, 0 };
	float time = 0.f;

	chitmarkers( Vector3D postition, float time ) {
		this->postition = postition;
		this->time = time;
	}
};
extern std::vector<chitmarkers> hitmarkers;
extern float hit_curtime;