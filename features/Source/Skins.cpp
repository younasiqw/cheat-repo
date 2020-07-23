#include "../Headers/skins.h"
#include <string>
#include <intrin.h>
RecvVarProxyFn oRecvnModelIndex;
RecvVarProxyFn fnSequenceProxyFn = NULL;
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
c_skinchanger skin_changer;

void c_skinchanger::run_skin_changer() {
	if ( !Global::Local->IsAlive( ) || !Global::Local )
		return;

	auto weapons = Global::Local->GetWeapons( );
	if ( !weapons )
		return;

	for ( int i = 0; i < Interfaces::GlobalVarsBase->MaxClients; i++ ) {
		if ( weapons[ i ] == -1 )
			continue;

		C_BaseCombatWeapon* weapon = ( C_BaseCombatWeapon* ) Interfaces::EntityList->get_client_entity_from_handle( weapons[ i ] );
		if ( !weapon )
			continue;

		if ( weapon->clientclass( )->class_id != CKnife || Controls.knife_model < 1 )
			continue;

		weapon->ItemDefinitionIndex( ) = get_knife_model_indicies( );
		weapon->set_model_index( Interfaces::ModelInfo->GetModelIndex( get_knife_model( true ) ) );

		weapon->original_owner_xuid_low( 0 );
		weapon->original_owner_xuid_high( 0 );
		weapon->item_id_high( ) = -1;
		weapon->fallback_paint_kit( ) = 17;

		const auto world_model = ( C_BaseWeaponWorldModel* ) ( Interfaces::EntityList->get_client_entity_from_handle( weapon->weapon_world_model_handle( ) ) );
		if ( !world_model )
			continue;

		*world_model->GetModelIndex( ) = Interfaces::ModelInfo->GetModelIndex( get_knife_model( false ) );
	}
}

void set_view_model_sequence( const CRecvProxyData* pDataConst, void* pStruct, void* pOut ) {

	CRecvProxyData* data = const_cast< CRecvProxyData* >( pDataConst );
	C_BaseViewModel* view_model = ( C_BaseViewModel* ) pStruct;

	if ( view_model ) {
		C_CSPlayer* owner = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->get_client_entity_from_handle( CBaseHandle( view_model->GetOwner( ) ) ) );

		if ( owner && owner->Networkable( )->EntIndex( ) == Interfaces::engine->get_local_player( ) ) {

			void* view_model_inf = Interfaces::ModelInfo->get_model( view_model->GetModelIndex( ) );
			const char* model = Interfaces::ModelInfo->get_model_name( ( model_t* ) view_model_inf );
			int data_sequence = data->m_Value.m_Int;

			if ( !strcmp( model, "models/weapons/v_knife_butterfly.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_DRAW: data_sequence = RandomInt( SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2 ); break;
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence = RandomInt( SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03 ); break;
				default: data_sequence++;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_falchion_advanced.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_IDLE2: data_sequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1: data_sequence = RandomInt( SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP ); break;
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence = RandomInt( SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02 ); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1: break;
				default: data_sequence--;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_push.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_IDLE2: data_sequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2: data_sequence = RandomInt( SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5 ); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1: data_sequence = RandomInt( SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1 ); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1: break;
				default: data_sequence += 2;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_survival_bowie.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1: break;
				case SEQUENCE_DEFAULT_IDLE2: data_sequence = SEQUENCE_BOWIE_IDLE1; break;
				default: data_sequence--;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_ursus.mdl" ) || !strcmp( model, "models/weapons/v_knife_skeleton.mdl" )
				|| !strcmp( model, "models/weapons/v_knife_outdoor.mdl" ) || !strcmp( model, "models/weapons/v_knife_canis.mdl" )
				|| !strcmp( model, "models/weapons/v_knife_cord.mdl" ) )
			{
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_DRAW: data_sequence = RandomInt( SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2 ); break;
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence = RandomInt( SEQUENCE_BUTTERFLY_LOOKAT01, 14 ); break;
				default: data_sequence++; break;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_stiletto.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence = RandomInt( 12, 13 ); break;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_css.mdl.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence = RandomInt( 14, 15 ); break;
				}
			}
			else if ( !strcmp( model, "models/weapons/v_knife_widowmaker.mdl" ) ) {
				switch ( data_sequence ) {
				case SEQUENCE_DEFAULT_LOOKAT01: data_sequence = RandomInt( 14, 15 ); break;
				}
			}
			data->m_Value.m_Int = data_sequence;
		}
	}
	fnSequenceProxyFn( data, pStruct, pOut );
}

