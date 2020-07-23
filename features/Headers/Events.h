#pragma once
#include "../../hooks/hooks.h"
#include "../../menu/config.h"
#include "Ragebot.h"
extern void weapon_fire( IGameEvent* event );
extern void bullet_impact( IGameEvent* event );
extern void bomb_planted( IGameEvent* event );
extern void player_hurt( IGameEvent* event );
extern void bomb_defused( IGameEvent* event );
extern void item_purchase( IGameEvent* event );
extern void player_footstep( IGameEvent* event );
extern void draw_step_circle( color colors, Vector3D position );
extern void player_death( IGameEvent* event );
extern void smokegrenade_detonate( IGameEvent* event );

class cevents
{
public:
	void on_attach( );
	void on_detach( );
	std::string hitrgoupstr( int hitgroup )
	{
		switch ( hitgroup ) {
		case 1: return "HEAD"; break;
		case 2: return "CHEST"; break;
		case 3: return "STOMACH"; break;
		case 4: return "LEFT ARM"; break;
		case 5: return "RIGHT ARM"; break;
		case 6: return "LEFT LEG"; break;
		case 7: return "RIGHT LEG"; break;
		default: return "BODY";
		}
	}
private:
	DamageEventListener* weaponfire;
	DamageEventListener* playerhurt;
	DamageEventListener* bulletimpact;
	DamageEventListener* bombplanted;
	DamageEventListener* bombdefused;
	DamageEventListener* itempurchase;
	DamageEventListener* playerfootstep;
	DamageEventListener* playerdeath;
	DamageEventListener* smokegrenadedetonate;
};
extern cevents events;

