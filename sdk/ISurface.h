#pragma once

#include "../utilities/utilities.h"

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/vgui/ISurface.h

typedef unsigned long HFont;

struct vertex_t {
	vector2d_t	mPosition;
	vector2d_t	m_TexCoord;

	vertex_t() {}
	vertex_t(const vector2d_t& pos, const vector2d_t& coord = vector2d_t(0, 0)) {
		mPosition = pos;
		m_TexCoord = coord;
	}
	void init(const vector2d_t& pos, const vector2d_t& coord = vector2d_t(0, 0)) {
		mPosition = pos;
		m_TexCoord = coord;
	}
};
typedef  vertex_t font_vertex_t;

enum FontFlags_t {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

class ISurface {
public:
	// color Setting methods
	virtual_fn( DrawSetColor( int r, int g, int b, int a ), 15, void( __thiscall* )( void*, int, int, int, int ), r, g, b, a ); // 15

	// draws a rectangle colored with the current drawcolor using the white material
	virtual_fn( DrawFilledRect( int x0, int y0, int x1, int y1 ), 16, void( __thiscall* )( void*, int, int, int, int ), x0, y0, x1, y1 ); // 16

	// draws an unfilled rectangle in the current drawcolor
	virtual_fn( DrawOutlinedRect( int x0, int y0, int x1, int y1 ), 18, void( __thiscall* )( void*, int, int, int, int ), x0, y0, x1, y1 ); // 18

	// draws a line!
	virtual_fn( draw_line( int x0, int y0, int x1, int y1 ), 19, void( __thiscall* )( void*, int, int, int, int ), x0, y0, x1, y1 ); // 19

	// sets the Text font
	virtual_fn( draw_set_Text_font( HFont font ), 23, void( __thiscall* )( void*, HFont ), font ); // 23

	// sets the Text color
	virtual_fn( SetTextColor( int r, int g, int b, int a ), 25, void( __thiscall* )( void*, int, int, int, int ), r, g, b, a ); // 25

	// Text rendering location
	virtual_fn( draw_set_Text_pos( int x, int y ), 26, void( __thiscall* )( void*, int, int ), x, y ); // 26

	// renders a Text buffer
	virtual_fn( DrawPrintText( const wchar_t* Text, int Text_length ), 28, void( __thiscall* )( void*, const wchar_t*, int, int ), Text, Text_length, 0 ); // 28


	virtual_fn(set_Texture_rgba(int id, const unsigned char* rgba, int wide, int tall), 37, void(__thiscall*)(void*, int, const unsigned char*, int, int), id, rgba, wide, tall); // 37

	// binds a Texture
	virtual_fn( draw_set_Texture( int id ), 38, void( __thiscall* )( void*, int ), id ); // 38

	// checks if Texture is valid
	virtual_fn( is_Texture_id_valid( int id ), 42, bool( __thiscall* )( void*, int ), id ); // 42

	// loads a particular Texture (material)
	virtual_fn( create_new_Texture_id( bool procedural = false ), 43, int( __thiscall* )( void*, bool ), procedural ); // 43

	// returns the screen size
	virtual_fn( get_screen_size( int& wide, int& tall ), 44, void( __thiscall* )( void*, int&, int& ), wide, tall ); // 44

	// unlocks the cursor
	virtual_fn( unlock_cursor( void ), 66, void( __thiscall* )( void* ) ); // 66

	// locks the cursor
	virtual_fn( lock_cursor( void ), 67, void( __thiscall* )( void* ) ); // 67

	// creates a new empty font
	virtual_fn( create_font( void ), 71, HFont( __thiscall* )( void* ) ); // 71

	// adds glyphs to a font created by CreateFont()
	virtual_fn( set_font_glyph_set( HFont font, const char* font_name, int tall, int weight, int blur, int scan_lines, int flags, int min_range, int max_range ), 72, void( __thiscall* )( void*, HFont, const char*, int, int, int, int, int, int, int ), font, font_name, tall, weight, blur, scan_lines, flags, min_range, max_range ); // 72

	// adds a custom font file (only supports true type font files (.ttf) for now)
	virtual_fn( add_custom_font_file( const char* font_file_name ), 73, bool( __thiscall* )( void*, const char* ), font_file_name ); // 73

