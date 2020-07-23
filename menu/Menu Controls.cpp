#include "menu.h"
#include "../render/Textures.h"
int selected_item = 0;
int addy = 32;
int addx = 223;

void cmenu::AddCheckbox( std::string name, bool* control, ctabs tab, bool keybind, int* ckey, int subg, int y, std::string tip ) {
	int pos_y = 0;
	if ( SelectedTab == tab ) {
		/*if (is_sub_g) {
			if (selected_subgroup != subg)
				return;
		}*/
		if ( y == 0 )
			y = MenuY;

		if ( position_y < y + gbheight + 55 ) {

			int resize_width = mwidth / 3.6;

			int check_rect[ 4 ] = { itemname_x, position_y + 3, resize_width, 11 };
			static bool pressed = false;
			if ( !open_colorpicker ) {
				if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( check_rect[ 0 ], check_rect[ 1 ], check_rect[ 2 ], check_rect[ 3 ] ) ) {
					if ( pressed )
						*control = !*control;
					pressed = false;
				}
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( check_rect[ 0 ], check_rect[ 1 ], check_rect[ 2 ], check_rect[ 3 ] ) && !pressed ) pressed = true;
			}

			Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y + 1, false, 180, 180, 180, 255 );

			Render::RoundedRect( check_rect[ 0 ], check_rect[ 1 ], 11, 11, 2, color( 42, 42, 42 ) );

			if ( !open_colorpicker ) {
				if ( mouse_in_region( check_rect[ 0 ], check_rect[ 1 ], check_rect[ 2 ], check_rect[ 3 ] ) ) {
					Render::FilledRect( check_rect[ 0 ], check_rect[ 1 ], 11, 11, color( 50, 50, 50 ) );
					Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y + 1, false, 205, 205, 205, 255 );

				}
			}

			static std::string NameSelected = "";
			if ( mouse_in_region( check_rect[ 0 ], check_rect[ 1 ], check_rect[ 2 ], check_rect[ 3 ] ) ) {
				NameSelected = name;
			}

			if ( *control ) {
				Render::RoundedRect( check_rect[ 0 ] + 2, check_rect[ 1 ] + 2, 7, 7, 2, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );
				Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y + 1, false, 205, 205, 205, 255 );
			}
			if ( keybind ) {
				const char* key_name = "[None]";

				std::string str = key_name;
				std::wstring converted_Text = std::wstring( str.begin( ), str.end( ) );

				int width, height;
				Interfaces::Surface->get_Text_size( Render::Fonts::tab, converted_Text.c_str( ), width, height );

				bool open = false;
				bool pressed = false;
				static bool selected_open = false;
				static bool click_rest;
				static bool rest;
				static std::string name_selected;

				if ( !open_colorpicker ) {

					if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + ( resize_width + 20 ) - ( width / 0.99 ), position_y + 1, width, 20 ) && !click_rest )
					{
						name_selected = name;
						pressed = true;
						click_rest = true;
					}
					else if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + ( resize_width + 20 ) - ( width / 0.99 ), position_y + 1, width, 20 ) )
						click_rest = false;

					if ( pressed ) {
						if ( !rest )
							selected_open = !selected_open;
						rest = true;
					}
					else
						rest = false;

					if ( name_selected == name )
						open = selected_open;
				}

				static bool valid_name = false;
				static bool getting_key = false;
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + ( resize_width + 20 ) - ( width / 0.99 ), position_y + 1, width, 20 ) ) {
					if ( !getting_key ) {
						getting_key = true;
					}
				}

				static int key = 0;
				if ( getting_key ) {
					for ( int i = 0; i < 255; i++ ) {
						if ( GetKeyPress( i ) ) {
							if ( i == VK_ESCAPE ) {
								key = -1;
								*ckey = key;
								getting_key = false;
								return;
							}

							key = i;
							*ckey = key;
							getting_key = false;
							return;
						}
					}
				}
				if ( getting_key ) {
					key_name = "[---]";
				}
				else {
					char buf[ 128 ];
					if ( *ckey >= 0 ) {
						key_name = KeyStringsStick[ *ckey ];
						if ( key_name ) {
							valid_name = true;
						}
						else {
							if ( GetKeyNameText( *ckey << 16, buf, 127 ) ) {
								key_name = buf;
								valid_name = true;
							}
						}
					}
					if ( !valid_name ) {
						key_name = "[---]";
					}
				}
				Render::Text( Render::Fonts::menu, key_name, itemname_x + ( resize_width + 20 ) - ( width / 0.99 ), position_y + 1, false, 210, 210, 210 );
			}

			if ( name == NameSelected ) {
				if ( mouse_in_region( itemname_x, position_y + 3, resize_width, 11 ) )
					this->ToolTip = tip;
				else
					this->ToolTip = "";
			}
			position_y += mheight / 25;
		}
	}

	tab_items++;
	position_y2 += 17;
	previous_control = 0;
}
void cmenu::add_button( std::string name, bool* control, ctabs tab ) {

	if ( SelectedTab == tab ) {

	}
}
void cmenu::add_listbox( int* control, ctabs tab, std::vector<std::string> items ) {
	if ( SelectedTab == tab ) {

		Render::RoundedRect( itemname_x, position_y + 3, 125, items.size( ) * 20 + 3, 2, color( 42, 42, 42 ) );

		for ( int i = 0; i < items.size( ); i++ ) {
			int item_rect[ 4 ] = { itemname_x, position_y + 3 + ( i * 20 ), 125, 17 };


			if ( !open_colorpicker ) {
				if ( GetKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) )
					selected_cfg = i;
			}

			if ( i != items.size( ) - 1 )
				Render::FilledRect( item_rect[ 0 ] + 5, item_rect[ 1 ] + 20, item_rect[ 2 ] - 10, 1, color( 0, 0, 0, 100 ) );

			if ( selected_cfg == i ) {
				Render::Text( Render::Fonts::menu, items.at( i ), item_rect[ 0 ] + 9, item_rect[ 1 ] + 2, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
			}
			else {
				Render::Text( Render::Fonts::menu, items.at( i ), item_rect[ 0 ] + 9, item_rect[ 1 ] + 2, false, 205, 205, 205 );
			}
			*control = selected_cfg;
		}
		position_y += 10 + items.size( ) * 20;
	}
	//control_names.emplace_back(name);
}
void cmenu::AddMultiBox( std::string name, std::vector<std::string> items, bool* control, ctabs tab, int y, int subg ) {

	bool open = false;
	if ( SelectedTab == tab ) {
		/*if (is_sub_g) {
			if (selected_subgroup != subg)
				return;
		}*/

		if ( y == 0 )
			y = MenuY;

		if ( position_y < y + gbheight + 60 ) {

			int resize_width = mwidth / 3.6;

			if ( previous_control == 0 ) {
				position_y += 2;
			}
			Render::RoundedRect( itemname_x + 19, position_y + 17, resize_width, 14, 3, color( 42, 42, 42 ) );

			if ( mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) ) {
				Render::RoundedRect( itemname_x + 19, position_y + 17, resize_width, 14, 3, color( 50, 50, 50 ) );
			}

			//Render::OutlinedRect(itemname_x + 19, position_y + 17, 125, 16, color(0, 0, 0)); // outline box
			Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y, false, 205, 205, 205, 255 );


			bool pressed = false;
			static bool selected_open = false;
			static bool click_rest;
			static bool rest;
			static std::string name_selected;

			if ( !open_colorpicker ) {
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) && !click_rest )
				{
					name_selected = name;
					pressed = true;
					click_rest = true;
				}
				else if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) )
					click_rest = false;

				if ( pressed ) {
					if ( !rest )
						selected_open = !selected_open;
					rest = true;
				}
				else
					rest = false;

				if ( name_selected == name )
					open = selected_open;
			}
			static bool multi_pressed = false;
			bool has_items = false;
			std::string items_str = "";
			int last_item = 0;

			if ( open ) {
				Render::RoundedRect( itemname_x + 19, position_y + 36, resize_width, items.size( ) * 15, 3, color( 42, 42, 42 ) ); //background
				for ( int i = 0; i < 4; i++ ) {
					Render::line( itemname_x + ( resize_width + 6 ) + i, position_y + 23 + i, itemname_x + ( resize_width + 12 ) - i, position_y + 23 + i, color( 190, 190, 190, 160 ) );
				}
			}
			else {
				for ( int i = 0; i < 4; i++ ) {
					Render::line( itemname_x + ( resize_width + 6 ) + i, position_y + 25 - i, itemname_x + ( resize_width + 12 ) - i, position_y + 25 - i, color( 190, 190, 190, 160 ) );
				}
			}

			for ( int i = 0; i < items.size( ); i++ ) {
				int item_rect[ 4 ] = { itemname_x + 19, position_y + 34 + ( i * 15 ), resize_width, 16 };


				if ( open ) {
					if ( !open_colorpicker ) {
						if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) ) {
							if ( multi_pressed )
								control[ i ] = !control[ i ];
							multi_pressed = false;
						}

						if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) && !multi_pressed )
							multi_pressed = true;

					}
					if ( control[ i ] || mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) ) {
						Render::Text( Render::Fonts::menu, items.at( i ), item_rect[ 0 ] + 4, item_rect[ 1 ] + 2, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
					}
					else {
						Render::Text( Render::Fonts::menu, items.at( i ), item_rect[ 0 ] + 4, item_rect[ 1 ] + 2, false, 205, 205, 205 );
					}
				}
			}
			int size = 0;
			for ( int i = 0; i < items.size( ); i++ ) {
				RECT Text_size = Interfaces::Surface->get_Text_size_rect( Render::Fonts::tab, items_str.c_str( ) );
				RECT will_add = Interfaces::Surface->get_Text_size_rect( Render::Fonts::tab, items.at( i ).c_str( ) );

				if ( control[ i ] ) {
					size++;
					has_items = true;

					if ( Text_size.right + will_add.right < resize_width - 10 ) {
						if ( size > 1 ) {
							items_str += ", " + items.at( i );
						}
						else {
							items_str += items.at( i );
						}
					}
					else
						items_str += ".";
				}
			}

			if ( !has_items )
				Render::Text( Render::Fonts::menu, "Off", itemname_x + 23, position_y + 17, false, 180, 180, 180, 255 );
			else
				Render::Text( Render::Fonts::menu, items_str, itemname_x + 23, position_y + 17, false, 205, 205, 205, 255 );

			if ( open )
				position_y += mheight / 12 + items.size( ) * 17;
			else
				position_y += mheight / 12;
		}
	}

	tab_items++;
	previous_control = 1;

	if ( open )
		position_y2 += 38 + items.size( ) * 18;
	else
		position_y2 += 40;

}
void cmenu::AddCombobox( std::string name, std::vector<std::string> items, int* control, ctabs tab, int y, int subg, std::string tooltip ) {

	bool open = false;
	if ( SelectedTab == tab ) {
		/*if (is_sub_g) {
			if (selected_subgroup != subg)
				return;
		}*/

		if ( y == 0 )
			y = MenuY;

		if ( position_y < y + gbheight + 50 ) {

			int resize_width = mwidth / 3.6;
			if ( previous_control == 0 ) {
				position_y += 2;
			}
			Render::RoundedRect( itemname_x + 19, position_y + 17, resize_width, 14, 3, color( 42, 42, 42 ) );

			Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y, false, 180, 180, 180, 255 );


			if ( !open_colorpicker ) {
				if ( mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) ) {
					Render::RoundedRect( itemname_x + 19, position_y + 17, resize_width, 14, 3, color( 50, 50, 50 ) );
					Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y, false, 205, 205, 205 );

				}
			}


			bool pressed = false;
			static bool selected_open = false;
			static bool click_rest;
			static bool rest;
			static std::string name_selected;

			if ( !open_colorpicker ) {
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) && !click_rest )
				{
					name_selected = name;
					pressed = true;
					click_rest = true;
				}
				else if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) )
					click_rest = false;

				if ( pressed ) {
					if ( !rest )
						selected_open = !selected_open;
					rest = true;
				}
				else
					rest = false;
			}

			if ( name_selected == name )
				open = selected_open;

			if ( open )
				Render::RoundedRect( itemname_x + 19, position_y + 33, resize_width, items.size( ) * 15, 3, color( 42, 42, 42 ) ); //background

			if ( open ) {
				for ( int i = 0; i < 4; i++ ) {
					Render::line( itemname_x + ( resize_width + 6 ) + i, position_y + 23 + i, itemname_x + ( resize_width + 12 ) - i, position_y + 23 + i, color( 190, 190, 190, 160 ) );
				}
			}
			else {
				for ( int i = 0; i < 4; i++ ) {
					Render::line( itemname_x + ( resize_width + 6 ) + i, position_y + 25 - i, itemname_x + ( resize_width + 12 ) - i, position_y + 25 - i, color( 190, 190, 190, 160 ) );
				}
			}

			if ( *control >= 0 && *control <= items.size() ) { 
				Render::Text( Render::Fonts::menu, items.at( *control ), itemname_x + 23, position_y + 17, false, 205, 205, 205, 255 );
			}
			else {
				*control = 0;
			}

			for ( int i = 0; i < items.size( ); i++ ) {
				int item_rect[ 4 ] = { itemname_x + 19, position_y + 31 + ( i * 15 ), resize_width, 16 };

				if ( open ) {
					if ( !open_colorpicker ) {
						if ( GetKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) )
							( *control ) = i;
					}

					if ( ( *control ) == i || mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) ) {
						Render::Text( Render::Fonts::menu, items.at( i ), item_rect[ 0 ] + 4, item_rect[ 1 ] + 2, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
					}
					else {
						Render::Text( Render::Fonts::menu, items.at( i ), item_rect[ 0 ] + 4, item_rect[ 1 ] + 2, false, 205, 205, 205 );
					}
				}
			}

			if ( open )
				position_y += ( mheight / 13 ) + items.size( ) * 17;
			else
				position_y += mheight / 11.5;

			if ( name == name_selected ) {
				if ( mouse_in_region( itemname_x + 19, position_y + 13, resize_width, 21 ) )
					this->ToolTip = tooltip;
				else
					this->ToolTip = "";
			}
		}
	}
	tab_items++;
	previous_control = 1;
}

