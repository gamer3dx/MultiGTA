//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_tolsma_included
  #endinput
#endif

#define _race_tolsma_included
#pragma library race_tolsma
#include "race"

static racename[MAX_NAME] = "Tolsma Beginning";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ,V_BF400,V_NRG500};
static Float:startlineheading = 2.0725;
static Float:racecheckpoints[][CoordInfo] = {
	{-2466.1228,2233.4314,4.0904},
	{-2495.3354,2292.5574,4.2680},
	{-2602.4617,2278.0642,7.6430},
	{-2582.6208,2331.2820,4.8021},
	{-2482.8782,2383.3501,12.4832},
	{-2599.0508,2493.9336,22.7826},
	{-2677.7957,2423.3071,51.8139},
	{-2699.0256,2359.2585,70.2152},
	{-2786.9138,2333.5564,79.2322},
	{-2823.5393,2378.8794,102.5622},
	{-2826.7163,2423.3049,111.9351},
	{-2806.9763,2479.7598,108.9405},
	{-2796.4172,2594.0601,113.5658},
	{-2747.6057,2649.0413,100.7013},
	{-2721.0574,2696.8987,106.3175},
	{-2635.0566,2714.0505,92.3162},
	{-2552.2458,2726.1377,91.9948},
	{-2403.2334,2720.0105,84.2616},
	{-2250.0698,2676.1355,79.2267},
	{-2283.0044,2547.1575,4.3349},
	{-2348.4207,2498.5837,0.6907},
	{-2359.2656,2436.9182,7.2499},
	{-2353.9934,2359.6650,4.2907},
	{-2443.0078,2333.2876,4.1197},
	{-2466.1228,2233.4314,4.0904}
};

static RaceID;

forward race_tolsma_init();

public race_tolsma_init()
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

