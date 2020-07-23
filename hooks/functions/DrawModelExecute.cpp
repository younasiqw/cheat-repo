#include "../hooks.h"
#include "../../Controls/Controls.h"
#include "../../menu/config.h"
#include "../../features/Headers/Lagcomp.h"
#include <iostream>
#include <exception>
#include <stdexcept>
namespace n_hooks {
	bool isWeapon( const char* const mname) {
		if ( ( strstr( mname, "v_" )  || strstr( mname, "uid" ) || strstr( mname, "stattrack" ) ) && !strstr( mname, "arms" ) ) {
			return true;
		}

		return false;
	}
	void __fastcall n_functions::DrawModelExecute( std::uintptr_t ecx, std::uintptr_t edx, void* conText, void* state, ModelRenderInfo_t& info, void* custom_bone_to_world ) {
		static auto OriginalFn = n_hooks::ModelRender->get_original_function< decltype( &DrawModelExecute ) >( 21 );

		if ( Interfaces::ModelRender->is_forced_material_override( ) ) {
			return OriginalFn( ecx, edx, conText, state, info, custom_bone_to_world );
		}

		C_CSPlayer* model_entity = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( info.entity_index ) );


		const char* const model_name = Interfaces::ModelInfo->get_model_name( info.model );

		static IMaterial* Textured = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures", true, nullptr );
		if ( Textured )
			Textured->increment_reference_count( );

		float old_color[ 4 ] = { 0.f };

		float fakecolor[ 4 ] {
			Controls.fakechams_color.r / 255.f,
			Controls.fakechams_color.g / 255.f,
			Controls.fakechams_color.b / 255.f,
			Controls.fakechams_color.a / 255.f
		};

		float armcolor[ 4 ] {
			Controls.arm_color.r / 255.f,
			Controls.arm_color.g / 255.f,
			Controls.arm_color.b / 255.f,
			Controls.arm_color.a / 255.f
		};

		float weaponcolor[ 4 ] {
			Controls.weaponc_color.r / 255.f,
			Controls.weaponc_color.g / 255.f,
			Controls.weaponc_color.b / 255.f,
			Controls.weaponc_color.a / 255.f
		};

		if ( !Global::Local )
			return;

		Interfaces::RenderView->GetColorModulation( old_color );
		if ( model_entity == Global::Local && Controls.fakelag_chams && !Global::scene_end && !Global::Local->IsScoped( ) && Global::Local->get_velocity( ).Length( ) > 30 ) {
			Interfaces::ModelRender->ForceMaterialOverride( Textured );
			Interfaces::RenderView->SetBlend( fakecolor[ 3 ] );
			Interfaces::RenderView->SetColorModulation( fakecolor );
			OriginalFn( ecx, edx, conText, state, info, Global::local_last_bone_matrix );
			Interfaces::RenderView->SetColorModulation( old_color );
			//if ( Controls.chams_colorf[ 2 ].a == 255 )
			//	Interfaces::RenderView->SetBlend( 1.0f );

			Interfaces::ModelRender->ForceMaterialOverride( nullptr );
		}

		/*static IMaterial* weapon_mat;
		if ( strstr( model_name, "weapon" ) && Controls.weapon_mat > 0 ) {

			switch ( Controls.weapon_mat ) {
			case 1: weapon_mat = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures" ); break; //Material
			case 2: weapon_mat = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures" ); break; //Flat
			case 3: weapon_mat = Interfaces::MaterialSystem->get_material( "metallic", nullptr, true, nullptr ); break; //Metallic
			case 4: weapon_mat = Interfaces::MaterialSystem->get_material( "pearl", nullptr, true, nullptr ); break; //Pearl
			case 5: weapon_mat = Interfaces::MaterialSystem->get_material( "models/inventory_items/trophy_majors/crystal_clear", nullptr, true, nullptr ); //Crystal
			}

			Interfaces::ModelRender->ForceMaterialOverride( weapon_mat );

			if ( Controls.weapon_mat == 5 ) {
				auto tint = weapon_mat->FindVar( _( "$envmaptint" ) );
				if ( tint )
					tint->set_vec_value( weaponcolor[ 0 ], weaponcolor[ 1 ], weaponcolor[ 2 ] );
			}

			std::cout << "r" << std::endl;
			Interfaces::RenderView->SetBlend( weaponcolor[ 3 ] );
			Interfaces::RenderView->SetColorModulation( weaponcolor );
			OriginalFn( ecx, edx, conText, state, info, custom_bone_to_world );


			Interfaces::RenderView->SetColorModulation( old_color );
			Interfaces::ModelRender->ForceMaterialOverride( nullptr );
			//Interfaces::ModelRender->ForceMaterialOverride( nullptr );
		}*/

