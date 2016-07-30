//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_bouncer_included
  #endinput
#endif

#define _race_bouncer_included
#pragma library race_bouncer
#include "race"

static racename[MAX_NAME] = "Bouncer";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
	{834.3040,-2021.4475,12.3261},
	{857.2571,-1782.0365,13.2445},
	{917.2284,-1710.6387,12.8643},
	{935.2226,-1574.8379,12.8622},
	{958.9678,-1448.3715,12.9586},
	{996.8513,-1369.6133,12.7840},
	{973.8067,-1286.3115,13.0323},
	{993.0671,-1246.9014,18.815},
	{1042.0275,-1220.2257,16.2051},
	{1149.3699,-1168.5941,31.5102},
	{1132.9978,-1197.7538,31.5086}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_bouncer_init();

public race_bouncer_init() // unique named init function (must be ran on gamemode init)
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

