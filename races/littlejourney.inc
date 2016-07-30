//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_littlejourney_included
  #endinput
#endif

#define _race_littlejourney_included
#pragma library race_littlejourney
#include "race"

static racename[MAX_NAME] = "Little Journey";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 90.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2368.0000,1379.0000,6.8000},
{-2528.3464,1375.6029,6.6641},
{-2840.7727,1244.9110,5.2526},
{-2881.3677,1134.1848,19.9439},
{-2823.1367,900.1500,43.5311},
{-2853.8469,445.5127,3.9804},
{-2809.2769,74.7827,6.6631},
{-2764.8413,-502.9003,6.8424},
{-2668.3347,-485.9198,22.3671},
{-2378.0408,-380.5171,76.0722},
{-2343.2861,-460.4362,79.6410},
{-2628.7117,-494.8253,69.7813},
{-2466.7876,-495.4199,104.3873},
{-2343.4341,-788.7264,93.4383},
{-2208.2705,-752.9784,62.7743},
{-2211.7266,-980.6576,38.0720},
{-1896.5822,-1380.9646,39.5201},
{-1545.3030,-1593.2690,37.3573},
{-2012.7313,-1883.9139,44.5558},
{-2442.1516,-2256.4453,15.6178},
{-2916.0176,-1621.3160,12.4284},
{-2881.2583,-1013.1700,9.1153},
{-2813.7854,-456.3710,6.7453},
{-2785.7107,-330.2958,6.6630},
{-2649.7593,-291.7804,7.1671}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_littlejourney_init();

public race_littlejourney_init() // unique named init function (must be ran on gamemode init)
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

