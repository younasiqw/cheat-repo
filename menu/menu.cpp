#include "menu.h"
cmenu menu;
void cmenu::DrawMenu( ) {

	static bool pressed = false;
	if ( !pressed && GetAsyncKeyState( VK_INSERT ) )
		pressed = true;

	else if ( pressed && !GetAsyncKeyState( VK_INSERT ) )
	{
		pressed = false;
		n_menu::menu_opened = !n_menu::menu_opened;
	}


	static int drag_x = 300;
	static int drag_y = 300;
	static int screen_width, screen_height;
	bool click = false;
	static bool dragging = false;

	if ( n_menu::menu_opened ) {

		if ( GetAsyncKeyState( VK_LBUTTON ) )
			click = true;

		Interfaces::engine->get_screen_size( screen_width, screen_height );
		vector2d_t mouse_pos = get_mouse_position( );

		if ( dragging && !click )
			dragging = false;

		bool adjusted = false;
		if ( dragging && click ) {
			MenuX = mouse_pos.x - drag_x;
			MenuY = mouse_pos.y - drag_y;
		}
		else {}

		if ( MenuX + mwidth > screen_width )
			MenuX = screen_width - mwidth;

		if ( MenuX < 0 )
			MenuX = 0;

		if ( MenuY + mheight > screen_height )
			MenuY = screen_height - mheight;

		if ( MenuY < 0 )
			MenuY = 0;

		if ( mouse_in_region( MenuX, MenuY, mwidth, 60 ) && !drawing ) {
			dragging = true;
			drag_x = mouse_pos.x - MenuX;
			drag_y = mouse_pos.y - MenuY;
		}

		//initalize variables
		item_x = MenuX + 190;
		itemname_x = MenuX + 95;
		position_y2 = MenuY + 72;
		position_y = MenuY + 72;
		TabIndex = 0;
		gbwidth = 190, gbheight = 0;
		//search_controls.clear();
		tab_items = 0;

		GROUP_X1 = 25;
		GROUP_X2 = 255;

		GROUP_HALF_Y1 = 80;
		GROUP_HALF_Y2 = 250;

		GROUP_FULL_Y = 80;

		previous_group_type = 0;

		if ( !Controls.lock_layout && !drawing ) {
			if ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 && mouse_in_region( MenuX + mwidth - 50, MenuY + mheight - 50, 90, 90 ) ) {
				mwidth = get_mouse_position( ).x - MenuX;
				mheight = get_mouse_position( ).y - MenuY;
			}
		}

		/* make sure size isn't too large */
		if ( mwidth < 475 ) mwidth = 475;
		if ( mwidth > 1340 ) mwidth = 1340;
		if ( mheight < 410 ) mheight = 410;
		if ( mheight > 750 ) mheight = 750;

		Render::FilledRect( 0, 0, screen_width, screen_height, color( 0, 0, 0, 150 ) );
		////
		//outside
		//Render::RoundedRect( MenuX - 1, MenuY - 1, mwidth + 2, mheight + 2, 5, color( 0, 0, 0, 100 ) );

		if ( this->ToolTip != "" ) {
			Render::RoundedRect( MenuX, MenuY, mwidth, mheight + 10, 9, color( 45, 45, 50 ) );
		}
		else {
			Render::RoundedRect( MenuX, MenuY, mwidth, mheight, 9, color( 45, 45, 50 ) );
		}

		Render::RoundedRect( MenuX, MenuY, mwidth, 48, 9, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

		Render::Text( Render::Fonts::title, _( "COCKHACK" ), MenuX + 50, MenuY + 8, false, 240, 240, 240, 200 );

		Render::RoundedRect( MenuX, MenuY + 48, mwidth, 10, 5, color( 0, 0, 0, 70 ) );

		//Render::PulseRect( MenuX - 20, MenuY - 18, mwidth + 20, mheight + 18, 20, color( 255, 0, 255, 255 ) );

		if ( this->ToolTip != "" ) {
			Render::RoundedRect( MenuX + 4, MenuY + mheight - 15, mwidth - 200, 2, 1, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b, 100 ) );

			Render::Text( Render::Fonts::menu, "* " + this->ToolTip, MenuX + 7, MenuY + mheight - 10, false, 210, 210, 210 );
		}

		if ( SelectedTab == ctabs::TAB_AIM ) {

			int x = MenuX + 9;
			int y = MenuY + mheight / 2 + 24;
			int rect[ 4 ] = { x - 2, MenuY + 93, 12, 280 };

			int LargerRect[ 4 ] = { rect[ 0 ] - 95, rect[ 1 ] - 70, 110, rect[ 3 ] + 82 };

			static float FadedAlpha;
			FadeAlpha( LargerRect, 20, &FadedAlpha, false, 0, 0, true );
			int ClampedFadedAlpha = std::clamp( FadedAlpha, 0.f, 255.f );

			static float PulseAlpha;
			FadeAlpha( LargerRect, 20, &PulseAlpha, true, 0, 10, true );
			int ClampedPulseAlpha = std::clamp( PulseAlpha, 0.f, 255.f );

			//Render::RoundedRect( rect[ 0 ] - 2, rect[ 1 ] - 2, rect[ 2 ] + 4, rect[ 3 ] + 4, 3, color( 80, 80, 80, ClampedPulseAlpha ) );
			//Render::RoundedRect( rect[ 0 ] - 1, rect[ 1 ] - 1, rect[ 2 ] + 2, rect[ 3 ] + 2, 3, color( 0, 0, 0, ClampedPulseAlpha ) );
			Render::RoundedRect( rect[ 0 ] - 1, rect[ 1 ] - 1, rect[ 2 ] + 2, rect[ 3 ] + 2, 3, color( 60, 60, 64, ClampedPulseAlpha ) );

			Render::RoundedRect( LargerRect[ 0 ] - 2, LargerRect[ 1 ] - 2, LargerRect[ 2 ] - 31, LargerRect[ 3 ] + 4, 3, color( 80, 80, 80, ClampedFadedAlpha ) );
			Render::RoundedRect( LargerRect[ 0 ] - 1, LargerRect[ 1 ] - 1, LargerRect[ 2 ] - 33, LargerRect[ 3 ] + 2, 3, color( 0, 0, 0, ClampedFadedAlpha ) );
			Render::RoundedRect( LargerRect[ 0 ], LargerRect[ 1 ], LargerRect[ 2 ] - 35, LargerRect[ 3 ], 3, color( 53, 53, 53, ClampedFadedAlpha ) );

			//middle to top
			Render::line( x, y, x + 6, y - 8, color( 255, 255, 255 ) );
			Render::line( x + 1, y, x + 7, y - 8, color( 255, 255, 255 ) );

			//middle to bottom
			Render::line( x, y, x + 6, y + 8, color( 255, 255, 255 ) );
			Render::line( x + 1, y, x + 7, y + 8, color( 255, 255, 255 ) );

			this->WeaponConfigTab( ClampedFadedAlpha );
		}

		this->AddTab( _( "Aimbot" ) );
		this->AddTab( _( "Visuals" ) );
		this->AddTab( _( "Misc" ) );
		//this->AddTab( "SKINS" );
		this->AddTab( _( "Config" ) );


		// aimbot tab
		this->register_subgroupbox( { _( "Ragebot" ),  _( "Legitbot") }, TAB_AIM, type::GB_FULL, GROUP_X1, GROUP_FULL_Y, &selected_subgroup4, 6, 7 ); {

			if ( selected_subgroup4 == 7 ) {
				this->AddCheckbox( _( "Aim Active" ), &Controls.legit_aim, TAB_AIM );
				this->AddCheckbox( _( "Lag Compensation" ), &Controls.legit_lagcomp, TAB_AIM );
				this->AddSlider( _( "FOV" ), 6, "deg", &Controls.legit_fov, TAB_AIM, 0, 0, 10 );
				this->AddCheckbox( _( "Triggerbot" ), &Controls.triggerbot, TAB_AIM );
				if ( Controls.triggerbot ) {
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.trigger_hitchance, TAB_AIM );
				}
			}
			else if ( selected_subgroup4 == 6 ) {
				this->AddCheckbox( _( "Aim Active" ), &Controls.aim_active, TAB_AIM );
				this->AddCheckbox( _( "Lag Compensation" ), &Controls.lag_compensation, TAB_AIM );
				this->AddCheckbox( _( "Autoshoot" ), &Controls.autoshoot, TAB_AIM );
				this->AddCheckbox( _( "Resolver" ), &Controls.resolver_active, TAB_AIM, false, nullptr, 0, 0, "Resolves players fake angles" );
				this->AddSlider( _( "Baim After X Missed Shots" ), 10, "shots", &Controls.baimafterx, TAB_AIM, 0, 0, 10);

				
				switch ( Controls.weapon_cfg ) {
				case AUTO: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.AutoHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ AUTO ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "hp", &Controls.MinimumDamage[ AUTO ], TAB_AIM );
				} break;
				case RIFLE: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.RifleHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ RIFLE ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "hp", &Controls.MinimumDamage[ RIFLE ], TAB_AIM );
				} break;
				case SNIPER: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.SniperHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ SNIPER ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "hp", &Controls.MinimumDamage[ SNIPER ], TAB_AIM );
				} break;
				case PISTOL: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.PistolHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ PISTOL ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "hp", &Controls.MinimumDamage[ PISTOL ], TAB_AIM );
				} break;
				case SMG: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.SMGHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ SMG ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "hp", &Controls.MinimumDamage[ SMG ], TAB_AIM );
				} break;
				case HEAVY_PISTOL: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.HeavyPistolHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ HEAVY_PISTOL ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "dmg", &Controls.MinimumDamage[ HEAVY_PISTOL ], TAB_AIM );
				} break;
				case MISC: {
					this->AddMultiBox( _( "Hitboxes" ), { "Head", "Neck", "Chest", "Pelvis", "Arms", "Legs" }, Controls.MiscHitboxes, TAB_AIM );
					this->AddSlider( _( "Hitchance" ), 100, "%", &Controls.whitchance[ MISC ], TAB_AIM );
					this->AddSlider( _( "Minimum Damage" ), 100, "hp", &Controls.MinimumDamage[ MISC ], TAB_AIM );
				} break;
				}

				this->AddCheckbox( _( "Pointscale" ), &Controls.PointScale, TAB_AIM, false, nullptr, 0, 0, "Percent of the hitbox shot at, starting from center" );

				if ( Controls.PointScale ) {
					this->AddSlider( _( "Head Pointscale" ), 100, "%", &Controls.HeadPointScale, TAB_AIM );
					this->AddSlider( _( "Body Pointscale" ), 100, "%", &Controls.BodyPointScale, TAB_AIM );
				}
			}
		}

		this->register_groupbox( _( "Movement" ), TAB_AIM, type::GB_HALF, GROUP_X2, GROUP_HALF_Y2 + 20, "", -9 ); {
			this->AddCheckbox( _( "Fakeduck" ), &Controls.fakeduck, TAB_AIM, true, &Controls.fakeduck_key, 0, MenuY + 190, "Crouches server-side, stands/crouches client-side " );
			this->AddCheckbox( _( "Slow-walk" ), &Controls.slow_walk, TAB_AIM, false, nullptr, 0, MenuY + 190, " Slows movement to accurate speed when walk held" );
			this->AddCombobox( _( "Autostop" ), { "Off", "Full", "Slide" }, &Controls.auto_stop_mode, TAB_AIM, MenuY + 200, 0, " Slows movement to accurate speed when opponent shootable" );
		}

		this->register_subgroupbox(  { _( "Anti-Aim"), _( "Fakelag" )}, TAB_AIM, type::GB_HALF, GROUP_X2, GROUP_HALF_Y1, &selected_subgroup5, 8, 9, 15 ); {
			if ( selected_subgroup5 == 8 ) {
				this->AddCheckbox( _( "Anti-Aim Enabled" ), &Controls.antiaim, TAB_AIM );
				this->AddCombobox( _( "Pitch" ), { "Off", "Down", "Up" }, &Controls.pitch, TAB_AIM );
				this->AddCombobox( _( "Yaw" ), { "Off", "Desync", "Auto" }, &Controls.yaw, TAB_AIM );
			}
			else if ( selected_subgroup5 == 9 ) {
				this->AddMultiBox( _( "Fakelag Triggers" ), { "On Ground", "In Attack", "In Air" }, Controls.fakelag_triggers, TAB_AIM );
				if ( Controls.fakelag_triggers[ 0 ] || Controls.fakelag_triggers[ 1 ] || Controls.fakelag_triggers[ 2 ] ) {
					this->AddSlider( _( "Fakelag" ), 16, "ticks", &Controls.fakelag, TAB_AIM, 0, 0, 8 );
				}
			}
		}

		this->register_subgroupbox( { _( "Removals" ), _( "Misc") }, TAB_VISUALS, type::GB_HALF, GROUP_X2, GROUP_HALF_Y2, &selected_subgroup6, 10, 11); {

			if ( selected_subgroup6 == 10 ) {
				this->AddCheckbox( _( "Remove Scope" ), &Controls.remove_scope, TAB_VISUALS, false, 0, 0, MenuY + 200 );
				this->AddCheckbox( _( "Remove Zoom" ), &Controls.remove_zoom, TAB_VISUALS, false, 0, 0, MenuY + 200 );
				this->AddCombobox( _( "Remove Smoke" ), { "Off", "No Draw", "Wireframe" }, &Controls.remove_smoke, TAB_VISUALS, MenuY + 200 );
				this->AddCheckbox( _( "Remove Flash" ), &Controls.remove_flash, TAB_VISUALS, false, 0, 0, MenuY + 170 );
			}
			else if ( selected_subgroup6 == 11 ) {
				this->AddColorPicker( _( "PAC" ), Controls.arrow_color, TAB_VISUALS, true, 18 );
				this->AddMultiBox( _( "Player Arrows" ), { "In FOV" , "Out of FOV" }, Controls.fov_arrows, TAB_VISUALS, MenuY + 200 );
				if ( Controls.fov_arrows[ 0 ] || Controls.fov_arrows[ 1 ] ) {
					this->AddSlider( _( "Diameter" ), 100, "px", &Controls.diameter, TAB_VISUALS, 0, MenuY + 200 );
					this->AddSlider( _( "Size" ), 40, "px", &Controls.size, TAB_VISUALS, 0, MenuY + 170 );
				}
			}
		}

		this->register_subgroupbox( {_("World 1" ), _( "World 2") }, TAB_VISUALS, type::GB_HALF, GROUP_X2, GROUP_HALF_Y1 + 1, &selected_subgroup2, 2, 3 ); {

			if ( selected_subgroup2 == 2 ) {
				this->AddSlider( _( "Nightmode" ), 100, "%", &Controls.nightmode, TAB_VISUALS, 0 );
				this->AddSlider( _( "Alpha Props" ), 100, "%", &Controls.alphaprops, TAB_VISUALS, 0 );
				this->AddCheckbox( _( "Full Bright" ), &Controls.fullbright, TAB_VISUALS, false );
				this->AddCheckbox( _( "Autowall Indicator" ), &Controls.autowall_crosshair, TAB_VISUALS, false );
				this->AddCheckbox( _( "Grenade Prediciton" ), &Controls.grenade_prediciton, TAB_VISUALS, false );

			}
			else if ( selected_subgroup2 == 3 ) {
				this->AddCombobox( _( "Skybox" ), {"Off", "Night", "Jungle", "Office", "Dust" }, &Controls.skybox, TAB_VISUALS, 0, 1 );
				this->AddCheckbox( _( "Cock Mode" ), &Controls.cock_mode, TAB_VISUALS, false );
				this->AddCheckbox( _( "Footstep ESP" ), &Controls.footstep_esp, TAB_VISUALS );
				this->AddColorPicker( _( "FSEPC" ), Controls.footstep_esp_color, TAB_VISUALS, false );
				this->AddCheckbox( _( "Smoke Radius" ), &Controls.smoke_radius, TAB_VISUALS, false );
			}
		}
		this->register_subgroupbox( { _( "Players" ), _( "Models" ) }, TAB_VISUALS, type::GB_FULL, GROUP_X1, GROUP_FULL_Y, &selected_subgroup1, 0, 1 ); {

			if ( selected_subgroup1 == 0 ) {
				this->AddCheckbox( _( "Enemies Only" ), &Controls.teamcheck, TAB_VISUALS );
				this->AddCheckbox( _( "Health" ), &Controls.health, TAB_VISUALS );
				this->AddColorPicker( _( "WC" ), Controls.weapon_color, TAB_VISUALS, true, 17 );
				this->AddCombobox( _( "Weapon" ), { "Off", "Text", "Icon", "Both" }, &Controls.weapon, TAB_VISUALS );
				this->AddCheckbox( _( "Name" ), &Controls.name, TAB_VISUALS );
				this->AddColorPicker( _( "NC" ), Controls.name_color, TAB_VISUALS, true );
				this->AddCheckbox( _( "Skeleton" ), &Controls.skeleton, TAB_VISUALS, false );
				this->AddColorPicker( _( "SC" ), Controls.skeleton_color, TAB_VISUALS, true );
				this->AddCheckbox( _( "Box" ), &Controls.box, TAB_VISUALS );
				this->AddColorPicker( _( "BC" ), Controls.box_color, TAB_VISUALS, true );
				this->AddCheckbox( _( "Flags" ), &Controls.info, TAB_VISUALS );
				this->AddCheckbox( _( "Ammo" ), &Controls.ammo, TAB_VISUALS );
				this->AddColorPicker( _( "CC" ), Controls.capsule_overlay_color, TAB_VISUALS, true, 17 );
				this->AddCombobox( _( "Capsule Overlay" ), { "Off", "On Damage", "On Kill" }, &Controls.capsule_overlay, TAB_VISUALS );
				this->AddCheckbox( _( "ESP Preview" ), &Controls.esp_preview, TAB_VISUALS );
				this->AddSlider( _( "Player Viewmodel" ), 80, "", &Controls.viewmodel, TAB_VISUALS );
				this->AddSlider( _( "World Viewmodel" ), 80, "", &Controls.world_viewmodel, TAB_VISUALS );
			}
			else if ( selected_subgroup1 == 1 ) {

				this->AddCombobox( _( "Model Entity" ), { "Enemies", "Teammates", "Local", "Other" }, &Controls.chams_entity, TAB_VISUALS, 0, 1 );

				if ( Controls.chams_entity == 0 ) {
					this->AddColorPicker( _( "Pr" ), Controls.chams_colorf[ 0 ], TAB_VISUALS, true, 17, 1 );
					this->AddCombobox( _( "Chams" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent" }, &Controls.chams_type[ 0 ], TAB_VISUALS, 0, 1 );
					this->AddColorPicker( _( "Behind Wall C" ), Controls.chams_xqz_colorf[ 0 ], TAB_VISUALS, true, 17, 1 );
					this->AddCombobox( _( "Behind Wall" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent" }, &Controls.chams_type_xqz[ 0 ], TAB_VISUALS, 0, 1 );
					this->AddCheckbox( _( "Glow" ), &Controls.glowf[ 0 ], TAB_VISUALS );
					this->AddColorPicker( _( "Glow C" ), Controls.glow_colorf[ 0 ], TAB_VISUALS, true );
					this->AddCheckbox( _( "Lag Compensation Chams" ), &Controls.lagcomp_chams, TAB_VISUALS, false, 0, 1 );
				}
				else if ( Controls.chams_entity == 1 ) {
					this->AddColorPicker( _( "Visible" ), Controls.chams_colorf[ 1 ], TAB_VISUALS, true, 17, 1 );
					this->AddCombobox( _( "Visible" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent" }, &Controls.chams_type[ 1 ], TAB_VISUALS, 0, 1 );
					this->AddColorPicker( _( "Behind Wall CH" ), Controls.chams_xqz_colorf[ 1 ], TAB_VISUALS, true, 17, 1 );
					this->AddCombobox( _( "Behind Wall" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent" }, &Controls.chams_type_xqz[ 1 ], TAB_VISUALS, 0, 1 );
					this->AddCheckbox( _( "Render Teammates" ), &Controls.render_teammates, TAB_VISUALS, false, 0, 1 );
					this->AddCheckbox( _( "Glow" ), &Controls.glowf[ 1 ], TAB_VISUALS );
					this->AddColorPicker( _( "Glow C" ), Controls.glow_colorf[ 1 ], TAB_VISUALS, true );
				}
				else if ( Controls.chams_entity == 2 ) {
					this->AddColorPicker( _( "Chams" ), Controls.chams_colorf[ 2 ], TAB_VISUALS, false, 17, 1 );
					this->AddCombobox( _( "Chams" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent" }, &Controls.chams_type[ 2 ], TAB_VISUALS, 0, 1 );
					this->AddCheckbox( _( "Fakelag Chams" ), &Controls.fakelag_chams, TAB_VISUALS, false, 0, 1 );
					this->AddColorPicker( _( "FCC" ), Controls.fakechams_color, TAB_VISUALS, true );

					if ( Controls.yaw == 1 ) {
						this->AddColorPicker( _( "DCC" ), Controls.desync_chams_color, TAB_VISUALS, true, 18 );
						this->AddMultiBox( _( "Desync Display" ), { "Chams", "Lines" }, Controls.desync_display, TAB_VISUALS );
					}

					this->AddColorPicker( _( "ACC" ), Controls.arm_color, TAB_VISUALS, true, 17, 1 );
					this->AddCombobox( _( "Arms" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent", "Crystal" }, &Controls.arm_mat, TAB_VISUALS, 0, 1 );
					//this->AddColorPicker( _( "WCC" ), Controls.weaponc_color, TAB_VISUALS, true, 17, 1 );
					//this->AddCombobox( _( "Weapons" ), { "Off", "Material", "Flat", "Metallic", "Pearlescent", "Crystal" }, & Controls.weapon_mat, TAB_VISUALS, 0, 1 );
					this->AddCheckbox( _( "Glow" ), &Controls.glowf[ 2 ], TAB_VISUALS );
					this->AddColorPicker( _( "Glow C" ), Controls.glow_colorf[ 2 ], TAB_VISUALS, true );
				}
				else if ( Controls.chams_entity == 3 ) {
					this->AddCombobox( _( "Dropped Model Entity" ), { "Weapons", "Grenades" }, &Controls.other_entity, TAB_VISUALS, 0, 1 );

					if ( Controls.other_entity == 0 ) {
						this->AddMultiBox( _( "Entity ESP" ), { "Glow", "Chams", "Name" }, Controls.weapon_esp, TAB_VISUALS );

						if ( Controls.weapon_esp[ 0 ] ) {
							if ( SelectedTab == TAB_VISUALS ) {
								Render::Text( Render::Fonts::menu, _( "Glow Color" ), itemname_x, position_y + 2, false, 210, 210, 210 );
							}
							this->AddColorPicker( _( "Glow Color" ), Controls.entity_glow_color[ 0 ], TAB_VISUALS, true, 17 );
							position_y += 17;
						}
						if ( Controls.weapon_esp[ 1 ] ) {
							if ( SelectedTab == TAB_VISUALS ) {
								Render::Text( Render::Fonts::menu, _( "Chams Color" ), itemname_x, position_y + 2, false, 210, 210, 210 );
							}
							this->AddColorPicker( _( "Chams Color" ), Controls.entity_chams_color[ 0 ], TAB_VISUALS, false, 17 );
							position_y += 19;
						}
					}
					else if ( Controls.other_entity == 1 ) {
						this->AddMultiBox( _( "Entity ESP" ), { "Glow", "Chams", "Name" }, Controls.grenade_esp, TAB_VISUALS );

						if ( Controls.grenade_esp[ 0 ] ) {
							if ( SelectedTab == TAB_VISUALS ) {
								Render::Text( Render::Fonts::menu, _( "Glow Color" ), itemname_x, position_y + 2, false, 210, 210, 210 );
							}
							this->AddColorPicker( _( "Glow Color" ), Controls.entity_glow_color[ 1 ], TAB_VISUALS, true, 17 );
							position_y += 17;
						}
						if ( Controls.grenade_esp[ 1 ] ) {
							if ( SelectedTab == TAB_VISUALS ) {
								Render::Text( Render::Fonts::menu, _( "Chams Color" ), itemname_x, position_y + 2, false, 210, 210, 210 );
							}
							this->AddColorPicker( _( "Chams Color" ), Controls.entity_chams_color[ 1 ], TAB_VISUALS, false, 17 );
							position_y += 19;
						}
					}
				}
			}
		}
		//if ( !Controls.playerlist ) {
			this->register_groupbox( _( "Draw Settings" ), TAB_MISC, type::GB_HALF, GROUP_X2, GROUP_HALF_Y1 ); {
				this->AddCheckbox( _( "Draw Enabled (M2)" ), &Controls.DrawMenu, TAB_MISC, false );

				itemname_x += 20;
				this->AddColorPicker( _( "Draw Color" ), Controls.draw_color, TAB_MISC, true );
				itemname_x -= 20;

				this->AddCheckbox( _( "Clear All" ), &Global::clear_draw, TAB_MISC );
				this->AddCheckbox( _( "Undo Last Draw" ), &Global::undo_draw, TAB_MISC );
				this->AddSlider( _( "Thickness" ), 20, "px", &Controls.thickness, TAB_MISC );
				this->AddCheckbox( _( "On Menu Open" ), &Controls.open_menu_draw, TAB_MISC );
			}
		//}
	/*	else {
			this->register_groupbox( "Playerlist", TAB_MISC, type::GB_FULL, GROUP_X2, GROUP_FULL_Y ); {

				position_y += 210;
				this->AddCheckbox( "Body Aim", &Controls.BodyAim[ Global::selected_player ], TAB_MISC );
				this->AddCheckbox( "Whitelist", &Controls.WhiteList[ Global::selected_player ], TAB_MISC );
			}
		}*/

		this->register_subgroupbox( { _( "Players") ,_( "World" )}, TAB_MISC, type::GB_FULL, GROUP_X1, GROUP_FULL_Y, &selected_subgroup3, 4, 5 ); {

			if ( selected_subgroup3 == 4 ) {
				this->AddCheckbox( _( "Bunnyhop" ), &Controls.bunnyhop, TAB_MISC, false, 0, 4 );
				this->AddCheckbox( _( "Compass" ), &Controls.compass, TAB_MISC, false, 0, 4 );
				this->AddMultiBox( _( "Player Logs" ), { "Shot Log", "Kill Log" }, Controls.player_logs, TAB_MISC );
				this->AddCheckbox( _( "Thirdperson" ), &Controls.thirdperson, TAB_MISC, true, &Controls.ThirdpersonKey, 4 );
				if ( Controls.ThirdpersonKey != -1 ) {
					this->AddSlider( _( "Distance" ), 200, "units", &Controls.tpdistance, TAB_MISC, 0, 0, 6 );
				}
				this->AddCombobox( _( "Hitmarkers" ), {"Off", "On Players", "On Screen" }, &Controls.hitmarkers, TAB_MISC );

				if ( Controls.hitmarkers > 0 ) {
					this->AddCombobox( _( "Hitmarker Sound" ), {"Off", "Premade", "Custom" }, &Controls.hitmarkersound, TAB_MISC, 0, 4 );
					if ( Controls.hitmarkersound == 2 ) {
						this->AddTextField( _( "Custom Sound" ), &Controls.custom_hitsounds, TAB_MISC, 4 );
					}
				}
				//this->AddCheckbox( _( "Playerlist" ), &Controls.playerlist, TAB_MISC );
			}
			else if ( selected_subgroup3 == 5 ) {
				this->AddCheckbox( _( "Bullet Tracers" ), &Controls.bullet_tracers, TAB_MISC, false, 0, 5 );
				this->AddColorPicker( _( "Bullet Tracers" ), Controls.bullettrace_color, TAB_MISC, true, 0, 5 );
				this->AddSlider( _( "Aspect Ratio" ), 200, "", &Controls.aspect_ratio, TAB_MISC, 5 );
				this->AddMultiBox( _( "Event Logs" ), {"Damage" , "Purchases", "Bomb" }, Controls.event_logs, TAB_MISC );
				this->AddTextField( _( "Killsay" ), &Controls.killsay, TAB_MISC, 5 );
				this->AddTextField( _( "Clantag" ), &Controls.clantag, TAB_MISC, 5 );
				this->AddCheckbox( _( "Damage Indicators" ), &Controls.damage_indicator, TAB_MISC, false, 0, 5 );
				this->AddColorPicker( _( "Damage Indicator C" ), Controls.damage_indicator_color, TAB_MISC, true, 0, 5 );
				this->AddCheckbox( _( "Bomb Timer" ), &Controls.bomb_timer, TAB_MISC, false, 0, 5 );
				this->AddCheckbox( _( "Disable Post Processing" ), &Controls.disable_post_processing, TAB_MISC, false, 0, 5 );
				this->AddCheckbox( _( "Remove Visual Recoil" ), &Controls.remove_visual_recoil, TAB_MISC, false, 0, 5 );
				this->AddCheckbox( _( "Indicators" ), &Controls.indicators, TAB_MISC );
			}
			//this->AddCheckbox("Player List", &Controls.playerlist, TAB_MISC);
		}
		if ( Controls.esp_preview && SelectedTab == TAB_VISUALS ) {
			Render::RoundedRect( MenuX + mwidth + 45, MenuY + mheight - 300, 195, 300, 5, color( 53, 53, 53 ) );

			Render::RoundedRect( MenuX + mwidth + 40, MenuY + mheight - 310, 205, 18, 4, color( Controls.MenuColor.r, Controls.MenuColor.g, Controls.MenuColor.b ) );

			Render::Text( Render::Fonts::menu, _( "ESP Preview" ), MenuX + mwidth + 140, MenuY + mheight - 308, true, 205, 205, 205 );

			run_esp_preview( MenuX + mwidth + 75, MenuY + mheight - 275 );

		}

		this->register_groupbox( _( "Skins" ), TAB_CONFIG, type::GB_HALF, GROUP_X2, GROUP_HALF_Y2 + 50, "", -23 ); {
			this->AddCombobox( _( "Knifes" ), { "Default", "M9 Bayonet", "Bayonet", "Flip", "Gut", "Karambit", "Huntsman",
			"Falchion", "Bowie", "Butterfly", "Shadow Daggers"/*, "Navaja", "Stiletto", "Ursus", "Talon",
			"Classic","Cord","Cannis","Outdoor","Skeleton"*/ }, &Controls.knife_model, TAB_CONFIG, MenuY + 180 );
		}

		this->register_groupbox( _( "Buybot" ), TAB_CONFIG, type::GB_HALF, GROUP_X2, GROUP_HALF_Y1, "", 30 ); {
			this->AddCombobox( _( "Primary" ), { "Off", "SCAR20 / G3SG1", "M4A1 / M4A4 / AK-47", "AUG / SG556" }, &Controls.primary_weapon, TAB_CONFIG );
			this->AddCombobox( _( "Secondary" ), { "Off", "Dualies", "Revolver / Deagle", "CZ75 / Tec9", "P250" }, &Controls.secondary_weapon, TAB_CONFIG );
			this->AddMultiBox( _( "Grenades" ), { "Smoke", "Molotov / Incendiary", "Decoy", "Flash", "High Explosive" }, Controls.grenade_choice, TAB_CONFIG );
			this->AddMultiBox( _( "Armor" ), { "Kevlar", "Kevlar + Helmet" }, Controls.armor_choice, TAB_CONFIG );
		}


		this->register_groupbox( _( "Config" ), TAB_CONFIG, type::GB_FULL, GROUP_X1, GROUP_FULL_Y ); {

			std::vector<std::string> configs = { "Legit" , "HvH" , "Config 1", "Config 2" };
#ifdef _DEBUG
			configs.push_back( "Debug" );
#endif
			this->add_listbox( &Global::config, TAB_CONFIG, configs );
			this->AddCheckbox( _( "Save Config" ), &Global::save_cfg, TAB_CONFIG );
			this->AddCheckbox( _( "Load Config" ), &Global::load_cfg, TAB_CONFIG );

			//draw_search_box();

			if ( SelectedTab == TAB_CONFIG ) {
				Render::Text( Render::Fonts::menu, _( "Menu Color" ), itemname_x, position_y + 2, false, 210, 210, 210 );
			}
			this->AddColorPicker( _( "Menu Color" ), Controls.MenuColor, TAB_CONFIG, false, 17 );
			position_y += 19;
			this->AddCheckbox( _( "Lock Menu Layout" ), &Controls.lock_layout, TAB_CONFIG );

			if ( Global::save_cfg ) {
				config.save( );
				event_vec.emplace_back( ceventlog( _( "Saved Config." ), Interfaces::GlobalVarsBase->CurrentTime ) );
				Global::save_cfg = false;
			}
			if ( Global::load_cfg ) {
				config.load( );
				event_vec.emplace_back( ceventlog( _( "Loaded Config." ), Interfaces::GlobalVarsBase->CurrentTime ) );
				Global::load_cfg = false;
			}
		}


		cTextures penis = cTextures( 103, 50, 50 );
		Global::Texture_id = Interfaces::Surface->create_new_Texture_id( true );
		create_Texture( penis );

		Interfaces::Surface->draw_Textured_rect( MenuX + 5, MenuY + 5, 38, 38 );

		if ( Controls.playerlist && SelectedTab == TAB_MISC ) {
			draw_playerlist( );
			Interfaces::Surface->m_bClippingEnabled( ) = false;
		}

		//if (SelectedTab == TAB_SKINS) {
		//	draw_weapons_group();
		//	draw_skins_group();
		//	//Interfaces::Surface->m_bClippingEnabled() = false;
		//}
		//Interfaces::Surface->m_bClippingEnabled() = false;
	}
}

void cmenu::FadeAlpha( int* size, int FadeSpeed, float* value, bool pulse, int min, int PulseSpeed, bool RegionLimit ) {
	 static bool hit_top = false, hit_bottom = true;

	 if ( size ) {
		 if ( !this->mouse_in_region( size[ 0 ], size[ 1 ], size[ 2 ], size[ 3 ] ) && RegionLimit ) {
			 if ( Interfaces::GlobalVarsBase->TickCount % 3 == 0 && *value > 0 )
				 *value -= FadeSpeed;
			 return;
		 }
	 }

	 if ( Interfaces::GlobalVarsBase->TickCount % 2 == 0 && *value < 255 + PulseSpeed ) {
		 if ( pulse ) {
			 if ( Interfaces::GlobalVarsBase->TickCount % 2 == 0 ) {
				 if ( *value < 255 && hit_bottom ) {
					 *value += PulseSpeed;

					 if ( *value >= 255 ) {
						 hit_top = true;
						 hit_bottom = false;
					 }
				 }
				 else if ( hit_top ) {
					 *value -= PulseSpeed;

					 if ( *value < min ) {
						 hit_bottom = true;
						 hit_top = false;
					 }
				 }
			 }
		 }
		 else {
			 *value += FadeSpeed;
		 }
	 }
}