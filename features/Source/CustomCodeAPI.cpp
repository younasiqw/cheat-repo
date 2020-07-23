#include "../Headers/CustomCodeAPI.h"

namespace MainAPI {
	namespace Drawing {
		std::vector<std::string> Lines;
		int TextDrawPos = 0;
		void DrawOnPaint( ) {

			std::string Captured = CaptureText( );

			for ( int i = 0; i < Lines.size( ); i++ ) {
				Drawing::Text::Text( Lines.at( i ), Fonts::Menu, 10, 10 + (i * 20), color( 255, 255, 255 ) );

			}
			//std::cout << Captured << std::endl;
			if ( Captured.find( "text: " ) ) {
				//std::cout << "found " << std::endl;
				std::string text = Captured.substr( 0, Captured.size( ) );

				Drawing::Text::Text( text, Fonts::Menu, 10, 10 + ( TextDrawPos ) , color( 255, 255, 255 ) );
			}
		}

		std::string CaptureText( ) {
			static std::string item;
			for ( int i = 0; i < 255; i++ ) {
				if ( menu.GetKeyPress( i ) ) {
					if ( i == VK_BACK && strlen( item.c_str( ) ) != 0  ) {
						std::string tmp = item.substr( 0, strlen( item.c_str( ) ) - 1 );
						item = tmp;
					}

					const char* KeyArray[ 254 ];
					if ( GetAsyncKeyState( VK_SHIFT ) ) {
						KeyArray[ i ] = menu.StrDigitsUpper[ i ];
					}
					else {
						KeyArray[ i ] = menu.StrDigitsLower[ i ];
					}

					if ( i != NULL && KeyArray[ i ] != nullptr && KeyArray[ i ] != "NIGGER") {
						item += KeyArray[ i ];
						return "";
					}

					if ( KeyArray[ i ] == "NIGGER" ) {
						Lines.push_back( item );
						item = "";
						TextDrawPos += 20;
						//std::cout << "ENTER" << std::endl;
						return "";
					}
				}
			}
			return item;
		}

		void ChoseFont( HFont& font, Fonts cFont ) {
			switch ( cFont ) {
			case Fonts::Menu: font = Render::Fonts::menu; break;
			case Fonts::ESP: font = Render::Fonts::visuals; break;
			case Fonts::Large: font = Render::Fonts::title; break;
			}
		}

		namespace Text {
			void Text( std::string text, Fonts font, int x, int y, color color ) {
				HFont cFont;
				Drawing::ChoseFont( cFont, font );

				Render::Text( cFont, text, x, y, false, color.r, color.g, color.b, color.a );
			}

			void TextW2S( std::string text, Fonts font, Vector3D position, color color ) {
				HFont cFont;
				Drawing::ChoseFont( cFont, font );

				Vector3D W2S;

				if ( !Interfaces::DebugOverlay->WorldToScreen( position, W2S ) )
					return;

				Render::Text( cFont, text, W2S.x, W2S.y, false, color.r, color.g, color.b, color.a );
			}
		};

		namespace Rect {

			void Rect( int x, int y, int w, int h, color color ) {
				Render::FilledRect( x, y, w, h, color );
			}

			void RectW2S( Vector3D position, int w, int h, color color ) {
				Vector3D W2S;

				if ( !Interfaces::DebugOverlay->WorldToScreen( position, W2S ) )
					return;

				Render::FilledRect( W2S.x, W2S.y, w, h, color );
			}
		};
	};
};