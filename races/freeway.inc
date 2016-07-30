//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_freeway_included
  #endinput
#endif

#define _race_freeway_included
#pragma library race_freeway
#include "race"

static racename[MAX_NAME] = "Freeway";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 170.2866;
static Float:racecheckpoints[][CoordInfo] = {
{1239.1427,1695.1627,6.3168},
{1226.3138,1601.5934,6.3585},
{1224.6870,1334.1022,6.3585},
{1224.9535,1093.8094,6.4366},
{1373.5900,859.4901,6.4366},
{1480.3903,840.0465,6.4331},
{1657.9940,834.3144,6.3807},
{1900.6215,833.3372,7.5685},
{2064.5989,833.2748,6.3585},
{2181.0193,810.4846,6.3679},
{2295.9771,765.2249,10.3628},
{2378.4038,807.1584,7.1340},
{2486.1809,838.0077,6.3575},
{2652.8877,948.6396,6.3531},
{2706.2939,1125.8922,6.3586},
{2706.1467,1353.7439,6.3585},
{2709.1023,1671.8213,6.3623},
{2708.8352,2020.2767,6.3582},
{2723.1846,2165.8037,6.3683},
{2723.8447,2329.2424,6.3586},
{2668.4348,2447.2083,6.3586},
{2549.8311,2564.8225,4.5320},
{2443.1423,2605.3872,5.6154},
{2164.1670,2584.8892,6.3962},
{1894.5842,2507.6167,6.4288},
{1637.9525,2457.5022,6.4601},
{1471.5781,2454.2603,6.3585},
{1307.0748,2413.2354,6.3597},
{1227.7690,2095.6714,6.3650},
{1226.8463,1826.5822,6.3573}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_freeway_init();

public race_freeway_init() // unique named init function (must be ran on gamemode init)
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

