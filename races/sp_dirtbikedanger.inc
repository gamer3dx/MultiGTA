//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_dbdanger_included
  #endinput
#endif

#define _race_sp_dbdanger_included
#pragma library race_sp_dirtbikedanger
#include "race"

static racename[MAX_NAME] = "(SP)Dirtbike Danger";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ};
static Float:startlineheading = 50.9076;
static Float:racecheckpoints[][CoordInfo] = {
{-796.8273,-2468.529,83.6549},
{-938.5198,-2364.511,57.0608},
{-1065.656,-2379.436,46.5486},
{-1247.823,-2313.528,19.8007},
{-1356.137,-2180.365,21.3467},
{-1541.303,-2154.782,5.8607},
{-1674.876,-2235.132,33.8691},
{-1839.937,-2349.368,31.9587},
{-1841.037,-2454.4,27.4044},
{-1702.044,-2587.976,26.1706},
{-1508.332,-2634.733,46.8766},
{-1310.905,-2637.832,12.808},
{-1122.616,-2652.592,15.4117},
{-934.2363,-2668.473,80.3479},
{-753.5882,-2672.785,84.0788},
{-668.018,-2496.988,37.5787},
{-550.4019,-2334.538,26.8502},
{-366.3848,-2257.25,41.7144},
{-276.1146,-2189.364,27.7264}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_dirtbikedanger_init();

public race_sp_dirtbikedanger_init() // unique named init function (must be ran on gamemode init)
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

