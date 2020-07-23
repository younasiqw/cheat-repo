#include "../../hooks/hooks.h"
#include "../../Controls/Controls.h"
#include "../../menu/config.h"
#include <iostream>
#include "../../features/Headers/Events.h"
#include "../../features/Headers/Lagcomp.h"
//#include "../../../CSCHEAT/key_values.h"

namespace n_hooks {
	bool is_behind_smoke( Vector3D start_pos, Vector3D end_pos ) {
		typedef bool( __cdecl* line_goes_through_smoke ) ( Vector3D, Vector3D );
		static line_goes_through_smoke line_goes_through_smoke_fn = 0;

		if ( !line_goes_through_smoke_fn )
			line_goes_through_smoke_fn = reinterpret_cast< line_goes_through_smoke >( Utilities::pattern_scan( "client_panorama.dll", ( PBYTE ) "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx" ) );

		if ( line_goes_through_smoke_fn )
			return line_goes_through_smoke_fn( start_pos, end_pos );
		return false;
	}
	void __fastcall n_functions::SceneEnd( std::uintptr_t ecx, std::uintptr_t edx ) {
		static auto OriginalFn = n_hooks::RenderView->get_original_function< decltype( &SceneEnd ) >( 9 );
		OriginalFn( ecx, edx );

		float localcolor[ 4 ] {
			Controls.chams_colorf[ 2 ].r / 255.f,
			Controls.chams_colorf[ 2 ].g / 255.f,
			Controls.chams_colorf[ 2 ].b / 255.f,
			Controls.chams_colorf[ 2 ].a / 255.f
		};

		float desync_color[ 4 ] {
			Controls.desync_chams_color.r / 255.f,
			Controls.desync_chams_color.g / 255.f,
			Controls.desync_chams_color.b / 255.f,
			Controls.desync_chams_color.a / 255.f
		};

		for ( int i = 1; i <= Interfaces::EntityList->get_highest_entity_index( ); i++ ) {
			auto entity = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

			if ( !entity )
				continue;

			float grenade_color[ 4 ] {
				Controls.entity_chams_color[ 1 ].r / 255.f,
				Controls.entity_chams_color[ 1 ].g / 255.f,
				Controls.entity_chams_color[ 1 ].b / 255.f,
				Controls.entity_chams_color[ 1 ].a / 255.f
			};

			float weapon_color[ 4 ] {
				Controls.entity_chams_color[ 0 ].r / 255.f,
				Controls.entity_chams_color[ 0 ].g / 255.f,
				Controls.entity_chams_color[ 0 ].b / 255.f,
				Controls.entity_chams_color[ 0 ].a / 255.f
			};

			static IMaterial* entity_mat = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures" );
			if ( entity_mat != nullptr )
				entity_mat->increment_reference_count( );

			/*if ( entity->clientclass( )->class_id == ( int ) CCSRagdoll ) {
				Vector3D original_origin = entity->get_absolute_origin( );
				Vector3D original_angles = entity->get_abs_angles( );

				for ( int i = 0; i < storedhits.size( ); i++ ) {

					entity->set_abs_origin( storedhits.at( i ).origin );
					entity->set_abs_origin( storedhits.at( i ).angles );

					Interfaces::RenderView->SetBlend( 0.5f );
					Interfaces::ModelRender->ForceMaterialOverride( entity_mat );
					Interfaces::RenderView->SetColorModulation( weapon_color );

					entity->renderable( )->draw_model( 1, 255 );

					entity->set_abs_origin( original_origin );
					entity->set_abs_origin( original_angles );

					Interfaces::ModelRender->ForceMaterialOverride( nullptr );
				}
			}*/

			if ( strstr( entity->clientclass( )->network_name, "CWeapon" ) && Controls.weapon_esp[ 1 ] ) {
				if ( entity->get_owner_handle( ) == -1 )
				{
					Interfaces::RenderView->SetColorModulation( weapon_color );
					Interfaces::ModelRender->ForceMaterialOverride( entity_mat );
					entity->renderable( )->draw_model( 1, 255 );
					Interfaces::ModelRender->ForceMaterialOverride( nullptr );
				}
			}
			if ( Controls.grenade_esp[ 1 ] ) {
				if ( strstr( entity->clientclass( )->network_name, "CSmoke" ) || strstr( entity->clientclass( )->network_name, "CHE" )
					|| strstr( entity->clientclass( )->network_name, "CMolo" ) || strstr( entity->clientclass( )->network_name, "CFlash" ) || strstr( entity->clientclass( )->network_name, "CDecoy" ) ) {
					if ( entity->get_owner_handle( ) == -1 )
					{
						Interfaces::RenderView->SetColorModulation( grenade_color );
						Interfaces::ModelRender->ForceMaterialOverride( entity_mat );
						entity->renderable( )->draw_model( 1, 255 );
						Interfaces::ModelRender->ForceMaterialOverride( nullptr );
					}
				}
			}
		}

		if ( Global::Local && Global::Local->IsAlive( ) ) {

			float old_color[ 4 ];
			Interfaces::RenderView->GetColorModulation( old_color );

			if ( Global::Local->IsAlive( ) && Controls.desync_display[ 0 ] ) {
				if ( Global::InThirdperson && Controls.yaw == 1 ) {
					Vector3D original_angle = Global::Local->get_abs_angles( );

					static IMaterial* desync_mat = Interfaces::MaterialSystem->get_material( "glow_overlay", "Model Textures" );
					if ( desync_mat ) {

						desync_mat->increment_reference_count( );

						Global::Local->SetAbsAngles( Vector3D( 0, Global::fake_angle.y, 0 ) );
						Interfaces::ModelRender->ForceMaterialOverride( desync_mat );

						auto tint = desync_mat->FindVar( _( "$envmaptint" ) );
						if ( tint )
							tint->set_vec_value( desync_color[ 0 ], desync_color[ 1 ], desync_color[ 2 ] );

						static float alpha;
						menu.FadeAlpha( nullptr, 0, &alpha, true, 0, 6, false );

						Interfaces::RenderView->SetBlend( (alpha / 255.f) - 0.01f );

						Global::Local->renderable( )->draw_model( 1, 255 );
						Global::Local->SetAbsAngles( original_angle );

						Interfaces::ModelRender->ForceMaterialOverride( nullptr );
						Interfaces::RenderView->SetColorModulation( old_color );
						Interfaces::RenderView->SetBlend( 1 );
					}
				}
			}

			static IMaterial* localTexture;
			if ( Controls.chams_type[ 2 ] > 0 ) {
				switch ( Controls.chams_type[ 2 ] ) {
				case 1: localTexture = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures" ); break;
				case 2: localTexture = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures" ); break;
				case 3: localTexture = Interfaces::MaterialSystem->get_material( "metallic", nullptr, true, nullptr ); break;
				case 4: localTexture = Interfaces::MaterialSystem->get_material( "pearl", nullptr, true, nullptr ); break;
				}
			}
			if ( localTexture != nullptr )
				localTexture->increment_reference_count( );

			if ( Controls.chams_type[ 2 ] > 0 && Global::Local ) {
				Interfaces::ModelRender->ForceMaterialOverride( localTexture );
				Interfaces::RenderView->SetColorModulation( localcolor );
				Global::Local->renderable( )->draw_model( 1, 255 );
				Interfaces::ModelRender->ForceMaterialOverride( nullptr );
			}
		}

		for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {
			auto entity = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

			if ( !entity || entity->IsDormant( ) )
				continue;

			if ( !Global::Local )
				continue;

			if ( entity->IsAlive( ) ) {
				static IMaterial* Textured;
				static IMaterial* xqz_Textured;

				if ( Textured != nullptr )
					Textured->increment_reference_count( );

				if ( xqz_Textured != nullptr )
					xqz_Textured->increment_reference_count( );

				float chams_color[ 4 ];
				float chams_xqz_color[ 4 ];
				bool xqz = false;


				if ( entity->GetTeam( ) != Global::Local->GetTeam( ) ) {

					if ( Controls.lagcomp_chams && Global::Local->IsAlive( ) && entity->get_velocity( ).Length( ) > 10 ) {
						const auto oldOrigin = entity->get_absolute_origin( );
						static IMaterial* backtrack_mat = Interfaces::MaterialSystem->get_material( "glow_overlay", "Model Textures" );
						if ( backtrack_mat != nullptr && !PlayerRecords.at( entity->Networkable( )->EntIndex( ) ).empty( ) ) {

							float color[ 4 ] = { 255 / 255.f, 255 / 255.f, 0 / 255.f };
							Interfaces::RenderView->SetColorModulation( color );
							backtrack_mat->color_modulate( 255, 255, 0 );
							entity->set_abs_origin( PlayerRecords.at( entity->Networkable( )->EntIndex( ) ).back( ).origin );
							Interfaces::ModelRender->ForceMaterialOverride( backtrack_mat );
							backtrack_mat->alpha_modulate( 0.5f );

							entity->renderable( )->draw_model( 1, 255 );
						}
						Interfaces::ModelRender->ForceMaterialOverride( nullptr );
						entity->set_abs_origin( oldOrigin );
					}


					chams_xqz_color[ 0 ] = Controls.chams_xqz_colorf[ 0 ].r / 255.f;
					chams_xqz_color[ 1 ] = Controls.chams_xqz_colorf[ 0 ].g / 255.f;
					chams_xqz_color[ 2 ] = Controls.chams_xqz_colorf[ 0 ].b / 255.f;
					chams_xqz_color[ 3 ] = Controls.chams_xqz_colorf[ 0 ].a / 255.f;

					float old_color[ 3 ] = { 0.f };
					Interfaces::RenderView->GetColorModulation( old_color );

					if ( Controls.chams_type_xqz[ 0 ] ) {
						if ( Global::Local->IsAlive( ) && Global::Local != nullptr && entity != nullptr) {
							if ( is_behind_smoke( Global::Local->GetEyePosition( ), entity->GetEyePosition( ) ) ) //1
								continue;
						}

						Global::scene_end = true;


						if ( xqz_Textured != nullptr ) {
							xqz_Textured->set_material_var_flag( MATERIAL_VAR_IGNOREZ, true );
							Interfaces::ModelRender->ForceMaterialOverride( xqz_Textured );
						}
						Interfaces::RenderView->SetColorModulation( chams_xqz_color );
						Interfaces::RenderView->SetBlend( chams_xqz_color[ 3 ] );
						entity->renderable( )->draw_model( 1, 255 );
					}

					if ( Controls.chams_type[ 0 ] > 0 ) {
						switch ( Controls.chams_type[ 0 ] ) {
						case 1: Textured = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures", true, nullptr ); break;
						case 2: Textured = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures", true, nullptr ); break;
						case 3: Textured = Interfaces::MaterialSystem->get_material( "metallic", nullptr, true, nullptr ); break;
						case 4: Textured = Interfaces::MaterialSystem->get_material( "pearl", nullptr, true, nullptr ); break;
						}

						switch ( Controls.chams_type_xqz[ 0 ] ) {
						case 1: xqz_Textured = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures", true, nullptr ); break;
						case 2: xqz_Textured = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures", true, nullptr ); break;
						case 3: xqz_Textured = Interfaces::MaterialSystem->get_material( "metallic_xqz", nullptr, true, nullptr ); break;
						case 4: xqz_Textured = Interfaces::MaterialSystem->get_material( "pearl_xqz", nullptr, true, nullptr ); break;
						}

						chams_color[ 0 ] = Controls.chams_colorf[ 0 ].r / 255.f;
						chams_color[ 1 ] = Controls.chams_colorf[ 0 ].g / 255.f;
						chams_color[ 2 ] = Controls.chams_colorf[ 0 ].b / 255.f;
						chams_color[ 3 ] = Controls.chams_colorf[ 0 ].a / 255.f;

						Global::scene_end = true;

						if ( Global::Local->IsAlive( ) && Global::Local && entity ) {
							if ( is_behind_smoke( Global::Local->GetEyePosition( ), entity->GetEyePosition( ) ) )
								continue;
						}

						if ( Textured != nullptr ) {
							Textured->set_material_var_flag( MATERIAL_VAR_IGNOREZ, false );
							Interfaces::ModelRender->ForceMaterialOverride( Textured );
						}
						Interfaces::RenderView->SetColorModulation( chams_color );
						Interfaces::RenderView->SetBlend( chams_color[ 3 ] );
						entity->renderable( )->draw_model( 1, 255 );

						Interfaces::ModelRender->ForceMaterialOverride( nullptr );
					}
				}
				else {
					if ( entity != Global::Local && Controls.render_teammates ) {
						switch ( Controls.chams_type_xqz[ 1 ] ) {
						case 1: xqz_Textured = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures", true, nullptr ); break;
						case 2: xqz_Textured = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures", true, nullptr ); break;
						case 3: xqz_Textured = Interfaces::MaterialSystem->get_material( "metallic_xqz", nullptr, true, nullptr ); break;
						case 4: xqz_Textured = Interfaces::MaterialSystem->get_material( "pearl_xqz", nullptr, true, nullptr ); break;
						}

						chams_xqz_color[ 0 ] = Controls.chams_xqz_colorf[ 1 ].r / 255.f;
						chams_xqz_color[ 1 ] = Controls.chams_xqz_colorf[ 1 ].g / 255.f;
						chams_xqz_color[ 2 ] = Controls.chams_xqz_colorf[ 1 ].b / 255.f;
						chams_xqz_color[ 3 ] = Controls.chams_xqz_colorf[ 1 ].a / 255.f;

						if ( Controls.chams_type_xqz[ 1 ] ) {
							if ( Global::Local->IsAlive( ) && Global::Local && entity ) {
								if ( is_behind_smoke( Global::Local->GetEyePosition( ), entity->GetEyePosition( ) ) )
									continue;
							}

							Global::scene_end = true;

							if ( xqz_Textured != nullptr ) {
								xqz_Textured->set_material_var_flag( MATERIAL_VAR_IGNOREZ, true );
								Interfaces::ModelRender->ForceMaterialOverride( xqz_Textured );
							}
							Interfaces::RenderView->SetColorModulation( chams_xqz_color );
							Interfaces::RenderView->SetBlend( chams_xqz_color[ 3 ] );
							entity->renderable( )->draw_model( 1, 255 );
						}

						if ( Controls.chams_type[ 1 ] > 0 ) {
							switch ( Controls.chams_type[ 1 ] ) {
							case 1: Textured = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures", true, nullptr ); break;
							case 2: Textured = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures", true, nullptr ); break;
							case 3: Textured = Interfaces::MaterialSystem->get_material( "metallic", nullptr, true, nullptr ); break;
							case 4: Textured = Interfaces::MaterialSystem->get_material( "pearl", nullptr, true, nullptr ); break;
							}

							//if ( Textured != nullptr )
							//	Textured->increment_reference_count( );

							chams_color[ 0 ] = Controls.chams_colorf[ 1 ].r / 255.f;
							chams_color[ 1 ] = Controls.chams_colorf[ 1 ].g / 255.f;
							chams_color[ 2 ] = Controls.chams_colorf[ 1 ].b / 255.f;
							chams_color[ 3 ] = Controls.chams_colorf[ 1 ].a / 255.f;

							Global::scene_end = true;

							if ( Global::Local->IsAlive( ) && Global::Local && entity ) {
								if ( is_behind_smoke( Global::Local->GetEyePosition( ), entity->GetEyePosition( ) ) )
									continue;
							}

							if ( Textured != nullptr ) {
								Textured->set_material_var_flag( MATERIAL_VAR_IGNOREZ, false );
								Interfaces::ModelRender->ForceMaterialOverride( Textured );
							}

							Interfaces::RenderView->SetColorModulation( chams_color );
							Interfaces::RenderView->SetBlend( chams_color[ 3 ] );
							entity->renderable( )->draw_model( 1, 255 );

							Interfaces::ModelRender->ForceMaterialOverride( nullptr );
						}
					}
				}
			}

			/*static IMaterial* hitchams = Interfaces::MaterialSystem->get_material( "glow_overlay", "Model Textures" );
			const Vector3D old_origin = entity->get_absolute_origin( );
			if ( entity != local_player ) {
				if ( Controls.model_on_hit && !Global::matrix[ entity->Networkable( )->EntIndex( ) ].empty( ) ) {
					for ( int i = 0; i < Global::matrix[ entity->Networkable( )->EntIndex( ) ].size( ); i++ )
					{
						//if ( Global::matrix[ entity->Networkable( )->EntIndex( ) ].at( i ).flDisplayTime + 4.5f < Interfaces::GlobalVarsBase->CurrentTime )
						//	Global::matrix[ entity->Networkable( )->EntIndex( ) ].erase( Global::matrix[ entity->Networkable( )->EntIndex( ) ].begin( ) + i );

						hitchams->alpha_modulate( Global::matrix[ entity->Networkable( )->EntIndex( ) ][ i ].flDisplayTime + 5.f - Interfaces::GlobalVarsBase->CurrentTime );

						//entity->SetAbsAngles( Global::matrix[ entity->Networkable( )->EntIndex( ) ].at( i ).angles );
						entity->set_abs_origin( Global::matrix[ entity->Networkable( )->EntIndex( ) ].at( i ).origin );

						Interfaces::ModelRender->ForceMaterialOverride( hitchams );
						entity->renderable( )->draw_model( 1, 255 );

						hitchams->increment_reference_count( );
					}
					Interfaces::ModelRender->ForceMaterialOverride( nullptr );

					//entity->SetAbsAngles( old_angles );
					entity->set_abs_origin( old_origin );
				}
			}*/
			Global::scene_end = false;
		}
	}
}
