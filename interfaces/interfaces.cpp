#include "interfaces.h"

#include "../utilities/utilities.h"

#include <cstdint>
namespace Interfaces
{
	IBaseClientDLL* client = nullptr;
	IClientMode* ClientMode = nullptr;
	IPanel* Panel = nullptr;
	IInputSystem* input_system = nullptr;
	IVEngineClient* engine = nullptr;
	IGameEventManager2* event_manager2 = nullptr;
	IClientEntityList* EntityList = nullptr;
	IVEngineVGui* engine_vgui = nullptr;
	ISurface* Surface = nullptr;
	CGlobalVarsBase* GlobalVarsBase = nullptr;
	IPrediction* prediction = nullptr;
	IMoveHelper* move_helper = nullptr;
	IGameMovement* game_movement = nullptr;
	IWeaponSystem* weapon_system = nullptr;
	IMDLCache* model_cache = nullptr;
	IVModelRender* ModelRender = nullptr;
	IMaterialSystem* MaterialSystem = nullptr;
	IVModelInfo* ModelInfo = nullptr;
	IVDebugOverlay* DebugOverlay = nullptr;
	IPhysicsSurfaceProps* SurfaceData = nullptr;
	IEngineTrace* Trace = nullptr;
	IConvar* Convar = nullptr;
	IVRenderView* RenderView = nullptr;
	CGlowObjectManager* glow_manager = nullptr;
	IViewRenderBeams* RenderBeams = nullptr;
	IMemoryAllocate* MemAlloc = nullptr;
	CInput* Input = nullptr;
	IClientState* ClientState = nullptr;

	bool initialize( void ) {
		client = reinterpret_cast< IBaseClientDLL* >( Utilities::CaptureInterface( "client_panorama.dll", "VClient018" ) );

		if ( !client )
			return false;

		ClientMode = **reinterpret_cast< IClientMode*** >( ( *reinterpret_cast< std::uintptr_t** >( client ) )[ 10 ] + 5 );

		if ( !ClientMode )
			return false;

		engine = reinterpret_cast< IVEngineClient* >( Utilities::CaptureInterface( "engine.dll", "VEngineClient014" ) );

		if ( !engine )
			return false;

		event_manager2 = reinterpret_cast< IGameEventManager2* >( Utilities::CaptureInterface( "engine.dll", "GAMEEVENTSMANAGER002" ) );

		if ( !event_manager2 )
			return false;

		Panel = reinterpret_cast< IPanel* >( Utilities::CaptureInterface( "vgui2.dll", "VGUI_Panel009" ) );

		if ( !Panel )
			return false;

		MemAlloc = *reinterpret_cast< IMemoryAllocate** >( GetProcAddress( GetModuleHandle( "tier0.dll" ), "g_pMemAlloc" ) );

		if ( !MemAlloc )
			return false;

		glow_manager = *reinterpret_cast< CGlowObjectManager** >( Utilities::FindPattern( "client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00" ) + 0x3 );

		if ( !glow_manager )
			return false;

		EntityList = reinterpret_cast< IClientEntityList* >( Utilities::CaptureInterface( "client_panorama.dll", "VClientEntityList003" ) );

		if ( !EntityList )
			return false;

		input_system = reinterpret_cast< IInputSystem* >( Utilities::CaptureInterface( "inputsystem.dll", "InputSystemVersion001" ) );

		if ( !input_system )
			return false;

		Input = *reinterpret_cast< CInput** >( Utilities::FindPattern( "client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10" ) + 1 );

		if ( !Input )
			return false;

		ClientState = reinterpret_cast< IClientState* >( Utilities::FindPattern( "engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3" ) + 1 );

		if ( !ClientState )
			return false;

		engine_vgui = reinterpret_cast< IVEngineVGui* >( Utilities::CaptureInterface( "engine.dll", "VEngineVGui001" ) );

		if ( !engine_vgui )
			return false;

		RenderBeams = *reinterpret_cast< IViewRenderBeams** >( Utilities::FindPattern( "client_panorama.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) + 1 );

		if ( !RenderBeams )
			return false;


		Convar = reinterpret_cast< IConvar* >( Utilities::CaptureInterface( "vstdlib.dll", "VEngineCvar007" ) );

		if ( !Convar )
			return false;

		RenderView = reinterpret_cast< IVRenderView* >( Utilities::CaptureInterface( "engine.dll", "VEngineRenderView014" ) );

		if ( !RenderView )
			return false;

		Surface = reinterpret_cast< ISurface* >( Utilities::CaptureInterface( "vguimatsurface.dll", "VGUI_Surface031" ) );

		if ( !Surface )
			return false;

		GlobalVarsBase = **reinterpret_cast< CGlobalVarsBase*** >( ( *reinterpret_cast< std::uintptr_t** >( client ) )[ 11 ] + 10 );

		if ( !GlobalVarsBase )
			return false;

		prediction = reinterpret_cast< IPrediction* >( Utilities::CaptureInterface( "client_panorama.dll", "VClientPrediction001" ) );

		if ( !prediction )
			return false;

		Trace = reinterpret_cast< IEngineTrace* >( Utilities::CaptureInterface( "engine.dll", "EngineTraceClient004" ) );

		if ( !Trace )
			return false;

		SurfaceData = reinterpret_cast< IPhysicsSurfaceProps* >( Utilities::CaptureInterface( "vphysics.dll", "VPhysicsSurfaceProps001" ) ); //VPhysicsSurfaceProps001

		if ( !SurfaceData )
			return false;

		move_helper = **reinterpret_cast< IMoveHelper*** >( Utilities::FindPattern( "client_panorama.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) + 0x2 );

		if ( !move_helper )
			return false;

		game_movement = reinterpret_cast< IGameMovement* >( Utilities::CaptureInterface( "client_panorama.dll", "GameMovement001" ) );

		if ( !game_movement )
			return false;

		weapon_system = *reinterpret_cast< IWeaponSystem** >( Utilities::FindPattern( "client_panorama.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 0x2 );

		if ( !weapon_system )
			return false;

		model_cache = reinterpret_cast< IMDLCache* >( Utilities::CaptureInterface( "datacache.dll", "MDLCache004" ) );

		if ( !model_cache )
			return false;

		ModelRender = reinterpret_cast< IVModelRender* >( Utilities::CaptureInterface( "engine.dll", "VEngineModel016" ) );

		if ( !ModelRender )
			return false;

		MaterialSystem = reinterpret_cast< IMaterialSystem* >( Utilities::CaptureInterface( "materialsystem.dll", "VMaterialSystem080" ) );

		if ( !MaterialSystem )
			return false;

		ModelInfo = reinterpret_cast< IVModelInfo* >( Utilities::CaptureInterface( "engine.dll", "VModelInfoClient004" ) );

		if ( !ModelInfo )
			return false;

		DebugOverlay = reinterpret_cast< IVDebugOverlay* >( Utilities::CaptureInterface( "engine.dll", "VDebugOverlay004" ) );

		if ( !DebugOverlay )
			return false;

		return true;
	}
}