int number = 0;
void cmenu::AddSlider( std::string name, int max, std::string add, int* control, ctabs tab, int subg, int y, int over_x, std::string tooltip ) {
	if ( SelectedTab == tab ) {
		/*if (is_sub_g) {
			if (selected_subgroup != subg)
				return;
		}*/

		if ( y == 0 )
			y = MenuY;

		int resize_width = mwidth / 3.6;

		if ( position_y < y + gbheight + 50 ) {
			/*if (position_y > MenuY + mheight) {
				itemname_x = MenuX + addx;
				position_y = MenuY + addy;
				started_new = true;
			}*/
			if ( previous_control == 0 ) {
				position_y += 2;
			}
			int item_rect[ 4 ] = { itemname_x + 19, position_y + 17, resize_width, 12 };

			Render::RoundedRect( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ], 2, color( 42, 42, 42 ) );
			//Render::OutlinedRect(item_rect[0], item_rect[1], item_rect[2], item_rect[3], color(0, 0, 0)); // outline box

			Render::Text( Render::Fonts::menu, name, itemname_x + 20, position_y, false, 180, 180, 180, 255 );

			float pixel_add = resize_width - 2.f;
			float pixelValue = max / pixel_add;

			int possible_value = ( get_mouse_position( ).x - ( item_rect[ 0 ] ) ) * pixelValue;
			int possible_position = ( ( ( ( get_mouse_position( ).x - ( itemname_x + 19 ) ) * pixelValue ) / pixelValue ) );

			static std::string NameSelected = "";

			if ( !open_colorpicker ) {
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 12 ) )
					*control = ( get_mouse_position( ).x - ( itemname_x + 19 ) ) * pixelValue;

				if ( mouse_in_region( item_rect[ 0 ], item_rect[ 1 ] - 3, item_rect[ 2 ], item_rect[ 3 ] + 6 ) )
					NameSelected = name;

				if ( mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) && !( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) ) {
					Render::RoundedRect( item_rect[ 0 ] + 1, item_rect[ 1 ] + 1, possible_position, 10, 2, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 100 ) );
				}

				if ( *control > max )
					*control = max;

				if ( *control < 0.f )
					*control = 0.f;
			}
			if (*control != 0 && ( *control / pixelValue ) > 2 )
				Render::RoundedRect( item_rect[ 0 ] + 1, item_rect[ 1 ] + 1, ( *control / pixelValue ), 10, 2, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

			std::wstring converted_Text = std::wstring( add.begin( ), add.end( ) );
			int width, height;
			Interfaces::Surface->get_Text_size( Render::Fonts::tab, converted_Text.c_str(), width, height );

			if ( !open_colorpicker && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) ) {
				Render::Text( Render::Fonts::slider, std::to_string( possible_value ) + " " + add, item_rect[ 0 ] + item_rect[ 2 ] / 2 - 5 - (width * .7) + over_x, item_rect[ 1 ], false, 205, 205, 205 );
			}

			if ( name == NameSelected ) {
				if ( mouse_in_region( item_rect[ 0 ], item_rect[ 1 ] - 3, item_rect[ 2 ], item_rect[ 3 ] + 6 ) )
					this->ToolTip = tooltip;
				else
					this->ToolTip = "";
			}

			position_y += mheight / 13;
		}
	}

	tab_items++;

	previous_control = 2;
}
void cmenu::register_groupbox( std::string name, ctabs tab, type TYPE, int x, int y, std::string subname, int override_height ) {
	if ( SelectedTab == tab ) {
		is_sub_g = false;

		int override_h = 0;

		if ( override_height == 0 )
			override_h = 0;
		else
			override_h = override_height;


		if ( TYPE == type::GB_FULL ) {
			gbheight = mheight - 105 + override_h;
		}
		else if ( TYPE == type::GB_HALF ) {
			gbheight = ( mheight - 140 ) / 2 + override_h;

			if ( y == 81 ) {
				y = 80;
				gbheight += 10;
			}
		}

		static int w = mwidth / 2.41;

		gbwidth = mwidth / 2.41;

		if ( x == GROUP_X2 ) {
			x += mwidth / 2.41 - 200;
			gbwidth = mwidth / 2.41 + ( fabsf( w - ( mwidth / 2.41 ) ) * 0.4 );
		}

		if ( y != GROUP_FULL_Y ) {
			y += (previous_height / 2) - 155;
		}

		itemname_x = MenuX + x + 15;
		position_y = MenuY + y + 16;

		//Render::RoundedRect( MenuX + x - 1, MenuY + y - 1, gbwidth + 2, gbheight + override_h + 2, 5, color( 0, 0, 0, 100 ) );

		Render::RoundedRect( MenuX + x, MenuY + y, gbwidth, gbheight + override_h, 5, color( 53, 53, 53 ) );

		Render::RoundedRect( MenuX + x - 5, MenuY + y - 10, gbwidth + 10, 18, 4, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

		Render::Text( Render::Fonts::menu, name, MenuX + x + gbwidth / 2, MenuY + y - 9, true, 205, 205, 205 );

		previous_height = gbheight;
	}
	previous_group_type = TYPE;
}
void cmenu::register_subgroupbox( std::vector<std::string> name, ctabs tab, type TYPE, int x, int y, int* to_pass, int first, int second, int override_height ) {
	if ( SelectedTab == tab ) {

		is_sub_g = true;

		int override_h = 0;

		if ( override_height == 0 )
			override_h = 0;
		else
			override_h = override_height;


		if ( TYPE == type::GB_FULL ) {
			gbheight = mheight - 105 + override_h;
		}
		else if ( TYPE == type::GB_HALF ) {
			gbheight = ( mheight - 140 ) / 2 + override_h;

			if ( y == 81 ) {
				y = 80;
				gbheight += 10;
			}
		}

		static int w = mwidth / 2.41;

		gbwidth = mwidth / 2.41;

		if ( x == GROUP_X2 ) {
			x += mwidth / 2.41 - 200;
			gbwidth = mwidth / 2.41 + (fabsf(w - ( mwidth / 2.41 )) * 0.4);
		}

		if ( y != GROUP_FULL_Y ) {
			y += ( previous_height / 2 ) - 155;
		}

		itemname_x = MenuX + x + 15;
		position_y = MenuY + y + 16;

		//Render::RoundedRect( MenuX + x - 1, MenuY + y - 1, gbwidth + 2, gbheight + override_h + 2, 5, color( 0, 0, 0, 100 ) );

		Render::RoundedRect( MenuX + x, MenuY + y, gbwidth, gbheight + override_h, 5, color( 53, 53, 53 ) );

		Render::RoundedRect( MenuX + x - 5, MenuY + y - 10, gbwidth + 10, 18, 4, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

		Render::FilledRect( MenuX + x + gbwidth / 2, MenuY + y - 8, 1, 14, color( 205, 205, 205, 150 ) );

		Render::Text( Render::Fonts::menu, name.at( 0 ), MenuX + x + gbwidth / 2.84 - 20, MenuY + y - 9, true, 205, 205, 205 );

		Render::Text( Render::Fonts::menu, name.at( 1 ), MenuX + x + gbwidth / 1.2 - 20, MenuY + y - 9, true, 205, 205, 205 );

		if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( MenuX + x + gbwidth / 2.84 - 60, MenuY + y - 9, 70, 20 ) )
			*to_pass = first;

		if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( MenuX + x + gbwidth / 1.2 - 60, MenuY + y - 9, 70, 20 ) )
			*to_pass = second;

		if ( *to_pass == first )
			Render::Text( Render::Fonts::menu, name.at( 0 ), MenuX + x + gbwidth / 2.84 - 20, MenuY + y - 9, true, 240, 240, 240 );
		else
			Render::Text( Render::Fonts::menu, name.at( 1 ), MenuX + x + gbwidth / 1.2 - 20, MenuY + y - 9, true, 240, 240, 240 );
		
		previous_height = gbheight;
	}
	previous_group_type = TYPE;
}

