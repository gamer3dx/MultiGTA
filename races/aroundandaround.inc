//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ
// Converter: 	MadCat
//

#if defined _race_aroundandaround_included
  #endinput
#endif

#define _race_aroundandaround_included
#pragma library race_aroundandaround
#include "race"

static racename[MAX_NAME] = "Around And Around";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 359.7591;
static Float:racecheckpoints[][CoordInfo] = {
{1258.2293,-1385.2028,12.7247},
{1236.3317,-1280.9645,12.9619},
{1200.6877,-1309.8824,12.9590},
{1223.8494,-1400.0991,12.7307},
{1258.1188,-1367.5178,12.6515},
{1231.5783,-1279.9818,12.9510},
{1199.9631,-1315.8101,12.9642},
{1228.5935,-1401.0917,12.6883},
{1258.4353,-1353.2585,12.5637},
{1231.8826,-1280.9089,12.9580},
{1064.3187,-1280.6472,13.1477}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_aroundandaround_init();

public race_aroundandaround_init() // unique named init function (must be ran on gamemode init)
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

