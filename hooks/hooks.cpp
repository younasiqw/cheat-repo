#include "hooks.h"
#include "../Controls/Controls.h"
#include "../interfaces/interfaces.h"
#include "../sdk/IGameEvent.h"
#include "../features/Headers/Events.h"
#include "../features/Headers/Skins.h"
cmatricies matrix;
std::vector<shot_log> shotlog;
std::vector<chitmarkers> hitmarkers;
std::vector<damage_indicator_t> damage_indicator;
std::vector<cstoredhits> storedhits;
std::vector<cfootsteps> footsteps;
std::vector<grenade_info> grenadeinfo;
std::vector<bulletinfo> info;

float hit_curtime = 0.f;

namespace n_hooks {
	std::unique_ptr< c_vmt > ClientMode;
	std::unique_ptr< c_vmt > Panel;
	std::unique_ptr< c_vmt > EngineClient;
	std::unique_ptr< c_vmt > ClientHook;
	std::unique_ptr< c_vmt > BSPQuery;
	std::unique_ptr< c_vmt > ModelRender;
	std::unique_ptr< c_vmt > SurfaceVGUI;
	std::unique_ptr< c_vmt > RenderView;
	std::unique_ptr<VMTHook> pClientHook;

	bool initialize( void ) {
		ClientMode = std::make_unique< c_vmt >( );
		ClientHook = std::make_unique< c_vmt >( );
		Panel = std::make_unique< c_vmt >( );
		EngineClient = std::make_unique< c_vmt >( );
		BSPQuery = std::make_unique< c_vmt >( );
		ModelRender = std::make_unique< c_vmt >( );
		SurfaceVGUI = std::make_unique< c_vmt >( );
		RenderView = std::make_unique< c_vmt >( );

		events.on_attach( );

		//pClientHook = std::make_unique<VMTHook>( Interfaces::client );
		//pClientHook->Hook( 24, n_functions::WriteUsercmdDeltaToBuffer_hook );

		/* IClientMode */
		if ( !ClientMode->initialize( Interfaces::ClientMode ) )
			return false;

		if ( !ClientMode->hook_function( 44, n_functions::hkDoPostScreenEffects ) )
			return false;

		if ( !ClientMode->hook_function( 24, n_functions::CreateMove ) )
			return false;

		if ( !ClientMode->hook_function( 35, n_functions::GetViewModelFOV ) )
			return false;

		if ( !ClientMode->hook_function( 18, n_functions::OverrideView ) )
			return false;

		/* IPanel */
		if ( !Panel->initialize( Interfaces::Panel ) )
			return false;

		if ( !Panel->hook_function( 41, n_functions::PaintTraverse ) )
			return false;

		/* IVEngineClient */
		if ( !EngineClient->initialize( Interfaces::engine ) )
			return false;

		if ( !EngineClient->hook_function( 101, n_functions::Hook_GetScreenAspectRatio ) )
			return false;

		/* g_pRenderView */
		if ( !RenderView->initialize( Interfaces::RenderView ) )
			return false;

		if ( !RenderView->hook_function( 9, n_functions::SceneEnd ) )
			return false;

		/* IClientHook */
		if ( !ClientHook->initialize( Interfaces::client ) )
			return false;

		if ( !ClientHook->hook_function( 37, n_functions::FrameStageNotify ) )
			return false;

		//if ( !ClientHook->hook_function( 24, n_functions::WriteUsercmdDeltaToBuffer_hook ) )
		//	return false;

		/* VGUI_Surface */
		if ( !SurfaceVGUI->initialize( Interfaces::Surface ) )
			return false;

		if ( !SurfaceVGUI->hook_function( 67, n_functions::LockCursor ) )
			return false;

		/* ISpatialQuery */
		if ( !BSPQuery->initialize( Interfaces::engine->get_bsp_tree_query( ) ) )
			return false;

		if ( !BSPQuery->hook_function( 6, n_functions::ListLeavesInBox ) )
			return false;

		/* IVModelRender */
		if ( !ModelRender->initialize( Interfaces::ModelRender ) )
			return false;

		if ( !ModelRender->hook_function( 21, n_functions::DrawModelExecute ) )
			return false;

		static bool init = false;
		if ( !init ) { 		// i'm sorry
			cconvar* sv_cheats = Interfaces::Convar->FindVar( "sv_cheats" );
			sv_cheats->set_value( 1 );
			Interfaces::Convar->FindVar( "cl_foot_contact_shadows" )->set_value( 0 );
			init = true;
		}

		skin_changer.apply_sequence_update( );
		return true;
	}

	bool unload( void ) {

		//pClientHook->Unhook( 24 );

		/* IClientMode */
		if ( !ClientMode->unhook( ) )
			return false;

		/* IClientHook */
		if ( !ClientHook->unhook( ) )
			return false;

		/* g_pRenderView */
		if ( !RenderView->unhook( ) )
			return false;

		/* IPanel */
		if ( !Panel->unhook( ) )
			return false;

		/* IVEngineClient */
		if ( !EngineClient->unhook( ) )
			return false;

		/* VGUI_Surface */
		if ( !SurfaceVGUI->unhook( ) )
			return false;

		/* ISpatialQuery */
		if ( !BSPQuery->unhook( ) )
			return false;

		/* IVModelRender */
		if ( !ModelRender->unhook( ) )
			return false;

		events.on_detach( );

		//Interfaces::event_manager2->RemoveListener(weaponfire);

		//Interfaces::event_manager2->RemoveListener(playerhurt);

		//Interfaces::event_manager2->RemoveListener(bulletimpact);

		return true;
	}
}