void cmenu::AddTab( std::string name ) {

	std::wstring converted_Text = std::wstring( name.begin( ), name.end( ) );

	int width, height;
	Interfaces::Surface->get_Text_size( Render::Fonts::tab, converted_Text.c_str( ), width, height );


	int tab_rect[ 4 ] = { MenuX + (mwidth / 2.1) + ( TabIndex * (mwidth / 8.79) ) + 7 - ( width / 3 ), MenuY + 14, 62, 20 };

	if ( TabIndex > 0 )
		tab_rect[ 0 ] += 5;

	if ( TabIndex == 2 )
		tab_rect[ 0 ] += 6;

	if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( tab_rect[ 0 ], tab_rect[ 1 ], tab_rect[ 2 ], tab_rect[ 3 ] ) )
		SelectedTab = TabIndex;

	if ( SelectedTab == TabIndex )
		Render::Text( Render::Fonts::tab, name, tab_rect[ 0 ] + 9, tab_rect[ 1 ] + 2, false, 240, 240, 240 );
	else {
		if ( mouse_in_region( tab_rect[ 0 ], tab_rect[ 1 ], tab_rect[ 2 ], tab_rect[ 3 ] ) )
			Render::Text( Render::Fonts::tab, name, tab_rect[ 0 ] + 9, tab_rect[ 1 ] + 2, false, 240, 240, 240 );
		else
			Render::Text( Render::Fonts::tab, name, tab_rect[ 0 ] + 9, tab_rect[ 1 ] + 2, false, 180, 180, 180 );
	}
	TabIndex++;
}

