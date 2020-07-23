#include "../hooks.h"
#include "../../Controls/Controls.h"
#include "../../render/render.h"
#include "../../features/Headers/visuals.h"
#include "../../menu/menu.h"
#include "../../features/Headers/Autowall.h"
#include "../../features/Headers/Misc.h"
#include <iostream>
#include <iomanip>
#include "../../render/Textures.h"
#include "../../sdk/IEngineTrace.h"
#include "../../features/Headers/Ragebot.h"
#include "../../features/Headers/Events.h"
#include "../../features/Headers/CustomCodeAPI.h"

/*class cpoints {
public:
	vector2d_t start;
	vector2d_t end;

	cpoints( vector2d_t start, vector2d_t end ) {
		this->start = start;
		this->end = end;
	}
};*/
namespace n_hooks {
	void draw_angle_lines( ) {
		if ( !Global::Local || !Global::Local->IsAlive() || !Controls.desync_display[ 1 ] )
			return;

		Vector3D src3D, dst3D, forward, src, dst;
		C_Trace tr;
		C_Ray ray;
		CTraceFilter filter;

		filter.pSkip1 = Global::Local;
		Math.AngleVectors( Vector3D( 0, Global::Local->lower_body_yaw( ), 0 ), &forward );
		src3D = Global::Local->GetVecOrigin( );
		dst3D = src3D + ( forward * 20.f );

		ray.Init( src3D, dst3D );

		Interfaces::Trace->TraceRay( ray, 0, &filter, &tr );

		if ( !Interfaces::DebugOverlay->WorldToScreen( src3D, src ) || !Interfaces::DebugOverlay->WorldToScreen( tr.end, dst ) )
			return;

		Render::line( src.x, src.y, dst.x, dst.y, color( 255, 0, 0 ) );
		Render::line( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, color( 255, 0, 0 ) );
		Render::Text( Render::Fonts::visuals, "lby", dst.x + 4, dst.y + 4, false, 255, 255, 255 );

		Math.AngleVectors( Vector3D( 0, Global::real_angle.y, 0 ), &forward );
		dst3D = src3D + ( forward * 20.f );

		ray.Init( src3D, dst3D );

		Interfaces::Trace->TraceRay( ray, 0, &filter, &tr );

		if ( !Interfaces::DebugOverlay->WorldToScreen( src3D, src ) || !Interfaces::DebugOverlay->WorldToScreen( tr.end, dst ) )
			return;

		Render::line( src.x, src.y, dst.x, dst.y, color( 255, 255, 255 ) );
		Render::line( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, color( 255, 255, 255 ) );
		Render::Text( Render::Fonts::visuals, "real", dst.x + 4, dst.y + 4, false, 255, 255, 255 );

		Math.AngleVectors( Vector3D( 0, Global::fake_angle.y, 0 ), &forward );
		dst3D = src3D + ( forward * 20.f );

		ray.Init( src3D, dst3D );

		Interfaces::Trace->TraceRay( ray, 0, &filter, &tr );

		if ( !Interfaces::DebugOverlay->WorldToScreen( src3D, src ) || !Interfaces::DebugOverlay->WorldToScreen( tr.end, dst ) )
			return;

		Render::line( src.x, src.y, dst.x, dst.y, color( 255, 255, 0 ) );
		Render::line( src.x + 1, src.y + 1, dst.x + 1, dst.y + 1, color( 255, 255, 0 ) );
		Render::Text( Render::Fonts::visuals, "fake", dst.x + 4, dst.y + 4, false, 255, 255, 255 );
	}
	void __fastcall n_functions::PaintTraverse( PVOID panels, int edx, unsigned int vgui_panel, bool force_repaint, bool allow ) {
		static auto OriginalFn = n_hooks::Panel->get_original_function< decltype( &PaintTraverse ) >( 41 );

		if ( !strcmp( "HudZoom", Interfaces::Panel->get_panel_name( vgui_panel ) ) && Controls.remove_scope )
			return;

		OriginalFn( panels, edx, vgui_panel, force_repaint, allow );

		static unsigned int _panel = 0;
		static bool once = false;

		if ( !once ) {
			PCHAR panel_char = ( PCHAR ) Interfaces::Panel->get_panel_name( vgui_panel );
			if ( strstr( panel_char, "MatSystemTopPanel" ) ) {
				_panel = vgui_panel;
				once = true;
			}
		}
		else if ( _panel == vgui_panel ) {

			static int sw, sh;
			Interfaces::engine->get_screen_size( sw, sh );

			/* foot step esp*/
			if ( Controls.footstep_esp ) {
				for ( size_t i = 0; i < footsteps.size( ); i++ ) {
					draw_step_circle( color( Controls.footstep_esp_color.r, Controls.footstep_esp_color.g, Controls.footstep_esp_color.b ), footsteps.at( i ).start );
					footsteps.erase( footsteps.begin( ) + i );
				}
			}

			std::copy( menu.keys, menu.keys + 255, menu.oldKeys );
			for ( int x = 0; x < 255; x++ ) {
				menu.keys[ x ] = ( GetAsyncKeyState( x ) );
			}

			//MainAPI::Drawing::DrawOnPaint( );

			if ( GetAsyncKeyState( WM_MOUSEWHEEL ) ) {
				Render::Text( Render::Fonts::main, "MOUSE" , 10, 10, false, 255, 255, 255 );
			}
			draw_angle_lines( );

			/* draw visuals */
			visuals.run_visuals( );

			//Render::Text( Render::Fonts::main, std::to_string( Global::MissedShots ), 10, 10, false, 255, 255, 255 );

			if ( Global::Local ) {
				/* draw no scope lines*/
				if ( Global::Local->IsScoped( ) && Controls.remove_scope && Interfaces::engine->IsInGame( ) ) {
					Interfaces::Surface->DrawSetColor( 15, 15, 15, 255 );
					Interfaces::Surface->draw_line( sw / 2, 0, sw / 2, sh );
					Interfaces::Surface->draw_line( 0, sh / 2, sw, sh / 2 );
				}

				/* draw local info indicators*/
				visuals.RunIndicators( );

				/* draw damage indicators*/
				visuals.RunDamageIndicators( );

				/* draw bullet beams */
				visuals.RunBulletBeams( );
			}

			/* draw hitmarkers */
			visuals.RunHitmarkers( );

			/* draw and logic for event logs*/
			if ( Controls.event_logs )
				misc.PaintEvents( );

			/* draw and logic for bomb timer*/
			visuals.RunBombTimer( );

			/* draw shot log and kill log*/
			if ( Controls.player_logs[ 0 ] )
				misc.PaintShotLog( );

			if ( Controls.player_logs[ 1 ] )
				misc.RunStatsBox( );

			/* autowall crosshair*/
			if ( Interfaces::engine->IsInGame( ) && Controls.autowall_crosshair ) {
				Render::RoundedRect( sw / 2 - 1.5f, ( sh / 2 ) - 1.5f, 5, 5, 3, autowall.CanWallbang( ) ? color( 0, 255, 0 ) : color( 255, 0, 0 ) );
			}

			/* create draw Surface for draw feature*/
			if ( Controls.DrawMenu )
				menu.csgo_draw( );

			/* draw menu*/
			menu.DrawMenu( );

			/* run remove smoke */
			visuals.RunRemoveSmoke( );
		}
	}
}