//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_littleloop_included
  #endinput
#endif

#define _race_sp_littleloop_included
#pragma library race_sp_littleloop
#include "race"

static racename[MAX_NAME] = "(SP)Little Loop";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_NRG500,V_FCR900};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
{2875.192,-1458.85,10.5892},
{2875.362,-1404.768,10.7143},
{2797.037,-1291.152,41.389},
{2609.984,-1256.458,47.9322},
{2371.282,-1280.477,23.6341},
{2450.557,-1444.24,23.6318},
{2618.436,-1444.826,31.6234},
{2657.3,-1404.327,30.0795},
{2680.506,-1489.556,30.2098},
{2755.952,-1488.515,29.3812}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_littleloop_init();

public race_sp_littleloop_init() // unique named init function (must be ran on gamemode init)
{
	RaceID = RegisterRace(racename);
	if (RaceID == INVALID_RACE_ID) return;

	Race[RaceID][race_frequency] = racefrequency;
 	Race[RaceID][race_lineupdelay] = lineupdelay;
 	Race[RaceID][race_minracers] = minracers;

	Race[RaceID][race_startheading] = startlineheading;
	set(Race[RaceID][race_vehicles],allowedcars);

	RaceSize[RaceID] = sizeof(racecheckpoints);
	for (new cpid=0;cpid<sizeof(racecheckpoints);cpid++)
	{
		RaceCheckpoints[RaceID][cpid][Coord_X] = racecheckpoints[cpid][Coord_X];
		RaceCheckpoints[RaceID][cpid][Coord_Y] = racecheckpoints[cpid][Coord_Y];
		RaceCheckpoints[RaceID][cpid][Coord_Z] = racecheckpoints[cpid][Coord_Z];
	}

	RaceStats[RaceID][race_timer] = MakeRaceSleepTime(RaceID);
	Race[RaceID][race_cashprize] = CalculateRewardMoney(RaceID); 
	Race[RaceID][race_cashentry] = CalculateEntryFee(RaceID); 
	Race[RaceID][race_xpprize] = CalculateRewardXP(RaceID); 
	Race[RaceID][race_maxracetime] = CalculateMaxRaceTime(RaceID);
    RaceStats[RaceID][race_state] = RACE_STATE_SLEEPING;

}