void recv_proxy_data_hook( CRecvProxyData* pData, void* pStruct, void* pOut ) {
	int default_t = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_default_t.mdl" );
	int default_ct = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_default_ct.mdl" );
	int bayonet = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_bayonet.mdl" );
	int butterfly = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_butterfly.mdl" );
	int flip = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_flip.mdl" );
	int gut = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_gut.mdl" );
	int karambit = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_karam.mdl" );
	int m9bayonet = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_m9_bay.mdl" );
	int huntsman = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_tactical.mdl" );
	int falchion = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_falchion_advanced.mdl" );
	int dagger = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_push.mdl" );
	int bowie = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_survival_bowie.mdl" );
	int gun_game = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_gg.mdl" );
	int classic = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_css.mdl" );
	int cord = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_cord.mdl" );
	int canis = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_canis.mdl" );
	int outdoor = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_outdoor.mdl" );
	int skeleton = Interfaces::ModelInfo->GetModelIndex( "models/weapons/v_knife_skeleton.mdl" );

	if ( Global::Local && Controls.knife_model > 0 ) {
		if ( Global::Local->IsAlive( ) && ( pData->m_Value.m_Int == default_t || pData->m_Value.m_Int == default_ct || pData->m_Value.m_Int == bayonet || pData->m_Value.m_Int == butterfly || pData->m_Value.m_Int == flip ||
			pData->m_Value.m_Int == gun_game || pData->m_Value.m_Int == classic || pData->m_Value.m_Int == cord || pData->m_Value.m_Int == outdoor || pData->m_Value.m_Int == skeleton || pData->m_Value.m_Int == canis || pData->m_Value.m_Int == gut ||
			pData->m_Value.m_Int == karambit || pData->m_Value.m_Int == m9bayonet || pData->m_Value.m_Int == huntsman || pData->m_Value.m_Int == falchion || pData->m_Value.m_Int == dagger || pData->m_Value.m_Int == bowie ) )
		{
			pData->m_Value.m_Int = Interfaces::ModelInfo->GetModelIndex( skin_changer.get_knife_model( ) );
		}
	}
	oRecvnModelIndex( pData, pStruct, pOut );
}

void c_skinchanger::apply_sequence_update( ) {

	ClientClass2* pClass = Interfaces::client->get_all_classes2( );
	while ( pClass ) {
		const char* pszName = pClass->pRecvTable->pNetTableName;

		if ( !strcmp( pszName, "DT_BaseViewModel" ) ) {
			RecvTable2* pClassTable = pClass->pRecvTable;

			for ( int nIndex = 0; nIndex < pClass->pRecvTable->nProps; nIndex++ ) {
				RecvProp2* pProp = &( pClass->pRecvTable->pProps[ nIndex ] );

				if ( !pProp || strcmp( pProp->pVarName, "data_sequence" ) )
					continue;

				fnSequenceProxyFn = ( RecvVarProxyFn ) pProp->ProxyFn;
				pProp->ProxyFn = set_view_model_sequence;
			}
		}

		if ( !strcmp( pszName, "DT_BaseViewModel" ) ) {
			for ( int i = 0; i < pClass->pRecvTable->nProps; i++ ) {
				RecvProp2* pProp = &( pClass->pRecvTable->pProps[ i ] );
				const char* name = pProp->pVarName;

				if ( !strcmp( name, "m_nModelIndex" ) ) {
					oRecvnModelIndex = ( RecvVarProxyFn ) pProp->ProxyFn;
					pProp->ProxyFn = ( RecvVarProxyFn ) recv_proxy_data_hook;
				}
			}
		}
		pClass = pClass->pNext;
	}
}