//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_overthehills_included
  #endinput
#endif

#define _race_overthehills_included
#pragma library race_overthehills
#include "race"

static racename[MAX_NAME] = "Over The Hills";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 225.0;
static Float:racecheckpoints[][CoordInfo] = {
{-295.1542,1573.3831,76.0111},
{-301.8010,1499.8794,75.6499},
{-305.9216,1395.1843,72.3068},
{-366.9702,1464.3815,63.1366},
{-324.9794,1317.3221,52.8868},
{-433.9323,1443.0209,33.6285},
{-341.0801,1263.0487,22.8778},
{-69.6579,1290.3396,11.6238},
{-176.6943,1600.4540,16.3333},
{-404.9345,1716.1780,40.7995},
{-403.5096,1915.8479,58.0001},
{-438.8009,1760.8464,72.0352},
{-383.1830,2075.0645,60.8733},
{-462.9485,2044.0658,60.8672},
{-375.7122,2205.6616,42.0938},
{-401.6114,2460.3984,42.6724}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_overthehills_init();

public race_overthehills_init() // unique named init function (must be ran on gamemode init)
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

