#include "common_includes.h"
#include "utilities/utilities.h"
#include "interfaces/interfaces.h"
#include "netvar_manager/netvar_manager.h"
#include "render/render.h"
#include "entities/animations/animations.h"
#include "hooks/hooks.h"
#include "Controls/Controls.h"
#include <thread>
#include <chrono>
#include <stdexcept>

bool unload( void ) {
#ifdef _DEBUG
	Utilities::unload_console( );
#endif

	g_animations.unload( );

	if ( !n_hooks::unload( ) )
		return false;

	return true;
}

bool exists( const std::string& filename ) {
	WIN32_FIND_DATAA fd = { 0 };
	HANDLE hFound = FindFirstFileA( filename.c_str( ), &fd );

	bool retval = hFound != INVALID_HANDLE_VALUE;
	FindClose( hFound );

	return retval;
}

unsigned long __stdcall initialize_cheat( void* argument ) {
	try {
#ifdef _DEBUG
		if ( !Utilities::initialize_console( "debug" ) )
			std::abort( );
#endif
		printf( _("initializing cheat... \n") );

		if ( !Interfaces::initialize( ) )
			throw std::runtime_error( _("failed to initialize interfaces") );

		if ( !exists( "csgo/materials/glow_overlay.vmt" ) ) {
			std::ofstream( "csgo/materials/glow_overlay.vmt" ) << _(R"#("VertexLitGeneric" {
 
			"$additive" "1"
			"$ignorez"      "1"
			"$color2"				"[.18 .15 .06]"
			"$envmap" "models/effects/cube_white"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			})#");
		}

		if ( !exists( "csgo\\materials\\metallic.vmt" ) ) {
			std::ofstream( "csgo\\materials\\metallic.vmt" ) << _(R"#("VertexLitGeneric" {
			  "$baseTexture" "vgui/white_additive"
			  "$ignorez"      "0"
			  "$envmap"       "env_cubemap"
			  "$nofog"        "1"
			  "$model"        "1"
			  "$nocull"       "0"
			  "$selfillum"    "1"
			  "$halflambert"  "1"
			  "$znearer"      "0"
			  "$flat"         "1"
			  "$rimlight"     "1"
			  "$rimlightexponent"  "2"
			  "$rimlightboost"     ".2"	
			  "$reflectivity" "[1 1 1]"
			}
			)#");
		}

		if ( !exists( "csgo\\materials\\metallic_xqz.vmt" ) ) {
			std::ofstream( "csgo\\materials\\metallic_xqz.vmt" ) << _(R"#("UnlitGeneric" {
			  "$baseTexture" "vgui/white_additive"
			  "$ignorez"      "1"
			  "$envmap"       "env_cubemap"
			  "$nofog"        "1"
			  "$model"        "1"
			  "$nocull"       "0"
			  "$selfillum"    "1"
			  "$halflambert"  "1"
			  "$znearer"      "0"
			  "$flat"         "1"
			  "$rimlight"     "1"
			  "$rimlightexponent"  "2"
			  "$rimlightboost"     ".2"	
			  "$reflectivity" "[1 1 1]"
			}
			)#");
		}

		if ( !exists( "csgo\\materials\\pearl.vmt" ) ) {
			std::ofstream( "csgo\\materials\\pearl.vmt" ) <<
				_("\"VertexLitGeneric\"\
				\n{\
				\n\t\"$baseTexture\" \"vgui/white_additive\"\
				\n\t\"$nocull\" \"1\"\
				\n\t\"$nofog\" \"1\"\
				\n\t\"$model\" \"1\"\
				\n\t\"$phong\" \"1\"\
				\n\t\"$ignorez\" \"0\"\
				\n\t\"$phongboost\" \"0\"\
				\n\t\"$basemapalphaphongmask\" \"1\"\
				\n\t\"$pearlescent\" \"6""\"\
				\n}\n");
		}

		if ( !exists( "csgo\\materials\\pearl_xqz.vmt" ) ) {
			std::ofstream( "csgo\\materials\\pearl_xqz.vmt" ) <<
				_("\"VertexLitGeneric\"\
				\n{\
				\n\t\"$baseTexture\" \"vgui/white_additive\"\
				\n\t\"$nocull\" \"1\"\
				\n\t\"$nofog\" \"1\"\
				\n\t\"$model\" \"1\"\
				\n\t\"$phong\" \"1\"\
				\n\t\"$ignorez\" \"1\"\
				\n\t\"$phongboost\" \"0\"\
				\n\t\"$basemapalphaphongmask\" \"1\"\
				\n\t\"$pearlescent\" \"6""\"\
				\n}\n");
		}

		if ( !n_netvar_tree::initialize( ) )
			throw std::runtime_error( _( "failed to populate netvar tree") );

		if ( !Render::initialize( ) )
			throw std::runtime_error( _( "failed to initialize renderer") );

		g_animations.initialize( );

		if ( !n_hooks::initialize( ) )
			throw std::runtime_error( _( "failed to initialize hooks") );
	}
	catch ( const std::runtime_error & exception ) {
#ifdef _DEBUG
		printf( exception.what( ) );

		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
#endif
	}

	while ( !( GetAsyncKeyState( VK_END ) && ( GetAsyncKeyState( VK_CONTROL ) ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

	if ( !unload( ) )
		return 0;

	FreeLibraryAndExitThread( static_cast< HMODULE >( argument ), 0 );

	return 1;
}

int __stdcall DllMain( const HMODULE hinst_dll, const unsigned long reason_for_call, const void* reserved ) {
	if ( hinst_dll )
		DisableThreadLibraryCalls( hinst_dll );

	if ( reason_for_call == 1 ) {
		const auto cheat_thread = CreateThread( 0, 0, initialize_cheat, hinst_dll, 0, 0 );

		if ( !cheat_thread )
			return 0;

		Global::dllmodule = hinst_dll;

		CloseHandle( cheat_thread );
	}

	return 1;
}