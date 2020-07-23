#include "../hooks.h"
#include "../../menu/config.h"
namespace n_hooks {
	int __fastcall n_functions::hkDoPostScreenEffects( void* _this, int edx, int a1 )
	{
		static auto OriginalFn = n_hooks::ClientMode->get_original_function< decltype( &hkDoPostScreenEffects ) >( 44 );

		//std::cout << "BEFORE" << "\n";
		for ( auto i = 0; i < Interfaces::glow_manager->m_GlowObjectDefinitions.count( ); i++ ) {
			auto& glow = Interfaces::glow_manager->m_GlowObjectDefinitions[ i ];
			auto entity = reinterpret_cast< C_CSPlayer* >( glow.m_pEntity );

			auto local_player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( Interfaces::engine->get_local_player( ) ) );

			if ( entity != nullptr && !glow.IsUnused( ) ) {

				if ( strstr( entity->clientclass( )->network_name, "CWeapon" ) ) {
					if ( Controls.weapon_esp[ 0 ] ) {
						glow.m_flRed = Controls.entity_glow_color[ 0 ].r / 255.f;
						glow.m_flGreen = Controls.entity_glow_color[ 0 ].g / 255.f;
						glow.m_flBlue = Controls.entity_glow_color[ 0 ].b / 255.f;
						glow.m_flAlpha = Controls.entity_glow_color[ 0 ].a / 255.f;
						glow.m_bRenderWhenOccluded = true;
						glow.m_bRenderWhenUnoccluded = false;
					}
					else {
						glow.m_flAlpha = 0.f;
					}
				}
				if ( strstr( entity->clientclass( )->network_name, "CSmoke" ) || strstr( entity->clientclass( )->network_name, "CHE" )
					|| strstr( entity->clientclass( )->network_name, "CMolo" ) || strstr( entity->clientclass( )->network_name, "CFlash" ) || strstr( entity->clientclass( )->network_name, "CDecoy" ) ) 
				{
					if ( Controls.grenade_esp[ 0 ] ) {
						glow.m_flRed = Controls.entity_glow_color[ 1 ].r / 255.f;
						glow.m_flGreen = Controls.entity_glow_color[ 1 ].g / 255.f;
						glow.m_flBlue = Controls.entity_glow_color[ 1 ].b / 255.f;
						glow.m_flAlpha = Controls.entity_glow_color[ 1 ].a / 255.f;
						glow.m_bRenderWhenOccluded = true;
						glow.m_bRenderWhenUnoccluded = false;
					}
					else {
						glow.m_flAlpha = 0.f;
					}
				}

				auto class_id = entity->clientclass( )->class_id;

				if ( !entity->is_player( ) || entity->IsDormant( ) || !entity->GetHealth( ) )
					continue;


				switch ( class_id ) {
				case CCSPlayer:

					//if (e->GetTeam() != local_player->GetTeam()) {

					if ( entity->GetTeam( ) == local_player->GetTeam( ) ) {
						if ( entity == local_player ) {
							if ( Controls.glowf[ 2 ] ) {
								Global::post_screen = true;
								glow.m_flRed = Controls.glow_colorf[ 2 ].r / 255.f;
								glow.m_flGreen = Controls.glow_colorf[ 2 ].g / 255.f;
								glow.m_flBlue = Controls.glow_colorf[ 2 ].b / 255.f;
								glow.m_flAlpha = Controls.glow_colorf[ 2 ].a / 255.f;
							}
						}
						else {
							if ( Controls.glowf[ 1 ] ) {
								Global::post_screen = true;
								glow.m_flRed = Controls.glow_colorf[ 1 ].r / 255.f;
								glow.m_flGreen = Controls.glow_colorf[ 1 ].g / 255.f;
								glow.m_flBlue = Controls.glow_colorf[ 1 ].b / 255.f;
								glow.m_flAlpha = Controls.glow_colorf[ 1 ].a / 255.f;
							}
						}
					}
					else {
						if ( Controls.glowf[ 0 ] ) {
							Global::post_screen = true;
							glow.m_flRed = Controls.glow_colorf[ 0 ].r / 255.f;
							glow.m_flGreen = Controls.glow_colorf[ 0 ].g / 255.f;
							glow.m_flBlue = Controls.glow_colorf[ 0 ].b / 255.f;
							glow.m_flAlpha = Controls.glow_colorf[ 0 ].a / 255.f;
						}
					}

					glow.m_bRenderWhenOccluded = true;
					glow.m_bRenderWhenUnoccluded = false;
				}
			}
		}
		Global::post_screen = false;
		//std::cout << "RETURNED" << "\n";
		return OriginalFn( Interfaces::ClientMode, edx, a1 );
	}
}