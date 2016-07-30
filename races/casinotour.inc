//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_casinotour_included
  #endinput
#endif

#define _race_casinotour_included
#pragma library race_casinotour
#include "race"

static racename[MAX_NAME] = "Casino Tour";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{2049.8652,938.7567,9.3167},
{2088.2185,851.9510,6.3554},
{2285.4741,907.7445,10.2877},
{2310.2722,714.2592,10.2970},
{2545.4888,786.0992,10.2969},
{2527.4429,1086.4207,10.2968},
{2546.5313,1392.5409,10.4578},
{2538.8367,1613.5020,10.3318},
{2608.4846,1726.7882,10.4452},
{2508.8655,1837.2635,10.3047},
{2462.9502,1972.1683,10.2968},
{2328.5710,2021.2909,10.2972},
{2128.3584,1995.6837,10.3007},
{2114.5774,1150.7073,13.1300}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_casinotour_init();

public race_casinotour_init() // unique named init function (must be ran on gamemode init)
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

