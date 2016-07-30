//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_sanfierro_included
  #endinput
#endif

#define _race_sanfierro_included
#pragma library race_sanfierro
#include "race"

static racename[MAX_NAME] = "San Fierro";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 90.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1754.3436,926.6018,24.4716},
{-1952.2273,924.3353,41.7985},
{-2059.5796,927.4534,57.4218},
{-2143.4741,900.8864,79.6135},
{-2163.3350,505.9830,34.7789},
{-2567.1965,407.1298,17.0804},
{-2646.4907,268.3049,3.9438}, 
{-2705.7979,181.5047,3.9417},
{-2703.3296,-210.8015,3.9396},
{-2724.1343,-311.7078,6.8753},
{-2724.1343,-311.7078,6.8753},
{-2763.9468,-405.6590,6.9095}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sanfierro_init();

public race_sanfierro_init() // unique named init function (must be ran on gamemode init)
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

