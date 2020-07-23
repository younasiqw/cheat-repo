#include "../Headers/Legitbot.h"

void clegitbot::run_triggerbot( C_BaseCombatWeapon* weapon ) {
	if ( Controls.triggerbot ) {
		C_Ray ray;
		C_Trace trace;
		C_TraceFilter filter( Global::Local );

		Vector3D traceStart, traceEnd;

		//Save viewAngles and compensate punchAngles
		Vector3D viewAngles;
		Interfaces::engine->get_view_angles( viewAngles );
		Vector3D viewAnglesRcs = viewAngles + Global::Local->aim_punch_angle( ) * 2.0f;

		Math.AngleVectors( viewAnglesRcs, &traceEnd );

		traceStart = Global::Local->GetBonePosition( 8 );
		traceEnd = traceStart + ( traceEnd * 8192.0f );

		Interfaces::Trace->TraceRay( C_Ray( traceStart, traceEnd ), 0x46004003, &filter, &trace );

		if ( !trace.m_pEnt || trace.m_pEnt->clientclass( )->class_id != CCSPlayer )
			return;

		if ( trace.m_pEnt->IsImmune( ) || !trace.m_pEnt->IsAlive( ) || trace.m_pEnt->GetTeam( ) == Global::Local->GetTeam( ) )
			return;

		/* sanity check, only shoot if weapon*/
		if ( Global::Local->IsKnife( ) || Global::Local->WeaponNade( ) )
			return;

		/* update accuracy and check if higher than hitchance */
		weapon->UpdateAccuracyPenalty( );

		if ( 1 / weapon->GetInaccuracy( ) <= Controls.trigger_hitchance )
			return;

		Global::uCmd->buttons |= in_attack;
	}
}

void clegitbot::on_move( ) {

	C_BaseCombatWeapon* weapon = Global::Local->GetActiveWeapon( );
	if ( !weapon )
		return;

	CCSWeaponData* WeaponData = weapon->GetWeaponData( );
	if ( !WeaponData )
		return;

	auto auto_pistol = [ ] ( C_BaseCombatWeapon* weapon, CCSWeaponData* WeaponData ) -> void {
		if ( WeaponData->type == ( int ) c_weapon_types::type_pistol ) {
			if ( weapon->m_flNextPrimaryAttack( ) < Interfaces::GlobalVarsBase->CurrentTime )
				return;

			if ( !Controls.auto_pistol )
				return;

			Global::uCmd->buttons &= ~in_attack;
		}
	};

	/* run lambda for auto pistol, before check if legit aim, can be used rage aswell*/
	auto_pistol( weapon, WeaponData );

	/* run triggerbot, before we check if aim active */
	this->run_triggerbot( weapon );

	if ( !Controls.legit_aim )
		return;

	/* sanity check, only aim if weapon*/
	if ( Global::Local->IsKnife( ) || Global::Local->WeaponNade( ) )
		return;

	for ( int i = 1; i <= Interfaces::GlobalVarsBase->MaxClients; i++ ) {
		C_CSPlayer* player = reinterpret_cast< C_CSPlayer* >( Interfaces::EntityList->GetClientEntity( i ) );

		if ( !player )
			continue;

		Vector3D destination = player->GetBonePosition( 8 );
		Vector3D angle_to_player = Math.CalcAngle( Global::Local->GetEyePosition(), destination );

		float dist_to_player = aimbot.CalculateDist( Global::uCmd->viewangles, angle_to_player );

		if ( dist_to_player >= Controls.legit_fov || angle_to_player == Vector3D( 0, 0, 0 ) )
			continue;

		if ( !( Global::uCmd->buttons & in_attack ) )
			continue;

		angle_to_player.clamp( );

		Global::uCmd->viewangles = angle_to_player - ( Global::Local->aim_punch_angle( ) * Interfaces::Convar->FindVar( "weapon_recoil_scale" )->GetFloat( ) );
	}
}
clegitbot legitbot;