void cmenu::AddTextField( std::string name, std::string* item, ctabs tab, int subg ) {
	if ( SelectedTab == tab ) {
		/*if (is_sub_g) {
			if (selected_subgroup != subg)
				return;
		}*/

		if ( position_y < MenuY + gbheight + 50 ) {

			int resize_width = mwidth / 3.6;

			Render::RoundedRect( itemname_x + 19, position_y + 17, resize_width, 14, 2, color( 42, 42, 42 ) );

			if ( mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) ) {
				Render::RoundedRect( itemname_x + 19, position_y + 17, resize_width, 14, 2, color( 50, 50, 50 ) );
			}

			Render::Text( Render::Fonts::menu, name, itemname_x + 19, position_y, false, 205, 205, 205, 255 );

			
			bool open = false;
			bool pressed = false;
			static bool selected_open = false;
			static std::string name_selected;
			static bool click_rest = false;

			if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) ) {
				if ( click_rest ) {
					selected_open = !selected_open;
					name_selected = name;
				}
				click_rest = false;
			}

			if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, resize_width, 14 ) && !click_rest )
				click_rest = true;


			if ( name_selected == name )
				open = selected_open;


			if ( open ) {

				/*static bool getting_key = false;
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + 19, position_y + 17, 125, 14 ) ) {
					if ( !getting_key ) {
						getting_key = true;
					}
					pressed_box = true;
				}
				else if ( GetAsyncKeyState( VK_LBUTTON ) && !mouse_in_region( itemname_x + 19, position_y + 17, 125, 14 ) ) {
					pressed_box = false;
				}*/

				const char* strg = item->c_str( );

				//if ( pressed_box ) {
					for ( int i = 0; i < 255; i++ ) {
						if ( GetKeyPress( i ) ) {

							const char* KeyArray[ 254 ];
							if ( GetAsyncKeyState( VK_SHIFT ) ) {
								KeyArray[ i ] = menu.StrDigitsUpper[ i ];
							}
							else {
								KeyArray[ i ] = menu.StrDigitsLower[ i ];
							}


							if ( i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT || i == VK_END || i == VK_LBUTTON ) {
								selected_open = false;
							}

							if ( i == VK_BACK && strlen( strg ) != 0 ) {
								std::string tmp = item->substr( 0, strlen( strg ) - 1 );
								*item = tmp;
							}

							if ( strlen( strg ) < 25 && i != NULL && KeyArray[ i ] != nullptr ) {
								*item += KeyArray[i];
								return;
							}

							if ( strlen( strg ) < 25 && i == 32 ) {
								*item += " ";
								return;
							}
						}
					}
				//}
				std::wstring converted_Text;
				if ( item->size() > 0 )
					converted_Text = std::wstring( item->begin( ), item->end( ) );

				int width, height;
				Interfaces::Surface->get_Text_size( Render::Fonts::tab, converted_Text.c_str( ), width, height );

				int curtime = Interfaces::GlobalVarsBase->CurrentTime;
				if ( curtime % 2 == 0/* && pressed_box*/ ) {
					if ( *item == "" )
						Render::Text( Render::Fonts::menu, "|", itemname_x + 21, position_y + 16, false, 205, 205, 205, 120 );
					else
						Render::Text( Render::Fonts::menu, "|", itemname_x + 23 + ( width * .86 ), position_y + 16, false, 205, 205, 205, 120 );

				}

			}
			//else {
			//	Text = "";
			//}
			Render::Text( Render::Fonts::menu, *item, itemname_x + 22, position_y + 17, false, 205, 205, 205 );
			if ( *item == "" ) {
				Render::Text( Render::Fonts::menu, "Begin Typing", itemname_x + 22, position_y + 17, false, 205, 205, 205, 70 );
			}

			position_y += mheight / 12;
		}
	}

	tab_items++;
	previous_control = 3;
}
void cmenu::AddColorPicker( std::string name, color2& item, ctabs tab, bool dalpha, int y, int subg ) {
	if ( SelectedTab == tab ) {
		/*if (is_sub_g) {
			if (selected_subgroup != subg)
				return;
		}*/

		if ( position_y < MenuY + gbheight + 55 ) {

			int yoffset = position_y + 10 + y;
			int xoffset = itemname_x + 330;

			color rainbow = { 0, 0, 0 };

			int resize_width = mwidth / 3.6;

			bool pressed = false;
			bool open = false;
			static bool selectedOpened = false;
			static bool clickRest;
			static bool rest;
			static std::string name_selected;

			bool pressed1 = false;
			bool open1 = false;
			static bool selectedOpened1 = false;
			static bool clickRest1;
			static bool rest1;
			static std::string name_selected1;

			if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width, position_y - 12 + y, 15, 8 ) && !clickRest ) {
				name_selected = name;
				pressed = true;
				clickRest = true;
			}
			else if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width, position_y - 12 + y, 15, 8 ) )
				clickRest = false;

			if ( pressed ) {
				if ( !rest )
					selectedOpened = !selectedOpened;
				rest = true;
			}
			else
				rest = false;

			if ( name_selected == name )
				open = selectedOpened;
			else
				open = false;

			if ( open ) {

				if ( dalpha )
					Render::RoundedRect( itemname_x + resize_width + 23, position_y - 15 + y, 150, 128, 2, color( 42, 42, 42 ) ); //background
				else
					Render::RoundedRect( itemname_x + resize_width + 23, position_y - 15 + y, 134, 128, 2, color( 42, 42, 42 ) ); //background

				cTextures rainbow = cTextures( 102, 115, 115 );
				Global::Texture_id = Interfaces::Surface->create_new_Texture_id( true );
				create_Texture( rainbow );

				Interfaces::Surface->draw_Textured_rect( itemname_x + resize_width + 26, position_y - 12 + y, 115, 112 );

				POINT p; COLORREF col;
				HDC hDC; BOOL b;

				hDC = GetDC( NULL );
				b = GetCursorPos( &p );
				col = GetPixel( hDC, p.x, p.y );
				ReleaseDC( GetDesktopWindow( ), hDC );

				static color SelectedColor = item;

				//static vector2d_t old_pos;
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 26, yoffset - 21, 115, 112 ) ) {
					SelectedColor.r = GetRValue( col );
					SelectedColor.g = GetGValue( col );
					SelectedColor.b = GetBValue( col );
				}
				else if ( GetAsyncKeyState( VK_LBUTTON ) && !mouse_in_region( itemname_x + resize_width - 9, yoffset - 26, 185, 132 ) ) {
					selectedOpened = false;
				}

				Render::FilledRect( itemname_x + resize_width + 144, yoffset - 22, 10, 112, color( 255, 255, 255 ));
				auto FilledRectFade = [ ] ( int x, int y, int w, int h, int a1, int a2, bool horiz ) -> void {
					Interfaces::Surface->draw_filled_rect_fade( x, y, x + w, y + h, a1, a2, horiz );
				};
				Interfaces::Surface->DrawSetColor( SelectedColor.r, SelectedColor.g, SelectedColor.b, 255 );
				FilledRectFade( itemname_x + resize_width + 144, yoffset - 22, 10, 112, 0, 255, false );

				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 144, yoffset - 22, 10, 110 ) ) {
					item.r = GetRValue( col );
					item.g = GetGValue( col );
					item.b = GetBValue( col );
				}

				if ( dalpha ) {
					/*int x = itemname_x + resize_width + 167;
					int y = yoffset - 22;
					for ( int i = 0; i < 12; i++ ) {

						int size = 9;
						//Render::FilledRect( x, y, size, size, color( 100, 100, 100 ) );
						x += size, y += size;

						if ( i % 2 == 0 )
							x -= size * 2;

						std::cout << i << std::endl;
					}*/

					Render::FilledRect( itemname_x + resize_width + 157, yoffset - 22, 10, 112, color( 255, 255, 255 ) );
					Interfaces::Surface->DrawSetColor( 0, 0, 0, 255 );
					FilledRectFade( itemname_x + resize_width + 157, yoffset - 22, 10, 112, 0, 255, false );

					if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 157, yoffset - 22, 20, 111 ) )
						item.a = GetRValue( col );
				}
				else
					item.a = 255;

				Render::Text( Render::Fonts::slider, "RGBA: " + std::to_string( item.r ) + ", " + std::to_string( item.g ) + ", " + std::to_string( item.b ) + ", " + std::to_string( item.a ), itemname_x + resize_width + 26, yoffset + 92, false, 205, 205, 205 );

				selectedOpened1 = false;
			}

			if ( selectedOpened )
				open_colorpicker = true;
			else
				open_colorpicker = false;

			rainbow.r = item.r;
			rainbow.g = item.g;
			rainbow.b = item.b;
			rainbow.a = 255;

			if ( GetAsyncKeyState( VK_RBUTTON ) && mouse_in_region( itemname_x + resize_width, position_y - 15 + y, 16, 10 ) && !clickRest1 ) {
				name_selected1 = name;
				pressed1 = true;
				clickRest1 = true;
			}
			else if ( !GetAsyncKeyState( VK_RBUTTON ) && mouse_in_region( itemname_x + resize_width, position_y - 15 + y, 16, 10 ) )
				clickRest1 = false;

			if ( pressed1 ) {
				if ( !rest1 )
					selectedOpened1 = !selectedOpened1;
				rest1 = true;
			}
			else
				rest1 = false;

			if ( name_selected1 == name )
				open1 = selectedOpened1;
			else
				open1 = false;

			if ( open1 ) {
				Render::RoundedRect( itemname_x + resize_width + 23, position_y - 15 + y, 40, 32, 2, color( 42, 42, 42 ) ); //c+p background

				if ( mouse_in_region( itemname_x + resize_width + 25, position_y - 14 + y, 35, 14 ) )
					Render::Text( Render::Fonts::menu, "Copy", itemname_x + resize_width + 30, position_y - 14 + y, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );
				else
					Render::Text( Render::Fonts::menu, "Copy", itemname_x + resize_width + 30, position_y - 14 + y, false, 205, 205, 205 );

				if ( mouse_in_region( itemname_x + resize_width + 25, position_y + y, 35, 14 ) )
					Render::Text( Render::Fonts::menu, "Paste", itemname_x + resize_width + 30, position_y + y, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );
				else
					Render::Text( Render::Fonts::menu, "Paste", itemname_x + resize_width + 30, position_y + y, false, 205, 205, 205 );

				static bool pressedc = false;
				if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 25, position_y - 14 + y, 35, 14 ) ) {
					if ( pressedc )
						stored_color = item;
					pressedc = false;
				}
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 25, position_y - 14 + y, 35, 14 ) && !pressedc ) pressedc = true;

				static bool pressedp = false;
				if ( !GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 25, position_y + y, 35, 14 ) ) {
					if ( pressedp )
						item = stored_color;
					pressedp = false;
				}
				if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( itemname_x + resize_width + 25, position_y + y, 35, 14 ) && !pressedp ) pressedp = true;

				selectedOpened = false;
			}
			//Render::Text(Render::Fonts::slider, std::to_string(stored_color.r) + ", " + std::to_string(stored_color.g) + ", " + std::to_string(stored_color.b) + ", " + std::to_string(stored_color.a), 0, 0, false, 205, 205, 205);

			Render::RoundedRect(itemname_x + resize_width, position_y - 12 + y, 15, 8, 2, rainbow ); //the preivew box
		}
	}
}
std::string to_str( int integer ) {

	switch ( integer ) {
	case 0: return "Aimbot"; break;
	case 1: return "Visuals"; break;
	case 2: return "Misc"; break;
	case 3: return "Config"; break;
	}
}
static int selected_weapon = 0;
std::string selected_weaponf = "";
void cmenu::draw_weapons_group( ) {
	//this->register_groupbox("Application", TAB_SKINS, GB_FULL, GROUP_X2, GROUP_FULL_Y); {

	std::vector<std::string> valid_items;
	std::vector<int> valid_indecies;

	static bool pressed_item[ 50 ];
	static int selected;
	int box_x = MenuX + 245, box_y = MenuY + 58;
	static std::string Text = "";

	int index = 0;
	for ( int i = 0; i < weapons.size( ); i++ ) {
		std::transform( weapons.at( i ).begin( ), weapons.at( i ).end( ), weapons.at( i ).begin( ), ::tolower );

		if ( weapons.at( i ).find( Text ) != std::string::npos ) {
			valid_items.emplace_back( weapons.at( i ) );

		}
	}
	static bool pressed_search = false;

	static bool getting_key = false;
	if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( box_x + 27, box_y + 48, 163, 18 ) ) {
		if ( !getting_key ) {
			getting_key = true;
		}
		pressed_search = true;
	}
	else if ( GetAsyncKeyState( VK_LBUTTON ) && !mouse_in_region( box_x + 27, box_y + 48, 163, 18 ) ) {
		pressed_search = false;
	}
	const char* strg = Text.c_str( );

	if ( pressed_search ) {
		for ( int i = 0; i < 255; i++ ) {
			if ( GetKeyPress( i ) ) {
				if ( i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT || i == VK_END ) {
					pressed_search = false;
				}

				if ( i == VK_BACK && strlen( strg ) != 0 ) {
					Text = Text.substr( 0, strlen( strg ) - 1 );
				}

				if ( strlen( strg ) < 28 && i != NULL && StrDigitsUpper[ i ] != nullptr ) {
					Text += StrDigitsUpper[ i ];
					return;
				}

				if ( strlen( strg ) < 28 && i == 32 ) {
					Text += " ";
					return;
				}
			}
		}
	}

	int y = box_y + 65;
	int x = box_x + 177;
	int slider_y = box_y + 45;
	static int old_pos = slider_y;
	static bool ever_changed = false;

	float viewportHeight = 230;
	float contentHeight = valid_items.size( ) * 15.94;

	float viewableRatio = viewportHeight / contentHeight; // 1/3 or 0.333333333n

	float scrollBarArea = viewportHeight; // 150px

	int max = scrollBarArea * viewableRatio;
	int thumbHeight = std::clamp( max, 3, 162 ); // 50px

	float scrollTrackSpace = contentHeight - viewportHeight; // (600 - 200) = 400 
	float scrollThumbSpace = viewportHeight - thumbHeight; // (200 - 50) = 150
	float scrollJump = scrollTrackSpace / scrollThumbSpace; //  (400 / 150 ) = 2.666666666666667

	static bool cont = true;

	if ( slider_y < y )
		slider_y = y;

	if ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( x, y, 15, viewportHeight ) ) {
		if ( get_mouse_position( ).y - thumbHeight / 2 > y&& get_mouse_position( ).y + thumbHeight / 2 - 2 < y + viewportHeight ) {
			slider_y = get_mouse_position( ).y - thumbHeight / 2;
			old_pos = get_mouse_position( ).y;
			std::cout << "hold in pos" << "\n";
		}
		else {
			if ( slider_y > y )
				slider_y = old_pos - thumbHeight / 2;
			else
				slider_y = y;

			std::cout << "hold out of pos" << "\n";
		}
	}
	else {
		if ( old_pos - thumbHeight / 2 > y&& old_pos - thumbHeight / 2 - 2 < y + viewportHeight ) {
			slider_y = old_pos - thumbHeight / 2;
			std::cout << "let go in pos" << "\n";
		}
	}


	Render::FilledRect( box_x + 27, box_y + 45, 163, 18, color( 42, 42, 42 ) ); //search box


	std::wstring converted_Text = std::wstring( Text.begin( ), Text.end( ) );
	int width, height;
	Interfaces::Surface->get_Text_size( Render::Fonts::tab, converted_Text.c_str( ), width, height );
	int curtime = Interfaces::GlobalVarsBase->CurrentTime;
	if ( curtime % 2 == 0 && pressed_search ) {
		if ( Text == "" )
			Render::Text( Render::Fonts::menu, "|", box_x + 31, box_y + 46, false, 205, 205, 205, 120 );
		else
			Render::Text( Render::Fonts::menu, "|", box_x + 33 + ( width * 0.84 ), box_y + 46, false, 205, 205, 205, 120 );
	}
	if ( Text == "" ) {
		Render::Text( Render::Fonts::menu, "Begin Typing", box_x + 31, box_y + 47, false, 205, 205, 205, 120 );
	}

	Render::Text( Render::Fonts::menu, Text, box_x + 31, box_y + 47, false, 205, 205, 205 ); //search test

	Render::FilledRect( box_x + 27, y, 163, 230, color( 49, 49, 49 ) ); //result box


	Render::FilledRect( x, y, 15, viewportHeight - 2, color( 48, 48, 48 ) );
	Render::FilledRect( x + 2, slider_y, 12, thumbHeight, color( 55, 55, 55 ) );
	Render::FilledRect( x, y, 1, viewportHeight - 1, color( 20, 20, 20, 180 ) ); //slider

	for ( int i = 0; i < valid_items.size( ); i++ ) {

		int x = box_x + 35, y = box_y + ( i * 16 ) + 71;


		int item_rect[ 4 ] = { x, y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ) - 2, 124, 18 };

		if ( GetKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) )
			selected_weapon = i;


		if ( y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ) > box_y + 64 ) { //check if lower than top
			if ( y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ) < box_y + 57 + viewportHeight ) { //check if higher than bottom


				if ( selected_weapon == i ) {
					Render::FilledRect( item_rect[ 0 ] + 38, item_rect[ 1 ] - 3, item_rect[ 2 ] - 35, item_rect[ 3 ] + 1, color( 50, 50, 50, 190 ) ); //idx
					Render::Text( Render::Fonts::menu, std::to_string( i + 1 ) + ". " + valid_items.at( i ), x, y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ), false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
					pressed_item[ selected_weapon ] = true;
					selected_weaponf = valid_items.at( i );
				}
				else {
					Render::Text( Render::Fonts::menu, std::to_string( i + 1 ) + ". " + valid_items.at( i ), x, y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ), false, 205, 205, 205 );
				}
			}
		}


	}

	std::cout << selected_weaponf << "\n";
	if ( pressed_item[ selected_weapon ] ) {



	}
	//}
}
static int selected_skin = 0;
void cmenu::draw_skins_group( ) {

	//this->register_groupbox("Skins", TAB_SKINS, GB_FULL, GROUP_X1, GROUP_FULL_Y); {
	std::vector<std::string> valid_items;
	std::vector<int> valid_indecies;

	static bool pressed_item[ 50 ];
	static int selected;
	int box_x = MenuX + 10, box_y = MenuY + 58;
	static std::string Text = "";

	int index = 0;
	for ( int i = 0; i < skins.size( ); i++ ) {
		if ( skins.at( i ).find( Text ) != std::string::npos ) {
			valid_items.emplace_back( skins.at( i ) );

			if ( skins.at( i ) == "groundwater" )
				index = 2;
			else if ( skins.at( i ) == "candy apple" )
				index = 3;
			else if ( skins.at( i ) == "forest ddpat" )
				index = 5;
			else if ( skins.at( i ) == "arctic Camo" )
				index = 6;
			else if ( skins.at( i ) == "hot rod" )
				index = 33;
			else if ( skins.at( i ) == "metallic ddpat" )
				index = 32;
			else if ( skins.at( i ) == "ossified" )
				index = 36;
			else if ( skins.at( i ) == "blaze" )
				index = 37;
			else if ( skins.at( i ) == "fade" )
				index = 38;
			else if ( skins.at( i ) == "bulldozer" )
				index = 39;
			else if ( skins.at( i ) == "night" )
				index = 40;
			else if ( skins.at( i ) == "orange ddpat" )
				index = 83;
			else if ( skins.at( i ) == "pink DDPAT" )
				index = 84;
			else if ( skins.at( i ) == "boom" )
				index = 174;
			else if ( skins.at( i ) == "fire serpent" )
				index = 180;
			else if ( skins.at( i ) == "golden koi" )
				index = 185;
			else if ( skins.at( i ) == "nitro" )
				index = 254;
			else if ( skins.at( i ) == "asiimov" )
				index = 255;
			else if ( skins.at( i ) == "guardian" )
				index = 257;
			else if ( skins.at( i ) == "atomic alloy" )
				index = 301;
			else if ( skins.at( i ) == "vulcan" )
				index = 302;
			else if ( skins.at( i ) == "dragon lore" )
				index = 344;
			else if ( skins.at( i ) == "man-o'-war" )
				index = 395;
			else if ( skins.at( i ) == "tiger tooth" )
				index = 409;
			else if ( skins.at( i ) == "damascus steel 1" )
				index = 410;
			else if ( skins.at( i ) == "marble fade 1" )
				index = 413;
			else if ( skins.at( i ) == "doppler 1" )
				index = 415;
			else if ( skins.at( i ) == "hyper beast" )
				index = 430;
			else if ( skins.at( i ) == "man-o'-war" )
				index = 432;
			else if ( skins.at( i ) == "hyper beast" )
				index = 537;
			else if ( skins.at( i ) == "chantico's fire" )
				index = 548;
			else if ( skins.at( i ) == "asiimov" )
				index = 551;
			else if ( skins.at( i ) == "wasteland rebel" )
				index = 586;
			else if ( skins.at( i ) == "desolate space" )
				index = 588;
			else if ( skins.at( i ) == "doppler 2" )
				index = 854;
			else if ( skins.at( i ) == "doppler 3" )
				index = 855;
			else if ( skins.at( i ) == "marble fade 2" )
				index = 856;
			else if ( skins.at( i ) == "damascus steel 2" )
				index = 857;
			else if ( skins.at( i ) == "crimson weave" )
				index = 10016;
			else if ( skins.at( i ) == "boom!" )
				index = 10027;
			else if ( skins.at( i ) == "vice" )
				index = 10048;
			else if ( skins.at( i ) == "pow!" )
				index = 10049;
			else if ( skins.at( i ) == "emerald" )
				index = 10057;
			else if ( skins.at( i ) == "case hardened" )
				index = 10060;
			else if ( skins.at( i ) == "crimson web" )
				index = 10061;


			valid_indecies.emplace_back( index );
		}
	}

	static bool pressed_search = false;

	static bool getting_key = false;
	if ( GetAsyncKeyState( VK_LBUTTON ) && mouse_in_region( box_x + 27, box_y + 48, 163, 18 ) ) {
		if ( !getting_key ) {
			getting_key = true;
		}
		pressed_search = true;
	}
	else if ( GetAsyncKeyState( VK_LBUTTON ) && !mouse_in_region( box_x + 27, box_y + 48, 163, 18 ) ) {
		pressed_search = false;
	}
	const char* strg = Text.c_str( );

	if ( pressed_search ) {
		for ( int i = 0; i < 255; i++ ) {
			if ( GetKeyPress( i ) ) {
				if ( i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT || i == VK_END ) {
					pressed_search = false;
				}

				if ( i == VK_BACK && strlen( strg ) != 0 ) {
					Text = Text.substr( 0, strlen( strg ) - 1 );
				}

				if ( strlen( strg ) < 28 && i != NULL && StrDigitsUpper[ i ] != nullptr ) {
					Text += StrDigitsUpper[ i ];
					return;
				}

				if ( strlen( strg ) < 28 && i == 32 ) {
					Text += " ";
					return;
				}
			}
		}
	}

	int y = box_y + 65;
	int x = box_x + 177;
	int slider_y = box_y + 45;
	static int old_pos = slider_y;
	static bool ever_changed = false;

	float viewportHeight = 230;
	float contentHeight = valid_indecies.size( ) * 15.94;

	float viewableRatio = viewportHeight / contentHeight; // 1/3 or 0.333333333n

	float scrollBarArea = viewportHeight; // 150px

	int max = scrollBarArea * viewableRatio;
	int thumbHeight = std::clamp( max, 3, 162 ); // 50px

	float scrollTrackSpace = contentHeight - viewportHeight; // (600 - 200) = 400 
	float scrollThumbSpace = viewportHeight - thumbHeight; // (200 - 50) = 150
	float scrollJump = scrollTrackSpace / scrollThumbSpace; //  (400 / 150 ) = 2.666666666666667

	static bool cont = true;

	if ( slider_y < y )
		slider_y = y;

	if ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( x, y, 15, viewportHeight ) ) {
		if ( get_mouse_position( ).y - thumbHeight / 2 > y&& get_mouse_position( ).y + thumbHeight / 2 - 2 < y + viewportHeight ) {
			slider_y = get_mouse_position( ).y - thumbHeight / 2;
			old_pos = get_mouse_position( ).y;
		}
		else {
			slider_y = old_pos - thumbHeight / 2;
		}
	}
	else {
		if ( old_pos - thumbHeight / 2 > y&& old_pos - thumbHeight / 2 - 2 < y + viewportHeight ) {
			slider_y = old_pos - thumbHeight / 2;
		}
	}


	Render::FilledRect( box_x + 27, box_y + 45, 163, 18, color( 42, 42, 42 ) ); //search box


	std::wstring converted_Text = std::wstring( Text.begin( ), Text.end( ) );
	int width, height;
	Interfaces::Surface->get_Text_size( Render::Fonts::tab, converted_Text.c_str( ), width, height );
	int curtime = Interfaces::GlobalVarsBase->CurrentTime;
	if ( curtime % 2 == 0 && pressed_search ) {
		if ( Text == "" )
			Render::Text( Render::Fonts::menu, "|", box_x + 31, box_y + 46, false, 205, 205, 205, 120 );
		else
			Render::Text( Render::Fonts::menu, "|", box_x + 33 + ( width * 0.84 ), box_y + 46, false, 205, 205, 205, 120 );
	}
	if ( Text == "" ) {
		Render::Text( Render::Fonts::menu, "Begin Typing", box_x + 31, box_y + 47, false, 205, 205, 205, 120 );
	}

	Render::Text( Render::Fonts::menu, Text, box_x + 31, box_y + 47, false, 205, 205, 205 ); //search test

	//Interfaces::Surface->m_bClippingEnabled() = true;
	//Interfaces::Surface->clip(vector2d_t(box_x + 27, box_y + 45 + 20), vector2d_t(163, 230), false);

	Render::FilledRect( box_x + 27, y, 163, 230, color( 49, 49, 49 ) ); //result box


	Render::FilledRect( x, y, 15, viewportHeight - 2, color( 48, 48, 48 ) );
	Render::FilledRect( x + 2, slider_y, 12, thumbHeight, color( 55, 55, 55 ) );
	Render::FilledRect( x, y, 1, viewportHeight - 1, color( 20, 20, 20, 180 ) ); //slider

	for ( int i = 0; i < valid_items.size( ); i++ ) {

		int x = box_x + 35, y = box_y + ( i * 16 ) + 71;

		int item_rect[ 4 ] = { x, y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ) - 2, 124, 18 };

		if ( GetKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) )
			selected_skin = i;


		if ( y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ) > box_y + 64 ) { //check if lower than top
			if ( y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ) < box_y + 57 + viewportHeight ) { //check if higher than bottom


				if ( selected_skin == i ) {
					Render::FilledRect( item_rect[ 0 ] + 38, item_rect[ 1 ] - 3, item_rect[ 2 ] - 35, item_rect[ 3 ] + 1, color( 50, 50, 50, 190 ) ); //idx
					Render::Text( Render::Fonts::menu, std::to_string( i + 1 ) + ". " + valid_items.at( i ), x, y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ), false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
					pressed_item[ selected_skin ] = true;
				}
				else {
					Render::Text( Render::Fonts::menu, std::to_string( i + 1 ) + ". " + valid_items.at( i ), x, y + ( ( ( box_y + 65 ) - slider_y ) * scrollJump ), false, 205, 205, 205 );
				}
			}
		}
	}
	//Interfaces::Surface->m_bClippingEnabled() = false;

	//std::cout << pressed_item[selected_skin] << "\t" << valid_indecies.at(selected_skin) << "\n" ;

	C_CSPlayer* Local = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );
	if ( !Local ) return;
	C_BaseCombatWeapon* weapon = Local->GetActiveWeapon( );
	if ( !weapon ) return;
	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData ) return;
	std::cout << "current weapon:" << WeaponData->weapon_name << "\n \n";
	if ( WeaponData->weapon_name == selected_weaponf ) {
		weapon->fallback_paint_kit( ) = valid_indecies.at( selected_skin );
		std::cout << "set" << selected_weaponf << " to " << valid_indecies.at( selected_skin ) << "\n";
		weapon->item_id_high( ) = -1;
	}
	/*else if (WeaponData->weapon_name == "weaapon_m4a1_silencer")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_ak47")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_aug")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_awp")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_famas")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_gs3sg1")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_galilar")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_scar20")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_sg556")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_ssg08")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_bizon")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_mac10")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_mp7")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_mp9")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_p90")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_ump45")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_mp5sd")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_m249")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_mag7")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_negev")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_nova")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_sawedoff")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_xm1014")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_usp_silencer")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_deagle")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_elite")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_fiveseven")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_glock")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_hkp2000")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_p250")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_tec9")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_cz75a")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);
	else if (WeaponData->weapon_name == "weaapon_revolver")
		weapon->fallback_paint_kit() = valid_indecies.at(selected_skin);*/
		//}
}
void DrawSlider( int x, int y, int width, int height, int* ToPass, std::vector<std::string> items ) {
	int slider_y = x;
	static int old_pos = slider_y;
	static bool ever_changed = false;

	float viewportHeight = 200;
	float contentHeight = items.size() * 19.3;

	float viewableRatio = viewportHeight / contentHeight; // 1/3 or 0.333333333n

	float scrollBarArea = viewportHeight; // 150px

	int max = scrollBarArea * viewableRatio;
	int thumbHeight = std::clamp( max, 3, 195 ); // 50px

	float scrollTrackSpace = contentHeight - viewportHeight; // (600 - 200) = 400 
	float scrollThumbSpace = viewportHeight - thumbHeight; // (200 - 50) = 150
	float scrollJump = scrollTrackSpace / scrollThumbSpace; //  (400 / 150 ) = 2.666666666666667

	static bool cont = true;

	if ( slider_y < y + 33 )
		slider_y = y + 33;

	if ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 && menu.mouse_in_region( x, y, 15, viewportHeight ) ) {
		if ( menu.get_mouse_position( ).y - thumbHeight / 2 > y + 31 && menu.get_mouse_position( ).y + thumbHeight / 2 - 2 < y + 31 + viewportHeight - 2 ) {
			slider_y = menu.get_mouse_position( ).y - thumbHeight / 2;
			old_pos = menu.get_mouse_position( ).y;
		}
		else {
			slider_y = old_pos - thumbHeight / 2;
		}
	}
	else {
		if ( old_pos - thumbHeight / 2 > y + 31 && old_pos - thumbHeight / 2 - 2 < slider_y + thumbHeight + 200 ) {
			slider_y = old_pos - thumbHeight / 2;
		}
	}

	Render::FilledRect( x, y + 31, 15, 198, color( 48, 48, 48 ) );

	Render::FilledRect( x + 2, slider_y, 12, thumbHeight, color( 55, 55, 55 ) );

	Render::FilledRect( x, y, 1, viewportHeight - 1, color( 20, 20, 20, 180 ) ); //slider
}

