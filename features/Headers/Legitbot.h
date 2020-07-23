#pragma once
#include "../../hooks/hooks.h"
#include "Ragebot.h"
#include "autowall.h"
class clegitbot {
public:
	void on_move( );
	void run_triggerbot( C_BaseCombatWeapon* weapon );

	struct aiminfo {
		Vector3D hitbox;
		float distance;
		Vector3D CalcAngle;
	} info;
};
extern clegitbot legitbot;