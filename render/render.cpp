#include "render.h"
#define M_PI 3.14159265358979323846
#include "../icomoon.h"
#include <wingdi.h>
#include "../entities/entity.h"
#include "../resource.h"
#include <fstream>
#include "../menu/menu.h"

namespace Render {
	namespace Fonts {
		HFont main;
		HFont menu;
		HFont visuals;
		HFont title;
		HFont tab;
		HFont slider;
		HFont preview;
		HFont WeaponIcon;
		HFont WeaponConfig;
		HFont WeaponText;


		bool initialize( void ) {
			auto create_font = [ ] ( HFont& font, const char* name, const int size, const int weight, const int flags ) -> bool {
				font = Interfaces::Surface->create_font( );

				if ( !font )
					return false;

				Interfaces::Surface->set_font_glyph_set( font, name, size, weight, 0, 0, flags, 0, 0 );

				return true;
			};

			if ( !( create_font( Render::Fonts::main, "Arial", 14, 500, FONTFLAG_ANTIALIAS ) ) )
				return false;

			if ( !( create_font( Render::Fonts::menu, "Arial", 14, 450, FONTFLAG_ANTIALIAS ) ) )
				return false;

			if ( !( create_font( Render::Fonts::slider, "Arial", 12, 450, FONTFLAG_ANTIALIAS ) ) )
				return false;

			if ( !( create_font( Render::Fonts::tab, "Open Sans", 17, 550, FONTFLAG_ANTIALIAS ) ) )
				return false;

			if ( !( create_font( Render::Fonts::WeaponText, "Open Sans", 14, 550, FONTFLAG_ANTIALIAS ) ) )
				return false;

			if ( !( create_font( Render::Fonts::title, "Impact", 33, 0, FONTFLAG_ANTIALIAS | FONTFLAG_GAUSSIANBLUR) ) )
				return false;

			if ( !create_font( Render::Fonts::visuals, "Roboto", 10.3, 550, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW) )
				return false;

			if ( !create_font( Render::Fonts::preview, "Small Fonts", 10, 400, FONTFLAG_ANTIALIAS ) )
				return false;

			if ( !create_font( Render::Fonts::WeaponIcon, "Counter-Strike", 20, 400, FONTFLAG_ANTIALIAS ) )
				return false;

			if ( !create_font( Render::Fonts::WeaponConfig, "Counter-Strike", 42, 400, FONTFLAG_ANTIALIAS ) )
				return false;

			return true;
		}
	}

	int screen_w, screen_h;

	bool initialize( void ) {
		Interfaces::Surface->get_screen_size( screen_w, screen_h );

		if ( !screen_w || !screen_h )
			return false;

		if ( !Fonts::initialize( ) )
			return false;

		return true;
	}

	void FilledRect( const int x, const int y, const int w, const int h, color col ) {
		Interfaces::Surface->DrawSetColor( col.r, col.g, col.b, col.a );
		Interfaces::Surface->DrawFilledRect( x, y, x + w, y + h );
	}

	void OutlinedRect( const int x, const int y, const int w, const int h, color col ) {
		Interfaces::Surface->DrawSetColor( col.r, col.g, col.b, col.a );
		Interfaces::Surface->DrawOutlinedRect( x, y, x + w, y + h );
	}

	void line( const int x, const int y, const int x2, const int y2, color col ) {
		Interfaces::Surface->DrawSetColor( col.r, col.g, col.b, col.a );
		Interfaces::Surface->draw_line( x, y, x2, y2 );
	}

	void faded_rect( const int x, const int y, const int w, const int h, color top, color bottom, bool horizontal )
	{
		Interfaces::Surface->DrawSetColor( top.r, top.g, top.b, top.a );
		Interfaces::Surface->draw_filled_rect_fade( x, y, x + w, y + h, 255, 255, horizontal );

		Interfaces::Surface->DrawSetColor( bottom.r, bottom.g, bottom.b, bottom.a );
		Interfaces::Surface->draw_filled_rect_fade( x, y, x + w, y + h, 0, 255, horizontal );
	}

	
	void PulseRect( int x, int y, int w, int h, int FadeWidth, color c ) {

		auto filledrectfade = [ ] ( int x, int y, int w, int h, int a1, int a2, bool horiz ) -> void {
			Interfaces::Surface->draw_filled_rect_fade( x, y, x + w, y + h, a1, a2, horiz );
		};

		static float fade;
		menu.FadeAlpha( nullptr, 0, &fade, true, 50, 7, false );

		Interfaces::Surface->DrawSetColor( c.r, c.g, c.b, fade );

		//left
		filledrectfade( x, y + FadeWidth, FadeWidth, h - FadeWidth, 0, fade, true );

		//right
		filledrectfade( x + w, y + FadeWidth, FadeWidth, h - FadeWidth, fade, 0, true );

		//top
		filledrectfade( x + FadeWidth, y, w - FadeWidth, FadeWidth, 0, fade, false );

		//bottom
		filledrectfade( x + FadeWidth, y + h, w - FadeWidth, FadeWidth, fade, 0, false );
	
	}

