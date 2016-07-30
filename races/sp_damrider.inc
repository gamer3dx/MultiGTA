//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_damrider_included
  #endinput
#endif

#define _race_sp_damrider_included
#pragma library race_sp_damrider
#include "race"

static racename[MAX_NAME] = "(SP)Dam Rider";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_NRG500,V_FCR900,V_PCJ600};
static Float:startlineheading = 170.0;
static Float:racecheckpoints[][CoordInfo] = {
{-878.0113,1947.484,59.1845},
{-857.6418,1873.25,59.183},
{-1030.689,1853.546,58.8558},
{-1093.182,1748.023,31.6318},
{-902.488,1687.666,26.267},
{-847.1951,1495.58,17.5443},
{-782.4052,1305.81,12.6016},
{-616.3677,1193.645,9.2126},
{-474.1805,1052.086,10.0313},
{-319.1918,925.6066,10.3099},
{-267.4492,825.0159,13.2739},
{-188.4539,1007.19,18.5885},
{-164.8873,1198.263,18.5938},
{-142.1028,1250.022,18.5869},
{-341.5205,1265.695,22.1515},
{-440.031,1439.669,32.334},
{-438.1716,1639.673,34.6319},
{-381.6811,1831.026,49.8567},
{-433.4627,1883.992,59.8611},
{-465.4392,1768.532,71.9576},
{-464.5399,1968.423,81.2536},
{-411.0666,2074.596,60.6433},
{-610.1716,2050.887,59.1798},
{-790.69,2053.91,59.1798}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_damrider_init();

public race_sp_damrider_init() // unique named init function (must be ran on gamemode init)
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