static int selected_player = 0;
void cmenu::draw_playerlist( ) { // TODO: fix this ugly peice of shit :)
	static int width = 200, height = 250;

	int playerl_x = MenuX + 250;
	int playerl_y = MenuY + 71;

	//List players
	Render::FilledRect( playerl_x + 26, playerl_y + 30, 149, 200, color( 45, 45, 45 ) );

	if ( Interfaces::engine->IsConnected( ) && Interfaces::engine->IsInGame( ) ) {
		C_CSPlayer* local_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );
		std::vector<int> player_index;
		std::vector<std::string> players;

		for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {
			C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

			if ( !player || player == local_player )
				continue;

			player_info_t info;
			Interfaces::engine->get_player_info( player->Networkable()->EntIndex( ), &info );

			players.emplace_back( info.name );
			player_index.emplace_back( i );
		}


		int y = playerl_y + 31;
		int x = playerl_x + 159;
		int slider_y = playerl_y + 31;
		static int old_pos = slider_y;
		static bool ever_changed = false;

		float viewportHeight = 200;
		float contentHeight = players.size( ) * 19.3;

		float viewableRatio = viewportHeight / contentHeight; // 1/3 or 0.333333333n

		float scrollBarArea = viewportHeight; // 150px

		int max = scrollBarArea * viewableRatio;
		int thumbHeight = std::clamp( max, 3, 195 ); // 50px

		float scrollTrackSpace = contentHeight - viewportHeight; // (600 - 200) = 400 
		float scrollThumbSpace = viewportHeight - thumbHeight; // (200 - 50) = 150
		float scrollJump = scrollTrackSpace / scrollThumbSpace; //  (400 / 150 ) = 2.666666666666667

		static bool cont = true;


		if ( slider_y < playerl_y + 33 )
			slider_y = playerl_y + 33;

		if ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( x, y, 15, viewportHeight ) ) {
			if ( get_mouse_position( ).y - thumbHeight / 2 > playerl_y + 31 && get_mouse_position( ).y + thumbHeight / 2 - 2 < playerl_y + 31 + viewportHeight - 2 ) {
				slider_y = get_mouse_position( ).y - thumbHeight / 2;
				old_pos = get_mouse_position( ).y;
			}
			else {
				slider_y = old_pos - thumbHeight / 2;
			}
		}
		else {
			if ( old_pos - thumbHeight / 2 > playerl_y + 31 && old_pos - thumbHeight / 2 - 2 < slider_y + thumbHeight + 200 ) {
				slider_y = old_pos - thumbHeight / 2;
			}
		}

		Render::FilledRect( x, playerl_y + 31, 15, 198, color( 48, 48, 48 ) );

		Render::FilledRect( x + 2, slider_y, 12, thumbHeight, color( 55, 55, 55 ) );

		Render::FilledRect( x, y, 1, viewportHeight - 1, color( 20, 20, 20, 180 ) ); //slider

		Render::FilledRect( playerl_x + 68, y, 1, viewportHeight, color( 20, 20, 20, 180 ) ); //idx line


		Render::Text( Render::Fonts::menu, std::to_string( get_mouse_position( ).y - thumbHeight ), 0, 0, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
		Render::Text( Render::Fonts::menu, std::to_string( slider_y + thumbHeight ), 0, 15, false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );

		auto filledrectfade = [ ] ( int x, int y, int w, int h, int a1, int a2, bool horiz ) -> void {
			Interfaces::Surface->draw_filled_rect_fade( x, y, x + w, y + h, a1, a2, horiz );
		};

		Interfaces::Surface->DrawSetColor( 0, 0, 0, 180 );
		filledrectfade( playerl_x + 26, playerl_y + 210, 134, 20, 0, 180, false );

		Interfaces::Surface->m_bClippingEnabled( ) = true;
		Interfaces::Surface->clip( vector2d_t( playerl_x + 26, playerl_y + 30 ), vector2d_t( 149, 200 ), false );

		for ( int i = 0; i < players.size( ); i++ ) {
			int item_rect[ 4 ] = { playerl_x + 31, playerl_y + 34 + ( i * 19 ) - ( ( slider_y - y ) * scrollJump ), 124, 17 };

			if ( mouse_in_region( playerl_x + 31, playerl_y + 34, 124, 200 ) ) {
				if ( GetKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( item_rect[ 0 ], item_rect[ 1 ], item_rect[ 2 ], item_rect[ 3 ] ) )
					selected_player = i;
			}

			if ( i != players.size( ) - 1 )
				Render::FilledRect( item_rect[ 0 ] - 4, item_rect[ 1 ] + 15, item_rect[ 2 ] + 8, 1, color( 20, 20, 20, 180 ) );

			if ( selected_player == i ) {
				Render::FilledRect( item_rect[ 0 ] + 38, item_rect[ 1 ] - 3, item_rect[ 2 ] - 35, item_rect[ 3 ] + 1, color( 50, 50, 50, 190 ) ); //idx
				Render::Text( Render::Fonts::menu, players.at( i ), item_rect[ 0 ] + 45, item_rect[ 1 ], false, Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 200 );
				Global::selected_player = player_index[ selected_player ];
			}
			else {
				Render::Text( Render::Fonts::menu, players.at( i ), item_rect[ 0 ] + 45, item_rect[ 1 ], false, 205, 205, 205 );
			}

			Render::Text( Render::Fonts::menu, "IDX: " + std::to_string( player_index.at( i ) ), item_rect[ 0 ], item_rect[ 1 ], false, 205, 205, 205 );
		}

		Interfaces::Surface->m_bClippingEnabled( ) = false;

		if ( Controls.BodyAim[ player_index.at( Global::selected_player ) ] )
			Global::BodyAimIndex[ player_index.at( Global::selected_player ) ] = true;
		else
			Global::BodyAimIndex[ player_index.at( Global::selected_player ) ] = false;

		if ( Controls.WhiteList[ player_index.at( Global::selected_player ) ] )
			Global::WhiteListIndex[ player_index.at( Global::selected_player ) ] = true;
		else
			Global::WhiteListIndex[ player_index.at( Global::selected_player ) ] = false;

	}
	else
		Render::Text( Render::Fonts::menu, "UNAVAILABLE", playerl_x + 62, playerl_y + 35, false, 205, 205, 205 );

}
void cmenu::csgo_draw( ) {

	static color color_array[ ] = {
	color( 255, 0, 0 ), color( 255, 180, 0 ) , color( 255, 255, 0 ),
	color( 0, 255, 0 ) , color( 0, 0, 255 ) , color( 255, 0, 255 )
	};

	int draw_x = MenuX + 250, draw_y = MenuY + 82;


	static int drag_x = 400;
	static int drag_y = 300;
	static int screen_width, screen_height;
	bool click = false;
	static bool dragging = false;

	static int width = 320, height = 320;
	if ( GetAsyncKeyState( VK_LBUTTON ) )
		click = true;

	Interfaces::engine->get_screen_size( screen_width, screen_height );
	vector2d_t mouse_pos = get_mouse_position( );

	//Render::FilledRect(draw_x + 13, draw_y + 13, width - 76, height - 26, color(55, 55, 55)); //inside outline

	//Render::FilledRect(draw_x + 13, draw_y + 13, width - 76, 5, color(0, 0, 0, 80)); //shadow

	//Render::FilledRect(draw_x + 2, draw_y + 3, 270, 28, color(Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b)); //clear button
	//Render::Text(Render::Fonts::tab, "Draw Menu", draw_x + 130, draw_y + 10, true, 205, 205, 205);


	//static int index = 0;

	//for (int i = 0; i < 6; i++) {
	//	Render::FilledRect(draw_x + (width - 50), draw_y + (i * 45) + 50, 28, 28, color_array[i]);

	//	static bool pressed = false;
	//	if (!GetAsyncKeyState(VK_LBUTTON) && mouse_in_region(draw_x + (width - 50), draw_y + (i * 45) + 50, 28, 28)) {
	//		if (pressed)
		//		index = i;
		//	pressed = false;
	//	}
	//	if (GetAsyncKeyState(VK_LBUTTON) && mouse_in_region(draw_x + (width - 50), draw_y + (i * 45) + 50, 28, 28) && !pressed) pressed = true;

//	}

	//int undorect[4] = { draw_x + 67, draw_y + 317, 38, 17 };
	//Render::FilledRect(undorect[0], undorect[1], undorect[2], undorect[3], color(60, 60, 60)); //clear button
	//Render::Text(Render::Fonts::menu, "Undo", undorect[0] + 6, undorect[1] + 1, false, 205, 205, 205);


	//int clearrect[4] = { draw_x + 14, draw_y + 317, 38, 17 };
//	Render::FilledRect(clearrect[0], clearrect[1], clearrect[2], clearrect[3], color(60, 60, 60)); //clear button
//	Render::Text(Render::Fonts::menu, "Clear", clearrect[0] + 7, clearrect[1] + 1, false, 205, 205, 205);

//	Render::OutlinedRect(draw_x + (width - 50), draw_y + (index * 45) + 50, 28, 28, color(0, 0, 0)); //color box outline

	static std::vector<draw_csgo> draw_vector;
	static vector2d_t end_mouse_pos = get_mouse_position( );
	static vector2d_t start_mouse_pos = get_mouse_position( );

	if ( Global::clear_draw ) { //clear
		if ( draw_vector.size( ) != 0 )
			draw_vector.clear( );

		Global::clear_draw = false;
	}

	if ( Global::undo_draw ) { //undo

		if ( draw_vector.size( ) > 3 ) {
			draw_vector.pop_back( );
			draw_vector.pop_back( );
			draw_vector.pop_back( );
		}
		else {
			if ( draw_vector.size( ) == 2 ) {
				draw_vector.pop_back( );
				draw_vector.pop_back( );
			}
			else if ( draw_vector.size( ) == 1 ) {
				draw_vector.pop_back( );
			}
		}
		Global::undo_draw = false;
	}

	color dcolor = color( Controls.draw_color.r, Controls.draw_color.g, Controls.draw_color.b );

	if ( Controls.open_menu_draw && !n_menu::menu_opened )
		return;


	static bool draw = false;
	if ( GetAsyncKeyState( VK_RBUTTON ) & 0x8000 && n_menu::menu_opened ) {
		if ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) {
			start_mouse_pos = get_mouse_position( );
			draw = true;
		}
		else {
			end_mouse_pos = get_mouse_position( );
			start_mouse_pos = get_mouse_position( );
			draw = false;
		}
		if ( draw ) {
			drawing = true;
			draw_vector.emplace_back( draw_csgo( end_mouse_pos, start_mouse_pos, dcolor, Controls.thickness ) );
		}
		else {
			drawing = false;
		}

		end_mouse_pos = get_mouse_position( );

	}
	for ( int i = 0; i < draw_vector.size( ); i++ ) {

		//	if (draw_vector.at(i).start.x > draw_x + width - 66 || draw_vector.at(i).start.y > draw_y + height - 20)
		//		continue;

		//	if (draw_vector.at(i).start.x < draw_x + 15 || draw_vector.at(i).start.y < draw_y + 30)
			//	continue;

		for ( int t = 0; t < draw_vector.at( i ).thickness + 1; t++ ) {
			Render::line( draw_vector.at( i ).start.x - t, draw_vector.at( i ).start.y - t, draw_vector.at( i ).end.x - t, draw_vector.at( i ).end.y - t, draw_vector.at( i ).col );
			Render::line( draw_vector.at( i ).start.x, draw_vector.at( i ).start.y, draw_vector.at( i ).end.x, draw_vector.at( i ).end.y, draw_vector.at( i ).col );
			Render::line( draw_vector.at( i ).start.x + t, draw_vector.at( i ).start.y + t, draw_vector.at( i ).end.x + t, draw_vector.at( i ).end.y + t, draw_vector.at( i ).col );
		}
	}
}