	void Text( const HFont& font, const std::string Text, const int x, const int y, const bool centered, const int r, const int g, const int b, const int a ) {

		std::wstring converted_Text = std::wstring( Text.begin( ), Text.end( ) );

		int width, height;
		Interfaces::Surface->get_Text_size( font, converted_Text.c_str( ), width, height );

		int _x = x, _y = y;

		if ( centered == true )
			_x -= width / 2;

		Interfaces::Surface->SetTextColor( r, g, b, a );
		Interfaces::Surface->draw_set_Text_font( font );

		Interfaces::Surface->draw_set_Text_pos( _x, _y );

		Interfaces::Surface->DrawPrintText( converted_Text.c_str( ), converted_Text.length( ) );
	}

	void draw_circle( int x, int y, int r, int s, int red, int green, int blue ) {
		float Step = M_PI * 2.0 / s;
		for ( float a = 0; a < ( M_PI * 2.0 ); a += Step ) {
			float x1 = r * cos( a ) + x;
			float y1 = r * sin( a ) + y;
			float x2 = r * cos( a + Step ) + x;
			float y2 = r * sin( a + Step ) + y;
			Interfaces::Surface->DrawSetColor( red, green, blue, 255 );
			Interfaces::Surface->draw_line( x1, y1, x2, y2 );
		}
	}

	void RoundedRect( int x, int y, int width, int height, float radius, color col ) {
		constexpr int quality = 24;
		static vertex_t verts[ quality ];
		vector2d_t add = { 0, 0 };

		for ( int i = 0; i < quality; i++ ) {
			float angle = ( static_cast < float > ( i ) / -quality ) * 6.28f - ( 6.28f / 16.f );

			verts[ i ].mPosition.x = radius + x + add.x + ( radius * sin( angle ) );
			verts[ i ].mPosition.y = height - radius + y + add.y + ( radius * cos( angle ) );

			if ( i == 4 ) {
				add.y = -height + ( radius * 2 );
			}
			else if ( i == 10 ) {
				add.x = width - ( radius * 2 );
			}
			else if ( i == 16 ) {
				add.y = 0;
			}
			else if ( i == 22 ) {
				add.x = 0;
			}
		}
		Interfaces::Surface->DrawSetColor( col.r, col.g, col.b, col.a );
		Interfaces::Surface->draw_Textured_polygon( quality, verts );
	}
	void trace_line( Vector3D& start, Vector3D& end, unsigned int mask, C_CSPlayer* ignore, C_Trace* trace ) {

		C_TraceFilter filter( ignore );
		filter.pSkip1 = ignore;

		Interfaces::Trace->TraceRay( C_Ray( start, end ), mask, &filter, trace );
	}
	void draw_circle_3d( Vector3D position )
	{
		Vector3D prev_scr_pos { -1, -1, -1 };
		Vector3D scr_pos;

		float step = M_PI * 2.0 / 150;

		float rad = 100.f;

		Vector3D origin = position;

		for ( float rotation = 0; rotation < ( M_PI * 2.0 ); rotation += step )
		{
			Vector3D pos( rad * cos( rotation ) + origin.x, rad * sin( rotation ) + origin.y, origin.z + 2 );
			Vector3D tracepos( origin.x, origin.y, origin.z + 2 );

			C_Ray ray;
			C_Trace trace;
			CTraceFilter filter;

			ray.Init( tracepos, pos );

			Interfaces::Trace->TraceRay( ray, mask_splitareaportal, &filter, &trace );

			if ( Interfaces::DebugOverlay->WorldToScreen( trace.end, scr_pos ) )
			{
				if ( prev_scr_pos != Vector3D { -1, -1, -1 } ) {

					Render::line( prev_scr_pos.x - 1, prev_scr_pos.y - 1, scr_pos.x - 1, scr_pos.y - 1, color( 255, 255, 255 ) );
					Render::line( prev_scr_pos.x, prev_scr_pos.y, scr_pos.x, scr_pos.y, color( 255, 255, 255 ) );
					Render::line( prev_scr_pos.x + 1, prev_scr_pos.y + 1, scr_pos.x + 1, scr_pos.y + 1, color( 255, 255, 255 ) );
				}
				prev_scr_pos = scr_pos;
			}
		}
	}
}