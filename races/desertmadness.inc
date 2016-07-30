//
// Created:     Unknown
// Aurthor:    	DracoBlue
// Converter: 	MadCat
//

#if defined _race_desertmadness_included
  #endinput
#endif

#define _race_desertmadness_included
#pragma library race_desertmadness
#include "race"

forward race_desertmadness_init();

static racename[MAX_NAME] = "Desert Madness";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 19.4459;
static Float:racecheckpoints[][CoordInfo] = {
{-2681.2217,2204.1177,55.4284},
{-2710.2295,2531.9883,76.8421},
{-2282.1865,2640.3154,55.4297},
{-1963.6248,2519.8911,55.6124},
{-1792.5736,2190.6296,15.9926},
{-1605.9341,1985.9574,18.1792},
{-1535.3855,1850.4476,28.8315},
{-1308.2968,1853.6447,38.3930},
{-1019.3351,1855.8835,61.6327},
{-891.6075,1793.2700,60.1558},
{-884.8877,1974.8967,60.2056},
{-498.9703,1986.9784,60.1875},
{-383.9112,2075.6394,60.9066},
{-480.0778,1939.0903,86.4313},
{-448.3761,1757.2957,72.0625},
{-414.7153,1917.8036,57.5720},
{-401.2432,1746.8286,42.3020},
{-439.5034,1434.8472,33.1780},
{-366.7043,1285.4880,24.4513},
{-136.8959,1251.2456,19.2491},
{-114.3461,1206.0704,19.5938},
{-180.6803,1198.9581,19.5869},
{-189.9950,1107.4268,19.5931},
{-80.2044,1098.3799,19.5938},
{-65.3099,1183.9861,19.5853},
{116.6861,1198.3247,18.0781},
{178.3199,1146.3450,14.2611},
{346.3835,1398.9991,6.7451},
{522.2918,1675.0626,12.208},
{590.0461,1836.1155,14.169},
{588.0261,2129.6440,38.826},
{547.4000,2326.2407,32.497},
{436.9457,2378.1404,26.639},
{307.8759,2290.1575,25.144},
{-34.2623,2301.6616,24.983},
{-101.6164,2443.4092,15.0854},
{-311.3797,2521.0708,34.6200},
{-528.4926,2422.9319,59.5954},
{-711.3705,2530.5603,74.1524},
{-740.3790,2650.9927,64.1261},
{-699.4290,2688.7192,56.3222},
{-774.8487,2694.9023,47.1203},
{-779.8789,2716.8301,45.2979},
{-1217.9634,2684.4548,46.2030},
{-1332.6854,2643.6489,50.2061},
{-1445.6566,2725.2620,64.4525},
{-1737.5898,2727.3923,60.4538},
{-1834.5874,2687.3398,55.0831},
{-1928.0001,2612.4158,47.2673},
{-2294.3291,2676.0623,56.1461},
{-2700.9158,2650.3264,86.9902},
{-2770.0264,2372.5742,78.0366},
{-2710.0400,2364.8213,71.0947},
{-2609.8721,2500.9224,25.8188},
{-2511.3870,2430.6072,16.6719},
{-2382.0825,2426.7949,9.0312},
{-2262.7786,2342.5852,4.8125}

};

static RaceID; // this will hold the id this race is assigned at init

public race_desertmadness_init() // unique named init function (must be ran on gamemode init)
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

