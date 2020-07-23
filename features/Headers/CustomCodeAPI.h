#pragma once
#include "../../hooks/hooks.h"
#include "../../menu/menu.h"

namespace MainAPI {

	namespace Drawing {

		void DrawOnPaint( );
		std::string CaptureText( );

		enum Fonts {
			Menu = 0,
			ESP,
			Large
		};

		void ChoseFont( HFont& font, Fonts cFont );

		namespace Text {
			void Text( std::string text, Fonts font, int x, int y, color color );

			void TextW2S( std::string text, Fonts font, Vector3D position, color color );
		};

		namespace Rect {

			void Rect( int x, int y, int w, int h, color color );

			void RectW2S( Vector3D position, int w, int h, color color );
		};
	};
};