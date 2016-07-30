//
// Created:     13.October.2006
// Aurthor:    |Insane|
// Also Thanks to Iain for a little help about how to make it
// Thanks Again
// Changes:    MadCat

#if defined _race_striptease_included
  #endinput
#endif

#define _race_striptease_included
#pragma library race_striptease
#include "race"

static racename[MAX_NAME] = "Strip Tease";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 133.1593;
static Float:racecheckpoints[][CoordInfo] = {
{-1103.2333,388.6740,14.1484},
{-1155.2865,337.2700,14.2500},
{-1235.9453,242.7267,14.1484},
{-1324.7435,166.7910,14.1409},
{-1411.2069,82.9837,14.1484},
{-1496.8667,-5.6173,14.1484},
{-1575.0631,-82.6446,14.1484},
{-1652.7694,-171.0437,14.1484}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_striptease_init();

public race_striptease_init() // unique named init function (must be ran on gamemode init)
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

