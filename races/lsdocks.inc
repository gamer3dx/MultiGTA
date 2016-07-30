//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ
// Converter: 	MadCat
//

#if defined _race_lsdocks_included
  #endinput
#endif

#define _race_lsdocks_included
#pragma library race_lsdocks
#include "race"

static racename[MAX_NAME] = "LS Docks";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 89.6867;
static Float:racecheckpoints[][CoordInfo] = {
{2812.6997,-2049.0087,10.5193},
{2488.8173,-2048.8669,24.3599},
{2267.8229,-2096.6342,13.2092},
{2229.8398,-2196.9135,12.9320},
{2287.3793,-2278.0437,13.1213},
{2293.4130,-2368.9174,12.9795},
{2224.0400,-2492.0102,12.9342},
{2316.8427,-2664.2077,13.0782},
{2483.4487,-2583.7797,13.0842},
{2606.9897,-2504.8190,13.0575},
{2760.9416,-2456.7226,13.1267}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lsdocks_init();

public race_lsdocks_init() // unique named init function (must be ran on gamemode init)
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