void cmenu::run_esp_preview( int x, int y ) {

	if ( SelectedTab != TAB_VISUALS )
		return;

	cTextures player = cTextures( 101, 128, 227 );
	Global::Texture_id = Interfaces::Surface->create_new_Texture_id( true );
	create_Texture( player );

	Interfaces::Surface->draw_Textured_rect( x, y, 128, 227 );

	if ( Controls.box ) {
		Render::OutlinedRect( x, y, 133, 233, color( 220, 220, 220 ) );
		Render::OutlinedRect( x - 1, y - 1, 135, 235, color( 0, 0, 0, 100 ) );
	}

	if ( Controls.name ) {
		Render::Text( Render::Fonts::preview, "Player01", x + 50, y - 11, false, 220, 220, 220 );
	}

	if ( Controls.health ) {
		Render::FilledRect( x - 6, y + 30, 1, 203, color( 0, 255, 0 ) );
		Render::OutlinedRect( x - 7, y + 29, 3, 205, color( 0, 0, 0, 100 ) );
		Render::Text( Render::Fonts::preview, "89", x - 12, y + 23, false, 220, 220, 220 );
	}

	if ( Controls.weapon ) {
		Render::Text( Render::Fonts::preview, "SCAR-20", x + 49, y + ( Controls.ammo ? 246 : 236 ), false, 220, 220, 220 );

		if (Controls.weapon == 2 || Controls.weapon == 3)
			Render::Text( Render::Fonts::WeaponIcon, "I", x + 52, y + ( Controls.ammo ? 256 : 246 ), false, 220, 220, 220 );
	}

	if ( Controls.ammo ) {
		Render::FilledRect( x + 2, y + 238, 120, 1, color( 0, 191, 191 ) );
		Render::OutlinedRect( x + 1, y + 237, 122, 3, color( 0, 0, 0, 100 ) );
		Render::Text( Render::Fonts::preview, "19", x + 120, y + 233, false, 220, 220, 220 );
	}

	if ( Controls.info ) {
		Render::Text( Render::Fonts::preview, "HK", x + 135, y - 2, false, 220, 220, 220 );
		Render::Text( Render::Fonts::preview, "DUCK", x + 135, y + 12, false, 220, 220, 220 );
	}
}
//
void cmenu::WeaponConfigTab( int alpha ) {

	std::vector<std::string> WeaponIcons = { "I", "B", "R", "C", "X", "F", "Z" };
	std::vector<std::string> WeaponNames = { "Auto", "Rifle", "Sniper", "Pistol", "SMG", "Heavy", "Misc" };

	for ( int i = 0; i < WeaponIcons.size( ); i++ ) {
		int rect[ 4 ] = { MenuX - 52, MenuY + 20 + ( i * 52 ), 50, 30 };

		if ( GetAsyncKeyState( VK_LBUTTON ) && this->mouse_in_region( rect[ 0 ] - 25, rect[ 1 ], rect[ 2 ], rect[ 3 ] ) )
			Controls.weapon_cfg = i;

		color MenuColor = color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b );


		if ( i == Controls.weapon_cfg ) {
			Render::Text( Render::Fonts::WeaponText, WeaponNames.at( i ), rect[ 0 ], rect[ 1 ] + 34, true, MenuColor.r, MenuColor.g, MenuColor.b, alpha );
			Render::Text( Render::Fonts::WeaponConfig, WeaponIcons.at( i ), rect[ 0 ], rect[ 1 ], true, MenuColor.r, MenuColor.g, MenuColor.b, alpha );
		}
		else {
			Render::Text( Render::Fonts::WeaponConfig, WeaponIcons.at( i ), rect[ 0 ], rect[ 1 ], true, 215, 215, 215, alpha );
			Render::Text( Render::Fonts::WeaponText, WeaponNames.at( i ), rect[ 0 ], rect[ 1 ] + 34, true, 215, 215, 215, alpha );
		}
	}
}