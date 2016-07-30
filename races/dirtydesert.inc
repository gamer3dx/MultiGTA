//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_dirtydesert_included
  #endinput
#endif

#define _race_dirtydesert_included
#pragma library race_dirtydesert
#include "race"

static racename[MAX_NAME] = "Dirty Desert";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ};
static Float:startlineheading = 18.0;
static Float:racecheckpoints[][CoordInfo] = {
{-96.0000,1484.0000,11.0000},
{-148.7928,1595.1973,16.9940},
{-231.3813,1633.0071,25.1549},
{-306.1346,1673.9835,27.8545},
{-406.2258,1688.4559,38.3082},
{-365.7274,1755.6154,42.4025},
{-398.8960,1845.4202,52.8941},
{-439.1153,1759.9121,71.7067},
{-466.2158,1966.8381,82.2996},
{-393.4998,2081.4487,60.9993},
{-467.1420,2052.9773,60.5354},
{-380.9936,2226.6089,41.7632},
{-421.8212,2291.0657,42.2354},
{-601.4646,2393.1760,86.3789},
{-720.2062,2450.1350,124.7621},
{-743.2903,2345.4666,124.4714},
{-841.2864,2416.1455,120.4353},
{-771.6838,2559.9800,85.4555},
{-713.7250,2521.1787,74.5538},
{-516.8535,2432.5073,57.6740},
{-592.6600,2518.2554,76.7202},
{-586.6263,2691.4553,72.0730},
{-669.6572,2690.7708,72.4696},
{-825.3923,2729.5259,45.2539},
{-1222.8938,2674.0979,45.9931},
{-1109.8296,2465.4163,80.4160},
{-1111.6053,2369.1992,84.6986},
{-1219.0592,2216.0693,104.7713},
{-1344.0342,2288.1875,94.1182},
{-1283.2471,2431.7024,88.2085},
{-1301.7217,2516.7305,86.8355}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_dirtydesert_init();

public race_dirtydesert_init() // unique named init function (must be ran on gamemode init)
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

