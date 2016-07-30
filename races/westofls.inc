//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ (original race name: Around The West)
// Converter: 	MadCat
//

#if defined _race_westofls_included
  #endinput
#endif

#define _race_westofls_included
#pragma library race_westofls
#include "race"

static racename[MAX_NAME] = "West Of Los-Santos";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 183.3742;
static Float:racecheckpoints[][CoordInfo] = {
{794.5209,-1057.3442,24.2271},
{749.5024,-1586.1552,13.4167},
{330.3264,-1609.5281,32.7136},
{649.4843,-1672.0362,14.1892},
{857.6870,-1593.0963,12.9558},
{1038.0872,-1550.1126,12.9167},
{1033.8394,-1323.7639,12.9599},
{617.4507,-1320.5148,13.4054},
{405.2371,-1428.3184,32.5730},
{628.9672,-1399.0640,12.8666},
{1349.1621,-1402.5863,12.8790}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_westofls_init();

public race_westofls_init() // unique named init function (must be ran on gamemode init)
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