		static IMaterial* arm_mat;
		if ( strstr( model_name, "arms" ) && Controls.arm_mat > 0) {

			switch ( Controls.arm_mat ) {
			case 1: arm_mat = Interfaces::MaterialSystem->get_material( "debug/debugambientcube", "Model Textures" ); break; //Material
			case 2: arm_mat = Interfaces::MaterialSystem->get_material( "debug/debugdrawflat", "Model Textures" ); break; //Flat
			case 3: arm_mat = Interfaces::MaterialSystem->get_material( "metallic", nullptr, true, nullptr ); break; //Metallic
			case 4: arm_mat = Interfaces::MaterialSystem->get_material( "pearl", nullptr, true, nullptr ); break; //Pearl
			case 5: arm_mat = Interfaces::MaterialSystem->get_material( "models/inventory_items/trophy_majors/crystal_clear", nullptr, true, nullptr ); //Crystal
			}
			arm_mat->increment_reference_count( );

			Interfaces::ModelRender->ForceMaterialOverride( arm_mat );

			if ( Controls.arm_mat == 5 ) {
				auto tint = arm_mat->FindVar( _( "$envmaptint" ) );
				if ( tint )
					tint->set_vec_value( armcolor[ 0 ], armcolor[ 1 ], armcolor[ 2 ] );
			}

			Interfaces::RenderView->SetBlend( armcolor[ 3 ] );
			Interfaces::RenderView->SetColorModulation( armcolor );
			OriginalFn( ecx, edx, conText, state, info, custom_bone_to_world );
			//Interfaces::ModelRender->ForceMaterialOverride( nullptr );
		}

		/*static IMaterial* hitchams = Interfaces::MaterialSystem->get_material( "models/inventory_items/trophy_majors/gloss", nullptr, true, nullptr );
		if ( model_entity != local_player && Global::matrix && hitchams && model_entity ) {
			if ( !Global::matrix[ model_entity->Networkable( )->EntIndex( ) ].empty( ) ) {
				for ( int i = 0; i < Global::matrix[ model_entity->Networkable( )->EntIndex( ) ].size( ); i++ )
				{
					try {
						hitchams->alpha_modulate( Global::matrix[ model_entity->Networkable( )->EntIndex( ) ].at( i ).flDisplayTime + 5.f - Interfaces::GlobalVarsBase->CurrentTime );
						Interfaces::ModelRender->ForceMaterialOverride( hitchams );

						OriginalFn( ecx, edx, conText, state, info, Global::matrix[ model_entity->Networkable()->EntIndex() ].at( i ).matrix );

						//if ( Global::matrix[ info.entity_index ].at( i ).flDisplayTime + 5.f < Interfaces::GlobalVarsBase->CurrentTime )
							//Global::matrix[ info.entity_index ].erase( Global::matrix[ info.entity_index ].begin( ) + i );
					}
					catch ( const std::out_of_range & e ) {
						std::cout << "fuck.";
					}


					hitchams->increment_reference_count( );
				}
				Interfaces::ModelRender->ForceMaterialOverride( nullptr );
			}
		}*/

		if ( Global::Local->IsScoped( ) ) {
			if ( strstr( model_name, "models/weapons/v_" ) || strstr( model_name, "weapons" ) || strstr( model_name, "models/weapons/w_" ) )
				Interfaces::RenderView->SetBlend( 0.25f );

			if ( model_entity == Global::Local )
				Interfaces::RenderView->SetBlend( 0.25f );
		}


		if ( model_entity != Global::Local ) {
			if ( model_entity != nullptr && Global::Local != nullptr && model_entity->GetTeam( ) == Global::Local->GetTeam( ) && !Controls.render_teammates ) {

			}
			else {
				OriginalFn( ecx, edx, conText, state, info, custom_bone_to_world );
			}
		}
		else {
			OriginalFn( ecx, edx, conText, state, info, custom_bone_to_world );
		}

		if ( !Global::scene_end ) {
			if ( model_entity == Global::Local )
				Interfaces::ModelRender->ForceMaterialOverride( nullptr );
		}
	}
}