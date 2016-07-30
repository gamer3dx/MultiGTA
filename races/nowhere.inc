//
// Converted:   22.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_nowhere_included
  #endinput
#endif

#define _race_nowhere_included
#pragma library race_nowhere
#include "race"

static racename[MAX_NAME] = "No Where";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 172.4679;
static Float:racecheckpoints[][CoordInfo] = {
{1042.7639,-2019.8535,12.5588},
{1104.2753,-2375.0413,11.4097},
{1336.3232,-2600.8538,12.9998},
{2128.7466,-2662.5674,12.9999},
{2128.6660,-2326.4580,12.9813},
{1903.9204,-2166.6145,13.0077},
{1529.8291,-1986.6089,23.4409},
{1266.4916,-1852.2683,13.0077},
{1042.7639,-2019.8535,12.5588},
{1104.2753,-2375.0413,11.4097},
{1336.3232,-2600.8538,12.9998},
{2128.7466,-2662.5674,12.9999},
{2128.6660,-2326.4580,12.9813},
{1903.9204,-2166.6145,13.0077},
{1529.8291,-1986.6089,23.4409},
{1266.4916,-1852.2683,13.0077},
{1042.7639,-2019.8535,12.5588}
};


static RaceID;

forward race_nowhere_init();

public race_nowhere_init()
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

