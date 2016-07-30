//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ (original race name: EastColinas)
// Converter: 	MadCat
//

#if defined _race_eastlascolinas_included
  #endinput
#endif

#define _race_eastlascolinas_included
#pragma library race_eastlascolinas
#include "race"

static racename[MAX_NAME] = "East Las Colinas";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 176.7941;
static Float:racecheckpoints[][CoordInfo] = {
{2602.7758,-1489.6369,16.0263},
{2566.8698,-1732.2001,1.2061},
{2567.7272,-2045.5838,3.4245},
{2582.5756,-2166.0302,-0.6536},
{2599.8256,-2047.3000,3.4255},
{2601.0012,-1737.4322,1.2060},
{2625.4963,-1618.1973,9.6986},
{2643.3090,-1470.5380,29.8462},
{2644.1721,-1246.0772,50.0572},
{2664.9152,-1048.3099,68.9792},
{2887.2319,-1049.4108,10.4253}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_eastlascolinas_init();

public race_eastlascolinas_init() // unique named init function (must be ran on gamemode init)
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

