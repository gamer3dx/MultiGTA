//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_lsrace2_included
  #endinput
#endif

#define _race_lsrace2_included
#pragma library race_lsrace2
#include "race"

static racename[MAX_NAME] = "LSRace 2";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 9.5602;
static Float:racecheckpoints[][CoordInfo] = {
{2860.3979,-1304.5483,14.8463},
{2838.1360,-1041.4520,22.9481},
{2816.1543,-551.1018,10.5030},
{2781.2380,-474.3102,10.5029},
{2836.8813,-201.3644,15.2571},
{2874.0034,10.7072,17.2382},
{2762.9436,45.3147,21.1388},
{2535.3833,28.3088,26.0161},
{2375.7429,-27.8276,26.0169},
{2294.2969,-15.6986,26.0156},
{2261.8162,38.6796,26.0157},
{1893.2461,44.4995,34.6361},
{1502.6812,92.0784,29.5051},
{1158.7399,-174.3430,40.8905},
{522.0509,-175.9186,36.7208},
{407.9124,-620.5479,32.5111},
{265.0148,-1030.8605,55.9597},
{391.3763,-1168.4457,78.4532},
{663.0034,-1073.5166,48.2069},
{842.0735,-870.4026,69.4939},
{1047.9963,-778.4110,104.8836},
{1369.1682,-677.2889,93.2179},
{1351.8022,-575.3937,89.2637},
{1163.5756,-632.8982,103.4299},
{623.1299,-904.4813,62.7901},
{391.8871,-1015.7651,91.8023},
{153.0015,-1219.3154,45.1416},
{158.5267,-1416.6545,42.6332},
{108.8642,-1533.8445,6.3703},
{219.7646,-1638.6219,13.0745},
{335.9173,-1742.6293,4.1473},
{351.2616,-1790.2089,4.7513}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lsrace2_init();

public race_lsrace2_init() // unique named init function (must be ran on gamemode init)
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

