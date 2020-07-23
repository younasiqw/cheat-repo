#pragma once

#include "../sdk.h"
#include "../sdk/IEngineTrace.h"
namespace Interfaces {
	extern IBaseClientDLL* client;
	extern IPanel* Panel;
	extern IInputSystem* input_system;
	extern IClientMode* ClientMode;
	extern IGameEventManager2* event_manager2;
	extern IVEngineClient* engine;
	extern IClientEntityList* EntityList;
	extern IVEngineVGui* engine_vgui;
	extern ISurface* Surface;
	extern CGlobalVarsBase* GlobalVarsBase;
	extern IPrediction* prediction;
	extern IMoveHelper* move_helper;
	extern IGameMovement* game_movement;
	extern IWeaponSystem* weapon_system;
	extern IMDLCache* model_cache;
	extern IVModelRender* ModelRender;
	extern IMaterialSystem* MaterialSystem;
	extern IVModelInfo* ModelInfo;
	extern IVDebugOverlay* DebugOverlay;
	extern IPhysicsSurfaceProps* SurfaceData;
	extern IEngineTrace* Trace;
	extern IConvar* Convar;
	extern IVRenderView* RenderView;
	extern CGlowObjectManager* glow_manager;
	extern IViewRenderBeams* RenderBeams;
	extern IMemoryAllocate* MemAlloc;
	extern CInput* Input;
	extern IClientState* ClientState;

	bool initialize( void );
}