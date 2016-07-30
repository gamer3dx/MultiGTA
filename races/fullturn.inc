//
// Converted:   26.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_fullturn_included
  #endinput
#endif

#define _race_fullturn_included
#pragma library race_fullturn
#include "race"

static racename[MAX_NAME] = "FullTurn";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{2907.1707,2406.6133,10.4005},
{2848.6821,2234.1079,10.2948},
{2847.8733,2070.4319,10.2974},
{2828.0847,1975.5433,10.2960},
{2672.8003,1951.6693,11.3938},
{2489.7747,1969.3590,10.2975},
{2357.3093,1957.5857,10.3005},
{2297.6909,1893.1490,10.2949},
{2154.1106,1904.3524,10.2950},
{2148.8367,2057.1125,10.3039},
{2148.2568,2188.1633,10.2961},
{2191.7668,2327.8037,10.2952},
{2228.0015,2477.6013,10.3034},
{2251.4468,2513.2791,10.2949},
{2394.4265,2513.3110,10.3540},
{2474.9592,2513.0181,10.4309},
{2528.3340,2488.9541,10.4441},
{2527.4434,2378.1226,10.2993},
{2541.6848,2232.6526,10.2953},
{2641.8872,2290.1973,10.2947},
{2797.1089,2294.5388,10.3165},
{2902.7891,2299.5833,10.2947},
{2906.9424,2403.9465,10.3297}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_fullturn_init();

public race_fullturn_init() // unique named init function (must be ran on gamemode init)
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

