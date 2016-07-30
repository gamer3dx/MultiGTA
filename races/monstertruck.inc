//
// Created:     23.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_monstertruck_included
  #endinput
#endif

#define _race_monstertruck_included
#pragma library race_monstertruck
#include "race"


static racename[MAX_NAME] = "Monstertruck Madness";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_MONSTER};
static Float:startlineheading = 271.6779;
static Float:racecheckpoints[][CoordInfo] = {
{2440.9668,113.5096,26.8487},
{2551.2300,43.3540,26.7119},
{2706.8911,-18.2239,33.5599},
{2678.3398,-227.1222,34.2675},
{2712.9468,-449.5779,53.7797},
{2763.4717,-687.5537,65.0474},
{2763.4758,-687.3193,65.0584},
{2734.7915,-881.5974,46.9124},
{2451.9067,-970.9760,80.4371},
{2210.9900,-1018.4454,61.1580},
{2161.8081,-1019.6061,62.9544},
{2229.9709,-1134.7003,25.9963},
{2287.2305,-1117.3301,27.3203},
{2130.1616,-634.4129,61.9359},
{2018.6781,-516.7719,78.3279},
{2027.6985,-439.4772,77.3268},
{2510.0322,-282.2389,17.3027},
{2726.4839,-283.2032,17.3046},
{2826.2715,72.0825,27.2309},
{2766.7141,315.7903,8.7557},
{2767.4351,616.3610,8.8465},
{2767.2903,890.3668,11.3363}
};


static RaceID; // this will hold the id this race is assigned at init

forward race_monstertruck_init();

public race_monstertruck_init() // unique named init function (must be ran on gamemode init)
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
 	

