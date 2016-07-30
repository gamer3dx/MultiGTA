//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_sffastlane_included
  #endinput
#endif

#define _race_sp_sffastlane_included
#pragma library race_sp_sffastlane
#include "race"

static racename[MAX_NAME] = "(SP)San-Fierro Fastlane";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_ALPHA,V_CLUB,V_ELEGY,V_FLASH,V_URANUS};
static Float:startlineheading = 314.1325;
static Float:racecheckpoints[][CoordInfo] = {
{-2657.164,1153.872,34.7396},
{-2593.221,1217.854,34.7402},
{-2312.288,1176.387,49.608},
{-2157.193,1268.196,26.0127},
{-1959.526,1287.205,6.7514},
{-1741.076,1325.298,6.083},
{-1759.865,1269.147,8.8598},
{-1884.242,1160.69,45.0107},
{-1897.101,960.1686,34.7366},
{-1998.178,841.1865,45.0219},
{-2189.258,808.5273,56.0602},
{-2389.186,808.6409,34.7495},
{-2589.692,808.5219,49.5411},
{-2711.488,812.8809,48.9309},
{-2750.894,890.6993,65.8952},
{-2653.912,1157.345,34.7386}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_sffastlane_init();

public race_sp_sffastlane_init() // unique named init function (must be ran on gamemode init)
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

