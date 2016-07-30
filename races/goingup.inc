//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_goingup_included
  #endinput
#endif

#define _race_goingup_included
#pragma library race_goingup
#include "race"

forward race_goingup_init();

static racename[MAX_NAME] = "Going Up";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 293.1748;
static Float:racecheckpoints[][CoordInfo] = {
	{-2390.3538,-2203.0872,33.2891},
	{-2132.3562,-1917.4426,116.5317},
	{-2344.9683,-2095.3113,114.5274},
	{-2627.4224,-1835.1287,218.7748},
	{-2527.4897,-1855.2715,306.5247},
	{-2586.2493,-1681.8401,352.7166},
	{-2435.1367,-1804.9152,411.1393},
	{-2324.2275,-1623.8970,484.0653}
};

static RaceID; // this will hold the id this race is assigned at init


public race_goingup_init() // unique named init function (must be ran on gamemode init)
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