	// returns the max height of a font
	virtual_fn( get_font_tall( HFont font ), 74, int( __thiscall* )( void*, HFont ), font ); // 74

	// returns the pixel width of a single character
	virtual_fn( get_font_tall( HFont font, int ch ), 78, int( __thiscall* )( void*, HFont, int ), font, ch ); // 78

	// returns the area of a Text string, including newlines
	virtual_fn( get_Text_size( HFont font, const wchar_t* Text, int& wide, int& tall ), 79, void( __thiscall* )( void*, unsigned long, const wchar_t*, int&, int& ), font, Text, wide, tall ); // 79

	// draws an outlined circle in the current drawcolor
	virtual_fn( draw_outlined_circle( int x, int y, int radius, int segments ), 103, void( __thiscall* )( void*, int, int, int, int ), x, y, radius, segments ); // 103

	// draws a Textured polygon
	virtual_fn( draw_Textured_polygon( int n, vertex_t* vertices, bool clip = true ), 106, void( __thiscall* )( void*, int, vertex_t*, bool ), n, vertices, clip ); // 106

	// notification of a new screen size
	virtual_fn( on_screen_size_change( int old_width, int old_height ), 116, int( __thiscall* )( void*, int, int ), old_width, old_height ); // 116

	// draws a fade with the current draw color oriented according to argument
	virtual_fn( draw_filled_rect_fade( int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool horizontal ), 123, void( __thiscall* )( void*, int, int, int, int, unsigned int, unsigned int, bool ), x0, y0, x1, y1, alpha0, alpha1, horizontal ); // 123

	// adds a bitmap font file
	virtual_fn( add_bitmap_font_file( const char* font_file_name ), 127, bool( __thiscall* )( void*, const char* ), font_file_name ); // 127

	// https://www.unknowncheats.me/forum/counterstrike-Global-offensive/333981-using-clipping-viewports-isurface.html
	virtual_fn( get_clip_rect( int& x0, int& y0, int& x1, int& y1 ), 146, void( __thiscall* )( void*, int, int, int, int ), x0, y0, x1, y1 ); // 146
	virtual_fn( set_clip_rect( int x0, int y0, int x1, int y1 ), 147, void( __thiscall* )( void*, int, int, int, int ), x0, y0, x1, y1 ); // 147

#define offset_fn( function_name, type, offset ) \
	type& function_name { \
		return *reinterpret_cast< type* >( std::uintptr_t( this ) + offset ); \
	};

	offset_fn(m_bClippingEnabled(), bool, 0x280);

	void clip(const vector2d_t& pos, const vector2d_t& size, bool override)
	{
		RECT rec;
		rec.left = static_cast<LONG>(pos.x);
		rec.top = static_cast<LONG>(pos.y);
		rec.right = static_cast<LONG>(pos.x + size.x + 1);
		rec.bottom = static_cast<LONG>(pos.y + size.y + 1);


		set_clip_rect(rec.left, rec.top, rec.right, rec.bottom);
	}

//	void set_Texture_rgba(int id, const unsigned char* rgba, int wide, int tall, int a, bool b) {
	//	using OriginalFn = void(__thiscall*)(ISurface*, int, const unsigned char*, int, int, int, bool);
	//	return (*(OriginalFn**)this)[37](this, id, rgba, wide, tall, a, b);
	//}

	//virtual_fn(set_Texture_rgba(int id, const unsigned char* rgba, int wide, int tall), 37, void(__thiscall*)(void*, int, const unsigned char*, int, int, int , bool), id, rgba, wide, tall, 1, false);

	void draw_Textured_rect(int x, int y, int width, int height) {
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int);
		return (*(OriginalFn**)this)[41](this, x, y, x + width, y + height);
	}

	RECT get_Text_size_rect(DWORD font, const char* Text) {
		if (this != nullptr) {
			size_t origsize = strlen(Text) + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t wcstring[newsize];
			mbstowcs_s(&convertedChars, wcstring, origsize, Text, _TRUNCATE);

			RECT rect; int x, y;
			get_Text_size(font, wcstring, x, y);
			rect.left = x; rect.bottom = y;
			rect.right = x;
			return rect;
		}
	}
};