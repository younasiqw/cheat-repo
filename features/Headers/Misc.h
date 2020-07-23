#pragma once
#include "../../entities/entity.h"
#include "../../render/render.h"
#include "../../Controls/Controls.h"
#include "../../hooks/hooks.h"
#include "../../Math/Math.h"
#include "../../menu/config.h"

class ceventlog {
public:
	std::string name;
	int time;
	int alpha;
	ceventlog(std::string name, int time) {
		this->name = name;
		this->time = time;
		alpha = 255;
	}
};
extern std::vector<ceventlog> event_vec;
class cmisc
{
public:
	void run_thirdperson( );
	void StartMoveFix();
	void EndMoveFix(  );
	void RunBunnyhop(  );
	void RunFakelag( );
	void PaintEvents();
	void PaintShotLog();
	void RunFakeduck();
	void OnPlayerDeath( std::string file );
	void RunBuyBot( );
	void RunStatsBox( );
private:
	int choke;
	bool run_buybot = true;
};

extern cmisc misc;