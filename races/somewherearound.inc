//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ
// Converter: 	MadCat
//

#if defined _race_somewherearound_included
  #endinput
#endif

#define _race_somewherearound_included
#pragma library race_somewherearound
#include "race"

static racename[MAX_NAME] = "Somewhere Around";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 270.4818;
static Float:racecheckpoints[][CoordInfo] = {
{367.9506,-1775.7265,4.8983},
{1120.0612,-1850.3714,12.9491},
{1530.7902,-1937.0718,17.1071},
{2180.8737,-2183.0937,12.9506},
{2303.9724,-2063.4960,12.9124},
{2820.4301,-2009.1661,10.5043},
{2848.7470,-1217.9310,22.8447},
{2799.3962,-442.1192,10.3761},
{2867.2519,-101.1756,22.2319},
{2728.4013,246.3749,32.5805},
{1663.9531,297.0053,20.6606}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_somewherearound_init();

public race_somewherearound_init() // unique named init function (must be ran on gamemode init)
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

